#!/usr/bin/env python3
"""Prove native controller updates, frontend leaves and C4/ST018 register IO.

All 1548 selected instruction bytes retain original field addends and match
historical source or independent frozen frontend leaf reconstruction. Six
routines additionally reproduce 672 complete target bytes. The ST018 diagnostic
string reproduces all 24 frozen target data bytes. Original shared state and
frontend callback ABI are retained; no private-image recapture is inferred.
"""
from __future__ import annotations
import csv
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from build_source_tree import SOURCE_FIXED_FLAGS
from compare_elf_functions import ELFFile
ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-controller-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
EVIDENCE = ROOT / "analysis/functions/native_controller_chip_io_exact_1548.tsv"
PROVIDERS = {'DAT_003454e0': 3429600,
 'DAT_0034e2b0': 3465904,
 'DAT_0035b788': 3520392,
 'DAT_0035c268': 3523176,
 'DAT_003f4bf0': 4148208,
 'DAT_003f4bf4': 4148212,
 'S9xProcessMouse': 1428192,
 'S9xReadJoypad': 1067964,
 '__gxx_personality_v0': 1742632,
 'bRam0042e888': 4384904}
LEGACY_NAMES = {'DAT_003454e0': 'Settings',
 'DAT_0034e2b0': 'Memory',
 'DAT_0035b788': 'PPU',
 'DAT_0035c268': 'IPPU',
 'DAT_003f4bf0': 'justifiers',
 'DAT_003f4bf4': 'in_bit'}
CHIP_PROVIDERS = {'DAT_0034e2b0': 3465904, 'printf': 1696648}
REFERENCE_PROVIDERS = {'Settings': 3429600,
 'Memory': 3465904,
 'PPU': 3520392,
 'IPPU': 3523176,
 'justifiers': 4148208,
 'in_bit': 4148212,
 'S9xProcessMouse': 1428192,
 'S9xReadJoypad': 1067964,
 '__gxx_personality_v0': 1742632,
 'S9xReadSuperScopePosition': 1068616,
 '_Z18JustifierOffscreenv': 1054848,
 '_Z16JustifierButtonsRj': 1054856}
SPECS = [{'address': '0x00101880',
  'symbol': '_Z18JustifierOffscreenv',
  'source_file': 'src/snes9x/native_controllers.cpp',
  'size': '8',
  'relocations': '0',
  'historical_raw_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'isolated_raw_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'normalized_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'linked_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'target_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'proof_level': 'provider-linked-target',
  'reference_unit': 'frontend-leaves',
  'reference_symbol': 'snes_leaf_00101880',
  'evidence': 'analysis/matching/hunt1000plus-v41-validated-28.tsv'},
 {'address': '0x00101888',
  'symbol': '_Z16JustifierButtonsRj',
  'source_file': 'src/snes9x/native_controllers.cpp',
  'size': '8',
  'relocations': '0',
  'historical_raw_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'isolated_raw_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'normalized_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'linked_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'target_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'proof_level': 'provider-linked-target',
  'reference_unit': 'frontend-leaves',
  'reference_symbol': 'snes_leaf_00101888',
  'evidence': 'analysis/matching/hunt1000plus-v41-validated-28.tsv'},
 {'address': '0x00104e48',
  'symbol': 'S9xReadSuperScopePosition',
  'source_file': 'src/snes9x/native_controllers.cpp',
  'size': '8',
  'relocations': '0',
  'historical_raw_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'isolated_raw_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'normalized_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'linked_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'target_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'proof_level': 'provider-linked-target',
  'reference_unit': 'frontend-leaves',
  'reference_symbol': 'snes_leaf_00104e48',
  'evidence': 'analysis/matching/hunt1000plus-v41-validated-28.tsv'},
 {'address': '0x00104e50',
  'symbol': 'S9xReadMousePosition',
  'source_file': 'src/snes9x/native_controllers.cpp',
  'size': '8',
  'relocations': '0',
  'historical_raw_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'isolated_raw_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'normalized_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'linked_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'target_sha256': '008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061',
  'proof_level': 'provider-linked-target',
  'reference_unit': 'frontend-leaves',
  'reference_symbol': 'snes_leaf_00104e50',
  'evidence': 'analysis/matching/hunt1000plus-v41-validated-28.tsv'},
 {'address': '0x0015cce4',
  'symbol': '_Z17ProcessSuperScopev',
  'source_file': 'src/snes9x/native_controllers.cpp',
  'size': '276',
  'relocations': '9',
  'historical_raw_sha256': '22b5a40838e9c561f31b339322bc7f1b4ad6a511c5160f0ab67f67a62ce4f27e',
  'isolated_raw_sha256': '22b5a40838e9c561f31b339322bc7f1b4ad6a511c5160f0ab67f67a62ce4f27e',
  'normalized_sha256': '45199e5b8d652a2f9949a3df1c097a0bfc28cdd94dc65b41bc2ceff958cc0c3c',
  'linked_sha256': 'ef2d087723e147dc059ca4f618baf3eece27e132035e341afd8da1b80aa1d36c',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'reference_unit': 'ppu.cpp',
  'reference_symbol': '_Z17ProcessSuperScopev',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015cea4',
  'symbol': '_Z19S9xUpdateJustifiersv',
  'source_file': 'src/snes9x/native_controllers.cpp',
  'size': '536',
  'relocations': '40',
  'historical_raw_sha256': 'c9c147564af08f7ad46d2eb35d4940144c9df8e978535d6c30e65e0cab123890',
  'isolated_raw_sha256': 'c9c147564af08f7ad46d2eb35d4940144c9df8e978535d6c30e65e0cab123890',
  'normalized_sha256': 'ebcc3a4aab69bc9469fa3898ab125a0eb27746f4bcc73672c228f20e2a86939d',
  'linked_sha256': 'a7de07f8e99b6c848bde79e50a16f51242dfbbca80cd820d029af979484410d1',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'reference_unit': 'ppu.cpp',
  'reference_symbol': '_Z19S9xUpdateJustifiersv',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015d0bc',
  'symbol': 'S9xUpdateJoypads',
  'source_file': 'src/snes9x/native_controllers.cpp',
  'size': '632',
  'relocations': '30',
  'historical_raw_sha256': '4ff61b33f9fab0561af7ec92b43be3b9a5448a7370838b6231b81fa3dd796e11',
  'isolated_raw_sha256': '4ff61b33f9fab0561af7ec92b43be3b9a5448a7370838b6231b81fa3dd796e11',
  'normalized_sha256': 'd2cacb2a41141658c3cd29a759f074454da59bb824dbac217a037a00f3836c58',
  'linked_sha256': '8cd72d9d056f3409721392d9ec02266f3592b022f5f05f2c0d47ba8a97222d27',
  'target_sha256': '8cd72d9d056f3409721392d9ec02266f3592b022f5f05f2c0d47ba8a97222d27',
  'proof_level': 'provider-linked-target',
  'reference_unit': 'ppu.cpp',
  'reference_symbol': 'S9xUpdateJoypads',
  'evidence': 'analysis/matching/hunt1041-v51-validated-16.tsv'},
 {'address': '0x0010c328',
  'symbol': 'S9xGetC4',
  'source_file': 'src/snes9x/native_chip_io.cpp',
  'size': '24',
  'relocations': '2',
  'historical_raw_sha256': 'f09dd93720e9ed708190173dd1e782f27cfbd5a85d3a4f3993b5cda82459061a',
  'isolated_raw_sha256': 'f09dd93720e9ed708190173dd1e782f27cfbd5a85d3a4f3993b5cda82459061a',
  'normalized_sha256': '0655e68f0531a93b1320d0d44abe1e2b16785521e2eb549ffb5684582cdb6079',
  'linked_sha256': '429718da46347c45d345198b2ca263ea054a7f4242b7b4ccdd7d2b5913fd530e',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'reference_unit': 'c4emu.cpp',
  'reference_symbol': 'S9xGetC4',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x001701fc',
  'symbol': 'S9xGetST018',
  'source_file': 'src/snes9x/native_chip_io.cpp',
  'size': '8',
  'relocations': '0',
  'historical_raw_sha256': '7c1beca9eda80c8c72c253befd4a5785437822b5b0e89354ae44db8359f503ca',
  'isolated_raw_sha256': '7c1beca9eda80c8c72c253befd4a5785437822b5b0e89354ae44db8359f503ca',
  'normalized_sha256': '7c1beca9eda80c8c72c253befd4a5785437822b5b0e89354ae44db8359f503ca',
  'linked_sha256': '7c1beca9eda80c8c72c253befd4a5785437822b5b0e89354ae44db8359f503ca',
  'target_sha256': '7c1beca9eda80c8c72c253befd4a5785437822b5b0e89354ae44db8359f503ca',
  'proof_level': 'provider-linked-target',
  'reference_unit': 'seta018.cpp',
  'reference_symbol': 'S9xGetST018',
  'evidence': 'analysis/matching/hunt1000plus-v41-validated-28.tsv'},
 {'address': '0x00170204',
  'symbol': 'S9xSetST018',
  'source_file': 'src/snes9x/native_chip_io.cpp',
  'size': '40',
  'relocations': '3',
  'historical_raw_sha256': '5a8b7dace6499cb88c05adca7dd00409c2d16af20505dfe0119534b02a1b4275',
  'isolated_raw_sha256': '5a8b7dace6499cb88c05adca7dd00409c2d16af20505dfe0119534b02a1b4275',
  'normalized_sha256': '5a8b7dace6499cb88c05adca7dd00409c2d16af20505dfe0119534b02a1b4275',
  'linked_sha256': 'ad45a40dca3515a56aaf2519dd69f47965843c44e4a998692c353cf8b132aeef',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'reference_unit': 'seta018.cpp',
  'reference_symbol': 'S9xSetST018',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'}]


def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command)) + "\n" + result.stdout[-10000:])
    return result.stdout


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def normalized(elf, symbol):
    raw = bytearray(elf.symbol_bytes(symbol, symbol.size))
    for relocation in elf.relocation_masks(symbol, symbol.size):
        if not relocation.known:
            raise SystemExit("unknown relocation: " + symbol.name)
        for index, mask in enumerate(relocation.mask_bytes):
            raw[relocation.start + index] &= 255 ^ mask
    return bytes(raw)


def section_bytes(elf, name):
    section = next(s for s in elf.sections if s.name == name)
    return elf.data[section.offset:section.offset + section.size]


def main():
    for compiler in (CC, CXX):
        if run([compiler, "-dumpmachine"]).strip() != "ee" or run([compiler, "-dumpversion"]).strip() != "3.2.2":
            raise SystemExit("wrong historical compiler")
    flags = SOURCE_FIXED_FLAGS["src/snes9x/native_controllers.cpp"]
    if flags[-1] != "-ffunction-sections" or SOURCE_FIXED_FLAGS["src/snes9x/native_chip_io.cpp"] != flags:
        raise SystemExit("native controller/chip IO profile drift")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v52_closure as recipe
    recipe.v47.ensure_git_commit(recipe.v47.PS2DEV, recipe.v47.PS2DEV_REPO, recipe.v47.PS2DEV_COMMIT)
    BUILD.mkdir(parents=True, exist_ok=True)
    saved = recipe.BUILD
    try:
        recipe.BUILD = BUILD / "profile"
        root, _upstream, layout = recipe.prepare_snes_layout()
        recipe.patch_sources(layout)
        recipe.write_compat_headers(BUILD / "compat")
    finally:
        recipe.BUILD = saved
    newlib = recipe.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    includes = ["-I" + str(p) for p in (BUILD / "compat", newlib, layout, layout / "unzip", root / "zlib")]
    originals = {}
    for source in ("ppu.cpp", "c4emu.cpp", "seta018.cpp", "GLOBALS.CPP"):
        path = BUILD / (source + ".historical.o")
        run([CXX, *flags[:-1], "-DZLIB", *includes, "-x", "c++", "-c", layout / source, "-o", path])
        originals[source] = ELFFile(path)
    leaf_bodies = ("int snes_leaf_00104e50(void) { return 0; }",
                   "int snes_leaf_00101880(void) { return 0; }",
                   "void snes_leaf_00101888(void) {}",
                   "int snes_leaf_00104e48(void) { return 0; }",
                   "int snes_leaf_001701fc(void) { return 0x81; }")
    frozen_leaf_source = (ROOT / "src/ps2/address_leaf_recovered.c").read_text()
    if any(body not in frozen_leaf_source for body in leaf_bodies):
        raise SystemExit("independent frontend address-leaf source drift")
    path = BUILD / "frontend-leaves.c"
    path.write_text("\n".join(leaf_bodies) + "\n")
    run([CC, *flags[:-1], "-O2", "-c", path, "-o", BUILD / "frontend-leaves.o"])
    originals["frontend-leaves"] = ELFFile(BUILD / "frontend-leaves.o")
    windows = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    ppu_sha = "00dfe755f9cf62c03e0109de99c58eddf554f8a226a59d775b870a0fa69da17a"
    window = [r for r in windows if r["name"] == "ppu"]
    if (len(section_bytes(originals["ppu.cpp"], ".text")) != 18580
            or digest(section_bytes(originals["ppu.cpp"], ".text")) != ppu_sha
            or len(window) != 1 or any(window[0][k] != v for k, v in
                {"address": 0x159058, "size": 18580, "raw_sha256": ppu_sha, "relocations": 1061}.items())):
        raise SystemExit("complete frozen PPU code window drift")
    c4 = [r for r in windows if r["address"] == 0x10c328 and r["symbol"] == "S9xGetC4"]
    if len(c4) != 1 or any(c4[0][k] != v for k, v in
            {"size": 24, "new_bytes": 24, "relocations": 2,
             "raw_sha256": "f09dd93720e9ed708190173dd1e782f27cfbd5a85d3a4f3993b5cda82459061a"}.items()):
        raise SystemExit("complete frozen C4 read code slice drift")
    state = originals["GLOBALS.CPP"]
    state_sha = "8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    owners = json.loads((ROOT / "analysis/link_identity/historical_data.json").read_text())["owners"]
    owner = [r for r in owners if r["unit"] == "globals" and r["symbol"] == "@section:.data"]
    if (len(section_bytes(state, ".data")) != 0xaf848 or digest(section_bytes(state, ".data")) != state_sha
            or len(owner) != 1 or any(owner[0][k] != v for k, v in
                {"address": 0x345060, "size": 0xaf848, "source_offset": 0, "sha256": state_sha}.items())):
        raise SystemExit("complete frozen GLOBALS state digest drift")
    for name, address, size in (("Settings", 0x3454e0, 328), ("PPU", 0x35b788, 2780),
                                ("IPPU", 0x35c268, 4340), ("Memory", 0x34e2b0, 54404)):
        symbol = state.find_symbol(name)
        if symbol.value + 0x345060 != address or symbol.size != size:
            raise SystemExit("controller shared state geometry drift: " + name)
    ppu = originals["ppu.cpp"]
    for name, address, size in (("justifiers", 0x3f4bf0, 4), ("in_bit", 0x3f4bf4, 1)):
        symbol = ppu.find_symbol(name)
        if symbol.value + 0x3f4bf0 != address or symbol.size != size:
            raise SystemExit("original Justifier shared state geometry drift: " + name)
    last = ppu.find_symbol("_ZZ19S9xUpdateJustifiersvE7last_p1")
    if (ppu.sections[last.section_index].name != ".bss" or last.value != 0
            or ppu.sections[last.section_index].type != 8
            or ppu.sections[last.section_index].size != 2):
        raise SystemExit("original Justifier last-player zero-fill geometry drift")
    with (ROOT / "analysis/link_identity/named_contracts.tsv").open(newline="") as stream:
        last_rows = [r for r in csv.DictReader(stream, delimiter="\t") if r["symbol"] == "bRam0042e888"]
    if len(last_rows) != 1 or any(last_rows[0][k] != v for k, v in
            {"target_address": "0x0042e888", "extent_hex": "0x1", "region": "zero-fill",
             "sha256": "6e340b9cffb37a989ca544e6bb780a2c78901d3fb33738768511a30617afa01d"}.items()):
        raise SystemExit("frozen Justifier last-player byte evidence drift")
    isolated, placed = {}, {}
    for source, providers in (("native_controllers", PROVIDERS), ("native_chip_io", CHIP_PROVIDERS)):
        path = BUILD / (source + ".o")
        run([CXX, *flags, "-c", ROOT / ("src/snes9x/" + source + ".cpp"), "-o", path])
        elf = isolated[source] = ELFFile(path)
        functions = [s for s in elf.symbols if s.info & 15 == 2 and s.size]
        writable = [s for s in elf.symbols if s.info & 15 == 1 and s.size and 0 < s.section_index < len(elf.sections)
                    and elf.sections[s.section_index].name.startswith((".data", ".bss"))]
        expected = [r for r in SPECS if r["source_file"] == "src/snes9x/" + source + ".cpp"]
        if ({s.name for s in functions} != {r["symbol"] for r in expected}
                or any(s.info >> 4 != 1 for s in functions) or writable
                or sum(s.size for s in functions) != sum(int(r["size"]) for r in expected)
                or {s.name for s in elf.symbols if s.section_index == 0 and s.name} != set(providers)):
            raise SystemExit("native function/shared-storage/provider inventory drift: " + source)
        script = BUILD / (source + ".ld")
        text = "".join(f"PROVIDE({n} = {v:#x});\n" for n, v in providers.items()) + "SECTIONS {\n"
        text += "".join(f".text.{i} {int(r['address'], 0):#x} : {{ *(.text.{r['symbol']}) }}\n" for i, r in enumerate(expected))
        if source == "native_chip_io":
            text += ".rodata 0x1b80d8 : { *(.rodata*) }\n"
        text += "/DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n"
        script.write_text(text)
        linked = BUILD / (source + ".elf")
        run([CXX.with_name("ee-ld"), "-EL", "-T", script, path, "-o", linked])
        placed[source] = ELFFile(linked)
    references = {}
    contexts = (("ppu.cpp", REFERENCE_PROVIDERS, 0x159058, 0x3f4bf0, 0x1b7318, 0x42e888),
                ("c4emu.cpp", {"Memory": 0x34e2b0, "__gxx_personality_v0": 0x1a9728}, 0x10c300, 0x332ee0, 0x1b3340, 0x7fc00000),
                ("seta018.cpp", {"printf": 0x19e388}, 0x1701fc, 0x3f4000, 0x1b80d8, 0x7fc00000))
    for unit, providers, text_base, data_base, rodata_base, bss_base in contexts:
        bindings = dict(providers)
        # Unselected historical context is comparison-only and never promoted
        # or executed. Every selected callee and shared-state import is bound.
        for symbol in originals[unit].symbols:
            if symbol.section_index == 0 and symbol.name and symbol.name not in bindings:
                bindings[symbol.name] = 0
        script = BUILD / (unit + ".ld")
        script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in bindings.items())
            + f"SECTIONS {{ .text {text_base:#x} : {{ *(.text) }} .data {data_base:#x} : {{ *(.data*) }} "
              f".rodata {rodata_base:#x} : {{ *(.rodata*) }} .bss {bss_base:#x} : {{ *(.bss*) *(COMMON) }} "
              "/DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
        path = BUILD / (unit + ".elf")
        run([CXX.with_name("ee-ld"), "-EL", "-T", script, BUILD / (unit + ".historical.o"), "-o", path])
        references[unit] = ELFFile(path)
    rodata_sha = "ae8ecbe06d560bb54ec48a81d4930d9638148fe7433d36597133a161e52f9239"
    rodata_rows = json.loads((ROOT / "analysis/link_identity/window11_rodata.json").read_text())["source_sections"]
    rodata = [r for r in rodata_rows if r["name"] == "seta018"]
    if (len(rodata) != 1 or any(rodata[0][k] != v for k, v in
            {"address": 0x1b80d8, "full_size": 24, "size": 24, "relocations": 0,
             "raw_sha256": rodata_sha, "linked_sha256": rodata_sha}.items())
            or any(len(section_bytes(elf, ".rodata")) != 24 or digest(section_bytes(elf, ".rodata")) != rodata_sha
                   for elf in (originals["seta018.cpp"], isolated["native_chip_io"], placed["native_chip_io"]))):
        raise SystemExit("complete ST018 diagnostic string/target data digest drift")
    for spec in SPECS:
        unit = Path(spec["source_file"]).stem
        old_elf, local_elf, linked_elf = originals[spec["reference_unit"]], isolated[unit], placed[unit]
        old, local, final = old_elf.find_symbol(spec["reference_symbol"]), local_elf.find_symbol(spec["symbol"]), linked_elf.find_symbol(spec["symbol"])
        size, address = int(spec["size"]), int(spec["address"], 0)
        raw, linked_raw = local_elf.symbol_bytes(local, size), linked_elf.symbol_bytes(final, size)
        if (any(s.size != size for s in (old, local, final)) or final.value != address
                or raw != old_elf.symbol_bytes(old, size)
                or digest(raw) != spec["historical_raw_sha256"] or digest(raw) != spec["isolated_raw_sha256"]
                or normalized(local_elf, local) != normalized(old_elf, old)
                or digest(normalized(local_elf, local)) != spec["normalized_sha256"]
                or len(local_elf.relocation_ranges(local, size)) != int(spec["relocations"])
                or digest(linked_raw) != spec["linked_sha256"]):
            raise SystemExit("native instructions/addends/full linked digest drift: " + spec["symbol"])
        if spec["reference_unit"] != "frontend-leaves":
            reference = references[spec["reference_unit"]]
            ref = reference.find_symbol(spec["reference_symbol"])
            if ref.value != address or ref.size != size or linked_raw != reference.symbol_bytes(ref, size):
                raise SystemExit("complete selected historical reference drift: " + spec["symbol"])
        with (ROOT / spec["evidence"]).open(newline="") as stream:
            witnesses = [r for r in csv.DictReader(stream, delimiter="\t") if r["address"] == spec["address"]]
        expected_symbol = "snes_leaf_001701fc" if spec["symbol"] == "S9xGetST018" else spec["reference_symbol"]
        if (len(witnesses) != 1 or witnesses[0]["result"] != "MATCH" or witnesses[0]["differing_bytes"] != "0"
                or witnesses[0]["unknown_relocations"] or witnesses[0]["normalized_equal"] != "True"
                or witnesses[0]["object_symbol"] != expected_symbol or witnesses[0]["object_size"] != str(size)):
            raise SystemExit("frozen native controller/chip IO witness drift: " + spec["symbol"])
        if spec["target_sha256"]:
            witness = witnesses[0]
            if (spec["proof_level"] != "provider-linked-target" or digest(linked_raw) != spec["target_sha256"]
                    or (witness.get("target_span_sha256", "") != spec["target_sha256"]
                        and not (witness["raw_equal"] == "True" and int(spec["relocations"]) == 0))):
                raise SystemExit("complete target instruction proof drift: " + spec["symbol"])
        elif spec["proof_level"] != "linked-historical-reference":
            raise SystemExit("unsupported native proof level")
        print(f"native controller/chip IO: MATCH {spec['symbol']} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native controller/chip IO evidence ledger drift")
    print("native controller/chip IO proof: 10/10 routines; 1548 historical instruction bytes; "
          "672 complete target instruction bytes; 24 complete target data bytes; shared native state retained")


if __name__ == "__main__":
    main()
