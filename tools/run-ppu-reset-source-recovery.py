#!/usr/bin/env python3
"""Prove all five original native PPU reset and helper bodies (3372 bytes).

Palette/controller helpers match all 364 public target bytes. Both resets
and mouse processing preserve original raw instruction bytes and match a
fully linked historical reference plus frozen normalized MATCH witnesses.
They have no independently frozen complete target digest; no fresh private
rerun or whole-image identity is claimed. Only selected historical functions
have nonzero proved providers in the comparison reference; other retained
context is never executed and is not promoted.
"""
from __future__ import annotations
import csv
import hashlib
import json
import re
import shlex
import subprocess
import sys
from pathlib import Path
from compare_elf_functions import ELFFile
ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build/matching/ppu-reset-source-recovery"
CXX=ROOT/"build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
CC=ROOT/"build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
EVIDENCE=ROOT/"analysis/functions/ppu_reset_exact_3372.tsv"
FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections')
PROVIDERS={'DAT_0033cac8': 3394248,
 'DAT_003454e0': 3429600,
 'DAT_0034e2b0': 3465904,
 'DAT_0035b788': 3520392,
 'DAT_0035c268': 3523176,
 'memset': 1688476,
 'snes_leaf_00104e50': 1068624}
LEGACY_NAMES={'DAT_0033cac8': 'mul_brightness',
 'DAT_003454e0': 'Settings',
 'DAT_0034e2b0': 'Memory',
 'DAT_0035b788': 'PPU',
 'DAT_0035c268': 'IPPU',
 'memset': 'memset',
 'snes_leaf_00104e50': 'S9xReadMousePosition'}
SPECS=[{'address': '0x001591a8',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'historical_raw_sha256': 'bc59f1d20e4fa9008f121882d99f241cbd89201a85a72aa675cdd84db318145b',
  'isolated_raw_sha256': 'bc59f1d20e4fa9008f121882d99f241cbd89201a85a72aa675cdd84db318145b',
  'linked_sha256': '31309e011a212ce11fe156e38c0ecfcec68efac5d26be4c8250bea3fce46434d',
  'normalized_sha256': '68f1d0affd34e67fa880eb1d0f165e5291c5eaf4252d74b3083f666f3641fad6',
  'proof_level': 'provider-linked-target',
  'relocations': '12',
  'size': '192',
  'source_file': 'src/snes9x/ppu_reset.cpp',
  'symbol': 'S9xFixColourBrightness'},
 {'address': '0x0015c124',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'historical_raw_sha256': 'c4302d9df3a025c9d1f0e534e5de99bbeaeade1a9f073466e892561ae1de379b',
  'isolated_raw_sha256': 'c4302d9df3a025c9d1f0e534e5de99bbeaeade1a9f073466e892561ae1de379b',
  'linked_sha256': '5a733a721624194a1d7add51c7541cc2ae44db590773b24e060ab78ec4c3b6ae',
  'normalized_sha256': 'f807685490f9b485146351820b22207f25b34093534d02293698943126c284c8',
  'proof_level': 'linked-historical-reference',
  'relocations': '41',
  'size': '1280',
  'source_file': 'src/snes9x/ppu_reset.cpp',
  'symbol': 'S9xResetPPU'},
 {'address': '0x0015c624',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'historical_raw_sha256': '3c875a979bca9d2a59aa4256623496e685dbfb1c39f30e57c973b38e6eae3617',
  'isolated_raw_sha256': '3c875a979bca9d2a59aa4256623496e685dbfb1c39f30e57c973b38e6eae3617',
  'linked_sha256': '12164cdac9e22b6446d876694dce19572920a2b9080c0770dd7d94afd51a040d',
  'normalized_sha256': '143608c7778b22e2d25fd3a67d6c4f0f34c0d4e7806cf9fcd9a66dbe4f489338',
  'proof_level': 'linked-historical-reference',
  'relocations': '41',
  'size': '1212',
  'source_file': 'src/snes9x/ppu_reset.cpp',
  'symbol': 'S9xSoftResetPPU'},
 {'address': '0x0015cae0',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'historical_raw_sha256': 'a2e7d2b8c1a2546796a93be7fd5a73e95f106f84d623b48de8125bb11aed57e8',
  'isolated_raw_sha256': 'a2e7d2b8c1a2546796a93be7fd5a73e95f106f84d623b48de8125bb11aed57e8',
  'linked_sha256': 'c8f37f15e213ed8870a8576b5068efc7547f43362dcc44965800dc47f8138620',
  'normalized_sha256': 'a2e7d2b8c1a2546796a93be7fd5a73e95f106f84d623b48de8125bb11aed57e8',
  'proof_level': 'linked-historical-reference',
  'relocations': '19',
  'size': '516',
  'source_file': 'src/snes9x/ppu_reset.cpp',
  'symbol': 'S9xProcessMouse'},
 {'address': '0x0015cdf8',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'historical_raw_sha256': '20be2e82fc41be55b0b0fda15d44663b4da8748d6c0ef5035f57ef2abd9cfc3e',
  'isolated_raw_sha256': 'b0bdf6316dd935e2b8b32b5e5467031aa5c122a4c161ac61f0ae610a115b64cd',
  'linked_sha256': '45c7ed5a455794f8f9e40a6c2bae90fa0e3612cff0c526a49737477e38d1d333',
  'normalized_sha256': 'e0650a95f74573dd3e1592645a9376865517ee6440c544168e01065cdc64de9e',
  'proof_level': 'provider-linked-target',
  'relocations': '20',
  'size': '172',
  'source_file': 'src/snes9x/ppu_reset.cpp',
  'symbol': 'S9xNextController'}]
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
    original_path=BUILD/"ppu.historical.o"
    run([CXX,*FLAGS[:-1],"-DZLIB",*includes,"-x","c++","-c",layout/"ppu.cpp","-o",original_path])
    obj=BUILD/"ppu_reset.o"
    run([CXX,*FLAGS,"-c",ROOT/"src/snes9x/ppu_reset.cpp","-o",obj])
    original,isolated=ELFFile(original_path),ELFFile(obj)
    functions=[s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    ro=next(s for s in isolated.sections if s.name==".rodata")
    if (len(functions)!=5 or sum(s.size for s in functions)!=3372
        or {s.name for s in functions}!={r["symbol"] for r in SPECS} or ro.size!=28
        or any(s.size and s.info >> 4 in (1,2) and s.section_index < len(isolated.sections)
               and isolated.sections[s.section_index].name in (".data",".bss")
               for s in isolated.symbols if s.section_index)):
        raise SystemExit("native PPU functions/controller table/shared-storage inventory drift")
    state_path=BUILD/"GLOBALS.layout.o"
    run([CXX,*FLAGS[:-1],"-DZLIB",*includes,"-x","c++","-c",layout/"GLOBALS.CPP","-o",state_path])
    state=ELFFile(state_path);data=next(s for s in state.sections if s.name==".data")
    sha="8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    if data.size!=0xaf848 or digest(state.data[data.offset:data.offset+data.size])!=sha:
        raise SystemExit("frozen complete shared PPU state data drift")
    owners=json.loads((ROOT/"analysis/link_identity/historical_data.json").read_text())["owners"]
    owner=[r for r in owners if r["unit"]=="globals" and r["symbol"]=="@section:.data"]
    if len(owner)!=1 or any(owner[0][k]!=v for k,v in
            {"address":0x345060,"size":0xaf848,"source_offset":0,"sha256":sha}.items()):
        raise SystemExit("frozen shared PPU source-owner interval drift")
    for name,address,size in (("PPU",0x35b788,2780),("IPPU",0x35c268,4340),
                             ("Settings",0x3454e0,328),("Memory",0x34e2b0,54404)):
        symbol=state.find_symbol(name)
        if symbol.size!=size or symbol.value+0x345060!=address:
            raise SystemExit("original PPU/native Settings provider geometry drift: "+name)
    tables_path=BUILD/"data.historical.o"
    run([CXX,*FLAGS[:-1],"-DZLIB",*includes,"-x","c++","-c",layout/"data.cpp","-o",tables_path])
    tables=ELFFile(tables_path);section=next(s for s in tables.sections if s.name==".data")
    table=tables.find_symbol("mul_brightness")
    captured=json.loads((ROOT/"analysis/link_identity/window35_data.json").read_text())["source_sections"]
    source=[r for r in captured if r["name"]=="color_math_tables"]
    table_sha="b9777e8c09650d936e0a09af6506a492784000792fa6582a53bdd2b1ff4337eb"
    if (len(source)!=1 or section.size!=4608 or digest(tables.data[section.offset:section.offset+4608])!=table_sha
        or any(source[0][k]!=v for k,v in {"address":0x33bac8,"size":4608,"full_size":4608,
                "full_sha256":table_sha,"raw_sha256":table_sha,"relocations":0}.items())
        or table.value!=4096 or table.size!=512 or table.value+source[0]["address"]!=0x33cac8):
        raise SystemExit("frozen 512-byte brightness table provider/source interval drift")
    # Native four-argument mouse call resolves to the already proved zero leaf.
    # Returning zero touches no output references, so its existing int-return
    # implementation satisfies the original bool8 ABI for this actual entry.
    from build_source_tree import DEFAULT_CFLAGS
    leaf_path=BUILD/"mouse-leaf.o"
    run([CC,*shlex.split(DEFAULT_CFLAGS),"-c",ROOT/"src/ps2/address_leaf_recovered.c","-o",leaf_path])
    leaf=ELFFile(leaf_path);symbol=leaf.find_symbol("snes_leaf_00104e50")
    if symbol.size!=8 or leaf.symbol_bytes(symbol,8)!=bytes.fromhex("0800e0032d100000"):
        raise SystemExit("native original no-mouse leaf drift")
    with (ROOT/"analysis/matching/hunt1000plus-v41-validated-28.tsv").open(newline="") as stream:
        witness=[r for r in csv.DictReader(stream,delimiter="\t") if r["address"]=="0x00104e50"]
    if (len(witness)!=1 or witness[0]["object_symbol"]!=symbol.name or witness[0]["result"]!="MATCH"
        or witness[0]["raw_equal"]!="True" or witness[0]["object_size"]!="8"):
        raise SystemExit("frozen exact native mouse leaf witness drift")
    script=BUILD/"ppu_reset.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in PROVIDERS.items())
        +"SECTIONS {\n"+"".join(f" .text.{i} {int(r['address'],0):#x} : {{ *(.text.{r['symbol']}) }}\n" for i,r in enumerate(SPECS))
        +" .rodata 0x001b7dbc : { *(.rodata*) } /DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked=BUILD/"ppu_reset.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",script,obj,"-o",linked]);final=ELFFile(linked)
    bindings={LEGACY_NAMES[name]:address for name,address in PROVIDERS.items()}
    # Unselected code and exception/context data are comparison-only. They
    # receive no runnable-provider claim and are never part of byte promotions.
    for symbol in original.symbols:
        if symbol.section_index==0 and symbol.name and symbol.name not in bindings:
            bindings[symbol.name]=0
    reference_script=BUILD/"ppu.historical.ld"
    reference_script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in bindings.items())
        +"SECTIONS { .text 0x00159058 : { *(.text) } .rodata 0x001b7318 : { *(.rodata*) } "
         ".data 0x003f4bf0 : { *(.data*) } .bss 0x7fc00000 : { *(.bss*) *(COMMON) } "
         "/DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    reference_path=BUILD/"ppu.historical.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",reference_script,original_path,"-o",reference_path]);reference=ELFFile(reference_path)
    line_pattern=re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} ){3}[0-9a-fA-F]{2})\s")
    code={}
    for line in (ROOT/"analysis/functions/progress13_targets.asm").read_text().splitlines():
        match=line_pattern.match(line)
        if match:code[int(match[1],16)]=bytes.fromhex(match[2])
    for spec in SPECS:
        name,size,address=spec["symbol"],int(spec["size"]),int(spec["address"],0)
        old,local,placed,ref=original.find_symbol(name),isolated.find_symbol(name),final.find_symbol(name),reference.find_symbol(name)
        target=final.symbol_bytes(placed,size)
        if (any(s.size!=size for s in (old,local,placed,ref)) or placed.value!=address or ref.value!=address
            or digest(original.symbol_bytes(old,size))!=spec["historical_raw_sha256"]
            or digest(isolated.symbol_bytes(local,size))!=spec["isolated_raw_sha256"]
            or normalized(original,old)!=normalized(isolated,local)
            or digest(normalized(isolated,local))!=spec["normalized_sha256"]
            or len(isolated.relocation_ranges(local,size))!=int(spec["relocations"])
            or target!=reference.symbol_bytes(ref,size) or digest(target)!=spec["linked_sha256"]):
            raise SystemExit("native PPU original instructions/linked historical reference drift: "+name)
        if spec["proof_level"]=="provider-linked-target":
            if b"".join(code[a] for a in range(address,address+size,4))!=target:
                raise SystemExit("complete PPU palette/controller public target listing drift: "+name)
        else:
            if isolated.symbol_bytes(local,size)!=original.symbol_bytes(old,size):
                raise SystemExit("complete historical PPU field-addend/raw-byte drift: "+name)
            with (ROOT/spec["evidence"]).open(newline="") as stream:
                witness=[r for r in csv.DictReader(stream,delimiter="\t") if r["address"]==spec["address"]]
            if (len(witness)!=1 or witness[0]["result"]!="MATCH" or witness[0]["differing_bytes"]!="0"
                or witness[0]["unknown_relocations"] or witness[0]["normalized_equal"]!="True"
                or witness[0]["object_size"]!=spec["size"] or witness[0]["object_symbol"]!=name):
                raise SystemExit("frozen normalized native PPU instruction witness drift: "+name)
        print(f"native PPU: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=SPECS:raise SystemExit("native PPU evidence ledger drift")
    print("native PPU source proof: 5/5 functions; 3372 linked historical bytes; 364 complete public target bytes")

if __name__=="__main__":
    main()
