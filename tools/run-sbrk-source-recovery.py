#!/usr/bin/env python3
"""Reprove the pinned historical sbrk source against the public frozen evidence."""
from __future__ import annotations
import csv
import hashlib
import shlex
import subprocess
import urllib.request
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/sbrk-source-recovery"
LOCAL = ROOT / "src/ps2/sbrk.c"
RAW_URL = "https://raw.githubusercontent.com/ps2dev/ps2sdk/694100b78ad5bc8f8248a1138143860af4f8435f/ee/libc/src/sbrk.c"
ADDRESS = "0x0019f078"
SIZE = 192
RAW_SHA256 = "020c1ac0acf1013fd69ca59ee00213634d75a58729c327a85f14c34d872fac05"
HIST_OBJECT_SHA256 = "0c03c51748af4d7fd42f400f150094f9ad9d88c5e880352ced2b4b8045b43868"
EXPECTED_RELOCS = 13
PROFILE = "ps2lib-20040415-os-split"
EVIDENCE = ROOT / "analysis/matching/hunt1000plus-v47-validated-79.tsv"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

def run(cmd):
    cp = subprocess.run([str(x) for x in cmd], cwd=ROOT, text=True,
                        stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if cp.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, cmd)) + "\n" + cp.stdout[-12000:])
    return cp.stdout

def symbol_contract(path: Path, symbol_name: str):
    elf = ELFFile(path)
    sym = elf.find_symbol(symbol_name)
    raw = elf.symbol_bytes(sym, sym.size)
    relocs = len(elf.relocation_ranges(sym, sym.size))
    return sym.size, raw, relocs

def check_evidence():
    with EVIDENCE.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    row = next((r for r in rows if r["address"] == ADDRESS), None)
    if row is None:
        raise SystemExit("missing frozen evidence row")
    expected = {
        "profile": PROFILE,
        "object_size": str(SIZE),
        "result": "MATCH",
        "differing_bytes": "0",
        "normalized_equal": "True",
        "unknown_relocations": "",
        "object_sha256": HIST_OBJECT_SHA256,
    }
    for key, value in expected.items():
        if row.get(key) != value:
            raise SystemExit(f"frozen evidence drift: {key}={row.get(key)!r}")

def main():
    BUILD.mkdir(parents=True, exist_ok=True)
    if run([CC, "-dumpmachine"]).strip() != "ee" or run([CC, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")

    check_evidence()
    upstream = BUILD / "historical.c"
    upstream.write_bytes(urllib.request.urlopen(RAW_URL, timeout=30).read())

    historical_text = upstream.read_text(encoding="utf-8")
    local_text = LOCAL.read_text(encoding="utf-8")
    marker = "extern void * _end;"
    if marker not in historical_text or marker not in local_text:
        raise SystemExit("historical body marker missing")
    if historical_text.split(marker, 1)[1] != local_text.split(marker, 1)[1]:
        raise SystemExit("local historical body drift")

    compat = BUILD / "compat"
    compat.mkdir(parents=True, exist_ok=True)
    (compat / "tamtypes.h").write_text("typedef unsigned int size_t;\n", encoding="utf-8")
    (compat / "kernel.h").write_text("int DIntr(void);\nint EIntr(void);\nvoid *EndOfHeap(void);\n#define DI() DIntr()\n#define EI() EIntr()\n", encoding="utf-8")

    hist_obj = BUILD / "historical.o"
    run([CC, "-D_EE", "-DPS2_EE", "-G0", "-EL", "-pipe", "-w", "-Os", "-I", "build/matching/sbrk-source-recovery/compat", "-c", upstream, "-o", hist_obj])
    hsize, hraw, hrel = symbol_contract(hist_obj, "ps2_sbrk")
    if (hsize, hrel, hashlib.sha256(hraw).hexdigest()) != (SIZE, EXPECTED_RELOCS, RAW_SHA256):
        raise SystemExit(f"historical object drift size={hsize} relocs={hrel} sha={hashlib.sha256(hraw).hexdigest()}")
    print(f"SBRK historical: MATCH bytes={hsize}/{SIZE} relocations={hrel} raw_sha256={RAW_SHA256}")

    local_obj = BUILD / "canonical-candidate.o"
    run([CC, "-D_EE", "-DPS2_EE", "-G0", "-EL", "-pipe", "-w", "-Os",
         "-I", ROOT / "include", "-c", LOCAL, "-o", local_obj])
    lsize, lraw, lrel = symbol_contract(local_obj, "ps2_sbrk")
    if (lsize, lrel, hashlib.sha256(lraw).hexdigest()) != (SIZE, EXPECTED_RELOCS, RAW_SHA256):
        raise SystemExit(f"local source drift size={lsize} relocs={lrel} sha={hashlib.sha256(lraw).hexdigest()}")
    if lraw != hraw:
        raise SystemExit("local .text differs from pinned historical .text outside symbol naming")
    print(f"SBRK local source: MATCH bytes={lsize}/{SIZE} relocations={lrel} raw_sha256={RAW_SHA256}")

if __name__ == "__main__":
    main()
