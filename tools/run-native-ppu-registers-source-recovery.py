#!/usr/bin/env python3
"""Prove native PPU/CPU registers, horizontal timer and OAM/CGRAM helpers.

All 13580 instruction bytes match fully linked original historical source;
6292 bytes additionally match complete previously frozen target hashes.
Both original VRAM tables and all 2724 jump-table bytes retain target placement.
Shared CPU/PPU/APU/DMA state is reused through existing providers. Unselected
historical context is never executed or promoted. No private-image rerun or
whole-image claim is inferred from this public source proof.
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
BUILD = ROOT / "build/matching/native-ppu-registers-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
SOURCE = "src/snes9x/native_ppu_registers.cpp"
EVIDENCE = ROOT / "analysis/functions/native_ppu_registers_exact_13580.tsv"
FLAGS = SOURCE_FIXED_FLAGS[SOURCE]
PROVIDERS = {'DAT_00345268': 3428968,
 'DAT_00345498': 3429528,
 'DAT_003454e0': 3429600,
 'DAT_0034e2b0': 3465904,
 'DAT_0035b738': 3520312,
 'DAT_0035b788': 3520392,
 'DAT_0035c268': 3523176,
 'DAT_0035d360': 3527520,
 'DAT_0035d410': 3527696,
 'DAT_0035fda0': 3538336,
 'DAT_003f2eb0': 4140720,
 'DAT_003f4bf0': 4148208,
 'DAT_003f4bf4': 4148212,
 'S9xDoDMA': 1219316,
 'S9xFixColourBrightness': 1413544,
 'S9xGetSA1': 1434264,
 'S9xGetSPC7110': 1577968,
 'S9xSetIRQ': 1138984,
 'S9xSetSA1': 1434636,
 'S9xSuperFXExec': 1430324,
 'S9xUpdateScreen': 1371220,
 '_Z10S9xGetSRTCt': 1588188,
 '_Z10S9xSetSRTCht': 1587876,
 '_Z12FxFlushCachev': 1244828,
 '_Z13S9xSetSPC7110ht': 1579948,
 '_Z18FxCacheWriteAccesst': 1244764,
 '_Z19S9xSetSDD1MemoryMapjj': 1505712,
 '_ZN7CMemory11FixROMSpeedEv': 1390016,
 '__divdi3': 1711544,
 '__gxx_personality_v0': 0,
 '__muldi3': 1710880,
 'g_APU_003453b8': 3429304,
 'g_CPU_blob': 3429184,
 'g_OpenBus_byte': 3520360,
 'rand': 1083956}
LEGACY_NAMES = {'DAT_00345268': 'missing',
 'DAT_00345498': 'IAPU',
 'DAT_003454e0': 'Settings',
 'DAT_0034e2b0': 'Memory',
 'DAT_0035b738': 'SNESGameFixes',
 'DAT_0035b788': 'PPU',
 'DAT_0035c268': 'IPPU',
 'DAT_0035d360': 'DMA',
 'DAT_0035d410': 'HDMAMemPointers',
 'DAT_0035fda0': 'GetBank',
 'DAT_003f2eb0': 'SignExtend',
 'DAT_003f4bf0': 'justifiers',
 'DAT_003f4bf4': 'in_bit',
 'g_APU_003453b8': 'APU',
 'g_CPU_blob': 'CPU',
 'g_OpenBus_byte': 'OpenBus'}
SPECS = [{'address': '0x00159058',
  'symbol': '_Z15S9xUpdateHTimerv',
  'source_file': 'src/snes9x/native_ppu_registers.cpp',
  'size': '336',
  'relocations': '21',
  'historical_raw_sha256': '91a558fcff00854e6e7f751c9c85c9f636f137fd9c1ed68579c2013e727e0a42',
  'isolated_raw_sha256': '91a558fcff00854e6e7f751c9c85c9f636f137fd9c1ed68579c2013e727e0a42',
  'normalized_sha256': '5cbcfa48f8a6194ef45f28790f485e7f2260168bd34369c746ded1c30c8af410',
  'linked_sha256': 'c0e7e6523b857dc491c85dfed8c3d7628a08c1153896e394a14f31d572e1303f',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x00159268',
  'symbol': 'S9xSetPPU',
  'source_file': 'src/snes9x/native_ppu_registers.cpp',
  'size': '5000',
  'relocations': '356',
  'historical_raw_sha256': 'd99f5465d5d911b941f5c985bdccee68d1f0b04eeea82e34c0186e5b1a13ca80',
  'isolated_raw_sha256': 'edb5ce9297b94eda4dc132c1a509f09966a56f51aa82786a4ecd68ab2b84549d',
  'normalized_sha256': '7b2d9e4ef60cb4583ce68f2ad5cc3fbf1f7d989c180b7dfc8eaa71bbbe042661',
  'linked_sha256': 'c3159c2619767a4e145c394f8d9a2964fa20e2e0a3412f3511ab20a4abaf9782',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt1000plus-v46-validated-42.tsv'},
 {'address': '0x0015a5f0',
  'symbol': 'S9xGetPPU',
  'source_file': 'src/snes9x/native_ppu_registers.cpp',
  'size': '1916',
  'relocations': '132',
  'historical_raw_sha256': 'f1b0e7e84640c632cf3cfa73a949ae43994f88c4af5ce5683346745e27d4e0a8',
  'isolated_raw_sha256': 'f1b0e7e84640c632cf3cfa73a949ae43994f88c4af5ce5683346745e27d4e0a8',
  'normalized_sha256': '7f3f5cdbd85bef1ec68139a11a925b3bebd683111f755492a1054ef4b727ccd0',
  'linked_sha256': '665f0c485c5b840a0c6bf2adbb7270537921f4976b6dcc0c346fc65efe349850',
  'target_sha256': '665f0c485c5b840a0c6bf2adbb7270537921f4976b6dcc0c346fc65efe349850',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/matching/hunt1041-v52-validated-17.tsv'},
 {'address': '0x0015ad6c',
  'symbol': 'S9xSetCPU',
  'source_file': 'src/snes9x/native_ppu_registers.cpp',
  'size': '3844',
  'relocations': '185',
  'historical_raw_sha256': '1c91394b507be8f334704291849b0a00c847f2e931c255392dd2e65462851057',
  'isolated_raw_sha256': '1c91394b507be8f334704291849b0a00c847f2e931c255392dd2e65462851057',
  'normalized_sha256': 'd8d83edc032353ebc01485acfcfaac4d707b5d0cf3015633f0171860dcf7f394',
  'linked_sha256': '3990a1e071cda330fd6b538ed2f2a3aaadf72a68801f02b275ea1fd1e8bd411f',
  'target_sha256': '3990a1e071cda330fd6b538ed2f2a3aaadf72a68801f02b275ea1fd1e8bd411f',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/matching/hunt1041-v52-validated-17.tsv'},
 {'address': '0x0015bc70',
  'symbol': 'S9xGetCPU',
  'source_file': 'src/snes9x/native_ppu_registers.cpp',
  'size': '1204',
  'relocations': '95',
  'historical_raw_sha256': '2398a11e5dbd5ef733d874def890f0795fac609ed6b22ee5727e1d4931e5c403',
  'isolated_raw_sha256': '2398a11e5dbd5ef733d874def890f0795fac609ed6b22ee5727e1d4931e5c403',
  'normalized_sha256': 'da41c6d7280f86c673ab2547eb1018ffca0671625367c6eabf110a0f670248c0',
  'linked_sha256': 'ae627c8c72b1daf4f073bba5b6dfb73c2d002bb109ed710f743e3d51734b92af',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015d3ec',
  'symbol': '_Z13REGISTER_2104h',
  'source_file': 'src/snes9x/native_ppu_registers.cpp',
  'size': '748',
  'relocations': '18',
  'historical_raw_sha256': 'ad730aa8fce958e7d92903c9681dac0156c7c89352a67000ed6475faa9fe1153',
  'isolated_raw_sha256': 'ad730aa8fce958e7d92903c9681dac0156c7c89352a67000ed6475faa9fe1153',
  'normalized_sha256': '3a516e2967137fdf5edb46e16cf400610733d759b440c5cd2e4f9ea2dc2dc496',
  'linked_sha256': '9d24ac102fca82c1e51176fcb0c7c8bcb079aaff1885cef714a28d94cfa10661',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015d6d8',
  'symbol': '_Z13REGISTER_2122h',
  'source_file': 'src/snes9x/native_ppu_registers.cpp',
  'size': '532',
  'relocations': '34',
  'historical_raw_sha256': 'f7f88072110dfba0def6df371c932266ab2d9a123156db5dbf5d0b83d1ca663b',
  'isolated_raw_sha256': 'f7f88072110dfba0def6df371c932266ab2d9a123156db5dbf5d0b83d1ca663b',
  'normalized_sha256': '0e7cbfeadb3a2392b6023e27d6050f2924cf7084b0e8155935e17f0924be0a9a',
  'linked_sha256': 'b6f6d33c2c930eed15a938f4cf28a230404ace5cafffeaee081c5f054114a67c',
  'target_sha256': 'b6f6d33c2c930eed15a938f4cf28a230404ace5cafffeaee081c5f054114a67c',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/matching/hunt1041-v49-validated-20.tsv'}]


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
    if FLAGS[-2:] != ("-ffunction-sections", "-fdata-sections"):
        raise SystemExit("native register section profile drift")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v52_closure as recipe
    recipe.v47.ensure_git_commit(recipe.v47.PS2DEV, recipe.v47.PS2DEV_REPO,
                                 recipe.v47.PS2DEV_COMMIT)
    BUILD.mkdir(parents=True, exist_ok=True)
    old_build = recipe.BUILD
    try:
        recipe.BUILD = BUILD
        root, upstream, layout = recipe.prepare_snes_layout()
        recipe.patch_sources(layout)
        recipe.write_compat_headers(BUILD / "compat")
    finally:
        recipe.BUILD = old_build
    newlib = recipe.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    includes = ["-I" + str(p) for p in
                (BUILD / "compat", newlib, layout, layout / "unzip", root / "zlib")]
    original_path = BUILD / "ppu.historical.o"
    state_path = BUILD / "globals.historical.o"
    for source, output in ((layout / "ppu.cpp", original_path),
                           (layout / "GLOBALS.CPP", state_path)):
        run([CXX, *FLAGS[:-2], "-DZLIB", *includes, "-x", "c++", "-c", source,
             "-o", output])
    obj = BUILD / "native_ppu_registers.o"
    run([CXX, *FLAGS, "-c", ROOT / SOURCE, "-o", obj])
    original, isolated, state = (ELFFile(p) for p in (original_path, obj, state_path))
    window_sha = "00dfe755f9cf62c03e0109de99c58eddf554f8a226a59d775b870a0fa69da17a"
    window = [r for r in json.loads((ROOT / "analysis/link_identity/code_windows.json")
              .read_text())["result"]["selected_sources"] if r["name"] == "ppu"]
    if (len(section_bytes(original, ".text")) != 18580
            or digest(section_bytes(original, ".text")) != window_sha
            or len(window) != 1 or any(window[0][k] != v for k, v in
                {"address": 0x159058, "size": 18580, "raw_sha256": window_sha,
                 "relocations": 1061}.items())):
        raise SystemExit("frozen complete historical PPU code window drift")
    state_sha = "8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    owners = json.loads((ROOT / "analysis/link_identity/historical_data.json")
                        .read_text())["owners"]
    owner = [r for r in owners if r["unit"] == "globals" and r["symbol"] == "@section:.data"]
    if (len(section_bytes(state, ".data")) != 0xaf848
            or digest(section_bytes(state, ".data")) != state_sha
            or len(owner) != 1 or any(owner[0][k] != v for k, v in
                {"address": 0x345060, "size": 0xaf848, "source_offset": 0,
                 "sha256": state_sha}.items())):
        raise SystemExit("frozen complete original shared-state geometry drift")
    expected_sizes = {"CPU": 104, "Settings": 328, "PPU": 2780, "IPPU": 4340,
                      "Memory": 54404, "APU": 224, "IAPU": 60, "DMA": 176,
                      "missing": 174, "SNESGameFixes": 7, "OpenBus": 1,
                      "HDMAMemPointers": 32, "GetBank": 1, "SignExtend": 4,
                      "justifiers": 4, "in_bit": 1}
    for name, historical_name in LEGACY_NAMES.items():
        is_ppu = historical_name in ("justifiers", "in_bit")
        symbol = (original if is_ppu else state).find_symbol(historical_name)
        base = 0x3f4bf0 if is_ppu else 0x345060
        if symbol.value + base != PROVIDERS[name] or symbol.size != expected_sizes[historical_name]:
            raise SystemExit("native register shared-provider layout drift: " + name)
    functions = [s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    data_objects = [s for s in isolated.symbols if s.info & 15 == 1 and s.size
                    and 0 < s.section_index < len(isolated.sections)
                    and isolated.sections[s.section_index].name.startswith((".data", ".bss"))]
    expected_tables = {"_ZZ9S9xSetPPUE8IncCount": bytes.fromhex("0000200040008000"),
                       "_ZZ9S9xSetPPUE5Shift": bytes.fromhex("0000050006000700")}
    if ({s.name for s in functions} != {r["symbol"] for r in SPECS}
            or sum(s.size for s in functions) != 13580
            or {s.name for s in data_objects} != set(expected_tables)
            or len(section_bytes(isolated, ".rodata")) != 2724):
        raise SystemExit("native register functions/tables/shared-storage inventory drift")
    for symbol in data_objects:
        if symbol.size != 8 or isolated.symbol_bytes(symbol, 8) != expected_tables[symbol.name]:
            raise SystemExit("original native VRAM increment/shift table drift")
    imports = {s.name for s in isolated.symbols if s.section_index == 0 and s.name}
    if imports != set(PROVIDERS):
        raise SystemExit("native register provider ABI/import roster drift")
    script = BUILD / "ppu-registers.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
        + "SECTIONS {\n" + "".join(
            f".text.{i} {int(r['address'], 0):#x} : {{ *(.text.{r['symbol']}) }}\n"
            for i, r in enumerate(SPECS))
        + ".rodata 0x1b7318 : { *(.rodata*) } "
          ".data.inc 0x3f4bf8 : { *(.data.*_ZZ9S9xSetPPUE8IncCount) } "
          ".data.shift 0x3f4c00 : { *(.data.*_ZZ9S9xSetPPUE5Shift) } "
          "/DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) "
          "*(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked_path = BUILD / "ppu-registers.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked_path])
    linked = ELFFile(linked_path)
    bindings = {LEGACY_NAMES.get(name, name): address for name, address in PROVIDERS.items()}
    # Unselected historical context is comparison-only and never executed.
    for symbol in original.symbols:
        if symbol.section_index == 0 and symbol.name and symbol.name not in bindings:
            bindings[symbol.name] = 0
    script = BUILD / "ppu.historical.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in bindings.items())
        + "SECTIONS { .text 0x159058 : { *(.text) } "
          ".rodata 0x1b7318 : { *(.rodata*) } .data 0x3f4bf0 : { *(.data*) } "
          ".bss 0x7fc00000 : { *(.bss*) *(COMMON) } /DISCARD/ : { *(.reginfo) "
          "*(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    reference_path = BUILD / "ppu.historical.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, original_path, "-o", reference_path])
    reference = ELFFile(reference_path)
    if section_bytes(linked, ".rodata") != section_bytes(reference, ".rodata")[:2724]:
        raise SystemExit("complete native register jump-table bytes/placement drift")
    for name, address in (("_ZZ9S9xSetPPUE8IncCount", 0x3f4bf8),
                          ("_ZZ9S9xSetPPUE5Shift", 0x3f4c00)):
        placed, historical = linked.find_symbol(name), reference.find_symbol(name)
        if (placed.value != address or historical.value != address
                or linked.symbol_bytes(placed, 8) != reference.symbol_bytes(historical, 8)):
            raise SystemExit("native register VRAM-table bytes/placement drift")
    for spec in SPECS:
        name, size, address = spec["symbol"], int(spec["size"]), int(spec["address"], 0)
        old, local, placed, ref = (elf.find_symbol(name)
                                  for elf in (original, isolated, linked, reference))
        raw = linked.symbol_bytes(placed, size)
        if (any(symbol.size != size for symbol in (old, local, placed, ref))
                or placed.value != address or ref.value != address
                or digest(original.symbol_bytes(old, size)) != spec["historical_raw_sha256"]
                or digest(isolated.symbol_bytes(local, size)) != spec["isolated_raw_sha256"]
                or normalized(original, old) != normalized(isolated, local)
                or digest(normalized(isolated, local)) != spec["normalized_sha256"]
                or len(isolated.relocation_ranges(local, size)) != int(spec["relocations"])
                or raw != reference.symbol_bytes(ref, size)
                or digest(raw) != spec["linked_sha256"]):
            raise SystemExit("native register full instructions/provider-linked reference drift: " + name)
        with (ROOT / spec["evidence"]).open(newline="") as stream:
            witnesses = [r for r in csv.DictReader(stream, delimiter="\t")
                         if r["address"] == spec["address"]]
        if (len(witnesses) != 1 or witnesses[0]["result"] != "MATCH"
                or witnesses[0]["differing_bytes"] != "0" or witnesses[0]["unknown_relocations"]
                or witnesses[0]["normalized_equal"] != "True"
                or witnesses[0]["object_symbol"] != name or witnesses[0]["object_size"] != str(size)):
            raise SystemExit("frozen normalized native register witness drift: " + name)
        if spec["target_sha256"]:
            if (spec["proof_level"] != "provider-linked-target"
                    or witnesses[0]["target_span_sha256"] != spec["target_sha256"]
                    or digest(raw) != spec["target_sha256"]):
                raise SystemExit("complete provider-linked native register target digest drift: " + name)
        elif spec["proof_level"] != "linked-historical-reference":
            raise SystemExit("unsupported native register proof claim: " + name)
        print(f"native registers: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native register source evidence ledger drift")
    print("native register source proof: 7/7 routines; 13580 linked historical bytes; "
          "6292 complete provider-linked target bytes; shared state and VRAM tables preserved")


if __name__ == "__main__":
    main()
