#!/usr/bin/env python3
"""Prove the byte-exact C4ConvOAM reconstruction against frozen formal evidence."""
from __future__ import annotations

import hashlib
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/c4convoam-source-recovery"
LOCAL = ROOT / "src/ps2/c4convoam.S"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

ADDRESS = 0x0010C340
END = 0x0010C6F8
SIZE = END - ADDRESS
EXPECTED_RELOCS = 0
EXPECTED_RAW_SHA256 = "b429d0a5da3bb161b7b8000da3e1d516a7c8e47f413c73b31e2891a5c3f2ec7a"
EXPECTED_SOURCE_SHA256 = "3c2905cd571060b6d95ced9d6e56f39a1ddf3a2aa6dd7987ffe84f27d3a38efc"

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

    source_sha = hashlib.sha256(LOCAL.read_bytes()).hexdigest()
    if source_sha != EXPECTED_SOURCE_SHA256:
        raise SystemExit(f"C4ConvOAM source SHA drift: {source_sha}")

    obj = BUILD / "c4convoam_exact.o"
    run([
        CC, "-G0", "-EL", "-pipe", "-w",
        "-DC4ConvOAM_candidate=snes_p28_0010c340",
        "-c", LOCAL, "-o", obj,
    ])

    elf = ELFFile(obj)
    sym = elf.find_symbol("snes_p28_0010c340")
    raw = elf.symbol_bytes(sym, sym.size)
    relocs = len(elf.relocation_ranges(sym, sym.size))

    if sym.size != SIZE:
        raise SystemExit(f"C4ConvOAM size drift: {sym.size} != {SIZE}")
    if relocs != EXPECTED_RELOCS:
        raise SystemExit(f"C4ConvOAM relocation drift: {relocs} != {EXPECTED_RELOCS}")

    digest = hashlib.sha256(raw).hexdigest()
    if digest != EXPECTED_RAW_SHA256:
        raise SystemExit(f"C4ConvOAM raw SHA drift: {digest}")

    print(
        f"C4CONVOAM local: MATCH bytes={SIZE}/{SIZE} relocations={relocs} "
        f"raw_sha256={digest}"
    )

if __name__ == "__main__":
    main()
