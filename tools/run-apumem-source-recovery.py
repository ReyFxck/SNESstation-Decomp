#!/usr/bin/env python3
"""Prove all four historical APU memory bodies with exact provider linkage."""
from __future__ import annotations

import csv
import hashlib
import json
import re
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "src/snes9x/apumem.cpp"
BUILD = ROOT / "build/matching/apumem-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
LISTING = ROOT / "analysis/functions/core_getset_apumem_001ab3c0.asm"
WINDOWS = ROOT / "analysis/link_identity/code_windows.json"
EVIDENCE = ROOT / "analysis/functions/apumem_exact_880.tsv"
SPECS = (
    (0x001AC994, "_Z14S9xAPUGetByteZh", 188, 7,
     "10320a3420eb43c288237bda094cfeb9fded338d775db46009ff75d3766cbc38"),
    (0x001ACA50, "_Z14S9xAPUSetByteZhh", 236, 12,
     "b30fc9b19b510c24f0e2c225f42d5af4fb688e818be74589852f72c895fa1291"),
    (0x001ACB3C, "_Z13S9xAPUGetBytej", 180, 9,
     "14810facfef7851c5081fc6248992d211e83bcd4ba4333b578feb3f9e402b0d2"),
    (0x001ACBF0, "_Z13S9xAPUSetBytehj", 276, 15,
     "41633a262af982ce4eaeb76ec415c1c51aad90f388e445bb1cc12d1e577ba728"),
)
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
    "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
    "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)
LINE = re.compile(r"^\s*([0-9a-fA-F]+):\s+"
                  r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+"
                  r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s")


def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command))
                         + "\n" + result.stdout[-12000:])
    return result.stdout


def target_words():
    words = {}
    for line in LISTING.read_text(encoding="utf-8").splitlines():
        match = LINE.match(line)
        if match:
            address = int(match[1], 16)
            if address in words:
                raise SystemExit(f"duplicate target instruction: {address:#x}")
            words[address] = bytes(int(match[i], 16) for i in range(2, 6))
    return words


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical C++ compiler")
    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "apumem.o"
    run([CXX, *FLAGS, "-c", SOURCE, "-o", obj])
    elf = ELFFile(obj)
    selected = json.loads(WINDOWS.read_text())["result"]["selected_sources"]
    words = target_words()
    with EVIDENCE.open(encoding="utf-8", newline="") as stream:
        evidence_rows = list(csv.DictReader(stream, delimiter="\t"))
    if len(evidence_rows) != len(SPECS):
        raise SystemExit("APUMEM evidence must contain exactly four functions")
    targets = {}
    for address, name, size, relocations, raw_sha in SPECS:
        evidence = [s for s in selected if s.get("address") == address and s.get("symbol") == name]
        if len(evidence) != 1 or any(evidence[0].get(k) != v for k, v in
                                   (("size", size), ("new_bytes", size),
                                    ("relocations", relocations), ("raw_sha256", raw_sha))):
            raise SystemExit(f"frozen historical evidence drift: {name}")
        symbol = elf.find_symbol(name)
        raw = elf.symbol_bytes(symbol, symbol.size)
        if symbol.size != size or len(elf.relocation_ranges(symbol, size)) != relocations:
            raise SystemExit(f"size/relocation drift: {name}")
        if hashlib.sha256(raw).hexdigest() != raw_sha:
            raise SystemExit(f"historical object byte drift: {name}")
        try:
            targets[address] = b"".join(words[a] for a in range(address, address + size, 4))
        except KeyError as missing:
            raise SystemExit(f"missing target instruction: {missing}")

    script = BUILD / "apumem.target.ld"
    script.write_text("""PROVIDE(g_apu_state_00345498 = 0x00345498);
PROVIDE(g_APU_003453b8 = 0x003453b8);
PROVIDE(snes_p24_0010b7f8 = 0x0010b7f8);
PROVIDE(snes_p28_0010ad48 = 0x0010ad48);
PROVIDE(snes_p28_0010b564 = 0x0010b564);
SECTIONS {
  . = 0x001ac994;
  .text : { *(.text) }
  /DISCARD/ : {
    *(.data) *(.bss) *(COMMON) *(.reginfo) *(.pdr)
    *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) *(.rodata*)
  }
}
""", encoding="utf-8")
    linked = BUILD / "apumem.target.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked])
    final = ELFFile(linked)
    report = []
    for address, name, size, relocations, raw_sha in SPECS:
        symbol = final.find_symbol(name)
        actual = final.symbol_bytes(symbol, symbol.size)
        if symbol.value != address or actual != targets[address]:
            raise SystemExit(f"provider-linked target byte mismatch: {name} at {symbol.value:#x}")
        digest = hashlib.sha256(actual).hexdigest()
        frozen = [r for r in evidence_rows if r["address"] == f"0x{address:08x}"]
        expected = {"address": f"0x{address:08x}", "symbol": name,
                    "size": str(size), "relocations": str(relocations),
                    "raw_sha256": raw_sha, "linked_sha256": digest,
                    "evidence": LISTING.relative_to(ROOT).as_posix()}
        if frozen != [expected]:
            raise SystemExit(f"APUMEM evidence ledger drift: {name}")
        print(f"APUMEM {name}: MATCH bytes={size}/{size} relocations={relocations} target={address:#010x}")
        report.append({"address": address, "symbol": name, "size": size,
                       "relocations": relocations, "raw_sha256": raw_sha,
                       "linked_sha256": digest, "result": "MATCH"})
    (BUILD / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print("APUMEM provider-linked: MATCH functions=4/4 bytes=880/880")


if __name__ == "__main__":
    main()
