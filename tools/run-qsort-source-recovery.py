#!/usr/bin/env python3
"""Reprove the pinned historical qsort source against the public frozen evidence."""
from __future__ import annotations
import csv
import hashlib
import shlex
import subprocess
import urllib.request
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/qsort-source-recovery"
LOCAL = ROOT / "src/ps2/qsort.c"
RAW_URL = "https://raw.githubusercontent.com/duduclx/PS2DEV/bac0006c6302edcf1bdae253799484497b4e5032/ps2toolchain/soft/newlib-1.10.0/newlib/libc/stdlib/qsort.c"
ADDRESS = "0x001080cc"
SIZE = 2408
RAW_SHA256 = "59daebda19ccf8c46a78de83ac2a7a60c4ba2c8323e1ade887a2de32156978a9"
HIST_OBJECT_SHA256 = "350080fd28f85d6e085434c9e8143e5ce15f0320c47ca3a800441cc99590daed"
EXPECTED_RELOCS = 1
PROFILE = "newlib-1.10-os-size_t-int"
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
    marker = "#ifndef __GNUC__"
    if marker not in historical_text or marker not in local_text:
        raise SystemExit("historical body marker missing")
    if historical_text.split(marker, 1)[1] != local_text.split(marker, 1)[1]:
        raise SystemExit("local historical body drift")

    compat = BUILD / "compat"
    compat.mkdir(parents=True, exist_ok=True)
    (compat / "_ansi.h").write_text("#define _PARAMS(parameters) parameters\n#define _DEFUN(name, arglist, args) name(args)\n#define _DEFUN_VOID(name) name(void)\n#define _AND ,\n", encoding="utf-8")
    (compat / "stdlib.h").write_text("#define size_t int\n", encoding="utf-8")

    hist_obj = BUILD / "historical.o"
    run([CC, "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64", "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900", "-Os", "-I", "build/matching/qsort-source-recovery/compat", "-c", upstream, "-o", hist_obj])
    hsize, hraw, hrel = symbol_contract(hist_obj, "qsort")
    if (hsize, hrel, hashlib.sha256(hraw).hexdigest()) != (SIZE, EXPECTED_RELOCS, RAW_SHA256):
        raise SystemExit(f"historical object drift size={hsize} relocs={hrel} sha={hashlib.sha256(hraw).hexdigest()}")
    print(f"QSORT historical: MATCH bytes={hsize}/{SIZE} relocations={hrel} raw_sha256={RAW_SHA256}")

    local_obj = BUILD / "canonical-candidate.o"
    run([CC, "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer", "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64", "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900", "-Os", "build/matching/qsort-source-recovery/compat", "-I", ROOT / "include",
         "-c", LOCAL, "-o", local_obj])
    lsize, lraw, lrel = symbol_contract(local_obj, "snes_qsort_001080cc")
    if (lsize, lrel, hashlib.sha256(lraw).hexdigest()) != (SIZE, EXPECTED_RELOCS, RAW_SHA256):
        raise SystemExit(f"local source drift size={lsize} relocs={lrel} sha={hashlib.sha256(lraw).hexdigest()}")
    if lraw != hraw:
        raise SystemExit("local .text differs from pinned historical .text outside symbol naming")
    print(f"QSORT local source: MATCH bytes={lsize}/{SIZE} relocations={lrel} raw_sha256={RAW_SHA256}")

if __name__ == "__main__":
    main()
