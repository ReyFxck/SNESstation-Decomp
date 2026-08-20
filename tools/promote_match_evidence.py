#!/usr/bin/env python3
"""Validate strict miner evidence and promote the same rows in both manifests."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import re
import subprocess
import sys
from pathlib import Path

from compare_elf_functions import ELFFile, compare_function
from run_match_miner import has_terminal_control_flow


ROOT = Path(__file__).resolve().parents[1]
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
EXPECTED_REFERENCE_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
REFERENCE_BASE = 0x00100000
ZERO_GAP_RE = re.compile(r"^historical-symbol\+target-zero-gap:0x([0-9a-f]+)$")
ALLOWED_BOUNDARIES = {"exact-next-boundary", "terminal-control-flow-boundary"}


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def resolve_repo_path(value: str) -> Path:
    path = (ROOT / value).resolve() if not Path(value).is_absolute() else Path(value).resolve()
    try:
        path.relative_to(ROOT)
    except ValueError as exc:
        raise ValueError(f"path is outside the repository: {value}") from exc
    return path


def read_csv(path: Path, delimiter: str = ",") -> tuple[list[str], list[dict[str, str]]]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter=delimiter)
        rows = list(reader)
        fields = list(reader.fieldnames or ())
    return fields, rows


def rows_by_address(rows: list[dict[str, str]], label: str) -> dict[str, dict[str, str]]:
    result: dict[str, dict[str, str]] = {}
    for row in rows:
        address = row["address"].lower()
        if address in result:
            raise ValueError(f"duplicate {label} address: {address}")
        result[address] = row
    return result


def validate_boundary(value: str) -> None:
    if value in ALLOWED_BOUNDARIES:
        return
    match = ZERO_GAP_RE.fullmatch(value)
    if not match:
        raise ValueError(f"unsupported boundary proof: {value}")
    gap = int(match.group(1), 16)
    if gap <= 0 or gap > 64 or gap % 4:
        raise ValueError(f"invalid zero-gap boundary proof: {value}")


def validate_evidence_row(
    row: dict[str, str],
    evidence_path: Path,
    reference: bytes,
    next_address: int,
) -> None:
    """Recompute a miner row from the candidate object and target bytes."""
    address = row.get("address", "")
    if not re.fullmatch(r"0x[0-9a-fA-F]{8}", address):
        raise ValueError(f"invalid evidence address: {address!r}")
    address_value = int(address, 0)
    if address_value < REFERENCE_BASE or next_address <= address_value:
        raise ValueError(f"{address}: invalid reference span")
    if row.get("result") != "MATCH":
        raise ValueError(f"{address}: result is not MATCH")
    if row.get("differing_bytes") != "0":
        raise ValueError(f"{address}: differing_bytes is not zero")
    if row.get("normalized_equal") != "True":
        raise ValueError(f"{address}: normalized_equal is not True")
    if row.get("unknown_relocations", "").strip():
        raise ValueError(f"{address}: unknown relocations are present")
    validate_boundary(row.get("boundary", ""))

    source = resolve_repo_path(row.get("source", ""))
    object_path = resolve_repo_path(row.get("object", ""))
    if not source.is_file():
        raise ValueError(f"{address}: missing source: {source}")
    if not object_path.is_file():
        raise ValueError(f"{address}: missing candidate object: {object_path}")
    expected_object_sha = row.get("object_sha256", "")
    if not re.fullmatch(r"[0-9a-f]{64}", expected_object_sha):
        raise ValueError(f"{address}: invalid object SHA-256")
    actual_object_sha = sha256_file(object_path)
    if actual_object_sha != expected_object_sha:
        raise ValueError(
            f"{address}: candidate object SHA-256 mismatch in {evidence_path.name}"
        )

    cache_key = row.get("cache_key", "")
    if not re.fullmatch(r"[0-9a-f]{64}", cache_key):
        raise ValueError(f"{address}: invalid cache key")
    metadata_path = object_path.with_suffix(".json")
    try:
        metadata = json.loads(metadata_path.read_text(encoding="utf-8"))
    except (OSError, ValueError) as exc:
        raise ValueError(f"{address}: missing or invalid candidate metadata") from exc
    expected_metadata = {
        "cache_key": cache_key,
        "source": row.get("source", ""),
        "profile": row.get("profile", ""),
    }
    for key, expected in expected_metadata.items():
        if metadata.get(key) != expected:
            raise ValueError(f"{address}: candidate metadata mismatch for {key}")

    try:
        object_size = int(row.get("object_size", ""), 0)
    except ValueError as exc:
        raise ValueError(f"{address}: invalid object size") from exc
    if object_size <= 0 or object_size % 4:
        raise ValueError(f"{address}: invalid aligned object size")
    span = next_address - address_value
    boundary = row["boundary"]
    if boundary == "exact-next-boundary":
        if object_size != span:
            raise ValueError(f"{address}: exact boundary does not reach next target")
    elif boundary == "terminal-control-flow-boundary":
        if object_size >= span or object_size > 512:
            raise ValueError(f"{address}: invalid terminal-prefix boundary size")
    else:
        match = ZERO_GAP_RE.fullmatch(boundary)
        assert match is not None
        gap = int(match.group(1), 16)
        if object_size + gap != span:
            raise ValueError(f"{address}: zero-gap proof does not reach next target")
        gap_start = address_value - REFERENCE_BASE + object_size
        if reference[gap_start : gap_start + gap] != b"\0" * gap:
            raise ValueError(f"{address}: claimed target gap is not all zero")

    try:
        elf = ELFFile(object_path)
        symbol = elf.find_symbol(row.get("object_symbol", ""))
        candidate = elf.symbol_bytes(symbol, object_size)
        comparison = compare_function(
            reference,
            address_value - REFERENCE_BASE,
            object_size,
            elf,
            symbol.name,
        )
    except (OSError, ValueError) as exc:
        raise ValueError(f"{address}: unable to recompare candidate object") from exc
    if symbol.size != object_size or len(candidate) != object_size:
        raise ValueError(f"{address}: object symbol size changed")
    if boundary == "terminal-control-flow-boundary" and not has_terminal_control_flow(
        candidate, elf.endian
    ):
        raise ValueError(f"{address}: candidate has no proven terminal control flow")
    if not comparison.matching or comparison.differing_bytes != 0:
        raise ValueError(f"{address}: independent object comparison is not a match")
    if comparison.unknown_relocation_types:
        raise ValueError(f"{address}: independent comparison found unknown relocations")
    if row.get("raw_equal") != str(comparison.raw_equal):
        raise ValueError(f"{address}: raw comparison field disagrees with object")
    if row.get("normalized_equal") != str(comparison.normalized_equal):
        raise ValueError(f"{address}: normalized comparison field disagrees with object")


def write_csv_atomic(path: Path, fields: list[str], rows: list[dict[str, str]]) -> None:
    temporary = path.with_name(path.name + ".tmp")
    with temporary.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    os.replace(temporary, path)


def evidence_note(label: str, evidence_rel: str, row: dict[str, str]) -> str:
    mode = row["detail"].split(";", 1)[0]
    return (
        f"{label} strict MATCH; mode={mode}; provenance={row['provenance']}; "
        f"source={row['source']}; profile={row['profile']}; "
        f"boundary={row['boundary']}; differing_bytes=0; "
        f"raw_equal={row['raw_equal']}; normalized_equal=True; "
        f"unknown_relocations=none; evidence={evidence_rel}"
    )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("evidence", type=Path)
    parser.add_argument("--label", required=True)
    parser.add_argument("--apply", action="store_true")
    args = parser.parse_args()

    evidence_path = args.evidence.expanduser().resolve()
    try:
        evidence_rel = evidence_path.relative_to(ROOT).as_posix()
    except ValueError as exc:
        raise SystemExit("evidence must be inside the repository") from exc
    if not evidence_path.is_file():
        raise SystemExit(f"missing evidence: {evidence_path}")
    if not REFERENCE.is_file() or sha256_file(REFERENCE) != EXPECTED_REFERENCE_SHA256:
        raise SystemExit("verified unpacked reference is unavailable; run make reference")

    evidence_fields, evidence_rows = read_csv(evidence_path, delimiter="\t")
    required = {
        "address",
        "name",
        "area",
        "provenance",
        "source",
        "profile",
        "detail",
        "object",
        "object_symbol",
        "object_size",
        "boundary",
        "result",
        "differing_bytes",
        "raw_equal",
        "normalized_equal",
        "unknown_relocations",
        "object_sha256",
        "cache_key",
    }
    if not evidence_rows or not required <= set(evidence_fields):
        raise SystemExit(f"evidence must contain {sorted(required)}")
    evidence_by_address = rows_by_address(evidence_rows, "evidence")

    target_fields, target_rows = read_csv(TARGETS)
    symbol_fields, symbol_rows = read_csv(SYMBOLS)
    targets = rows_by_address(target_rows, "target")
    symbols = rows_by_address(symbol_rows, "symbol")
    if set(targets) != set(symbols):
        raise SystemExit("target and symbol manifests have different address sets")
    ordered_addresses = sorted(int(address, 0) for address in targets)
    next_by_address = {
        start: end for start, end in zip(ordered_addresses, ordered_addresses[1:])
    }
    reference = REFERENCE.read_bytes()
    try:
        for row in evidence_rows:
            address_value = int(row["address"], 0)
            next_address = next_by_address.get(address_value)
            if next_address is None:
                raise ValueError(f"{row['address']}: no audited next-address boundary")
            validate_evidence_row(row, evidence_path, reference, next_address)
    except (KeyError, ValueError) as exc:
        raise SystemExit(f"evidence validation failed: {exc}") from exc

    promotable: list[str] = []
    for address, evidence in evidence_by_address.items():
        target = targets.get(address)
        symbol = symbols.get(address)
        if target is None or symbol is None:
            raise SystemExit(f"evidence address is absent from manifests: {address}")
        for field in ("name", "status", "confidence", "notes"):
            if target[field] != symbol[field]:
                raise SystemExit(f"manifest mismatch at {address}: {field}")
        if evidence["name"] != target["name"] or evidence["area"] != target["area"]:
            raise SystemExit(f"evidence identity mismatch at {address}")
        if target["status"] == "MATCHING":
            continue
        if target["status"] != "RECONSTRUCTED":
            raise SystemExit(f"unexpected current status at {address}: {target['status']}")
        promotable.append(address)

    print(
        f"validated evidence: rows={len(evidence_rows)} "
        f"promotable={len(promotable)} already_matching={len(evidence_rows) - len(promotable)}"
    )
    if not args.apply:
        print("dry run only; pass --apply to update manifests")
        return

    for address in promotable:
        evidence = evidence_by_address[address]
        note = evidence_note(args.label, evidence_rel, evidence)
        for manifest_row in (targets[address], symbols[address]):
            manifest_row["status"] = "MATCHING"
            manifest_row["confidence"] = "very-high"
            manifest_row["notes"] = manifest_row["notes"].rstrip("; ") + "; " + note

    write_csv_atomic(TARGETS, target_fields, target_rows)
    write_csv_atomic(SYMBOLS, symbol_fields, symbol_rows)
    subprocess.run(
        [sys.executable, str(ROOT / "tools" / "audit_source_completeness.py")],
        cwd=ROOT,
        check=True,
    )
    subprocess.run(
        [sys.executable, str(ROOT / "tools" / "update_progress.py")],
        cwd=ROOT,
        check=True,
    )
    print(f"promoted strict matches: {len(promotable)}")


if __name__ == "__main__":
    main()
