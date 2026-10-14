#!/usr/bin/env python3
"""Prove fifteen native ROM/RAM maps and their original shared providers.

All 12,816 selected instructions match a fully linked historical reference;
2,116 also match independently frozen complete target hashes. Only the Tales
literal LO16 and two scratch-provider LO16 addends change upon isolation.
Earlier LoROM/BSLoROM witnesses retain
three older Settings field addends; reproduce and bound them explicitly rather
than treating relocation masks as proof of native state layout. No private
image recapture or replacement-image comparison is claimed.
"""
from __future__ import annotations
import csv
import hashlib
import json
import shutil
import subprocess
import sys
from pathlib import Path
from build_source_tree import SOURCE_FIXED_FLAGS
from compare_elf_functions import ELFFile
from code_windows import DIRECT_MEMMAP_CORE_OBJECT

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-rom-maps-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
SOURCE = "src/snes9x/native_rom_maps.cpp"
EVIDENCE = ROOT / "analysis/functions/native_rom_maps_exact_12816.tsv"
PROVIDERS = {
    "DAT_0034e2b0": 0x34e2b0, "DAT_003454e0": 0x3454e0,
    "DAT_0034e298": 0x34e298, "DAT_00426820": 0x426820,
    "g_SA1_blob": 0x345af8, "_ZN7CMemory15WriteProtectROMEv": 0x153608,
    "memmove": 0x19c4a0, "memset": 0x19c39c, "memcmp": 0x19c458,
    "_Z14S9xSpc7110Initv": 0x1806a4, "__gxx_personality_v0": 0x1a9728,
}
SPECS = [{'address': '0x00153674',
  'symbol': '_ZN7CMemory6MapRAMEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '268',
  'relocations': '3',
  'historical_raw_sha256': 'a2ce71c1dc798bb1e7b4757983af0a1803a780c9e13eb9d8916e40287ec31c51',
  'isolated_raw_sha256': 'a2ce71c1dc798bb1e7b4757983af0a1803a780c9e13eb9d8916e40287ec31c51',
  'normalized_sha256': '1e6fd564a3d7995fb08c02507114db20607c0a705fcd6669d7bd7cb6197d2f2d',
  'evidence': 'analysis/matching/hunt500plus-v29-validated-2.tsv',
  'frozen_source_raw_sha256': 'a2ce71c1dc798bb1e7b4757983af0a1803a780c9e13eb9d8916e40287ec31c51',
  'linked_sha256': 'c6231c1723e14a6e838d8e90be8de8063bf57ce75663efce607a3656c63c83f8',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x00153780',
  'symbol': '_ZN7CMemory11MapExtraRAMEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '304',
  'relocations': '2',
  'historical_raw_sha256': 'f799ca06e4c8d81ef854a5a1ae87564f67b21719df126fed9d64f3fdf50473f7',
  'isolated_raw_sha256': 'f799ca06e4c8d81ef854a5a1ae87564f67b21719df126fed9d64f3fdf50473f7',
  'normalized_sha256': 'f799ca06e4c8d81ef854a5a1ae87564f67b21719df126fed9d64f3fdf50473f7',
  'evidence': 'analysis/matching/hunt500-v10-validated-17.tsv',
  'frozen_source_raw_sha256': 'f799ca06e4c8d81ef854a5a1ae87564f67b21719df126fed9d64f3fdf50473f7',
  'linked_sha256': '26143745b61aa07114408fe8b260e22dba79db40298ef0e6926427e9c90f8c35',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x001538b0',
  'symbol': '_ZN7CMemory8LoROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '1268',
  'relocations': '16',
  'historical_raw_sha256': 'f04db9f6ed7aebf37733d8afafc5af17a21178de79cd1150cd37f5fb8bbee70d',
  'isolated_raw_sha256': 'ffb07732e4ed05e83ca7bc7894d698b826f82f04f3c6b5b4e87165880e7c0d12',
  'normalized_sha256': 'd1b44ab9541354ca438b285aec8181cb98ea00a4b60f1e3f35d81b87638f3a20',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': '5b0a07b4c56512e4c6ca8cf84ca49bf0c57cedd2de9a48ab5eabc734f8b57659',
  'linked_sha256': 'bf6d02755641d99b717fd25df7b8fa904d5a021da58b4d1594840940451a018b',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x00153da4',
  'symbol': '_ZN7CMemory10BSLoROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '1084',
  'relocations': '4',
  'historical_raw_sha256': 'ddbfc35ca380938114652c4ea83f1e503591b843122a4b1d452620d1a1839853',
  'isolated_raw_sha256': 'ddbfc35ca380938114652c4ea83f1e503591b843122a4b1d452620d1a1839853',
  'normalized_sha256': 'ba3a1ba60286490629d1c827f32ca87373412b33ff93c265614757be3ff7e59e',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': '90eac3cbce9023916c34a7b2dd59aefc77df609f25737d37f3ac455024136272',
  'linked_sha256': '66525e9943ccc19ab62b93776d268ad279c138c906c9edd0d9890b1b5ee7b23e',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x001541e0',
  'symbol': '_ZN7CMemory8HiROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '940',
  'relocations': '6',
  'historical_raw_sha256': '2adc38554f8ef6b61b73e97bf605a313df79111ceefdfbadd40328939db348eb',
  'isolated_raw_sha256': '2adc38554f8ef6b61b73e97bf605a313df79111ceefdfbadd40328939db348eb',
  'normalized_sha256': 'b029d8e7c4418202a8d273f4ec0b831d0c057921ed8e616aab5c5945609627e6',
  'evidence': 'analysis/matching/hunt1041-v51-validated-16.tsv',
  'frozen_source_raw_sha256': '2adc38554f8ef6b61b73e97bf605a313df79111ceefdfbadd40328939db348eb',
  'linked_sha256': '6fab244a1444315cfef6a995dfe53edf34cb35089766a947255635ee2eadc17b',
  'target_sha256': '6fab244a1444315cfef6a995dfe53edf34cb35089766a947255635ee2eadc17b',
  'proof_level': 'provider-linked-target'},
 {'address': '0x0015458c',
  'symbol': '_ZN7CMemory11TalesROMMapEh',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '1176',
  'relocations': '9',
  'historical_raw_sha256': '590950ab7cbea22f0239ea69a9a489fd0e2e5a645d534ae2d98e0bfebd0562ae',
  'isolated_raw_sha256': '24f17c767b7c511f43a23037732623c5a306675c42687a6beb30d3bc43d585f0',
  'normalized_sha256': '9c543718288408da0fd1c5aaadb565371259194529035596ee9886085c42d23f',
  'evidence': 'analysis/matching/hunt1041-v51-validated-16.tsv',
  'frozen_source_raw_sha256': '6f7afaba9d174d17040030f693584226d2a4d3b514930076541f278faff2d1a2',
  'linked_sha256': 'dc113ba1690ef758b87d96f6f5671b52817c07deecf164b948deee524950a144',
  'target_sha256': 'dc113ba1690ef758b87d96f6f5671b52817c07deecf164b948deee524950a144',
  'proof_level': 'provider-linked-target'},
 {'address': '0x00154a24',
  'symbol': '_ZN7CMemory11AlphaROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '520',
  'relocations': '2',
  'historical_raw_sha256': '38c42b5364820c2cf49e5b87947ef26fdb844d64d877652b3bd25a3f180cae40',
  'isolated_raw_sha256': '38c42b5364820c2cf49e5b87947ef26fdb844d64d877652b3bd25a3f180cae40',
  'normalized_sha256': '38c42b5364820c2cf49e5b87947ef26fdb844d64d877652b3bd25a3f180cae40',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': '38c42b5364820c2cf49e5b87947ef26fdb844d64d877652b3bd25a3f180cae40',
  'linked_sha256': 'f177bab06c17940a21079dddedb6d467605c5b7f44e25a92f5fa6157e00aa72d',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x00154c2c',
  'symbol': '_ZN7CMemory13SuperFXROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '880',
  'relocations': '7',
  'historical_raw_sha256': '4df6742db7f19e7f59f120f625a363fd8c49837bf3fd5f33a48427f7f967c461',
  'isolated_raw_sha256': '4df6742db7f19e7f59f120f625a363fd8c49837bf3fd5f33a48427f7f967c461',
  'normalized_sha256': '4df6742db7f19e7f59f120f625a363fd8c49837bf3fd5f33a48427f7f967c461',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': '4df6742db7f19e7f59f120f625a363fd8c49837bf3fd5f33a48427f7f967c461',
  'linked_sha256': 'fe4ccccb5cc1a55044f02123e8162b13803e9d3860aa8bd28abe8576671518e2',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x00154f9c',
  'symbol': '_ZN7CMemory9SA1ROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '920',
  'relocations': '13',
  'historical_raw_sha256': '7f1a4cb587216cd4d48447cfca0361b7308c2fcc4b17870bc711f0da90a2ed96',
  'isolated_raw_sha256': '7f1a4cb587216cd4d48447cfca0361b7308c2fcc4b17870bc711f0da90a2ed96',
  'normalized_sha256': '4aaf05c4f566d610f1d4d0d7beea265c27521c583fb15b80591842b6af759d52',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': '7f1a4cb587216cd4d48447cfca0361b7308c2fcc4b17870bc711f0da90a2ed96',
  'linked_sha256': '820faf6758a883f19f11bdfc1bbd9d79ed2c6ebbdfb43b601f98518c886e225a',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x00155334',
  'symbol': '_ZN7CMemory13LoROM24MBSMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '856',
  'relocations': '2',
  'historical_raw_sha256': 'e9d2492b76524887df188a663aaeecde13217e8c6efde2b7d0ba5a5b9cd2f220',
  'isolated_raw_sha256': 'e9d2492b76524887df188a663aaeecde13217e8c6efde2b7d0ba5a5b9cd2f220',
  'normalized_sha256': 'e9d2492b76524887df188a663aaeecde13217e8c6efde2b7d0ba5a5b9cd2f220',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': 'e9d2492b76524887df188a663aaeecde13217e8c6efde2b7d0ba5a5b9cd2f220',
  'linked_sha256': 'e70582ae6b681942bd345d06937b30fdf61993698521e5ba886a64d555977c15',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x0015568c',
  'symbol': '_ZN7CMemory19SufamiTurboLoROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '952',
  'relocations': '7',
  'historical_raw_sha256': '8827f55998520c2a2286be5ecf1d8898d66888b6bfa62a7aa174ff7df157c51b',
  'isolated_raw_sha256': '8827f55998520c2a2286be5ecf1d8898d66888b6bfa62a7aa174ff7df157c51b',
  'normalized_sha256': 'f3d60f5f5b5860c69599770c6cb70f73bcedb425f634ed4ea7f53367fb177dac',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': '8827f55998520c2a2286be5ecf1d8898d66888b6bfa62a7aa174ff7df157c51b',
  'linked_sha256': '70015d64acbd675defcf51481ba79f51d33f483027843385ac6fd97c9f4b32cb',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x00155a44',
  'symbol': '_ZN7CMemory16SRAM512KLoROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '604',
  'relocations': '2',
  'historical_raw_sha256': 'defff5f967f14f6759d5ad89d15eefacb159cd76a92434aa62600f6e2ff097b6',
  'isolated_raw_sha256': 'defff5f967f14f6759d5ad89d15eefacb159cd76a92434aa62600f6e2ff097b6',
  'normalized_sha256': 'defff5f967f14f6759d5ad89d15eefacb159cd76a92434aa62600f6e2ff097b6',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': 'defff5f967f14f6759d5ad89d15eefacb159cd76a92434aa62600f6e2ff097b6',
  'linked_sha256': '2ba4b0e89c9ffbc5c1b200a1a2e6e72d09dec6dc1ced26131029673c06096305',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x00155ca0',
  'symbol': '_ZN7CMemory10BSHiROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '1144',
  'relocations': '2',
  'historical_raw_sha256': '7e05eb56bc1fa2061185ea7bc2ead6f548bf599707dc8922b841c4593a70181c',
  'isolated_raw_sha256': '7e05eb56bc1fa2061185ea7bc2ead6f548bf599707dc8922b841c4593a70181c',
  'normalized_sha256': '7e05eb56bc1fa2061185ea7bc2ead6f548bf599707dc8922b841c4593a70181c',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': '7e05eb56bc1fa2061185ea7bc2ead6f548bf599707dc8922b841c4593a70181c',
  'linked_sha256': 'ab3f0b8a4dcc2f6491429c14435cb520f636474f1fa07c8d8b62ea556638cc75',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x00156118',
  'symbol': '_ZN7CMemory13JumboLoROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '1020',
  'relocations': '8',
  'historical_raw_sha256': '8f015679260de47dfeda7c9946f3bfbe34c993b4ddc22d54d5cffb148459010f',
  'isolated_raw_sha256': '22f8114db4863c97509c70da464da2d7cb6c5fdd9ce82c1cf548d64087a8b5a3',
  'normalized_sha256': 'd19ff1d3ae25ed6d3c0eff7e59dfb33efc515c05b191364eae476e11f04bd616',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': '8f015679260de47dfeda7c9946f3bfbe34c993b4ddc22d54d5cffb148459010f',
  'linked_sha256': '5afd48894c305f72a4c54fc422252a3bb234d1c94d4d99339f19d6313e856dcd',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'},
 {'address': '0x00156514',
  'symbol': '_ZN7CMemory15SPC7110HiROMMapEv',
  'source_file': 'src/snes9x/native_rom_maps.cpp',
  'size': '880',
  'relocations': '3',
  'historical_raw_sha256': '533052aa2d42ede74f63379f306e5ae5a60028e110866780d987e6f56cbbcc10',
  'isolated_raw_sha256': '533052aa2d42ede74f63379f306e5ae5a60028e110866780d987e6f56cbbcc10',
  'normalized_sha256': '533052aa2d42ede74f63379f306e5ae5a60028e110866780d987e6f56cbbcc10',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'frozen_source_raw_sha256': '533052aa2d42ede74f63379f306e5ae5a60028e110866780d987e6f56cbbcc10',
  'linked_sha256': '273ccde22c0f057589585f1a4dde77b9fa1600d3a48f1097be4c5a14f1c1b96b',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference'}]


def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command)) + "\n" + result.stdout[-10000:])
    return result.stdout


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def section_bytes(elf, name):
    section = next(s for s in elf.sections if s.name == name)
    if section.type == 8:
        raise SystemExit("cannot read zero-fill section as stored bytes")
    return elf.data[section.offset:section.offset + section.size]


def normalized(elf, symbol):
    raw = bytearray(elf.symbol_bytes(symbol, symbol.size))
    for relocation in elf.relocation_masks(symbol, symbol.size):
        if not relocation.known:
            raise SystemExit("unknown relocation: " + symbol.name)
        for index, mask in enumerate(relocation.mask_bytes):
            raw[relocation.start + index] &= 255 ^ mask
    return bytes(raw)


def place(name, path, providers, sections):
    script = BUILD / (name + ".ld")
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in providers.items())
        + "SECTIONS {\n" + sections
        + "\n/DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    output = BUILD / (name + ".elf")
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, path, "-o", output])
    return ELFFile(output)


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    flags = SOURCE_FIXED_FLAGS[SOURCE]
    if flags != SOURCE_FIXED_FLAGS["src/snes9x/native_rom_deinterleave.cpp"] or flags[-1] != "-ffunction-sections":
        raise SystemExit("native ROM map compiler profile drift")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v52_closure as recipe
    recipe.v47.ensure_git_commit(recipe.v47.PS2DEV, recipe.v47.PS2DEV_REPO, recipe.v47.PS2DEV_COMMIT)
    BUILD.mkdir(parents=True, exist_ok=True)
    saved = recipe.BUILD
    try:
        recipe.BUILD = BUILD / "profile"
        root, upstream, layout = recipe.prepare_snes_layout()
        recipe.patch_sources(layout)
        recipe.write_compat_headers(BUILD / "compat")
    finally:
        recipe.BUILD = saved
    newlib = recipe.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"

    def compile_reference(folder, source, output):
        includes = ["-I" + str(p) for p in (BUILD / "compat", newlib, folder, folder / "unzip", root / "zlib")]
        path = BUILD / output
        run([CXX, *flags[:-1], "-DZLIB", *includes, "-x", "c++", "-c", folder / source, "-o", path])
        return ELFFile(path)

    historical = compile_reference(layout, "MEMMAP.CPP", "MEMMAP.historical.o")
    state = compile_reference(layout, "GLOBALS.CPP", "GLOBALS.historical.o")
    old_layout = BUILD / "legacy-settings"
    shutil.copytree(layout, old_layout, dirs_exist_ok=True)
    header = old_layout / "snes9x.h"
    text = header.read_text()
    marker = "    bool8  PS2PortLayoutByte;\n"
    if text.count(marker) != 1:
        raise SystemExit("native Settings port-byte context drift")
    header.write_text(text.replace(marker, ""))
    legacy = compile_reference(old_layout, "MEMMAP.CPP", "MEMMAP.legacy-settings.o")
    # V51 retained the unpatched upstream diagnostic before the Tales literal.
    # Compile that actual source with the native headers to reproduce its +8
    # rodata displacement; do not invent an arbitrary relocation adjustment.
    old_literal = BUILD / "upstream-literal"
    shutil.copytree(layout, old_literal, dirs_exist_ok=True)
    shutil.copyfile(upstream / "MEMMAP.CPP", old_literal / "MEMMAP.CPP")
    prior = compile_reference(old_literal, "MEMMAP.CPP", "MEMMAP.upstream-literal.o")
    prior_rodata = section_bytes(prior, ".rodata")
    if len(prior_rodata) != 4360 or prior_rodata[0x358:0x360] != b"TALES\0\0\0":
        raise SystemExit("frozen Tales literal source context drift")
    path = BUILD / "native_rom_maps.o"
    run([CXX, *flags, "-c", ROOT / SOURCE, "-o", path])
    local = ELFFile(path)
    functions = [s for s in local.symbols if s.info & 15 == 2 and s.size]
    storage = [s for s in local.symbols if s.info & 15 == 1 and s.size
               and 0 < s.section_index < len(local.sections)
               and local.sections[s.section_index].name.startswith((".data", ".bss"))]
    if ({s.name for s in functions} != {r["symbol"] for r in SPECS}
            or sum(s.size for s in functions) != 12816 or any(s.info >> 4 != 1 for s in functions)
            or storage or {s.name for s in local.symbols if s.section_index == 0 and s.name} != set(PROVIDERS)):
        raise SystemExit("native ROM map code/shared-storage/provider inventory drift")
    # GCC 3.2.2 emits frame records into .data on this target. They are
    # compiler metadata, with no source storage objects or scratch allocation.
    frame_relocs = next(s for s in local.sections if s.name == ".rel.data")
    if (len(section_bytes(local, ".data")) != 592
            or digest(section_bytes(local, ".data")) != "3481a31e497db3058864a3ba97b142cfc95bc33fbed3e19ec38a8979490671cc"
            or frame_relocs.size != 15 * frame_relocs.entry_size):
        raise SystemExit("native ROM map compiler frame metadata drift")
    owners = json.loads((ROOT / "analysis/link_identity/historical_data.json").read_text())
    state_sha = "8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    owner = [r for r in owners["owners"] if r["unit"] == "globals" and r["symbol"] == "@section:.data"]
    if (len(section_bytes(state, ".data")) != 0xaf848 or digest(section_bytes(state, ".data")) != state_sha
            or len(owner) != 1 or any(owner[0][k] != v for k, v in
                {"address": 0x345060, "size": 0xaf848, "source_offset": 0, "sha256": state_sha}.items())):
        raise SystemExit("complete frozen GLOBALS state digest drift")
    for name, address, size in (("Settings", 0x3454e0, 328), ("Memory", 0x34e2b0, 54404),
                                ("SRAM", 0x34e298, 4), ("SA1", 0x345af8, 32856)):
        symbol = state.find_symbol(name)
        if symbol.value + 0x345060 != address or symbol.size != size:
            raise SystemExit("native ROM shared-state geometry drift: " + name)
    # bytes0x2000 is zero-fill storage, not a stored file payload. Keep the
    # existing target provider; the canonical unit allocates no second buffer.
    bss = next(s for s in historical.sections if s.name == ".bss")
    scratch = historical.find_symbol("bytes0x2000")
    if (bss.type != 8 or bss.size != 8288 or scratch.value != 0 or scratch.section_index != bss.index
            or dict(DIRECT_MEMMAP_CORE_OBJECT[3])[".bss"] != (PROVIDERS["DAT_00426820"] + 0x6000)):
        raise SystemExit("native ROM scratch zero-fill/provider geometry drift")
    pseudocode = (ROOT / "analysis/functions/progress16_r5900_pseudocode.c.txt").read_text()
    for address in ("0x001538b0", "0x00156118"):
        body = pseudocode.split("/* ===== " + address + " ===== */", 1)[1].split("/* =====", 1)[0]
        if "&DAT_00426820" not in body or (PROVIDERS["DAT_00426820"] + 0x6000) - 0x6000 != 0x426820:
            raise SystemExit("independent LoROM scratch target reference drift")
    lorom = pseudocode.split("/* ===== 0x001538b0 ===== */", 1)[1].split("/* =====", 1)[0]
    bslorom = pseudocode.split("/* ===== 0x00153da4 ===== */", 1)[1].split("/* =====", 1)[0]
    if ("DAT_00345610._4_1_" not in lorom or "DAT_0034560d" not in bslorom
            or PROVIDERS["DAT_003454e0"] + 0x134 != 0x345614
            or PROVIDERS["DAT_003454e0"] + 0x12d != 0x34560d):
        raise SystemExit("independent native Settings SETA/BS field address drift")
    spc = pseudocode.split("/* ===== 0x00156514 ===== */", 1)[1].split("/* =====", 1)[0]
    if "FUN_001806a4();" not in spc or PROVIDERS["_Z14S9xSpc7110Initv"] != 0x1806a4:
        raise SystemExit("native SPC7110 initialization callee/address drift")
    old_rodata = section_bytes(historical, ".rodata")
    prefix_sha = "d77ceaa56edfbe58df4448df1c1cf77ef7295707acafda54f47e012f6eedc7be"
    prefixes = json.loads((ROOT / "analysis/link_identity/window11_rodata.json").read_text())["source_sections"]
    prefix = [r for r in prefixes if r["name"] == "memmap_prefix"]
    if (len(old_rodata) != 4352
            or digest(old_rodata) != "9137276f30f4eff45412264c0a20379b487f2114b4f037a3aa9ed31ba4816c63"
            or digest(old_rodata[:2848]) != prefix_sha or len(prefix) != 1
            or any(prefix[0][k] != v for k, v in {"address": 0x1b63d8, "size": 2848,
                    "source_offset": 0, "raw_sha256": prefix_sha, "linked_sha256": prefix_sha}.items())):
        raise SystemExit("complete frozen MEMMAP literal window drift")
    literal = section_bytes(local, ".rodata")
    if literal != b"TALES\0\0\0" or literal != old_rodata[0x350:0x358]:
        raise SystemExit("complete native Tales literal drift")
    linked = place("native-rom-maps", path, PROVIDERS,
        "".join(f".text.{i} {int(r['address'], 0):#x} : {{ *(.text.{r['symbol']}) }}\n" for i, r in enumerate(SPECS))
        + ".rodata 0x1b6728 : { *(.rodata*) }\n/DISCARD/ : { *(.data*) *(.bss*) *(COMMON) }")
    bindings = {n.replace("DAT_0034e2b0", "Memory").replace("DAT_003454e0", "Settings")
                  .replace("DAT_0034e298", "SRAM").replace("g_SA1_blob", "SA1"): a
                for n, a in PROVIDERS.items() if n not in {"DAT_00426820", "_ZN7CMemory15WriteProtectROMEv"}}
    # Only selected map instructions are compared. All selected callees have
    # their genuine addresses; unrelated historical context remains unproved.
    for symbol in historical.symbols:
        if symbol.name and symbol.section_index == 0:
            bindings.setdefault(symbol.name, 0)
    reference = place("historical-rom-maps", historical.path, bindings,
        ".text 0x150a74 : { *(.text) }\n.rodata 0x1b63d8 : { *(.rodata*) }\n"
        ".data 0x3f4910 : { *(.data*) }\n.bss 0x42c820 : { *(.bss*) *(COMMON) }")
    windows = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    if section_bytes(linked, ".rodata") != literal:
        raise SystemExit("linked Tales data drift")
    legacy_changes = {"_ZN7CMemory8LoROMMapEv": {0xe0: ("33014380", "34014380"), 0x3c8: ("33014380", "34014380")},
                      "_ZN7CMemory10BSLoROMMapEv": {0x18: ("2c014290", "2d014290")}}
    for spec in SPECS:
        name, size, address = spec["symbol"], int(spec["size"]), int(spec["address"], 0)
        old, isolated, final, ref = (e.find_symbol(name) for e in (historical, local, linked, reference))
        old_raw, raw, final_raw = (e.symbol_bytes(s, size) for e, s in ((historical, old), (local, isolated), (linked, final)))
        frozen_elf = legacy if name in legacy_changes else prior if name == "_ZN7CMemory11TalesROMMapEh" else historical
        frozen = frozen_elf.find_symbol(name)
        frozen_raw = frozen_elf.symbol_bytes(frozen, size)
        captures = [r for r in windows if r["address"] == address and r["symbol"] == name]
        with (ROOT / spec["evidence"]).open(newline="") as stream:
            witnesses = [r for r in csv.DictReader(stream, delimiter="\t") if r["address"] == spec["address"] and r["object_symbol"] == name]
        if (len(captures) != 1 or any(captures[0][k] != v for k, v in
                {"size": size, "new_bytes": size, "relocations": int(spec["relocations"]), "raw_sha256": spec["frozen_source_raw_sha256"]}.items())
                or len(witnesses) != 1 or any(witnesses[0][k] != v for k, v in
                    {"object_size": str(size), "result": "MATCH", "differing_bytes": "0", "normalized_equal": "True", "unknown_relocations": ""}.items())
                or any(s.size != size for s in (old, isolated, final, ref, frozen))
                or digest(frozen_raw) != spec["frozen_source_raw_sha256"]
                or digest(old_raw) != spec["historical_raw_sha256"] or digest(raw) != spec["isolated_raw_sha256"]
                or normalized(frozen_elf, frozen) != normalized(historical, old)
                or normalized(local, isolated) != normalized(historical, old)
                or digest(normalized(local, isolated)) != spec["normalized_sha256"]
                or local.relocation_masks(isolated, size) != historical.relocation_masks(old, size)
                or len(local.relocation_ranges(isolated, size)) != int(spec["relocations"])
                or final.value != address or ref.value != address
                or final_raw != reference.symbol_bytes(ref, size) or digest(final_raw) != spec["linked_sha256"]):
            raise SystemExit("native ROM map frozen source/linked code drift: " + name)
        changes = legacy_changes.get(name, {0x374: ("58038424", "50038424")} if name == "_ZN7CMemory11TalesROMMapEh" else {})
        if {i for i in range(0, size, 4) if frozen_raw[i:i+4] != old_raw[i:i+4]} != set(changes):
            raise SystemExit("unbounded historical state/literal correction: " + name)
        for offset, (before, after) in changes.items():
            if (frozen_raw[offset:offset+4].hex() != before or old_raw[offset:offset+4].hex() != after
                    or not any(r.start == offset and r.relocation_type == 6 for r in historical.relocation_masks(old, size))):
                raise SystemExit("historical correction is not the verified LO16 field: " + name)
        # Bind the original 8 KiB scratch buffer through the already reviewed
        # bank-biased provider. The explicit assembler expression preserves the
        # historical pointer calculation and changes only this verified LO16.
        isolated_changes = {
            "_ZN7CMemory8LoROMMapEv": {0x4dc: ("00004224", "00604224")},
            "_ZN7CMemory13JumboLoROMMapEv": {0x3f0: ("00004224", "00604224")},
            "_ZN7CMemory11TalesROMMapEh": {0x374: ("50038424", "00008424")},
        }.get(name, {})
        if {i for i in range(0, size, 4) if old_raw[i:i+4] != raw[i:i+4]} != set(isolated_changes):
            raise SystemExit("unbounded native source/provider addend drift: " + name)
        for offset, (before, after) in isolated_changes.items():
            if (old_raw[offset:offset+4].hex() != before or raw[offset:offset+4].hex() != after
                    or not any(r.start == offset and r.relocation_type == 6 for r in local.relocation_masks(isolated, size))):
                raise SystemExit("native source correction is not its verified LO16: " + name)
        if spec["proof_level"] == "provider-linked-target":
            if witnesses[0].get("target_span_sha256") != spec["target_sha256"] or digest(final_raw) != spec["target_sha256"]:
                raise SystemExit("complete native ROM map target digest drift: " + name)
        elif spec["proof_level"] != "linked-historical-reference" or spec["target_sha256"]:
            raise SystemExit("native ROM map proof scope drift")
        print(f"native ROM map: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native ROM map evidence ledger drift")
    print("Native ROM maps proof: 15/15 functions; 12816 linked historical instruction bytes; "
          "2116 complete target code bytes; 8 complete target literal bytes; original shared providers retained")


if __name__ == "__main__":
    main()
