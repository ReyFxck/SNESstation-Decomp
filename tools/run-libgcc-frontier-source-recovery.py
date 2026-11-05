#!/usr/bin/env python3
"""Public proof for the two remaining simple libgcc frontier members."""
from __future__ import annotations

import hashlib
import re
import struct
import subprocess
from pathlib import Path

import code_windows as cw
import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "libgcc-frontier-source-recovery"
LISTING = ROOT / "analysis" / "functions" / "libgcc_frontier_001a1b00.asm"
TARGET_BASE = 0x00100000

SPECS = (
    ("gccmuldi", "_muldi3.o", 0x001A1B20, 0x78, 0, {}),
    ("gccmakedp", "_make_df.o", 0x001A3C90, 0x30, 1, {"__pack_d": 0x001A7F40}),
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


def listing_bytes(start: int, size: int) -> bytes:
    byte_map: dict[int, int] = {}
    for line in LISTING.read_text(encoding="utf-8", errors="replace").splitlines():
        m = INSN.match(line)
        if not m:
            continue
        address = int(m.group(1), 16)
        raw = bytes(int(m.group(i), 16) for i in range(2, 6))
        for off, value in enumerate(raw):
            byte_map[address + off] = value
    missing = [a for a in range(start, start + size) if a not in byte_map]
    if missing:
        raise SystemExit(
            f"listing missing {len(missing)} bytes for 0x{start:08x}; "
            f"first=0x{missing[0]:08x}"
        )
    return bytes(byte_map[a] for a in range(start, start + size))


def extract(archive: Path, member: str) -> Path:
    out = BUILD / member
    cp = subprocess.run(
        ["ar", "p", str(archive), member], cwd=ROOT,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
    )
    if cp.returncode:
        raise SystemExit(cp.stderr.decode(errors="replace")[-4000:])
    out.write_bytes(cp.stdout)
    return out


def raw_link(cc: Path, obj: Path, address: int, provides: dict[str, int], expected: bytes) -> None:
    ld = cc.with_name("ee-ld")
    objcopy = cc.with_name("ee-objcopy")
    stem = obj.stem
    script = BUILD / f"{stem}.ld"
    linked = BUILD / f"{stem}.elf"
    raw_path = BUILD / f"{stem}.bin"
    provide_text = "\n".join(
        f"PROVIDE({name} = 0x{value:08x});" for name, value in sorted(provides.items())
    )
    script.write_text(
        provide_text + f"""
SECTIONS
{{
  . = 0x{address:08x};
  .text : {{ *(.text) }}
  /DISCARD/ : {{
    *(.data*) *(.bss*) *(COMMON) *(.rodata*) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*)
  }}
}}
""",
        encoding="utf-8",
    )
    run([ld, "-EL", "-T", script, obj, "-o", linked])
    run([objcopy, "-j", ".text", "-O", "binary", linked, raw_path])
    raw = raw_path.read_bytes()
    if raw != expected:
        first = next(
            (i for i, (a, b) in enumerate(zip(raw, expected)) if a != b),
            None,
        )
        raise SystemExit(
            f"{stem} raw link differs: bytes={len(raw)}/{len(expected)} "
            f"first_diff={('-' if first is None else hex(first))}"
        )
    print(f"RAW MATCH {obj.name} bytes={len(raw)}/{len(expected)}")


def producer_hash(member_paths: dict[str, Path]) -> str:
    digest = hashlib.sha256()
    relevant_sections = {
        ".text", ".rel.text", ".data", ".rel.data",
        ".rodata", ".rel.rodata", ".bss",
    }
    for basename in sorted(member_paths):
        candidate = cw.ELFFile(member_paths[basename])
        digest.update(basename.encode("utf-8") + b"\0")
        referenced_symbols: set[int] = set()
        for section in candidate.sections:
            if section.name in relevant_sections:
                raw = (
                    b""
                    if section.name == ".bss"
                    else candidate.data[section.offset:section.offset + section.size]
                )
                digest.update(section.name.encode("utf-8") + b"\0")
                digest.update(struct.pack("<II", section.type, section.size))
                digest.update(raw)
            if section.type not in (4, 9) or not section.entry_size:
                continue
            for index in range(section.size // section.entry_size):
                offset = section.offset + index * section.entry_size
                _relocation_offset, info = struct.unpack_from(
                    candidate.endian + "II", candidate.data, offset
                )
                referenced_symbols.add(info >> 8)
        for index in sorted(referenced_symbols):
            symbol = candidate.symbols[index]
            digest.update(struct.pack("<I", index))
            digest.update(symbol.name.encode("utf-8") + b"\0")
            digest.update(
                struct.pack(
                    "<IIII",
                    symbol.value,
                    symbol.size,
                    symbol.section_index,
                    symbol.info,
                )
            )
    return digest.hexdigest()


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    cc = rm.libgcc.resolve_tool(
        str(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc")
    )
    if run([cc, "-dumpmachine"]).strip() != "ee" or run([cc, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("frontier proof requires EE GCC 3.2.2")
    archive = ROOT / cw.LIBGCC_ARCHIVE
    if not archive.is_file():
        raise SystemExit(f"missing libgcc archive: {archive}")

    prospective = {row[1] for row in cw.DIRECT_LIBGCC_SLICES}
    prospective.update(member for _tag, member, _address, _size, _relocs, _provides in SPECS)
    members = {name: extract(archive, name) for name in prospective}

    for tag, member, address, size, reloc_count, provides in SPECS:
        obj = members[member]
        image, masks, normalized = rm.libgcc.text_image(obj)
        expected = listing_bytes(address, size)
        diff = rm.libgcc.differing_unmasked(expected, image, masks)
        ok = (
            len(image) == size
            and len(masks) == reloc_count
            and diff == 0
            and normalized == rm.libgcc.normalize_target(expected, masks)
        )
        print(
            f"{'MATCH' if ok else 'DIFF':5} {tag:10} {member:12} "
            f"target=0x{address:08x} bytes={len(image)}/{size} "
            f"relocs={len(masks)} diff={diff}"
        )
        if not ok:
            raise SystemExit(f"{tag} normalized proof failed")
        raw_link(cc, obj, address, provides, expected)

    value = producer_hash(members)
    print(f"PROSPECTIVE_LIBGCC_PRODUCER_SHA256={value}")
    print("LIBGCC frontier source gate: 2/2 MATCH")


if __name__ == "__main__":
    main()
