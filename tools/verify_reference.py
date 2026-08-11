#!/usr/bin/env python3
"""Verify the immutable packed and unpacked SNES Station reference artifacts."""
from __future__ import annotations

import argparse
import hashlib
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PACKED_SIZE = 726_968
PACKED_SHA256 = "4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487"
PACKED_ENTRY = 0x01B00008
UNPACKED_SIZE = 3_304_936
UNPACKED_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
UNPACKED_BASE = 0x00100000
UNPACKED_ENTRY = 0x00100008
PS2_EFLAGS = 0x20924001


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def parse_elf32(path: Path, data: bytes) -> dict[str, object]:
    if len(data) < 52 or data[:4] != b"\x7fELF":
        raise ValueError(f"{path}: not an ELF file")
    if data[4] != 1:
        raise ValueError(f"{path}: expected ELF32, found class {data[4]}")
    if data[5] not in (1, 2):
        raise ValueError(f"{path}: invalid ELF byte order {data[5]}")
    endian = "<" if data[5] == 1 else ">"
    header = struct.unpack_from(endian + "16sHHIIIIIHHHHHH", data, 0)
    entry = header[4]
    phoff = header[5]
    flags = header[7]
    phentsize = header[9]
    phnum = header[10]
    loads: list[dict[str, int]] = []
    for index in range(phnum):
        offset = phoff + index * phentsize
        if offset + 32 > len(data):
            raise ValueError(f"{path}: truncated program header {index}")
        values = struct.unpack_from(endian + "IIIIIIII", data, offset)
        if values[0] == 1:
            loads.append(
                {
                    "offset": values[1],
                    "vaddr": values[2],
                    "filesz": values[4],
                    "memsz": values[5],
                    "flags": values[6],
                    "align": values[7],
                }
            )
    return {"entry": entry, "flags": flags, "loads": loads}


def require_file(path: Path, label: str) -> bytes:
    if not path.is_file():
        raise ValueError(f"missing {label}: {path}")
    return path.read_bytes()


def expect(actual: object, expected: object, label: str, errors: list[str]) -> None:
    if actual != expected:
        errors.append(f"{label}: expected {expected!r}, got {actual!r}")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--packed", type=Path, default=ROOT / "original" / "SNES_EMU.ELF")
    parser.add_argument("--unpacked", type=Path, default=ROOT / "build" / "SNES_EMU.unpacked.bin")
    parser.add_argument("--wrapper", type=Path, default=ROOT / "build" / "SNES_EMU.analysis.elf")
    args = parser.parse_args()

    errors: list[str] = []
    try:
        packed = require_file(args.packed, "packed reference ELF")
        packed_elf = parse_elf32(args.packed, packed)
        unpacked = require_file(args.unpacked, "unpacked reference image")
        wrapper = require_file(args.wrapper, "analysis ELF wrapper")
        wrapper_elf = parse_elf32(args.wrapper, wrapper)
    except ValueError as exc:
        raise SystemExit(f"reference verification failed: {exc}") from exc

    expect(len(packed), PACKED_SIZE, "packed size", errors)
    expect(sha256(packed), PACKED_SHA256, "packed SHA-256", errors)
    expect(packed_elf["entry"], PACKED_ENTRY, "packed ELF entry", errors)
    expect(len(unpacked), UNPACKED_SIZE, "unpacked size", errors)
    expect(sha256(unpacked), UNPACKED_SHA256, "unpacked SHA-256", errors)
    expect(wrapper_elf["entry"], UNPACKED_ENTRY, "analysis wrapper entry", errors)
    expect(wrapper_elf["flags"], PS2_EFLAGS, "analysis wrapper ELF flags", errors)

    loads = wrapper_elf["loads"]
    if not isinstance(loads, list) or len(loads) != 1:
        errors.append(f"analysis wrapper PT_LOAD count: expected 1, got {len(loads)}")
    else:
        load = loads[0]
        expect(load["vaddr"], UNPACKED_BASE, "analysis wrapper load address", errors)
        expect(load["filesz"], UNPACKED_SIZE, "analysis wrapper file size", errors)
        expect(load["memsz"], UNPACKED_SIZE, "analysis wrapper memory size", errors)
        start = load["offset"]
        end = start + load["filesz"]
        if end > len(wrapper):
            errors.append("analysis wrapper PT_LOAD extends beyond the file")
        else:
            expect(sha256(wrapper[start:end]), UNPACKED_SHA256, "analysis wrapper payload SHA-256", errors)

    if errors:
        for error in errors:
            print(f"ERROR: {error}")
        raise SystemExit(f"reference verification failed with {len(errors)} error(s)")

    print(f"packed:   OK  size={len(packed):,} sha256={PACKED_SHA256}")
    print(f"unpacked: OK  size={len(unpacked):,} sha256={UNPACKED_SHA256}")
    print(
        f"layout:   OK  packed_entry=0x{PACKED_ENTRY:08x} "
        f"base=0x{UNPACKED_BASE:08x} unpacked_entry=0x{UNPACKED_ENTRY:08x}"
    )


if __name__ == "__main__":
    main()
