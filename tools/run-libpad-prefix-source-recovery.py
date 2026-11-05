#!/usr/bin/env python3
"""Focused modern proof for the exact ps2lib-era NEW_PADMAN prefix."""
from __future__ import annotations

import hashlib
import os
import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile, compare_function

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "libpad-prefix-source-recovery"
SOURCE = ROOT / "matching/candidates/progress68/libpad-ps2lib-2004-newpadman.c.txt"
SOURCE_SHA256 = "4d96a31c5f8d3f814f3bec872fadf6afd9a4cb67c12705053a9073ae4f8e76d1"
TARGET_BASE = 0x00100000
LISTING = ROOT / "analysis/functions/libpad_001a8420.asm"
ZERO_RANGES = ROOT / "analysis/matching/progress68-inferred-zero-ranges.tsv"
PS2SDK_REPO = "https://github.com/ps2dev/ps2sdk.git"
PS2SDK_COMMIT = "01d625018c3fde3044292446c910b8ea508adfdc"
PS2DEV_REPO = "https://github.com/duduclx/PS2DEV.git"
PS2DEV_COMMIT = "bac0006c6302edcf1bdae253799484497b4e5032"

SPECS = (
    ("padGetDmaStr", 0x001A8420, 100),
    ("padInit", 0x001A8484, 400),
    ("padEnd", 0x001A8614, 124),
    ("padPortOpen", 0x001A8690, 288),
    ("padPortClose", 0x001A87B0, 180),
    ("padRead", 0x001A8864, 68),
)

INSN = re.compile(
    r"^\s*([0-9A-Fa-f]+):\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
)


def run(cmd: list[str | Path], *, cwd: Path = ROOT) -> str:
    cp = subprocess.run(
        [str(x) for x in cmd], cwd=cwd, text=True,
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    if cp.returncode:
        raise SystemExit(cp.stdout[-8000:])
    return cp.stdout


def checkout(path: Path, repo: str, commit: str, sparse_lines: str) -> None:
    if not (path / ".git").exists():
        path.mkdir(parents=True, exist_ok=True)
        run(["git", "init", "-q", path])
        run(["git", "-C", path, "remote", "add", "origin", repo])
    got = subprocess.run(
        ["git", "-C", str(path), "rev-parse", "HEAD"],
        text=True, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL,
    ).stdout.strip()
    if got == commit:
        return
    run(["git", "-C", path, "config", "core.sparseCheckout", "true"])
    sparse = path / ".git/info/sparse-checkout"
    sparse.parent.mkdir(parents=True, exist_ok=True)
    sparse.write_text(sparse_lines, encoding="utf-8")
    run(["git", "-C", path, "fetch", "-q", "--depth=1", "origin", commit])
    run(["git", "-C", path, "checkout", "-q", "--detach", "FETCH_HEAD"])
    run(["git", "-C", path, "read-tree", "-mu", "HEAD"])
    got = run(["git", "-C", path, "rev-parse", "HEAD"]).strip()
    if got != commit:
        raise SystemExit(f"checkout drift: wanted {commit}, got {got}")


def target_image() -> tuple[bytes, set[int]]:
    byte_map: dict[int, int] = {}
    for line in LISTING.read_text(encoding="utf-8", errors="replace").splitlines():
        m = INSN.match(line)
        if not m:
            continue
        address = int(m.group(1), 16)
        raw = bytes(int(m.group(i), 16) for i in range(2, 6))
        for offset, value in enumerate(raw):
            byte_map[address + offset] = value

    rows = ZERO_RANGES.read_text(encoding="utf-8").splitlines()[1:]
    for row in rows:
        if not row.strip():
            continue
        start_s, end_s, _size, _source = row.split("\t", 3)
        start, end = int(start_s, 16), int(end_s, 16)
        for address in range(start, end):
            old = byte_map.get(address)
            if old not in (None, 0):
                raise SystemExit(f"zero-gap conflict @ 0x{address:08x}")
            byte_map[address] = 0

    end = max(byte_map) + 1
    image = bytearray(end - TARGET_BASE)
    for address, value in byte_map.items():
        image[address - TARGET_BASE] = value
    return bytes(image), set(byte_map)


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest() != SOURCE_SHA256:
        raise SystemExit("frozen libpad source identity drift")

    cc = Path(os.environ.get(
        "EE_CC",
        ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc",
    ))
    if not cc.is_file():
        raise SystemExit(f"missing historical compiler: {cc}")
    if run([cc, "-dumpmachine"]).strip() != "ee" or run([cc, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("libpad prefix proof requires EE GCC 3.2.2")

    ps2sdk = BUILD / "ps2sdk-20040415"
    ps2dev = BUILD / "PS2DEV-bac0006c"
    checkout(ps2sdk, PS2SDK_REPO, PS2SDK_COMMIT, "ee/\ncommon/\n")
    checkout(ps2dev, PS2DEV_REPO, PS2DEV_COMMIT, "ps2sdk/ee/\nps2sdk/common/\n")

    includes: list[str] = []
    pad_inc = ps2sdk / "ee/rpc/pad/include"
    if pad_inc.is_dir():
        includes += ["-I", str(pad_inc)]
    roots = sorted({p for p in ps2sdk.rglob("include") if p.is_dir()})
    roots += sorted({p for p in (ps2dev / "ps2sdk").rglob("include") if p.is_dir()})
    seen: set[Path] = set()
    for inc in roots:
        if inc in seen:
            continue
        seen.add(inc)
        includes += ["-I", str(inc)]

    obj = BUILD / "libpad-ps2lib-2004-newpadman-os.o"
    flags = [
        "-G0", "-EL", "-pipe", "-w",
        "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common",
        "-fshort-double", "-mlong64", "-mhard-float", "-mno-abicalls",
        "-march=r5900", "-mtune=r5900",
        "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD",
        "-DCODE_PLATFORM=3", "-Os",
    ]
    run([cc, *flags, *includes, "-x", "c", "-c", SOURCE, "-o", obj])

    elf = ELFFile(obj)
    sections = {section.name: section for section in elf.sections}
    print("=== libpad prefix historical-source proof ===")
    print(f"source_sha256={SOURCE_SHA256}")
    print(f"compiler={run([cc, '-dumpmachine']).strip()} gcc {run([cc, '-dumpversion']).strip()}")
    print("sections=" + ",".join(
        f"{name}:{sections[name].size}"
        for name in (".text", ".rel.text", ".data", ".rodata", ".bss")
        if name in sections
    ))
    for name in (".text", ".rel.text", ".data", ".rodata"):
        section = sections.get(name)
        if section is not None:
            raw = elf.data[section.offset:section.offset + section.size]
            print(f"{name}_sha256={hashlib.sha256(raw).hexdigest()}")

    target, known = target_image()
    pad_init_ok = False
    for symbol_name, address, expected_size in SPECS:
        sym = elf.find_symbol(symbol_name)
        if sym.size != expected_size:
            print(
                f"DIFF  {symbol_name:16} target=0x{address:08x} "
                f"obj_off=0x{sym.value:x} size={sym.size}/{expected_size} reason=size"
            )
            continue
        missing = [a for a in range(address, address + expected_size) if a not in known]
        if missing:
            print(
                f"MISS  {symbol_name:16} target=0x{address:08x} "
                f"obj_off=0x{sym.value:x} size={sym.size} first_missing=0x{missing[0]:08x}"
            )
            continue
        comp = compare_function(
            target, address - TARGET_BASE, expected_size, elf, symbol_name
        )
        relocs = elf.relocation_masks(sym, 4)
        ok = comp.matching and not comp.unknown_relocation_types
        print(
            f"{'MATCH' if ok else 'DIFF':5} {symbol_name:16} "
            f"target=0x{address:08x} obj_off=0x{sym.value:x} "
            f"size={sym.size} relocs={len(relocs)} diff={comp.differing_bytes} "
            f"raw={comp.raw_equal} normalized={comp.normalized_equal}"
        )
        if symbol_name == "padInit":
            pad_init_ok = ok

    if not pad_init_ok:
        raise SystemExit("libpad prefix gate: padInit exact proof failed")
    print("libpad prefix gate: padInit MATCH; peer diagnostics complete")


if __name__ == "__main__":
    main()
