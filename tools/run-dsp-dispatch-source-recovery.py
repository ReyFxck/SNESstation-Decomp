#!/usr/bin/env python3
"""Prove original DSP1 init/reset and both native dispatchers.

All 200 historical instruction bytes reproduce the frozen public source
windows after masking only known relocations. Fully linked historical
references additionally check every state-field/provider addend, including
reuse of the original once-only initialization flag. No new private-target
digest or DSP math implementation recovery is claimed.
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
BUILD=ROOT/"build/matching/dsp-dispatch-source-recovery"
CXX=ROOT/"build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE=ROOT/"analysis/functions/dsp_dispatch_exact_200.tsv"
FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET')
SPECS=[{'address': '0x0012e688',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': 'd56461415443f7fdf599059f46a510c1d55b6e099a9d8a0d495869bdda84382c',
  'isolated_raw_sha256': '71d75456e6de44982586c8b8ca2850f91a2dcdbf2a818eb2e99390cbf003a0d1',
  'linked_sha256': 'e57b7aa76188c8ccff99957d54c21d64efbc6ab64a4e2458a3615e76efc20731',
  'normalized_sha256': '71d75456e6de44982586c8b8ca2850f91a2dcdbf2a818eb2e99390cbf003a0d1',
  'proof_level': 'linked-historical-reference',
  'relocations': '4',
  'size': '60',
  'source_file': 'src/snes9x/dsp_dispatch.cpp',
  'symbol': '_Z11S9xInitDSP1v'},
 {'address': '0x0012e6c4',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '71d39fd7d79842bb5737b5762678d551f3857867d9391a0b8a67ee1a85ae403a',
  'isolated_raw_sha256': '71d39fd7d79842bb5737b5762678d551f3857867d9391a0b8a67ee1a85ae403a',
  'linked_sha256': '65f7539c53b0180edc16897624b1f2e0f409f60e28a21fc061ec034b3a708c9a',
  'normalized_sha256': '71d39fd7d79842bb5737b5762678d551f3857867d9391a0b8a67ee1a85ae403a',
  'proof_level': 'linked-historical-reference',
  'relocations': '4',
  'size': '64',
  'source_file': 'src/snes9x/dsp_dispatch.cpp',
  'symbol': 'S9xResetDSP1'},
 {'address': '0x0012e704',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '9df035f261c6ac454414760917db0557b960d427427d6b0c77e522e481419c22',
  'isolated_raw_sha256': '9df035f261c6ac454414760917db0557b960d427427d6b0c77e522e481419c22',
  'linked_sha256': 'e8b5e649edf6aecc51e46cdeca6ba6cb0d2d0c54e2a9be53bf17cab33629d127',
  'normalized_sha256': '9df035f261c6ac454414760917db0557b960d427427d6b0c77e522e481419c22',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '36',
  'source_file': 'src/snes9x/dsp_dispatch.cpp',
  'symbol': 'S9xGetDSP'},
 {'address': '0x0012e728',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_raw_sha256': '408bb029986a26f74519e4938659ad18f5624fc7e5ebe218751975885508c0c4',
  'isolated_raw_sha256': '408bb029986a26f74519e4938659ad18f5624fc7e5ebe218751975885508c0c4',
  'linked_sha256': '883a7a5aac97c81fda3c5c3f0812d4589f3fe2e060056c30ff70ff343f50e0b4',
  'normalized_sha256': '408bb029986a26f74519e4938659ad18f5624fc7e5ebe218751975885508c0c4',
  'proof_level': 'linked-historical-reference',
  'relocations': '2',
  'size': '40',
  'source_file': 'src/snes9x/dsp_dispatch.cpp',
  'symbol': 'S9xSetDSP'}]
PROVIDERS={'DAT_00341660': 3413600, 'S9xInitDSP': 1228844, 'DAT_00345628': 3429928, 'PTR_FUN_0034165c': 3413596, 'PTR_FUN_00341658': 3413592}
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
    import hunt1000plus_v47_closure as historical
    import hunt1041_v52_closure as layout_recipe
    archive=historical.download_archive(historical.SNES_141_1_ARCHIVE,historical.SNES_CACHE)
    source_root=historical.safe_extract_archive(archive,historical.SNES_CACHE/"source-1.41-1",historical.SNES_141_1_ARCHIVE.source_directory)
    historical.ensure_git_commit(historical.PS2DEV,historical.PS2DEV_REPO,historical.PS2DEV_COMMIT)
    upstream=source_root/"snes9x"
    newlib=historical.PS2DEV/"ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    BUILD.mkdir(parents=True,exist_ok=True)
    compat=BUILD/"compat";compat.mkdir(exist_ok=True)
    (compat/"memory.h").write_text("#include <string.h>\n")
    includes=["-I"+str(p) for p in (compat,newlib,upstream,upstream/"unzip",source_root/"zlib")]
    original_path=BUILD/"DSP1.historical.o"
    run([CXX,*FLAGS,*includes,"-x","c++","-c",upstream/"DSP1.CPP","-o",original_path])
    obj=BUILD/"dsp_dispatch.o"
    run([CXX,*FLAGS,"-c",ROOT/"src/snes9x/dsp_dispatch.cpp","-o",obj])
    original,isolated=ELFFile(original_path),ELFFile(obj)
    functions=[s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    text=[s for s in isolated.sections if s.name==".text" or s.name.startswith(".text.")]
    if len(functions)!=4 or len(text)!=1 or text[0].size!=200 or {s.name for s in functions}!={r["symbol"] for r in SPECS}:
        raise SystemExit("DSP native dispatch instruction inventory drift")
    script=BUILD/"dsp_dispatch.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in PROVIDERS.items())
        +"SECTIONS { .text 0x0012e688 : { *(.text) } /DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked=BUILD/"dsp_dispatch.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",script,obj,"-o",linked]);final=ELFFile(linked)
    data_proof=json.loads((ROOT/"analysis/link_identity/historical_data.json").read_text())
    dsp_bindings=data_proof["bindings"]["dsp1"]
    if dsp_bindings["@section:.data"]["address"]!=0x33ce78 or dsp_bindings["_Z7InitDSPv"]["address"]!=0x12c02c:
        raise SystemExit("frozen DSP data/math-init address drift")

    # Reuse the full original data section: its function pointer slots and
    # one-byte initializer flag retain all original relocation addends.
    # Only the selected 200 bytes are compared. Other code is not executed;
    # unused external dependencies are zero-bound in this reference artifact.
    # InitDSP itself is separately assigned its independently recorded native
    # entry because unselected leading math code has a different PS2 layout.
    bindings={"DSP1":0x345628}
    for s in original.symbols:
        if s.name and s.section_index==0:bindings.setdefault(s.name,0)
    refscript=BUILD/"DSP1.reference.ld"
    refscript.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in bindings.items())
        +"_Z7InitDSPv = 0x0012c02c;\nSECTIONS { .text 0x0012c0a0 : { *(.text) } .data 0x0033ce78 : { *(.data*) } .rodata 0x02000000 : { *(.rodata*) } .bss 0x03000000 : { *(.bss*) *(COMMON) } /DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    reference_path=BUILD/"DSP1.reference.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",refscript,original_path,"-o",reference_path]);reference=ELFFile(reference_path)
    for name,address in (("_Z7InitDSPv",0x12c02c),("SetDSP",0x341658),("GetDSP",0x34165c),("_ZZ11S9xInitDSP1vE4init",0x341660)):
        if reference.find_symbol(name).value!=address:raise SystemExit("DSP historical data/provider placement drift: "+name)

    old_build=layout_recipe.BUILD
    try:
        layout_recipe.BUILD=BUILD/"state-layout"
        _source,_original,layout=layout_recipe.prepare_snes_layout();layout_recipe.patch_sources(layout)
        state_path=BUILD/"GLOBALS.layout.o"
        state_includes=["-I"+str(p) for p in (compat,newlib,layout,layout/"unzip",source_root/"zlib")]
        run([CXX,*FLAGS,"-DZLIB",*state_includes,"-x","c++","-c",layout/"GLOBALS.CPP","-o",state_path])
    finally:
        layout_recipe.BUILD=old_build
    state=ELFFile(state_path);data=next(s for s in state.sections if s.name==".data")
    state_sha="8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    owners=[r for r in data_proof["owners"] if r["unit"]=="globals" and r["symbol"]=="@section:.data"]
    if (data.size!=0xaf848 or digest(state.data[data.offset:data.offset+data.size])!=state_sha
        or state.find_symbol("DSP1").value+0x345060!=0x345628
        or len(owners)!=1 or owners[0]["sha256"]!=state_sha or owners[0]["address"]!=0x345060):
        raise SystemExit("frozen shared DSP1 state layout drift")
    selected=json.loads((ROOT/"analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    for spec in SPECS:
        name,size,address=spec["symbol"],int(spec["size"]),int(spec["address"],0)
        captures=[s for s in selected if s.get("symbol")==name and s.get("address")==address]
        expected={"size":size,"new_bytes":size,"raw_sha256":spec["historical_raw_sha256"],"relocations":int(spec["relocations"])}
        if len(captures)!=1 or any(captures[0].get(k)!=v for k,v in expected.items()):raise SystemExit("frozen DSP dispatch instruction window drift: "+name)
        old,local,placed,ref=(e.find_symbol(name) for e in (original,isolated,final,reference))
        if (any(s.size!=size for s in (old,local,placed,ref)) or placed.value!=address or ref.value!=address
            or digest(original.symbol_bytes(old,size))!=spec["historical_raw_sha256"]
            or digest(isolated.symbol_bytes(local,size))!=spec["isolated_raw_sha256"]
            or digest(normalized(original,old))!=spec["normalized_sha256"]
            or digest(normalized(isolated,local))!=spec["normalized_sha256"]
            or len(isolated.relocation_ranges(local,size))!=int(spec["relocations"])
            or final.symbol_bytes(placed,size)!=reference.symbol_bytes(ref,size)
            or digest(final.symbol_bytes(placed,size))!=spec["linked_sha256"]):
            raise SystemExit("DSP native dispatch instructions/state/provider drift: "+name)
        print(f"DSP dispatch: MATCH {name} bytes={size}/{size} proof=linked-historical-reference")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=SPECS:raise SystemExit("DSP native dispatch evidence drift")
    print("DSP native dispatch source proof: 4/4 functions; 200/200 historical instructions and fully linked reference bytes")

if __name__=="__main__":
    main()
