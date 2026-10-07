#!/usr/bin/env python3
"""Prove the complete 2448-byte historical CPU execution module.

All four functions retain complete raw object bytes, including state-field
addends. All routines match a fully linked historical reference. Each routine retains its frozen normalized-instruction MATCH witness. This
does not claim newly captured private-target bytes for the execution module.
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
BUILD = ROOT / "build/matching/cpu-execution-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/functions/cpu_execution_exact_2448.tsv"
FLAGS = ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET')
PROVIDERS = {'DAT_00345268': 3428968,
 'DAT_003453a8': 3429288,
 'DAT_00345498': 3429528,
 'DAT_003454d8': 3429592,
 'DAT_003454e0': 3429600,
 'DAT_0034e2b0': 3465904,
 'DAT_0035b788': 3520392,
 'DAT_0035c268': 3523176,
 'RenderLine': 1323920,
 'S9xDeinterleaveMode2': 1384632,
 'S9xDoHDMA': 1225880,
 'S9xEndScreenRefresh': 1324204,
 'S9xGenerateSound': 1054980,
 'S9xOpcode_IRQ': 1211256,
 'S9xOpcode_NMI': 1211904,
 'S9xSA1MainLoop': 1503136,
 'S9xStartHDMA': 1225704,
 'S9xStartScreenRefresh': 1323292,
 'S9xSuperFXExec': 1430324,
 'S9xSyncSpeed': 1071256,
 'S9xUpdateJoypads': 1429692,
 'g_APU_003453b8': 3429304,
 'g_CPU_blob': 3429184,
 'g_ICPU_00345318': 3429144,
 'g_S9xAPUCycles_003f44a8': 4146344,
 'g_S9xApuOpcodes_00411010': 4263952,
 'g_SA1_blob': 3431160}
LEGACY_NAMES = {'DAT_00345268': 'missing',
 'DAT_003453a8': 'Registers',
 'DAT_00345498': 'IAPU',
 'DAT_003454d8': 'APURegisters',
 'DAT_003454e0': 'Settings',
 'DAT_0034e2b0': 'Memory',
 'DAT_0035b788': 'PPU',
 'DAT_0035c268': 'IPPU',
 'S9xOpcode_IRQ': '_Z13S9xOpcode_IRQv',
 'S9xOpcode_NMI': '_Z13S9xOpcode_NMIv',
 'g_APU_003453b8': 'APU',
 'g_CPU_blob': 'CPU',
 'g_ICPU_00345318': 'ICPU',
 'g_S9xAPUCycles_003f44a8': 'S9xAPUCycles',
 'g_S9xApuOpcodes_00411010': 'S9xApuOpcodes',
 'g_SA1_blob': 'SA1'}
SPECS = [{'address': '0x00115db0',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'linked_sha256': 'a61d5b00a4933937d6bafda4bd925035b2ba7c21dc78f04222eaa4cd3e933633',
  'normalized_sha256': 'd304c0901b3239c1cd00418602d11aa13b92ee999296d1c097bf3e26200845b6',
  'proof_level': 'linked-historical-reference',
  'raw_sha256': '7587aedfd4ff28c9a0e6ef1c261eb513899b286c493d345682e61aed26ce7f90',
  'relocations': '73',
  'size': '888',
  'source_file': 'src/snes9x/cpu_execution.cpp',
  'symbol': 'S9xMainLoop'},
 {'address': '0x00116128',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'linked_sha256': 'ffac9a4f07a6419ac50b89fbc5eeeb2d2497587a6846a5eb512a16e637d33b95',
  'normalized_sha256': '945a97f18706649f757bb6e89d0a63b39fd22291660a617dbb78609a8246c9ca',
  'proof_level': 'linked-historical-reference',
  'raw_sha256': '945a97f18706649f757bb6e89d0a63b39fd22291660a617dbb78609a8246c9ca',
  'relocations': '4',
  'size': '76',
  'source_file': 'src/snes9x/cpu_execution.cpp',
  'symbol': 'S9xSetIRQ'},
 {'address': '0x00116174',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'linked_sha256': '7c6ee1be2a101705d21cf1b320099a6ae80334cee534753911dba62d8ba0b13e',
  'normalized_sha256': 'a99e9b8e03e350b98aa265a418a29f1da646a8d461394318d58faf73eafc82c4',
  'proof_level': 'linked-historical-reference',
  'raw_sha256': 'a99e9b8e03e350b98aa265a418a29f1da646a8d461394318d58faf73eafc82c4',
  'relocations': '4',
  'size': '52',
  'source_file': 'src/snes9x/cpu_execution.cpp',
  'symbol': 'S9xClearIRQ'},
 {'address': '0x001161a8',
  'evidence': 'analysis/matching/hunt1000plus-v46-validated-42.tsv',
  'linked_sha256': '2ec6ebb4a8a8934817467f937fb7c8ac4215349a0e006c0ecb949c52c3bd90f7',
  'normalized_sha256': 'a9b6c7ba567a6fd05d459071463891843a05e484446bd6742db721896df984b1',
  'proof_level': 'linked-historical-reference',
  'raw_sha256': '5dbb38438f4ca4ab4744756f0d1a3d3d7a0ace7312a0532efa58d7ed735416c0',
  'relocations': '120',
  'size': '1432',
  'source_file': 'src/snes9x/cpu_execution.cpp',
  'symbol': 'S9xDoHBlankProcessing'}]

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
    original_path = BUILD / "CPUEXEC.historical.o"
    run([CXX, *FLAGS, *includes, "-x", "c++", "-c", upstream / "CPUEXEC.CPP", "-o", original_path])
    isolated_path = BUILD / "cpu_execution.o"
    run([CXX, *FLAGS, "-c", ROOT / "src/snes9x/cpu_execution.cpp", "-o", isolated_path])
    original, isolated = ELFFile(original_path), ELFFile(isolated_path)
    selected = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    captures = [s for s in selected if s.get("name") == "cpuexec"]
    expected = {"address": 0x115db0, "size": 2448, "new_bytes": 2448, "relocations": 201,
                "raw_sha256": "e1b08ace7f8650e29c197b49ecc25808927d2f4c767ed08f46919d15e9b578c7"}
    if len(captures) != 1 or any(captures[0].get(k) != v for k,v in expected.items()):
        raise SystemExit("frozen complete CPU execution instruction window drift")
    for elf in (original, isolated):
        text = [s for s in elf.sections if s.name == ".text" or s.name.startswith(".text.")]
        functions = [s for s in elf.symbols if s.info & 15 == 2 and s.size]
        if (len(text) != 1 or text[0].size != 2448 or len(functions) != 4
                or digest(elf.data[text[0].offset:text[0].offset+2448]) != expected["raw_sha256"]):
            raise SystemExit("complete CPU function/field-addend/raw-byte drift")

    # The frozen full GLOBALS data interval independently establishes shared
    # CPU/APU/PPU/register/settings storage and the full debug state block.
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
    for name,address in (("CPU",0x345340),("ICPU",0x345318),("Registers",0x3453a8),
                         ("Settings",0x3454e0),("Memory",0x34e2b0),("APU",0x3453b8),
                         ("IAPU",0x345498),("APURegisters",0x3454d8),("SA1",0x345af8),
                         ("PPU",0x35b788),("IPPU",0x35c268),("missing",0x345268)):
        if state.find_symbol(name).value + 0x345060 != address:
            raise SystemExit("CPU shared-state address drift: "+name)

    finals = []
    for name,path,bindings in (("CPUEXEC.historical",original_path,{LEGACY_NAMES.get(k,k):v for k,v in PROVIDERS.items()}),
                               ("cpu_execution",isolated_path,PROVIDERS)):
        script = BUILD / (name+".ld")
        script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in bindings.items())
            + "SECTIONS { .text 0x00115db0 : { *(.text) } /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
        target = BUILD / (name+".elf")
        run([CXX.with_name("ee-ld"),"-EL","-T",script,path,"-o",target])
        finals.append(ELFFile(target))
    for spec in SPECS:
        name,size,address = spec["symbol"],int(spec["size"]),int(spec["address"],0)
        old,local = original.find_symbol(name),isolated.find_symbol(name)
        ref,placed = (e.find_symbol(name) for e in finals)
        if (any(s.size != size for s in (old,local,ref,placed))
            or isolated.symbol_bytes(local,size) != original.symbol_bytes(old,size)
            or digest(original.symbol_bytes(old,size)) != spec["raw_sha256"]
            or digest(normalized(isolated,local)) != spec["normalized_sha256"]
            or len(isolated.relocation_ranges(local,size)) != int(spec["relocations"])
            or ref.value != address or placed.value != address
            or finals[0].symbol_bytes(ref,size) != finals[1].symbol_bytes(placed,size)
            or digest(finals[1].symbol_bytes(placed,size)) != spec["linked_sha256"]):
            raise SystemExit("CPU execution function instructions/provider drift: "+name)
        with (ROOT / spec["evidence"]).open(newline="") as stream:
            witness = [r for r in csv.DictReader(stream,delimiter="\t")
                       if r["address"]==spec["address"] and r["object_symbol"]==name]
        if (len(witness)!=1 or witness[0]["result"]!="MATCH" or witness[0]["differing_bytes"]!="0"
            or witness[0]["unknown_relocations"] or witness[0]["normalized_equal"]!="True"
            or witness[0]["object_size"]!=spec["size"]
            ):
            raise SystemExit("frozen CPU execution instruction witness drift")
        print(f"CPU execution: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t")) != SPECS:
            raise SystemExit("CPU execution evidence ledger drift")
    print("CPU execution source proof: 4/4 functions; 2448/2448 complete raw historical bytes; all shared-state addends retained")

if __name__ == "__main__":
    main()
