#!/usr/bin/env python3
"""Prove the byte-exact get_tree reconstruction with target relocations resolved."""
from __future__ import annotations

import hashlib
import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/get-tree-source-recovery"
LOCAL = ROOT / "matching/candidates/get_tree.S"
LISTING = ROOT / "analysis/functions/unzip_explode_0018c124.asm"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
LD = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-ld"

ADDRESS = 0x0018C124
END = 0x0018C1F8
SIZE = END - ADDRESS
READBYTE = 0x0018DC60
BYTEBUF = 0x00426E28
TARGET_SHA256 = "b4c2ff8e8d0f56d131afe79349fa26863a75a030fbf56692415c529bb0a2a08c"
EXPECTED_RELOCS = 9
LINE_RE = re.compile(r"^\s*([0-9A-Fa-f]+):\s+((?:[0-9A-Fa-f]{2}\s+){4})")

def run(cmd):
    cp = subprocess.run([str(x) for x in cmd], cwd=ROOT, text=True,
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, cmd)) + "\n" + cp.stdout[-12000:])
    return cp.stdout

def target_bytes() -> bytes:
    image = bytearray(SIZE)
    seen = set()
    for line in LISTING.read_text(encoding="utf-8").splitlines():
        match = LINE_RE.match(line)
        if not match:
            continue
        address = int(match.group(1), 16)
        if not ADDRESS <= address < END:
            continue
        raw = bytes(int(x, 16) for x in match.group(2).split())
        offset = address - ADDRESS
        if offset + 4 > SIZE or offset in seen:
            raise SystemExit("invalid or duplicate committed listing row")
        image[offset:offset + 4] = raw
        seen.add(offset)
    if not seen or min(seen) != 0 or max(seen) != SIZE - 4:
        raise SystemExit("committed get_tree listing does not cover both boundaries")
    raw = bytes(image)
    digest = hashlib.sha256(raw).hexdigest()
    if digest != TARGET_SHA256:
        raise SystemExit(f"committed target slice drift: {digest}")
    return raw

def main():
    BUILD.mkdir(parents=True, exist_ok=True)
    if run([CC, "-dumpmachine"]).strip() != "ee" or run([CC, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")

    target = target_bytes()
    obj = BUILD / "get_tree.o"
    run([CC, "-G0", "-EL", "-pipe", "-w", "-c", LOCAL, "-o", obj])

    elf = ELFFile(obj)
    sym = elf.find_symbol("get_tree_candidate")
    raw = elf.symbol_bytes(sym, sym.size)
    relocs = len(elf.relocation_ranges(sym, sym.size))
    if sym.size != SIZE or relocs != EXPECTED_RELOCS:
        raise SystemExit(f"relocatable gate drift size={sym.size} relocs={relocs}")

    linked = BUILD / "get_tree-linked.elf"
    run([
        LD, "-EL", "-Ttext", f"0x{ADDRESS:08x}",
        f"--defsym=ReadByte=0x{READBYTE:08x}",
        f"--defsym=bytebuf=0x{BYTEBUF:08x}",
        "-o", linked, obj,
    ])
    linked_elf = ELFFile(linked)
    linked_sym = linked_elf.find_symbol("get_tree_candidate")
    linked_raw = linked_elf.symbol_bytes(linked_sym, linked_sym.size)
    if linked_sym.value != ADDRESS or linked_sym.size != SIZE:
        raise SystemExit(
            f"linked symbol drift value=0x{linked_sym.value:08x} size={linked_sym.size}"
        )
    if linked_raw != target:
        first = next(i for i, (a, b) in enumerate(zip(linked_raw, target)) if a != b)
        raise SystemExit(f"linked get_tree differs from target at +0x{first:x}")

    digest = hashlib.sha256(linked_raw).hexdigest()
    print(
        f"GET_TREE relocatable: MATCH bytes={SIZE}/{SIZE} relocations={relocs} "
        f"target_sha256={TARGET_SHA256}"
    )
    print(
        f"GET_TREE linked: MATCH bytes={SIZE}/{SIZE} ReadByte=0x{READBYTE:08x} "
        f"bytebuf=0x{BYTEBUF:08x} raw_sha256={digest}"
    )

if __name__ == "__main__":
    main()
