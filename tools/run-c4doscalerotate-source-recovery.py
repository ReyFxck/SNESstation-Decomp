#!/usr/bin/env python3
"""Prove the isolated byte-exact C4DoScaleRotate reconstruction."""
from __future__ import annotations

import hashlib
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/c4doscalerotate-source-recovery"
LOCAL = ROOT / "src/ps2/c4doscalerotate.S"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

ADDRESS = 0x0010C6F8
SIZE = 1208
EXPECTED_RELOCS = 0
EXPECTED_RAW_SHA256 = "af69f02cd294a134cc49ec19776a6072947ebdb950f69b7705de1e1008b8d1f6"

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

    obj = BUILD / "c4doscalerotate_exact.o"
    run([CC, "-G0", "-EL", "-pipe", "-w", "-c", LOCAL, "-o", obj])

    elf = ELFFile(obj)
    sym = elf.find_symbol("snes_p28_0010c6f8")
    raw = elf.symbol_bytes(sym, sym.size)
    relocs = len(elf.relocation_ranges(sym, sym.size))

    if sym.size != SIZE:
        raise SystemExit(f"C4DoScaleRotate size drift: {sym.size} != {SIZE}")
    if relocs != EXPECTED_RELOCS:
        raise SystemExit(f"C4DoScaleRotate relocation drift: {relocs} != {EXPECTED_RELOCS}")

    digest = hashlib.sha256(raw).hexdigest()
    if digest != EXPECTED_RAW_SHA256:
        raise SystemExit(f"C4DoScaleRotate raw SHA drift: {digest}")

    print(
        f"C4DOSCALEROTATE local: MATCH bytes={SIZE}/{SIZE} relocations={relocs} "
        f"raw_sha256={digest}"
    )

if __name__ == "__main__":
    main()
