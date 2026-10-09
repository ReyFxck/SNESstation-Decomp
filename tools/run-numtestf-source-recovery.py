#!/usr/bin/env python3
"""Prove the byte-exact numtestf reconstruction against the committed listing."""
from __future__ import annotations

import hashlib
import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/numtestf-source-recovery"
LOCAL = ROOT / "src/ps2/numtestf.S"
LISTING = ROOT / "analysis/functions/math_frontier_0019fddc.asm"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

ADDRESS = 0x001A06C0
END = 0x001A0740
SIZE = END - ADDRESS
EXPECTED_RELOCS = 0
LINE_RE = re.compile(r"^\s*([0-9A-Fa-f]+):\s+((?:[0-9A-Fa-f]{2}\s+){4})")

def run(cmd):
    cp = subprocess.run([str(x) for x in cmd], cwd=ROOT, text=True,
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, cmd)) + "\n" + cp.stdout[-12000:])
    return cp.stdout

def target_bytes() -> bytes:
    image = bytearray(SIZE)
    seen = set()
    for line in LISTING.read_text(encoding="utf-8").splitlines():
        match = LINE_RE.match(line)
        if not match:
            continue
        address = int(match.group(1), 16)
        if not ADDRESS <= address < END:
            continue
        raw = bytes(int(x, 16) for x in match.group(2).split())
        offset = address - ADDRESS
        if offset + 4 > SIZE or offset in seen:
            raise SystemExit("invalid or duplicate committed numtestf listing row")
        image[offset:offset + 4] = raw
        seen.add(offset)

    if 0 not in seen or (SIZE - 4) not in seen:
        raise SystemExit("committed numtestf listing does not cover both boundaries")
    return bytes(image)

def main():
    BUILD.mkdir(parents=True, exist_ok=True)
    if run([CC, "-dumpmachine"]).strip() != "ee" or run([CC, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")

    target = target_bytes()
    obj = BUILD / "numtestf.o"
    run([
        CC, "-G0", "-EL", "-pipe", "-w",
        "-Dnumtestf_candidate=numtestf_001a06c0",
        "-c", LOCAL, "-o", obj,
    ])

    elf = ELFFile(obj)
    sym = elf.find_symbol("numtestf_001a06c0")
    raw = elf.symbol_bytes(sym, sym.size)
    relocs = len(elf.relocation_ranges(sym, sym.size))

    if sym.size != SIZE:
        raise SystemExit(f"numtestf size drift: {sym.size} != {SIZE}")
    if relocs != EXPECTED_RELOCS:
        raise SystemExit(f"numtestf relocation drift: {relocs} != {EXPECTED_RELOCS}")
    if raw != target:
        first = next(i for i, (a, b) in enumerate(zip(raw, target)) if a != b)
        raise SystemExit(f"numtestf differs from committed target at +0x{first:x}")

    digest = hashlib.sha256(raw).hexdigest()
    print(
        f"NUMTESTF local: MATCH bytes={SIZE}/{SIZE} relocations={relocs} "
        f"raw_sha256={digest}"
    )

if __name__ == "__main__":
    main()
