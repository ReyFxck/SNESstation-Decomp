#!/usr/bin/env python3
"""Prove the complete 956-byte historical CPU reset module.

All four functions retain complete raw object bytes, including state-field
addends. All routines match a fully linked historical reference. The hard reset also
retains its frozen normalized-instruction MATCH witness. This does not claim
newly captured private-target bytes for any reset routine.
"""
from __future__ import annotations
import csv
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from compare_elf_functions import ELFFile
ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/cpu-reset-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/functions/cpu_reset_exact_956.tsv"
FLAGS = ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET')
PROVIDERS = {'DAT_003453a8': 3429288,
 'DAT_003454e0': 3429600,
 'DAT_0034e2b0': 3465904,
 'DAT_0035b770': 3520368,
 'PTR_FUN_003367f8': 3368952,
 'ResetOBC1': 1413104,
 'S9xInitC4': 1098496,
 'S9xResetAPU': 1091892,
 'S9xResetDMA': 1227172,
 'S9xResetDSP1': 1238724,
 'S9xResetPPU': 1425700,
 'S9xSA1Init': 1431788,
 'S9xSoftResetPPU': 1426980,
 '_Z10S9xGetWordj': 1752104,
 '_Z12S9xResetSDD1v': 1505816,
 '_Z12S9xResetSRTCv': 1586784,
 '_Z12S9xSetPCBasej': 1753124,
 '_Z15S9xSpc7110Resetv': 1585828,
 '_Z16S9xInitCheatDatav': 1130720,
 'S9xFxReset': 1246968,
 'g_CPU_blob': 3429184,
 'g_ICPU_00345318': 3429144,
 'memset': 1688476}
LEGACY_NAMES = {'S9xFxReset': '_Z7FxResetP8FxInit_s', 'DAT_003453a8': 'Registers',
 'DAT_003454e0': 'Settings',
 'DAT_0034e2b0': 'Memory',
 'DAT_0035b770': 'SuperFX',
 'PTR_FUN_003367f8': 'S9xOpcodesM1X1',
 'g_CPU_blob': 'CPU',
 'g_ICPU_00345318': 'ICPU'}
SPECS = [{'address': '0x001159f4',
  'evidence': 'analysis/link_identity/code_windows.json',
  'linked_sha256': '3177834610386bf48e963b6e614ee6bce8b78a684e6376e2684b9de72929cb23',
  'normalized_sha256': '15c07040aeb93aa3f74d12d27dbd3c19e0eae6448d40f2c17e30448ded17e5ed',
  'proof_level': 'linked-historical-reference',
  'raw_sha256': '15c07040aeb93aa3f74d12d27dbd3c19e0eae6448d40f2c17e30448ded17e5ed',
  'relocations': '4',
  'size': '36',
  'source_file': 'src/snes9x/cpu_reset.cpp',
  'symbol': '_Z15S9xResetSuperFXv'},
 {'address': '0x00115a18',
  'evidence': 'analysis/link_identity/code_windows.json',
  'linked_sha256': '987cd262d474b0e14be976525c53cc5b75a1a6711c76e3a4abc04e58622b863a',
  'normalized_sha256': '8b8c68ddb3f83c1521295c248f8e6bfa5e47a90abd8d98cdf3f65d43d25dbfd8',
  'proof_level': 'linked-historical-reference',
  'raw_sha256': 'b72cd377e6adb6adaf192fe2451359dd599d527e9f023211f3515820eabc9375',
  'relocations': '15',
  'size': '320',
  'source_file': 'src/snes9x/cpu_reset.cpp',
  'symbol': '_Z11S9xResetCPUv'},
 {'address': '0x00115b58',
  'evidence': 'analysis/matching/hunt1000plus-v46-validated-42.tsv',
  'linked_sha256': 'ebcdfd330c98ca255137c7229c5c4030345d6323e15330f0733c9a21a2bbede4',
  'normalized_sha256': '8d3b3d0e44bbf607ff0f42fc55273f3522418a4dba422f3c52d1c0fd103c4861',
  'proof_level': 'linked-historical-reference',
  'raw_sha256': '8d3b3d0e44bbf607ff0f42fc55273f3522418a4dba422f3c52d1c0fd103c4861',
  'relocations': '22',
  'size': '312',
  'source_file': 'src/snes9x/cpu_reset.cpp',
  'symbol': 'S9xReset'},
 {'address': '0x00115c90',
  'evidence': 'analysis/link_identity/code_windows.json',
  'linked_sha256': '21d6c8defce0faccb2633846a0923e70f2e4a852db3b78267dc1666a4aa4b278',
  'normalized_sha256': '0cbdebd18966dadfcaad2ad46509f34762911a53d587e9d3549a51f1bca882de',
  'proof_level': 'linked-historical-reference',
  'raw_sha256': '0cbdebd18966dadfcaad2ad46509f34762911a53d587e9d3549a51f1bca882de',
  'relocations': '20',
  'size': '288',
  'source_file': 'src/snes9x/cpu_reset.cpp',
  'symbol': 'S9xSoftReset'}]
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
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1000plus_v47_closure as historical
    import hunt1041_v52_closure as layout_recipe
    archive = historical.download_archive(historical.SNES_141_1_ARCHIVE, historical.SNES_CACHE)
    source_root = historical.safe_extract_archive(archive, historical.SNES_CACHE / "source-1.41-1",
                                                 historical.SNES_141_1_ARCHIVE.source_directory)
    historical.ensure_git_commit(historical.PS2DEV, historical.PS2DEV_REPO, historical.PS2DEV_COMMIT)
    upstream = source_root / "snes9x"
    newlib = historical.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    BUILD.mkdir(parents=True, exist_ok=True)
    compat = BUILD / "compat"
    compat.mkdir(exist_ok=True)
    (compat / "memory.h").write_text("#include <string.h>\n")
    includes = ["-I"+str(p) for p in (compat, newlib, upstream, upstream / "unzip", source_root / "zlib")]
    original_path = BUILD / "CPU.historical.o"
    run([CXX, *FLAGS, *includes, "-x", "c++", "-c", upstream / "CPU.CPP", "-o", original_path])
    isolated_path = BUILD / "cpu_reset.o"
    run([CXX, *FLAGS, "-c", ROOT / "src/snes9x/cpu_reset.cpp", "-o", isolated_path])
    original, isolated = ELFFile(original_path), ELFFile(isolated_path)
    selected = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    captures = [s for s in selected if s.get("name") == "cpu"]
    expected = {"address": 0x1159f4, "size": 956, "new_bytes": 956, "relocations": 61,
                "raw_sha256": "416f88c7e71cdab3e7060761dcbf0984fc346ff9e1d24ecb3cc5327672f26938"}
    if len(captures) != 1 or any(captures[0].get(k) != v for k,v in expected.items()):
        raise SystemExit("frozen complete CPU instruction window drift")
    for elf in (original, isolated):
        text = [s for s in elf.sections if s.name == ".text" or s.name.startswith(".text.")]
        functions = [s for s in elf.symbols if s.info & 15 == 2 and s.size]
        if (len(text) != 1 or text[0].size != 956 or len(functions) != 4
                or digest(elf.data[text[0].offset:text[0].offset+956]) != expected["raw_sha256"]):
            raise SystemExit("complete CPU function/field-addend/raw-byte drift")

    # The frozen full GLOBALS data interval independently establishes shared
    # CPU/register/settings storage and the otherwise unlisted SuperFX block.
    old_build = layout_recipe.BUILD
    try:
        layout_recipe.BUILD = BUILD / "state-layout"
        _source, _original, layout = layout_recipe.prepare_snes_layout()
        layout_recipe.patch_sources(layout)
        state_path = BUILD / "GLOBALS.layout.o"
        state_includes = ["-I"+str(p) for p in (compat,newlib,layout,layout/"unzip",source_root/"zlib")]
        run([CXX,*FLAGS,"-DZLIB",*state_includes,"-x","c++","-c",layout/"GLOBALS.CPP","-o",state_path])
    finally:
        layout_recipe.BUILD = old_build
    state = ELFFile(state_path)
    data = next(s for s in state.sections if s.name == ".data")
    state_sha = "8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    if data.size != 0xaf848 or digest(state.data[data.offset:data.offset+data.size]) != state_sha:
        raise SystemExit("frozen historical shared-state data interval drift")
    owners = json.loads((ROOT / "analysis/link_identity/historical_data.json").read_text())["owners"]
    owner = [r for r in owners if r["unit"] == "globals" and r["symbol"] == "@section:.data"]
    if len(owner) != 1 or any(owner[0][k] != v for k,v in
            {"address":0x345060,"size":0xaf848,"source_offset":0,"sha256":state_sha}.items()):
        raise SystemExit("frozen historical shared-state ownership drift")
    for name,address in (("SuperFX",0x35b770),("CPU",0x345340),("ICPU",0x345318),
                         ("Registers",0x3453a8),("Settings",0x3454e0),("Memory",0x34e2b0)):
        if state.find_symbol(name).value + 0x345060 != address:
            raise SystemExit("CPU shared-state address drift: "+name)

    finals = []
    for name,path,bindings in (("CPU.historical",original_path,{LEGACY_NAMES.get(k,k):v for k,v in PROVIDERS.items()}),
                               ("cpu_reset",isolated_path,PROVIDERS)):
        script = BUILD / (name+".ld")
        script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in bindings.items())
            + "SECTIONS { .text 0x001159f4 : { *(.text) } /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
        target = BUILD / (name+".elf")
        run([CXX.with_name("ee-ld"),"-EL","-T",script,path,"-o",target])
        finals.append(ELFFile(target))
    for spec in SPECS:
        name,size,address = spec["symbol"],int(spec["size"]),int(spec["address"],0)
        old,local = original.find_symbol(name),isolated.find_symbol(name)
        ref,placed = (e.find_symbol(name) for e in finals)
        if (any(s.size != size for s in (old,local,ref,placed))
            or any(e.symbol_bytes(s,size) != original.symbol_bytes(old,size) for e,s in ((isolated,local),))
            or digest(original.symbol_bytes(old,size)) != spec["raw_sha256"]
            or digest(normalized(isolated,local)) != spec["normalized_sha256"]
            or len(isolated.relocation_ranges(local,size)) != int(spec["relocations"])
            or ref.value != address or placed.value != address
            or finals[0].symbol_bytes(ref,size) != finals[1].symbol_bytes(placed,size)
            or digest(finals[1].symbol_bytes(placed,size)) != spec["linked_sha256"]):
            raise SystemExit("CPU reset function instructions/provider drift: "+name)
        if name == "S9xReset":
            with (ROOT / spec["evidence"]).open(newline="") as stream:
                witness = [r for r in csv.DictReader(stream,delimiter="\t")
                           if r["address"]==spec["address"] and r["object_symbol"]==name]
            if (len(witness)!=1 or witness[0]["result"]!="MATCH" or witness[0]["differing_bytes"]!="0"
                or witness[0]["unknown_relocations"] or witness[0]["normalized_equal"]!="True"
                or witness[0]["object_size"]!=spec["size"]
                or "relocations=22" not in witness[0]["detail"]):
                raise SystemExit("frozen CPU hard-reset instruction witness drift")
        print(f"CPU reset: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t")) != SPECS:
            raise SystemExit("CPU reset evidence ledger drift")
    print("CPU reset source proof: 4/4 functions; 956/956 complete raw historical bytes; all shared-state addends retained")

if __name__ == "__main__":
    main()
