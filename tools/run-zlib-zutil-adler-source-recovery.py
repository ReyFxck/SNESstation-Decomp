#!/usr/bin/env python3
"""Exact zlib 1.1.3 zutil/adler tail proof for SNES Station."""
from __future__ import annotations

import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile, compare_function
import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "third_party" / "historical_refs" / "zlib-1.1.3"
BUILD = ROOT / "build" / "matching" / "zlib-zutil-adler-source-recovery"
LISTING = ROOT / "analysis" / "functions" / "zlib_zutil_adler_00198a84.asm"
TARGET_BASE = 0x00100000
START = 0x00198A84
END = 0x00198C58

COMMON = (
    "-G0", "-EL", "-pipe", "-w",
    "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common",
    "-fshort-double", "-mlong64", "-mhard-float", "-mno-abicalls",
    "-march=r5900", "-mtune=r5900",
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD",
    "-DCODE_PLATFORM=3",
)

INSN = re.compile(
    r"^\s*([0-9A-Fa-f]+):\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
)


def run(command: list[str | Path]) -> str:
    cp = subprocess.run(
        [str(x) for x in command], cwd=ROOT, text=True,
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    if cp.returncode:
        raise SystemExit(cp.stdout[-8000:])
    return cp.stdout


def target_image() -> tuple[bytes, dict[int, int]]:
    byte_map: dict[int, int] = {}
    for line in LISTING.read_text(encoding="utf-8", errors="replace").splitlines():
        m = INSN.match(line)
        if not m:
            continue
        address = int(m.group(1), 16)
        raw = bytes(int(m.group(i), 16) for i in range(2, 6))
        for off, value in enumerate(raw):
            byte_map[address + off] = value
    missing = [a for a in range(START, END) if a not in byte_map]
    if missing:
        raise SystemExit(
            f"zutil/adler target missing {len(missing)} bytes; first=0x{missing[0]:08x}"
        )
    image = bytearray(max(byte_map) + 1 - TARGET_BASE)
    for address, value in byte_map.items():
        image[address - TARGET_BASE] = value
    return bytes(image), byte_map


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    cc = rm.libgcc.resolve_tool(
        str(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc")
    )
    if run([cc, "-dumpmachine"]).strip() != "ee" or run([cc, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("zutil/adler proof requires EE GCC 3.2.2")

    zutil_obj = BUILD / "zutil.o"
    adler_obj = BUILD / "adler32.o"
    run([cc, *COMMON, "-O3", "-I", SRC, "-c", SRC / "zutil.c", "-o", zutil_obj])
    run([cc, *COMMON, "-Os", "-I", SRC, "-c", SRC / "adler32.c", "-o", adler_obj])

    target, byte_map = target_image()
    checks = (
        (zutil_obj, "zcalloc", 0x00198A84, 32),
        (zutil_obj, "zcfree", 0x00198AA4, 28),
        (adler_obj, "adler32", 0x00198AC0, 404),
    )
    for obj, symbol, address, size in checks:
        elf = ELFFile(obj)
        entry = elf.find_symbol(symbol)
        comp = compare_function(target, address - TARGET_BASE, size, elf, symbol)
        ok = (
            entry.size == size
            and comp.matching
            and not comp.unknown_relocation_types
        )
        print(
            f"{'MATCH' if ok else 'DIFF':5} {obj.name:10} {symbol:8} "
            f"target=0x{address:08x} bytes={entry.size}/{size} "
            f"relocs={len(comp.relocations)} diff={comp.differing_bytes}"
        )
        if not ok:
            raise SystemExit(f"{symbol} historical-source proof failed")

    # Final relocation proof. Link the full zutil object at its historical base,
    # then compare only the public zcalloc/zcfree slice. Link adler32 at its
    # target and require its function body, plus the public zero alignment word.
    ld = cc.with_name("ee-ld")
    objcopy = cc.with_name("ee-objcopy")
    linker = BUILD / "zutil-adler.ld"
    linked = BUILD / "zutil-adler.elf"
    binary = BUILD / "zutil-adler.bin"
    linker.write_text(
        """ENTRY(zlibVersion)

PROVIDE(calloc = 0x0019e648);
PROVIDE(free = 0x0019e784);
PROVIDE(__moddi3 = 0x001a2c78);

SECTIONS
{
  .zutil 0x00198a58 : { *zutil.o(.text) }
  .adler 0x00198ac0 : { *adler32.o(.text) }
  .pad 0x00198c54 : {
    BYTE(0); BYTE(0); BYTE(0); BYTE(0);
  }

  .zutil_rodata 0x001ba1a8 : { *zutil.o(.rodata) }
  .zutil_data 0x00425a00 : { *zutil.o(.data) }

  /DISCARD/ : {
    *(.bss*) *(COMMON) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*)
  }
}
""",
        encoding="utf-8",
    )
    run([ld, "-EL", "-T", linker, zutil_obj, adler_obj, "-o", linked])
    run([
        objcopy, "-j", ".zutil", "-j", ".adler", "-j", ".pad",
        "-O", "binary", linked, binary,
    ])
    raw = binary.read_bytes()
    slice_start = START - 0x00198A58
    actual = raw[slice_start:slice_start + (END - START)]
    expected = bytes(byte_map[a] for a in range(START, END))
    if actual != expected:
        limit = min(len(actual), len(expected))
        first = next((i for i in range(limit) if actual[i] != expected[i]), None)
        if first is None and len(actual) != len(expected):
            first = limit
        raise SystemExit(
            f"zutil/adler raw link FAIL bytes={len(actual)}/{len(expected)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )
    print(f"RAW MATCH zutil/adler tail bytes={len(actual)}/{len(expected)}")

    for obj in (zutil_obj, adler_obj):
        elf = ELFFile(obj)
        inventory = {
            section.name: section.size
            for section in elf.sections
            if section.name in {
                ".text", ".rel.text", ".data", ".rel.data",
                ".rodata", ".rel.rodata", ".bss",
            }
        }
        print(f"SECTIONS {obj.name}: {inventory}")
    print("ZLIB 1.1.3 zutil/adler source gate: 3/3 MATCH")


if __name__ == "__main__":
    main()
