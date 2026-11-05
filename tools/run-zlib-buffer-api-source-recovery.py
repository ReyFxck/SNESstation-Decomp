#!/usr/bin/env python3
"""Exact historical zlib 1.1.3 buffer-wrapper proof for SNES Station."""
from __future__ import annotations

import re
import subprocess
from pathlib import Path

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "third_party" / "historical_refs" / "zlib-1.1.3"
BUILD = ROOT / "build" / "matching" / "zlib-buffer-api-source-recovery"
LISTING = ROOT / "analysis" / "functions" / "zlib_buffer_api_00190700.asm"
BASE = 0x00190700
END = 0x001908BC

FLAGS = (
    "-G0", "-EL", "-pipe", "-w",
    "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common",
    "-fshort-double", "-mlong64", "-mhard-float", "-mno-abicalls",
    "-march=r5900", "-mtune=r5900",
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD",
    "-DCODE_PLATFORM=3", "-Os",
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


def target_bytes() -> bytes:
    byte_map: dict[int, int] = {}
    for line in LISTING.read_text(encoding="utf-8", errors="replace").splitlines():
        m = INSN.match(line)
        if not m:
            continue
        address = int(m.group(1), 16)
        raw = bytes(int(m.group(i), 16) for i in range(2, 6))
        for off, value in enumerate(raw):
            byte_map[address + off] = value
    missing = [a for a in range(BASE, END) if a not in byte_map]
    if missing:
        raise SystemExit(
            f"zlib buffer target missing {len(missing)} bytes; first=0x{missing[0]:08x}"
        )
    return bytes(byte_map[a] for a in range(BASE, END))


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    cc = rm.libgcc.resolve_tool(
        str(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc")
    )
    if run([cc, "-dumpmachine"]).strip() != "ee" or run([cc, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("zlib buffer proof requires EE GCC 3.2.2")
    if '#define ZLIB_VERSION "1.1.3"' not in (SRC / "zlib.h").read_text(encoding="latin-1"):
        raise SystemExit("historical zlib header identity drift")

    target = target_bytes()
    specs = (
        ("compress.c", "compress.o", 0x00190700, 0x00E8),
        ("uncompr.c", "uncompr.o", 0x001907E8, 0x00D4),
    )
    objects: list[Path] = []
    for source_name, object_name, address, size in specs:
        obj = BUILD / object_name
        run([cc, *FLAGS, "-I", SRC, "-c", SRC / source_name, "-o", obj])
        objects.append(obj)
        image, masks, normalized = rm.libgcc.text_image(obj)
        expected = target[address - BASE:address - BASE + size]
        diff = rm.libgcc.differing_unmasked(expected, image, masks)
        ok = (
            len(image) == size
            and diff == 0
            and normalized == rm.libgcc.normalize_target(expected, masks)
        )
        print(
            f"{'MATCH' if ok else 'DIFF':5} {object_name:12} "
            f"target=0x{address:08x} bytes={len(image)}/{size} "
            f"relocs={len(masks)} diff={diff}"
        )
        if not ok:
            raise SystemExit(f"{object_name} historical-source proof failed")

    ld = cc.with_name("ee-ld")
    objcopy = cc.with_name("ee-objcopy")
    linker = BUILD / "zlib-buffer-api.ld"
    linked = BUILD / "zlib-buffer-api.elf"
    linked_text = BUILD / "zlib-buffer-api.text.bin"
    linker.write_text(
        """ENTRY(compress2)

PROVIDE(deflateInit_ = 0x001908bc);
PROVIDE(deflate = 0x00190fa0);
PROVIDE(deflateEnd = 0x00191308);
PROVIDE(inflateInit_ = 0x00192948);
PROVIDE(inflate = 0x0019296c);
PROVIDE(inflateEnd = 0x00192784);

SECTIONS
{
  . = 0x00190700;
  .text : {
    *compress.o(.text)
    *uncompr.o(.text)
  }

  . = 0x001b8900;
  .rodata : {
    *compress.o(.rodata)
    *uncompr.o(.rodata)
  }

  /DISCARD/ : {
    *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*)
  }
}
""",
        encoding="utf-8",
    )
    run([ld, "-EL", "-T", linker, *objects, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, linked_text])
    raw = linked_text.read_bytes()
    if raw != target:
        limit = min(len(raw), len(target))
        first = next((i for i in range(limit) if raw[i] != target[i]), None)
        if first is None and len(raw) != len(target):
            first = limit
        raise SystemExit(
            f"zlib buffer raw link FAIL bytes={len(raw)}/{len(target)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )
    print(f"RAW MATCH zlib buffer wrappers bytes={len(raw)}/{len(target)}")

    objdump = cc.with_name("ee-objdump")
    for obj in objects:
        print(run([objdump, "-dr", obj]))
    print("ZLIB 1.1.3 buffer source gate: 2/2 MATCH")


if __name__ == "__main__":
    main()
