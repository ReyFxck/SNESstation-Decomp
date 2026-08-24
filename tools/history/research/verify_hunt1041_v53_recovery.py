#!/usr/bin/env python3
"""Verify the frozen target-side V53 recovery spans without compiler objects.

This historical checker predates the completed V72 compiler-side proof. For
formal reproduction use ``make hunt1041-v72-evidence``.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
MANIFEST = ROOT / "analysis" / "matching" / "hunt1041-v53-recovered-target-spans.tsv"
DEFAULT_REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    args = parser.parse_args()

    reference = args.reference.expanduser().resolve()
    if not reference.is_file():
        raise SystemExit(f"missing unpacked reference: {reference}; run make reference")
    target = reference.read_bytes()
    actual = sha256(target)
    if actual != TARGET_SHA256:
        raise SystemExit(f"reference SHA-256 mismatch: {actual}")

    with MANIFEST.open(encoding="utf-8", newline="") as handle:
        rows = list(csv.DictReader(handle, delimiter="\t"))
    if len(rows) != 6:
        raise SystemExit(f"recovery cardinality mismatch: expected 6, got {len(rows)}")

    for row in rows:
        address = int(row["address"], 16)
        size = int(row["bytes"])
        offset = address - TARGET_BASE
        if offset < 0 or offset + size > len(target):
            raise SystemExit(f"{row['address']}: target span out of range")
        digest = sha256(target[offset:offset + size])
        if digest != row["target_span_sha256"]:
            raise SystemExit(
                f"{row['address']} {row['recovered_identity']}: "
                f"span SHA-256 mismatch: {digest}"
            )
        print(f"{row['address']} {row['recovered_identity']}: OK ({size} bytes)")

    print(f"V53 recovered target spans: OK ({len(rows)}/6)")
    print(f"formal target SHA-256: {TARGET_SHA256}")
    print("compiler-side proof and promotion: HUNT1041 V72")


if __name__ == "__main__":
    main()
