#!/usr/bin/env python3
"""Byte-exact historical-source gate for SNES Station's PS2LIB strcmp.S."""
from __future__ import annotations

import hashlib
import re
import subprocess
from pathlib import Path
import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "strcmp-source-recovery"
SOURCE = ROOT / "src" / "ps2" / "strcmp.S"
LISTING = ROOT / "analysis" / "functions" / "libkernel_libc_strings_0019c3d4.asm"
ADDRESS = 0x0019C648
SIZE = 0x40
SOURCE_SHA256 = "0e2ab99e7b9da06ef8f91d0bb332847230eb77c32b6fc0c26675a1a1c6909763"
EXPECTED_TARGET_SHA256 = "6cd83dc8cddc5055f0a598c6f3d52530a9339c2fbbb5ba877c9e198509fe87ef"

def load_target() -> bytes:
    """Read the already-published instruction listing, not the private ELF."""
    bytes_at: dict[int, int] = {}
    for line in LISTING.read_text(encoding="utf-8").splitlines():
        match = re.fullmatch(
            r"\s*([0-9a-fA-F]+):\s+([0-9a-fA-F]{2})\s+"
            r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s*",
            line,
        )
        if match:
            address = int(match.group(1), 16)
            for index, field in enumerate(match.groups()[1:]):
                bytes_at[address + index] = int(field, 16)
    missing = [a for a in range(ADDRESS, ADDRESS + SIZE) if a not in bytes_at]
    if missing:
        raise SystemExit(f"strcmp target listing missing byte at 0x{missing[0]:08x}")
    raw = bytes(bytes_at[a] for a in range(ADDRESS, ADDRESS + SIZE))
    if hashlib.sha256(raw).hexdigest() != EXPECTED_TARGET_SHA256:
        raise SystemExit("strcmp target listing hash drift")
    return raw

def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest() != SOURCE_SHA256:
        raise SystemExit("historical strcmp.S source hash drift")

    compiler = rm.libgcc.resolve_tool(
        str(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc")
    )
    version = subprocess.check_output([str(compiler), "-dumpversion"], text=True).strip()
    machine = subprocess.check_output([str(compiler), "-dumpmachine"], text=True).strip()
    if (version, machine) != ("3.2.2", "ee"):
        raise SystemExit(f"unexpected compiler: {machine} gcc {version}")

    target = load_target()
    obj = BUILD / "strcmp.o"
    cp = subprocess.run(
        [str(compiler), *rm.FLAGS, "-c", str(SOURCE), "-o", str(obj)],
        cwd=ROOT, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    if cp.returncode:
        raise SystemExit(cp.stdout[-8000:])

    image, reloc_masks, normalized = rm.libgcc.text_image(obj)
    diff = rm.libgcc.differing_unmasked(target, image, reloc_masks)
    ok = (
        len(image) == SIZE
        and not reloc_masks
        and diff == 0
        and image == target
        and normalized == target
        and hashlib.sha256(image).hexdigest() == EXPECTED_TARGET_SHA256
    )
    print("source: src/ps2/strcmp.S")
    print(f"source provenance: ps2sdk@{rm.APR15} ee/libc/src/strcmp.S")
    print(f"compiler: {machine} gcc {version}")
    print(f"{'MATCH' if ok else 'DIFF':5} strcmp.o target={SIZE} text={len(image)} "
          f"relocs={len(reloc_masks)} diff={diff}")
    if not ok:
        raise SystemExit("STRCMP historical-source object gate: FAIL")
    print("STRCMP historical-source object gate: 1/1 MATCH")
    print("STRCMP raw target gate: MATCH 64/64 bytes")
    print("raw sha256:", hashlib.sha256(image).hexdigest())

if __name__ == "__main__":
    main()
