#!/usr/bin/env python3
"""Prove all 1044 historical APU reset bytes against the frozen target digest.

Preserves original state-field addends, 32-bit memory byte counts, native
sound-call ABI, boot ROM, DSP dump and cycle tables. Rebuilds public sources;
no new private-image execution or whole-image identity is claimed.
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
BUILD=ROOT/"build/matching/apu-reset-source-recovery"
CXX=ROOT/"build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE=ROOT/"analysis/functions/apu_reset_exact_1044.tsv"
FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET')
PROVIDERS={'DAT_00335378': 3363704,
 'DAT_00345498': 3429528,
 'DAT_003454d8': 3429592,
 'DAT_003454e0': 3429600,
 'DAT_003f4068': 4145256,
 'DAT_003f40a8': 4145320,
 'S9xResetSound': 1538692,
 'S9xSetEchoEnable': 1524000,
 'g_APU_003453b8': 3429304,
 'g_S9xAPUCycles_003f44a8': 4146344,
 'memmove': 1688736,
 'memset': 1688476}
LEGACY_NAMES={'DAT_00335378': 'spc_dump_dsp',
 'DAT_00345498': 'IAPU',
 'DAT_003454d8': 'APURegisters',
 'DAT_003454e0': 'Settings',
 'DAT_003f4068': 'APUROM',
 'DAT_003f40a8': 'S9xAPUCycleLengths',
 'S9xResetSound': '_Z13S9xResetSoundh',
 'S9xSetEchoEnable': '_Z16S9xSetEchoEnableh',
 'g_APU_003453b8': 'APU',
 'g_S9xAPUCycles_003f44a8': 'S9xAPUCycles',
 'memmove': 'memmove',
 'memset': 'memset'}
SPEC={'address': '0x0010a934',
 'evidence': 'analysis/matching/hunt1041-v52-validated-17.tsv',
 'normalized_sha256': 'bf0c125aa5fc5488289f958fea6d43b788f5017fcfefb7c4eef3a4c52d7d426a',
 'proof_level': 'provider-linked-target',
 'raw_sha256': '74aca7e57d128316da6d24a4dbf0f7384c751383521033fd2d414ca16ff03dce',
 'relocations': '51',
 'size': '1044',
 'source_file': 'src/snes9x/apu_reset.cpp',
 'symbol': 'S9xResetAPU',
 'target_sha256': '7b9b22e9e4219577dc0519b524ba93e24e2829c425bdbcfe6b7213c47f245ac0'}
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
    original_path=BUILD/"APU.historical.o"
    run([CXX,*FLAGS,"-DZLIB",*includes,"-x","c++","-c",layout/"APU.CPP","-o",original_path])
    obj=BUILD/"apu_reset.o"
    run([CXX,*FLAGS,"-c",ROOT/SPEC["source_file"],"-o",obj])
    original,isolated=ELFFile(original_path),ELFFile(obj)
    old,local=original.find_symbol("S9xResetAPU"),isolated.find_symbol("S9xResetAPU")
    functions=[s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    if (len(functions)!=1 or old.size!=1044 or local.size!=1044
        or original.symbol_bytes(old,1044)!=isolated.symbol_bytes(local,1044)
        or digest(isolated.symbol_bytes(local,1044))!=SPEC["raw_sha256"]
        or digest(normalized(isolated,local))!=SPEC["normalized_sha256"]
        or len(isolated.relocation_ranges(local,1044))!=51):
        raise SystemExit("APU reset original instructions/field addends drift")
    with (ROOT/SPEC["evidence"]).open(newline="") as stream:
        rows=[r for r in csv.DictReader(stream,delimiter="\t") if r["address"]==SPEC["address"]]
    if (len(rows)!=1 or rows[0]["object_symbol"]!="S9xResetAPU" or rows[0]["result"]!="MATCH"
        or rows[0]["differing_bytes"]!="0" or rows[0]["unknown_relocations"]
        or rows[0]["normalized_equal"]!="True" or rows[0]["object_size"]!="1044"
        or rows[0]["target_span_sha256"]!=SPEC["target_sha256"]):
        raise SystemExit("frozen complete APU reset target proof drift")
    historical=json.loads((ROOT/"analysis/link_identity/historical_data.json").read_text())
    for name,address in PROVIDERS.items():
        witness=historical["bindings"]["apu"].get(LEGACY_NAMES[name])
        if witness is None or witness["address"]!=address or not witness["witnesses"]:
            raise SystemExit("frozen APU reset native provider address drift: "+name)
    state_path=BUILD/"GLOBALS.layout.o"
    run([CXX,*FLAGS,"-DZLIB",*includes,"-x","c++","-c",layout/"GLOBALS.CPP","-o",state_path])
    state=ELFFile(state_path);data=next(s for s in state.sections if s.name==".data")
    sha="8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    if data.size!=0xaf848 or digest(state.data[data.offset:data.offset+data.size])!=sha:
        raise SystemExit("frozen complete shared APU state/table data drift")
    owner=[r for r in historical["owners"] if r["unit"]=="globals" and r["symbol"]=="@section:.data"]
    if len(owner)!=1 or any(owner[0][k]!=v for k,v in
            {"address":0x345060,"size":0xaf848,"source_offset":0,"sha256":sha}.items()):
        raise SystemExit("frozen shared APU source-owner interval drift")
    for name,size in (("APU",224),("IAPU",60),("APURegisters",8),("Settings",328),
                      ("APUROM",64),("S9xAPUCycleLengths",1024),("S9xAPUCycles",1024)):
        symbol=state.find_symbol(name)
        if symbol.size!=size or symbol.value+0x345060!=PROVIDERS[next(k for k,v in LEGACY_NAMES.items() if v==name)]:
            raise SystemExit("original shared APU object geometry drift: "+name)
    dump=[r for r in historical["owners"] if r["unit"]=="apu" and r["symbol"]=="spc_dump_dsp"]
    ds=original.find_symbol("spc_dump_dsp")
    if (len(dump)!=1 or dump[0]["address"]!=0x335378 or dump[0]["size"]!=256
        or ds.value!=8 or ds.size!=256 or digest(original.symbol_bytes(ds,256))!=dump[0]["sha256"]):
        raise SystemExit("original shared DSP dump storage drift")
    script=BUILD/"apu_reset.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in PROVIDERS.items())
        +"SECTIONS { .text 0x0010a934 : { *(.text) } /DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked=BUILD/"apu_reset.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",script,obj,"-o",linked])
    final=ELFFile(linked);placed=final.find_symbol("S9xResetAPU")
    if placed.value!=0x10a934 or placed.size!=1044 or digest(final.symbol_bytes(placed,1044))!=SPEC["target_sha256"]:
        raise SystemExit("complete provider-linked APU reset target mismatch")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=[SPEC]:raise SystemExit("APU reset evidence ledger drift")
    print("APU reset source proof: MATCH S9xResetAPU; 1044/1044 complete provider-linked target bytes; 51 native relocations")

if __name__=="__main__":
    main()
