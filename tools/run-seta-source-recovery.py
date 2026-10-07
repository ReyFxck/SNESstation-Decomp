#!/usr/bin/env python3
"""Prove two SETA dispatchers and three original ST010 helpers.

All 440 instruction bytes reproduce the hash-pinned historical modules after
masking only known relocation fields. The complete 248-byte rotation also
matches the public target listing after resolving its real math providers.
The 68-byte getter is outside the frozen 1041-row audit. The ST010 command
writer is not covered by this isolated recovery.
"""
from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/seta-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/functions/seta_helpers_exact_440.tsv"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900", "-Os",
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES", "-DCPU_SHUTDOWN",
    "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE", "-DSPC700_C",
    "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)
LINE = re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} ){3}[0-9a-fA-F]{2})\s")


SPECS = (('0x0016fc48',
  'S9xGetSetaDSP',
  'src/snes9x/seta_dispatch.cpp',
  '36',
  '2',
  '21ce794080b768ac6bed9200a5e555a8d9ef201ae5500e2c20dc7801c01c1228',
  '21ce794080b768ac6bed9200a5e555a8d9ef201ae5500e2c20dc7801c01c1228',
  '21ce794080b768ac6bed9200a5e555a8d9ef201ae5500e2c20dc7801c01c1228',
  '',
  'historical-object',
  'analysis/link_identity/code_windows.json'),
 ('0x0016fc6c',
  'S9xSetSetaDSP',
  'src/snes9x/seta_dispatch.cpp',
  '36',
  '2',
  '4b5a4cd5c6f782eb0e7b87523c894533b7d04cee70e764fb6572031ebea0084a',
  '4b5a4cd5c6f782eb0e7b87523c894533b7d04cee70e764fb6572031ebea0084a',
  '4b5a4cd5c6f782eb0e7b87523c894533b7d04cee70e764fb6572031ebea0084a',
  '',
  'historical-object',
  'analysis/link_identity/code_windows.json'),
 ('0x0016fc90',
  'S9xGetST010',
  'src/snes9x/st010_helpers.cpp',
  '68',
  '5',
  'ccbf004f574a42c98a7e7c60eca2ec4c1234f9e59b668f4c5477c0436824f3d5',
  '9fd5a33531822f9e2f68d4dda008d5c771eaf34f18ac0986a0baa0ba43b94496',
  'ac43d597d588b3112cc1597f67824a2acd99cd7ea7c86337cd13f10dae16059c',
  '',
  'historical-object',
  'analysis/link_identity/code_windows.json'),
 ('0x0016fcd4',
  '_Z10St010_Op03sssRiS_',
  'src/snes9x/st010_helpers.cpp',
  '52',
  '0',
  'ad57583ed4c3604fda12b9f3e76b8e34171fcc6a23891f1ae460c28fd10053e3',
  'ad57583ed4c3604fda12b9f3e76b8e34171fcc6a23891f1ae460c28fd10053e3',
  'ad57583ed4c3604fda12b9f3e76b8e34171fcc6a23891f1ae460c28fd10053e3',
  '',
  'historical-object',
  'analysis/link_identity/code_windows.json'),
 ('0x0016fd08',
  '_Z12St010_RotatesssRsS_',
  'src/snes9x/st010_helpers.cpp',
  '248',
  '4',
  '719abf5efa48b96b168a74b351c90fd1c422004b1661b32c90fec7e6572d3343',
  '719abf5efa48b96b168a74b351c90fd1c422004b1661b32c90fec7e6572d3343',
  '719abf5efa48b96b168a74b351c90fd1c422004b1661b32c90fec7e6572d3343',
  'a2a019df326ee1338ea90ed6a0425708f9a16ad838d8be5713a137633bd10005',
  'provider-linked',
  'analysis/functions/progress13_targets.asm'))
LISTING_SHA = 'c5100130cce74c3da0edffa651cbd827c3bc53079e268937ac7e81c701f60a9e'

def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command))
                         + "\n" + result.stdout[-12000:])
    return result.stdout


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def normalized(elf, symbol):
    raw = bytearray(elf.symbol_bytes(symbol, symbol.size))
    for relocation in elf.relocation_masks(symbol, symbol.size):
        if not relocation.known:
            raise SystemExit(f"unknown relocation: {symbol.name}")
        for index, mask in enumerate(relocation.mask_bytes):
            raw[relocation.start + index] &= 255 ^ mask
    return bytes(raw)


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1000plus_v47_closure as historical

    archive = historical.download_archive(historical.SNES_141_1_ARCHIVE, historical.SNES_CACHE)
    source_root = historical.safe_extract_archive(archive, historical.SNES_CACHE / "source-1.41-1",
                                                  historical.SNES_141_1_ARCHIVE.source_directory)
    historical.ensure_git_commit(historical.PS2DEV, historical.PS2DEV_REPO, historical.PS2DEV_COMMIT)
    upstream = source_root / "snes9x"
    newlib = historical.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    BUILD.mkdir(parents=True, exist_ok=True)
    compat = BUILD / "compat"
    compat.mkdir(exist_ok=True)
    (compat / "memory.h").write_text("#include <string.h>\n")
    originals = {}
    for unit in ("seta", "seta010"):
        obj = BUILD / f"historical-{unit}.o"
        run([CXX, *FLAGS, "-I", compat, "-I", newlib, "-I", upstream,
             "-c", upstream / f"{unit}.cpp", "-o", obj])
        originals[unit] = ELFFile(obj)
    selected = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    for unit, capture, address, size, relocations, sha in (
        ("seta", "seta", 0x0016FC48, 72, 4, "c8a69e699e58af2ab50fdb8dbbe6d2a2c177678c66d202f5f3a097973d8c410f"),
        ("seta010", "seta010_prefix", 0x0016FC90, 880, 26, "6131ab73fa5356af3d675bdd46cd6d98d2ed0708f3dcd6d4ddaa6efdd5a3bbb2"),
    ):
        captures = [s for s in selected if s.get("name") == capture]
        expected = {"address": address, "size": size, "new_bytes": size,
                    "source_offset": 0, "relocations": relocations, "raw_sha256": sha}
        elf = originals[unit]
        text = next(s for s in elf.sections if s.name == ".text")
        if (len(captures) != 1 or any(captures[0].get(k) != v for k, v in expected.items())
                or digest(elf.data[text.offset:text.offset + size]) != sha):
            raise SystemExit(f"frozen historical SETA/ST010 window drift: {unit}")
    objects = {}
    for unit in ("seta_dispatch", "st010_helpers"):
        obj = BUILD / f"{unit}.o"
        run([CXX, *FLAGS, "-c", ROOT / f"src/snes9x/{unit}.cpp", "-o", obj])
        elf = objects[unit] = ELFFile(obj)
        functions = [s for s in elf.symbols if s.info & 15 == 2 and s.size]
        specs = [r for r in SPECS if r[2] == f"src/snes9x/{unit}.cpp"]
        text = next(s for s in elf.sections if s.name == ".text")
        if (len(functions) != len(specs) or {s.name for s in functions} != {r[1] for r in specs}
                or text.size != sum(int(r[3]) for r in specs)):
            raise SystemExit(f"unproved SETA/ST010 functions/instructions: {unit}")
    listing = ROOT / "analysis/functions/progress13_targets.asm"
    if digest(listing.read_bytes()) != LISTING_SHA:
        raise SystemExit("public ST010 rotation listing drift")
    words = {}
    for line in listing.read_text().splitlines():
        match = LINE.match(line)
        if match:
            address = int(match[1], 16)
            if address in words:
                raise SystemExit("duplicate target instruction")
            words[address] = bytes.fromhex(match[2])
    target = b"".join(words[a] for a in range(0x0016FD08, 0x0016FE00, 4))
    script = BUILD / "st010_helpers.target.ld"
    script.write_text("PROVIDE(DAT_0034e2b0 = 0x0034e2b0);\n"
                      "PROVIDE(sinf_001a0024 = 0x001a0024);\n"
                      "PROVIDE(cosf_0019fddc = 0x0019fddc);\n"
                      "PROVIDE(printf = 0x0019e388);\n"
                      "SECTIONS { .text 0x0016fc90 : { *(.text) }\n"
                      " .rodata 0x001b8058 : { *(.rodata*) }\n"
                      " /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)"
                      " *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    output = BUILD / "st010_helpers.target.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, BUILD / "st010_helpers.o", "-o", output])
    final = ELFFile(output)
    rows = []
    for spec in SPECS:
        (address, name, source, size_text, relocation_text, historical_sha,
         isolated_sha, normalized_sha, linked_sha, level, evidence) = spec
        unit = Path(source).stem
        old = originals["seta" if unit == "seta_dispatch" else "seta010"]
        local = objects[unit]
        original_symbol, symbol = old.find_symbol(name), local.find_symbol(name)
        size, relocations = int(size_text), int(relocation_text)
        if (original_symbol.size != size or symbol.size != size
                or len(old.relocation_ranges(original_symbol, size)) != relocations
                or len(local.relocation_ranges(symbol, size)) != relocations
                or digest(old.symbol_bytes(original_symbol, size)) != historical_sha
                or digest(local.symbol_bytes(symbol, size)) != isolated_sha
                or digest(normalized(old, original_symbol)) != normalized_sha
                or digest(normalized(local, symbol)) != normalized_sha):
            raise SystemExit(f"SETA/ST010 historical instruction drift: {name}")
        if linked_sha:
            placed = final.find_symbol(name)
            if (placed.value != int(address, 0) or placed.size != size
                    or final.symbol_bytes(placed, size) != target or digest(target) != linked_sha):
                raise SystemExit("provider-linked ST010 rotation drift")
        rows.append(dict(zip(("address", "symbol", "source_file", "size", "relocations",
                              "historical_raw_sha256", "isolated_raw_sha256", "normalized_sha256",
                              "linked_sha256", "proof_level", "evidence"), spec)))
        print(f"SETA/ST010: MATCH {name} bytes={size}/{size} proof={level}")
    with EVIDENCE.open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if rows != frozen:
        raise SystemExit("SETA/ST010 ledger drift")
    (BUILD / "report.json").write_text(json.dumps(rows, indent=2) + "\n")
    print("SETA/ST010: MATCH functions=5/5 historical_bytes=440/440 provider_linked_bytes=248/248")


if __name__ == "__main__":
    main()
