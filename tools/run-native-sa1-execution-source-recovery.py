#!/usr/bin/env python3
"""Prove native SA-1 execution and IRQ entry without duplicating native state.

All 552 selected instruction bytes retain original field addends and match a
fully linked historical reference. Frozen normalized MATCH witnesses remain
unchanged; no complete private-target code digest is inferred. The original
21872-byte SA1CPU data section independently matches its frozen target digest
with actual opcode/helper/state addresses, including all four opcode tables.
"""
from __future__ import annotations

import csv
import hashlib
import json
import subprocess
import sys
from pathlib import Path

from compare_elf_functions import ELFFile
from build_source_tree import SOURCE_FIXED_FLAGS

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-sa1-execution-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
SOURCE = "src/snes9x/native_sa1_execution.cpp"
EVIDENCE = ROOT / "analysis/functions/native_sa1_execution_exact_552.tsv"
FLAGS = SOURCE_FIXED_FLAGS[SOURCE]
PROVIDERS = {'DAT_00345ae8': 3431144,
 'DAT_0034e2b0': 3465904,
 'S9xSA1SetByte': 1433192,
 'S9xSA1SetPCBase': 1433724,
 '__gxx_personality_v0': 1742632,
 'g_OpenBus_byte': 3520360,
 'g_SA1_blob': 3431160}
LEGACY_NAMES = {'DAT_00345ae8': 'SA1Registers',
 'DAT_0034e2b0': 'Memory',
 'g_OpenBus_byte': 'OpenBus',
 'g_SA1_blob': 'SA1'}
REFERENCE_PROVIDERS = {'A1': 3520319,
 'A2': 3520320,
 'A3': 3520321,
 'A4': 3520322,
 'Ans16': 3520328,
 'Ans32': 3520332,
 'Ans8': 3520327,
 'Int16': 3520346,
 'Int32': 3520352,
 'Int8': 3520344,
 'Memory': 3465904,
 'OpAddress': 3465896,
 'OpenBus': 3520360,
 'S9xSA1GetByte': 1432708,
 'S9xSA1GetWord': 1433108,
 'S9xSA1SetByte': 1433192,
 'S9xSA1SetPCBase': 1433724,
 'S9xSA1SetWord': 1433652,
 'SA1': 3431160,
 'SA1Registers': 3431144,
 'Settings': 3429600,
 'W1': 3520323,
 'W2': 3520324,
 'W3': 3520325,
 'W4': 3520326,
 'Work16': 3520338,
 'Work32': 3520340,
 'Work8': 3520336,
 '__gxx_personality_v0': 1742632,
 'missing': 3428968}
REFERENCE_STATE = {'A1': (3520319, 1),
 'A2': (3520320, 1),
 'A3': (3520321, 1),
 'A4': (3520322, 1),
 'Ans16': (3520328, 2),
 'Ans32': (3520332, 4),
 'Ans8': (3520327, 1),
 'Int16': (3520346, 2),
 'Int32': (3520352, 8),
 'Int8': (3520344, 1),
 'Memory': (3465904, 54404),
 'OpAddress': (3465896, 8),
 'OpenBus': (3520360, 1),
 'SA1': (3431160, 32856),
 'SA1Registers': (3431144, 16),
 'Settings': (3429600, 328),
 'W1': (3520323, 1),
 'W2': (3520324, 1),
 'W3': (3520325, 1),
 'W4': (3520326, 1),
 'Work16': (3520338, 2),
 'Work32': (3520340, 4),
 'Work8': (3520336, 1),
 'missing': (3428968, 174)}
SPECS = [{'address': '0x0016ded4',
  'symbol': '_Z16S9xSA1Opcode_IRQv',
  'source_file': 'src/snes9x/native_sa1_execution.cpp',
  'size': '296',
  'relocations': '15',
  'historical_raw_sha256': '6dc84877ad9840f3bc1369eecfe6cd2514f202f14b8da2d8e7ce05dbf63bf562',
  'isolated_raw_sha256': '6dc84877ad9840f3bc1369eecfe6cd2514f202f14b8da2d8e7ce05dbf63bf562',
  'normalized_sha256': 'c4f102cf610323d8cf4d0ae7b11eed1f35567c1159d373e0ac41dcb158e9f672',
  'linked_sha256': 'e1241edfae9bd701741ac664cb2e126ab2eeecb69b2a28e500e2e1b296228590',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt1000plus-v46-validated-42.tsv'},
 {'address': '0x0016efa0',
  'symbol': 'S9xSA1MainLoop',
  'source_file': 'src/snes9x/native_sa1_execution.cpp',
  'size': '256',
  'relocations': '14',
  'historical_raw_sha256': '4ec83a67c800969c33308bdff5d21d33ee28bc80e9db497b2c0fa6998bf7ba68',
  'isolated_raw_sha256': '4ec83a67c800969c33308bdff5d21d33ee28bc80e9db497b2c0fa6998bf7ba68',
  'normalized_sha256': 'e8d75deb6c716e1cf374fbfa9db93df0fef6f62653acb2e03a9c448d76fcaddd',
  'linked_sha256': 'fef84d4596989f5410422753dacbc0e72c6bd39633c3d302c4c25f8de01bf2fc',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'}]


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


def section_bytes(elf, name):
    section = next(s for s in elf.sections if s.name == name)
    return elf.data[section.offset:section.offset + section.size]


def main():
    if (run([CXX, "-dumpmachine"]).strip() != "ee"
            or run([CXX, "-dumpversion"]).strip() != "3.2.2"):
        raise SystemExit("wrong historical compiler")
    if FLAGS[-1] != "-ffunction-sections":
        raise SystemExit("native SA-1 execution section profile drift")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v52_closure as recipe
    recipe.v47.ensure_git_commit(recipe.v47.PS2DEV, recipe.v47.PS2DEV_REPO,
                                 recipe.v47.PS2DEV_COMMIT)
    BUILD.mkdir(parents=True, exist_ok=True)
    old_build = recipe.BUILD
    try:
        recipe.BUILD = BUILD / "profile"
        root, upstream, layout = recipe.prepare_snes_layout()
        recipe.patch_sources(layout)
        recipe.write_compat_headers(BUILD / "compat")
    finally:
        recipe.BUILD = old_build
    newlib = recipe.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    includes = ["-I" + str(p) for p in
                (BUILD / "compat", newlib, layout, layout / "unzip", root / "zlib")]
    original_path, state_path = BUILD / "SA1CPU.historical.o", BUILD / "GLOBALS.historical.o"
    for source, output in (("SA1CPU.CPP", original_path), ("GLOBALS.CPP", state_path)):
        run([CXX, *FLAGS[:-1], "-DZLIB", *includes, "-x", "c++", "-c",
             layout / source, "-o", output])
    obj = BUILD / "native_sa1_execution.o"
    run([CXX, *FLAGS, "-c", ROOT / SOURCE, "-o", obj])
    original, state, isolated = (ELFFile(p) for p in (original_path, state_path, obj))
    text_sha = "b6c87a7f3f80464070b556685e6490f91faafe93924c516ff3c8ed4dc3723674"
    windows = json.loads((ROOT / "analysis/link_identity/code_windows.json")
                         .read_text())["result"]["selected_sources"]
    window = [r for r in windows if r["name"] == "sa1cpu"]
    if (len(section_bytes(original, ".text")) != 67560
            or digest(section_bytes(original, ".text")) != text_sha
            or len(window) != 1 or any(window[0][k] != v for k, v in
                {"address": 0x15f1c8, "size": 67560, "raw_sha256": text_sha,
                 "relocations": 5131, "source_offset": 0}.items())):
        raise SystemExit("frozen complete historical SA1CPU code window drift")
    for name, count in ((".rel.text", 5131), (".rel.data", 1411)):
        sections = [s for s in original.sections if s.name == name]
        if len(sections) != 1 or sections[0].size // sections[0].entry_size != count:
            raise SystemExit("complete historical SA1CPU relocation roster drift")
    state_sha = "8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    owners = json.loads((ROOT / "analysis/link_identity/historical_data.json")
                        .read_text())["owners"]
    owner = [r for r in owners if r["unit"] == "globals" and r["symbol"] == "@section:.data"]
    if (len(section_bytes(state, ".data")) != 0xaf848
            or digest(section_bytes(state, ".data")) != state_sha
            or len(owner) != 1 or any(owner[0][k] != v for k, v in
                {"address": 0x345060, "size": 0xaf848, "source_offset": 0,
                 "sha256": state_sha}.items())):
        raise SystemExit("frozen complete original state source drift")
    for name, (address, size) in REFERENCE_STATE.items():
        symbol = state.find_symbol(name)
        if (symbol.value + 0x345060 != address or symbol.size != size
                or REFERENCE_PROVIDERS[name] != address):
            raise SystemExit("original SA1CPU shared-provider geometry drift: " + name)
    for name, historical_name in LEGACY_NAMES.items():
        if PROVIDERS[name] != REFERENCE_PROVIDERS[historical_name]:
            raise SystemExit("native SA-1 state binding drift: " + name)
    functions = [s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    writable = [s for s in isolated.symbols if s.info & 15 == 1 and s.size
                and 0 < s.section_index < len(isolated.sections)
                and isolated.sections[s.section_index].name.startswith((".data", ".bss"))]
    rodata = [s for s in isolated.sections if s.name.startswith(".rodata") and s.size]
    if ({s.name for s in functions} != {r["symbol"] for r in SPECS}
            or sum(s.size for s in functions) != 552
            or any(s.info >> 4 != 1 for s in functions) or writable or rodata):
        raise SystemExit("native SA-1 execution function/shared-storage inventory drift")
    imports = {s.name for s in isolated.symbols if s.section_index == 0 and s.name}
    original_imports = {s.name for s in original.symbols if s.section_index == 0 and s.name}
    if imports != set(PROVIDERS) or original_imports != set(REFERENCE_PROVIDERS):
        raise SystemExit("native/historical SA1CPU provider ABI/import roster drift")
    # These native memory callees already have complete source proofs. Keep
    # their real signatures and source ownership rather than contextual models.
    with (ROOT / "analysis/source_tree/defined_symbol_ownership.tsv").open(newline="") as stream:
        definitions = list(csv.DictReader(stream, delimiter="\t"))
    for name, size in (("S9xSA1SetByte", 460), ("S9xSA1SetPCBase", 276)):
        owners = [r for r in definitions if r["symbol"] == name]
        if (len(owners) != 1 or any(owners[0][k] != v for k, v in
                {"binding": "global", "section_class": "text", "size_hex": hex(size),
                 "source": "src/snes9x/native_sa1.cpp", "object": "snes9x/native_sa1.o"}.items())):
            raise SystemExit("canonical SA-1 memory callee ownership drift: " + name)
    script = BUILD / "native_sa1_execution.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
        + "SECTIONS {\n" + "".join(
            f".text.{i} {int(r['address'], 0):#x} : {{ *(.text.{r['symbol']}) }}\n"
            for i, r in enumerate(SPECS))
        + "/DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) "
          "*(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked_path = BUILD / "native_sa1_execution.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked_path])
    linked = ELFFile(linked_path)
    # Compile and place the full historical object for comparison only. Every
    # import has its actual address; unselected routines are not promoted.
    script = BUILD / "SA1CPU.historical.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in REFERENCE_PROVIDERS.items())
        + "SECTIONS { .text 0x15f1c8 : { *(.text) } .data 0x3f5040 : { *(.data*) } "
          ".bss 0x7fc00000 : { *(.bss*) *(COMMON) } /DISCARD/ : { *(.reginfo) "
          "*(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    reference_path = BUILD / "SA1CPU.historical.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, original_path, "-o", reference_path])
    reference = ELFFile(reference_path)
    data_sha = "be8548846f24b7a7bc2ab6d16eaa0ed930b13f1a3ecc05640efd9d440b23c9e6"
    linked_data_sha = "bb4e64679d46f3cc1fa54fc08ccaee1dc1102e8a910a039116a9583dd52d7a64"
    tables = json.loads((ROOT / "analysis/link_identity/historical_tail_data.json")
                        .read_text())["providers"]
    table = [r for r in tables if r["name"] == "sa1cpu"]
    if (len(section_bytes(original, ".data")) != 21872
            or digest(section_bytes(original, ".data")) != data_sha
            or digest(section_bytes(reference, ".data")) != linked_data_sha
            or len(table) != 1 or any(table[0][k] != v for k, v in
                {"filename": "SA1CPU.CPP", "full_size": 21872, "size": 21872,
                 "full_sha256": data_sha, "slice_sha256": data_sha,
                 "linked_sha256": linked_data_sha, "target_address": 0x3f5040,
                 "relocations": 1411, "source_offset": 0, "profile": "v46-official"}.items())):
        raise SystemExit("complete original SA1CPU data/target-linked digest drift")
    for name, address in (("S9xSA1OpcodesM1X1", 0x3f5040), ("S9xSA1OpcodesM1X0", 0x3f5440),
                          ("S9xSA1OpcodesM0X0", 0x3f5840), ("S9xSA1OpcodesM0X1", 0x3f5c40)):
        symbol = reference.find_symbol(name)
        if symbol.value != address or symbol.size != 1024:
            raise SystemExit("complete native SA-1 opcode-table placement drift: " + name)
    for spec in SPECS:
        name, size, address = spec["symbol"], int(spec["size"]), int(spec["address"], 0)
        old, local, placed, ref = (elf.find_symbol(name)
                                  for elf in (original, isolated, linked, reference))
        raw = linked.symbol_bytes(placed, size)
        if (any(symbol.size != size for symbol in (old, local, placed, ref))
                or placed.value != address or ref.value != address
                or original.symbol_bytes(old, size) != isolated.symbol_bytes(local, size)
                or digest(original.symbol_bytes(old, size)) != spec["historical_raw_sha256"]
                or digest(isolated.symbol_bytes(local, size)) != spec["isolated_raw_sha256"]
                or normalized(original, old) != normalized(isolated, local)
                or digest(normalized(isolated, local)) != spec["normalized_sha256"]
                or len(isolated.relocation_ranges(local, size)) != int(spec["relocations"])
                or raw != reference.symbol_bytes(ref, size)
                or digest(raw) != spec["linked_sha256"]
                or spec["proof_level"] != "linked-historical-reference"):
            raise SystemExit("native SA-1 execution instructions/full linked reference drift: " + name)
        with (ROOT / spec["evidence"]).open(newline="") as stream:
            witnesses = [r for r in csv.DictReader(stream, delimiter="\t")
                         if r["address"] == spec["address"]]
        if (len(witnesses) != 1 or witnesses[0]["result"] != "MATCH"
                or witnesses[0]["differing_bytes"] != "0" or witnesses[0]["unknown_relocations"]
                or witnesses[0]["normalized_equal"] != "True"
                or witnesses[0]["object_symbol"] != name or witnesses[0]["object_size"] != str(size)):
            raise SystemExit("frozen normalized SA-1 execution witness drift: " + name)
        print(f"native SA-1 execution: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native SA-1 execution evidence ledger drift")
    print("native SA-1 execution proof: 2/2 routines; 552 complete linked historical bytes; "
          "21872 complete target-linked original data bytes; shared native state preserved")


if __name__ == "__main__":
    main()
