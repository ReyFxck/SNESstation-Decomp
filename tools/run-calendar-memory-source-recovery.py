#!/usr/bin/env python3
"""Prove historical GetBasePointer and RTC/S-RTC bodies with real providers."""
from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/calendar-memory-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/functions/calendar_memory_exact_2300.tsv"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
    "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
    "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)
# address, symbol, source unit, bytes, relocations, frozen full-TU object hash
SPECS = (
    (0x001825E4, "_Z17S9xRTCDaysInMonthii", "rtc_days", 84, 2,
     "d19b93e455c1f554c661d21eb86051dc4e2cf6566ef769ed70be9598895eb7c0"),
    (0x00183660, "_Z12S9xResetSRTCv", "srtc", 24, 2,
     "5991e79f14936b36b8a0120e9da2de9a5414812184dfa8368633c95c6b8cf340"),
    (0x00183678, "_Z16S9xHardResetSRTCv", "srtc", 88, 4,
     "d4cb8cc7f580ffe1b074fbc1e7540066be39d904697485b88d7685b95d2fcab6"),
    (0x001836D0, "_Z23S9xSRTCComputeDayOfWeekv", "srtc", 168, 4,
     "7acd92085baeb8f3c47aea0a961cdc20dfc5d5704111665d69d911d0143dfc2e"),
    (0x00183778, "_Z19S9xSRTCDaysInMmonthii", "srtc", 84, 2,
     "f733a4ca4ace3625f6f090f4a137d3d2c7f1430de3c94ab867a4f1595486b169"),
    (0x001837CC, "_Z17S9xUpdateSrtcTimev", "srtc", 728, 14,
     "53a2ee3f5d1f6c64dc5d55d414899cbf942379372e524c165fa038ca4f35a1bf"),
    (0x00183AA4, "_Z10S9xSetSRTCht", "srtc", 312, 9,
     "cd12e821da5f6cb9978f3b1e8f2e2d60e5435b7291bbce08c1ab79fb7dcd315e"),
    (0x00183BDC, "_Z10S9xGetSRTCt", "srtc", 124, 3,
     "212fe0e430246c3bfdd63559ca70275b4a0c56b681ece8b5a68f15be753f5b04"),
    (0x00183C58, "_Z19S9xSRTCPreSaveStatev", "srtc", 232, 17,
     "e1e643a9244209a550222a0375ddeeb6f491e88227db9d9a7af3827527f8ad55"),
    (0x00183D40, "_Z20S9xSRTCPostLoadStatev", "srtc", 196, 13,
     "0e4324142d0d9705a662ffd2aef822f9834b6b5f7a31053a7a94a07a9cca2e4f"),
    (0x001AC734, "_Z14GetBasePointerj", "getbasepointer", 260, 22,
     "d2fbfd3e4ac3152007df1d1ed56b3b9f43426eece600cd1f22597ffc325e5f5f"),
)
LISTINGS = (
    "analysis/functions/core_getset_apumem_001ab3c0.asm",
    "analysis/functions/tile_init_and_helpers_00183000.asm",
    "analysis/functions/progress13_targets.asm",
)
LINE = re.compile(r"^\s*([0-9a-fA-F]+):\s+"
                  r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+"
                  r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s")
PROVIDERS = {
    "DAT_0034e2b0": 0x0034E2B0, "DAT_003454e0": 0x003454E0,
    "DAT_00413544": 0x00413544, "DAT_0034e298": 0x0034E298,
    "DAT_00423858": 0x00423858, "snes_p11_00182910": 0x00182910,
    "snes_leaf_00158fd0": 0x00158FD0, "memset": 0x0019C39C,
    "memmove": 0x0019C4A0, "__divdi3": 0x001A1DB8,
}


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
    words, origins = {}, {}
    for relative in LISTINGS:
        for line in (ROOT / relative).read_text().splitlines():
            match = LINE.match(line)
            if match:
                address = int(match[1], 16)
                word = bytes(int(match[i], 16) for i in range(2, 6))
                if address in words and words[address] != word:
                    raise SystemExit(f"conflicting public target instruction: {address:#x}")
                if address not in words:
                    origins[address] = relative
                words[address] = word
    BUILD.mkdir(parents=True, exist_ok=True)
    objects, paths = {}, {}
    for unit in ("getbasepointer", "rtc_days", "srtc"):
        paths[unit] = BUILD / f"{unit}.o"
        run([CXX, *FLAGS, "-c", ROOT / f"src/snes9x/{unit}.cpp", "-o", paths[unit]])
        objects[unit] = ELFFile(paths[unit])
    script = BUILD / "calendar-memory.target.ld"
    placements = (
        (".text.days", 0x001825E4, "rtc_days", ".text"),
        (".text.srtc", 0x00183660, "srtc", ".text"),
        (".text.base", 0x001AC734, "getbasepointer", ".text"),
        (".rodata.base", 0x001B2030, "getbasepointer", ".rodata*"),
        (".rodata.days", 0x001B87EC, "rtc_days", ".rodata*"),
        (".rodata.srtc", 0x001B8878, "srtc", ".rodata*"),
        (".data.keys", 0x00423878, "srtc", ".data"),
    )
    script.write_text("".join(f"PROVIDE({name} = {value:#x});\n" for name, value in PROVIDERS.items())
                      + "SECTIONS {\n"
                      + "".join(f' {section} {address:#x} : {{ "{paths[unit]}"({input_section}) }}\n'
                                for section, address, unit, input_section in placements)
                      + " /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)"
                      + " *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) }\n}\n")
    linked = BUILD / "calendar-memory.target.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, *paths.values(), "-o", linked])
    final = ELFFile(linked)
    rows = []
    for address, name, unit, size, relocations, historical_sha in SPECS:
        matches = [s for s in selected if s.get("address") == address and s.get("symbol") == name]
        expected = {"size": size, "new_bytes": size, "relocations": relocations,
                    "raw_sha256": historical_sha}
        if len(matches) != 1 or any(matches[0].get(k) != v for k, v in expected.items()):
            raise SystemExit(f"frozen historical slice drift: {name}")
        elf = objects[unit]
        symbol = elf.find_symbol(name)
        if (symbol.size != size or len(elf.relocation_ranges(symbol, size)) != relocations
                or any(not r.known for r in elf.relocation_masks(symbol, size))):
            raise SystemExit(f"object size/relocation drift: {name}")
        try:
            target = b"".join(words[a] for a in range(address, address + size, 4))
        except KeyError as missing:
            raise SystemExit(f"incomplete public target listing: {missing}")
        actual_symbol = final.find_symbol(name)
        actual = final.symbol_bytes(actual_symbol, actual_symbol.size)
        if actual_symbol.value != address or actual != target:
            raise SystemExit(f"provider-linked target mismatch: {name}")
        # Isolation changes full-TU table offsets and relocation addends.
        # Preserve both hashes and prove every final byte after real linkage.
        rows.append({"address": f"0x{address:08x}", "symbol": name,
                     "source_file": f"src/snes9x/{unit}.cpp", "size": str(size),
                     "relocations": str(relocations), "historical_raw_sha256": historical_sha,
                     "isolated_raw_sha256": hashlib.sha256(elf.symbol_bytes(symbol, size)).hexdigest(),
                     "linked_sha256": hashlib.sha256(actual).hexdigest(),
                     "evidence": origins[address]})
        print(f"calendar/memory provider-linked: MATCH {name} bytes={size}/{size} relocations={relocations}")
    with EVIDENCE.open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if rows != frozen:
        raise SystemExit("calendar/memory evidence ledger drift")
    (BUILD / "report.json").write_text(json.dumps(rows, indent=2) + "\n")
    print("calendar/memory provider-linked: MATCH functions=11/11 bytes=2300/2300 relocations=92")


if __name__ == "__main__":
    main()
