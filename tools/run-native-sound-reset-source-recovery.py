#!/usr/bin/env python3
"""Prove 844 original native audio reset, echo and playback bytes.

The 616-byte public target span contains two functions: 436-byte ResetSound
and 180-byte SetPlaybackRate, including the target-proved float intermediate.
Echo enable matches all 228 public listing bytes. Every provider and constant
is linked at its actual address; existing behavioral soft-double providers
retain their prior claim level. No fresh private-image rerun is claimed.
"""
from __future__ import annotations
import csv
import hashlib
import json
import re
import struct
import subprocess
import sys
from pathlib import Path
from compare_elf_functions import ELFFile
ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build/matching/sound-native-source-recovery"
CXX=ROOT/"build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE=ROOT/"analysis/functions/native_sound_reset_exact_844.tsv"
FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections')
STATE_FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET')
PROVIDERS={'DAT_003454e0': 3429600,
 'DAT_0034db50': 3464016,
 'DAT_003ab718': 3847960,
 'DAT_003ab748': 3848008,
 'DAT_003c2e48': 3944008,
 'DAT_003e2e48': 4075080,
 'DAT_003f2e48': 4140616,
 'DAT_003f2e70': 4140656,
 'S9xSetEchoDelay': 1524268,
 'S9xSetSoundFrequency': 1525544,
 'g_APU_003453b8': 3429304,
 'memset': 1688476,
 'snes_dpdiv': 1718608,
 'snes_dpmul': 1717888,
 'snes_dptoul': 1719576,
 'snes_fptodp': 1717056,
 'snes_litodp': 1719088}
SPECS=[{'address': '0x00174120',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'historical_raw_sha256': 'adb5cfe3557d4c1e2fa3618ef6ac8d7c0e3011a39784acf9b020ec15570aa591',
  'historical_symbol': '_Z16S9xSetEchoEnableh',
  'isolated_raw_sha256': 'adb5cfe3557d4c1e2fa3618ef6ac8d7c0e3011a39784acf9b020ec15570aa591',
  'normalized_sha256': '04a0119be7a024c445a4e4469bb75397d97b7cbfd1b5b9c4c7a479a67f12aa1a',
  'proof_level': 'provider-linked-target',
  'relocations': '21',
  'size': '228',
  'source_file': 'src/snes9x/native_sound_reset.cpp',
  'symbol': 'S9xSetEchoEnable',
  'target_sha256': '8a3bab3c1b1aef32ca56adda53cfa3a0f606fed1524f03ee91a57ebcf52264bd'},
 {'address': '0x00177a84',
  'evidence': 'analysis/matching/hunt1041-v80-validated-quickwins-23.tsv',
  'historical_raw_sha256': '3f4441e576e978904a1533be1c7c255b0d77a0f211073a63fd602be0c6794b58',
  'historical_symbol': '_Z13S9xResetSoundh',
  'isolated_raw_sha256': '213157c917b9f698ea8b7997f924f0d59408248bfc54efd5b01bba35b4463c1c',
  'normalized_sha256': '01316db3b8b47de650b7e637d6e1496d497681b73a773e0f91cc6575c9a3f44a',
  'proof_level': 'provider-linked-target',
  'relocations': '17',
  'size': '436',
  'source_file': 'src/snes9x/native_sound_reset.cpp',
  'symbol': 'S9xResetSound',
  'target_sha256': '871fae91e0ddaaaf72712125e8c5d99f89a7791d9b4ed6735c08a79ffd098743'},
 {'address': '0x00177c38',
  'evidence': 'analysis/matching/hunt1041-v80-validated-quickwins-23.tsv',
  'historical_raw_sha256': 'c649dcb459625cb49be18b69535d74906b2f20f3aea5b8da64e84c8c98c097fd',
  'historical_symbol': '_Z18S9xSetPlaybackRatej',
  'isolated_raw_sha256': '3b9b66d8b8159e14952d21897c24dc067c15b88b1eb3417de6c2f444f056229b',
  'normalized_sha256': '3ddcaeea55b764afc2cc35db0bda5e87bb1fd294ee2ff5c3acbf680c5caad6c8',
  'proof_level': 'provider-linked-target',
  'relocations': '14',
  'size': '180',
  'source_file': 'src/snes9x/native_sound_reset.cpp',
  'symbol': 'S9xSetPlaybackRate',
  'target_sha256': '6f19f8a68a656da96f3d416adfac1871e0f1837e1fe8b0cedb85091a1010fb1e'}]
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
    source=(upstream/"SOUNDUX.CPP").read_text(encoding="latin1")
    marker="(1.0 / (double) so.playback_rate)"
    if source.count(marker)!=1:raise SystemExit("original playback-rate float-adaptation context drift")
    reference=BUILD/"SOUNDUX.playback-float.cpp"
    reference.write_text(source.replace(marker,"(1.0 / (double)(float) so.playback_rate)"),encoding="latin1")
    original_path=BUILD/"SOUNDUX.playback-float.o"
    run([CXX,*FLAGS[:-1],"-DZLIB",*includes,"-x","c++","-c",reference,"-o",original_path])
    obj=BUILD/"native_sound.o"
    run([CXX,*FLAGS,"-c",ROOT/"src/snes9x/native_sound_reset.cpp","-o",obj])
    original,isolated=ELFFile(original_path),ELFFile(obj)
    functions=[s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    ro=next(s for s in isolated.sections if s.name==".rodata")
    if (len(functions)!=3 or sum(s.size for s in functions)!=844
        or {s.name for s in functions}!={r["symbol"] for r in SPECS}
        or ro.size!=16 or isolated.data[ro.offset:ro.offset+16]!=bytes.fromhex("485786c47fb21040485786c47fb21040")
        or any(s.size and s.info >> 4 in (1,2) and s.section_index < len(isolated.sections)
               and isolated.sections[s.section_index].name in (".data",".bss")
               for s in isolated.symbols if s.section_index)):
        raise SystemExit("native sound function/constants/shared-storage inventory drift")
    state_path=BUILD/"GLOBALS.layout.o"
    run([CXX,*STATE_FLAGS,"-DZLIB",*includes,"-x","c++","-c",layout/"GLOBALS.CPP","-o",state_path])
    state=ELFFile(state_path);data=next(s for s in state.sections if s.name==".data")
    sha="8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    if data.size!=0xaf848 or digest(state.data[data.offset:data.offset+data.size])!=sha:
        raise SystemExit("frozen complete shared sound-state data drift")
    owners=json.loads((ROOT/"analysis/link_identity/historical_data.json").read_text())["owners"]
    owner=[r for r in owners if r["unit"]=="globals" and r["symbol"]=="@section:.data"]
    if len(owner)!=1 or any(owner[0][k]!=v for k,v in
            {"address":0x345060,"size":0xaf848,"source_offset":0,"sha256":sha}.items()):
        raise SystemExit("frozen shared sound source-owner interval drift")
    for name,address,size in (("SoundData",0x34db50,1864),("so",0x3ab718,48),
            ("Echo",0x3ab748,96000),("DummyEchoBuffer",0x3c2e48,65536),
            ("EchoBuffer",0x3e2e48,65536),("FilterTaps",0x3f2e48,32),
            ("Loop",0x3f2e70,64),("APU",0x3453b8,224),("Settings",0x3454e0,328)):
        symbol=state.find_symbol(name)
        if symbol.size!=size or symbol.value+0x345060!=address:
            raise SystemExit("original sound object/channel geometry drift: "+name)
    assembly=(ROOT/"matching/candidates/hunt1041_v80_quickwins_exact.S").read_text()
    assembly=assembly[assembly.index("v80_00177a84:"):assembly.index(".size v80_00177a84")]
    target=b"".join(struct.pack("<I",int(word,16))
                    for word in re.findall(r"^\s*\.word (0x[0-9a-f]+)",assembly,re.M))
    with (ROOT/"analysis/matching/hunt1041-v80-validated-quickwins-23.tsv").open(newline="") as stream:
        witness=[r for r in csv.DictReader(stream,delimiter="\t") if r["address"]=="0x00177a84"]
    if (len(target)!=616 or digest(target)!="49c5bb585ba22d0ff11502960680ab301902d8df4315aa7ba86f4095b1134542"
        or len(witness)!=1 or witness[0]["result"]!="MATCH" or witness[0]["raw_equal"]!="True"
        or witness[0]["differing_bytes"]!="0" or witness[0]["object_size"]!="616"
        or witness[0]["target_span_sha256"]!=digest(target)):
        raise SystemExit("frozen complete sound-reset/playback composite target drift")
    targets={"S9xResetSound":target[:436],"S9xSetPlaybackRate":target[436:]}
    line_pattern=re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} ){3}[0-9a-fA-F]{2})\s")
    code={}
    for line in (ROOT/"analysis/functions/progress13_targets.asm").read_text().splitlines():
        match=line_pattern.match(line)
        if match and 0x174120<=int(match[1],16)<0x174204:
            code[int(match[1],16)]=bytes.fromhex(match[2])
    if len(code)!=57:raise SystemExit("complete echo-enable listing drift")
    targets["S9xSetEchoEnable"]=b"".join(code[a] for a in range(0x174120,0x174204,4))
    script=BUILD/"native_sound.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in PROVIDERS.items())
        +"SECTIONS {\n"+"".join(f" .text.{i} {int(r['address'],0):#x} : {{ *(.text.{r['symbol']}) }}\n" for i,r in enumerate(SPECS))
        +" .rodata 0x001b8440 : { *(.rodata*) } /DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked=BUILD/"native_sound.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",script,obj,"-o",linked]);final=ELFFile(linked)
    for spec in SPECS:
        name,size,address=spec["symbol"],int(spec["size"]),int(spec["address"],0)
        old,local,placed=original.find_symbol(spec["historical_symbol"]),isolated.find_symbol(name),final.find_symbol(name)
        if (any(s.size!=size for s in (old,local,placed)) or placed.value!=address
            or digest(original.symbol_bytes(old,size))!=spec["historical_raw_sha256"]
            or digest(isolated.symbol_bytes(local,size))!=spec["isolated_raw_sha256"]
            or normalized(original,old)!=normalized(isolated,local)
            or digest(normalized(isolated,local))!=spec["normalized_sha256"]
            or len(isolated.relocation_ranges(local,size))!=int(spec["relocations"])
            or final.symbol_bytes(placed,size)!=targets[name]
            or digest(targets[name])!=spec["target_sha256"]):
            raise SystemExit("native sound original instructions/complete target drift: "+name)
        print(f"native sound: MATCH {name} bytes={size}/{size} proof=provider-linked-target")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=SPECS:raise SystemExit("native sound evidence ledger drift")
    print("native sound source proof: 3/3 functions; 844/844 complete provider-linked target bytes")

if __name__=="__main__":
    main()
