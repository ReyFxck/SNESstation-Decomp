#!/usr/bin/env python3
"""Focused proof for the two PS2LIB glue.c DMAC wrappers used by SNES Station."""
from __future__ import annotations
import re, subprocess
from pathlib import Path
from types import SimpleNamespace
import runtime_members as rm

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build"/"matching"/"glue-source-recovery"
LISTING=ROOT/"analysis"/"functions"/"sifcmd_irq_0019fb00.asm"
BASE=0x0019FB00
SPECS=(("EnableDmac.o","F_EnableDmac",0x0019FB00,0x78),
       ("DisableDmac.o","F_DisableDmac",0x0019FB78,0x78))
INSN=re.compile(r"^\s*([0-9A-Fa-f]+):\s+([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)")

def target_bytes(start,end):
    m={}
    for line in LISTING.read_text(errors="replace").splitlines():
        x=INSN.match(line)
        if not x: continue
        a=int(x.group(1),16)
        raw=bytes(int(x.group(i),16) for i in range(2,6))
        for off,b in enumerate(raw): m[a+off]=b
    return bytes(m[a] for a in range(start,end))

def run(cmd):
    p=subprocess.run([str(x) for x in cmd],cwd=ROOT,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if p.returncode: raise SystemExit(p.stdout[-6000:])

def main():
    BUILD.mkdir(parents=True,exist_ok=True)
    cc=rm.libgcc.resolve_tool(str(ROOT/"build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"))
    args=SimpleNamespace(inputs=(ROOT/"analysis/link_identity/runtime_member_inputs.tsv").resolve(),source_cache=None,build_dir=(BUILD/"runtime-inputs").resolve())
    rm.materialize_inputs(args,cc)
    inc=[]
    for rev,rel in rm.INCLUDE_DIRS: inc += ["-I",str(args.build_dir/"inputs"/rev/rel)]
    inc += ["-I",subprocess.check_output([str(cc),"-print-file-name=include"],text=True).strip()]
    historical=ROOT/"third_party/historical_refs/ps2lib-kernel-lineage/glue.c"
    source=ROOT/"src/ps2/glue.c"
    use_local=source.is_file()
    if not use_local: source=historical
    print("source:", "src/ps2/glue.c" if use_local else f"ps2dev/ps2sdk@{rm.APR18} ee/kernel/src/glue.c")
    failed=False
    for name,define,address,size in SPECS:
        obj=BUILD/name
        cmd=[cc,*rm.FLAGS]
        if use_local: cmd += ["-I",str(ROOT/"include")]
        else: cmd += inc
        cmd += ["-D"+define,"-c",source,"-o",obj]
        run(cmd)
        image,masks,normalized=rm.libgcc.text_image(obj)
        expected=target_bytes(address,address+size)
        diff=rm.libgcc.differing_unmasked(expected,image,masks)
        ok=len(image)==size and diff==0 and rm.libgcc.normalize_target(expected,masks)==normalized
        print(f"{'MATCH' if ok else 'DIFF':5} {name:16} bytes={len(image)}/{size} relocs={len(masks)} diff={diff}")
        failed |= not ok
    if failed: raise SystemExit("GLUE historical-source gate: FAIL")
    print("GLUE historical-source gate: 2/2 MATCH")

if __name__=="__main__": main()
