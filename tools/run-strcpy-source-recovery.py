#!/usr/bin/env python3
"""Byte-exact historical-source gate for SNES Station's PS2LIB strcpy.S."""
from __future__ import annotations
import csv, hashlib, subprocess
from pathlib import Path
import runtime_members as rm

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build"/"matching"/"strcpy-source-recovery"
SOURCE=ROOT/"src"/"ps2"/"strcpy.S"
TARGET=ROOT/"analysis"/"functions"/"strcpy_exact_40.tsv"
SOURCE_SHA="a55bce9e11a42281ddd71f909b49cd3df8dfd7bbc8b11228caf215c4255e0c97"
TARGET_SHA="9b08d4ac7a7168b130b955f72b652b52df632cb88b835cb91e4b2f429dc127a7"

def run(cmd):
    cp=subprocess.run([str(x) for x in cmd],cwd=ROOT,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit(cp.stdout[-8000:])

def main():
    BUILD.mkdir(parents=True,exist_ok=True)
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest()!=SOURCE_SHA:
        raise SystemExit("strcpy.S source hash drift")
    with TARGET.open(encoding="utf-8",newline="") as f:
        rows=list(csv.DictReader(f,delimiter="\t"))
    if len(rows)!=1 or rows[0]["symbol"]!="strcpy":
        raise SystemExit("expected one strcpy target row")
    row=rows[0]
    expected=bytes.fromhex(row["raw_hex"])
    if int(row["address"],0)!=0x0019C528 or int(row["size_hex"],0)!=0x28:
        raise SystemExit("strcpy target geometry drift")
    if len(expected)!=40 or hashlib.sha256(expected).hexdigest()!=TARGET_SHA:
        raise SystemExit("strcpy target drift")
    cc=rm.libgcc.resolve_tool(str(ROOT/"build"/"toolchains"/"ee-gcc-3.2.2-stage1"/"prefix"/"bin"/"ee-gcc"))
    version=subprocess.check_output([str(cc),"-dumpversion"],text=True).strip()
    machine=subprocess.check_output([str(cc),"-dumpmachine"],text=True).strip()
    if (version,machine)!=("3.2.2","ee"):
        raise SystemExit(f"unexpected compiler: {machine} gcc {version}")
    obj=BUILD/"strcpy.o"
    run([cc,*rm.FLAGS,"-c",SOURCE,"-o",obj])
    image,masks,normalized=rm.libgcc.text_image(obj)
    diff=rm.libgcc.differing_unmasked(expected,image,masks)
    ok=(len(image)==40 and not masks and diff==0 and image==expected and normalized==expected
        and hashlib.sha256(image).hexdigest()==TARGET_SHA)
    print(f"{'MATCH' if ok else 'DIFF':5} strcpy.o target=40 text={len(image)} relocs={len(masks)} diff={diff}")
    if not ok:
        raise SystemExit("STRCPY historical-source object gate: FAIL")
    print("STRCPY historical-source object gate: 1/1 MATCH")
    print("STRCPY raw target gate: MATCH 40/40 bytes")
    print("raw sha256:",hashlib.sha256(image).hexdigest())

if __name__=="__main__":
    main()
