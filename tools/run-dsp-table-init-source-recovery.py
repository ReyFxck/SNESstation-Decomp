#!/usr/bin/env python3
"""Prove the 272-byte PS2 DSP table initializer against its frozen target hash.

Rebuild the public V51 float variant, compare every historical instruction,
then resolve native math/runtime entries, both original 8192-byte tables and
the two normal-double constants. The complete target digest independently
checks every relocated instruction. Behavioral runtime/math providers retain
their existing claim level; no fresh private-image rerun is needed or claimed.
"""
from __future__ import annotations
import csv
import hashlib
import json
import subprocess
from pathlib import Path
from compare_elf_functions import ELFFile
ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build/matching/dsp-table-init-source-recovery"
CC=ROOT/"build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
EVIDENCE=ROOT/"analysis/functions/dsp_table_init_exact_272.tsv"
FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET')
REFERENCE_FLAGS=('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DALIGN_DWORD', '-DCODE_PLATFORM=3')
PROVIDERS={'DAT_0033ce78': 3395192, 'DAT_0033ee78': 3403384, 'cosf_0019fddc': 1703388, 'sinf_001a0024': 1703972, 'snes_litodp': 1719088, 'snes_dpadd': 1717680, 'snes_dpmul': 1717888, 'snes_dptofp': 1719488}
SPEC={'address': '0x0012c02c', 'symbol': 'S9xInitDSP', 'source_file': 'src/snes9x/dsp_table_init.c', 'size': '272', 'relocations': '20', 'historical_raw_sha256': 'ca7c3740c9e6f20e9c256e21cee751be3580701922c26de4c430ecdfc05c5794', 'isolated_raw_sha256': '28a2f5e461dfd092f3f2c252d09376387452f5ca8b2ef5d2ca6e1e4db94c7a66', 'normalized_sha256': 'e9bda83967f983d3edf9c6a3fe954de9e3a9e95ed1a065fcbdebee4192a9a9d4', 'target_sha256': 'be74a18720317a6ffb3ecc3cdfa0677e896e13a217946bc28aeddcff54547858', 'proof_level': 'provider-linked-target', 'evidence': 'analysis/matching/hunt1041-v51-validated-16.tsv'}
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
    if run([CC,"-dumpmachine"]).strip()!="ee" or run([CC,"-dumpversion"]).strip()!="3.2.2":
        raise SystemExit("wrong historical compiler")
    BUILD.mkdir(parents=True,exist_ok=True)
    original_path=BUILD/"dsp-v51-reference.o"
    run([CC,*REFERENCE_FLAGS,"-c",ROOT/"matching/candidates/hunt1041_v51_dsp.c","-o",original_path])
    obj=BUILD/"dsp_table_init.o"
    run([CC,*FLAGS,"-c",ROOT/"src/snes9x/dsp_table_init.c","-o",obj])
    original,isolated=ELFFile(original_path),ELFFile(obj)
    old,local=original.find_symbol("snes_p26_0012c02c"),isolated.find_symbol("S9xInitDSP")
    functions=[s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    text=[s for s in isolated.sections if s.name==".text" or s.name.startswith(".text.")]
    ro=next(s for s in isolated.sections if s.name==".rodata")
    constants=bytes.fromhex("182d4454fb211940182d4454fb211940")
    if (len(functions)!=1 or len(text)!=1 or text[0].size!=272 or old.size!=272 or local.size!=272
        or ro.size!=16 or isolated.data[ro.offset:ro.offset+16]!=constants
        or digest(original.symbol_bytes(old,272))!=SPEC["historical_raw_sha256"]
        or digest(isolated.symbol_bytes(local,272))!=SPEC["isolated_raw_sha256"]
        or digest(normalized(original,old))!=SPEC["normalized_sha256"]
        or digest(normalized(isolated,local))!=SPEC["normalized_sha256"]
        or len(isolated.relocation_ranges(local,272))!=20):
        raise SystemExit("DSP initializer historical instructions/constants drift")
    selected=json.loads((ROOT/"analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    capture=[s for s in selected if s.get("address")==0x12c02c and s.get("symbol")=="snes_p26_0012c02c"]
    expected={"size":272,"new_bytes":272,"raw_sha256":SPEC["historical_raw_sha256"],"relocations":20}
    if len(capture)!=1 or any(capture[0].get(k)!=v for k,v in expected.items()):
        raise SystemExit("frozen DSP initializer source-window drift")
    with (ROOT/SPEC["evidence"]).open(newline="") as stream:
        rows=[r for r in csv.DictReader(stream,delimiter="\t") if r["address"]==SPEC["address"]]
    if (len(rows)!=1 or rows[0]["object_symbol"]!="snes_p26_0012c02c" or rows[0]["result"]!="MATCH"
        or rows[0]["differing_bytes"]!="0" or rows[0]["unknown_relocations"]
        or rows[0]["object_size"]!="272" or rows[0]["target_span_sha256"]!=SPEC["target_sha256"]):
        raise SystemExit("frozen complete DSP initializer target proof drift")
    owners=json.loads((ROOT/"analysis/link_identity/historical_data.json").read_text())["owners"]
    for name,address in (("CosTable2",0x33ce78),("SinTable2",0x33ee78)):
        found=[r for r in owners if r["unit"]=="dsp1" and r["symbol"]==name]
        if len(found)!=1 or found[0]["address"]!=address or found[0]["size"]!=8192:
            raise SystemExit("original DSP float table provider geometry drift")
    script=BUILD/"dsp_table_init.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in PROVIDERS.items())
        +"SECTIONS { .text 0x0012c02c : { *(.text) } .rodata 0x001b20c8 : { *(.rodata*) } /DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked=BUILD/"dsp_table_init.elf"
    run([CC.with_name("ee-ld"),"-EL","-T",script,obj,"-o",linked]);final=ELFFile(linked);placed=final.find_symbol("S9xInitDSP")
    if placed.value!=0x12c02c or placed.size!=272 or digest(final.symbol_bytes(placed,272))!=SPEC["target_sha256"]:
        raise SystemExit("complete provider-linked DSP initializer target mismatch")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=[SPEC]:raise SystemExit("DSP initializer evidence ledger drift")
    print("DSP table initializer source proof: MATCH S9xInitDSP; 272/272 complete provider-linked target bytes")

if __name__=="__main__":
    main()
