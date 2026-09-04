#!/usr/bin/env python3
"""Bind unnamed addresses to proved storage without claiming full C objects.

Reuse the exact Stage-3C/E and private-asset sections, materialize only the
uncovered minimum-access ranges, and rebind address anchors *inside* their
output sections.  An isolated link probe proves pointer and HI16/LO16 values;
it is not executable emulator code or a replacement ELF.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from collections import Counter
from pathlib import Path
from typing import Sequence

import libgcc_contracts as libgcc
import named_contracts as stage3e
import named_data as stage3c
import private_asset_providers as assets
import runtime_members
import runtime_overrides
import unnamed_data as accesses
import historical_data
import recovered_literals
import rom_offsets
import runtime_code_pointers
import pcm_buffer_consumed_extent
import runtime_residual_identities
import final_residual_identities
from compare_elf_functions import ELFFile, Symbol
from source_aliases import alloc_section_fingerprints, resolve_tool, run, sibling_tool

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/data_backing.tsv"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_BUILD = ROOT / "build/data-backing"
BACKED = "SECTION_BACKED_ADDRESS"
UNBACKED = "NO_PROVED_BACKING"
NEW = "stage3f-access"
HISTORICAL = "historical-source-exact"
RECOVERED = recovered_literals.ORIGIN
CLAIM = "section-backed address only; complete C object/array extent unproved"
UNBACKED_CLAIM = "absolute address only; storage and complete object/array extent unproved"
FIELDS = ("symbol", "target_address", "status", "section", "section_offset_hex",
          "access_extent_hex", "coverage_kind", "requesters", "claim")
SECTION_FIELDS = ("section", "target_address", "extent_hex", "origin", "region",
                  "alignment_hex", "sha256", "evidence_sha256")
SECTION_RE = re.compile(r"\.(?:data|bss)\.[A-Za-z0-9_.]+")
SYMBOL_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
POINTER_SECTION = ".data.stage3f.address_probe"
CODE_SECTION = ".text.stage3f.address_probe"
POINTER_BASE = 0x00500000
CODE_BASE = 0x00510000


class DataBackingError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise DataBackingError(message)


def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def evidence_hash(rows: Sequence[dict]) -> str:
    return digest(json.dumps(list(rows), sort_keys=True, separators=(",", ":")).encode())


def merge_ranges(ranges: Sequence[tuple[int, int]]) -> list[tuple[int, int]]:
    result: list[tuple[int, int]] = []
    for start, end in sorted(ranges):
        if start >= end:
            fail("empty or negative range")
        if result and start <= result[-1][1]:
            result[-1] = (result[-1][0], max(end, result[-1][1]))
        else:
            result.append((start, end))
    return result


def subtract_ranges(ranges: Sequence[tuple[int, int]], covered: Sequence[tuple[int, int]]) -> list[tuple[int, int]]:
    result = []
    for start, end in merge_ranges(ranges):
        cursor = start
        for left, right in merge_ranges(covered):
            if right <= cursor or left >= end:
                continue
            if left > cursor:
                result.append((cursor, left))
            cursor = max(cursor, right)
            if cursor >= end:
                break
        if cursor < end:
            result.append((cursor, end))
    return result


def interval(row: dict[str, str]) -> tuple[int, int]:
    start = int(row["target_address"], 0)
    return start, start + int(row["extent_hex"], 0)


def no_overlap(sections: Sequence[dict[str, str]]) -> None:
    if len({r["section"] for r in sections}) != len(sections):
        fail("duplicate backing section")
    end = -1
    for row in sorted(sections, key=interval):
        start, stop = interval(row)
        if start < end or stop <= start:
            fail("overlapping or empty backing sections")
        end = stop


def section_row(name: str, start: int, end: int, origin: str, alignment: int,
                evidence: Sequence[dict], layout: dict) -> dict[str, str]:
    if not SECTION_RE.fullmatch(name):
        fail("unsafe backing section name")
    if not int(layout["base"]) <= start < end <= int(layout["memory_end"]):
        fail("backing outside target memory")
    initialized_end = int(layout["initialized_end"])
    if start < initialized_end < end:
        fail("backing crosses initialized/BSS boundary")
    if alignment not in (1, 4) or start % alignment:
        fail("invalid backing alignment")
    region = "initialized" if end <= initialized_end else "zero-fill"
    if name.startswith(".bss.") != (region == "zero-fill"):
        fail("backing section kind disagrees with memory region")
    return {"section": name, "target_address": f"0x{start:08x}", "extent_hex": hex(end-start),
            "origin": origin, "region": region, "alignment_hex": hex(alignment),
            "sha256": "", "evidence_sha256": evidence_hash(evidence)}


def access_args(args: argparse.Namespace, command: str = "validate") -> argparse.Namespace:
    return accesses.parse_args([command, "--manifest", str(args.access_manifest),
                               "--reference", str(args.reference)])


def derive(args: argparse.Namespace) -> tuple[list[dict[str, str]], list[dict[str, str]]]:
    rom_offsets.validate()
    runtime_code_pointers.validate()
    pcm_buffer_consumed_extent.validate()
    runtime_residual_identities.validate()
    final_residual_identities.validate()
    historical = historical_data.validate()
    c_rows, layout = stage3c.validate_manifest(stage3c.parse_args(["validate"]))
    e_rows, e_layout = stage3e.validate_manifest(stage3e.parse_args(["validate"]))
    if layout != e_layout:
        fail("Stage-3C/E layout mismatch")
    asset_rows, _ = assets.validate_frozen_manifest(assets.parse_args(["validate"]))
    access_rows = accesses.validate_manifest(access_args(args))
    ranges = ([r for r in c_rows if r["status"] == stage3c.RANGE_PROVED]
              + [r for r in e_rows if r["status"] == stage3e.TARGET_RANGE_PROVED])
    sections = []
    for cluster in stage3e.cluster_ranges(ranges):
        start, end = int(cluster["start"]), int(cluster["end"])
        prefix = ".bss" if start >= int(layout["initialized_end"]) else ".data"
        sections.append(section_row(f"{prefix}.stage3ce.va_{start:08x}", start, end,
                                    "stage3ce", 1, cluster["rows"], layout))
    for row in asset_rows:
        start = int(row["target_va"], 0)
        end = int(row["size_word_va"], 0) + 4
        sections.append(section_row(row["section_name"], start, end, "private-asset", 4, [row], layout))
    no_overlap(sections)
    for owner in historical["owners"]:
        first, last = owner["address"], owner["address"] + owner["size"]
        for start, end in subtract_ranges([(first, last)], [interval(r) for r in sections]):
            sections.append(section_row(f".data.stage3f.history.va_{start:08x}", start, end,
                                        HISTORICAL, 1, [owner], layout))
    no_overlap(sections)
    recovered = recovered_literals.validate()
    for owner in recovered["owners"]:
        first, last = owner["address"], owner["address"] + owner["size"]
        for start, end in subtract_ranges([(first, last)], [interval(r) for r in sections]):
            sections.append(section_row(f".data.stage3f.recovered.va_{start:08x}", start, end,
                                        RECOVERED, 1, [owner], layout))
    no_overlap(sections)
    proved = [r for r in access_rows if r["status"] == accesses.PROVED]
    uncovered = subtract_ranges([interval(r) for r in proved], [interval(r) for r in sections])
    for start, end in uncovered:
        witnesses = [r for r in proved if interval(r)[0] < end and start < interval(r)[1]]
        prefix = ".bss" if start >= int(layout["initialized_end"]) else ".data"
        sections.append(section_row(f"{prefix}.stage3f.va_{start:08x}", start, end,
                                    NEW, 1, witnesses, layout))
    sections.sort(key=lambda r: (int(r["target_address"], 0), r["section"]))
    no_overlap(sections)
    rows = []
    for source in access_rows:
        name = source["symbol"]
        if not SYMBOL_RE.fullmatch(name):
            fail("unsafe data alias name")
        address = int(source["target_address"], 0)
        if name in final_residual_identities.SPEC:
            spec = final_residual_identities.SPEC[name]
            if address != spec["target_address"]:
                fail("final residual identity target address drift")
            rows.append({**{key: "" for key in FIELDS}, "symbol": name,
                         "target_address": source["target_address"],
                         "status": spec["status"],
                         "requesters": source["requesters"],
                         "claim": spec["claim"]})
            continue
        if name in runtime_residual_identities.SPEC:
            spec = runtime_residual_identities.SPEC[name]
            if address != spec["target_address"]:
                fail("runtime residual identity target address drift")
            rows.append({**{key: "" for key in FIELDS}, "symbol": name,
                         "target_address": source["target_address"],
                         "status": spec["status"],
                         "requesters": source["requesters"],
                         "claim": spec["claim"]})
            continue
        if name in pcm_buffer_consumed_extent.SYMBOL_TO_OWNER:
            owner = pcm_buffer_consumed_extent.SYMBOL_TO_OWNER[name]
            spec = pcm_buffer_consumed_extent.OWNER_SPEC[owner]
            expected = spec["base"] + 2 * spec["aliases"].index(name)
            if address != expected:
                fail("pcm buffer minimum-extent target address drift")
            rows.append({**{key: "" for key in FIELDS}, "symbol": name,
                         "target_address": source["target_address"],
                         "status": pcm_buffer_consumed_extent.STATUS,
                         "requesters": source["requesters"],
                         "claim": pcm_buffer_consumed_extent.CLAIM})
            continue
        if name in runtime_code_pointers.SPEC:
            proof = runtime_code_pointers.SPEC[name]
            if address != proof["target"]:
                fail("runtime code pointer target address drift")
            rows.append({**{key: "" for key in FIELDS}, "symbol": name,
                         "target_address": source["target_address"],
                         "status": runtime_code_pointers.STATUS,
                         "requesters": source["requesters"],
                         "claim": runtime_code_pointers.CLAIM})
            continue
        if source["status"] == rom_offsets.STATUS:
            # A numerical overlap with initialized RAM is NOT an identity:
            # these 29 names were offsets into a separately loaded ROM.
            rows.append({**{key: "" for key in FIELDS}, "symbol": name,
                         "target_address": source["target_address"], "status": rom_offsets.STATUS,
                         "requesters": source["requesters"], "claim": rom_offsets.CLAIM})
            continue
        if source["status"] == accesses.CODE_ALIAS:
            # This historical LAB_ label is an exact callable entry point.
            # Source-alias proof owns the identity; do not manufacture data.
            rows.append({**{key: "" for key in FIELDS}, "symbol": name,
                         "target_address": source["target_address"], "status": accesses.CODE_ALIAS,
                         "requesters": source["requesters"], "claim": accesses.CODE_ALIAS_CLAIM})
            continue
        owners = [r for r in sections if interval(r)[0] <= address < interval(r)[1]]
        if len(owners) > 1:
            fail("ambiguous backing owner")
        row = {**{key: "" for key in FIELDS}, "symbol": name,
               "target_address": source["target_address"], "status": UNBACKED,
               "requesters": source["requesters"], "claim": UNBACKED_CLAIM}
        if owners:
            section = owners[0]
            direct = source["status"] == accesses.PROVED
            if direct and subtract_ranges([interval(source)], [interval(r) for r in sections]):
                fail("direct access not completely covered")
            row.update({"status": BACKED, "section": section["section"],
                        "section_offset_hex": hex(address-int(section["target_address"], 0)),
                        "access_extent_hex": source["extent_hex"] if direct else "",
                        "coverage_kind": "direct-access" if direct else
                            "historical-source-owner" if section["origin"] == HISTORICAL else
                            "recovered-source-literal" if section["origin"] == RECOVERED else "interior-address-only",
                        "claim": CLAIM})
        elif source["status"] == accesses.PROVED:
            fail("proved access lacks storage")
        rows.append(row)
    return rows, sections


def fingerprint(raw: bytes, row: dict[str, str]) -> str:
    start, end = interval(row)
    material = bytes(end-start) if row["region"] == "zero-fill" else raw[start-libgcc.TARGET_BASE:end-libgcc.TARGET_BASE]
    if len(material) != end-start:
        fail("truncated private backing range")
    return digest(material)


def validate(args: argparse.Namespace) -> tuple[list[dict[str, str]], list[dict[str, str]]]:
    rows, sections = derive(args)
    actual = libgcc.read_table(args.manifest, FIELDS)
    frozen = libgcc.read_table(args.sections, SECTION_FIELDS)
    if actual != rows:
        fail("data-backing address roster, ownership or claim drift")
    if len(frozen) != len(sections):
        fail("data-backing section roster drift")
    for expected, found in zip(sections, frozen):
        if any(found[key] != expected[key] for key in SECTION_FIELDS if key != "sha256"):
            fail("data-backing section geometry or evidence drift")
        if not libgcc.SHA_RE.fullmatch(found["sha256"]):
            fail("backing section lacks SHA-256")
    return actual, frozen


def verify_reference(args: argparse.Namespace, sections: Sequence[dict[str, str]]) -> bytes:
    raw = libgcc.load_reference(args.reference)
    for row in sections:
        if fingerprint(raw, row) != row["sha256"]:
            fail(f"private backing fingerprint mismatch: {row['section']}")
    historical_data.verify_reference(args.reference, historical_data.validate())
    recovered_literals.verify_reference(args.reference)
    rom_offsets.verify_reference(args.reference)
    runtime_code_pointers.verify_reference(args.reference)
    pcm_buffer_consumed_extent.verify_reference(args.reference)
    runtime_residual_identities.verify_reference(args.reference)
    final_residual_identities.verify_reference(args.reference)
    if accesses.capture(access_args(args, "verify")) != accesses.validate_manifest(access_args(args)):
        fail("private access proofs changed")
    return raw


def _statistics_before_runtime_code_pointers(rows: Sequence[dict[str, str]], sections: Sequence[dict[str, str]]) -> dict:
    backed = [r for r in rows if r["status"] == BACKED]
    new = [r for r in sections if r["origin"] in (NEW, HISTORICAL, RECOVERED)]
    rom_closed = sum(r["status"] == rom_offsets.STATUS for r in rows)
    code_closed = sum(r["status"] == accesses.CODE_ALIAS for r in rows)
    closed = rom_closed + code_closed
    return {"contracts_total": len(rows), "section_backed_addresses": len(backed),
            "unbacked_addresses": len(rows)-len(backed)-closed,
            "rom_offset_refactors_closed": rom_closed,
            "code_pointer_source_aliases_closed": code_closed,
            "resolved_contracts": len(backed)+closed,
            "historical_source_sections": sum(r["origin"] == HISTORICAL for r in sections),
            "historical_source_bytes": sum(int(r["extent_hex"], 0) for r in sections if r["origin"] == HISTORICAL),
            "recovered_source_sections": sum(r["origin"] == RECOVERED for r in sections),
            "recovered_source_bytes": sum(int(r["extent_hex"], 0) for r in sections if r["origin"] == RECOVERED),
            "coverage_kinds": dict(sorted(Counter(r["coverage_kind"] for r in backed).items())),
            "reused_sections": len(sections)-len(new), "new_sections": len(new),
            "new_backing_bytes": sum(int(r["extent_hex"], 0) for r in new),
            "new_initialized_bytes": sum(int(r["extent_hex"], 0) for r in new if r["region"] == "initialized"),
            "new_zero_fill_bytes": sum(int(r["extent_hex"], 0) for r in new if r["region"] == "zero-fill"),
            "total_backing_bytes": sum(int(r["extent_hex"], 0) for r in sections),
            "complete_object_extents_proved": False, "stage3f_closed": False, "replacement_elf": False}


def _statistics_before_pcm_buffer_consumed_extent(rows: Sequence[dict[str, str]], sections: Sequence[dict[str, str]]) -> dict:
    result = _statistics_before_runtime_code_pointers(rows, sections)
    runtime_closed = sum(r["status"] == runtime_code_pointers.STATUS for r in rows)
    result["runtime_code_pointer_refactors_closed"] = runtime_closed
    result["unbacked_addresses"] = sum(r["status"] == UNBACKED for r in rows)
    result["resolved_contracts"] = len(rows) - result["unbacked_addresses"]
    return result

def _statistics_before_runtime_residual_identities(rows: Sequence[dict[str, str]], sections: Sequence[dict[str, str]]) -> dict:
    result = _statistics_before_pcm_buffer_consumed_extent(rows, sections)
    pcm_closed = sum(r["status"] == pcm_buffer_consumed_extent.STATUS for r in rows)
    result["pcm_buffer_minimum_extents_closed"] = pcm_closed
    result["unbacked_addresses"] = sum(r["status"] == UNBACKED for r in rows)
    result["resolved_contracts"] = len(rows) - result["unbacked_addresses"]
    return result

def _statistics_before_final_residual_identities(rows: Sequence[dict[str, str]], sections: Sequence[dict[str, str]]) -> dict:
    result = _statistics_before_runtime_residual_identities(rows, sections)
    result["runtime_member_data_objects_closed"] = sum(
        r["status"] == runtime_residual_identities.DATA_STATUS for r in rows
    )
    result["runtime_internal_code_labels_closed"] = sum(
        r["status"] == runtime_residual_identities.CODE_STATUS for r in rows
    )
    result["unbacked_addresses"] = sum(r["status"] == UNBACKED for r in rows)
    result["resolved_contracts"] = len(rows) - result["unbacked_addresses"]
    return result

def statistics(rows: Sequence[dict[str, str]], sections: Sequence[dict[str, str]]) -> dict:
    result = _statistics_before_final_residual_identities(rows, sections)
    result["historical_object_interior_fields_closed"] = sum(
        r["status"] == final_residual_identities.RTC_STATUS for r in rows
    )
    result["target_native_eh_frame_relocation_bytes_closed"] = sum(
        r["status"] == final_residual_identities.EH_STATUS for r in rows
    )
    result["unbacked_addresses"] = sum(r["status"] == UNBACKED for r in rows)
    result["resolved_contracts"] = len(rows) - result["unbacked_addresses"]
    return result

def expected_section(row: dict[str, str]) -> dict:
    return {"type": 8 if row["region"] == "zero-fill" else 1, "flags": 3,
            "size": int(row["extent_hex"], 0), "alignment": int(row["alignment_hex"], 0),
            "sha256": None if row["region"] == "zero-fill" else row["sha256"]}


def check_sections(path: Path, sections: Sequence[dict[str, str]]) -> None:
    found = alloc_section_fingerprints(path)
    for row in sections:
        if found.get(row["section"]) != expected_section(row):
            fail(f"allocated backing section mismatch: {row['section']}")


def global_map(elf: ELFFile, file_type: int = 1) -> dict[str, Symbol]:
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, file_type):
        fail("expected little-endian MIPS ELF32 with the required file type")
    rows = [s for s in elf.symbols if s.info >> 4 in (1, 2)]
    result = {s.name: s for s in rows}
    if len(result) != len(rows) or any(s.section_index == 0 for s in rows):
        fail("duplicate or undefined global symbols")
    return result


def symbol_identity(elf: ELFFile, symbol: Symbol) -> tuple:
    index = symbol.section_index
    section = elf.sections[index].name if 0 < index < len(elf.sections) else index
    return symbol.value, symbol.size, section, symbol.info


def check_input(elf: ELFFile, rows: Sequence[dict[str, str]]) -> dict[str, Symbol]:
    symbols = global_map(elf)
    for row in rows:
        symbol = symbols.get(row["symbol"])
        if row["status"] == rom_offsets.STATUS:
            if symbol is not None:
                fail("ROM offset must not have an image-address anchor")
            continue
        if row["status"] == accesses.CODE_ALIAS:
            if (symbol is None or not 0 < symbol.section_index < len(elf.sections)
                    or not elf.sections[symbol.section_index].name.startswith(".text")):
                fail("code-pointer alias must resolve to recovered text, not an absolute data anchor")
            continue
        if (symbol is None or symbol.section_index != 0xFFF1 or symbol.size != 0
                or symbol.value != int(row["target_address"], 0)):
            fail(f"input address anchor drift: {row['symbol']}")
    return symbols


def check_output(before: ELFFile, after: ELFFile, rows: Sequence[dict[str, str]]) -> None:
    source, result = check_input(before, rows), global_map(after)
    if set(source) != set(result):
        fail("global symbol roster changed")
    backed = {r["symbol"]: r for r in rows if r["status"] == BACKED}
    for name, original in source.items():
        if name not in backed:
            if symbol_identity(before, original) != symbol_identity(after, result[name]):
                fail(f"unrelated or unbacked global changed: {name}")
        else:
            row = backed[name]
            expected = (int(row["section_offset_hex"], 0), 0, row["section"], original.info)
            if symbol_identity(after, result[name]) != expected:
                fail(f"alias is not a zero-sized section-relative address: {name}")


def reference_roster(elf: ELFFile, names: set[str]) -> list[tuple[str, int, int, str]]:
    result = []
    for section in elf.sections:
        if section.type in (4, 9) or not section.size:
            continue
        span = Symbol(section.name, 0, section.size, section.index, 0)
        for offset, kind, name in runtime_overrides.named_relocations(elf, span):
            if name in names:
                result.append((section.name, offset, kind, name))
    return sorted(result)


def source_backing_payloads(sections: Sequence[dict[str, str]], owners: Sequence[dict],
                            payloads: dict[tuple[str, str], bytes]) -> dict[str, bytes]:
    """Slice only rebuilt provider bytes, never fall back to reference extraction."""
    result = {}
    for row in sections:
        if row["origin"] != HISTORICAL:
            continue
        start, end = interval(row)
        found = [r for r in owners if r["address"] <= start and end <= r["address"]+r["size"]]
        if len(found) != 1:
            fail("historical backing lacks one complete source provider")
        owner = found[0]
        payload = payloads.get((owner["unit"], owner["symbol"]))
        if payload is None or len(payload) != owner["size"] or digest(payload) != owner["sha256"]:
            fail("rebuilt historical provider is absent or changed")
        offset = start-owner["address"]
        piece = payload[offset:offset+end-start]
        if digest(piece) != row["sha256"]:
            fail("rebuilt source bytes differ from the frozen backing interval")
        result[row["section"]] = piece
    return result


def rebuild_source_backing(args: argparse.Namespace, sections: Sequence[dict[str, str]]) -> dict[str, Path]:
    frozen, payloads = historical_data.validate(), {}
    build = args.build_dir / "historical-source"
    settings = historical_data.parse_args(["build", "--compiler", str(args.historical_compiler),
                                           "--reference", str(args.reference), "--build-dir", str(build)])
    if historical_data.build_manifest(settings, payloads) != frozen:
        fail("fresh source rebuild differs from the historical data proof")
    pieces = source_backing_payloads(sections, frozen["owners"], payloads)
    pieces.update(recovered_literals.source_backing_payloads(sections))
    paths = {}
    for index, (section, payload) in enumerate(sorted(pieces.items())):
        path = build / f"provider-{index:04d}.bin"
        path.write_bytes(payload)
        paths[section] = path
    return paths


def render_additions(reference: Path, sections: Sequence[dict[str, str]],
                     source_payloads: dict[str, Path] | None = None) -> str:
    quoted = assets.quote_assembly_path(reference.resolve())
    lines = ["/* Rebuilt historical/recovered source plus private minimum-access backing. */", ".set noreorder"]
    for row in sections:
        if row["origin"] not in (NEW, HISTORICAL, RECOVERED):
            continue
        size = int(row["extent_hex"], 0)
        kind = "nobits" if row["region"] == "zero-fill" else "progbits"
        lines.extend([f'.section {row["section"]},"aw",@{kind}', ".balign 1"])
        if row["origin"] in (HISTORICAL, RECOVERED):
            if source_payloads is None or row["section"] not in source_payloads or kind != "progbits":
                fail("source-backed bytes must come from fresh source rebuild or recovered source payload")
            path = assets.quote_assembly_path(source_payloads[row["section"]].resolve())
            lines.append(f'.incbin "{path}",0,{size}')
            continue
        lines.append(f".space {size}" if kind == "nobits" else
                     f'.incbin "{quoted}",{int(row["target_address"], 0)-libgcc.TARGET_BASE},{size}')
    return "\n".join(lines) + "\n"


def render_rebind(rows: Sequence[dict[str, str]], sections: Sequence[dict[str, str]]) -> str:
    lines = ["/* Assign INSIDE output sections: --defsym can collapse offsets to ABS. */", "SECTIONS {"]
    for section in sections:
        name = section["section"]
        lines.append(f"  {name} 0 : {{")
        for row in rows:
            if row["status"] == BACKED and row["section"] == name:
                lines.append(f"    {row['symbol']} = . + {row['section_offset_hex']};")
        lines.extend([f"    KEEP(*({name}))", "  }"])
    lines.append("}")
    return "\n".join(lines) + "\n"


def render_probe(rows: Sequence[dict[str, str]]) -> str:
    backed = [r for r in rows if r["status"] == BACKED]
    lines = ["/* Synthetic address probe, NOT emulator code. */", ".set noreorder",
             f'.section {POINTER_SECTION},"aw",@progbits', ".balign 4"]
    lines.extend(f".word {r['symbol']}" for r in backed)
    lines.extend([f'.section {CODE_SECTION},"ax",@progbits', ".balign 4"])
    for row in backed:
        lines.extend([f"lui $2,%hi({row['symbol']})", f"addiu $2,$2,%lo({row['symbol']})"])
    return "\n".join(lines) + "\n"


def render_probe_layout(sections: Sequence[dict[str, str]]) -> str:
    lines = ["/* Isolated data placement only, never a replacement ELF. */", "SECTIONS {"]
    for row in sections:
        lines.append(f"  {row['section']} {row['target_address']} : {{ KEEP(*({row['section']})) }}")
    lines.extend([f"  {POINTER_SECTION} 0x{POINTER_BASE:08x} : {{ KEEP(*({POINTER_SECTION})) }}",
                  f"  {CODE_SECTION} 0x{CODE_BASE:08x} : {{ KEEP(*({CODE_SECTION})) }}",
                  "  /DISCARD/ : { *(.reginfo) *(.mdebug*) *(.comment) *(.pdr) }", "}"])
    return "\n".join(lines) + "\n"


def section_bytes(elf: ELFFile, name: str) -> bytes:
    rows = [s for s in elf.sections if s.name == name]
    if len(rows) != 1:
        fail(f"missing/duplicate ELF section: {name}")
    section = rows[0]
    return elf.data[section.offset:section.offset + section.size]


def check_probe_relocations(elf: ELFFile, rows: Sequence[dict[str, str]]) -> None:
    backed = [r for r in rows if r["status"] == BACKED]
    names = {r["symbol"] for r in backed}
    imports = {s.name for s in elf.symbols if s.info >> 4 in (1, 2) and s.section_index == 0}
    if imports != names:
        fail("synthetic probe import roster drift")
    expected = []
    for index, row in enumerate(backed):
        expected.extend([(POINTER_SECTION, index*4, 2, row["symbol"]),
                         (CODE_SECTION, index*8, 5, row["symbol"]),
                         (CODE_SECTION, index*8+4, 6, row["symbol"])])
    if reference_roster(elf, names) != sorted(expected):
        fail("synthetic pointer/HI16/LO16 relocation drift")
    if section_bytes(elf, POINTER_SECTION) != bytes(len(backed)*4):
        fail("synthetic pointer addends must be zero")
    if section_bytes(elf, CODE_SECTION) != struct.pack("<II", 0x3C020000, 0x24420000)*len(backed):
        fail("synthetic HI16/LO16 instructions or addends drift")


def check_final_probe(elf: ELFFile, rows: Sequence[dict[str, str]], sections: Sequence[dict[str, str]]) -> dict:
    symbols = global_map(elf, 2)
    if any(s.type in (4, 9) and s.size for s in elf.sections):
        fail("unapplied relocations in final address probe")
    expected_addresses = {r["section"]: int(r["target_address"], 0) for r in sections}
    expected_addresses.update({POINTER_SECTION: POINTER_BASE, CODE_SECTION: CODE_BASE})
    for section in elf.sections:
        if section.name in expected_addresses and section.address != expected_addresses[section.name]:
            fail("final probe section placement drift")
    pointers, code = section_bytes(elf, POINTER_SECTION), section_bytes(elf, CODE_SECTION)
    backed = [r for r in rows if r["status"] == BACKED]
    if len(pointers) != 4*len(backed) or len(code) != 8*len(backed):
        fail("final probe geometry drift")
    for index, row in enumerate(backed):
        name, address = row["symbol"], int(row["target_address"], 0)
        symbol = symbols.get(name)
        if (symbol is None or symbol.size or symbol.value != address
                or not 0 < symbol.section_index < len(elf.sections)
                or elf.sections[symbol.section_index].name != row["section"]):
            fail("final probe lost section-backed identity")
        pointer = struct.unpack_from("<I", pointers, index*4)[0]
        hi, lo = struct.unpack_from("<II", code, index*8)
        decoded = ((hi & 65535) << 16) + accesses.signed16(lo & 65535)
        if hi >> 16 != 0x3C02 or lo >> 16 != 0x2442 or pointer != address or decoded != address:
            fail(f"final relocated address mismatch: {name}")
    return {"relocations_proved": len(backed)*3, "exact_pointer_values": len(backed),
            "exact_hi16_lo16_pairs": len(backed)}


def link(args: argparse.Namespace, rows: Sequence[dict[str, str]], sections: Sequence[dict[str, str]]) -> dict:
    verify_reference(args, sections)
    before = ELFFile(args.input)
    check_input(before, rows)
    old = [r for r in sections if r["origin"] not in (NEW, HISTORICAL, RECOVERED)]
    new = [r for r in sections if r["origin"] in (NEW, HISTORICAL, RECOVERED)]
    check_sections(args.input, old)
    original_sections = alloc_section_fingerprints(args.input)
    if set(original_sections) & {r["section"] for r in new}:
        fail("new backing already present in input")
    compiler = resolve_tool(args.compiler)
    assembler = sibling_tool(compiler, None, "as")
    linker = sibling_tool(compiler, None, "ld")
    objcopy = sibling_tool(compiler, None, "objcopy")
    args.build_dir.mkdir(parents=True, exist_ok=True)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    source_payloads = rebuild_source_backing(args, sections)
    additions = args.build_dir / "minimum_access_backing.S"
    addition_object = additions.with_suffix(".o")
    script = args.build_dir / "section_relative_aliases.ld"
    additions.write_text(render_additions(args.reference, sections, source_payloads), encoding="utf-8")
    script.write_text(render_rebind(rows, sections), encoding="utf-8")
    run([str(assembler), "-EL", "-o", str(addition_object), str(additions)])
    check_sections(addition_object, new)
    run([str(linker), "-EL", "-r", "-T", str(script), "-o", str(args.output), str(args.input), str(addition_object)])
    after = ELFFile(args.output)
    check_output(before, after, rows)
    check_sections(args.output, sections)
    final_sections = alloc_section_fingerprints(args.output)
    expected_sections = {**original_sections, **{r["section"]: expected_section(r) for r in new}}
    if final_sections != expected_sections:
        fail("pre-existing allocated sections changed or unexpected sections added")
    names = {r["symbol"] for r in rows if r["status"] == BACKED}
    roster = reference_roster(before, names)
    if roster != reference_roster(after, names) or {r[3] for r in roster} != names:
        fail("source relocation roster changed or backed alias has no requester")
    data_only = args.build_dir / "proved_backing_only.o"
    command = [str(objcopy)]
    for row in sections:
        command.extend(["-j", row["section"]])
    run(command + [str(args.output), str(data_only)])
    check_sections(data_only, sections)
    probe_source = args.build_dir / "address_probe.S"
    probe_object = args.build_dir / "address_probe.o"
    probe_source.write_text(render_probe(rows), encoding="utf-8")
    run([str(assembler), "-EL", "-o", str(probe_object), str(probe_source)])
    check_probe_relocations(ELFFile(probe_object), rows)
    combined = args.build_dir / "address_probe.partial.o"
    run([str(linker), "-EL", "-r", "-o", str(combined), str(data_only), str(probe_object)])
    placement = args.build_dir / "address_probe.ld"
    placement.write_text(render_probe_layout(sections), encoding="utf-8")
    probe = args.build_dir / "address_probe.elf"
    run([str(linker), "-EL", "-T", str(placement), "-o", str(probe), str(combined)])
    check_sections(probe, sections)
    nonempty = {k for k, v in alloc_section_fingerprints(probe).items() if v["size"]}
    if nonempty != {r["section"] for r in sections} | {POINTER_SECTION, CODE_SECTION}:
        fail("address probe includes unexpected application sections")
    return {**statistics(rows, sections), **check_final_probe(ELFFile(probe), rows, sections),
            "input_sha256": digest(args.input.read_bytes()), "output_sha256": digest(args.output.read_bytes()),
            "existing_allocated_sections_unchanged": True, "global_symbol_roster_unchanged": True,
            "unbacked_anchors_unchanged": True, "source_relocations_preserved": len(roster),
            "historical_backing_from_fresh_source": True,
            "historical_source_rebuilt_bytes": sum(int(r["extent_hex"], 0) for r in new if r["origin"] == HISTORICAL),
            "recovered_source_rebuilt_bytes": sum(int(r["extent_hex"], 0) for r in new if r["origin"] == RECOVERED),
            "recovered_bytes_extracted_from_reference": 0,
            "historical_bytes_extracted_from_reference": 0,
            "source_relocation_types": dict(sorted(Counter(str(r[2]) for r in roster).items())),
            "undefined_globals_before": 0, "undefined_globals_after": 0,
            "probe_scope": "data placement and synthetic address relocations only; not emulator code"}


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "capture", "verify", "link"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--sections", type=Path, default=DEFAULT_SECTIONS)
    parser.add_argument("--access-manifest", type=Path, default=accesses.DEFAULT_MANIFEST)
    parser.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    parser.add_argument("--compiler", default="ee-gcc")
    parser.add_argument("--historical-compiler", type=Path, default=historical_data.DEFAULT_COMPILER)
    parser.add_argument("--input", type=Path, default=stage3e.DEFAULT_OUTPUT)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--output", type=Path, default=DEFAULT_BUILD / "source-tree.data-backed.partial.o")
    parser.add_argument("--report", type=Path, default=DEFAULT_BUILD / "report.json")
    args = parser.parse_args(argv)
    for key, value in vars(args).items():
        if isinstance(value, Path):
            setattr(args, key, value.expanduser().resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "capture":
            rows, sections = derive(args)
            raw = libgcc.load_reference(args.reference)
            for row in sections:
                row["sha256"] = fingerprint(raw, row)
            verify_reference(args, sections)
            args.manifest.write_text(runtime_members.render(FIELDS, rows), encoding="utf-8")
            args.sections.write_text(runtime_members.render(SECTION_FIELDS, sections), encoding="utf-8")
        rows, sections = validate(args)
        report = statistics(rows, sections)
        if args.command == "link":
            report = link(args, rows, sections)
        elif args.command == "verify":
            verify_reference(args, sections)
        if args.command != "validate":
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        print(f"verified section-backed addresses: {report['section_backed_addresses']}/{report['contracts_total']} "
              f"unbacked={report['unbacked_addresses']} new_bytes={report['new_backing_bytes']} "
              "(address backing only; full object extents and replacement ELF unproved)")
        if args.command == "link":
            print(f"source relocations preserved: {report['source_relocations_preserved']}; "
                  f"isolated address relocations proved: {report['relocations_proved']}")
        return 0
    except (DataBackingError, libgcc.LibgccContractError, stage3c.NamedDataError,
            stage3e.NamedContractError, accesses.UnnamedDataError, assets.ProviderError,
            historical_data.HistoricalDataError, recovered_literals.RecoveredLiteralError, rom_offsets.ROMOffsetError,
            runtime_overrides.RuntimeOverrideError, OSError, ValueError, KeyError) as error:
        print(f"data backing: FAIL -- {error}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
