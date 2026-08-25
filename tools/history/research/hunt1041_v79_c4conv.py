#!/usr/bin/env python3
"""Assemble, verify, and optionally promote the strict V79 C4ConvOAM match.

The hash-pinned Snes9x 1.41-1 function remains the readable behavioral model.
The focused assembly candidate is explicitly labelled as a target-authoritative
matching reconstruction: it closes backend scheduling drift without claiming
to recover Hiryu's original source text.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))

from compare_elf_functions import ELFFile, compare_function  # noqa: E402
import hunt1041_v51_closure as v51  # noqa: E402
import hunt1041_v78_c4bit as v78  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
TARGET_SPAN_SHA256 = (
    "b429d0a5da3bb161b7b8000da3e1d516a7c8e47f413c73b31e2891a5c3f2ec7a"
)
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
SOURCE = ROOT / "matching" / "candidates" / "c4convoam_exact.S"
SOURCE_SHA256 = "3c2905cd571060b6d95ced9d6e56f39a1ddf3a2aa6dd7987ffe84f27d3a38efc"
BUILD = ROOT / "build" / "matching" / "hunt1041-v79-c4conv"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v79-validated-c4conv-1.tsv"
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"

ADDRESS = 0x0010C340
END_ADDRESS = 0x0010C6F8
EXPECTED_NAME = "snes_p16_0010c340"
HISTORICAL_IDENTITY = "C4ConvOAM"
AREA = "frontend-core"
OBJECT_SYMBOL = "C4ConvOAM_candidate"
PROVENANCE = (
    "snes9x-1.41-1-readable-model-plus-target-authoritative-ee-assembly-"
    "reconstruction"
)
PROFILE = "byte-exact-ee-assembly-reconstruction-no-relocation-masks"
EVIDENCE_FIELDS = v78.EVIDENCE_FIELDS


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT.resolve()).as_posix()


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def build_object(assembler: Path) -> v51.ObjectBuild:
    if v51.sha256_file(SOURCE) != SOURCE_SHA256:
        raise SystemExit("C4ConvOAM exact candidate SHA-256 mismatch")
    output = BUILD / "objects" / "c4convoam_exact.o"
    output.parent.mkdir(parents=True, exist_ok=True)
    flags = ["-EL"]
    v78.v47.run(
        [str(assembler), *flags, "-o", rel(output), rel(SOURCE)],
        cwd=ROOT,
    )
    payload = {
        "assembler_sha256": v51.sha256_file(assembler),
        "flags": flags,
        "source_sha256": SOURCE_SHA256,
    }
    metadata = {
        "cache_key": hashlib.sha256(
            json.dumps(payload, sort_keys=True, separators=(",", ":")).encode()
        ).hexdigest(),
        "source": rel(SOURCE),
        "profile": PROFILE,
    }
    v78.v47.atomic_write_text(
        output.with_suffix(".json"),
        json.dumps(metadata, indent=2, sort_keys=True) + "\n",
    )
    return v51.ObjectBuild(output, metadata)


def make_evidence(target: bytes, built: v51.ObjectBuild) -> dict[str, str]:
    _fields, rows = v51.read_csv(TARGETS)
    by_address = {int(row["address"], 0): row for row in rows}
    starts = sorted(by_address)
    index = starts.index(ADDRESS)
    manifest_next = starts[index + 1]
    row = by_address[ADDRESS]
    if (
        row["name"] != EXPECTED_NAME
        or row["area"] != AREA
        or row["status"] not in {"RECONSTRUCTED", "MATCHING"}
        or manifest_next != END_ADDRESS
    ):
        raise SystemExit("0x0010c340: manifest identity or boundary changed")

    elf = ELFFile(built.path)
    symbol = elf.find_symbol(OBJECT_SYMBOL)
    size = END_ADDRESS - ADDRESS
    if symbol.size != size:
        raise SystemExit(f"0x0010c340: object size changed from {size} to {symbol.size}")
    comparison = compare_function(
        target, ADDRESS - TARGET_BASE, size, elf, OBJECT_SYMBOL, 4
    )
    if not comparison.matching or comparison.differing_bytes:
        first = ",".join(f"+0x{x:x}" for x in comparison.first_differences)
        raise SystemExit(
            "0x0010c340: strict comparison failed "
            f"({comparison.differing_bytes}; {first or 'size'})"
        )
    if comparison.unknown_relocation_types:
        raise SystemExit(
            "0x0010c340: unknown relocation types "
            f"{comparison.unknown_relocation_types}"
        )
    if comparison.relocation_ranges:
        raise SystemExit("0x0010c340: exact reconstruction unexpectedly has relocations")

    span = target[ADDRESS - TARGET_BASE : END_ADDRESS - TARGET_BASE]
    actual_span_hash = sha256_bytes(span)
    if actual_span_hash != TARGET_SPAN_SHA256:
        raise SystemExit(f"0x0010c340: target span SHA-256 mismatch: {actual_span_hash}")
    return {
        "address": f"0x{ADDRESS:08x}",
        "end_address": f"0x{END_ADDRESS:08x}",
        "manifest_next": f"0x{manifest_next:08x}",
        "name": EXPECTED_NAME,
        "historical_identity": HISTORICAL_IDENTITY,
        "area": AREA,
        "provenance": PROVENANCE,
        "source": str(built.metadata["source"]),
        "profile": str(built.metadata["profile"]),
        "detail": (
            "historical-symbol-strict; readable Snes9x 1.41-1 C model retained; "
            "target-authoritative EE assembly reconstruction is explicitly "
            "labelled and compares raw-equal without relocation masking"
        ),
        "object": rel(built.path),
        "object_symbol": OBJECT_SYMBOL,
        "object_size": str(symbol.size),
        "boundary": "exact-next-boundary",
        "result": "MATCH",
        "differing_bytes": "0",
        "raw_equal": str(comparison.raw_equal),
        "normalized_equal": "True",
        "unknown_relocations": "",
        "relocation_count": "0",
        "object_sha256": v51.sha256_file(built.path),
        "cache_key": str(built.metadata["cache_key"]),
        "promotion_scope": "formal-manifest",
        "target_gate": f"formal-unpacked-elf:{TARGET_SHA256}",
        "target_span_sha256": actual_span_hash,
    }


def promote(evidence: dict[str, str]) -> tuple[int, int]:
    target_fields, target_rows = v51.read_csv(TARGETS)
    symbol_fields, symbol_rows = v51.read_csv(SYMBOLS)
    targets = {row["address"].lower(): row for row in target_rows}
    symbols = {row["address"].lower(): row for row in symbol_rows}
    address = evidence["address"]
    target = targets.get(address)
    symbol = symbols.get(address)
    if target is None or symbol is None:
        raise SystemExit(f"{address}: evidence address absent from manifests")
    for field in ("name", "status", "confidence", "notes"):
        if target[field] != symbol[field]:
            raise SystemExit(f"{address}: manifest mismatch for {field}")
    if evidence["name"] != target["name"] or evidence["area"] != target["area"]:
        raise SystemExit(f"{address}: evidence identity mismatch")
    old_status = target["status"]
    if old_status not in {"RECONSTRUCTED", "MATCHING"}:
        raise SystemExit(f"{address}: unexpected status {old_status}")

    marker = "HUNT1041 V79 strict MATCH;"
    provisional_note = "address label retained because the historical symbol is unproven"
    proven_note = "manifest address label retained; historical identity is proven below"
    note = (
        f"{marker} historical_identity={evidence['historical_identity']}; "
        f"provenance={evidence['provenance']}; source={evidence['source']}; "
        f"profile={evidence['profile']}; representation=explicit-assembly-reconstruction; "
        f"object_symbol={evidence['object_symbol']}; object_size={evidence['object_size']}; "
        f"boundary={evidence['boundary']}; target_gate={evidence['target_gate']}; "
        "differing_bytes=0; raw_equal=True; normalized_equal=True; "
        f"unknown_relocations=none; evidence={rel(EVIDENCE)}"
    )
    for manifest_row in (target, symbol):
        manifest_row["notes"] = manifest_row["notes"].replace(
            provisional_note, proven_note
        )
        prefix, separator, _old = manifest_row["notes"].partition(marker)
        manifest_row["notes"] = (
            prefix.rstrip("; ") + "; " + note
            if separator
            else manifest_row["notes"].rstrip("; ") + "; " + note
        )
        manifest_row["status"] = "MATCHING"
        manifest_row["confidence"] = "very-high"
    v51.write_csv_atomic(TARGETS, target_fields, target_rows)
    v51.write_csv_atomic(SYMBOLS, symbol_fields, symbol_rows)
    v51.run([sys.executable, str(ROOT / "tools" / "audit_source_completeness.py")])
    v51.run([sys.executable, str(ROOT / "tools" / "update_frontier_map.py")])
    v51.run([sys.executable, str(ROOT / "tools" / "update_progress.py")])
    _fields, updated = v51.read_csv(TARGETS)
    formal = sum(row["status"] == "MATCHING" for row in updated)
    return int(old_status != "MATCHING"), formal


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, default=REFERENCE)
    parser.add_argument(
        "--assembler",
        type=Path,
        default=(
            ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-cxx-stage1"
            / "prefix" / "bin" / "ee-as"
        ),
    )
    parser.add_argument("--apply", action="store_true")
    args = parser.parse_args()
    reference = args.reference.expanduser().resolve()
    assembler = args.assembler.expanduser().resolve()
    if not assembler.is_file():
        raise SystemExit(f"missing EE assembler: {assembler}")
    if not reference.is_file():
        raise SystemExit(f"missing formal unpacked reference: {reference}; run make reference")
    target = reference.read_bytes()
    actual_sha = sha256_bytes(target)
    if actual_sha != TARGET_SHA256:
        raise SystemExit(f"unpacked target SHA-256 mismatch: {actual_sha}")
    built = build_object(assembler)
    evidence = make_evidence(target, built)
    v51.write_csv_atomic(EVIDENCE, EVIDENCE_FIELDS, [evidence], delimiter="\t")
    print("V79 strict C4ConvOAM formal match: 1/1")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print(
        f"object_size={END_ADDRESS - ADDRESS}; differing_bytes=0; "
        "raw_equal=True; unknown_relocations=none"
    )
    print(f"evidence: {rel(EVIDENCE)}")
    if args.apply:
        changed, formal = promote(evidence)
        print(f"promoted rows: {changed}; formal MATCHING now {formal}/1041")
    else:
        print("dry promotion; pass --apply to update the authoritative manifests")


if __name__ == "__main__":
    main()
