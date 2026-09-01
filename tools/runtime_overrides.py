#!/usr/bin/env python3
"""Prove the two target-selected runtime overrides, not an archive pedigree.

Named incoming relocations in independently reproduced historical callers
establish which target implements abort/puts. Recovered C then links to exact
target bytes, including the outgoing fioWrite relocation. The linker output
is an isolated proof ELF, not a replacement application. Historical PS2LIB
puts/terminate candidates remain rejected as providers of these contracts.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import struct
from pathlib import Path
from typing import Sequence

import libgcc_contracts as libgcc
from compare_elf_functions import ELFFile, Symbol, MIPS32_RELOCATION_MASKS

ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "analysis/link_identity"
SOURCE = ROOT / "matching/candidates/runtime_overrides.c"
DEFAULT_MANIFEST = BASE / "runtime_overrides.tsv"
DEFAULT_WITNESSES = BASE / "runtime_override_witnesses.tsv"
DEFAULT_BUILD = ROOT / "build/runtime-overrides"
KIND = "recovered-runtime"
STATUS = "TARGET_OVERRIDE_EXACT"
FLAGS = ("-G0", "-EL", "-pipe", "-w", "-Os", "-fomit-frame-pointer",
         "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
         "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
         "-ffunction-sections", "-fno-builtin", "-ffreestanding", "-nostdinc")
SPECS = {
    "abort": (0x107578, 8, "snes_fatal_spin_00107578", "src/ps2/libc_misc_recovered.c",
              "analysis/matching/hunt1000plus-v41-validated-28.tsv"),
    "puts": (0x19E414, 96, "puts_like_recovered", "src/ps2/stdio_wrappers_recovered.c",
             "analysis/matching/hunt1000plus-v47-validated-79.tsv"),
}
# The termination witness is one complete 36-byte weak function, NOT a whole
# matching terminate.o: its neighboring exit body is different in the target.
WITNESSES = (
    ("unwind-dw2.o", ".text", 0x1A3DC0, 7936, "abort",
     (0x64, 0x120, 0x320, 0xA80, 0xFAC, 0x1408, 0x1508, 0x17E0, 0x1E8C)),
    ("unwind-dw2-fde.o", ".text", 0x1A5CC0, 6336, "abort",
     (0x64, 0x268, 0x4A0, 0x578, 0x176C)),
    ("libc/terminate.o", "abort", 0x19C5A8, 36, "puts", (0xC,)),
)
FIELDS = ("symbol", "status", "target_address", "extent_hex", "source",
          "source_sha256", "profile_sha256", "canonical_symbol", "canonical_source",
          "requesters", "target_sha256", "normalized_sha256", "linked_sha256",
          "incoming_calls", "outgoing_calls", "matching_evidence", "evidence_sha256")
WITNESS_FIELDS = ("member", "span", "target_address", "extent_hex", "text_sha256",
                  "target_sha256", "normalized_sha256", "relocation_count",
                  "contract", "call_offsets", "callee_address", "scope")


class RuntimeOverrideError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise RuntimeOverrideError(message)


def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def profile_hash() -> str:
    return digest(("\n".join(FLAGS) + "\n").encode())


def ownership(symbol: str) -> tuple[str, str] | None:
    if symbol not in SPECS:
        return None
    return (f"target-selected recovered override: {SPECS[symbol][2]}",
            "runtime-override-callsite-identity")


def unique_symbol(elf: ELFFile, name: str) -> Symbol:
    rows = [s for s in elf.symbols if s.name == name and s.section_index != 0]
    if len(rows) != 1 or rows[0].info & 15 != 2:
        fail(f"missing/ambiguous function symbol: {name}")
    return rows[0]


def span_symbol(elf: ELFFile, name: str) -> Symbol:
    if name != ".text":
        return unique_symbol(elf, name)
    sections = [s for s in elf.sections if s.name == ".text"]
    if len(sections) != 1:
        fail("historical witness must have one complete .text section")
    section = sections[0]
    return Symbol(".text", section.address, section.size, section.index, 2)


def named_relocations(elf: ELFFile, symbol: Symbol, require_undefined_name: str | None = None) -> list[tuple[int, int, str]]:
    """Read ELF32 MIPS REL entries and retain their symbol-table identities."""
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, 1):
        fail("witness must be a little-endian MIPS ELF32 relocatable")
    result = []
    for section in elf.sections:
        if section.info != symbol.section_index or section.type not in (4, 9):
            continue
        if section.type != 9 or section.entry_size != 8 or section.size % 8:
            fail("unsupported witness relocation encoding")
        table = elf.sections[section.link]
        if table.type != 2 or table.entry_size != 16:
            fail("unsupported witness symbol table")
        strings = elf.sections[table.link]
        names = elf.data[strings.offset:strings.offset + strings.size]
        for pos in range(section.offset, section.offset + section.size, 8):
            offset, info = struct.unpack_from("<II", elf.data, pos)
            if not symbol.value <= offset < symbol.value + symbol.size:
                continue
            index = info >> 8
            if index * 16 >= table.size:
                fail("witness relocation symbol outside table")
            name_offset = struct.unpack_from("<I", elf.data, table.offset + index * 16)[0]
            if name_offset >= len(names):
                fail("witness relocation symbol name outside strings")
            name = ELFFile._cstring(names, name_offset)
            shndx = struct.unpack_from("<H", elf.data, table.offset + index * 16 + 14)[0]
            if name == require_undefined_name and shndx != 0:
                fail("named incoming/outgoing contract is not an undefined external")
            result.append((offset - symbol.value, info & 255, name))
    return sorted(result)


def direct_callee(word: int, address: int) -> int:
    if word >> 26 != 3:
        fail(f"expected direct JAL at 0x{address:08x}")
    return ((address + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)


def compare_text(image: bytes, masks: Sequence[object], target: bytes, label: str) -> None:
    if len(image) != len(target) or any(not m.known or m.start < 0 or m.end > len(image)
                                      or m.start % 4 or m.end - m.start != 4
                                      or m.relocation_type not in MIPS32_RELOCATION_MASKS
                                      or m.mask_bytes != MIPS32_RELOCATION_MASKS.get(m.relocation_type, 0).to_bytes(4, "little")
                                      for m in masks):
        fail(f"invalid span/relocation mask: {label}")
    if libgcc.differing_unmasked(target, image, masks):
        fail(f"witness/provider bytes differ outside relocation bits: {label}")


def verify_named_calls(image: bytes, target: bytes, relocations: Sequence[tuple[int, int, str]],
                       base: int, contract: str, expected_offsets: Sequence[int], callee: int) -> None:
    selected = [(o, k) for o, k, name in relocations if name == contract]
    if selected != [(o, 4) for o in expected_offsets]:
        fail(f"named incoming relocation roster drift: {contract}")
    for offset in expected_offsets:
        if offset < 0 or offset % 4 or offset + 8 > len(target):
            fail("incoming call outside complete witness")
        # An R_MIPS_26 addend must be zero: do not misidentify symbol+offset.
        original = struct.unpack_from("<I", image, offset)[0]
        if original != 0x0C000000:
            fail(f"nonzero/non-JAL incoming addend: {contract}")
        actual = direct_callee(struct.unpack_from("<I", target, offset)[0], base + offset)
        if actual != callee:
            fail(f"incoming call selects a different target: {contract}")


def fixed_provider(symbol: str, external: dict[str, dict[str, str]]) -> dict[str, str]:
    address, size, canonical, source, evidence = SPECS[symbol]
    return {"symbol": symbol, "status": STATUS, "target_address": f"0x{address:08x}",
            "extent_hex": hex(size), "source": SOURCE.relative_to(ROOT).as_posix(),
            "source_sha256": digest(SOURCE.read_bytes()), "profile_sha256": profile_hash(),
            "canonical_symbol": canonical, "canonical_source": source,
            "requesters": external[symbol]["requesters"],
            "incoming_calls": str(sum(len(w[5]) for w in WITNESSES if w[4] == symbol)),
            "outgoing_calls": "fioWrite@0x0019d244" if symbol == "puts" else "",
            "matching_evidence": evidence, "evidence_sha256": digest((ROOT / evidence).read_bytes())}


def fixed_witness(spec: tuple) -> dict[str, str]:
    member, span, address, size, contract, offsets = spec
    return {"member": member, "span": span, "target_address": f"0x{address:08x}",
            "extent_hex": hex(size), "contract": contract,
            "call_offsets": ";".join(hex(o) for o in offsets),
            "callee_address": f"0x{SPECS[contract][0]:08x}",
            "scope": "complete-member-text" if span == ".text" else "complete-weak-function-only"}


def live_contracts(args: argparse.Namespace) -> dict[str, dict[str, str]]:
    external = {r["symbol"]: r for r in libgcc.read_table(args.external_map, libgcc.EXTERNAL_FIELDS)}
    contracts = {r["symbol"]: r for r in libgcc.read_table(args.contracts, libgcc.CONTRACT_FIELDS)}
    frontier = {r["symbol"]: r for r in libgcc.read_table(args.frontier_manifest, libgcc.FRONTIER_FIELDS)}
    with args.defined_map.open(encoding="utf-8", newline="") as stream:
        defined = list(csv.DictReader(stream, delimiter="\t"))
    if len(external) != 1892 or len(contracts) != 1569 or len(frontier) != 223:
        fail("runtime override namespace count drift")
    for symbol, (address, size, canonical, source, _) in SPECS.items():
        row = external.get(symbol, {})
        if (row.get("category"), row.get("provider_kind"), row.get("owner"), row.get("resolution_gate")) != (
                "c-runtime", KIND, *ownership(symbol)):
            fail(f"runtime override ownership drift: {symbol}")
        bindings = [r for r in defined if r["symbol"] == canonical and r["binding"] == "global"
                    and r["section_class"] == "text" and r["source"] == source]
        if len(bindings) != 1:
            fail(f"recovered canonical definition drift: {symbol}")
        binding = frontier.get(symbol, {}) if symbol == "abort" else contracts.get(symbol, {})
        key = "target_symbol" if symbol == "abort" else "canonical_symbol"
        if binding.get("resolution_kind") != "semantic-text-alias" or binding.get(key) != canonical:
            fail(f"recovered runtime binding drift: {symbol}")
        if symbol == "puts" and binding.get("target_address") != f"0x{address:08x}":
            fail("puts target address drift")
    return external


def validate_manifest(args: argparse.Namespace) -> tuple[list[dict[str, str]], list[dict[str, str]]]:
    external = live_contracts(args)
    rows = libgcc.read_table(args.manifest, FIELDS)
    witnesses = libgcc.read_table(args.witnesses, WITNESS_FIELDS)
    if [r["symbol"] for r in rows] != sorted(SPECS) or len(witnesses) != len(WITNESSES):
        fail("runtime override ledger must retain two providers and three witnesses")
    for row in rows:
        for key, value in fixed_provider(row["symbol"], external).items():
            if row[key] != value:
                fail(f"runtime override {row['symbol']} {key} drift")
        for key in ("target_sha256", "normalized_sha256", "linked_sha256"):
            if not libgcc.SHA_RE.fullmatch(row[key]):
                fail(f"runtime override missing {key}")
        if row["target_sha256"] != row["linked_sha256"]:
            fail("runtime override final linked bytes must equal target")
        with (ROOT / row["matching_evidence"]).open(encoding="utf-8", newline="") as stream:
            evidence = list(csv.DictReader(stream, delimiter="\t"))
        matches = [r for r in evidence if int(r["address"], 0) == int(row["target_address"], 0)]
        if (len(matches) != 1 or matches[0]["result"] != "MATCH"
                or matches[0]["differing_bytes"] != "0" or matches[0]["unknown_relocations"]
                or int(matches[0]["object_size"]) != int(row["extent_hex"], 0)):
            fail("runtime override immutable function evidence drift")
    for row, spec in zip(witnesses, WITNESSES):
        for key, value in fixed_witness(spec).items():
            if row[key] != value:
                fail(f"runtime witness {spec[0]} {key} drift")
        for key in ("text_sha256", "target_sha256", "normalized_sha256"):
            if not libgcc.SHA_RE.fullmatch(row[key]):
                fail(f"runtime witness missing {key}")
        if row["relocation_count"] != str({"unwind-dw2.o": 105, "unwind-dw2-fde.o": 103,
                                            "libc/terminate.o": 4}[spec[0]]):
            fail("runtime witness complete relocation count drift")
    return rows, witnesses


def target_span(raw: bytes, address: int, size: int) -> bytes:
    offset = address - libgcc.TARGET_BASE
    if offset < 0 or size <= 0 or size % 4 or offset + size > len(raw):
        fail("runtime span outside reference")
    return raw[offset:offset + size]


def linker_script() -> str:
    return """OUTPUT_ARCH(mips)
ENTRY(abort)
fioWrite = 0x0019d244;
SECTIONS {
  .text.abort 0x00107578 : { *(.text.abort) }
  .text.puts 0x0019e414 : { *(.text.puts) }
  /DISCARD/ : { *(.text) *(.data) *(.bss) *(.rodata*) *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) }
}
"""


def capture(args: argparse.Namespace) -> tuple[list[dict[str, str]], list[dict[str, str]], dict]:
    raw = libgcc.load_reference(args.reference)
    external = live_contracts(args)
    compiler = libgcc.resolve_tool(args.compiler)
    if (str(libgcc.run([str(compiler), "-dumpversion"])).strip(),
            str(libgcc.run([str(compiler), "-dumpmachine"])).strip()) != ("3.2.2", "ee"):
        fail("runtime overrides require historical EE GCC 3.2.2")
    archive, ar = libgcc.compiler_archive(str(compiler), None)
    args.build_dir.mkdir(parents=True, exist_ok=True)
    witnesses = []
    for spec in WITNESSES:
        member, span, address, size, contract, offsets = spec
        if member.startswith("libc/"):
            import runtime_members
            # First verify ALL frozen member text/input hashes. An arbitrary
            # object dropped into build/ is not acceptable caller evidence.
            member_args = runtime_members.parse_args(["verify", "--compiler", str(compiler),
                "--reference", str(args.reference), "--build-dir", str(args.member_build_dir)])
            frozen = runtime_members.validate_manifest(member_args)
            actual_rows, actual_objects, _ = runtime_members.capture(member_args)
            if (actual_rows, actual_objects) != frozen:
                fail("historical caller input/member fingerprints drifted")
            path = args.member_build_dir / "objects" / member
        else:
            path = args.build_dir / member
            path.write_bytes(libgcc.run([str(ar), "p", str(archive), member], binary=True))
        elf = ELFFile(path)
        symbol = span_symbol(elf, span)
        if symbol.size != size:
            fail(f"historical witness complete span changed: {member}")
        image = elf.symbol_bytes(symbol, size)
        masks = elf.relocation_masks(symbol, 4)
        target = target_span(raw, address, size)
        compare_text(image, masks, target, member)
        verify_named_calls(image, target, named_relocations(elf, symbol, contract), address,
                           contract, offsets, SPECS[contract][0])
        witnesses.append({**fixed_witness(spec), "text_sha256": digest(image),
            "target_sha256": digest(target), "normalized_sha256": digest(libgcc.normalize_target(image, masks)),
            "relocation_count": str(len(masks))})
    obj = args.build_dir / "runtime_overrides.o"
    libgcc.run([str(compiler), *FLAGS, "-c", str(SOURCE), "-o", str(obj)])
    script = args.build_dir / "runtime-overrides.proof.ld"
    script.write_text(linker_script(), encoding="utf-8")
    linked = args.build_dir / "runtime-overrides.proof.elf"
    ld = compiler.with_name(compiler.name[:-3] + "ld")
    libgcc.run([str(ld), "-EL", "-T", str(script), "-o", str(linked), str(obj)])
    candidate, executable = ELFFile(obj), ELFFile(linked)
    if any(s.size and s.name.startswith((".data", ".bss", ".rodata", ".sdata", ".sbss")) for s in candidate.sections):
        fail("runtime overrides unexpectedly emit data/storage")
    if any(s.size and s.name.startswith(".text") and s.name not in (".text.abort", ".text.puts") for s in candidate.sections):
        fail("runtime overrides unexpectedly emit additional code")
    if executable.file_type != 2 or any(s.section_index == 0 and s.name for s in executable.symbols):
        fail("isolated runtime proof did not link completely")
    rows = []
    for name, (address, size, _, _, _) in SPECS.items():
        symbol, final = unique_symbol(candidate, name), unique_symbol(executable, name)
        if (symbol.size, final.value, final.size) != (size, address, size):
            fail(f"runtime provider geometry drift: {name}")
        target = target_span(raw, address, size)
        image = candidate.symbol_bytes(symbol, size)
        masks = candidate.relocation_masks(symbol, 4)
        compare_text(image, masks, target, name)
        outgoing = named_relocations(candidate, symbol, "fioWrite")
        expected = [(0x44, 4, "fioWrite")] if name == "puts" else []
        if outgoing != expected:
            fail(f"runtime provider outgoing relocation drift: {name}")
        final_bytes = executable.symbol_bytes(final, size)
        if final_bytes != target:
            fail(f"runtime provider final relocated bytes differ: {name}")
        rows.append({**fixed_provider(name, external), "target_sha256": digest(target),
                     "normalized_sha256": digest(libgcc.normalize_target(image, masks)),
                     "linked_sha256": digest(final_bytes)})
    report = {**statistics(rows, witnesses), "reference_sha256": libgcc.TARGET_SHA256,
              "scope": "two exact target-selected runtime overrides; isolated proof ELF, not final application",
              "whole_archive_origin_proved": False, "replacement_elf": False}
    return rows, witnesses, report


def statistics(rows: Sequence[dict[str, str]], witnesses: Sequence[dict[str, str]]) -> dict:
    return {"contracts_closed": len(rows), "provider_bytes": sum(int(r["extent_hex"], 0) for r in rows),
            "incoming_named_calls": sum(int(r["incoming_calls"]) for r in rows),
            "witness_bytes": sum(int(r["extent_hex"], 0) for r in witnesses),
            "stage3d_closed": 7 + 1 + 43 + len(rows), "stage3d_total": 53}


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "verify", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--witnesses", type=Path, default=DEFAULT_WITNESSES)
    parser.add_argument("--external-map", type=Path, default=libgcc.DEFAULT_EXTERNAL)
    parser.add_argument("--contracts", type=Path, default=libgcc.DEFAULT_CONTRACTS)
    parser.add_argument("--frontier-manifest", type=Path, default=libgcc.DEFAULT_FRONTIER)
    parser.add_argument("--defined-map", type=Path, default=ROOT / "analysis/source_tree/defined_symbol_ownership.tsv")
    parser.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    parser.add_argument("--compiler", default="ee-gcc")
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--member-build-dir", type=Path, default=ROOT / "build/runtime-members")
    parser.add_argument("--report", type=Path, default=DEFAULT_BUILD / "report.json")
    args = parser.parse_args(argv)
    for key, value in vars(args).items():
        if isinstance(value, Path):
            setattr(args, key, value.expanduser().resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "validate":
            rows, witnesses = validate_manifest(args)
            report = statistics(rows, witnesses)
        else:
            frozen = validate_manifest(args) if args.command == "verify" else None
            rows, witnesses, report = capture(args)
            if frozen is not None and frozen != (rows, witnesses):
                fail("runtime override private fingerprints drifted")
            if args.command == "capture":
                import runtime_members
                args.manifest.write_text(runtime_members.render(FIELDS, rows), encoding="utf-8")
                args.witnesses.write_text(runtime_members.render(WITNESS_FIELDS, witnesses), encoding="utf-8")
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        print(f"verified target runtime overrides: contracts={report['contracts_closed']}/2 "
              f"linked_bytes={report['provider_bytes']} incoming_named_calls={report['incoming_named_calls']} "
              f"stage3d={report['stage3d_closed']}/53")
        return 0
    except (RuntimeOverrideError, libgcc.LibgccContractError, OSError, KeyError, ValueError) as error:
        print(f"runtime overrides: FAIL: {error}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
