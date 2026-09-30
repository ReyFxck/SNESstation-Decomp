#!/usr/bin/env python3
"""Diagnostic: identify which source-tree C flags diverge from exact F_strstr."""
from pathlib import Path
import subprocess

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
WORK = ROOT / "build/matching/strstr-source-tree-flag-probe"
SOURCE = ROOT / "src/ps2/strstr.c"

def trial(cc, name, flags):
    obj = WORK / (name + ".o")
    cp = subprocess.run(
        [str(cc), *flags, "-DF_strstr", "-c", str(SOURCE), "-o", str(obj)],
        cwd=ROOT, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT
    )
    if cp.returncode:
        print(f"PROBE {name:<32} COMPILER ERROR {cp.stdout[-700:]}", flush=True)
        return
    image, masks, normalized = rm.libgcc.text_image(obj)
    print(f"PROBE {name:<32} text={len(image)} relocations={len(masks)}", flush=True)

def main():
    WORK.mkdir(parents=True, exist_ok=True)
    cc = rm.libgcc.resolve_tool(str(
        ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
    ))
    # Actual Makefile EE_SOURCE_TREE_FLAGS + local SOURCE_FLAGS -Os.
    base = [
        "-G0", "-O2", "-EL", "-pipe", "-Wall",
        "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common",
        "-ffreestanding", "-fno-builtin", "-fshort-double",
        "-mlong64", "-mhard-float", "-mno-abicalls",
        "-march=r5900", "-mtune=r5900",
        "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD",
        "-DCODE_PLATFORM=3", "-Iinclude", "-Iinclude/ee_stage1_compat",
        "-w", "-Os",
    ]
    def without(*items):
        return [flag for flag in base if flag not in items]
    tests = [
        ("source-tree-current", base),
        ("without-freestanding", without("-ffreestanding")),
        ("without-fno-builtin", without("-fno-builtin")),
        ("without-both", without("-ffreestanding", "-fno-builtin")),
        ("without-march-mtune", without("-march=r5900", "-mtune=r5900")),
        ("without-mlong64", without("-mlong64")),
        ("without-mhard-float", without("-mhard-float")),
        ("without-mno-abicalls", without("-mno-abicalls")),
        ("without-fshort-double", without("-fshort-double")),
        ("without-frame-omit", without("-fomit-frame-pointer")),
        ("without-strict-aliasing", without("-fstrict-aliasing")),
        ("without-fno-common", without("-fno-common")),
        ("no-align-jumps", base + ["-fno-align-jumps"]),
        ("no-align-functions", base + ["-fno-align-functions"]),
        ("no-align-labels", base + ["-fno-align-labels"]),
        ("no-align-loops", base + ["-fno-align-loops"]),
        ("no-align-all", base + [
            "-fno-align-jumps", "-fno-align-functions",
            "-fno-align-labels", "-fno-align-loops",
        ]),
        ("hosted", base + ["-fhosted"]),
        ("builtin", base + ["-fbuiltin"]),
        ("hosted-builtin", base + ["-fhosted", "-fbuiltin"]),
        ("no-freestanding-with-builtin", without("-ffreestanding", "-fno-builtin")+["-fbuiltin"]),
        ("no-optimization-first", [f for f in base if f != "-O2"]),
        ("old-ps2lib-compiler-flags", [
            *rm.FLAGS, "-Iinclude", "-Iinclude/ee_stage1_compat",
        ]),
    ]
    tests.extend([
        ("no-mlong64-hosted", without("-mlong64") + ["-fhosted"]),
        ("no-mlong64-builtin", without("-mlong64") + ["-fbuiltin"]),
        ("no-mlong64-no-both", without("-mlong64", "-ffreestanding", "-fno-builtin")),
        ("no-mlong64-no-both-no-r5900", without("-mlong64", "-ffreestanding", "-fno-builtin", "-march=r5900", "-mtune=r5900")),
        ("force-mno-long64", base + ["-mno-long64"]),
        ("force-mno-long64-hosted", base + ["-mno-long64", "-fhosted"]),
        ("force-mno-long64-builtin", base + ["-mno-long64", "-fbuiltin"]),
        ("force-mno-long64-both", base + ["-mno-long64", "-fhosted", "-fbuiltin"]),
        ("force-mlong32-hosted", base + ["-mlong32", "-fhosted"]),
        ("no-march-no-long64-hosted", without("-march=r5900", "-mtune=r5900", "-mlong64")+["-fhosted"]),
        ("no-long64-both-no-common", without("-mlong64", "-ffreestanding", "-fno-builtin", "-fno-common")),
        ("nostdinc-historical-options", [*rm.FLAGS, "-Iinclude", "-Iinclude/ee_stage1_compat",
            "-I", str(Path(subprocess.check_output([str(cc), "-print-file-name=include"], text=True).strip()).resolve())]),
    ])
    for name, flags in tests:
        trial(cc, name, flags)

if __name__ == "__main__":
    main()
