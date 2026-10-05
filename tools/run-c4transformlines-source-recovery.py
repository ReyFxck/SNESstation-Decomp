#!/usr/bin/env python3
"""Prove the isolated byte-exact C4TransformLines reconstruction."""
from __future__ import annotations

import hashlib
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/c4transformlines-source-recovery"
LOCAL = ROOT / "matching/candidates/c4transformlines_exact.S"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

SIZE = 772
EXPECTED_RELOCS = 0
EXPECTED_RAW_SHA256 = "ecd50e64600de46c56c6b1f0de28cb97ba401dcd5c6125c2dcd154c543462690"

def run(cmd):
    cp = subprocess.run([str(x) for x in cmd], cwd=ROOT, text=True,
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, cmd)) + "\n" + cp.stdout[-12000:])
    return cp.stdout

def main():
    BUILD.mkdir(parents=True, exist_ok=True)
    if run([CC, "-dumpmachine"]).strip() != "ee" or run([CC, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")

    obj = BUILD / "c4transformlines_exact.o"
    run([CC, "-G0", "-EL", "-pipe", "-w", "-c", LOCAL, "-o", obj])

    elf = ELFFile(obj)
    sym = elf.find_symbol("v81_0010cfa4")
    raw = elf.symbol_bytes(sym, sym.size)
    relocs = len(elf.relocation_ranges(sym, sym.size))

    if sym.size != SIZE:
        raise SystemExit(f"C4TransformLines size drift: {sym.size} != {SIZE}")
    if relocs != EXPECTED_RELOCS:
        raise SystemExit(f"C4TransformLines relocation drift: {relocs} != {EXPECTED_RELOCS}")

    digest = hashlib.sha256(raw).hexdigest()
    if digest != EXPECTED_RAW_SHA256:
        raise SystemExit(f"C4TransformLines raw SHA drift: {digest}")

    print(
        f"C4TRANSFORMLINES local: MATCH bytes={SIZE}/{SIZE} relocations={relocs} "
        f"raw_sha256={digest}"
    )

if __name__ == "__main__":
    main()
