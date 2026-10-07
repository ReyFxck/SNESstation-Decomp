#!/usr/bin/env python3
"""Prove the remaining native Snes9x 1.41-1 SA-1 module.

All 16 routines (6172 bytes) match fully linked historical source; eight
routines (1632 bytes) also match complete committed target assembly. The
original 632 jump-table bytes match the frozen target digest. Shared state
and the four opcode-table providers retain original geometry; no storage or
initializer is duplicated. This public proof makes no new private-image claim.
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
from build_source_tree import SOURCE_FIXED_FLAGS

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-sa1-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
SOURCE = "src/snes9x/native_sa1.cpp"
EVIDENCE = ROOT / "analysis/functions/native_sa1_exact_6172.tsv"
FLAGS = SOURCE_FIXED_FLAGS[SOURCE]
PROVIDERS = {'DAT_00345ae8': 3431144,
 'DAT_0034e2b0': 3465904,
 'DAT_003f5040': 4149312,
 'DAT_003f5440': 4150336,
 'DAT_003f5840': 4151360,
 'DAT_003f5c40': 4152384,
 'S9xClearIRQ': 1139060,
 'S9xSetIRQ': 1138984,
 '__gxx_personality_v0': 0,
 'g_CPU_blob': 3429184,
 'g_OpenBus_byte': 3520360,
 'g_SA1_blob': 3431160,
 'memmove': 1688736,
 'printf': 1696648,
 'puts': 1696788}
LEGACY_NAMES = {'DAT_00345ae8': 'SA1Registers',
 'DAT_0034e2b0': 'Memory',
 'DAT_003f5040': 'S9xSA1OpcodesM1X1',
 'DAT_003f5440': 'S9xSA1OpcodesM1X0',
 'DAT_003f5840': 'S9xSA1OpcodesM0X0',
 'DAT_003f5c40': 'S9xSA1OpcodesM0X1',
 'g_CPU_blob': 'CPU',
 'g_OpenBus_byte': 'OpenBus',
 'g_SA1_blob': 'SA1'}
SPECS = [{'address': '0x0015d9ac',
  'symbol': '_Z11S9xSA1Resetv',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '264',
  'relocations': '12',
  'historical_raw_sha256': '47be430dab0bd3dcb5b7e6bac8dbedc8ea68265adfa7be3b7e3ebcadfb1bc3e2',
  'isolated_raw_sha256': 'c52c864127cbb004a205c467260294caa318b9a99e178f9893544386726d2e37',
  'normalized_sha256': 'c52c864127cbb004a205c467260294caa318b9a99e178f9893544386726d2e37',
  'linked_sha256': '228be3e9bca08cf6c36445c2254db836d7d9d9449a508768ce458090e1e9fd1c',
  'target_sha256': '228be3e9bca08cf6c36445c2254db836d7d9d9449a508768ce458090e1e9fd1c',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'matching_evidence': 'analysis/matching/hunt400-validated-19.tsv'},
 {'address': '0x0015dab4',
  'symbol': '_Z20S9xSA1SetBWRAMMemMaph',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '192',
  'relocations': '8',
  'historical_raw_sha256': '988b8bd37d309d784a518077f73e123772dbaf192bd9a9e4daff7f014f4bb5d8',
  'isolated_raw_sha256': '988b8bd37d309d784a518077f73e123772dbaf192bd9a9e4daff7f014f4bb5d8',
  'normalized_sha256': '7d539f7e3a46be1fbe4d5e97825b0c19d595014b721d78cd4a2f86d4f91c607f',
  'linked_sha256': '5f21a93bf8dba4fcf48d349130054bad11be6d849ed258a0b165939c79b55017',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'matching_evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015db74',
  'symbol': 'S9xFixSA1AfterSnapshotLoad',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '272',
  'relocations': '14',
  'historical_raw_sha256': 'eafc8183d950c1cabfaeb835ce7f4831b4916f398edf6eeea848f31ef67e59ed',
  'isolated_raw_sha256': '3d937ed33a7f79e9da4d09f178744f1a7cb1db95e11ff1314570dd0a1df9b4c4',
  'normalized_sha256': 'd6531f6a10b602beb29d5e39c93ed57d62529f3e0672d6dc157c2105611a625b',
  'linked_sha256': '80e033e1af9af68a10abc4a910f28240f1f6869d3fe7645f3520d0501c25b3f0',
  'target_sha256': '80e033e1af9af68a10abc4a910f28240f1f6869d3fe7645f3520d0501c25b3f0',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'matching_evidence': 'analysis/matching/hunt400-validated-19.tsv'},
 {'address': '0x0015dc84',
  'symbol': 'S9xSA1GetByte',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '400',
  'relocations': '20',
  'historical_raw_sha256': '78aeeac9783fb68a7fc718a57314eb4d6b5a031e9c395ef58c8a26f96549a7d0',
  'isolated_raw_sha256': '78aeeac9783fb68a7fc718a57314eb4d6b5a031e9c395ef58c8a26f96549a7d0',
  'normalized_sha256': '96690b94930d7af6665435130be0218cceb346b1e7b8506afc1e644994c0de3d',
  'linked_sha256': 'd87be91f8ea508c401f4ba820b07c741e665432d8565bca153ee5425d1c56bf7',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'matching_evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015de14',
  'symbol': 'S9xSA1GetWord',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '84',
  'relocations': '4',
  'historical_raw_sha256': 'fc29b9fa25b7733972e5b65d30d8469cacce30f019ca94a84d25de1ad5e0c7ca',
  'isolated_raw_sha256': 'fc29b9fa25b7733972e5b65d30d8469cacce30f019ca94a84d25de1ad5e0c7ca',
  'normalized_sha256': 'fc29b9fa25b7733972e5b65d30d8469cacce30f019ca94a84d25de1ad5e0c7ca',
  'linked_sha256': 'e98546fc27ae70d8a3e2334c1878ae77c3c7bdd8f8f881c7d91fe1b76b4c5643',
  'target_sha256': 'e98546fc27ae70d8a3e2334c1878ae77c3c7bdd8f8f881c7d91fe1b76b4c5643',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/functions/progress11_short_targets.asm',
  'matching_evidence': 'analysis/matching/hunt400-validated-19.tsv'},
 {'address': '0x0015de68',
  'symbol': 'S9xSA1SetByte',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '460',
  'relocations': '16',
  'historical_raw_sha256': 'f23bac3dfe81923eb9bbed128a7759f9c5d89aacc0f333ba1682a8aeab7bfd26',
  'isolated_raw_sha256': 'f23bac3dfe81923eb9bbed128a7759f9c5d89aacc0f333ba1682a8aeab7bfd26',
  'normalized_sha256': '6bc804142125e9959460dcb5dae53b1389a232351187f465740c12c72f6bec78',
  'linked_sha256': '5f1ae05e254ee127e57a2941c96a12fea31af4b0752e3493c28c1400ad408dbc',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'matching_evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015e034',
  'symbol': 'S9xSA1SetWord',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '72',
  'relocations': '2',
  'historical_raw_sha256': 'e485a09466202d389ff67539485e56d1f6e0c9e514512eaed9093160161f0f38',
  'isolated_raw_sha256': 'e485a09466202d389ff67539485e56d1f6e0c9e514512eaed9093160161f0f38',
  'normalized_sha256': 'e485a09466202d389ff67539485e56d1f6e0c9e514512eaed9093160161f0f38',
  'linked_sha256': '3e5b9ea2996d0528ef1bd07d13ebd3c2697302db756e77ed679142ec81263e66',
  'target_sha256': '3e5b9ea2996d0528ef1bd07d13ebd3c2697302db756e77ed679142ec81263e66',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/functions/progress11_short_targets.asm',
  'matching_evidence': 'analysis/matching/hunt400-validated-19.tsv'},
 {'address': '0x0015e07c',
  'symbol': 'S9xSA1SetPCBase',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '276',
  'relocations': '27',
  'historical_raw_sha256': '59443bda53d3b7c3cec037f8edbff29ea41cb4b18150c6347e9d426d153a92e8',
  'isolated_raw_sha256': '59443bda53d3b7c3cec037f8edbff29ea41cb4b18150c6347e9d426d153a92e8',
  'normalized_sha256': '0f2514bae153eeaa267b5a5517bcf3a57142cfe1fa59790aa0381d9b123d472e',
  'linked_sha256': 'a03b631ddad2adf6a263a155a829dbf10f843bc3d09bf38268b26d998d7e0eff',
  'target_sha256': 'a03b631ddad2adf6a263a155a829dbf10f843bc3d09bf38268b26d998d7e0eff',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'matching_evidence': 'analysis/matching/hunt400-validated-19.tsv'},
 {'address': '0x0015e190',
  'symbol': 'S9xSA1ExecuteDuringSleep',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '8',
  'relocations': '0',
  'historical_raw_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'isolated_raw_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'normalized_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'linked_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'matching_evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015e198',
  'symbol': '_Z15S9xSetSA1MemMapjh',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '256',
  'relocations': '12',
  'historical_raw_sha256': '1f12ba2962806040f4b392e470055de06dba71645e3e87a57e559eb34b4ed411',
  'isolated_raw_sha256': '1f12ba2962806040f4b392e470055de06dba71645e3e87a57e559eb34b4ed411',
  'normalized_sha256': '94e4b685272d667a097bafee570f901a4734d6dc45e8639a5d542582edcabac3',
  'linked_sha256': '0a91261ab6a4905f6e39bafcd0375778e70f5095065cb172c45e4bd9c09511c9',
  'target_sha256': '0a91261ab6a4905f6e39bafcd0375778e70f5095065cb172c45e4bd9c09511c9',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'matching_evidence': 'analysis/matching/hunt400-validated-19.tsv'},
 {'address': '0x0015e298',
  'symbol': 'S9xGetSA1',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '372',
  'relocations': '28',
  'historical_raw_sha256': 'fbfe735c4ef4182e392352791662e8f6c07325b5eba5cd8b52b8b81569c7b581',
  'isolated_raw_sha256': '8383806abd3bfdd3729015d093187f7a99ae3e7f754551d3f429d02c3443348f',
  'normalized_sha256': '4a18ca3b48a49c24dcb0bd0b2731fdcd74c808093aa190870705e1ac073c838a',
  'linked_sha256': '53f9db553317e993d99111c52713739254eeac883122a58215936a46b6421617',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'matching_evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015e40c',
  'symbol': 'S9xSetSA1',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '2272',
  'relocations': '134',
  'historical_raw_sha256': 'da2dec8fb18e79245e8fa93d895186501420e8ed4caf66ad9ab35f038bc95c74',
  'isolated_raw_sha256': '7c6ff4d4df29dd982b2bfde7f5ecf05471759583d39f1b7693cdb5e256c366a5',
  'normalized_sha256': '41e1e17b51260591516dc8572dff0459a3ed91423831f4a80505ae81a5545df7',
  'linked_sha256': '341ad56dbcd5434c74590090d87a56223cef03fb4adf33a41c904b0dedc1a638',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'matching_evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015ecec',
  'symbol': '_Z15S9xSA1CharConv2v',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '448',
  'relocations': '6',
  'historical_raw_sha256': 'ddb54decd668235f186dc533baff47e7c7446c2eca086d05edd9dfc3feefba9c',
  'isolated_raw_sha256': 'ddb54decd668235f186dc533baff47e7c7446c2eca086d05edd9dfc3feefba9c',
  'normalized_sha256': '97da7dddbe87af9027fd10016acecd7b1aaedceba4b302924a554f33a9f75073',
  'linked_sha256': 'e2383fc9fd3b7fe14b4841c07478db47454bf2ec974d8158998879d09ff6bcb6',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'matching_evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015eeac',
  'symbol': '_Z9S9xSA1DMAv',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '388',
  'relocations': '13',
  'historical_raw_sha256': '8d7582eee4c7d1b241c97fcfa532e4a9a3140319b18788b790c1cfa4fe513f53',
  'isolated_raw_sha256': '8d7582eee4c7d1b241c97fcfa532e4a9a3140319b18788b790c1cfa4fe513f53',
  'normalized_sha256': 'ac59a6e34fe37afb492a9300211a73f588a6c8e1514a8cdd665f2a00976f9a16',
  'linked_sha256': '328498093053e8b45ec7dbe5ec6dfe4c2d86f2d9f6995cc4097bc870e813acf0',
  'target_sha256': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'matching_evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0015f030',
  'symbol': '_Z28S9xSA1ReadVariableLengthDatahh',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '300',
  'relocations': '8',
  'historical_raw_sha256': '5b6d45a8b54c5a5d0c79ed6b069df2ff16183e21427e04cc7e0cad5c3b514e80',
  'isolated_raw_sha256': '5b6d45a8b54c5a5d0c79ed6b069df2ff16183e21427e04cc7e0cad5c3b514e80',
  'normalized_sha256': 'dd9c21f9f627dd48ff4039d4b6effd6e6eb063f71b8a441fd0941218be952a8e',
  'linked_sha256': '325a716d66dc0c65ae269f17cb953187f021849f81c09ef32c53eb002fcf7bf9',
  'target_sha256': '325a716d66dc0c65ae269f17cb953187f021849f81c09ef32c53eb002fcf7bf9',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'matching_evidence': 'analysis/matching/hunt400-validated-19.tsv'},
 {'address': '0x0015f15c',
  'symbol': '_Z15S9xSA1FixCyclesv',
  'source_file': 'src/snes9x/native_sa1.cpp',
  'size': '108',
  'relocations': '13',
  'historical_raw_sha256': '4c03cc84bd77711916e171d73525e393e4be08d5e19ae3802350bfe5c6da9298',
  'isolated_raw_sha256': '4c03cc84bd77711916e171d73525e393e4be08d5e19ae3802350bfe5c6da9298',
  'normalized_sha256': '4c03cc84bd77711916e171d73525e393e4be08d5e19ae3802350bfe5c6da9298',
  'linked_sha256': 'b7a708bdfd25b13ecebc6090fe0cf036ba4c7a65a73c68ed210371033631fd66',
  'target_sha256': 'b7a708bdfd25b13ecebc6090fe0cf036ba4c7a65a73c68ed210371033631fd66',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/functions/progress11_short_targets.asm',
  'matching_evidence': ''}]


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


def target_bytes(spec):
    address, size = int(spec["address"], 0), int(spec["size"])
    pattern = re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} ){3}[0-9a-fA-F]{2})\s")
    words = {}
    for line in (ROOT / spec["evidence"]).read_text().splitlines():
        match = pattern.match(line)
        if match and address <= int(match[1], 16) < address + size:
            at = int(match[1], 16)
            if at in words:
                raise SystemExit("duplicate target instruction: " + spec["symbol"])
            words[at] = bytes.fromhex(match[2])
    if set(words) != set(range(address, address + size, 4)):
        raise SystemExit("incomplete frozen target listing: " + spec["symbol"])
    return b"".join(words[at] for at in range(address, address + size, 4))


def main():
    if (run([CXX, "-dumpmachine"]).strip() != "ee"
            or run([CXX, "-dumpversion"]).strip() != "3.2.2"):
        raise SystemExit("wrong historical compiler")
    if FLAGS[-1] != "-ffunction-sections":
        raise SystemExit("native SA-1 section profile drift")
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
    historical_paths = [BUILD / (name + ".historical.o")
                        for name in ("sa1", "GLOBALS", "SA1CPU")]
    for source, output in zip(("sa1.cpp", "GLOBALS.CPP", "SA1CPU.CPP"), historical_paths):
        run([CXX, *FLAGS[:-1], "-DZLIB", *includes, "-x", "c++", "-c",
             layout / source, "-o", output])
    obj = BUILD / "native_sa1.o"
    run([CXX, *FLAGS, "-c", ROOT / SOURCE, "-o", obj])
    original, state, opcodes = (ELFFile(p) for p in historical_paths)
    isolated = ELFFile(obj)
    text = section_bytes(original, ".text")
    window_sha = "1c6f98d059ba43b236482c6750d57c47e001027c4d742a8802813a67da8c7a81"
    windows = json.loads((ROOT / "analysis/link_identity/code_windows.json")
                         .read_text())["result"]["selected_sources"]
    window = [r for r in windows if r["name"] == "sa1"]
    if (len(text) != 6364
            or digest(text) != "a183d3d23d631112ed665733a0d04a57a326dc588a70c0be142722eefd98893d"
            or digest(text[:6256]) != window_sha
            or len(window) != 1 or any(window[0][k] != v for k, v in
                {"address": 0x15d8ec, "size": 6256, "raw_sha256": window_sha,
                 "relocations": 309, "source_offset": 0}.items())):
        raise SystemExit("frozen historical SA-1 code window drift")
    original_functions = [s for s in original.symbols if s.info & 15 == 2 and s.size]
    initializer = original.find_symbol("S9xSA1Init")
    if ({s.name for s in original_functions} != {r["symbol"] for r in SPECS} | {"S9xSA1Init"}
            or initializer.size != 192 or initializer.value != 0
            or sum(s.size for s in original_functions) != 6364):
        raise SystemExit("original SA-1 module/initializer inventory drift")
    state_sha = "8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    owners = json.loads((ROOT / "analysis/link_identity/historical_data.json")
                        .read_text())["owners"]
    owner = [r for r in owners if r["unit"] == "globals" and r["symbol"] == "@section:.data"]
    if (len(section_bytes(state, ".data")) != 0xaf848
            or digest(section_bytes(state, ".data")) != state_sha
            or len(owner) != 1 or any(owner[0][k] != v for k, v in
                {"address": 0x345060, "size": 0xaf848, "source_offset": 0,
                 "sha256": state_sha}.items())):
        raise SystemExit("frozen complete shared-state geometry drift")
    opcode_sha = "be8548846f24b7a7bc2ab6d16eaa0ed930b13f1a3ecc05640efd9d440b23c9e6"
    tables = json.loads((ROOT / "analysis/link_identity/historical_tail_data.json")
                        .read_text())["providers"]
    table = [r for r in tables if r["name"] == "sa1cpu"]
    if (len(section_bytes(opcodes, ".data")) != 21872
            or digest(section_bytes(opcodes, ".data")) != opcode_sha
            or len(table) != 1 or any(table[0][k] != v for k, v in
                {"filename": "SA1CPU.CPP", "full_size": 21872, "size": 21872,
                 "full_sha256": opcode_sha, "slice_sha256": opcode_sha,
                 "target_address": 0x3f5040, "relocations": 1411,
                 "source_offset": 0, "profile": "v46-official"}.items())):
        raise SystemExit("frozen complete SA-1 opcode-table source drift")
    expected_sizes = {"CPU": 104, "Memory": 54404, "SA1": 32856,
                      "SA1Registers": 16, "OpenBus": 1}
    for name, historical_name in LEGACY_NAMES.items():
        is_table = historical_name.startswith("S9xSA1Opcodes")
        symbol = (opcodes if is_table else state).find_symbol(historical_name)
        base, size = (0x3f5040, 1024) if is_table else (0x345060, expected_sizes[historical_name])
        if symbol.value + base != PROVIDERS[name] or symbol.size != size:
            raise SystemExit("SA-1 shared-provider geometry drift: " + name)
    functions = [s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    writable = [s for s in isolated.symbols if s.info & 15 == 1 and s.size
                and 0 < s.section_index < len(isolated.sections)
                and isolated.sections[s.section_index].name.startswith((".data", ".bss"))]
    local_helpers = {"_Z15S9xSA1CharConv2v", "_Z9S9xSA1DMAv",
                     "_Z28S9xSA1ReadVariableLengthDatahh", "_Z15S9xSA1FixCyclesv"}
    if ({s.name for s in functions} != {r["symbol"] for r in SPECS}
            or sum(s.size for s in functions) != 6172 or writable
            or {s.name for s in functions if s.info >> 4 == 0} != local_helpers
            or len(section_bytes(isolated, ".rodata")) != 632):
        raise SystemExit("native SA-1 functions/local helpers/shared-storage inventory drift")
    imports = {s.name for s in isolated.symbols if s.section_index == 0 and s.name}
    if imports != set(PROVIDERS):
        raise SystemExit("native SA-1 provider ABI/import roster drift")
    script = BUILD / "native_sa1.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
        + "SECTIONS {\n" + "".join(
            f".text.{i} {int(r['address'], 0):#x} : {{ *(.text.{r['symbol']}) }}\n"
            for i, r in enumerate(SPECS))
        + ".rodata 0x1b7dd8 : { *(.rodata*) } /DISCARD/ : { *(.data*) *(.bss*) "
          "*(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked_path = BUILD / "native_sa1.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked_path])
    linked = ELFFile(linked_path)
    bindings = {LEGACY_NAMES.get(name, name): address for name, address in PROVIDERS.items()}
    # The initializer is owned elsewhere; memset is needed only by this reference.
    bindings["memset"] = 0x19c39c
    original_imports = {s.name for s in original.symbols if s.section_index == 0 and s.name}
    if original_imports != set(bindings):
        raise SystemExit("historical SA-1 native provider roster drift")
    script = BUILD / "sa1.historical.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in bindings.items())
        + "SECTIONS { .text 0x15d8ec : { *(.text) } .rodata 0x1b7dd8 : { *(.rodata*) } "
          ".data 0x7fc00000 : { *(.data*) } .bss 0x7fd00000 : { *(.bss*) *(COMMON) } "
          "/DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    reference_path = BUILD / "sa1.historical.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, historical_paths[0], "-o", reference_path])
    reference = ELFFile(reference_path)
    ro_sha = "28fb8f6227d14fbbc7d4c5ed6a79944bb7e8cb2cd46c2888d80f860af32c0148"
    linked_ro_sha = "3840850473acf567a4928690bd09759d35c58fc572a527cd9833ae6e2fd10b8b"
    ro_sources = json.loads((ROOT / "analysis/link_identity/window11_rodata.json")
                            .read_text())["source_sections"]
    ro_source = [r for r in ro_sources if r["name"] == "sa1"]
    if (digest(section_bytes(original, ".rodata")) != ro_sha
            or section_bytes(linked, ".rodata") != section_bytes(reference, ".rodata")
            or digest(section_bytes(linked, ".rodata")) != linked_ro_sha
            or len(ro_source) != 1 or any(ro_source[0][k] != v for k, v in
                {"address": 0x1b7dd8, "full_size": 632, "size": 632,
                 "raw_sha256": ro_sha, "full_sha256": ro_sha,
                 "linked_sha256": linked_ro_sha, "relocations": 142,
                 "source_offset": 0, "source_section": ".rodata"}.items())):
        raise SystemExit("complete SA-1 jump-table source/target bytes drift")
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
            raise SystemExit("native SA-1 full instructions/provider-linked reference drift: " + name)
        if spec["matching_evidence"]:
            with (ROOT / spec["matching_evidence"]).open(newline="") as stream:
                witnesses = [r for r in csv.DictReader(stream, delimiter="\t")
                             if r["address"] == spec["address"]]
            if (len(witnesses) != 1 or witnesses[0]["result"] != "MATCH"
                    or witnesses[0]["differing_bytes"] != "0" or witnesses[0]["unknown_relocations"]
                    or witnesses[0]["normalized_equal"] != "True"
                    or witnesses[0]["object_symbol"] != name or witnesses[0]["object_size"] != str(size)):
                raise SystemExit("frozen normalized SA-1 witness drift: " + name)
        if spec["target_sha256"]:
            if (spec["proof_level"] != "provider-linked-target"
                    or raw != target_bytes(spec) or digest(raw) != spec["target_sha256"]):
                raise SystemExit("complete provider-linked SA-1 target instructions drift: " + name)
        elif spec["proof_level"] != "linked-historical-reference":
            raise SystemExit("unsupported native SA-1 proof claim: " + name)
        print(f"native SA-1: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native SA-1 source evidence ledger drift")
    if sum(int(r["size"]) for r in SPECS if r["target_sha256"]) != 1632:
        raise SystemExit("complete native SA-1 target-proof scope drift")
    print("native SA-1 source proof: 16/16 routines; 6172 linked historical bytes; "
          "1632 complete provider-linked target bytes; 632 target jump-table bytes; "
          "original shared-state and opcode-table geometry preserved")


if __name__ == "__main__":
    main()
