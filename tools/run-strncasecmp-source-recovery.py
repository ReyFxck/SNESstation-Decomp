#!/usr/bin/env python3
"""Byte-exact historical PS2LIB F_strncasecmp source gate."""
from __future__ import annotations
import hashlib
import re
import shlex
import subprocess
from pathlib import Path
from types import SimpleNamespace
import build_source_tree
import runtime_members as rm

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build/matching/strncasecmp-source-recovery"
SRC=ROOT/"src/ps2/strncasecmp.c"
SHA_SOURCE="54ffd1b2845d412e30c934622bb57c8e85dfe99985e04a93bd634c9c1a696095"
SHA_TARGET="9aa0f9e869ea1d7d5e190b4875cc54b408742f313f550ccc33d0e28ecbd53c1a"
START=1698020
SIZE=184

def run(cmd):
    cp=subprocess.run([str(a) for a in cmd],cwd=ROOT,text=True,
                      stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit("command failed: "+' '.join(map(str,cmd))+"\n"+cp.stdout[-9000:])
    return cp.stdout

def target_bytes():
    mapping={}
    for line in (ROOT/"analysis/functions/libc_text_0019e860.asm").read_text().splitlines():
        m=re.match(r"^\s*([0-9a-fA-F]+):\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\b",line)
        if m:
            base=int(m.group(1),16)
            for i,v in enumerate(m.groups()[1:]):mapping[base+i]=int(v,16)
    missing=[a for a in range(START,START+SIZE) if a not in mapping]
    if missing:raise SystemExit("missing target byte "+hex(missing[0]))
    data=bytes(mapping[a] for a in range(START,START+SIZE))
    digest=hashlib.sha256(data).hexdigest()
    if digest!=SHA_TARGET:raise SystemExit(f"target hash drift {digest}")
    return data

def main():
    BUILD.mkdir(parents=True,exist_ok=True)
    cc=rm.libgcc.resolve_tool(str(ROOT/"build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"))
    machine,version=run([cc,"-dumpmachine"]).strip(),run([cc,"-dumpversion"]).strip()
    if (machine,version)!=("ee","3.2.2"):raise SystemExit("wrong historical compiler")
    args=SimpleNamespace(inputs=(ROOT/"analysis/link_identity/runtime_member_inputs.tsv").resolve(),
                         source_cache=None,build_dir=(BUILD/"runtime-inputs").resolve())
    rm.materialize_inputs(args,cc)
    original=args.build_dir/"inputs"/rm.APR15/"ee/libc/src/string.c"
    if hashlib.sha256(original.read_bytes()).hexdigest()!=SHA_SOURCE:raise SystemExit("upstream source drift")
    marker="#ifdef F_strncasecmp\n"
    hist=original.read_text().split(marker,1)[1].split("#endif",1)[0]
    local=SRC.read_text().split(marker,1)[1].split("#endif",1)[0]
    if hist!=local:raise SystemExit("F_strncasecmp body differs from 2004 source")
    includes=["-I",str(ROOT/"include")]
    for rev,path in rm.INCLUDE_DIRS:includes+=["-I",str(args.build_dir/"inputs"/rev/path)]
    includes+=["-I",run([cc,"-print-file-name=include"]).strip()]
    obj=BUILD/"strncasecmp-source-recovery.o"
    run([cc,*rm.FLAGS,*includes,"-DF_strncasecmp","-c",SRC,"-o",obj])
    target=target_bytes()
    image,masks,norm=rm.libgcc.text_image(obj)
    diff=rm.libgcc.differing_unmasked(target,image,masks)
    exact=len(image)==SIZE and diff==0 and norm==rm.libgcc.normalize_target(target,masks)
    print(f"STRNCASECMP historical compiler={machine} GCC {version}",flush=True)
    print(f"{'MATCH' if exact else 'DIFF':5} object bytes={len(image)}/{SIZE} relocations={len(masks)} diff={diff}",flush=True)
    print("STRNCASECMP unresolved:\n"+run([cc.with_name("ee-nm"),"-u",obj]),flush=True)
    print("STRNCASECMP relocs:\n"+run([cc.with_name("ee-readelf"),"-r",obj]),flush=True)
    if not exact:raise SystemExit("STRNCASECMP historical object gate: FAIL")

    ld=cc.with_name("ee-ld")
    objcopy=cc.with_name("ee-objcopy")
    lds=BUILD/"strncasecmp-source-recovery.link.ld"
    lds.write_text("""PROVIDE(tolower = 0x0019edac);
SECTIONS {
  . = 0x19e8e4;
  .text : { *(.text) }
  /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.rodata*) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) }
}
""")
    def raw_link(path,name):
        elf=BUILD/(name+".elf"); raw=BUILD/(name+".bin")
        run([ld,"-EL","-T",lds,path,"-o",elf])
        run([objcopy,"-j",".text","-O","binary",elf,raw])
        return raw.read_bytes()
    raw=raw_link(obj,"historical")
    if raw!=target:
        first=next((i for i,(a,b) in enumerate(zip(raw,target)) if a!=b),min(len(raw),len(target)))
        raise SystemExit(f"STRNCASECMP historical linked target bytes FAIL at 0x{first:x}")
    print(f"STRNCASECMP historical raw-linked: MATCH {SIZE}/{SIZE} sha256={hashlib.sha256(raw).hexdigest()}",flush=True)

    makeflags=run(["make","--no-print-directory","--silent","--eval",
                   "print-ps2-ee-flags: ; @echo $(EE_SOURCE_TREE_FLAGS)",
                   "print-ps2-ee-flags"]).strip()
    selected=build_source_tree.effective_source_cflags(shlex.split(makeflags),"src/ps2/strncasecmp.c")
    canonical=BUILD/"strncasecmp-source-recovery.canonical.o"
    run([cc,*selected,"-DF_strncasecmp","-c",SRC,"-o",canonical])
    ci,cm,cn=rm.libgcc.text_image(canonical)
    d=rm.libgcc.differing_unmasked(target,ci,cm)
    ok=len(ci)==SIZE and d==0 and cn==rm.libgcc.normalize_target(target,cm)
    print(f"{'MATCH' if ok else 'DIFF':5} canonical bytes={len(ci)}/{SIZE} relocations={len(cm)} diff={d}",flush=True)
    if not ok:raise SystemExit("STRNCASECMP canonical source-tree object gate: FAIL")
    craw=raw_link(canonical,"canonical")
    if craw!=target:raise SystemExit("STRNCASECMP canonical raw-linked target bytes FAIL")
    print(f"STRNCASECMP canonical raw-linked: MATCH {SIZE}/{SIZE} sha256={hashlib.sha256(craw).hexdigest()}",flush=True)

if __name__=="__main__":main()
