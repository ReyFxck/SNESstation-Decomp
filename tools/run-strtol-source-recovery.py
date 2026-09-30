#!/usr/bin/env python3
"""Prove pinned 2004 PS2LIB F_strtol against original EE executable bytes."""
from __future__ import annotations
import hashlib
import re
import subprocess
from pathlib import Path
import shlex
from types import SimpleNamespace
import runtime_members as rm

ROOT=Path(__file__).resolve().parents[1]
BUILD=ROOT/"build/matching/strtol-source-recovery"
SRC=ROOT/"src/ps2/strtol.c"
SHA_SOURCE="54ffd1b2845d412e30c934622bb57c8e85dfe99985e04a93bd634c9c1a696095"
SHA_TARGET="30408be72a59b145efb678fc251ddf9faa0d8c3e396ab1a5a8752dc60f7a43b7"
START=0x0019eb80
SIZE=0x22c

def run(cmd):
    cp=subprocess.run([str(p) for p in cmd],cwd=ROOT, text=True,
                      stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    if cp.returncode: raise SystemExit("command failed: "+' '.join(map(str,cmd))+"\n"+cp.stdout[-7000:])
    return cp.stdout

def target_bytes():
    seen={}
    for line in (ROOT/"analysis/functions/libc_text_0019e860.asm").read_text().splitlines():
        m=re.match(r"^\s*([0-9a-fA-F]+):\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\b",line)
        if m:
            pos=int(m.group(1),16)
            for i,v in enumerate(m.groups()[1:]):seen[pos+i]=int(v,16)
    missing=[v for v in range(START,START+SIZE) if v not in seen]
    if missing:raise SystemExit("missing target byte "+hex(missing[0]))
    result=bytes(seen[v] for v in range(START,START+SIZE))
    if hashlib.sha256(result).hexdigest()!=SHA_TARGET: raise SystemExit("target bytes hash drift")
    return result

def main():
    BUILD.mkdir(parents=True,exist_ok=True)
    compiler=rm.libgcc.resolve_tool(str(ROOT/"build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"))
    machine,version=run([compiler,"-dumpmachine"]).strip(),run([compiler,"-dumpversion"]).strip()
    if (machine,version)!=("ee","3.2.2"):raise SystemExit(f"unsupported toolchain {machine} {version}")
    args=SimpleNamespace(inputs=(ROOT/"analysis/link_identity/runtime_member_inputs.tsv").resolve(),
                         source_cache=None,build_dir=(BUILD/"runtime-inputs").resolve())
    rm.materialize_inputs(args,compiler)
    historic=args.build_dir/"inputs"/rm.APR15/"ee/libc/src/string.c"
    if hashlib.sha256(historic.read_bytes()).hexdigest()!=SHA_SOURCE:raise SystemExit("historical source hash drift")
    marker="#ifdef F_strtol\n"
    hist=historic.read_text().split(marker,1)[1].split("#endif",1)[0]
    local=SRC.read_text().split(marker,1)[1].split("#endif",1)[0]
    if hist!=local:raise SystemExit("F_strtol body is not byte-identical to original source")
    includes=["-I",str(ROOT/"include")]
    for rev,path in rm.INCLUDE_DIRS:includes+=["-I",str(args.build_dir/"inputs"/rev/path)]
    includes+=["-I",run([compiler,"-print-file-name=include"]).strip()]
    obj=BUILD/"strtol.o"
    run([compiler,*rm.FLAGS,*includes,"-DF_strtol","-c",SRC,"-o",obj])
    image,masks,normalized=rm.libgcc.text_image(obj)
    target=target_bytes()
    diff=rm.libgcc.differing_unmasked(target,image,masks)
    ok=len(image)==SIZE and len(masks)==9 and diff==0 and normalized==rm.libgcc.normalize_target(target,masks)
    print(f"STRTOL original compiler: {machine} GCC {version}",flush=True)
    print(f"{'MATCH' if ok else 'DIFF':5} strtol.o bytes={len(image)}/{SIZE} relocations={len(masks)} diff={diff}",flush=True)
    print("STRTOL unresolved symbols:\n"+run([compiler.with_name("ee-nm"),"-u",obj]),flush=True)
    print("STRTOL relocation table:\n"+run([compiler.with_name("ee-readelf"),"-r",obj]),flush=True)
    if not ok:raise SystemExit("STRTOL historical object gate: FAIL")
    print("STRTOL historical source object gate: MATCH 556/556 bytes, 9 relocations",flush=True)

    ld=compiler.with_name("ee-ld")
    objcopy=compiler.with_name("ee-objcopy")
    ldscript=BUILD/"strtol.link.ld"
    ldscript.write_text(
        """PROVIDE(isspace = 0x0019efac);
PROVIDE(__umoddi3 = 0x001a2c78);
PROVIDE(__udivdi3 = 0x001a25b0);
PROVIDE(isdigit = 0x0019ee80);
PROVIDE(__muldi3 = 0x001a1b20);
PROVIDE(ps2lib_errno_00425a70 = 0x00425a70);
PROVIDE(isalpha = 0x0019ee34);
PROVIDE(isupper = 0x0019ee0c);
SECTIONS
{
  . = 0x0019eb80;
  .text : { *(.text) }
  /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.rodata*) *(.reginfo) *(.pdr)
                *(.mdebug*) *(.comment) *(.note*) }
}
""",
        encoding="utf-8",
    )
    def raw_link(obj, name):
        linked=BUILD/(name+".linked.elf")
        binfile=BUILD/(name+".raw")
        run([ld,"-EL","-T",ldscript,obj,"-o",linked])
        run([objcopy,"-j",".text","-O","binary",linked,binfile])
        return binfile.read_bytes()
    raw=raw_link(obj,"strtol-historical")
    print("STRTOL original raw-linked: "+("MATCH" if raw==target else "DIFF")+
          f" {len(raw)}/{len(target)} bytes; sha256={hashlib.sha256(raw).hexdigest()}",flush=True)
    if raw!=target:
        first=next((i for i,(a,b) in enumerate(zip(raw,target)) if a!=b),min(len(raw),len(target)))
        raise SystemExit(f"STRTOL linked historical object FAIL at offset 0x{first:x}")

    # Test the exact Makefile compiler profile plus potential historical
    # per-TU flag overrides. This is diagnostic until the selected profile
    # is proven and fixed into the canonical translation-unit ledger.
    cflags_txt=run(["make","--no-print-directory","--silent","--eval",
                    "print-ps2-ee-flags: ; @echo $(EE_SOURCE_TREE_FLAGS)",
                    "print-ps2-ee-flags"]).strip()
    appflags=shlex.split(cflags_txt)
    variants={
        "current-app-Os": ["-Os"],
        "hosted": ["-Os","-fhosted"],
        "builtin": ["-Os","-fbuiltin"],
        "hosted-builtin": ["-Os","-fhosted","-fbuiltin"],
        "long32-hosted": ["-Os","-mlong32","-fhosted"],
        "long32-only": ["-Os","-mlong32"],
        "no-freestanding-or-builtin": ["-Os","-fhosted","-fbuiltin","-mlong64"],
    }
    for name,flags in variants.items():
        preview=BUILD/("strtol-"+name+".o")
        run([compiler,*appflags,*flags,"-DF_strtol","-c",SRC,"-o",preview])
        candidate,m,candidate_norm=rm.libgcc.text_image(preview)
        diff=rm.libgcc.differing_unmasked(target,candidate,m)
        success=(len(candidate)==SIZE and len(m)==9 and diff==0 and
                 candidate_norm==rm.libgcc.normalize_target(target,m))
        suffix=""
        if success:
            linked_raw=raw_link(preview,"strtol-"+name)
            suffix=f" raw_equal={linked_raw==target}"
        print(f"STRTOL PROFILE {name}: text={len(candidate)} "
              f"relocs={len(m)} diff={diff} object_equal={success}{suffix}",flush=True)

if __name__=="__main__":main()
