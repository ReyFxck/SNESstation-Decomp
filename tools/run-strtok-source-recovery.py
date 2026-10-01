#!/usr/bin/env python3
"""Byte-exact historical PS2LIB F_strtok source gate."""
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
BUILD=ROOT/"build/matching/strtok-source-recovery"
SRC=ROOT/"src/ps2/strtok.c"
SHA_SOURCE="54ffd1b2845d412e30c934622bb57c8e85dfe99985e04a93bd634c9c1a696095"
SHA_TARGET="085af3f51a19050c39d270bf41f7cd5d30d5a9d4191d82785ad3d6116bb6d4bf"
START=0x0019e99c
SIZE=0x108

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
    marker="#ifdef F_strtok\n"
    hist=original.read_text().split(marker,1)[1].split("#endif",1)[0]
    local=SRC.read_text().split(marker,1)[1].split("#endif",1)[0]
    if hist!=local:raise SystemExit("F_strtok body differs from 2004 source")
    includes=["-I",str(ROOT/"include")]
    for rev,path in rm.INCLUDE_DIRS:includes+=["-I",str(args.build_dir/"inputs"/rev/path)]
    includes+=["-I",run([cc,"-print-file-name=include"]).strip()]
    obj=BUILD/"strtok.o"
    run([cc,*rm.FLAGS,*includes,"-DF_strtok","-c",SRC,"-o",obj])
    target=target_bytes()
    image,masks,norm=rm.libgcc.text_image(obj)
    diff=rm.libgcc.differing_unmasked(target,image,masks)
    object_ok=len(image)==SIZE and len(masks)==27 and diff==0 and norm==rm.libgcc.normalize_target(target,masks)
    print(f"STRTOK historical compiler={machine} GCC {version}",flush=True)
    print(f"{'MATCH' if object_ok else 'DIFF':5} strtok.o bytes={len(image)}/{SIZE} relocations={len(masks)} diff={diff}",flush=True)
    print("STRTOK unresolved:\n"+run([cc.with_name("ee-nm"),"-u",obj]),flush=True)
    print("STRTOK symbols:\n"+run([cc.with_name("ee-nm"),"-n",obj]),flush=True)
    print("STRTOK relocs:\n"+run([cc.with_name("ee-readelf"),"-r",obj]),flush=True)
    if not object_ok:raise SystemExit("STRTOK historical object gate: FAIL")

    ld=cc.with_name("ee-ld")
    objcopy=cc.with_name("ee-objcopy")
    lds=BUILD/"strtok.link.ld"
    lds.write_text("""PROVIDE(strchr = 0x0019c610);
SECTIONS {
  . = 0x0019e99c;
  .text : { *(.text) }
  .bss 0x00445140 (NOLOAD) : { *(.bss) *(COMMON) }
  /DISCARD/ : { *(.data) *(.rodata*) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) }
}
""")
    def raw_link(path,name):
        elf=BUILD/(name+".elf"); raw=BUILD/(name+".bin")
        run([ld,"-EL","-T",lds,path,"-o",elf])
        run([objcopy,"-j",".text","-O","binary",elf,raw])
        return raw.read_bytes()
    raw=raw_link(obj,"historical-strtok")
    print(f"STRTOK historical raw-linked: {'MATCH' if raw==target else 'DIFF'} {len(raw)}/{SIZE} sha256={hashlib.sha256(raw).hexdigest()}",flush=True)
    if raw!=target:
        first=next((i for i,(a,b) in enumerate(zip(raw,target)) if a!=b),min(len(raw),len(target)))
        raise SystemExit(f"STRTOK historical linked target bytes FAIL at 0x{first:x}")

    makeflags=run(["make","--no-print-directory","--silent","--eval",
                   "print-ps2-ee-flags: ; @echo $(EE_SOURCE_TREE_FLAGS)",
                   "print-ps2-ee-flags"]).strip()
    selected=build_source_tree.effective_source_cflags(
        shlex.split(makeflags),"src/ps2/strtok.c"
    )
    canonical=BUILD/"strtok.canonical.o"
    run([cc,*selected,"-DF_strtok","-c",SRC,"-o",canonical])
    ci,cm,cn=rm.libgcc.text_image(canonical)
    d=rm.libgcc.differing_unmasked(target,ci,cm)
    exact=len(ci)==SIZE and len(cm)==27 and d==0 and cn==rm.libgcc.normalize_target(target,cm)
    print(f"{'MATCH' if exact else 'DIFF':5} canonical strtok.o bytes={len(ci)}/{SIZE} relocations={len(cm)} diff={d}",flush=True)
    if not exact:raise SystemExit("STRTOK canonical source-tree object gate: FAIL")
    canonical_raw=raw_link(canonical,"canonical-strtok")
    if canonical_raw!=target:raise SystemExit("STRTOK canonical raw-linked target bytes FAIL")
    print(f"STRTOK canonical raw-linked: MATCH {SIZE}/{SIZE} sha256={hashlib.sha256(canonical_raw).hexdigest()}",flush=True)

if __name__=="__main__":main()
