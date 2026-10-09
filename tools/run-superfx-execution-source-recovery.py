#!/usr/bin/env python3
"""Prove all 184 native Super FX execution/IRQ bridge target bytes.

Retains the original PS2 Settings field offsets, register state, no-argument
entry, FxEmulate(uint32) ABI and canonical CPU IRQ setter. Matches complete
historical instructions and the frozen V51 complete target digest. Rebuilds
public source only; no fresh private image execution is claimed.
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
BUILD=ROOT/"build/matching/superfx-execution-source-recovery"
CXX=ROOT/"build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE=ROOT/"analysis/functions/superfx_execution_exact_184.tsv"
FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET', '-ffunction-sections')
PROVIDERS={'DAT_003454e0': 3429600,
 'DAT_0034e2b0': 3465904,
 'S9xSetIRQ': 1138984,
 '_Z9FxEmulatej': 1247684}
SPEC={'address': '0x0015d334',
 'evidence': 'analysis/matching/hunt1041-v51-validated-16.tsv',
 'normalized_sha256': 'f4ff06bb60968c2824690d290e2218aca36eca10ae0c5bee5e68862ad5a2982c',
 'proof_level': 'provider-linked-target',
 'raw_sha256': '77be934bd65a29fef9d4242f6f543614f54d5fba33581167be9d39278e640f95',
 'relocations': '8',
 'size': '184',
 'source_file': 'src/snes9x/ppu_reset.cpp',
 'symbol': 'S9xSuperFXExec',
 'target_sha256': '25583590688ba0426131d17ee7da309a71c9f56675304f70978b89d1783f3636'}
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
    run([CXX,*FLAGS,"-c",ROOT/SPEC["source_file"],"-o",obj])
    original,isolated=ELFFile(original_path),ELFFile(obj)
    old,local=original.find_symbol("S9xSuperFXExec"),isolated.find_symbol("S9xSuperFXExec")
    if (old.size!=184 or local.size!=184 or original.symbol_bytes(old,184)!=isolated.symbol_bytes(local,184)
        or digest(isolated.symbol_bytes(local,184))!=SPEC["raw_sha256"]
        or digest(normalized(isolated,local))!=SPEC["normalized_sha256"]
        or len(isolated.relocation_ranges(local,184))!=int(SPEC["relocations"])):
        raise SystemExit("original native Super FX execution instructions/field addends drift")
    with (ROOT/SPEC["evidence"]).open(newline="") as stream:
        witness=[r for r in csv.DictReader(stream,delimiter="\t") if r["address"]==SPEC["address"]]
    if (len(witness)!=1 or witness[0]["result"]!="MATCH" or witness[0]["differing_bytes"]!="0"
        or witness[0]["unknown_relocations"] or witness[0]["object_size"]!="184"
        or witness[0]["object_symbol"]!="snes_p22_0015d334"
        or witness[0]["target_span_sha256"]!=SPEC["target_sha256"]):
        raise SystemExit("frozen complete native Super FX execution target witness drift")
    # Both callees now have canonical implementations with the original ABI.
    # Rebuild them and retain their already frozen complete instruction hashes.
    fx_path=BUILD/"native_fxemu.o"
    run([CXX,*FLAGS[:-1],"-c",ROOT/"src/snes9x/native_fxemu.cpp","-o",fx_path])
    fx=ELFFile(fx_path);symbol=fx.find_symbol("_Z9FxEmulatej")
    with (ROOT/"analysis/functions/native_fxemu_exact_3456.tsv").open(newline="") as stream:
        row=next(r for r in csv.DictReader(stream,delimiter="\t") if r["symbol"]==symbol.name)
    if (symbol.size!=180 or row["address"]!="0x001309c4"
        or digest(fx.symbol_bytes(symbol,180))!=row["isolated_raw_sha256"]):
        raise SystemExit("native Super FX execution callee source/ABI drift")
    cpu_path=BUILD/"cpu_execution.o"
    run([CXX,*FLAGS[:-1],"-c",ROOT/"src/snes9x/cpu_execution.cpp","-o",cpu_path])
    cpu=ELFFile(cpu_path);symbol=cpu.find_symbol("S9xSetIRQ")
    with (ROOT/"analysis/functions/cpu_execution_exact_2448.tsv").open(newline="") as stream:
        row=next(r for r in csv.DictReader(stream,delimiter="\t") if r["symbol"]==symbol.name)
    if symbol.size!=76 or row["address"]!="0x00116128" or digest(cpu.symbol_bytes(symbol,76))!=row["raw_sha256"]:
        raise SystemExit("native Super FX IRQ setter source/ABI drift")
    script=BUILD/"superfx_execution.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in PROVIDERS.items())
        +"SECTIONS { .text 0x0015d334 : { *(.text.S9xSuperFXExec) } /DISCARD/ : { *(.text*) *(.rodata*) *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked=BUILD/"superfx_execution.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",script,obj,"-o",linked])
    final=ELFFile(linked);placed=final.find_symbol("S9xSuperFXExec")
    if placed.value!=0x15d334 or placed.size!=184 or digest(final.symbol_bytes(placed,184))!=SPEC["target_sha256"]:
        raise SystemExit("complete provider-linked native Super FX execution target mismatch")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=[SPEC]:raise SystemExit("native Super FX execution ledger drift")
    print("native Super FX execution source proof: MATCH S9xSuperFXExec; 184/184 provider-linked target bytes; native FxEmulate/IRQ callees")

if __name__=="__main__":
    main()
