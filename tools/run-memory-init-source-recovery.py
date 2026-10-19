#!/usr/bin/env python3
"""Development gate for recovering CMemory::Init from canonical C source.

The target bytes come from the committed V80 exact public listing, not from a
private ELF.  This runner recompiles src/snes9x/memory_init_recovered.c with the
historical EE GCC 3.2.2 profile, links its live relocations to the target
providers, and reports literal instruction differences against the 700-byte
target span at 0x00151074.
"""
from __future__ import annotations

import shlex
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import build_source_tree as source_tree  # noqa: E402
from compare_elf_functions import ELFFile  # noqa: E402

SOURCE = ROOT / "src/snes9x/memory_init_recovered.c"
LISTING = ROOT / "matching/candidates/hunt1041_v80_quickwins_exact.S"
BUILD = ROOT / "build/matching/memory-init-source-recovery"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

SYMBOL = "CMemory_Init_recovered"
TARGET_ADDRESS = 0x00151074
TARGET_SIZE = 0x2BC
PROVIDERS = {
    "malloc": 0x0019E4B4,
    "memset": 0x0019C39C,
    "rom_cleanup_00151330": 0x00151330,
    "g_tile_cache_state": 0x0035C268,
    "g_global_sram": 0x0034E298,
    "g_global_rom": 0x0034E29C,
    "g_global_regram": 0x0034E2A0,
    "g_memory_fillram": 0x0034E2C4,
    "g_superfx_state": 0x0035B770,
}


def run(command):
    cp = subprocess.run(
        [str(x) for x in command],
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    if cp.returncode:
        raise SystemExit(
            "command failed: " + " ".join(map(str, command)) + "\n" + cp.stdout[-16000:]
        )
    return cp.stdout


def lower_text_section_alignment(source: Path, destination: Path) -> None:
    """Set only ELF32 .text sh_addralign to 4; leave every byte of code intact."""
    data = bytearray(source.read_bytes())
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        raise SystemExit("expected ELF32 little-endian EE object")
    shoff = struct.unpack_from("<I", data, 32)[0]
    shentsize = struct.unpack_from("<H", data, 46)[0]
    shnum = struct.unpack_from("<H", data, 48)[0]
    shstrndx = struct.unpack_from("<H", data, 50)[0]
    if shentsize != 40 or shstrndx >= shnum:
        raise SystemExit("unexpected ELF section-header geometry")
    shstr = shoff + shstrndx * shentsize
    names_off = struct.unpack_from("<I", data, shstr + 16)[0]
    names_size = struct.unpack_from("<I", data, shstr + 20)[0]
    names = data[names_off:names_off + names_size]
    for index in range(shnum):
        header = shoff + index * shentsize
        name_off = struct.unpack_from("<I", data, header)[0]
        if name_off >= len(names):
            continue
        end = names.find(b"\x00", name_off)
        if end < 0:
            continue
        if names[name_off:end] == b".text":
            struct.pack_into("<I", data, header + 32, 4)
            destination.write_bytes(data)
            return
    raise SystemExit("object has no .text section")


def target_bytes() -> bytes:
    text = LISTING.read_text(encoding="utf-8")
    start = text.index("/* CMemory_Init @ 0x00151074..0x00151330")
    end = text.index(".size v80_00151074, .-v80_00151074", start)
    words = []
    for line in text[start:end].splitlines():
        stripped = line.strip()
        if not stripped.startswith(".word "):
            continue
        value = int(stripped.split()[1], 0)
        words.append(value)
    raw = b"".join(struct.pack("<I", word) for word in words)
    if len(raw) != TARGET_SIZE:
        raise SystemExit(f"target listing geometry drift: {len(raw)} != {TARGET_SIZE}")
    return raw


def main() -> None:
    if run([CC, "-dumpmachine"]).strip() != "ee" or run([CC, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical EE compiler")

    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "memory_init_recovered.o"
    # The target's compact branch layout may use the size-optimized EE
    # profile; test that profile while keeping every other historical flag.
    flags = [
        "-Os" if flag == "-O2" else flag
        for flag in shlex.split(source_tree.DEFAULT_CFLAGS)
    ]
    flags.append("-mbranch-likely")
    run([CC, *flags, "-c", SOURCE, "-o", obj])

    # In the original shared .text, this function is 4-byte aligned but not
    # 8-byte aligned. Lower only the standalone object's section metadata;
    # objcopy does not change any instruction bytes.
    aligned_obj = BUILD / "memory_init_recovered.align4.o"
    lower_text_section_alignment(obj, aligned_obj)

    elf = ELFFile(obj)
    sym = elf.find_symbol(SYMBOL)
    raw = elf.symbol_bytes(sym, sym.size)
    relocs = elf.relocation_ranges(sym, sym.size)
    imports = sorted(
        s.name for s in elf.symbols
        if s.section_index == 0 and s.name
    )

    unknown = [name for name in imports if name not in PROVIDERS]
    if unknown:
        raise SystemExit("unreviewed imports: " + ", ".join(unknown))

    ld = CC.with_name("ee-ld")
    script = BUILD / "memory_init.target.ld"
    linked = BUILD / "memory_init.target.elf"
    base = TARGET_ADDRESS - sym.value
    script.write_text(
        "".join(f"PROVIDE({name} = 0x{address:08x});\n" for name, address in PROVIDERS.items())
        + "SECTIONS {\n"
        + f"  .text 0x{base:08x} : {{ *(.text) }}\n"
        + "  /DISCARD/ : { *(.data*) *(.rodata*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) }\n"
        + "}\n",
        encoding="utf-8",
    )
    run([ld, "-EL", "-T", script, aligned_obj, "-o", linked])

    linked_elf = ELFFile(linked)
    linked_sym = linked_elf.find_symbol(SYMBOL)
    linked_raw = linked_elf.symbol_bytes(linked_sym, linked_sym.size)
    target = target_bytes()

    print(
        f"CMemory::Init candidate: size={sym.size}/{TARGET_SIZE} "
        f"relocations={len(relocs)} imports={','.join(imports)}"
    )
    print(
        f"placement: symbol=0x{linked_sym.value:08x} target=0x{TARGET_ADDRESS:08x} "
        f"object_text_offset=0x{sym.value:x}"
    )

    limit = min(len(linked_raw), len(target))
    byte_diffs = [i for i in range(limit) if linked_raw[i] != target[i]]
    if len(linked_raw) != len(target):
        byte_diffs.extend(range(limit, max(len(linked_raw), len(target))))

    word_diffs = []
    for off in range(0, min(len(linked_raw), len(target)), 4):
        left = struct.unpack_from("<I", linked_raw, off)[0]
        right = struct.unpack_from("<I", target, off)[0]
        if left != right:
            word_diffs.append((off, left, right))

    if linked_sym.value != TARGET_ADDRESS:
        raise SystemExit("linked placement drift")

    if not byte_diffs and len(linked_raw) == TARGET_SIZE:
        print(f"CMemory::Init source recovery: MATCH bytes={TARGET_SIZE}/{TARGET_SIZE}")
        return

    print(
        f"CMemory::Init source recovery: MISMATCH differing_bytes={len(byte_diffs)} "
        f"candidate_size={len(linked_raw)} target_size={TARGET_SIZE}"
    )
    if byte_diffs:
        print("first differing byte offsets: " + ", ".join(f"+0x{x:x}" for x in byte_diffs[:24]))
    if word_diffs:
        print("first differing words (offset candidate target):")
        for off, left, right in word_diffs[:40]:
            print(f"  +0x{off:03x} 0x{left:08x} 0x{right:08x}")

    print("success-path words (offset candidate target):")
    for off in range(0x160, min(max(len(linked_raw), len(target)), 0x214), 4):
        left = struct.unpack_from("<I", linked_raw, off)[0] if off + 4 <= len(linked_raw) else None
        right = struct.unpack_from("<I", target, off)[0] if off + 4 <= len(target) else None
        left_text = f"0x{left:08x}" if left is not None else "--------"
        right_text = f"0x{right:08x}" if right is not None else "--------"
        print(f"  +0x{off:03x} {left_text} {right_text}")
    raise SystemExit(1)


if __name__ == "__main__":
    main()
