#!/usr/bin/env python3
"""Prove complete historical OBC1 and four S-DD1 mapping helpers.

The complete 1276-byte OBC1 object reproduces its frozen V72 slice; SetOBC1
matches the recorded 1116-byte target digest after resolving real providers.
Four S-DD1 bodies reproduce 340 historical bytes; two additionally match their
complete public target listing (204 bytes). Existing target state is shared.
"""
from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/otherchips-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
WINDOWS = ROOT / "analysis/link_identity/code_windows.json"
LISTING = ROOT / "analysis/functions/progress11_short_targets.asm"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
    "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
    "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)
SPECS = ((1411932, 'GetOBC1', 'obc1', 24, 2, 'f495b07ef146d9f69716dca69b848a0da29af2332f36e6e4ba1d8eeeb85371e3'),
 (1411956, 'SetOBC1', 'obc1', 1116, 58, '2879da3047d79d104c48fada69b1abd4f9e377e4e235ba9a02747210071d7183'),
 (1413072,
  'GetBasePointerOBC1',
  'obc1',
  12,
  2,
  '2397276a2f86e4c91d25b53833ce2a5040d80064e83fa86457e64871aa3ba1c8'),
 (1413084,
  'GetMemPointerOBC1',
  'obc1',
  20,
  2,
  '8df4f38d46406a5c41d2b9070eb7461c6840de9140239b7989ba465de6595e9f'),
 (1413104, 'ResetOBC1', 'obc1', 104, 6, '78ab06de1428bb861d8d7c07545329bd31e19f81c0d33cfadf0124c8aaf42058'),
 (1505712,
  '_Z19S9xSetSDD1MemoryMapjj',
  'sdd1_map',
  104,
  4,
  '999b23f42e89c2b3a1a3fdab9b0179bc6c39429b2047c31a2657e1f0dc757042'),
 (1505816,
  '_Z12S9xResetSDD1v',
  'sdd1_map',
  100,
  7,
  'f5622c03dde2d253178dcfa5a7805abf821a47de4610afafdca01433c7a16fdf'),
 (1505916,
  '_Z20S9xSDD1PostLoadStatev',
  'sdd1_map',
  72,
  4,
  '9094f4582ce11441e6e938a130702cb2509a70dc20324162bd6579b40933a2ac'),
 (1505988,
  '_Z31S9xCompareSDD1LoggedDataEntriesPKvS0_',
  'sdd1_map',
  64,
  0,
  '650efb7c22df5b943b7513b5c8db92567922454309e0327c28b8a1475839924a'))
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


def frozen_slice(selected, address, symbol, size, relocations, sha):
    matches = [s for s in selected if s.get("address") == address and s.get("symbol") == symbol]
    expected = {"size": size, "new_bytes": size, "relocations": relocations,
                "raw_sha256": sha}
    if len(matches) != 1 or any(matches[0].get(k) != v for k, v in expected.items()):
        raise SystemExit(f"frozen historical slice drift: {symbol}")


def check_ledger(relative, rows):
    with (ROOT / relative).open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if rows != frozen:
        raise SystemExit(f"evidence ledger drift: {relative}")



def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    BUILD.mkdir(parents=True, exist_ok=True)
    paths, objects, final = {}, {}, {}
    selected = json.loads(WINDOWS.read_text())["result"]["selected_sources"]
    for unit in ("obc1", "sdd1_map"):
        paths[unit] = BUILD / f"{unit}.o"
        run([CXX, *FLAGS, "-c", ROOT / f"src/snes9x/{unit}.cpp", "-o", paths[unit]])
        objects[unit] = ELFFile(paths[unit])
        functions = [s for s in objects[unit].symbols if s.info & 15 == 2 and s.size]
        specs = [s for s in SPECS if s[2] == unit]
        section = next(s for s in objects[unit].sections if s.name == ".text")
        if ({s.name for s in functions} != {s[1] for s in specs}
                or len(functions) != len(specs) or section.size != sum(s[3] for s in specs)):
            raise SystemExit(f"unproved instructions/functions: {unit}")
        if unit == "obc1":
            matches = [s for s in selected if s.get("name") == "obc1"]
            expected = {"address": 0x00158B5C, "size": 1276, "new_bytes": 1276,
                        "relocations": 70,
                        "raw_sha256": "9160226a00e05ea11df05db25a027608492e3ade2d22ac8c7e3f2da2cf4de5fb"}
            raw = objects[unit].data[section.offset:section.offset + section.size]
            if (len(matches) != 1 or any(matches[0].get(k) != v for k, v in expected.items())
                    or digest(raw) != expected["raw_sha256"]):
                raise SystemExit("frozen complete OBC1 object drift")
        script = BUILD / f"{unit}.target.ld"
        text_address = 0x00158B5C if unit == "obc1" else 0x0016F9B0
        data = (" .rodata 0x001b72f8 : { *(.rodata*) }\n" if unit == "obc1" else "")
        script.write_text("PROVIDE(DAT_0034e2b0 = 0x0034e2b0);\n"
                          "PROVIDE(DAT_003f4be8 = 0x003f4be8);\n"
                          "PROVIDE(memset = 0x0019c39c);\nSECTIONS {\n"
                          + f" .text {text_address:#x} : {{ *(.text) }}\n" + data
                          + " /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.rodata*) *(.reginfo)"
                          + " *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
        output = BUILD / f"{unit}.target.elf"
        run([CXX.with_name("ee-ld"), "-EL", "-T", script, paths[unit], "-o", output])
        final[unit] = ELFFile(output)
    words = {}
    for line in LISTING.read_text().splitlines():
        match = LINE.match(line)
        if match:
            address = int(match[1], 16)
            if address in words:
                raise SystemExit(f"duplicate target instruction: {address:#x}")
            words[address] = bytes(int(match[i], 16) for i in range(2, 6))
    rows = []
    cursor = {"obc1": 0, "sdd1_map": 0}
    for address, name, unit, size, relocations, sha in SPECS:
        elf = objects[unit]
        symbol = elf.find_symbol(name)
        if (symbol.value != cursor[unit] or symbol.size != size
                or len(elf.relocation_ranges(symbol, size)) != relocations
                or digest(elf.symbol_bytes(symbol, size)) != sha):
            raise SystemExit(f"historical object drift: {name}")
        normalized(elf, symbol)
        if unit == "sdd1_map":
            frozen_slice(selected, address, name, size, relocations, sha)
        cursor[unit] += size
        linked_sha, level, evidence = "", "historical-object", "analysis/link_identity/code_windows.json"
        actual_symbol = final[unit].find_symbol(name)
        if actual_symbol.value != address or actual_symbol.size != size:
            raise SystemExit(f"target placement drift: {name}")
        actual = final[unit].symbol_bytes(actual_symbol, size)
        if name == "SetOBC1":
            historical = "analysis/matching/hunt1041-v72-validated-v53-6.tsv"
            with (ROOT / historical).open(newline="") as stream:
                pinned = [r for r in csv.DictReader(stream, delimiter="\t") if r["object_symbol"] == name]
            expected_sha = "71cfce00cddb6a7597b1b81210ba754ce65f94e6442eb30959becaeba8932b24"
            if (len(pinned) != 1 or pinned[0]["address"] != f"0x{address:08x}"
                    or pinned[0]["object_size"] != str(size) or pinned[0]["relocation_count"] != str(relocations)
                    or pinned[0]["result"] != "MATCH" or pinned[0]["differing_bytes"] != "0"
                    or pinned[0]["target_span_sha256"] != expected_sha or digest(actual) != expected_sha):
                raise SystemExit("provider-linked OBC1 target digest mismatch")
            linked_sha, level, evidence = expected_sha, "provider-linked", historical
        elif unit == "sdd1_map" and address in words:
            try:
                target = b"".join(words[a] for a in range(address, address + size, 4))
            except KeyError as missing:
                raise SystemExit(f"incomplete S-DD1 listing: {missing}")
            if actual != target:
                raise SystemExit(f"provider-linked S-DD1 target mismatch: {name}")
            linked_sha, level, evidence = digest(actual), "provider-linked", LISTING.relative_to(ROOT).as_posix()
        rows.append({"address": f"0x{address:08x}", "symbol": name,
                     "source_file": f"src/snes9x/{unit}.cpp", "size": str(size),
                     "relocations": str(relocations), "raw_sha256": sha,
                     "linked_sha256": linked_sha, "proof_level": level, "evidence": evidence})
        print(f"other chips {level}: MATCH {name} bytes={size}/{size} relocations={relocations}")
    check_ledger("analysis/functions/otherchips_exact_1616.tsv", rows)
    (BUILD / "report.json").write_text(json.dumps(rows, indent=2) + "\n")
    print("other chips source: MATCH functions=9/9 bytes=1616/1616; provider-linked=1320/1320 bytes")


if __name__ == "__main__":
    main()
