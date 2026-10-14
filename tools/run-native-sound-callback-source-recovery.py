#!/usr/bin/env python3
"""Prove the original native PS2 S9xGenerateSound callback and its caller ABI."""
from __future__ import annotations
import csv
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from build_source_tree import SOURCE_FIXED_FLAGS
from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-sound-callback-source-recovery"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
SOURCE = "src/ps2/native_sound_callback.c"
SHA = "6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1"
EVIDENCE = ROOT / "analysis/functions/native_sound_callback_exact_8.tsv"
SPECS = [{"address": "0x00101904", "symbol": "S9xGenerateSound", "source_file": SOURCE,
          "size": "8", "relocations": "0", "historical_raw_sha256": SHA,
          "isolated_raw_sha256": SHA, "linked_sha256": SHA, "target_sha256": SHA,
          "proof_level": "provider-linked-target",
          "evidence": "analysis/matching/hunt1000plus-v41-validated-28.tsv"}]


def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command)) + "\n" + result.stdout[-6000:])
    return result.stdout


def main():
    if run([CC, "-dumpmachine"]).strip() != "ee" or run([CC, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1000plus_v47_closure as recipe
    archive = recipe.download_archive(recipe.SNES_141_1_ARCHIVE, recipe.SNES_CACHE)
    root = recipe.safe_extract_archive(archive, recipe.SNES_CACHE / "source-1.41-1",
                                       recipe.SNES_141_1_ARCHIVE.source_directory) / "snes9x"
    if ("EXTERN_C void S9xGenerateSound ();" not in (root / "port.h").read_text(encoding="latin-1")
            or "S9xGenerateSound ();" not in (root / "CPUEXEC.CPP").read_text(encoding="latin-1")):
        raise SystemExit("independent native sound callback declaration/caller ABI drift")
    # The maintained CPU execution proof independently binds this native entry
    # and proves the original call site. Its full instruction proof stays intact.
    import runpy
    cpu = runpy.run_path(str(ROOT / "tools/run-cpu-execution-source-recovery.py"))
    if cpu["PROVIDERS"].get("S9xGenerateSound") != 0x101904:
        raise SystemExit("proved CPU execution callback address drift")
    body = "void snes_leaf_00101904(void) {}"
    if body not in (ROOT / "src/ps2/address_leaf_recovered.c").read_text():
        raise SystemExit("independent frozen frontend leaf source drift")
    flags = SOURCE_FIXED_FLAGS[SOURCE]
    BUILD.mkdir(parents=True, exist_ok=True)
    leaf = BUILD / "reference.c"
    leaf.write_text(body + "\n")
    run([CC, *flags, "-c", leaf, "-o", BUILD / "reference.o"])
    run([CC, *flags, "-c", ROOT / SOURCE, "-o", BUILD / "native.o"])
    reference, local = ELFFile(BUILD / "reference.o"), ELFFile(BUILD / "native.o")
    functions = [s for s in local.symbols if s.info & 15 == 2 and s.size]
    objects = [s for s in local.symbols if s.info & 15 == 1 and s.size]
    if (len(functions) != 1 or functions[0].name != "S9xGenerateSound" or functions[0].info >> 4 != 1
            or objects or any(s.name for s in local.symbols if s.section_index == 0)):
        raise SystemExit("native callback inventory/storage/import drift")
    old, native = reference.find_symbol("snes_leaf_00101904"), local.find_symbol("S9xGenerateSound")
    if (old.size != 8 or native.size != 8 or local.relocation_ranges(native, 8)
            or reference.relocation_ranges(old, 8) or local.symbol_bytes(native, 8) != reference.symbol_bytes(old, 8)
            or hashlib.sha256(local.symbol_bytes(native, 8)).hexdigest() != SHA):
        raise SystemExit("native sound callback complete target instructions drift")
    script = BUILD / "native.ld"
    script.write_text("SECTIONS { .text 0x101904 : { *(.text.S9xGenerateSound) }\n"
        "/DISCARD/ : { *(.data*) *(.bss*) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    run([CC.with_name("ee-ld"), "-EL", "-T", script, BUILD / "native.o", "-o", BUILD / "native.elf"])
    linked = ELFFile(BUILD / "native.elf")
    final = linked.find_symbol("S9xGenerateSound")
    if final.value != 0x101904 or final.size != 8 or hashlib.sha256(linked.symbol_bytes(final, 8)).hexdigest() != SHA:
        raise SystemExit("native sound callback linked target placement drift")
    with (ROOT / SPECS[0]["evidence"]).open(newline="") as stream:
        rows = [r for r in csv.DictReader(stream, delimiter="\t") if r["address"] == "0x00101904"]
    if len(rows) != 1 or any(rows[0][k] != v for k, v in
            {"object_symbol": "snes_leaf_00101904", "object_size": "8", "result": "MATCH",
             "differing_bytes": "0", "raw_equal": "True", "normalized_equal": "True", "unknown_relocations": ""}.items()):
        raise SystemExit("independent complete target leaf witness drift")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native sound callback evidence ledger drift")
    print("native PS2 sound callback: MATCH S9xGenerateSound bytes=8/8 "
          "proof=provider-linked-target; original C ABI; canonical CPU execution callee")


if __name__ == "__main__":
    main()
