#!/usr/bin/env python3
"""Prove the isolated byte-exact S9xSetC4 reconstruction."""
from __future__ import annotations
import hashlib
import subprocess
from pathlib import Path
from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/s9xsetc4-source-recovery"
LOCAL = ROOT / "src/ps2/s9xsetc4.S"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
SIZE = 2912
EXPECTED_RELOCS = 0
EXPECTED_RAW_SHA256 = "c359e0e1703b848417d7277de9c79d4c63335d9a2e77d91c7961c1b6fc4d32cb"

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
    obj = BUILD / "s9xsetc4_exact.o"
    run([CC, "-G0", "-EL", "-pipe", "-w", "-c", LOCAL, "-o", obj])
    elf = ELFFile(obj)
    sym = elf.find_symbol("snes_p28_0010d7dc")
    raw = elf.symbol_bytes(sym, sym.size)
    relocs = len(elf.relocation_ranges(sym, sym.size))
    if sym.size != SIZE:
        raise SystemExit(f"S9xSetC4 size drift: {sym.size} != {SIZE}")
    if relocs != EXPECTED_RELOCS:
        raise SystemExit(f"S9xSetC4 relocation drift: {relocs} != {EXPECTED_RELOCS}")
    digest = hashlib.sha256(raw).hexdigest()
    if digest != EXPECTED_RAW_SHA256:
        raise SystemExit(f"S9xSetC4 raw SHA drift: {digest}")
    print(f"S9XSETC4 local: MATCH bytes={SIZE}/{SIZE} relocations={relocs} raw_sha256={digest}")

if __name__ == "__main__":
    main()
