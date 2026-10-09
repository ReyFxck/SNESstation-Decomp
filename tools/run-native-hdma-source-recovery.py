#!/usr/bin/env python3
"""Prove native HDMA setup and scanline bodies (1468 historical linked bytes).

Preserves complete historical instructions, native shared state, 64-bit cycle
accounting and original memory/register callees. Isolation only rebases the
HDMA jump table by 24 bytes. Frozen normalized MATCH witnesses supplement a
fully linked original-source comparison, not an independent full target hash
or a fresh private-image rerun. Unselected historical DMA context is never
executed or promoted by this proof.
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
BUILD=ROOT/"build/matching/native-hdma-source-recovery"
CXX=ROOT/"build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE=ROOT/"analysis/functions/native_hdma_exact_1468.tsv"
FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections')
PROVIDERS={'DAT_00345268': 3428968,
 'DAT_003454e0': 3429600,
 'DAT_0034e2b0': 3465904,
 'DAT_0035b738': 3520312,
 'DAT_0035b788': 3520392,
 'DAT_0035c268': 3523176,
 'DAT_0035d360': 3527520,
 'DAT_0035d410': 3527696,
 'DAT_0035d430': 3527728,
 'DAT_003f2eb8': 4140728,
 'S9xSetPPU': 1413736,
 '_Z10S9xGetBytej': 1750588,
 '_Z10S9xGetWordj': 1752104,
 '_Z16S9xGetMemPointerj': 1750248,
 '__gxx_personality_v0': 0,
 'g_CPU_blob': 3429184}
LEGACY_NAMES={'DAT_00345268': 'missing',
 'DAT_003454e0': 'Settings',
 'DAT_0034e2b0': 'Memory',
 'DAT_0035b738': 'SNESGameFixes',
 'DAT_0035b788': 'PPU',
 'DAT_0035c268': 'IPPU',
 'DAT_0035d360': 'DMA',
 'DAT_0035d410': 'HDMAMemPointers',
 'DAT_0035d430': 'HDMABasePointers',
 'DAT_003f2eb8': 'HDMA_ModeByteCounts',
 'g_CPU_blob': 'CPU'}
SPECS=[{'address': '0x0012b3e8',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'historical_raw_sha256': '721b4c0c9aded51a3d4c5b188de87b1f0b2f786badd60c7b6b24c2cd77cda7cd',
  'isolated_raw_sha256': '721b4c0c9aded51a3d4c5b188de87b1f0b2f786badd60c7b6b24c2cd77cda7cd',
  'linked_sha256': 'fbfdf9897ffaf143e97cfc59f77a3005dd853a3c2322abea20dde61d7a28056c',
  'normalized_sha256': 'e039bf6c8ecc2fc9b70ca4ff46eee967f2fe068040e70851d3ab32fc0db2ea6e',
  'proof_level': 'linked-historical-reference',
  'relocations': '18',
  'size': '176',
  'source_file': 'src/snes9x/native_hdma.cpp',
  'symbol': 'S9xStartHDMA'},
 {'address': '0x0012b498',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv',
  'historical_raw_sha256': 'c52c4467ff866fbfa493f2f43b06db41b5f241cb470e6b06a07e6aeb292c907f',
  'isolated_raw_sha256': 'a037c8e2bdeb579c82d59ccaaa5811688f073b6c54e55abfd9635197bf5eb9f1',
  'linked_sha256': 'a13b7aa6e7616cb0d4232f01ced00d6e7e009a03c8b06b59fcf7b8858c458bbf',
  'normalized_sha256': 'a5e0770f853f56a71c60b50237b8fe74c88172dc437d4aebc1f773ad433efa0a',
  'proof_level': 'linked-historical-reference',
  'relocations': '64',
  'size': '1292',
  'source_file': 'src/snes9x/native_hdma.cpp',
  'symbol': 'S9xDoHDMA'}]
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
    original_path=BUILD/"DMA.historical.o"
    run([CXX,*FLAGS[:-1],"-DZLIB",*includes,"-x","c++","-c",layout/"DMA.CPP","-o",original_path])
    obj=BUILD/"native_hdma.o"
    run([CXX,*FLAGS,"-c",ROOT/"src/snes9x/native_hdma.cpp","-o",obj])
    original,isolated=ELFFile(original_path),ELFFile(obj)
    text=next(s for s in original.sections if s.name==".text")
    raw_sha="a1e981ed42e67cf6f29ec9aa638c21f45c531c536a629a4a06e0a4b053e04ed3"
    capture=json.loads((ROOT/"analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    frozen=[r for r in capture if r["name"]=="dma"]
    if (text.size!=9320 or digest(original.data[text.offset:text.offset+text.size])!=raw_sha
        or len(frozen)!=1 or any(frozen[0][k]!=v for k,v in
             {"address":0x129af4,"size":9320,"raw_sha256":raw_sha,"relocations":351}.items())):
        raise SystemExit("complete frozen original DMA code window drift")
    functions=[s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    ro=next(s for s in isolated.sections if s.name==".rodata")
    if (len(functions)!=2 or sum(s.size for s in functions)!=1468
        or {s.name for s in functions}!={r["symbol"] for r in SPECS} or ro.size!=32
        or any(s.size and s.info >> 4 in (1,2) and s.section_index < len(isolated.sections)
               and isolated.sections[s.section_index].name in (".data",".bss")
               for s in isolated.symbols if s.section_index)):
        raise SystemExit("native HDMA function/jump-table/shared-storage inventory drift")
    state_path=BUILD/"GLOBALS.layout.o"
    run([CXX,*FLAGS[:-1],"-DZLIB",*includes,"-x","c++","-c",layout/"GLOBALS.CPP","-o",state_path])
    state=ELFFile(state_path);data=next(s for s in state.sections if s.name==".data")
    sha="8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    owners=json.loads((ROOT/"analysis/link_identity/historical_data.json").read_text())["owners"]
    owner=[r for r in owners if r["unit"]=="globals" and r["symbol"]=="@section:.data"]
    if (data.size!=0xaf848 or digest(state.data[data.offset:data.offset+data.size])!=sha
        or len(owner)!=1 or any(owner[0][k]!=v for k,v in
            {"address":0x345060,"size":0xaf848,"source_offset":0,"sha256":sha}.items())):
        raise SystemExit("complete frozen shared HDMA state source-owner drift")
    for name,address,size in (("CPU",0x345340,104),("DMA",0x35d360,176),("missing",0x345268,174),
        ("PPU",0x35b788,2780),("IPPU",0x35c268,4340),("Settings",0x3454e0,328),
        ("Memory",0x34e2b0,54404),("SNESGameFixes",0x35b738,7),
        ("HDMAMemPointers",0x35d410,32),("HDMABasePointers",0x35d430,32),
        ("HDMA_ModeByteCounts",0x3f2eb8,32)):
        symbol=state.find_symbol(name)
        if symbol.size!=size or symbol.value+0x345060!=address:
            raise SystemExit("original HDMA provider geometry drift: "+name)
    # All native memory callees already have independently complete public
    # target proofs. Reuse their fixed source profiles and preserve the ABI.
    from build_source_tree import SOURCE_FIXED_FLAGS
    for source,name,address,size in (("s9xgetbyte","_Z10S9xGetBytej",0x1ab63c,708),
        ("s9xgetword","_Z10S9xGetWordj",0x1abc28,1020),
        ("s9xgetmempointer","_Z16S9xGetMemPointerj",0x1ab4e8,340)):
        src="src/snes9x/"+source+".cpp";callee=BUILD/(source+".o")
        run([CXX,*SOURCE_FIXED_FLAGS[src],"-c",ROOT/src,"-o",callee])
        symbol=ELFFile(callee).find_symbol(name)
        if symbol.size!=size or PROVIDERS[name]!=address:
            raise SystemExit("canonical native memory callee ABI drift: "+name)
    entry=[r for r in json.loads((ROOT/"analysis/link_identity/historical_data.json").read_text())["functions"]
           if r["symbol"]=="S9xSetPPU"]
    if len(entry)!=1 or entry[0]["address"]!=PROVIDERS["S9xSetPPU"] or entry[0]["size"]!=5000:
        raise SystemExit("original HDMA native register-write entry drift")
    script=BUILD/"native_hdma.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in PROVIDERS.items())
        +"SECTIONS {\n"+"".join(f" .text.{i} {int(r['address'],0):#x} : {{ *(.text.{r['symbol']}) }}\n" for i,r in enumerate(SPECS))
        +" .rodata 0x001b1f38 : { *(.rodata*) } /DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked=BUILD/"native_hdma.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",script,obj,"-o",linked]);final=ELFFile(linked)
    bindings={LEGACY_NAMES.get(name,name):address for name,address in PROVIDERS.items()}
    for symbol in original.symbols:
        if symbol.section_index==0 and symbol.name and symbol.name not in bindings:
            bindings[symbol.name]=0
    reference_script=BUILD/"DMA.historical.ld"
    reference_script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in bindings.items())
        +"SECTIONS { .text 0x00129af4 : { *(.text) } .rodata 0x001b1f20 : { *(.rodata*) } "
         ".data 0x0033ccc8 : { *(.data*) } .bss 0x7fc00000 : { *(.bss*) *(COMMON) } "
         "/DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    reference_path=BUILD/"DMA.historical.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",reference_script,original_path,"-o",reference_path]);reference=ELFFile(reference_path)
    for spec in SPECS:
        name,size,address=spec["symbol"],int(spec["size"]),int(spec["address"],0)
        old,local,placed,ref=original.find_symbol(name),isolated.find_symbol(name),final.find_symbol(name),reference.find_symbol(name)
        raw,historical=isolated.symbol_bytes(local,size),bytearray(original.symbol_bytes(old,size))
        if name=="S9xDoHDMA":
            if historical[316]!=24 or raw[316]!=0:
                raise SystemExit("historical HDMA jump-table rebase drift")
            historical[316]=0
        target=final.symbol_bytes(placed,size)
        if (any(s.size!=size for s in (old,local,placed,ref)) or placed.value!=address or ref.value!=address
            or raw!=historical or digest(original.symbol_bytes(old,size))!=spec["historical_raw_sha256"]
            or digest(raw)!=spec["isolated_raw_sha256"] or normalized(original,old)!=normalized(isolated,local)
            or digest(normalized(isolated,local))!=spec["normalized_sha256"]
            or len(isolated.relocation_ranges(local,size))!=int(spec["relocations"])
            or target!=reference.symbol_bytes(ref,size) or digest(target)!=spec["linked_sha256"]):
            raise SystemExit("native HDMA complete historical instructions/linked reference drift: "+name)
        with (ROOT/spec["evidence"]).open(newline="") as stream:
            witness=[r for r in csv.DictReader(stream,delimiter="\t") if r["address"]==spec["address"]]
        if (len(witness)!=1 or witness[0]["result"]!="MATCH" or witness[0]["differing_bytes"]!="0"
            or witness[0]["unknown_relocations"] or witness[0]["normalized_equal"]!="True"
            or witness[0]["object_size"]!=spec["size"] or witness[0]["object_symbol"]!=name):
            raise SystemExit("frozen normalized native HDMA instruction witness drift: "+name)
        print(f"native HDMA: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    old_ro=next(s for s in reference.sections if s.name==".rodata")
    new_ro=next(s for s in final.sections if s.name==".rodata")
    if reference.data[old_ro.offset+24:old_ro.offset+56]!=final.data[new_ro.offset:new_ro.offset+32]:
        raise SystemExit("fully linked original HDMA jump table drift")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=SPECS:raise SystemExit("native HDMA evidence ledger drift")
    print("native HDMA source proof: 2/2 functions; 1468 linked historical bytes; no duplicate shared state")

if __name__=="__main__":
    main()
