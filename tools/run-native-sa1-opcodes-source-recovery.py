#!/usr/bin/env python3
"""Prove 465 native SA-1 opcode/helper routines and all four dispatch tables.

The existing execution proof rebuilds and authenticates the complete original
67560-byte code window, relocation roster and target-linked data digest. This
proof then places every new function at its original address and compares all
67008 instruction bytes, including relocated calls and shared-state operands.
The IRQ/main loop are excluded, and table bytes are counted separately.
No fresh private ELF or complete replacement-image comparison is implied.
"""
from __future__ import annotations

import csv
import importlib.util
import json
from pathlib import Path

from build_source_tree import SOURCE_FIXED_FLAGS
from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-sa1-opcodes-source-recovery"
SOURCE = "src/snes9x/native_sa1_opcodes.cpp"
LEDGER = ROOT / "analysis/functions/native_sa1_opcodes_exact_67008.tsv"
TABLE_LEDGER = ROOT / "analysis/functions/native_sa1_opcode_tables_exact_4096.tsv"
PROVIDERS = {'DAT_00345268': 3428968,
 'DAT_003454e0': 3429600,
 'DAT_00345ae8': 3431144,
 'DAT_0034e2a8': 3465896,
 'DAT_0034e2b0': 3465904,
 'DAT_0035b73f': 3520319,
 'DAT_0035b740': 3520320,
 'DAT_0035b741': 3520321,
 'DAT_0035b742': 3520322,
 'DAT_0035b743': 3520323,
 'DAT_0035b744': 3520324,
 'DAT_0035b745': 3520325,
 'DAT_0035b746': 3520326,
 'DAT_0035b747': 3520327,
 'DAT_0035b748': 3520328,
 'DAT_0035b74c': 3520332,
 'DAT_0035b750': 3520336,
 'DAT_0035b752': 3520338,
 'DAT_0035b75a': 3520346,
 'DAT_0035b760': 3520352,
 'S9xSA1GetByte': 1432708,
 'S9xSA1GetWord': 1433108,
 'S9xSA1SetByte': 1433192,
 'S9xSA1SetPCBase': 1433724,
 'S9xSA1SetWord': 1433652,
 '__gxx_personality_v0': 1742632,
 'g_OpenBus_byte': 3520360,
 'g_SA1_blob': 3431160}
TABLES = [{'symbol': 'S9xSA1OpcodesM1X1',
  'address': '0x003f5040',
  'size': '1024',
  'linked_sha256': 'b4dcba95e9bd550c50f6fff5c78540fcae6045b97a0f98ce9dec9e3c210113e8'},
 {'symbol': 'S9xSA1OpcodesM1X0',
  'address': '0x003f5440',
  'size': '1024',
  'linked_sha256': '6de5b91f53508ecc7972344cf9c73cde026103af9703254c969a350e9b8d88ab'},
 {'symbol': 'S9xSA1OpcodesM0X0',
  'address': '0x003f5840',
  'size': '1024',
  'linked_sha256': 'f63db5c57e8a0508060511e603708ad551ffeaeec31203c529e5f8c653686460'},
 {'symbol': 'S9xSA1OpcodesM0X1',
  'address': '0x003f5c40',
  'size': '1024',
  'linked_sha256': 'f1cf49a79693a73fd23821fae320e6f5a8f10b0000c1803bc3b83590ba0b3233'}]
LEDGER_SHA256 = '1a300a6f8a163629b7f4d07a5628119068419091b872364137db33fd2810ee4d'
CODE_SHA256 = 'ae9d13072e3fd1fde6d9aec6b91ada40dedd8421b0be44ee6966249d866f37b0'


def load_execution_proof():
    spec = importlib.util.spec_from_file_location(
        "native_sa1_execution_proof", ROOT / "tools/run-native-sa1-execution-source-recovery.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main():
    execution = load_execution_proof()
    # Reconstruct from the pinned archive and verify the independent frozen
    # window, GLOBALS geometry and entire target-linked original data section.
    execution.main()
    if SOURCE_FIXED_FLAGS[SOURCE] != execution.FLAGS:
        raise SystemExit("native SA-1 opcode compiler profile drift")
    BUILD.mkdir(parents=True, exist_ok=True)
    obj = BUILD / "native_sa1_opcodes.o"
    execution.run([execution.CXX, *SOURCE_FIXED_FLAGS[SOURCE], "-c", ROOT / SOURCE, "-o", obj])
    original = ELFFile(execution.BUILD / "SA1CPU.historical.o")
    reference = ELFFile(execution.BUILD / "SA1CPU.historical.elf")
    isolated = ELFFile(obj)
    if execution.digest(LEDGER.read_bytes()) != LEDGER_SHA256:
        raise SystemExit("native SA-1 opcode function ledger drift")
    with LEDGER.open(newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    excluded = {r["symbol"] for r in execution.SPECS}
    expected = sorted((s for s in original.symbols if s.info & 15 == 2 and s.size
                       and s.name not in excluded), key=lambda s: s.value)
    functions = [s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    if (len(rows) != 465 or sum(int(r["size"]) for r in rows) != 67008
            or [r["symbol"] for r in rows] != [s.name for s in expected]
            or {s.name for s in functions} != {s.name for s in expected}
            or sum(s.size for s in functions) != 67008):
        raise SystemExit("native SA-1 opcode complete function inventory drift")
    imports = {s.name for s in isolated.symbols if s.section_index == 0 and s.name}
    if imports != set(PROVIDERS):
        raise SystemExit("native SA-1 opcode provider ABI/import roster drift")
    # Work32 and Int8 are assembler expressions over existing anchors, not
    # duplicated or newly invented providers. Check the original state offsets.
    if (PROVIDERS["DAT_0035b752"] + 2 != execution.REFERENCE_STATE["Work32"][0]
            or PROVIDERS["DAT_0035b75a"] - 2 != execution.REFERENCE_STATE["Int8"][0]):
        raise SystemExit("native SA-1 scratch alias geometry drift")
    objects = [s for s in isolated.symbols if s.info & 15 == 1 and s.size]
    if ({(s.name, s.size, s.value) for s in objects}
            != {(r["symbol"], 1024, int(r["address"], 0) - 0x3f5040) for r in TABLES}
            or any(s.size for s in isolated.sections if s.name.startswith((".bss", ".rodata")))):
        raise SystemExit("native SA-1 opcode storage/table inventory drift")
    script = BUILD / "native_sa1_opcodes.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
        + "SECTIONS {\n" + "".join(
            f".text.{i} {int(r['address'], 0):#x} : {{ *(.text.{r['symbol']}) }}\n"
            for i, r in enumerate(rows))
        + ".data 0x3f5040 : { *(.data*) }\n"
          "/DISCARD/ : { *(.bss*) *(COMMON) *(.reginfo) *(.pdr) "
          "*(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked_path = BUILD / "native_sa1_opcodes.elf"
    execution.run([execution.CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked_path])
    linked = ELFFile(linked_path)
    aggregate = bytearray()
    for row, old in zip(rows, expected):
        name, size, address = row["symbol"], int(row["size"]), int(row["address"], 0)
        local, placed, ref = (elf.find_symbol(name) for elf in (isolated, linked, reference))
        historical_raw = original.symbol_bytes(old, old.size)
        isolated_raw = isolated.symbol_bytes(local, local.size)
        raw = linked.symbol_bytes(placed, placed.size)
        binding = "global" if old.info >> 4 == 1 else "local"
        if (any(s.size != size for s in (old, local, placed, ref))
                or old.value + 0x15f1c8 != address or placed.value != address or ref.value != address
                or local.info >> 4 != old.info >> 4 or row["binding"] != binding
                or row["source_file"] != SOURCE
                or execution.normalized(original, old) != execution.normalized(isolated, local)
                or execution.digest(historical_raw) != row["historical_raw_sha256"]
                or execution.digest(isolated_raw) != row["isolated_raw_sha256"]
                or execution.digest(execution.normalized(isolated, local)) != row["normalized_sha256"]
                or len(isolated.relocation_ranges(local, size)) != int(row["relocations"])
                or raw != reference.symbol_bytes(ref, ref.size)
                or execution.digest(raw) != row["linked_sha256"]
                or row["proof_level"] != "linked-historical-reference"):
            raise SystemExit("native SA-1 opcode full linked instruction drift: " + name)
        aggregate.extend(raw)
    if execution.digest(aggregate) != CODE_SHA256 or len(aggregate) != 67008:
        raise SystemExit("native SA-1 opcode aggregate instruction digest drift")
    for row in TABLES:
        name, address = row["symbol"], int(row["address"], 0)
        placed, ref = linked.find_symbol(name), reference.find_symbol(name)
        raw = linked.symbol_bytes(placed, placed.size)
        if (placed.size != 1024 or ref.size != 1024 or placed.value != address or ref.value != address
                or raw != reference.symbol_bytes(ref, 1024)
                or execution.digest(raw) != row["linked_sha256"]):
            raise SystemExit("native SA-1 complete target-linked dispatch table drift: " + name)
    with TABLE_LEDGER.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != TABLES:
            raise SystemExit("native SA-1 opcode table ledger drift")
    with (ROOT / "analysis/source_tree/defined_symbol_ownership.tsv").open(newline="") as stream:
        definitions = [r for r in csv.DictReader(stream, delimiter="\t") if r["source"] == SOURCE]
    owned = {(r["symbol"], r["binding"], r["section_class"], int(r["size_hex"], 0))
             for r in definitions if int(r["size_hex"], 0)}
    wanted = {(r["symbol"], r["binding"], "text", int(r["size"])) for r in rows}
    wanted |= {(r["symbol"], "global", "data", 1024) for r in TABLES}
    if owned != wanted or any(r["object"] != "snes9x/native_sa1_opcodes.o" for r in definitions):
        raise SystemExit("canonical native SA-1 opcode function/table ownership drift")
    result = {"routines": 465, "instruction_bytes": 67008, "dispatch_tables": 4,
              "dispatch_table_bytes": 4096, "linked_instruction_sha256": CODE_SHA256,
              "proof_level": "linked-historical-reference", "duplicate_shared_state": False,
              "excluded_preexisting_routines": sorted(excluded)}
    (BUILD / "report.json").write_text(json.dumps(result, indent=2) + "\n")
    print("native SA-1 opcodes: MATCH 465/465 routines, 67008/67008 linked historical instruction bytes; "
          "4/4 dispatch tables, 4096/4096 complete target-linked data bytes; shared state retained")


if __name__ == "__main__":
    main()
