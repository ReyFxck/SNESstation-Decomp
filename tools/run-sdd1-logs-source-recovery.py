#!/usr/bin/env python3
"""Prove both historical S-DD1 PS2 FIO log persistence functions.

The isolated C++ source reproduces the exact previously validated V51 object
fingerprints and all 324 provider-linked target bytes. Save uses -Os; load
uses -O2 with four-byte function alignment. Both instruction hashes remain
identical to V51. Read counts retain the target's original byte-count semantics.
"""
from __future__ import annotations

import csv
import hashlib
import json
import subprocess
from pathlib import Path

from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/sdd1-logs-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
EVIDENCE = ROOT / "analysis/functions/sdd1_logs_exact_324.tsv"
FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-Os", "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
    "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN", "-DEXECUTE_SUPERFX_PER_LINE",
    "-DSPC700_C", "-DUNZIP_SUPPORT", "-DNO_INLINE_SET_GET",
)
SPECS = ((1506052,
  '_Z21S9xSDD1SaveLoggedDatav',
  'sdd1_log_save',
  '-Os',
  176,
  13,
  '22e6c91c8c0d47723affba3391823fb0772fc3f79f95044c8cff785b092a5d2b',
  '6b8b4500514f65179d11b554797d018fbe0a3afa7d094026976d91a1b43ad154',
  'snes_p12_0016fb04'),
 (1506228,
  '_Z21S9xSDD1LoadLoggedDatav',
  'sdd1_log_load',
  '-O2',
  148,
  10,
  '641ea892d5319e3e5f45ef14dcbe2e1a9e5e4dfa245e9408da5e15f9d1d96cd8',
  '281678c065e90518cb6c5087bf12c735c59d2ff7a45721778cb17a8108ced7b1',
  'snes_p12_0016fbb4'))
def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command))
                         + "\n" + result.stdout[-12000:])
    return result.stdout



PROVIDERS = {
    "g_p12_memory": 0x0034E2B0, "g_p12_sdd1_dat_extension": 0x001B8050,
    "snes_p12_get_filename": 0x00101924,
    "snes_p12_compare_sdd1_entries": 0x0016FAC4,
    "snes_qsort_001080cc": 0x001080CC,
    "fioOpen": 0x0019CFC0, "fioClose": 0x0019D090,
    "fioRead": 0x0019D120, "fioWrite": 0x0019D244,
}
HISTORICAL = ROOT / "analysis/matching/hunt1041-v51-validated-16.tsv"
WINDOWS = ROOT / "analysis/link_identity/code_windows.json"


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    BUILD.mkdir(parents=True, exist_ok=True)
    selected = json.loads(WINDOWS.read_text())["result"]["selected_sources"]
    with HISTORICAL.open(newline="") as stream:
        historical = list(csv.DictReader(stream, delimiter="\t"))
    rows = []
    for address, name, unit, option, size, relocations, raw_sha, linked_sha, old_symbol in SPECS:
        captures = [s for s in selected if s.get("address") == address and s.get("symbol") == old_symbol]
        expected = {"size": size, "new_bytes": size, "relocations": relocations, "raw_sha256": raw_sha}
        witness = [r for r in historical if r["address"] == f"0x{address:08x}" and r["object_symbol"] == old_symbol]
        if (len(captures) != 1 or any(captures[0].get(k) != v for k, v in expected.items())
                or len(witness) != 1 or witness[0]["result"] != "MATCH"
                or witness[0]["differing_bytes"] != "0" or witness[0]["unknown_relocations"]
                or witness[0]["object_size"] != str(size) or witness[0]["target_span_sha256"] != linked_sha):
            raise SystemExit(f"frozen persistence proof drift: {name}")
        obj = BUILD / f"{unit}.o"
        # The target entry 0x0016fbb4 is four-byte aligned. This affects section
        # alignment only; the frozen 148-byte instruction fingerprint is exact.
        alignment = ("-falign-functions=4",) if option == "-O2" else ()
        run([CXX, *(f for f in FLAGS if f != "-Os"), option, *alignment,
             "-c", ROOT / f"src/snes9x/{unit}.cpp", "-o", obj])
        elf = ELFFile(obj)
        functions = [s for s in elf.symbols if s.info & 15 == 2 and s.size]
        symbol = elf.find_symbol(name)
        section = next(s for s in elf.sections if s.name == ".text")
        if (len(functions) != 1 or functions[0].name != name or section.size != size
                or symbol.value != 0 or symbol.size != size
                or len(elf.relocation_ranges(symbol, size)) != relocations
                or any(not r.known for r in elf.relocation_masks(symbol, size))
                or hashlib.sha256(elf.symbol_bytes(symbol, size)).hexdigest() != raw_sha):
            raise SystemExit(f"persistence object bytes drift: {name}")
        script = BUILD / f"{unit}.target.ld"
        script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
                          + f"SECTIONS {{ .text {address:#x} : {{ *(.text) }}\n"
                          + " /DISCARD/ : { *(.data) *(.bss) *(COMMON) *(.rodata*) *(.reginfo) *(.pdr)"
                          + " *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
        linked = BUILD / f"{unit}.target.elf"
        run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked])
        final = ELFFile(linked)
        actual = final.find_symbol(name)
        if (actual.value != address or actual.size != size
                or hashlib.sha256(final.symbol_bytes(actual, size)).hexdigest() != linked_sha):
            raise SystemExit(f"provider-linked persistence mismatch: {name}")
        rows.append({"address": f"0x{address:08x}", "symbol": name,
                     "source_file": f"src/snes9x/{unit}.cpp",
                     "profile": "O2-align4" if alignment else option[1:], "size": str(size),
                     "relocations": str(relocations), "raw_sha256": raw_sha, "linked_sha256": linked_sha,
                     "historical_symbol": old_symbol, "evidence": HISTORICAL.relative_to(ROOT).as_posix()})
        print(f"S-DD1 persistence provider-linked: MATCH {name} bytes={size}/{size} relocations={relocations}")
    with EVIDENCE.open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if rows != frozen:
        raise SystemExit("S-DD1 persistence ledger drift")
    (BUILD / "report.json").write_text(json.dumps(rows, indent=2) + "\n")
    print("S-DD1 persistence: MATCH functions=2/2 bytes=324/324 relocations=23")


if __name__ == "__main__":
    main()
