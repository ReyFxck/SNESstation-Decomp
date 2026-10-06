#!/usr/bin/env python3
"""Prove isolated historical Snes9x 1.40 S9xGetByte against the target span."""
from __future__ import annotations

import hashlib
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "matching/candidates/s9xgetbyte_historical.cpp"
BUILD = ROOT / "build/matching/s9xgetbyte-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"

SYMBOL = "_Z10S9xGetBytej"
SIZE = 708
RELOCS = 49
TARGET_SHA = "0e803c2aaafecb8bfc19887f9177e909d011156393483cb46d247f148b6e31e4"


def run(cmd):
    cp = subprocess.run([str(x) for x in cmd], cwd=ROOT, text=True,
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, cmd)) + "\n" + cp.stdout[-16000:])
    return cp.stdout


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical C++ compiler")

    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "s9xgetbyte.o"
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
        objdump = CXX.with_name("ee-objdump")
        dump = run([objdump, "-dr", obj])
        raise SystemExit(
            f"S9xGetByte size mismatch: {actual_size}/{SIZE}\n"
            + dump[-16000:]
        )

    readelf = CXX.with_name("ee-readelf")
    reltext = run([readelf, "-r", obj])
    lines = reltext.splitlines()
    start = next((i for i, line in enumerate(lines)
                  if "Relocation section '.rel.text'" in line), -1)
    if start < 0:
        raise SystemExit("missing .rel.text relocation section")
    end = next((i for i in range(start + 1, len(lines))
                if lines[i].startswith("Relocation section '")), len(lines))
    reloc_count = sum(1 for line in lines[start:end] if "R_MIPS_" in line)
    if reloc_count != RELOCS:
        raise SystemExit(
            f"S9xGetByte relocation count mismatch: {reloc_count}/{RELOCS}\n{reltext}"
        )

    ld = CXX.with_name("ee-ld")
    objcopy = CXX.with_name("ee-objcopy")
    script = BUILD / "s9xgetbyte.target.ld"
    linked = BUILD / "s9xgetbyte.target.elf"
    binary = BUILD / "s9xgetbyte.target.bin"
    script.write_text(
        """PROVIDE(g_p12_memory = 0x0034e2b0);
PROVIDE(g_CPU_blob = 0x00345340);
PROVIDE(g_OpenBus_byte = 0x0035b768);
PROVIDE(dep_S9xGetPPU = 0x0015a5f0);
PROVIDE(dep_S9xGetCPU = 0x0015bc70);
PROVIDE(dep_S9xGetDSP = 0x0012e704);
PROVIDE(dep_S9xGetC4 = 0x0010c328);
PROVIDE(dep_S9xGetSPC7110Byte = 0x0018255c);
PROVIDE(dep_S9xGetSPC7110 = 0x001813f0);
PROVIDE(dep_GetOBC1 = 0x00158b5c);
PROVIDE(dep_S9xGetSetaDSP = 0x0016fc48);
PROVIDE(dep_S9xGetST018 = 0x001701fc);
SECTIONS {
  . = 0x001ab63c;
  .text : { *(.text) }
  . = 0x001b1150;
  .rodata : { *(.rodata*) }
  /DISCARD/ : {
    *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*)
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
        objdump = CXX.with_name("ee-objdump")
        dump = run([objdump, "-dr", obj])
        raise SystemExit(
            f"S9xGetByte raw-linked target mismatch "
            f"bytes={len(raw)}/{SIZE} sha256={raw_sha} expected={TARGET_SHA}\n"
            f"relocations={reloc_count}/{RELOCS}\n"
            + dump[-16000:]
        )

    print(
        f"S9XGETBYTE raw-linked: MATCH bytes={SIZE}/{SIZE} "
        f"relocations={RELOCS} sha256={raw_sha} target=0x001ab63c"
    )


if __name__ == "__main__":
    main()
