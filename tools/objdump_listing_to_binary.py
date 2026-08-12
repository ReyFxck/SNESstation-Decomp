#!/usr/bin/env python3
"""Rebuild a bounded raw image from the byte columns in an objdump listing."""
from __future__ import annotations

import argparse
import re
from pathlib import Path


INSTRUCTION_RE = re.compile(
    r"^\s*([0-9a-fA-F]+):\s+"
    r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+"
    r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})(?:\s|$)"
)


def parse_address(value: str) -> int:
    return int(value, 0)


def rebuild_listing_range(text: str, base_address: int, end_address: int) -> tuple[bytes, int]:
    if base_address < 0 or end_address <= base_address:
        raise ValueError("end address must be greater than the non-negative base address")

    image = bytearray(end_address - base_address)
    written: dict[int, int] = {}
    instruction_count = 0

    for line_number, line in enumerate(text.splitlines(), start=1):
        match = INSTRUCTION_RE.match(line)
        if match is None:
            continue
        address = int(match.group(1), 16)
        raw = bytes(int(match.group(index), 16) for index in range(2, 6))
        if address < base_address or address + len(raw) > end_address:
            continue

        offset = address - base_address
        for index, value in enumerate(raw):
            absolute = address + index
            previous = written.get(absolute)
            if previous is not None and previous != value:
                raise ValueError(
                    f"line {line_number}: conflicting byte at 0x{absolute:08x}"
                )
            written[absolute] = value
            image[offset + index] = value
        instruction_count += 1

    if instruction_count == 0:
        raise ValueError("listing contains no instruction bytes in the requested range")
    return bytes(image), instruction_count


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, required=True, help="objdump text listing")
    parser.add_argument("--output", type=Path, required=True, help="raw output image")
    parser.add_argument("--base-address", type=parse_address, required=True)
    parser.add_argument("--end-address", type=parse_address, required=True)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    image, instruction_count = rebuild_listing_range(
        args.input.read_text(encoding="utf-8"),
        args.base_address,
        args.end_address,
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(image)
    print(
        f"listing image: {instruction_count} instructions, {len(image)} bytes, "
        f"0x{args.base_address:08x}-0x{args.end_address:08x}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
