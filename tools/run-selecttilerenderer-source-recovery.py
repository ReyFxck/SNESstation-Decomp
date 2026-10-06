#!/usr/bin/env python3
"""Prove the historical renderer selector against all provider-linked bytes."""
from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/selecttilerenderer-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
SOURCE = ROOT / "src/snes9x/selecttilerenderer.cpp"
LISTING = ROOT / "analysis/functions/core_getset_apumem_001ab3c0.asm"
EVIDENCE = ROOT / "analysis/functions/selecttilerenderer_exact_348.tsv"
ADDRESS, SIZE, RELOCATIONS = 0x001AC838, 348, 66
SYMBOL = "_Z18SelectTileRendererh"
RAW_SHA256 = "f9f748e190db92302f76e2fb545680b7eb6fcb38c534f32a9efc6071b772954d"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
    "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
    "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)
PROVIDERS = {
    "DAT_0035d480": 0x0035D480,
    "DAT_0035f98c": 0x0035F98C,
    "DAT_0035f990": 0x0035F990,
    "DAT_0035f99c": 0x0035F99C,
    "DrawTile16_recovered": 0x00185D8C,
    "DrawClippedTile16_recovered": 0x001860A8,
    "DrawLargePixel16_recovered": 0x001874A8,
    "DrawTile16Add_recovered": 0x0018789C,
    "DrawClippedTile16Add_recovered": 0x00187BB8,
    "DrawLargePixel16Add_recovered": 0x0018A6D4,
    "DrawTile16Add1_2_recovered": 0x00188050,
    "DrawClippedTile16Add1_2_recovered": 0x0018836C,
    "DrawLargePixel16Add1_2_recovered": 0x0018ADB8,
    "DrawTile16Sub_recovered": 0x00188804,
    "DrawClippedTile16Sub_recovered": 0x00188B20,
    "DrawLargePixel16Sub_recovered": 0x0018B43C,
    "DrawTile16Sub1_2_recovered": 0x00188FB8,
    "DrawClippedTile16Sub1_2_recovered": 0x001892D4,
    "DrawLargePixel16Sub1_2_recovered": 0x0018BAC0,
    "DrawTile16FixedAdd1_2_recovered": 0x0018976C,
    "DrawClippedTile16FixedAdd1_2_recovered": 0x00189A88,
    "DrawTile16FixedSub1_2_recovered": 0x00189F20,
    "DrawClippedTile16FixedSub1_2_recovered": 0x0018A23C,
}
LINE = re.compile(r"^\s*([0-9a-fA-F]+):\s+"
                  r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+"
                  r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s")


def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command))
                         + "\n" + result.stdout[-12000:])
    return result.stdout


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    selected = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    matches = [s for s in selected if s.get("address") == ADDRESS and s.get("symbol") == SYMBOL]
    expected_slice = {"size": SIZE, "new_bytes": SIZE,
                      "relocations": RELOCATIONS, "raw_sha256": RAW_SHA256}
    if len(matches) != 1 or any(matches[0].get(k) != v for k, v in expected_slice.items()):
        raise SystemExit("frozen historical slice drift")
    words = {}
    for line in LISTING.read_text().splitlines():
        match = LINE.match(line)
        if match and ADDRESS <= int(match[1], 16) < ADDRESS + SIZE:
            address = int(match[1], 16)
            if address in words:
                raise SystemExit(f"duplicate target instruction: {address:#x}")
            words[address] = bytes(int(match[i], 16) for i in range(2, 6))
    if sorted(words) != list(range(ADDRESS, ADDRESS + SIZE, 4)):
        raise SystemExit("incomplete target listing")
    target = b"".join(words[a] for a in sorted(words))
    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "selecttilerenderer.o"
    run([CXX, *FLAGS, "-c", SOURCE, "-o", obj])
    elf = ELFFile(obj)
    symbol = elf.find_symbol(SYMBOL)
    if (symbol.size != SIZE or len(elf.relocation_ranges(symbol, SIZE)) != RELOCATIONS
            or hashlib.sha256(elf.symbol_bytes(symbol, SIZE)).hexdigest() != RAW_SHA256):
        raise SystemExit("historical object drift")
    script = BUILD / "selector.target.ld"
    script.write_text("".join(f"PROVIDE({name} = {value:#x});\n" for name, value in PROVIDERS.items())
                      + "SECTIONS {\n . = 0x001ac838;\n .text : { *(.text) }\n"
                      + " /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)"
                      + " *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) *(.rodata*) }\n}\n")
    linked = BUILD / "selector.target.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked])
    final = ELFFile(linked)
    symbol = final.find_symbol(SYMBOL)
    actual = final.symbol_bytes(symbol, SIZE)
    if symbol.value != ADDRESS or actual != target:
        raise SystemExit("provider-linked target mismatch")
    digest = hashlib.sha256(actual).hexdigest()
    with EVIDENCE.open(newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    expected = {"address": f"0x{ADDRESS:08x}", "symbol": SYMBOL, "size": str(SIZE),
                "relocations": str(RELOCATIONS), "raw_sha256": RAW_SHA256,
                "linked_sha256": digest, "evidence": LISTING.relative_to(ROOT).as_posix()}
    if rows != [expected]:
        raise SystemExit("selector evidence ledger drift")
    (BUILD / "report.json").write_text(json.dumps(expected, indent=2) + "\n")
    print("SelectTileRenderer provider-linked: MATCH bytes=348/348 relocations=66")


if __name__ == "__main__":
    main()
