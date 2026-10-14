#!/usr/bin/env python3
"""Rebuild all target cheat management and RAM search bodies.

Eleven management functions match complete provider-linked target bytes. The
initialization body reproduces its frozen historical object. Search code matches
23936 bytes with only 15 known relocation fields normalized; the two large
search bodies have no relocations and reproduce 23364 raw bytes. Per-function
search hashes were derived from the upstream object after both frozen code
windows reproduced their historical raw hashes. No private ELF is needed.
"""
from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile, Symbol

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/cheats-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
WINDOWS = ROOT / "analysis/link_identity/code_windows.json"
WITNESS = ROOT / "matching/candidates/stage3p_code_residual_exact.S"
LISTING = ROOT / "analysis/functions/progress11_short_targets.asm"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
    "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
    "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)
# Reviewed immutable per-function hashes from independently validated objects.
MANAGEMENT = ((1130720,
  '_Z16S9xInitCheatDatav',
  56,
  7,
  'fc7c409f7309ea848253e3c3bf02b5b5f109bf2e034063671f1f2f39196a2148'),
 (1130776, '_Z11S9xAddCheathhjh', 180, 3, '1402837e7efeb1b6489e28e582fcec320994f310b4a3103ff2cf6233e0b796fa'),
 (1130956, '_Z14S9xDeleteCheatj', 148, 4, 'cf3f5b5a37166df2b14d7df89faa057ff1ffce39b6d96967f1f59c92f1403756'),
 (1131104, '_Z15S9xDeleteCheatsv', 36, 3, '85b4f592a257ce499eb47c34979fcd33d23cd60108110fd4aca7c240d19e2f18'),
 (1131140, '_Z14S9xEnableCheatj', 76, 3, 'aa7b519f4e8be9d0beae1785c3f8d8715483b133f342ffda9d9ae590c8faf22e'),
 (1131216, '_Z15S9xDisableCheatj', 88, 3, '4ebe967cd9c2a63f6a3833a0752b3a711e261be998231bcec6ab52a6d2847b58'),
 (1131304, '_Z14S9xRemoveCheatj', 116, 5, '378bd725f9f0bad926a5b99a934918b1c05282aa14c27767bfa1edf6a127dcc7'),
 (1131420, '_Z13S9xApplyCheatj', 176, 9, '9f41c3cc1947f72dd4f7254487c60fe2fc235f8b2a72a341a84f8b63a6ec9fbe'),
 (1131596, '_Z14S9xApplyCheatsv', 132, 7, 'd4be809a043d470c8404995059cf7da005d16908309bea3d50fd292694769b3a'),
 (1131728,
  '_Z15S9xRemoveCheatsv',
  116,
  5,
  '374b42e2d7e8a3392f86ce4314f70277fce754e1c4ce7a5744ca662415cbb94b'),
 (1131844,
  '_Z16S9xLoadCheatFilePKc',
  304,
  8,
  'b100696f671b05cc264c05743c9ab945663347253b63fe60caf20635ba396ac3'),
 (1132148,
  '_Z16S9xSaveCheatFilePKc',
  340,
  10,
  '8c517341da5726178e164fdff18fcd4f9e75c8725c739e6e7221d6cb4dbc7661'))
SEARCH = ((1106784,
  '_Z19S9xStartCheatSearchP10SCheatData',
  192,
  6,
  '9fcea50c3c5d9315ec2458312f5cfddbc6916bffbede5d703a5f632973c1944f',
  '9fcea50c3c5d9315ec2458312f5cfddbc6916bffbede5d703a5f632973c1944f',
  '9fcea50c3c5d9315ec2458312f5cfddbc6916bffbede5d703a5f632973c1944f'),
 (1106976,
  '_Z18S9xSearchForChangeP10SCheatData22S9xCheatComparisonType16S9xCheatDataSizehh',
  15208,
  0,
  '6e7b3a0bd49bead5ecfe6be53a0c480cc81b6dbd6ef257389581dc97f438c209',
  '6e7b3a0bd49bead5ecfe6be53a0c480cc81b6dbd6ef257389581dc97f438c209',
  '6e7b3a0bd49bead5ecfe6be53a0c480cc81b6dbd6ef257389581dc97f438c209'),
 (1122184,
  '_Z17S9xSearchForValueP10SCheatData22S9xCheatComparisonType16S9xCheatDataSizejhh',
  8156,
  0,
  '201efde66aa59de7e034d053ffb2d2063af3a912423657fc7d1e8f9b5c48eeb2',
  '201efde66aa59de7e034d053ffb2d2063af3a912423657fc7d1e8f9b5c48eeb2',
  '201efde66aa59de7e034d053ffb2d2063af3a912423657fc7d1e8f9b5c48eeb2'),
 (1130340,
  '_Z27S9xOutputCheatSearchResultsP10SCheatData',
  380,
  9,
  'a65fdac007d37ab27517d9c16e26e05f9a6f0d4969cd1a0931c31bb59cf3ea6a',
  '97df9507a93cfc331bb9608fa75e8e5a3b0b663002996dadc07ad27024c331aa',
  '94dfc0239577d3fefccf79ca122ea94a0c3cb94fd3f66d067be81e7c2e083124'))

RESIDUALS = {
    "_Z14S9xDeleteCheatj": ("stage3p_delete_cheat", 148, "e2e34a7c031d6307d39e812ee07f6873ceb51c841c4367e27123d5dd4b610239"),
    "_Z15S9xDeleteCheatsv": ("stage3p_delete_cheats", 36, "4a32cd5594f1fac39e6a0bd973ffa148cd360a8e486c57caf02963900d22d5fb"),
    "_Z14S9xEnableCheatj": ("stage3p_enable_cheat", 76, "9ec3f9d32a90d4e38fa9b2016a72c46de9c09dbbcb1e2683663b991b9cccea26"),
    "_Z15S9xDisableCheatj": ("stage3p_disable_cheat", 88, "ecae823bb01b6983dc733b734e38a6904810d12083ce68101c1bc2ed8b861d15"),
    # The captured window also includes two following 40-byte comparators.
    # Check its full 420-byte pin, but claim only the 340-byte save function.
    "_Z16S9xSaveCheatFilePKc": ("stage3p_save_cheat_file", 420, "c4feb0d58733f0c03ba2c07a06b3eac539e88d8a0d19a0a9132666e66a7abfa8"),
}
PROVIDERS = {
    "DAT_0035fda8": 0x0035FDA8, "DAT_0034e2b0": 0x0034E2B0,
    "DAT_003454e0": 0x003454E0, "DAT_0034e298": 0x0034E298,
    "_Z10S9xGetBytej": 0x001AB63C, "_Z10S9xSetBytehj": 0x001AB900,
    "memmove": 0x0019C4A0, "memset": 0x0019C39C,
    "fioOpen": 0x0019CFC0, "fioClose": 0x0019D090,
    "fioRead": 0x0019D120, "fioWrite": 0x0019D244,
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
    paths, objects = {}, {}
    for unit in ("cheats2", "cheat_search"):
        paths[unit] = BUILD / f"{unit}.o"
        run([CXX, *FLAGS, "-c", ROOT / f"src/snes9x/{unit}.cpp", "-o", paths[unit]])
        objects[unit] = ELFFile(paths[unit])
    selected = json.loads(WINDOWS.read_text())["result"]["selected_sources"]
    for unit, specs in (("cheats2", MANAGEMENT), ("cheat_search", SEARCH)):
        elf = objects[unit]
        functions = [s for s in elf.symbols if s.info & 15 == 2 and s.size]
        expected = {spec[1] for spec in specs}
        section = next(s for s in elf.sections if s.name == ".text")
        if ({s.name for s in functions} != expected or len(functions) != len(specs)
                or section.size != sum(spec[2] for spec in specs)):
            raise SystemExit(f"unproved function or instruction bytes: {unit}")
    words = {}
    for line in LISTING.read_text().splitlines():
        match = LINE.match(line)
        if match:
            address = int(match[1], 16)
            if address in words:
                raise SystemExit(f"duplicate listing instruction: {address:#x}")
            words[address] = bytes(int(match[i], 16) for i in range(2, 6))
    script = BUILD / "cheats2.target.ld"
    script.write_text("".join(f"PROVIDE({name} = {value:#x});\n" for name, value in PROVIDERS.items())
                      + "SECTIONS { .text 0x001140e0 : { *(.text) }\n"
                      + " /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.rodata*)"
                      + " *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked = BUILD / "cheats2.target.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, paths["cheats2"], "-o", linked])
    final = ELFFile(linked)
    witness = WITNESS.read_text()
    rows = []
    for address, name, size, relocations, sha in MANAGEMENT:
        elf = objects["cheats2"]
        symbol = elf.find_symbol(name)
        if (symbol.value != address - 0x001140E0 or symbol.size != size
                or len(elf.relocation_ranges(symbol, size)) != relocations
                or digest(elf.symbol_bytes(symbol, size)) != sha):
            raise SystemExit(f"management object drift: {name}")
        normalized(elf, symbol)  # Fail closed on unknown relocation types.
        linked_sha, level, evidence = "", "historical-object", "analysis/link_identity/code_windows.json"
        target_digest = None
        if name in RESIDUALS:
            label, full_size, full_sha = RESIDUALS[name]
            match = re.search(r"(?ms)^" + re.escape(label) + r":\n(.*?)^\s*\.size "
                              + re.escape(label) + r",", witness)
            if not match:
                raise SystemExit(f"missing target witness: {label}")
            span = b"".join(int(word, 16).to_bytes(4, "little")
                            for word in re.findall(r"^\s*\.word (0x[0-9a-fA-F]+)", match[1], re.M))
            frozen_slice(selected, address, label, full_size, 0, full_sha)
            if len(span) != full_size or digest(span) != full_sha:
                raise SystemExit(f"target witness drift: {label}")
            target = span[:size]
            evidence = WITNESS.relative_to(ROOT).as_posix()
        else:
            frozen_slice(selected, address, name, size, relocations, sha)
            target = None
            if address in words:
                try:
                    target = b"".join(words[a] for a in range(address, address + size, 4))
                except KeyError as missing:
                    raise SystemExit(f"incomplete management listing: {missing}")
                evidence = LISTING.relative_to(ROOT).as_posix()
            elif name == "_Z16S9xLoadCheatFilePKc":
                evidence = "analysis/matching/hunt1041-v73-validated-2.tsv"
                with (ROOT / evidence).open(newline="") as stream:
                    capture = [r for r in csv.DictReader(stream, delimiter="\t")
                               if r["object_symbol"] == name]
                target_digest = "5a0c72f83cad7ca411f73ddb552d5392f840d3e185ee754f073b6ec2504bdd94"
                if (len(capture) != 1 or capture[0]["address"] != f"0x{address:08x}"
                        or capture[0]["object_size"] != str(size)
                        or capture[0]["relocation_count"] != str(relocations)
                        or capture[0]["result"] != "MATCH" or capture[0]["differing_bytes"] != "0"
                        or capture[0]["unknown_relocations"]
                        or capture[0]["target_span_sha256"] != target_digest):
                    raise SystemExit("frozen cheat loader target digest drift")
        if target is not None or target_digest is not None:
            actual_symbol = final.find_symbol(name)
            actual = final.symbol_bytes(actual_symbol, actual_symbol.size)
            if (actual_symbol.value != address
                    or (target is not None and actual != target)
                    or (target_digest is not None and digest(actual) != target_digest)):
                raise SystemExit(f"provider-linked mismatch: {name}")
            linked_sha, level = digest(actual), "provider-linked"
        rows.append({"address": f"0x{address:08x}", "symbol": name,
                     "source_file": "src/snes9x/cheats2.cpp", "size": str(size),
                     "relocations": str(relocations), "raw_sha256": sha,
                     "linked_sha256": linked_sha, "proof_level": level, "evidence": evidence})
        print(f"cheats management {level}: MATCH {name} bytes={size}/{size}")
    check_ledger("analysis/functions/cheats2_exact_1768.tsv", rows)
    # Preserve the two original object pins; isolation removes parser strings.
    for address, offset, size, relocations, raw_sha, norm_sha in (
        (0x0010E360, 0, 7328, 6, "ece1253dea9b61b4a92d51e5beaebaf29d585e1dace22fff01ea3c76c469eb19",
         "ece1253dea9b61b4a92d51e5beaebaf29d585e1dace22fff01ea3c76c469eb19"),
        (0x00110000, 7328, 16608, 9, "ebbfeeb354c4e893a1bb040eefb5eceb91c04cb9ab98e9c4482ddcc1c4d2aa86",
         "8eddfb9c986664bb0b84398c732183966783ec24599491f8e13a53052d4f6466"),
    ):
        frozen_slice(selected, address, "", size, relocations, raw_sha)
        elf = objects["cheat_search"]
        section = next(s for s in elf.sections if s.name == ".text")
        span = Symbol("search slice", offset, size, section.index, 2)
        if (len(elf.relocation_ranges(span, size)) != relocations
                or digest(normalized(elf, span)) != norm_sha):
            raise SystemExit("normalized search slice drift")
    search_rows = []
    cursor = 0
    elf = objects["cheat_search"]
    for address, name, size, relocations, historical_sha, isolated_sha, normalized_sha in SEARCH:
        symbol = elf.find_symbol(name)
        if (symbol.value != cursor or address != 0x0010E360 + cursor or symbol.size != size
                or len(elf.relocation_ranges(symbol, size)) != relocations
                or digest(elf.symbol_bytes(symbol, size)) != isolated_sha
                or digest(normalized(elf, symbol)) != normalized_sha):
            raise SystemExit(f"search function drift: {name}")
        search_rows.append({"address": f"0x{address:08x}", "symbol": name,
                            "source_file": "src/snes9x/cheat_search.cpp", "size": str(size),
                            "relocations": str(relocations), "historical_raw_sha256": historical_sha,
                            "isolated_raw_sha256": isolated_sha, "normalized_sha256": normalized_sha,
                            "evidence": "analysis/link_identity/code_windows.json#cheats_prefix+cheats_text_tail"})
        cursor += size
        print(f"cheat search normalized: MATCH {name} bytes={size}/{size} relocations={relocations}")
    if cursor != 23936:
        raise SystemExit("search function coverage drift")
    check_ledger("analysis/functions/cheat_search_exact_23936.tsv", search_rows)
    (BUILD / "report.json").write_text(json.dumps({"management": rows, "search": search_rows}, indent=2) + "\n")
    print("cheats source: MATCH functions=16/16; management=1768/1768 bytes; "
          "provider-linked=1712/1712 bytes; search=23936/23936 normalized bytes")


if __name__ == "__main__":
    main()
