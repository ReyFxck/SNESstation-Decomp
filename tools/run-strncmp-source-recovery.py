#!/usr/bin/env python3
"""Byte-exact historical-source gate for SNES Station's PS2LIB strncmp.S."""
from __future__ import annotations

import hashlib
import re
import subprocess
from pathlib import Path
import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "strncmp-source-recovery"
SOURCE = ROOT / "src" / "ps2" / "strncmp.S"
LISTING = ROOT / "analysis" / "functions" / "libkernel_strings_0019c364.asm"
ADDRESS = 0x0019C410
SIZE = 0x48
SOURCE_SHA256 = "6e41c9ea954ff0fc742fb11a82a9303040a1674353bf50abb42ebf3f60b09a84"
EXPECTED_TARGET_SHA256 = "e29b5df166c0f5b26277258581541c863dd05380bf73493dca8a14007e39f512"

def load_target() -> bytes:
    """Read the already-published instruction listing, not the private ELF."""
    bytes_at: dict[int, int] = {}
    for line in LISTING.read_text(encoding="utf-8").splitlines():
        match = re.match(
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
        raise SystemExit(f"strncmp target listing missing byte at 0x{missing[0]:08x}")
    raw = bytes(bytes_at[a] for a in range(ADDRESS, ADDRESS + SIZE))
    if hashlib.sha256(raw).hexdigest() != EXPECTED_TARGET_SHA256:
        raise SystemExit("strncmp target listing hash drift")
    return raw

def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest() != SOURCE_SHA256:
        raise SystemExit("historical strncmp.S source hash drift")

    compiler = rm.libgcc.resolve_tool(
        str(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc")
    )
    version = subprocess.check_output([str(compiler), "-dumpversion"], text=True).strip()
    machine = subprocess.check_output([str(compiler), "-dumpmachine"], text=True).strip()
    if (version, machine) != ("3.2.2", "ee"):
        raise SystemExit(f"unexpected compiler: {machine} gcc {version}")

    target = load_target()
    obj = BUILD / "strncmp.o"
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
    print("source: src/ps2/strncmp.S")
    print(f"source provenance: ps2sdk@{rm.APR15} ee/libc/src/strncmp.S")
    print(f"compiler: {machine} gcc {version}")
    print(f"{'MATCH' if ok else 'DIFF':5} strncmp.o target={SIZE} text={len(image)} "
          f"relocs={len(reloc_masks)} diff={diff}")
    if not ok:
        raise SystemExit("STRNCMP historical-source object gate: FAIL")
    print("STRNCMP historical-source object gate: 1/1 MATCH")
    print("STRNCMP raw target gate: MATCH 72/72 bytes")
    print("raw sha256:", hashlib.sha256(image).hexdigest())

if __name__ == "__main__":
    main()
