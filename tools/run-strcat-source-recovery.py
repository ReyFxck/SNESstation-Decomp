#!/usr/bin/env python3
from __future__ import annotations
import csv, hashlib, subprocess
from pathlib import Path
import runtime_members as rm
ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build"/"matching"/"strcat-source-recovery"
SOURCE=ROOT/"src"/"ps2"/"strcat.S"
TARGET=ROOT/"analysis"/"functions"/"strcat_exact_56.tsv"
SOURCE_SHA="6d2c8474d964f2a50f5071f27f027cd5ceb3b75e5417ead1aeb674f79acf5587"
TARGET_SHA="bed4556f3f1a6e88dd681b2e8c1ac8d051958157051401dcb8377deef9502237"

def run(cmd):
    cp=subprocess.run([str(x) for x in cmd],cwd=ROOT,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if cp.returncode: raise SystemExit(cp.stdout[-8000:])

def main():
    BUILD.mkdir(parents=True,exist_ok=True)
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest()!=SOURCE_SHA: raise SystemExit("strcat.S source hash drift")
    with TARGET.open(encoding="utf-8",newline="") as f: row=list(csv.DictReader(f,delimiter="\t"))[0]
    expected=bytes.fromhex(row["raw_hex"])
    if len(expected)!=56 or hashlib.sha256(expected).hexdigest()!=TARGET_SHA: raise SystemExit("target drift")
    cc=rm.libgcc.resolve_tool(str(ROOT/"build"/"toolchains"/"ee-gcc-3.2.2-stage1"/"prefix"/"bin"/"ee-gcc"))
    if subprocess.check_output([str(cc),"-dumpversion"],text=True).strip()!="3.2.2": raise SystemExit("compiler drift")
    obj=BUILD/"strcat.o"
    run([cc,*rm.FLAGS,"-c",SOURCE,"-o",obj])
    image,masks,normalized=rm.libgcc.text_image(obj)
    diff=rm.libgcc.differing_unmasked(expected,image,masks)
    if len(image)!=56 or len(masks)!=1 or diff!=0 or rm.libgcc.normalize_target(expected,masks)!=normalized:
        raise SystemExit("STRCAT object gate: FAIL")
    print(f"MATCH strcat.o target=56 text={len(image)} relocs={len(masks)} diff={diff}")
    ld=cc.with_name("ee-ld"); objcopy=cc.with_name("ee-objcopy")
    script=BUILD/"strcat.target.ld"; linked=BUILD/"strcat.linked.elf"; raw=BUILD/"strcat.text.bin"
    script.write_text("""PROVIDE(strcpy = 0x0019c528);
SECTIONS {
  . = 0x0019c3d4;
  .text : { *(.text) }
  /DISCARD/ : { *(.data) *(.bss) *(.rodata*) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) }
}
""",encoding="utf-8")
    run([ld,"-T",script,obj,"-o",linked])
    run([objcopy,"-j",".text","-O","binary",linked,raw])
    linked_raw=raw.read_bytes()
    if linked_raw!=expected: raise SystemExit("STRCAT raw linked gate: FAIL")
    print("STRCAT historical-source object gate: 1/1 MATCH")
    print("STRCAT raw linked gate: MATCH 56/56 bytes")
    print("raw sha256:",hashlib.sha256(linked_raw).hexdigest())
if __name__=="__main__": main()
