#!/usr/bin/env python3
"""Byte-exact historical PS2LIB F_strcasecmp source gate (2004 SNES Station)."""
from __future__ import annotations
import hashlib
import re
import shlex
import subprocess
from pathlib import Path
from types import SimpleNamespace
import runtime_members as rm

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build/matching/strcasecmp-source-recovery"
SRC=ROOT/"src/ps2/strcasecmp.c"
SHA_SOURCE="54ffd1b2845d412e30c934622bb57c8e85dfe99985e04a93bd634c9c1a696095"
SHA_TARGET="4cfb2642d2d0ac04836cc2f559d00afdca78654de5f3f541408e12fcda22dbd1"
START=0x0019e860
SIZE=0x84

def run(cmd):
    cp=subprocess.run([str(arg) for arg in cmd],cwd=ROOT,text=True,
                      stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if cp.returncode:raise SystemExit("command failed: "+' '.join(map(str,cmd))+"\n"+cp.stdout[-8000:])
    return cp.stdout

def target_bytes():
    mapping={}
    for line in (ROOT/"analysis/functions/libc_text_0019e860.asm").read_text().splitlines():
        m=re.match(r"^\s*([0-9a-fA-F]+):\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\b",line)
        if m:
            base=int(m.group(1),16)
            for i,item in enumerate(m.groups()[1:]):mapping[base+i]=int(item,16)
    missing=[addr for addr in range(START,START+SIZE) if addr not in mapping]
    if missing:raise SystemExit("missing original target byte "+hex(missing[0]))
    expected=bytes(mapping[a] for a in range(START,START+SIZE))
    if hashlib.sha256(expected).hexdigest()!=SHA_TARGET:raise SystemExit("target source listing SHA-256 drift: expected "+SHA_TARGET+" got "+hashlib.sha256(expected).hexdigest())
    return expected

def main():
    BUILD.mkdir(parents=True,exist_ok=True)
    compiler=rm.libgcc.resolve_tool(str(ROOT/"build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"))
    machine,version=run([compiler,"-dumpmachine"]).strip(),run([compiler,"-dumpversion"]).strip()
    if (machine,version)!=("ee","3.2.2"):raise SystemExit("wrong historical compiler")
    args=SimpleNamespace(inputs=(ROOT/"analysis/link_identity/runtime_member_inputs.tsv").resolve(),
                         source_cache=None,build_dir=(BUILD/"runtime-inputs").resolve())
    rm.materialize_inputs(args,compiler)
    upstream=args.build_dir/"inputs"/rm.APR15/"ee/libc/src/string.c"
    if hashlib.sha256(upstream.read_bytes()).hexdigest()!=SHA_SOURCE:raise SystemExit("upstream source drift")
    marker="#ifdef F_strcasecmp\n"
    historical=upstream.read_text().split(marker,1)[1].split("#endif",1)[0]
    restored=SRC.read_text().split(marker,1)[1].split("#endif",1)[0]
    if historical!=restored:raise SystemExit("F_strcasecmp body differs from 2004 source")
    includes=["-I",str(ROOT/"include")]
    for rev,path in rm.INCLUDE_DIRS:includes+=["-I",str(args.build_dir/"inputs"/rev/path)]
    includes+=["-I",run([compiler,"-print-file-name=include"]).strip()]
    obj=BUILD/"strcasecmp.o"
    run([compiler,*rm.FLAGS,*includes,"-DF_strcasecmp","-c",SRC,"-o",obj])
    expected=target_bytes()
    image,masks,norm=rm.libgcc.text_image(obj)
    diff=rm.libgcc.differing_unmasked(expected,image,masks)
    ok=len(image)==SIZE and len(masks)==4 and diff==0 and norm==rm.libgcc.normalize_target(expected,masks)
    print(f"STRCASECMP historical compiler={machine} GCC {version}",flush=True)
    print(f"{'MATCH' if ok else 'DIFF':5} strcasecmp.o bytes={len(image)}/{SIZE} relocations={len(masks)} diff={diff}",flush=True)
    print("STRCASECMP unresolved:\n"+run([compiler.with_name("ee-nm"),"-u",obj]),flush=True)
    print("STRCASECMP relocs:\n"+run([compiler.with_name("ee-readelf"),"-r",obj]),flush=True)
    if not ok:raise SystemExit("STRCASECMP historical object gate: FAIL")
    ld=compiler.with_name("ee-ld")
    objcopy=compiler.with_name("ee-objcopy")
    linker_script=BUILD/"strcasecmp.link.ld"
    linker_script.write_text("""PROVIDE(tolower = 0x0019edac);
SECTIONS { . = 0x0019e860; .text : { *(.text) }
  /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.rodata*) *(.reginfo) *(.pdr)
                *(.mdebug*) *(.comment) *(.note*) } }
""")
    def link_raw(object_path,name):
        linked=BUILD/(name+".elf")
        raw=BUILD/(name+".text.bin")
        run([ld,"-EL","-T",linker_script,object_path,"-o",linked])
        run([objcopy,"-j",".text","-O","binary",linked,raw])
        return raw.read_bytes()
    historical_raw=link_raw(obj,"historical-strcasecmp")
    if historical_raw!=expected:raise SystemExit("STRCASECMP historical linked target bytes FAIL")
    print(f"STRCASECMP historical raw-linked: MATCH {SIZE}/{SIZE} bytes sha256={hashlib.sha256(historical_raw).hexdigest()}",flush=True)

    # Before source promotion, probe the application's real Makefile profile.
    makeflags=run(["make","--no-print-directory","--silent","--eval",
                   "print-ps2-ee-flags: ; @echo $(EE_SOURCE_TREE_FLAGS)",
                   "print-ps2-ee-flags"]).strip()
    for name,opts in (
        ("app-Os", ["-Os"]),
        ("app-hosted",["-Os","-fhosted"]),
        ("app-no-long64",["-Os","-mlong32"]),
        ("app-no-long64-hosted",["-Os","-mlong32","-fhosted"]),
    ):
        obj2=BUILD/("strcasecmp-"+name+".o")
        run([compiler,*shlex.split(makeflags),*opts,"-DF_strcasecmp","-c",SRC,"-o",obj2])
        preview,m,pnorm=rm.libgcc.text_image(obj2)
        diffs=rm.libgcc.differing_unmasked(expected,preview,m)
        exact=len(preview)==SIZE and len(m)==4 and diffs==0 and pnorm==rm.libgcc.normalize_target(expected,m)
        raw_equal=link_raw(obj2,name)==expected if exact else False
        print(f"STRCASECMP PROFILE {name}: bytes={len(preview)} relocs={len(m)} diff={diffs} exact={exact} raw_equal={raw_equal}",flush=True)

if __name__=="__main__":main()
