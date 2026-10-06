#!/usr/bin/env python3
"""Prove isolated historical S9xGetMemPointer against the target span."""
from __future__ import annotations

import hashlib
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "matching/candidates/s9xgetmempointer_historical.cpp"
BUILD = ROOT / "build/matching/s9xgetmempointer-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"

SYMBOL = "_Z16S9xGetMemPointerj"
SIZE = 340
TARGET_SHA = "c0fb3f0a7e91e013b5d9f76512599c76e64c85f02785168773594691e40d64cb"


def run(cmd):
    cp = subprocess.run([str(x) for x in cmd], cwd=ROOT, text=True,
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, cmd)) + "\n" + cp.stdout[-12000:])
    return cp.stdout


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical C++ compiler")

    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "s9xgetmempointer.o"
    flags = [
        "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
        "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
        "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
        "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET", "-x", "c++",
    ]
    run([CXX, *flags, "-c", SOURCE, "-o", obj])

    nm = CXX.with_name("ee-nm")
    rows = run([nm, "-S", "--size-sort", obj]).splitlines()
    matches = [row for row in rows if row.rstrip().endswith(" " + SYMBOL)]
    if len(matches) != 1:
        raise SystemExit(f"missing/duplicate {SYMBOL}: {matches}")
    fields = matches[0].split()
    actual_size = int(fields[1], 16)
    if actual_size != SIZE:
        raise SystemExit(f"S9xGetMemPointer size mismatch: {actual_size}/{SIZE}")

    ld = CXX.with_name("ee-ld")
    objcopy = CXX.with_name("ee-objcopy")
    script = BUILD / "s9xgetmempointer.target.ld"
    linked = BUILD / "s9xgetmempointer.target.elf"
    binary = BUILD / "s9xgetmempointer.target.bin"
    script.write_text(
        """PROVIDE(g_p12_memory = 0x0034e2b0);
PROVIDE(g_Settings_blob = 0x003454e0);
PROVIDE(DAT_00413544 = 0x00413544);
PROVIDE(snes_leaf_00158fdc = 0x00158fdc);
SECTIONS {
  . = 0x001ab4e8;
  .text : { *(.text) }
  . = 0x001b1b54;
  .rodata : { *(.rodata*) }
  /DISCARD/ : {
    *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*)
  }
}
""",
        encoding="utf-8",
    )
    run([ld, "-EL", "-T", script, obj, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, binary])
    raw = binary.read_bytes()
    raw_sha = hashlib.sha256(raw).hexdigest()
    if len(raw) != SIZE or raw_sha != TARGET_SHA:
        raise SystemExit(
            f"S9xGetMemPointer raw-linked target mismatch "
            f"bytes={len(raw)}/{SIZE} sha256={raw_sha}"
        )

    print(
        f"S9XGETMEMPOINTER raw-linked: MATCH bytes={SIZE}/{SIZE} "
        f"sha256={raw_sha} target=0x001ab4e8"
    )


if __name__ == "__main__":
    main()
