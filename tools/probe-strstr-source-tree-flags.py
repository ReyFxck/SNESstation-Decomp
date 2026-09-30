#!/usr/bin/env python3
"""Diagnostic: identify which source-tree C flags diverge from exact F_strstr."""
from pathlib import Path
import hashlib
import re
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
    raw_map = {}
    listing = (ROOT / "analysis/functions/libc_text_0019e860.asm").read_text()
    for line in listing.splitlines():
        match = re.match(r"^\s*([0-9a-fA-F]+):\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\b", line)
        if match:
            address = int(match.group(1), 16)
            for i, value in enumerate(match.groups()[1:]):
                raw_map[address + i] = int(value, 16)
    target = bytes(raw_map[0x19eaf8 + index] for index in range(136))
    diff = rm.libgcc.differing_unmasked(target, image, masks)
    output = f"PROBE {name:<32} text={len(image)} relocations={len(masks)} diff={diff}"
    if len(image) == 136 and len(masks) == 2 and diff == 0:
        linker = cc.with_name("ee-ld")
        objcopy = cc.with_name("ee-objcopy")
        ldfile = WORK / (name + ".ld")
        linked = WORK / (name + ".linked.elf")
        rawfile = WORK / (name + ".linked.bin")
        ldfile.write_text("""PROVIDE(strlen = 0x0019c5e8);
PROVIDE(strncmp = 0x0019c410);
SECTIONS { . = 0x0019eaf8; .text : { *(.text) } /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.rodata*) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) } }
""")
        ld = subprocess.run([str(linker), "-EL", "-T", str(ldfile), str(obj), "-o", str(linked)], capture_output=True, text=True)
        if ld.returncode:
            output += " ld_error=" + ld.stderr[-350:]
        else:
            conv = subprocess.run([str(objcopy), "-j", ".text", "-O", "binary", str(linked), str(rawfile)], capture_output=True, text=True)
            if conv.returncode:
                output += " objcopy_error=" + conv.stderr[-350:]
            else:
                linked_raw = rawfile.read_bytes()
                output += f" raw_equal={linked_raw == target} linked_sha={hashlib.sha256(linked_raw).hexdigest()}"
    print(output, flush=True)

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
