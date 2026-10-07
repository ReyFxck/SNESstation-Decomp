#!/usr/bin/env python3
"""Prove the complete original native 3456-byte, 21-function Super FX module.

The frozen raw complete code window, 2280-byte original state/data section
and 6672-byte instruction-table data section are rebuilt from pinned public
sources. Native storage is reused through extern aliases, with no substitute
GSU or dispatch arrays. Every linked instruction matches a complete linked
historical reference; no fresh private-target code digest or image rerun is
claimed. The already frozen complete state/data target digest is retained.
"""
from __future__ import annotations
import csv
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from compare_elf_functions import ELFFile
ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build/matching/native-fxemu-source-recovery"
CXX=ROOT/"build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE=ROOT/"analysis/functions/native_fxemu_exact_3456.tsv"
FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET')
PROVIDERS={'DAT_00342a38': 3418680,
 'DAT_00343234': 3420724,
 'DAT_00343238': 3420728,
 'DAT_0034323c': 3420732,
 'DAT_00343240': 3420736,
 'DAT_00343250': 3420752,
 'DAT_00343260': 3420768,
 'DAT_00343268': 3420776,
 'DAT_00343270': 3420784,
 'PTR_LAB_00343b30': 3423024,
 'memset': 1688476}
LEGACY_PROVIDERS={'fx_apfFunctionTable': 3423008,
 'fx_apfOpcodeTable': 3423064,
 'fx_apfPlotTable': 3423024,
 'memset': 1688476}
SPECS=[{'address': '0x0012fe5c',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '1a6fc3f9b53d28bce6d5d92bf3c82462dc311457a576f3be59c1c9448ab41ae1',
  'historical_symbol': '_Z18FxCacheWriteAccesst',
  'isolated_raw_sha256': '1a6fc3f9b53d28bce6d5d92bf3c82462dc311457a576f3be59c1c9448ab41ae1',
  'linked_sha256': '5db39cb01a3580fe56e7841cd653a4232807c31bfd8280c369d9280f5cf34322',
  'normalized_sha256': '1a6fc3f9b53d28bce6d5d92bf3c82462dc311457a576f3be59c1c9448ab41ae1',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '64',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z18FxCacheWriteAccesst'},
 {'address': '0x0012fe9c',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '8f6a6d3aaad5e536d7265bb7d1c94ef029286d26001e5f29f49b31843c89f0e8',
  'historical_symbol': '_Z12FxFlushCachev',
  'isolated_raw_sha256': '8f6a6d3aaad5e536d7265bb7d1c94ef029286d26001e5f29f49b31843c89f0e8',
  'linked_sha256': 'bd3bba24840d799c9a5cb1d58174a4a85b9897685ca9f5241375b55b9118bea4',
  'normalized_sha256': '8f6a6d3aaad5e536d7265bb7d1c94ef029286d26001e5f29f49b31843c89f0e8',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '24',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z12FxFlushCachev'},
 {'address': '0x0012feb4',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'historical_symbol': '_Z14fx_backupCachev',
  'isolated_raw_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'linked_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'normalized_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'proof_level': 'linked-historical-reference',
  'relocations': '0',
  'size': '8',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z14fx_backupCachev'},
 {'address': '0x0012febc',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'historical_symbol': '_Z15fx_restoreCachev',
  'isolated_raw_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'linked_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'normalized_sha256': '6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1',
  'proof_level': 'linked-historical-reference',
  'relocations': '0',
  'size': '8',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z15fx_restoreCachev'},
 {'address': '0x0012fec4',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '69293e9cd58b5fd38a0991d506530d560d75477322f3d52c870f42556eaa5c99',
  'historical_symbol': '_Z13fx_flushCachev',
  'isolated_raw_sha256': '69293e9cd58b5fd38a0991d506530d560d75477322f3d52c870f42556eaa5c99',
  'linked_sha256': '64dfa560560ddb1e146692f7ca90fe28e68a090c815675f09e39e1b97766587a',
  'normalized_sha256': '9d3a5acdc0e7d807cb52b18823e8e5b2f0e3f47cab28a2234dded9e66ddfc88b',
  'proof_level': 'linked-historical-reference',
  'relocations': '3',
  'size': '44',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z13fx_flushCachev'},
 {'address': '0x0012fef0',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': 'fe809f6bc3ac608ecf2498a8f0341910567ac0b36e909b475aa50f6a01c511b8',
  'historical_symbol': '_Z20fx_readRegisterSpacev',
  'isolated_raw_sha256': 'c26fffc93a71cb68d24f4bdd8de49733706c693ab9b695b2d9b2e14282b465b1',
  'linked_sha256': '8d2a527156418943c8ffe42f761a005584ddc217381cf1a68ddca68860ebd686',
  'normalized_sha256': '26528d2858bfe7c2fd0382e1390f7fada918bd6fec9dc8c4e3bdbd63ca63b2b7',
  'proof_level': 'linked-historical-reference',
  'relocations': '16',
  'size': '580',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z20fx_readRegisterSpacev'},
 {'address': '0x00130134',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '207125b643ad61c496f6d3d0e18bf84f8572b306f5af0325fd255961f484f86f',
  'historical_symbol': '_Z24fx_computeScreenPointersv',
  'isolated_raw_sha256': '207125b643ad61c496f6d3d0e18bf84f8572b306f5af0325fd255961f484f86f',
  'linked_sha256': '2a048111b31dc189da6f3a739ef64c0fe30029ee9c6f02673fc2ffa30695d8f9',
  'normalized_sha256': '89d3a79f946d690a5a47c3c623813d3e26070bf172380dd499612e10f733c5db',
  'proof_level': 'linked-historical-reference',
  'relocations': '55',
  'size': '1132',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z24fx_computeScreenPointersv'},
 {'address': '0x001305a0',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': 'acf4fc90a4d58e9c9eb9575650cf1f8026e51f13a1cddb86dc3aa1670f781647',
  'historical_symbol': '_Z21fx_writeRegisterSpacev',
  'isolated_raw_sha256': 'acf4fc90a4d58e9c9eb9575650cf1f8026e51f13a1cddb86dc3aa1670f781647',
  'linked_sha256': 'afe150441cef56882357bb4157ad088e20c8755d70b7b6de40258b84818f11f3',
  'normalized_sha256': '7d057ee344093f1781a53c9bc92d723f95427cd206d0eebabfa6a725c488e917',
  'proof_level': 'linked-historical-reference',
  'relocations': '13',
  'size': '344',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z21fx_writeRegisterSpacev'},
 {'address': '0x001306f8',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '224282a56131c8f9adac0b44b1b5d968352ea6a3d993a1f8e5a86dc72104d3f9',
  'historical_symbol': '_Z7FxResetP8FxInit_s',
  'isolated_raw_sha256': '871d413ac35bb6dc6ea2534769fc882d00c8fc7103b1a5cda84884241af59f7d',
  'linked_sha256': 'b2519da1d29ee584ae4b5c69b50874edcea33856d171d0bc0a99bec9795ef397',
  'normalized_sha256': '8e2c9eeaaf8985c45549cb10948e3f0a59770df4105271d74b88fca852463481',
  'proof_level': 'linked-historical-reference',
  'relocations': '26',
  'size': '512',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': 'S9xFxReset'},
 {'address': '0x001308f8',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '3961cc86c9e4495b94d5638b7866cd8ad9550d1b70739c8883739e6b2ec1dab6',
  'historical_symbol': '_Z20fx_checkStartAddressv',
  'isolated_raw_sha256': '3961cc86c9e4495b94d5638b7866cd8ad9550d1b70739c8883739e6b2ec1dab6',
  'linked_sha256': '51c4297845ad4049d3c35a662a6ce953d6ddea94285e18adaeabb23c85b33fd2',
  'normalized_sha256': 'fb9bd07f29d2de8ca61a9073d6f6e0208ef07299d072caf78fb494a4f8b58153',
  'proof_level': 'linked-historical-reference',
  'relocations': '12',
  'size': '204',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z20fx_checkStartAddressv'},
 {'address': '0x001309c4',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': 'a783812d30e07dfef8b8084383cdfcc97e142d92b48d9409736d4aeedef2826a',
  'historical_symbol': '_Z9FxEmulatej',
  'isolated_raw_sha256': 'a783812d30e07dfef8b8084383cdfcc97e142d92b48d9409736d4aeedef2826a',
  'linked_sha256': 'e291d1eb2a4cace46aa4fd0152556b62955695381ce6a01cc15625b84566abf5',
  'normalized_sha256': 'b6fe3a440118e391a77c90382e266fb6e0f70ac26d6b374a127b264ec768fbbd',
  'proof_level': 'linked-historical-reference',
  'relocations': '12',
  'size': '180',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z9FxEmulatej'},
 {'address': '0x00130a78',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': 'cea8109aa8072a3ccee6e0fb7f17e5e9619f2ae9fa68406b1f571e37575c2651',
  'historical_symbol': '_Z15FxBreakPointSetj',
  'isolated_raw_sha256': 'cea8109aa8072a3ccee6e0fb7f17e5e9619f2ae9fa68406b1f571e37575c2651',
  'linked_sha256': '06129d0d5a66b87eba5d1ac81d3da427991bdb12c8ff0b8caf3c31b340541b64',
  'normalized_sha256': 'cea8109aa8072a3ccee6e0fb7f17e5e9619f2ae9fa68406b1f571e37575c2651',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '28',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z15FxBreakPointSetj'},
 {'address': '0x00130a94',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '58b84f45780d450a9906ff720c4cfde484685a612416cf7a30398423f3b2f010',
  'historical_symbol': '_Z17FxBreakPointClearv',
  'isolated_raw_sha256': '58b84f45780d450a9906ff720c4cfde484685a612416cf7a30398423f3b2f010',
  'linked_sha256': '8c563a0350225dad73e188ca848363ada38af3d05974b9caa99b3d61aed4277f',
  'normalized_sha256': 'c38945534ae18bf9cd3c6921962b7222956ef629601ef9714b83fac6c6efe651',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '12',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z17FxBreakPointClearv'},
 {'address': '0x00130aa0',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '1a4640188cb380336c40b7ee6f4c4fac97b2bb99bebbaf396394a06038bdd311',
  'historical_symbol': '_Z10FxStepOverj',
  'isolated_raw_sha256': '1a4640188cb380336c40b7ee6f4c4fac97b2bb99bebbaf396394a06038bdd311',
  'linked_sha256': '4e9651108562105bfdf39c87aa6f24c22f910020d5fb738170fbb2a67ddf21b6',
  'normalized_sha256': '5047f44be58c4f3c414515b7aa1462edf57aeb8b263c0aab8e1c9e47eab42914',
  'proof_level': 'linked-historical-reference',
  'relocations': '9',
  'size': '204',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z10FxStepOverj'},
 {'address': '0x00130b6c',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '9fc6602500f654ada36677375db752ed608431bea0abf9924305a4a74568af60',
  'historical_symbol': '_Z14FxGetErrorCodev',
  'isolated_raw_sha256': '9fc6602500f654ada36677375db752ed608431bea0abf9924305a4a74568af60',
  'linked_sha256': '9a16fb86ddf1efe64d5200e29ed8cab760a834f7eee11d8e22e1f3aab30f1499',
  'normalized_sha256': '309a2e9eb5035d1900e8cd83b53b2122f1eb8e361ac35550eeca5fcdaedda222',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '12',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z14FxGetErrorCodev'},
 {'address': '0x00130b78',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '9753cf89b3dd2b825a9eb481345e5be12c5b7167a0297a3f3eea7dbe999bb802',
  'historical_symbol': '_Z19FxGetIllegalAddressv',
  'isolated_raw_sha256': '9753cf89b3dd2b825a9eb481345e5be12c5b7167a0297a3f3eea7dbe999bb802',
  'linked_sha256': '8fb8784571e29b4e87059e2a103b9df3156935f37f2670bc08f0b11a50614d33',
  'normalized_sha256': '309a2e9eb5035d1900e8cd83b53b2122f1eb8e361ac35550eeca5fcdaedda222',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '12',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z19FxGetIllegalAddressv'},
 {'address': '0x00130b84',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '7fc38b6f3a7d30b659fe7fb58f166d4d29271725ae36deec617efd1639ce3144',
  'historical_symbol': '_Z18FxGetColorRegisterv',
  'isolated_raw_sha256': '7fc38b6f3a7d30b659fe7fb58f166d4d29271725ae36deec617efd1639ce3144',
  'linked_sha256': 'b51141a08755182dc8c050216727d8fcc0fdd6b89eb3ca9c00f65af8a72698a9',
  'normalized_sha256': '32bac67b88328ee43eabec6aa9a7815d74c699512fa8814eccaeeb7ac3abbd34',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '12',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z18FxGetColorRegisterv'},
 {'address': '0x00130b90',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '8837d29768f1a83ad1edb1ded9d9dc595fba921ad59ab9a51e7a3e135d8a5bc8',
  'historical_symbol': '_Z23FxGetPlotOptionRegisterv',
  'isolated_raw_sha256': '8837d29768f1a83ad1edb1ded9d9dc595fba921ad59ab9a51e7a3e135d8a5bc8',
  'linked_sha256': '65fcdc5178da5ecc6840686b76aa3bad8bd5da55338fd7abd069dd4a2b21f8ae',
  'normalized_sha256': '928530ae91fe5a27bc56cf1f714861ce2d734fecf448cf11088e919328cfbba3',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '16',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z23FxGetPlotOptionRegisterv'},
 {'address': '0x00130ba0',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '2a57ac7b93c6025ae93f399e21a638d00e8805316cf52ff0abab8b02b495314d',
  'historical_symbol': '_Z24FxGetSourceRegisterIndexv',
  'isolated_raw_sha256': '2a57ac7b93c6025ae93f399e21a638d00e8805316cf52ff0abab8b02b495314d',
  'linked_sha256': '32ca147b0a551195a54dc23c13c3a2526a5a3c7eb060fed3a80d0b393f298718',
  'normalized_sha256': '2a57ac7b93c6025ae93f399e21a638d00e8805316cf52ff0abab8b02b495314d',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '24',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z24FxGetSourceRegisterIndexv'},
 {'address': '0x00130bb8',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '11ffbf2e6cc0513c24f1a8e33f976ee25aac06ab1152ee08c2f5a7f3762e941e',
  'historical_symbol': '_Z29FxGetDestinationRegisterIndexv',
  'isolated_raw_sha256': '11ffbf2e6cc0513c24f1a8e33f976ee25aac06ab1152ee08c2f5a7f3762e941e',
  'linked_sha256': '8be1a8f3637dcda02d7392ba91df6d22a54ce82567934ce3100c43153191f282',
  'normalized_sha256': '11ffbf2e6cc0513c24f1a8e33f976ee25aac06ab1152ee08c2f5a7f3762e941e',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '24',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z29FxGetDestinationRegisterIndexv'},
 {'address': '0x00130bd0',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': 'e64f6e6a750d3020b2179233ec3d2ef6a5fbaa00df953e8e466ac5effb22ae93',
  'historical_symbol': '_Z6FxPipev',
  'isolated_raw_sha256': 'e64f6e6a750d3020b2179233ec3d2ef6a5fbaa00df953e8e466ac5effb22ae93',
  'linked_sha256': 'fa47cd5769ec6a8e116432146f000a5bdae598f5b92c3693ee5c531ec19f2953',
  'normalized_sha256': '32bac67b88328ee43eabec6aa9a7815d74c699512fa8814eccaeeb7ac3abbd34',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '12',
  'source_file': 'src/snes9x/native_fxemu.cpp',
  'symbol': '_Z6FxPipev'}]
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



def main():
    if run([CXX,"-dumpmachine"]).strip()!="ee" or run([CXX,"-dumpversion"]).strip()!="3.2.2":
        raise SystemExit("wrong historical compiler")
    sys.path.insert(0,str(ROOT/"tools/history/research"))
    import hunt1041_v52_closure as recipe
    recipe.v47.ensure_git_commit(recipe.v47.PS2DEV,recipe.v47.PS2DEV_REPO,recipe.v47.PS2DEV_COMMIT)
    BUILD.mkdir(parents=True,exist_ok=True)
    old_build=recipe.BUILD
    try:
        recipe.BUILD=BUILD/"profile"
        root,upstream,layout=recipe.prepare_snes_layout()
        recipe.patch_sources(layout)
        recipe.write_compat_headers(BUILD/"compat")
    finally:
        recipe.BUILD=old_build
    newlib=recipe.v47.PS2DEV/"ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    includes=["-I"+str(p) for p in (BUILD/"compat",newlib,layout,layout/"unzip",root/"zlib")]
    original_path=BUILD/"fxemu.historical.o"
    run([CXX,*FLAGS,"-DZLIB",*includes,"-x","c++","-c",layout/"fxemu.cpp","-o",original_path])
    instructions_path=BUILD/"fxinst.historical.o"
    run([CXX,*FLAGS,"-DZLIB",*includes,"-x","c++","-c",layout/"fxinst.cpp","-o",instructions_path])
    obj=BUILD/"native_fxemu.o"
    run([CXX,*FLAGS,"-c",ROOT/"src/snes9x/native_fxemu.cpp","-o",obj])
    original,isolated,instructions=ELFFile(original_path),ELFFile(obj),ELFFile(instructions_path)
    text=next(s for s in original.sections if s.name==".text")
    local_text=next(s for s in isolated.sections if s.name==".text")
    capture=json.loads((ROOT/"analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    capture=[r for r in capture if r.get("name")=="fxemu"]
    original_sha="9aefc576908e7478760dc65ea1bbbbad7997c6c22205fd69b8a6f2b983c93c61"
    if (len(capture)!=1 or any(capture[0].get(k)!=v for k,v in {"address":0x12fe5c,
            "size":3456,"new_bytes":3456,"source_offset":0,"relocations":168,"raw_sha256":original_sha}.items())
        or text.size!=3456 or digest(original.data[text.offset:text.offset+3456])!=original_sha
        or local_text.size!=3456 or digest(isolated.data[local_text.offset:local_text.offset+3456])!=
            "5997b4df823c878e5224697c64a5170c2541f78a67f4da7586e891ba7d52c310"):
        raise SystemExit("frozen complete native Super FX instruction window drift")
    functions=[s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    if (len(functions)!=21 or sum(s.size for s in functions)!=3456
        or {s.name for s in functions}!={r["symbol"] for r in SPECS}
        or any(s.size and s.info >> 4 in (1,2) and s.section_index < len(isolated.sections)
               and isolated.sections[s.section_index].name in (".data",".bss")
               for s in isolated.symbols if s.section_index)):
        raise SystemExit("complete native Super FX functions/shared-storage inventory drift")
    window=json.loads((ROOT/"analysis/link_identity/window36_data.json").read_text())["source_sections"]
    for elf,name,size,sha,address,relocations in (
            (original,"fxemu_data",2280,"8511be0bbfef2cc57b50c744a607816859b2e7f460ea6260efc27e759d1334ed",0x342a38,8),
            (instructions,"fxinst_data",6672,"a16f5c06dd2daa0edb6af293c87369ff97093ec4e93566698695a639b7fd4471",0x343320,1049)):
        section=next(s for s in elf.sections if s.name==".data")
        rows=[r for r in window if r["name"]==name]
        if (section.size!=size or digest(elf.data[section.offset:section.offset+size])!=sha
            or len(rows)!=1 or any(rows[0][k]!=v for k,v in {"address":address,"size":size,
                "full_size":size,"full_sha256":sha,"raw_sha256":sha,"relocations":relocations}.items())):
            raise SystemExit("frozen complete Super FX state/dispatch data drift: "+name)
    data_geometry=(
        ("GSU",0x342a38,2044),("fx_ppfFunctionTable",0x343234,4),
        ("fx_ppfPlotTable",0x343238,4),("fx_ppfOpcodeTable",0x34323c,4),
        ("_ZZ20fx_readRegisterSpacevE8avHeight",0x343240,16),
        ("_ZZ20fx_readRegisterSpacevE6avMult",0x343250,16),
        ("_ZZ7FxResetP8FxInit_sE12appfFunction",0x343260,4),
        ("_ZZ7FxResetP8FxInit_sE8appfPlot",0x343268,4),
        ("_ZZ7FxResetP8FxInit_sE10appfOpcode",0x343270,4),
    )
    for name,address,size in data_geometry:
        symbol=original.find_symbol(name)
        if symbol.value+0x342a38!=address or symbol.size!=size:
            raise SystemExit("original Super FX state/mode-cell geometry drift: "+name)
    for name,address,size in (("fx_apfFunctionTable",0x343b20,12),
                             ("fx_apfPlotTable",0x343b30,40),("fx_apfOpcodeTable",0x343b58,4096)):
        symbol=instructions.find_symbol(name)
        if symbol.value+0x343320!=address or symbol.size!=size:
            raise SystemExit("original Super FX dispatch-array geometry drift: "+name)
    frozen=json.loads((ROOT/"analysis/link_identity/historical_data.json").read_text())
    for name,address in (("GSU",0x342a38),("fx_ppfOpcodeTable",0x34323c),("fx_apfPlotTable",0x343b30)):
        if frozen["bindings"]["fxemu"][name]["address"]!=address:
            raise SystemExit("frozen Super FX provider-address drift: "+name)
    for name,path,bindings,section_plan in (
            ("fxemu.historical",original_path,{**LEGACY_PROVIDERS,"__gxx_personality_v0":0x1a9728},
             " .data 0x00342a38 : { *(.data*) }"),
            ("native_fxemu",obj,PROVIDERS,"")):
        script=BUILD/(name+".ld")
        script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in bindings.items())
            +"SECTIONS { .text 0x0012fe5c : { *(.text) }"+section_plan
            +" /DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
        run([CXX.with_name("ee-ld"),"-EL","-T",script,path,"-o",BUILD/(name+".elf")])
    reference,final=ELFFile(BUILD/"fxemu.historical.elf"),ELFFile(BUILD/"native_fxemu.elf")
    ref_text=next(s for s in reference.sections if s.name==".text")
    final_text=next(s for s in final.sections if s.name==".text")
    ref_bytes=reference.data[ref_text.offset:ref_text.offset+3456]
    final_bytes=final.data[final_text.offset:final_text.offset+3456]
    if (ref_text.address!=0x12fe5c or final_text.address!=0x12fe5c
        or ref_bytes!=final_bytes or digest(final_bytes)!=
            "bdef973079b4856aaccb57c80ed54aeed423f3966ae26884043ac6eba5a85df3"):
        raise SystemExit("complete 3456-byte linked historical Super FX module drift")
    data=next(s for s in reference.sections if s.name==".data")
    frozen_data=next(r for r in window if r["name"]=="fxemu_data")
    if (data.address!=0x342a38 or data.size!=2280 or digest(reference.data[data.offset:data.offset+2280])!=
            frozen_data["linked_sha256"] or frozen_data["linked_sha256"]!=
            "8d739827be081d1cf7155d825fbd808227a75560ac6ba6afef247902505b7470"):
        raise SystemExit("complete frozen linked Super FX state/mode-cell target digest drift")
    for spec in SPECS:
        name,size,address=spec["symbol"],int(spec["size"]),int(spec["address"],0)
        old,local,placed=original.find_symbol(spec["historical_symbol"]),isolated.find_symbol(name),final.find_symbol(name)
        if (old.size!=size or local.size!=size or placed.size!=size or placed.value!=address
            or digest(original.symbol_bytes(old,size))!=spec["historical_raw_sha256"]
            or digest(isolated.symbol_bytes(local,size))!=spec["isolated_raw_sha256"]
            or normalized(original,old)!=normalized(isolated,local)
            or digest(normalized(isolated,local))!=spec["normalized_sha256"]
            or len(isolated.relocation_ranges(local,size))!=int(spec["relocations"])
            or digest(final.symbol_bytes(placed,size))!=spec["linked_sha256"]):
            raise SystemExit("native Super FX function/provider/field-addend drift: "+name)
        print(f"native Super FX: MATCH {name} bytes={size}/{size} proof=linked-historical-reference")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=SPECS:raise SystemExit("native Super FX evidence ledger drift")
    print("native Super FX source proof: 21/21 functions; complete 3456-byte linked historical module; frozen 2280-byte linked state target")

if __name__=="__main__":
    main()
