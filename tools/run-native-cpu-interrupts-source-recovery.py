#!/usr/bin/env python3
"""Prove native main-CPU IRQ/NMI against the fully linked historical reference.

All 1296 raw instruction bytes retain the original shared-state field addends.
The full original CPUOPS code window and GLOBALS state section are independently
pinned; selected functions also match frozen normalized target witnesses. No
complete private-target code digest or fresh full-image comparison is inferred.
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
BUILD = ROOT / "build/matching/native-cpu-interrupts-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
SOURCE = "src/snes9x/native_cpu_interrupts.cpp"
EVIDENCE = ROOT / "analysis/functions/native_cpu_interrupts_exact_1296.tsv"
FLAGS = SOURCE_FIXED_FLAGS[SOURCE]
PROVIDERS = {'DAT_003453a8': 3429288,
 'DAT_003454e0': 3429600,
 'DAT_0034e2b0': 3465904,
 '_Z10S9xGetWordj': 1752104,
 '_Z10S9xSetBytehj': 1751296,
 '_Z12S9xSetPCBasej': 1753124,
 '__gxx_personality_v0': 1742632,
 'g_CPU_blob': 3429184,
 'g_ICPU_00345318': 3429144,
 'g_OpenBus_byte': 3520360}
LEGACY_NAMES = {'DAT_003453a8': 'Registers',
 'DAT_003454e0': 'Settings',
 'DAT_0034e2b0': 'Memory',
 'g_CPU_blob': 'CPU',
 'g_ICPU_00345318': 'ICPU',
 'g_OpenBus_byte': 'OpenBus'}
REFERENCE_PROVIDERS = {'A1': 3520319,
 'A2': 3520320,
 'A3': 3520321,
 'A4': 3520322,
 'APU': 3429304,
 'Ans16': 3520328,
 'Ans32': 3520332,
 'Ans8': 3520327,
 'CPU': 3429184,
 'IAPU': 3429528,
 'ICPU': 3429144,
 'Int16': 3520346,
 'Int32': 3520352,
 'Int8': 3520344,
 'Memory': 3465904,
 'OpAddress': 3465896,
 'OpenBus': 3520360,
 'Registers': 3429288,
 'S9xAPUCycles': 4146344,
 'S9xApuOpcodes': 4263952,
 'S9xSA1ExecuteDuringSleep': 1434000,
 'Settings': 3429600,
 'W1': 3520323,
 'W2': 3520324,
 'W3': 3520325,
 'W4': 3520326,
 'Work16': 3520338,
 'Work32': 3520340,
 'Work8': 3520336,
 '_Z10S9xGetBytej': 1750588,
 '_Z10S9xGetWordj': 1752104,
 '_Z10S9xSetBytehj': 1751296,
 '_Z10S9xSetWordtj': 1753488,
 '_Z12S9xSetPCBasej': 1753124,
 '__gxx_personality_v0': 1742632,
 'missing': 3428968}
REFERENCE_STATE = {'A1': (3520319, 1),
 'A2': (3520320, 1),
 'A3': (3520321, 1),
 'A4': (3520322, 1),
 'APU': (3429304, 224),
 'Ans16': (3520328, 2),
 'Ans32': (3520332, 4),
 'Ans8': (3520327, 1),
 'CPU': (3429184, 104),
 'IAPU': (3429528, 60),
 'ICPU': (3429144, 36),
 'Int16': (3520346, 2),
 'Int32': (3520352, 8),
 'Int8': (3520344, 1),
 'Memory': (3465904, 54404),
 'OpAddress': (3465896, 8),
 'OpenBus': (3520360, 1),
 'Registers': (3429288, 16),
 'S9xAPUCycles': (4146344, 1024),
 'Settings': (3429600, 328),
 'W1': (3520323, 1),
 'W2': (3520324, 1),
 'W3': (3520325, 1),
 'W4': (3520326, 1),
 'Work16': (3520338, 2),
 'Work32': (3520340, 4),
 'Work8': (3520336, 1),
 'missing': (3428968, 174)}
SPECS = [{'address': '0x00127b78',
  'evidence': 'analysis/matching/hunt1000plus-v46-validated-42.tsv',
  'historical_raw_sha256': 'c5dcf050980308986b732793aafb752377d19cfd00e97a6a5baf03464e5a4b98',
  'isolated_raw_sha256': 'c5dcf050980308986b732793aafb752377d19cfd00e97a6a5baf03464e5a4b98',
  'linked_sha256': '6cc956f4227aa9bebd8fa045f54a8e50c313902143136c3b064f7c92d71802c8',
  'normalized_sha256': '630c242c232b7d10bd86c1bd3f41f3301b0014971fbe93fd785db731ee90bd83',
  'proof_level': 'linked-historical-reference',
  'relocations': '40',
  'size': '648',
  'source_file': 'src/snes9x/native_cpu_interrupts.cpp',
  'symbol': '_Z13S9xOpcode_IRQv'},
 {'address': '0x00127e00',
  'evidence': 'analysis/matching/hunt1000plus-v46-validated-42.tsv',
  'historical_raw_sha256': '274ab1b1aa4ea275964b11d8c0eb5ed168d873ba9fceb58ec0a04ea4a6c3cd09',
  'isolated_raw_sha256': '274ab1b1aa4ea275964b11d8c0eb5ed168d873ba9fceb58ec0a04ea4a6c3cd09',
  'linked_sha256': '2d24db9477cb783b14e7314ee7199b3413e9b710644edb6edab806481a923760',
  'normalized_sha256': 'b14cb522431808317d0d0240b72b85e3f2c9dd211fd6d734fdbdfc51c0741d9c',
  'proof_level': 'linked-historical-reference',
  'relocations': '40',
  'size': '648',
  'source_file': 'src/snes9x/native_cpu_interrupts.cpp',
  'symbol': '_Z13S9xOpcode_NMIv'}]


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
        raise SystemExit("native CPU interrupts section profile drift")
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
    original_path, state_path = BUILD / "CPUOPS.historical.o", BUILD / "GLOBALS.historical.o"
    for source, output in (("CPUOPS.CPP", original_path), ("GLOBALS.CPP", state_path)):
        run([CXX, *FLAGS[:-1], "-DZLIB", *includes, "-x", "c++", "-c",
             layout / source, "-o", output])
    obj = BUILD / "native_cpu_interrupts.o"
    run([CXX, *FLAGS, "-c", ROOT / SOURCE, "-o", obj])
    original, state, isolated = (ELFFile(p) for p in (original_path, state_path, obj))
    text_sha = "0c37669bbd3f8c333c0e75429b504793ea9ea2977d8f00753e16077ef47730d4"
    windows = json.loads((ROOT / "analysis/link_identity/code_windows.json")
                         .read_text())["result"]["selected_sources"]
    window = [r for r in windows if r["name"] == "cpuops"]
    if (len(section_bytes(original, ".text")) != 78772
            or digest(section_bytes(original, ".text")) != text_sha
            or len(window) != 1 or any(window[0][k] != v for k, v in
                {"address": 0x116740, "size": 78772, "raw_sha256": text_sha,
                 "relocations": 6058, "source_offset": 0}.items())):
        raise SystemExit("frozen complete historical CPUOPS code window drift")
    # The original .data has 1421 relocations; the historical 20876-byte
    # selected data slice has 1416. Only selected IRQ/NMI code is promoted.
    for name, count in ((".rel.text", 6058), (".rel.data", 1421)):
        sections = [s for s in original.sections if s.name == name]
        if len(sections) != 1 or sections[0].size // sections[0].entry_size != count:
            raise SystemExit("complete historical CPUOPS relocation roster drift")
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
            raise SystemExit("original CPUOPS shared-provider geometry drift: " + name)
    for name, historical_name in LEGACY_NAMES.items():
        if PROVIDERS[name] != REFERENCE_PROVIDERS[historical_name]:
            raise SystemExit("native CPU state binding drift: " + name)
    functions = [s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    writable = [s for s in isolated.symbols if s.info & 15 == 1 and s.size
                and 0 < s.section_index < len(isolated.sections)
                and isolated.sections[s.section_index].name.startswith((".data", ".bss"))]
    rodata = [s for s in isolated.sections if s.name.startswith(".rodata") and s.size]
    if ({s.name for s in functions} != {r["symbol"] for r in SPECS}
            or sum(s.size for s in functions) != 1296
            or any(s.info >> 4 != 1 for s in functions) or writable or rodata):
        raise SystemExit("native CPU interrupts function/shared-storage inventory drift")
    imports = {s.name for s in isolated.symbols if s.section_index == 0 and s.name}
    original_imports = {s.name for s in original.symbols if s.section_index == 0 and s.name}
    if imports != set(PROVIDERS) or original_imports != set(REFERENCE_PROVIDERS):
        raise SystemExit("native/historical CPUOPS provider ABI/import roster drift")
    # These native memory callees already have complete source proofs. Keep
    # their real signatures and source ownership rather than contextual models.
    with (ROOT / "analysis/source_tree/defined_symbol_ownership.tsv").open(newline="") as stream:
        definitions = list(csv.DictReader(stream, delimiter="\t"))
    for name, size, source in (("_Z10S9xGetWordj", 1020, "s9xgetword"),
                               ("_Z10S9xSetBytehj", 808, "s9xsetbyte"),
                               ("_Z12S9xSetPCBasej", 364, "s9xsetpcbase")):
        owners = [r for r in definitions if r["symbol"] == name]
        if (len(owners) != 1 or any(owners[0][k] != v for k, v in
                {"binding": "global", "section_class": "text", "size_hex": hex(size),
                 "source": f"src/snes9x/{source}.cpp", "object": f"snes9x/{source}.o"}.items())):
            raise SystemExit("canonical CPU memory callee ownership drift: " + name)
    script = BUILD / "native_cpu_interrupts.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
        + "SECTIONS {\n" + "".join(
            f".text.{i} {int(r['address'], 0):#x} : {{ *(.text.{r['symbol']}) }}\n"
            for i, r in enumerate(SPECS))
        + "/DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) "
          "*(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked_path = BUILD / "native_cpu_interrupts.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked_path])
    linked = ELFFile(linked_path)
    # Compile and place the full historical object for comparison only. Every
    # import has its actual address; unselected routines are not promoted.
    script = BUILD / "CPUOPS.historical.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in REFERENCE_PROVIDERS.items())
        + "SECTIONS { .text 0x116740 : { *(.text) } .data 0x3367f8 : { *(.data*) } "
          ".bss 0x7fc00000 : { *(.bss*) *(COMMON) } /DISCARD/ : { *(.reginfo) "
          "*(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    reference_path = BUILD / "CPUOPS.historical.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, original_path, "-o", reference_path])
    reference = ELFFile(reference_path)
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
            raise SystemExit("native CPU interrupts instructions/full linked reference drift: " + name)
        with (ROOT / spec["evidence"]).open(newline="") as stream:
            witnesses = [r for r in csv.DictReader(stream, delimiter="\t")
                         if r["address"] == spec["address"]]
        if (len(witnesses) != 1 or witnesses[0]["result"] != "MATCH"
                or witnesses[0]["differing_bytes"] != "0" or witnesses[0]["unknown_relocations"]
                or witnesses[0]["normalized_equal"] != "True"
                or witnesses[0]["object_symbol"] != name or witnesses[0]["object_size"] != str(size)):
            raise SystemExit("frozen normalized CPU interrupt witness drift: " + name)
        print(f"native CPU interrupts: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native CPU interrupts evidence ledger drift")
    print("native CPU interrupt proof: 2/2 routines; 1296 complete linked historical bytes; "
          "shared native state and original C++ ABI preserved")


if __name__ == "__main__":
    main()
