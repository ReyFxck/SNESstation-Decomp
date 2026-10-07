#!/usr/bin/env python3
"""Prove 452 complete native echo-delay/write and sound-frequency bytes.

Rebuild the public original source with the frozen V52 float-to-double
frequency adaptation. All echo instructions match public listings, and all
264 frequency bytes match the frozen target digest. Native sound/channel
storage, noise table and soft-double providers retain original geometry and
existing claim levels. No fresh private-image rerun is claimed.
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
ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build/matching/native-sound-controls-source-recovery"
CXX=ROOT/"build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE=ROOT/"analysis/functions/native_sound_controls_exact_452.tsv"
FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections')
STATE_FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET')
PROVIDERS={'DAT_003454e0': 3429600,
 'DAT_0034db50': 3464016,
 'DAT_003ab718': 3847960,
 'DAT_003f3fc0': 4145088,
 'S9xSetEchoEnable': 1524000,
 'g_APU_003453b8': 3429304,
 'snes___fixunsdfdi': 1711256,
 'snes___udivdi3': 1713584,
 'snes_dpmul': 1717888,
 'snes_fptodp': 1717056}
SPECS=[{'address': '0x0017422c',
  'evidence': 'analysis/functions/progress12_targets.asm',
  'historical_raw_sha256': 'cee1f12629b37cb9c9fa44167e8b36a1c444301a915828e13593edbdf8e5c6db',
  'historical_symbol': '_Z15S9xSetEchoDelayi',
  'isolated_raw_sha256': 'cee1f12629b37cb9c9fa44167e8b36a1c444301a915828e13593edbdf8e5c6db',
  'normalized_sha256': '66904b1543a0cc803a445a166bd5b561c57e90589e6254c925999d4599eec6ab',
  'proof_level': 'provider-linked-target',
  'relocations': '7',
  'size': '136',
  'source_file': 'src/snes9x/native_sound_controls.cpp',
  'symbol': 'S9xSetEchoDelay',
  'target_sha256': 'a6d88d5d2024058080897bc4578286045c895df51a83dc50ed2bbe3b965c9a97'},
 {'address': '0x001742b4',
  'evidence': 'analysis/functions/progress11_short_targets.asm',
  'historical_raw_sha256': 'ff979bb271e0bab64bf4d07ebbde23ccd13d03dfdbbb9c9cf10d7b2be728bbb2',
  'historical_symbol': '_Z21S9xSetEchoWriteEnableh',
  'isolated_raw_sha256': 'ff979bb271e0bab64bf4d07ebbde23ccd13d03dfdbbb9c9cf10d7b2be728bbb2',
  'normalized_sha256': '5b8dae23af6654d54ec06b8c563e79f25380eb53079db88d74cd72cfce16e783',
  'proof_level': 'provider-linked-target',
  'relocations': '5',
  'size': '52',
  'source_file': 'src/snes9x/native_sound_controls.cpp',
  'symbol': 'S9xSetEchoWriteEnable',
  'target_sha256': 'aa5de3e0a51941f8a0494c3a6eed85ea04bc1b168573135c97f41893074181eb'},
 {'address': '0x00174728',
  'evidence': 'analysis/matching/hunt1041-v52-validated-17.tsv',
  'historical_raw_sha256': '8acb07e34e3a35034de2097238adb06b79c0d60ab0c94f755e48858b67847500',
  'historical_symbol': '_Z20S9xSetSoundFrequencyii',
  'isolated_raw_sha256': '8acb07e34e3a35034de2097238adb06b79c0d60ab0c94f755e48858b67847500',
  'normalized_sha256': 'd4e4f9bb3c01c78a5cc33ce2d1be7b0698b3d084907fe3794e4df39fef33ae7a',
  'proof_level': 'provider-linked-target',
  'relocations': '16',
  'size': '264',
  'source_file': 'src/snes9x/native_sound_controls.cpp',
  'symbol': 'S9xSetSoundFrequency',
  'target_sha256': '4be60f798dca820fcd3c50e07ff262c64055ea6199c4b9fe471f21dfa8368e78'}]
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
    original_path=BUILD/"SOUNDUX.historical.o"
    run([CXX,*FLAGS[:-1],"-DZLIB",*includes,"-x","c++","-c",layout/"SOUNDUX.CPP","-o",original_path])
    obj=BUILD/"native_sound_controls.o"
    run([CXX,*FLAGS,"-c",ROOT/"src/snes9x/native_sound_controls.cpp","-o",obj])
    original,isolated=ELFFile(original_path),ELFFile(obj)
    functions=[s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    ro=next(s for s in isolated.sections if s.name==".rodata")
    if (len(functions)!=3 or sum(s.size for s in functions)!=452
        or {s.name for s in functions}!={r["symbol"] for r in SPECS}
        or ro.size!=8 or isolated.data[ro.offset:ro.offset+8]!=bytes.fromhex("5c8fc2f5285cef3f")
        or any(s.size and s.info >> 4 in (1,2) and s.section_index < len(isolated.sections)
               and isolated.sections[s.section_index].name in (".data",".bss")
               for s in isolated.symbols if s.section_index)):
        raise SystemExit("native sound controls/constants/shared-storage inventory drift")
    state_path=BUILD/"GLOBALS.layout.o"
    run([CXX,*STATE_FLAGS,"-DZLIB",*includes,"-x","c++","-c",layout/"GLOBALS.CPP","-o",state_path])
    state=ELFFile(state_path);data=next(s for s in state.sections if s.name==".data")
    sha="8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    if data.size!=0xaf848 or digest(state.data[data.offset:data.offset+data.size])!=sha:
        raise SystemExit("frozen complete shared sound/noise data drift")
    owners=json.loads((ROOT/"analysis/link_identity/historical_data.json").read_text())["owners"]
    owner=[r for r in owners if r["unit"]=="globals" and r["symbol"]=="@section:.data"]
    if len(owner)!=1 or any(owner[0][k]!=v for k,v in
            {"address":0x345060,"size":0xaf848,"source_offset":0,"sha256":sha}.items()):
        raise SystemExit("frozen shared sound source-owner interval drift")
    for name,address,size in (("SoundData",0x34db50,1864),("so",0x3ab718,48),
                             ("APU",0x3453b8,224),("Settings",0x3454e0,328),("NoiseFreq",0x3f3fc0,128)):
        symbol=state.find_symbol(name)
        if symbol.size!=size or symbol.value+0x345060!=address:
            raise SystemExit("original sound/noise provider geometry drift: "+name)
    script=BUILD/"native_sound_controls.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in PROVIDERS.items())
        +"SECTIONS {\n"+"".join(f" .text.{i} {int(r['address'],0):#x} : {{ *(.text.{r['symbol']}) }}\n" for i,r in enumerate(SPECS))
        +" .rodata 0x001b83e8 : { *(.rodata*) } /DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked=BUILD/"native_sound_controls.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",script,obj,"-o",linked]);final=ELFFile(linked)
    line_pattern=re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} ){3}[0-9a-fA-F]{2})\s")
    for spec in SPECS:
        name,size,address=spec["symbol"],int(spec["size"]),int(spec["address"],0)
        old,local,placed=original.find_symbol(spec["historical_symbol"]),isolated.find_symbol(name),final.find_symbol(name)
        target=final.symbol_bytes(placed,size)
        if (any(s.size!=size for s in (old,local,placed)) or placed.value!=address
            or digest(original.symbol_bytes(old,size))!=spec["historical_raw_sha256"]
            or digest(isolated.symbol_bytes(local,size))!=spec["isolated_raw_sha256"]
            or normalized(original,old)!=normalized(isolated,local)
            or digest(normalized(isolated,local))!=spec["normalized_sha256"]
            or len(isolated.relocation_ranges(local,size))!=int(spec["relocations"])
            or digest(target)!=spec["target_sha256"]):
            raise SystemExit("native sound controls original instructions/complete target drift: "+name)
        if spec["evidence"].endswith(".asm"):
            code={}
            for line in (ROOT/spec["evidence"]).read_text().splitlines():
                match=line_pattern.match(line)
                if match and address<=int(match[1],16)<address+size:
                    code[int(match[1],16)]=bytes.fromhex(match[2])
            if len(code)*4!=size or b"".join(code[a] for a in range(address,address+size,4))!=target:
                raise SystemExit("complete native echo public listing drift: "+name)
        else:
            with (ROOT/spec["evidence"]).open(newline="") as stream:
                witness=[r for r in csv.DictReader(stream,delimiter="\t") if r["address"]==spec["address"]]
            if (len(witness)!=1 or witness[0]["result"]!="MATCH" or witness[0]["differing_bytes"]!="0"
                or witness[0]["unknown_relocations"] or witness[0]["object_size"]!="264"
                or witness[0]["object_symbol"]!=spec["historical_symbol"]
                or witness[0]["target_span_sha256"]!=spec["target_sha256"]):
                raise SystemExit("frozen complete native frequency target drift")
        print(f"native sound controls: MATCH {name} bytes={size}/{size} proof=provider-linked-target")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=SPECS:raise SystemExit("native sound controls evidence ledger drift")
    print("native sound controls source proof: 3/3 functions; 452/452 complete provider-linked target bytes")

if __name__=="__main__":
    main()
