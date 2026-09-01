#!/usr/bin/env python3
"""Prove the V92 snprintf source-contract refactor, not an archive identity.

Four SHA-frozen target spans have direct JALs to sprintf at 0x0019e3d0.
The source model uses the existing sprintf provider and emits no snprintf
import or compatibility runtime shim.  Hashes/addresses only are public;
the private verifier never publishes reference bytes.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import struct
from collections import Counter
from pathlib import Path
from typing import Sequence

import libgcc_contracts as libgcc

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/runtime_refactors.tsv"
DEFAULT_PROGRESS = ROOT / "analysis/progress_targets.csv"
DEFAULT_REPORT = ROOT / "build/runtime-refactors/report.json"
FIELDS = (
    "source_symbol", "source", "former_contract", "replacement_contract",
    "status", "target_address", "extent_hex", "call_address", "callee_address",
    "target_sha256", "matching_evidence", "matching_evidence_sha256",
)
STATUS = "SOURCE_REFACTOR_CLOSED"
CALLEE = 0x0019E3D0
SPECS = (
    ("snes_dispatch_00105718", "src/ps2/small_dispatch_recovered.c",
     0x00105718, 56, 0x00105734,
     "61dc139326ba0e177e11399911258eb0ee2ec5d27cb5f2e82cd1f59ffffeb4f4",
     "analysis/matching/hunt1000plus-v47-validated-79.tsv",
     "fce375f4f3738af37cd57537ff374d0a7dbe0ca85559ecca8691858bce0bc623"),
    ("CMemory_StaticRAMSize_00156934", "src/snes9x/memmap_metadata_recovered.c",
     0x00156934, 96, 0x00156984,
     "de785690ef87f48cb2fe6c7e65ea7b5510bf55089a2c41f3aab412e2869e6a6c",
     "analysis/matching/hunt1000plus-v34-validated-16.tsv",
     "3e824f63d90666b021b356df579c5d3404e23acffa3e959ea863354f4219d1f3"),
    ("CMemory_Size_00156994", "src/snes9x/memmap_metadata_recovered.c",
     0x00156994, 112, 0x001569F4,
     "b107512268b6f08d3277ce677906e8a7ceca65c6b406d47228fb4d0b39b5fc69",
     "analysis/matching/hunt1000plus-v34-validated-16.tsv",
     "3e824f63d90666b021b356df579c5d3404e23acffa3e959ea863354f4219d1f3"),
    ("CMemory_MapMode_00156c0c", "src/snes9x/memmap_metadata_recovered.c",
     0x00156C0C, 72, 0x00156C38,
     "50b6ada1d86464829d785df73347a758cc23d6ef88de4819d440960e5d7ed095",
     "analysis/matching/hunt1000plus-v46-validated-42.tsv",
     "c13a582a9b6bc100256670dbc0d2b2bda6f10293f4359e3b2fd912dcf8c2f5db"),
)


class RuntimeRefactorError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise RuntimeRefactorError(message)


def expected_rows() -> list[dict[str, str]]:
    return [dict(zip(FIELDS, (
        symbol, source, "snprintf", "sprintf", STATUS, f"0x{address:08x}",
        f"0x{size:x}", f"0x{call:08x}", f"0x{CALLEE:08x}", digest, evidence, proof_hash,
    ))) for symbol, source, address, size, call, digest, evidence, proof_hash in SPECS]


def function_body(text: str, symbol: str) -> str:
    match = re.search(r"\b" + re.escape(symbol) + r"\s*\([^;]*?\)\s*\{", text)
    if not match:
        fail(f"missing source definition: {symbol}")
    start = match.end()
    depth = 1
    for pos in range(start, len(text)):
        depth += (text[pos] == "{") - (text[pos] == "}")
        if depth == 0:
            return text[start:pos]
    fail(f"unterminated source definition: {symbol}")


def validate_sources(rows: Sequence[dict[str, str]], root: Path = ROOT) -> None:
    for row in rows:
        text = (root / row["source"]).read_text(encoding="utf-8")
        body = function_body(text, row["source_symbol"])
        if re.search(r"\bsnprintf\s*\(", body) or not re.search(r"\bsprintf\s*\(", body):
            fail(f"formatter call drift: {row['source_symbol']}")


def validate_live_contracts(
    external: Sequence[dict[str, str]], contracts: Sequence[dict[str, str]],
    frontier: Sequence[dict[str, str]],
) -> None:
    for label, rows in (("externals", external), ("contracts", contracts), ("frontier", frontier)):
        if any(row["symbol"] == "snprintf" for row in rows):
            fail(f"snprintf returned to live {label}")
    if (len(external), len(contracts), len(frontier)) != (1892, 1569, 223):
        fail("post-refactor namespace count drift")
    expected_requesters = {
        "ps2/small_dispatch_recovered.o", "snes9x/memmap_metadata_recovered.o",
    }
    providers = [row for row in external if row["symbol"] == "sprintf"]
    if len(providers) != 1 or not expected_requesters <= set(providers[0]["requesters"].split(";")):
        fail("sprintf requester ownership drift")
    aliases = [row for row in contracts if row["symbol"] == "sprintf"]
    if len(aliases) != 1 or any(aliases[0][key] != value for key, value in {
        "status": "RESOLVED", "resolution_kind": "semantic-text-alias",
        "canonical_symbol": "sprintf_recovered", "target_address": "0x0019e3d0",
    }.items()):
        fail("sprintf canonical target drift")
    if any(row["resolution_kind"] == "compatibility-runtime-shim" for row in frontier):
        fail("compatibility runtime shim returned")
    runtime = Counter(row["category"] for row in external if row["provider_kind"] == "historical-archive")
    if runtime != {"ps2-runtime": 25, "c-runtime": 20, "compiler-runtime": 4}:
        fail(f"live Stage-3D partition drift: {dict(runtime)}")


def validate_manifest(args: argparse.Namespace) -> list[dict[str, str]]:
    rows = libgcc.read_table(args.manifest, FIELDS)
    if rows != expected_rows():
        fail("runtime-refactor ledger drift")
    with args.progress_manifest.open(encoding="utf-8", newline="") as stream:
        progress = {row["address"]: row for row in csv.DictReader(stream)}
    for row in rows:
        target = progress.get(row["target_address"], {})
        if target.get("status") != "MATCHING" or target.get("name") != row["source_symbol"]:
            fail(f"missing matching target: {row['source_symbol']}")
        proof = ROOT / row["matching_evidence"]
        if hashlib.sha256(proof.read_bytes()).hexdigest() != row["matching_evidence_sha256"]:
            fail(f"matching evidence hash drift: {proof}")
        with proof.open(encoding="utf-8", newline="") as stream:
            records = [item for item in csv.DictReader(stream, delimiter="\t")
                       if item["address"] == row["target_address"]]
        if (len(records) != 1 or records[0]["result"] != "MATCH"
                or records[0]["differing_bytes"] != "0"
                or records[0]["unknown_relocations"]):
            fail(f"matching evidence row drift: {row['source_symbol']}")
    target_formatter = progress.get("0x0019e3d0", {})
    if target_formatter.get("name") != "sprintf" or target_formatter.get("status") != "MATCHING":
        fail("sprintf target identity drift")
    validate_sources(rows)
    validate_live_contracts(
        libgcc.read_table(args.external_map, libgcc.EXTERNAL_FIELDS),
        libgcc.read_table(args.contracts, libgcc.CONTRACT_FIELDS),
        libgcc.read_table(args.frontier_manifest, libgcc.FRONTIER_FIELDS),
    )
    return rows


def verify_calls(raw: bytes, rows: Sequence[dict[str, str]]) -> list[dict[str, str]]:
    verified = []
    for row in rows:
        address, size = int(row["target_address"], 0), int(row["extent_hex"], 0)
        offset = address - libgcc.TARGET_BASE
        if offset < 0 or size <= 0 or size % 4 or offset + size > len(raw):
            fail(f"target range outside reference: {row['source_symbol']}")
        body = raw[offset:offset + size]
        if hashlib.sha256(body).hexdigest() != row["target_sha256"]:
            fail(f"target span hash drift: {row['source_symbol']}")
        calls = []
        for pos in range(0, size, 4):
            word = struct.unpack_from("<I", body, pos)[0]
            if word >> 26 == 3:
                target = ((address + pos + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
                calls.append((address + pos, target))
        expected = [(int(row["call_address"], 0), int(row["callee_address"], 0))]
        if calls != expected:
            fail(f"direct formatter call drift: {row['source_symbol']}: {calls}")
        verified.append({key: row[key] for key in (
            "source_symbol", "target_address", "call_address", "callee_address", "target_sha256",
        )})
    return verified


def verify_reference(path: Path, rows: Sequence[dict[str, str]]) -> list[dict[str, str]]:
    if not path.is_file():
        fail(f"missing private reference: {path}")
    raw = path.read_bytes()
    if len(raw) != libgcc.TARGET_SIZE or hashlib.sha256(raw).hexdigest() != libgcc.TARGET_SHA256:
        fail("private reference does not match the frozen unpacked target")
    return verify_calls(raw, rows)


def summary() -> str:
    return "contracts_closed=1 call_sites=4 runtime_shims=0 stage3d_closed=8/53 stage3d_open=45"


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "verify"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--progress-manifest", type=Path, default=DEFAULT_PROGRESS)
    parser.add_argument("--external-map", type=Path, default=libgcc.DEFAULT_EXTERNAL)
    parser.add_argument("--contracts", type=Path, default=libgcc.DEFAULT_CONTRACTS)
    parser.add_argument("--frontier-manifest", type=Path, default=libgcc.DEFAULT_FRONTIER)
    parser.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    parser.add_argument("--report", type=Path, default=DEFAULT_REPORT)
    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        rows = validate_manifest(args)
        if args.command == "verify":
            calls = verify_reference(args.reference, rows)
            report = {"reference_sha256": libgcc.TARGET_SHA256, "direct_calls": calls,
                      "scope": "source-contract refactor; not whole-function or final-link identity"}
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
            print(f"verified private runtime call sites: {len(calls)}/4 -> sprintf@0x0019e3d0")
        print(f"verified runtime refactors: {summary()}")
        return 0
    except (RuntimeRefactorError, libgcc.LibgccContractError, OSError, KeyError, ValueError) as error:
        print(f"runtime refactors: FAIL: {error}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
