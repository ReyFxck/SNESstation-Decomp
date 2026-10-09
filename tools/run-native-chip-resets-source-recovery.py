#!/usr/bin/env python3
"""Prove original native C4, DMA and SA1 reset bodies.

All 416 raw historical instruction bytes retain their state-field addends.
The DMA and SA1 resets additionally match all 376 public target-listing bytes.
The 40-byte C4 initializer matches its frozen raw source window and a fully
linked historical reference; no new private-target digest is claimed for it.
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
ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-reset-helpers-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/functions/native_chip_resets_exact_416.tsv"
FLAGS = ('-G0', '-EL', '-pipe', '-w', '-fomit-frame-pointer', '-fstrict-aliasing', '-fno-common', '-fshort-double', '-mlong64', '-mhard-float', '-mno-abicalls', '-march=r5900', '-mtune=r5900', '-Os', '-DPS2_EE', '-D_EE', '-DLSB_FIRST', '-DVAR_CYCLES', '-DCPU_SHUTDOWN', '-DSPC700_SHUTDOWN', '-DEXECUTE_SUPERFX_PER_LINE', '-DSPC700_C', '-DUNZIP_SUPPORT', '-DNO_INLINE_SET_GET') + ("-ffunction-sections",)
PROVIDERS = {'DAT_0034e2b0': 3465904, 'DAT_0035d360': 3527520, 'g_SA1_blob': 3431160, 'memset': 1688476}
SPECS = [{'address': '0x0010c300',
  'evidence': 'analysis/link_identity/code_windows.json',
  'historical_source': 'c4emu.cpp',
  'linked_sha256': '845f26c3e2bd360f6933da5376226d2c9ab199215ec76996ff77dd91c5ca75af',
  'normalized_sha256': 'e7082eb7f9758afbbfeaedc930c55cf08e6d016b139bc249485b9b368b569ce6',
  'proof_level': 'linked-historical-reference',
  'raw_sha256': 'd4648679568fc02f444631ea0570f1bb3c0bf0fd3a70414ab76129b1a1a88d5a',
  'relocations': '3',
  'size': '40',
  'source_file': 'src/snes9x/native_chip_resets.cpp',
  'symbol': 'S9xInitC4'},
 {'address': '0x0012b9a4',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'historical_source': 'DMA.CPP',
  'linked_sha256': '9b7eeb198a7b2d29d2ebcb1bc8e75cb0a3cbfdbcec952af8f4767365123a4984',
  'normalized_sha256': 'e21dcd09a3cb7bfd1bb0b93b2a72d8c6cce0c646c634317688434da53db3e323',
  'proof_level': 'provider-linked-target',
  'raw_sha256': '40d7def6580e0b791495a2799753425614cca0cd48930c7944ae53f0c22456df',
  'relocations': '6',
  'size': '184',
  'source_file': 'src/snes9x/native_chip_resets.cpp',
  'symbol': 'S9xResetDMA'},
 {'address': '0x0015d8ec',
  'evidence': 'analysis/functions/progress13_targets.asm',
  'historical_source': 'sa1.cpp',
  'linked_sha256': '614e28550e17bd8c157c24fedcbf6e568d020f6dad23b9916086330c01d4b6b3',
  'normalized_sha256': 'ccb4f601fb2ebcee2211b470f131b8ae0e9c378491b91594fc0292a41eb2aa12',
  'proof_level': 'provider-linked-target',
  'raw_sha256': 'ccb4f601fb2ebcee2211b470f131b8ae0e9c378491b91594fc0292a41eb2aa12',
  'relocations': '5',
  'size': '192',
  'source_file': 'src/snes9x/native_chip_resets.cpp',
  'symbol': 'S9xSA1Init'}]
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
    sys.path.insert(0,str(ROOT / "tools/history/research"))
    import hunt1000plus_v47_closure as historical
    archive = historical.download_archive(historical.SNES_141_1_ARCHIVE,historical.SNES_CACHE)
    source_root = historical.safe_extract_archive(archive,historical.SNES_CACHE / "source-1.41-1",historical.SNES_141_1_ARCHIVE.source_directory)
    historical.ensure_git_commit(historical.PS2DEV,historical.PS2DEV_REPO,historical.PS2DEV_COMMIT)
    upstream=source_root/"snes9x"
    newlib=historical.PS2DEV/"ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    BUILD.mkdir(parents=True,exist_ok=True)
    compat=BUILD/"compat";compat.mkdir(exist_ok=True)
    (compat/"memory.h").write_text("#include <string.h>\n")
    includes=["-I"+str(p) for p in (compat,newlib,upstream,upstream/"unzip",source_root/"zlib")]
    originals={}
    for spec in SPECS:
        filename=spec["historical_source"];obj=BUILD/(filename.split(".")[0]+".historical.o")
        run([CXX,*FLAGS[:-1],*includes,"-x","c++","-c",upstream/filename,"-o",obj])
        originals[spec["symbol"]]=ELFFile(obj)
    obj=BUILD/"native_chip_resets.o"
    run([CXX,*FLAGS,"-c",ROOT/"src/snes9x/native_chip_resets.cpp","-o",obj])
    isolated=ELFFile(obj)
    functions=[s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    if len(functions)!=3 or {s.name for s in functions}!={r["symbol"] for r in SPECS} or sum(s.size for s in functions)!=416:
        raise SystemExit("native reset function inventory drift")
    script=BUILD/"native_chip_resets.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in PROVIDERS.items())+"SECTIONS {\n"
        +"".join(f" .text.{i} {int(r['address'],0):#x} : {{ *(.text.{r['symbol']}) }}\n" for i,r in enumerate(SPECS))
        +" /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked=BUILD/"native_chip_resets.elf"
    run([CXX.with_name("ee-ld"),"-EL","-T",script,obj,"-o",linked]);final=ELFFile(linked)
    selected=json.loads((ROOT/"analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    line_pattern=re.compile(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} ){3}[0-9a-fA-F]{2})\s")
    for spec in SPECS:
        name,size,address=spec["symbol"],int(spec["size"]),int(spec["address"],0)
        original=originals[name];old,local,placed=original.find_symbol(name),isolated.find_symbol(name),final.find_symbol(name)
        if (any(s.size!=size for s in (old,local,placed)) or placed.value!=address
            or original.symbol_bytes(old,size)!=isolated.symbol_bytes(local,size)
            or digest(original.symbol_bytes(old,size))!=spec["raw_sha256"]
            or digest(normalized(isolated,local))!=spec["normalized_sha256"]
            or len(isolated.relocation_ranges(local,size))!=int(spec["relocations"])
            or digest(final.symbol_bytes(placed,size))!=spec["linked_sha256"]):
            raise SystemExit("native reset raw instructions/state addends drift: "+name)
        if name=="S9xInitC4":
            captures=[s for s in selected if s.get("symbol")==name and s.get("address")==address]
            expected={"size":40,"new_bytes":40,"raw_sha256":spec["raw_sha256"],"relocations":3}
            if len(captures)!=1 or any(captures[0].get(k)!=v for k,v in expected.items()):
                raise SystemExit("frozen C4 initializer window drift")
            # The full C4 comparison artifact never executes unselected code.
            # Its unused external helpers may bind to zero; both dependencies
            # of the selected initializer retain their real nonzero addresses.
            bindings={"Memory":0x34e2b0,"memset":0x19c39c}
            for s in original.symbols:
                if s.name and s.section_index==0:bindings.setdefault(s.name,0)
            refscript=BUILD/"c4-initializer.reference.ld"
            refscript.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n,v in bindings.items())
                +"SECTIONS { .text 0x0010c300 : { *(.text) } .rodata 0x02000000 : { *(.rodata*) } .data 0x03000000 : { *(.data*) } .bss 0x04000000 : { *(.bss*) *(COMMON) } /DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
            reference_path=BUILD/"c4-initializer.reference.elf"
            run([CXX.with_name("ee-ld"),"-EL","-T",refscript,original.path,"-o",reference_path])
            reference=ELFFile(reference_path);ref=reference.find_symbol(name)
            if ref.value!=address or reference.symbol_bytes(ref,size)!=final.symbol_bytes(placed,size):
                raise SystemExit("fully linked historical C4 initializer drift")
        else:
            words={}
            for line in (ROOT/spec["evidence"]).read_text().splitlines():
                match=line_pattern.match(line)
                if match and address<=int(match[1],16)<address+size:
                    pos=int(match[1],16)
                    if pos in words:raise SystemExit("duplicate target instruction")
                    words[pos]=bytes.fromhex(match[2])
            if set(words)!=set(range(address,address+size,4)) or b"".join(words[a] for a in range(address,address+size,4))!=final.symbol_bytes(placed,size):
                raise SystemExit("complete provider-linked native reset target mismatch: "+name)
        print(f"native reset: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream,delimiter="\t"))!=SPECS:raise SystemExit("native reset evidence ledger drift")
    print("Native chip reset source proof: 3/3 functions; 416/416 raw historical bytes; 376/376 public target bytes")

if __name__=="__main__":
    main()
