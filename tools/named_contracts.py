#!/usr/bin/env python3
"""Close the historical 212-row Stage-3E named-contract tranche.

The original Stage-2 plan assigned 205 named link contracts and seven zlib
peers to Stage 3E.  This gate preserves that historical denominator while
distinguishing real target identities from source-lift artifacts:

* exact aliases to already matched target text;
* exact target-backed data ranges, fingerprinted without publishing bytes;
* two reviewed target entries and two external hardware/ROM addresses;
* one canonical data alias for the target's existing errno word; and
* source refactors for instruction, BIOS-inline and stack-pseudo symbols that
  never existed as global providers in the target.

The private link action also replaces every remaining Stage-3C/3E
compatibility store with overlap-aware target-byte sections.  Final virtual
placement and section ordering remain the Stage-3G linker-script gate.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import sys
from collections import Counter
from io import StringIO
from pathlib import Path
from typing import Iterable, Sequence

import named_data as stage3c
from provider_frontier import (
    ABSOLUTE_ANCHOR,
    COMPAT_STORAGE,
    FrontierError,
    RUNTIME_SHIM,
    SEMANTIC_ALIAS,
    render_runtime_c,
    render_storage_assembly,
    symbol_map,
    verify_input_frontier,
)
from source_aliases import (
    AliasError,
    alloc_section_fingerprints,
    global_symbols,
    resolve_tool,
    run,
    sibling_tool,
)


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_EXTERNAL = ROOT / "analysis" / "source_tree" / "external_symbol_ownership.tsv"
DEFAULT_DEFINED = ROOT / "analysis" / "source_tree" / "defined_symbol_ownership.tsv"
DEFAULT_CONTRACTS = ROOT / "analysis" / "link_identity" / "link_contracts.tsv"
DEFAULT_FRONTIER = ROOT / "analysis" / "link_identity" / "provider_frontier_closure.tsv"
DEFAULT_SOURCE_ALIASES = ROOT / "analysis" / "link_identity" / "source_address_aliases.tsv"
DEFAULT_LAYOUT = ROOT / "analysis" / "link_identity" / "unpacked_layout.json"
DEFAULT_STAGE3C = ROOT / "analysis" / "link_identity" / "named_data.tsv"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "named_contracts.tsv"
DEFAULT_REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
DEFAULT_INPUT = ROOT / "build" / "private-assets" / "source-tree.private-assets.partial.o"
DEFAULT_BUILD = ROOT / "build" / "named-contracts"
DEFAULT_OUTPUT = DEFAULT_BUILD / "source-tree.named-contracts.partial.o"
DEFAULT_REPORT = DEFAULT_BUILD / "report.json"

EXTERNAL_FIELDS = (
    "symbol", "category", "provider_kind", "owner", "resolution_gate", "requesters",
)
DEFINED_FIELDS = (
    "symbol", "binding", "section_class", "size_hex", "source", "object",
)
CONTRACT_FIELDS = stage3c.CONTRACT_FIELDS
FRONTIER_FIELDS = stage3c.FRONTIER_FIELDS
SOURCE_ALIAS_FIELDS = (
    "alias", "target_address", "status", "canonical_symbol", "evidence",
    "canonical_source", "canonical_object", "requesters", "detail",
)
MANIFEST_FIELDS = (
    "symbol", "category", "status", "target_address", "extent_hex", "region",
    "sha256", "canonical_symbol", "evidence", "requesters", "detail",
)

TEXT_ALIAS_PROVED = "TEXT_ALIAS_PROVED"
TARGET_RANGE_PROVED = "TARGET_RANGE_PROVED"
TARGET_ENTRY_PROVED = "TARGET_ENTRY_PROVED"
EXTERNAL_ADDRESS_PROVED = "EXTERNAL_ADDRESS_PROVED"
DATA_ALIAS_PROVED = "DATA_ALIAS_PROVED"
SOURCE_REFACTOR_CLOSED = "SOURCE_REFACTOR_CLOSED"
RANGE_STATUSES = {TARGET_RANGE_PROVED, DATA_ALIAS_PROVED}
VALID_STATUSES = {
    TEXT_ALIAS_PROVED,
    TARGET_RANGE_PROVED,
    TARGET_ENTRY_PROVED,
    EXTERNAL_ADDRESS_PROVED,
    DATA_ALIAS_PROVED,
    SOURCE_REFACTOR_CLOSED,
}
SHA_RE = re.compile(r"[0-9a-f]{64}")
ADDRESS_SUFFIX_RE = re.compile(r"(?:^|_)([0-9a-fA-F]{8})$")

TEXT_ENTRY_SYMBOLS = {
    "snes_p12_compare_sdd1_entries",
    "snes_p12_get_filename",
}
EXTERNAL_ADDRESS_SYMBOLS = {
    "REG_GS_CSR": "mmio",
    "cRam1fc7ff52": "bios-rom",
}

# Exact source-consumed spans for arrays/tables whose extent is not described
# by the scalar prefix.  These are deliberately range claims, not assertions
# about original C object names or the final linker grouping.
SPECIAL_EXTENTS = {
    "PTR_DAT_001bb31c": 0x8,
    "PTR_DAT_003f4918": 0x40,
    "PTR_DAT_003f4958": 0xC,
    "PTR_DAT_003f4bf8": 0x8,
    "PTR_LAB_0033594e": 0x2,
    "PTR_LAB_00343b30": 0x10,
    "PTR_LAB_00343b44": 0x10,
    "PTR_LAB_00411010": 0x400,
    "PTR_s_________________00344d30": 0xD8,
    "crc_table_001c8a80": 0x800,
    "fixed_td_00425870": 0x100,
    "fixed_tl_00424870": 0x1000,
    "s_THIS_SCRIPT_WAS_STOLEN_001b6ed0": 0x17,
    "snes_data_001b0000": 0x1C01,
    "snes_ram_003f4be8": 0x4,
    "snes_state_0035e2c4": 0x4,
    "switchdataD_001b18f8": 0x34,
    "switchdataD_001ba82c": 0x2A4,
    "switchdataD_001baad0": 0xF8,
    "target_video_mode_byte_001fc752": 0x1,
    "uRam0044e206": 0x2000,
    "uRam00450205": 0x1,
    "zr_base_dist_001ba130": 0x78,
    "zr_base_length_001ba0b8": 0x74,
    "zr_dist_code_001b9db8": 0x200,
    "zr_length_code_001b9fb8": 0x100,
    "zr_static_dtree_001b9d40": 0x78,
    "zr_static_ltree_001b98c0": 0x480,
}

# Historical Stage-3E names removed from the live source namespace.  Each
# token is checked in the current source so an accidental regression cannot
# silently resurrect compatibility providers.
CLOSED_SOURCE_REFACTORS = {
    "EI": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "#define EI()"),
    "SQRT": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "#define SQRT(value)"),
    "SUB_00002214": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "DAT_0034e2c4 + 0x2214"),
    "_auStack_10e4": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "(uint)auStack_10e4[0]"),
    "isinf": ("ps2/libgcc_runtime_recovered.o", "src/ps2/libgcc_runtime_recovered.c", "#define isinf(value)"),
    "isnan": ("ps2/libgcc_runtime_recovered.o", "src/ps2/libgcc_runtime_recovered.c", "#define isnan(value)"),
    "lrintf": ("ps2/progress11_frontend_recovered.o;snes9x/progress13_core_helpers_recovered.o", "src/ps2/progress11_frontend_recovered.c;src/snes9x/progress13_core_helpers_recovered.c", "#define lrintf(value)"),
    "ps2_add_intc_handler_recovered": ("ps2/gsdriver_recovered.o", "src/ps2/gsdriver_recovered.c", "#define ps2_add_intc_handler_recovered"),
    "ps2_bios_syscall_2_recovered": ("ps2/gsdriver_recovered.o", "src/ps2/gsdriver_recovered.c", "#define ps2_bios_syscall_2_recovered"),
    "ps2_disable_intc_recovered": ("ps2/gsdriver_recovered.o", "src/ps2/gsdriver_recovered.c", "#define ps2_disable_intc_recovered"),
    "ps2_enable_intc_recovered": ("ps2/gsdriver_recovered.o", "src/ps2/gsdriver_recovered.c", "#define ps2_enable_intc_recovered"),
    "ps2_gs_put_imr_recovered": ("ps2/gsdriver_recovered.o", "src/ps2/gsdriver_recovered.c", "#define ps2_gs_put_imr_recovered"),
    "ps2_remove_intc_handler_recovered": ("ps2/gsdriver_recovered.o", "src/ps2/gsdriver_recovered.c", "#define ps2_remove_intc_handler_recovered"),
    "stack0x00000000": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "stack_slots_00000000[2]"),
    "stack0xffffef20": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "DAT_003453b8 = local_10e2[2]"),
    "stack0xffffef90": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "DAT_0035bfe0 = auStack_1072[2]"),
    "stack0xffffef98": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "DAT_0035bfe8 = auStack_106a[2]"),
    "stack0xffffefa4": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "(uint)auStack_105f[3] << 0x18"),
    "syscall": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "#define syscall(ignored)"),
    "trap": ("ps2/progress28_structural_lift_recovered.o", "src/ps2/progress28_structural_lift_recovered.c", "#define trap(code)"),
}

ERRNO_ALIAS = {
    "symbol": "errno",
    "requesters": "ps2/newlib_mathfp_recovered.o",
    "target_address": "0x00425a70",
    "extent_hex": "0x4",
    "canonical_symbol": "ps2lib_errno_00425a70",
}


class NamedContractError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise NamedContractError(message)


def read_table(path: Path, fields: Sequence[str]) -> list[dict[str, str]]:
    if not path.is_file():
        fail(f"missing manifest input: {path}")
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != tuple(fields):
            fail(f"unexpected columns in {path}")
        return list(reader)


def unique(rows: Sequence[dict[str, str]], field: str, label: str) -> dict[str, dict[str, str]]:
    result: dict[str, dict[str, str]] = {}
    for row in rows:
        key = row[field]
        if key in result:
            fail(f"duplicate {label}: {key}")
        result[key] = row
    return result


def is_stage3e(row: dict[str, str]) -> bool:
    return (row["category"], row["provider_kind"]) in {
        ("named-external", "link-contract"),
        ("zlib-peer", "source-or-archive"),
    }


def range_extent(symbol: str, frontier: dict[str, str]) -> int:
    if frontier["resolution_kind"] == COMPAT_STORAGE:
        return int(frontier["storage_size_hex"], 0)
    if symbol in SPECIAL_EXTENTS:
        return SPECIAL_EXTENTS[symbol]
    if symbol.startswith("PTR_"):
        return 4
    if symbol.startswith(("bRam", "cRam")):
        return 1
    if symbol.startswith(("fRam", "iRam", "puRam")):
        return 4
    if symbol.startswith("uRam004477") or symbol.startswith("uRam0042d33"):
        return 1
    if symbol.startswith("uRam"):
        return 4
    fail(f"no reviewed Stage-3E range extent: {symbol}")
    raise AssertionError


def alias_address(
    canonical: str, source_alias_rows: Sequence[dict[str, str]]
) -> str:
    match = ADDRESS_SUFFIX_RE.search(canonical)
    if match is not None:
        return f"0x{int(match.group(1), 16):08x}"
    addresses = {
        row["target_address"]
        for row in source_alias_rows
        if row["status"] == "PROVED" and row["canonical_symbol"] == canonical
    }
    if len(addresses) != 1:
        fail(f"cannot prove unique target address for canonical alias: {canonical}")
    return next(iter(addresses))


def region_for(address: int, extent: int, layout: dict[str, int | str]) -> str:
    end = address + extent
    base = int(layout["base"])
    initialized_end = int(layout["initialized_end"])
    memory_end = int(layout["memory_end"])
    if address < base or end > memory_end:
        fail(f"Stage-3E range lies outside target memory: 0x{address:08x}+0x{extent:x}")
    if end <= initialized_end:
        return "initialized"
    if address >= initialized_end:
        return "zero-fill"
    return "initialized+zero-fill"


def verify_refactor_evidence() -> None:
    for symbol, (_requesters, paths, token) in CLOSED_SOURCE_REFACTORS.items():
        for relative in paths.split(";"):
            source = ROOT / relative
            if not source.is_file() or token not in source.read_text(encoding="utf-8"):
                fail(f"Stage-3E source-refactor evidence drift: {symbol} ({relative}#{token})")


def derive_rows(args: argparse.Namespace) -> tuple[list[dict[str, str]], dict[str, int | str]]:
    external_rows = read_table(args.external_map, EXTERNAL_FIELDS)
    stage3c.stage3_partition(external_rows)
    live = sorted((row for row in external_rows if is_stage3e(row)), key=lambda row: row["symbol"])
    if len(live) != 191:
        fail(f"expected 191 live Stage-3E contracts after source cleanup, found {len(live)}")

    contracts = unique(read_table(args.contracts, CONTRACT_FIELDS), "symbol", "link contract")
    frontier = unique(read_table(args.frontier_manifest, FRONTIER_FIELDS), "symbol", "provider row")
    defined_rows = read_table(args.defined_map, DEFINED_FIELDS)
    source_aliases = read_table(args.source_alias_manifest, SOURCE_ALIAS_FIELDS)
    layout = stage3c.load_layout(args.layout_manifest)
    verify_refactor_evidence()

    errno_definitions = [
        row for row in defined_rows
        if row["symbol"] == ERRNO_ALIAS["canonical_symbol"] and row["binding"] == "global"
    ]
    if len(errno_definitions) != 1 or errno_definitions[0]["size_hex"] != "0x4":
        fail("canonical target errno definition drift")

    result: list[dict[str, str]] = []
    for external in live:
        symbol = external["symbol"]
        contract = contracts.get(symbol)
        provider = frontier.get(symbol)
        if contract is None:
            fail(f"live Stage-3E symbol lacks link contract: {symbol}")

        address = ""
        extent = ""
        canonical = ""
        detail = ""
        if contract["status"] == "RESOLVED" and contract["resolution_kind"] == "semantic-text-alias":
            status = TEXT_ALIAS_PROVED
            address = contract["target_address"]
            canonical = contract["canonical_symbol"]
            region = "text"
            evidence = contract["evidence"]
            detail = contract["detail"]
        elif provider is not None and provider["resolution_kind"] == SEMANTIC_ALIAS:
            status = TEXT_ALIAS_PROVED
            canonical = provider["target_symbol"]
            address = alias_address(canonical, source_aliases)
            region = "text"
            evidence = provider["evidence"]
            detail = provider["detail"]
        elif provider is not None and provider["resolution_kind"] == ABSOLUTE_ANCHOR:
            address = provider["target_address"]
            if symbol in TEXT_ENTRY_SYMBOLS:
                status = TARGET_ENTRY_PROVED
                region = "text"
                evidence = "reviewed-exact-target-entry"
                detail = "signature-compatible source call bound to the exact target entry"
            elif symbol in EXTERNAL_ADDRESS_SYMBOLS:
                status = EXTERNAL_ADDRESS_PROVED
                region = EXTERNAL_ADDRESS_SYMBOLS[symbol]
                evidence = "reviewed-external-address"
                detail = "architectural address outside the unpacked image; no payload storage emitted"
            else:
                size = range_extent(symbol, provider)
                status = TARGET_RANGE_PROVED
                extent = f"0x{size:x}"
                region = region_for(int(address, 0), size, layout)
                evidence = "target-address+reviewed-consumed-extent"
                detail = "exact private-reference range; final placement remains Stage 3G"
        elif provider is not None and provider["resolution_kind"] == COMPAT_STORAGE:
            if not provider["target_address"]:
                fail(f"Stage-3E storage lacks target address: {symbol}")
            size = range_extent(symbol, provider)
            address = provider["target_address"]
            extent = f"0x{size:x}"
            status = TARGET_RANGE_PROVED
            region = region_for(int(address, 0), size, layout)
            evidence = "reviewed-target-storage+private-range"
            detail = "compatibility store replaced by an exact target-byte range provider"
        else:
            fail(f"unclosed live Stage-3E contract: {symbol}")

        result.append({
            "symbol": symbol,
            "category": external["category"],
            "status": status,
            "target_address": address,
            "extent_hex": extent,
            "region": region,
            "sha256": "",
            "canonical_symbol": canonical,
            "evidence": evidence,
            "requesters": external["requesters"],
            "detail": detail,
        })

    result.append({
        "symbol": ERRNO_ALIAS["symbol"],
        "category": "named-external",
        "status": DATA_ALIAS_PROVED,
        "target_address": ERRNO_ALIAS["target_address"],
        "extent_hex": ERRNO_ALIAS["extent_hex"],
        "region": region_for(int(ERRNO_ALIAS["target_address"], 0), 4, layout),
        "sha256": "",
        "canonical_symbol": ERRNO_ALIAS["canonical_symbol"],
        "evidence": "compiler-checked-canonical-data-definition",
        "requesters": ERRNO_ALIAS["requesters"],
        "detail": "compatibility errno name canonicalized to the existing target-addressed data owner",
    })

    for symbol, (requesters, paths, token) in CLOSED_SOURCE_REFACTORS.items():
        result.append({
            "symbol": symbol,
            "category": "named-external",
            "status": SOURCE_REFACTOR_CLOSED,
            "target_address": "",
            "extent_hex": "",
            "region": "source-refactor-closed",
            "sha256": "",
            "canonical_symbol": "",
            "evidence": "reviewed-target-absent-source-contract",
            "requesters": requesters,
            "detail": f"source-local instruction/stack contract; evidence={paths}#{token}",
        })

    result.sort(key=lambda row: row["symbol"])
    if len(result) != 212 or len({row["symbol"] for row in result}) != 212:
        fail("historical Stage-3E 212-row ledger drift")
    expected_counts = {
        TEXT_ALIAS_PROVED: 23,
        TARGET_RANGE_PROVED: 164,
        TARGET_ENTRY_PROVED: 2,
        EXTERNAL_ADDRESS_PROVED: 2,
        DATA_ALIAS_PROVED: 1,
        SOURCE_REFACTOR_CLOSED: 20,
    }
    if dict(Counter(row["status"] for row in result)) != expected_counts:
        fail(f"Stage-3E classification drift: {Counter(row['status'] for row in result)}")
    return result, layout


def render_tsv(rows: Iterable[dict[str, str]]) -> str:
    output = StringIO(newline="")
    writer = csv.DictWriter(output, fieldnames=MANIFEST_FIELDS, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    return output.getvalue()


def fingerprint_rows(
    rows: Sequence[dict[str, str]], reference_path: Path, layout: dict[str, int | str]
) -> list[dict[str, str]]:
    if not reference_path.is_file():
        fail(f"missing private unpacked reference: {reference_path}")
    reference = reference_path.read_bytes()
    if len(reference) != int(layout["initialized_size"]) or hashlib.sha256(reference).hexdigest() != layout["sha256"]:
        fail("private reference does not match the frozen layout oracle")
    result: list[dict[str, str]] = []
    for source in rows:
        row = dict(source)
        if row["status"] in RANGE_STATUSES:
            row["sha256"] = hashlib.sha256(stage3c.range_bytes(reference, row, layout)).hexdigest()
        result.append(row)
    return result


def validate_manifest(args: argparse.Namespace) -> tuple[list[dict[str, str]], dict[str, int | str]]:
    expected, layout = derive_rows(args)
    actual = read_table(args.manifest, MANIFEST_FIELDS)
    if len(actual) != 212:
        fail(f"named-contract manifest must contain 212 rows, found {len(actual)}")
    actual_by_symbol = unique(actual, "symbol", "named-contract row")
    if set(actual_by_symbol) != {row["symbol"] for row in expected}:
        fail("named-contract manifest symbol set drift")
    ordered: list[dict[str, str]] = []
    for base in expected:
        row = actual_by_symbol[base["symbol"]]
        for field in MANIFEST_FIELDS:
            if field == "sha256":
                continue
            if row[field] != base[field]:
                fail(f"named-contract metadata drift for {base['symbol']}: {field}")
        if row["status"] not in VALID_STATUSES:
            fail(f"invalid Stage-3E status: {row['symbol']}")
        if row["status"] in RANGE_STATUSES:
            if SHA_RE.fullmatch(row["sha256"]) is None:
                fail(f"Stage-3E target range lacks SHA-256: {row['symbol']}")
        elif row["sha256"]:
            fail(f"non-range Stage-3E row carries SHA-256: {row['symbol']}")
        ordered.append(row)
    return ordered, layout


def summarize(rows: Sequence[dict[str, str]]) -> dict[str, object]:
    counts = Counter(row["status"] for row in rows)
    return {
        "total": len(rows),
        "text_aliases": counts[TEXT_ALIAS_PROVED],
        "target_ranges": counts[TARGET_RANGE_PROVED],
        "target_entries": counts[TARGET_ENTRY_PROVED],
        "external_addresses": counts[EXTERNAL_ADDRESS_PROVED],
        "data_aliases": counts[DATA_ALIAS_PROVED],
        "source_refactors": counts[SOURCE_REFACTOR_CLOSED],
        "fingerprinted": sum(row["status"] in RANGE_STATUSES for row in rows),
        "zlib_peers": sum(row["category"] == "zlib-peer" for row in rows),
    }


def cluster_ranges(rows: Sequence[dict[str, str]]) -> list[dict[str, object]]:
    intervals = sorted(
        [
            (
            int(row["target_address"], 0),
            int(row["target_address"], 0) + int(row["extent_hex"], 0),
            row,
            )
            for row in rows
        ],
        key=lambda item: (item[0], item[1], item[2]["symbol"]),
    )
    clusters: list[dict[str, object]] = []
    for start, end, row in intervals:
        if not clusters or start > int(clusters[-1]["end"]):
            clusters.append({"start": start, "end": end, "rows": [row]})
        else:
            clusters[-1]["end"] = max(int(clusters[-1]["end"]), end)
            members = clusters[-1]["rows"]
            assert isinstance(members, list)
            members.append(row)
    return clusters


def render_exact_assembly(
    rows: Sequence[dict[str, str]], exported: set[str], reference: bytes,
    layout: dict[str, int | str], build_dir: Path,
) -> tuple[str, dict[str, dict[str, object]]]:
    lines = [
        "/* Generated from the private Stage-3 reference; do not commit. */",
        ".set noreorder",
        "",
    ]
    expected: dict[str, dict[str, object]] = {}
    for cluster in cluster_ranges(rows):
        start = int(cluster["start"])
        end = int(cluster["end"])
        size = end - start
        members = cluster["rows"]
        assert isinstance(members, list)
        zero_fill = start >= int(layout["initialized_end"])
        if not zero_fill and end > int(layout["initialized_end"]):
            fail(f"exact provider cluster crosses initialized/BSS boundary: 0x{start:08x}")
        section = f".bss.stage3ce.va_{start:08x}" if zero_fill else f".data.stage3ce.va_{start:08x}"
        base_symbol = f"__stage3ce_range_{start:08x}"
        if zero_fill:
            lines.extend((f'.section {section},"aw",@nobits', ".balign 1", f"{base_symbol}:", f".space 0x{size:x}"))
            digest = None
            section_type = 8
        else:
            material = stage3c.range_bytes(reference, {
                "symbol": base_symbol,
                "target_address": f"0x{start:x}",
                "extent_hex": f"0x{size:x}",
            }, layout)
            binary = build_dir / f"range_{start:08x}_{end:08x}.bin"
            binary.write_bytes(material)
            lines.extend((f'.section {section},"aw",@progbits', ".balign 1", f"{base_symbol}:", f'.incbin "{binary}"'))
            digest = hashlib.sha256(material).hexdigest()
            section_type = 1
        for row in members:
            if row["symbol"] not in exported:
                continue
            offset = int(row["target_address"], 0) - start
            extent = int(row["extent_hex"], 0)
            lines.extend((
                f".globl {row['symbol']}",
                f".type {row['symbol']}, @object",
                f"{row['symbol']} = {base_symbol} + 0x{offset:x}",
                f".size {row['symbol']}, 0x{extent:x}",
            ))
        lines.append("")
        expected[section] = {
            "type": section_type,
            "flags": 3,
            "size": size,
            "alignment": 1,
            "sha256": digest,
        }
    return "\n".join(lines), dict(sorted(expected.items()))


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def link_exact_providers(
    args: argparse.Namespace, rows: Sequence[dict[str, str]], layout: dict[str, int | str]
) -> dict[str, object]:
    if not args.input.is_file():
        fail(f"missing private-asset aggregate: {args.input}")
    reference = args.reference.read_bytes() if args.reference.is_file() else b""
    if len(reference) != int(layout["initialized_size"]) or hashlib.sha256(reference).hexdigest() != layout["sha256"]:
        fail("private reference does not match the frozen layout oracle")

    frontier_rows = read_table(args.frontier_manifest, FRONTIER_FIELDS)
    if len(frontier_rows) != 223:
        fail(f"expected 223 post-snprintf-refactor provider rows, found {len(frontier_rows)}")
    frontier_by_name = unique(frontier_rows, "symbol", "provider row")

    stage3c_rows = read_table(args.stage3c_manifest, stage3c.MANIFEST_FIELDS)
    stage3c_ranges = [row for row in stage3c_rows if row["status"] == stage3c.RANGE_PROVED]
    if len(stage3c_ranges) != 40:
        fail(f"expected 40 exact non-private Stage-3C ranges, found {len(stage3c_ranges)}")
    for row in stage3c_ranges:
        if hashlib.sha256(stage3c.range_bytes(reference, row, layout)).hexdigest() != row["sha256"]:
            fail(f"private Stage-3C fingerprint mismatch: {row['symbol']}")
    stage3c_replacements = {
        row["symbol"] for row in stage3c_ranges
        if row["symbol"] in frontier_by_name
        and frontier_by_name[row["symbol"]]["resolution_kind"] == COMPAT_STORAGE
    }
    if len(stage3c_replacements) != 32:
        fail(f"expected 32 Stage-3C compatibility replacements, found {len(stage3c_replacements)}")

    stage3e_ranges = [row for row in rows if row["status"] == TARGET_RANGE_PROVED]
    stage3e_replacements = {row["symbol"] for row in stage3e_ranges}
    if len(stage3e_replacements) != 164 or not stage3e_replacements <= set(frontier_by_name):
        fail("Stage-3E exact provider replacement set drift")
    replacements = stage3c_replacements | stage3e_replacements
    if len(replacements) != 196:
        fail("combined Stage-3C/3E provider replacement set drift")

    exact_ranges = [*stage3c_ranges, *stage3e_ranges]
    remaining_frontier = [row for row in frontier_rows if row["symbol"] not in replacements]
    if any(row["resolution_kind"] == COMPAT_STORAGE for row in remaining_frontier):
        fail("Stage-3E link still needs compatibility storage")

    compiler = resolve_tool(args.compiler)
    assembler = sibling_tool(compiler, args.as_tool, "as")
    linker = sibling_tool(compiler, args.ld, "ld")
    nm = sibling_tool(compiler, args.nm, "nm")
    args.build_dir.mkdir(parents=True, exist_ok=True)

    exact_source = args.build_dir / "exact_stage3ce_data.S"
    exact_object = args.build_dir / "exact_stage3ce_data.o"
    storage_source = args.build_dir / "remaining_compatibility_storage.S"
    storage_object = args.build_dir / "remaining_compatibility_storage.o"
    runtime_source = args.build_dir / "remaining_runtime_shims.c"
    runtime_object = args.build_dir / "remaining_runtime_shims.o"
    provider_object = args.build_dir / "stage3e_providers.o"

    exact_assembly, expected_sections = render_exact_assembly(
        exact_ranges, replacements, reference, layout, args.build_dir,
    )
    exact_source.write_text(exact_assembly, encoding="utf-8")
    storage_source.write_text(render_storage_assembly(remaining_frontier), encoding="utf-8")
    runtime_source.write_text(render_runtime_c(), encoding="utf-8")
    run([str(assembler), "-EL", "-o", str(exact_object), str(exact_source)])
    run([str(assembler), "-EL", "-o", str(storage_object), str(storage_source)])
    run([
        str(compiler), "-G0", "-O2", "-EL", "-fomit-frame-pointer", "-fno-common",
        "-ffreestanding", "-fno-builtin", "-fshort-double", "-mlong64",
        "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
        "-ffunction-sections", "-w", "-c", str(runtime_source), "-o", str(runtime_object),
    ])
    run([str(linker), "-EL", "-r", "-o", str(provider_object), str(exact_object), str(storage_object), str(runtime_object)])

    actual_sections = {
        name: value for name, value in alloc_section_fingerprints(exact_object).items()
        if value["size"] != 0
    }
    if actual_sections != expected_sections:
        fail("assembled Stage-3C/3E exact sections differ from private fingerprints")

    provider_by_name = symbol_map(global_symbols(nm, provider_object))
    expected_defined = replacements | {
        row["symbol"] for row in remaining_frontier
        if row["resolution_kind"] in {COMPAT_STORAGE, RUNTIME_SHIM}
    }
    provider_defined = {name for name, symbol in provider_by_name.items() if not symbol.undefined}
    provider_undefined = {name for name, symbol in provider_by_name.items() if symbol.undefined}
    if provider_defined != expected_defined or provider_undefined:
        fail(
            "Stage-3E generated provider symbol drift; "
            f"missing={sorted(expected_defined-provider_defined)[:5]} "
            f"extra={sorted(provider_defined-expected_defined)[:5]} "
            f"undefined={sorted(provider_undefined)}"
        )

    input_symbols = global_symbols(nm, args.input)
    input_undefined = verify_input_frontier(input_symbols, frontier_rows)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    map_path = args.output.with_suffix(".map")
    command = [str(linker), "-EL", "-r", "-Map", str(map_path)]
    for row in remaining_frontier:
        if row["resolution_kind"] == ABSOLUTE_ANCHOR:
            command.extend(["--defsym", f"{row['symbol']}={row['target_address']}"])
        elif row["resolution_kind"] == SEMANTIC_ALIAS:
            command.extend(["--defsym", f"{row['symbol']}={row['target_symbol']}"])
    command.extend(["-o", str(args.output), str(args.input), str(provider_object)])
    run(command)

    output_symbols = global_symbols(nm, args.output)
    output_undefined = {symbol.name for symbol in output_symbols if symbol.undefined}
    if output_undefined:
        fail(f"Stage-3E aggregate still has externals: {sorted(output_undefined)[:10]}")
    input_sections = alloc_section_fingerprints(args.input)
    output_sections = alloc_section_fingerprints(args.output)
    for name, fingerprint in input_sections.items():
        if output_sections.get(name) != fingerprint:
            fail(f"existing allocated section changed during Stage 3E: {name}")
    added = {
        name: value for name, value in output_sections.items()
        if name not in input_sections and value["size"] != 0
    }
    provider_sections = {
        name: value for name, value in alloc_section_fingerprints(provider_object).items()
        if value["size"] != 0
    }
    if added != provider_sections:
        fail("Stage-3E aggregate did not add exactly the generated provider sections")

    summary = summarize(rows)
    report: dict[str, object] = {
        "schema": 1,
        "claim": "stage3e-exact-named-contract-closure",
        **summary,
        "stage3c_exact_replacements": len(stage3c_replacements),
        "stage3e_exact_replacements": len(stage3e_replacements),
        "combined_exact_replacements": len(replacements),
        "exact_range_clusters": len(expected_sections),
        "exact_range_bytes": sum(int(section["size"]) for section in expected_sections.values()),
        "compatibility_storage_before": 39,
        "compatibility_storage_after": 0,
        "remaining_runtime_shims": sum(row["resolution_kind"] == RUNTIME_SHIM for row in remaining_frontier),
        "input_external_symbols": len(input_undefined),
        "output_external_symbols": len(output_undefined),
        "existing_allocated_sections_unchanged": True,
        "exact_sections": expected_sections,
        "input_sha256": sha256_file(args.input),
        "provider_object_sha256": sha256_file(provider_object),
        "output_sha256": sha256_file(args.output),
        "manifest_sha256": sha256_file(args.manifest),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return report


def add_public_args(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--external-map", type=Path, default=DEFAULT_EXTERNAL)
    parser.add_argument("--defined-map", type=Path, default=DEFAULT_DEFINED)
    parser.add_argument("--contracts", type=Path, default=DEFAULT_CONTRACTS)
    parser.add_argument("--frontier-manifest", type=Path, default=DEFAULT_FRONTIER)
    parser.add_argument("--source-alias-manifest", type=Path, default=DEFAULT_SOURCE_ALIASES)
    parser.add_argument("--layout-manifest", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="action", required=True)
    for action in ("validate", "refresh", "verify", "link"):
        sub = subparsers.add_parser(action)
        add_public_args(sub)
        if action in {"refresh", "verify", "link"}:
            sub.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
        if action in {"verify", "link"}:
            sub.add_argument("--report", type=Path, default=DEFAULT_REPORT)
        if action == "link":
            sub.add_argument("--stage3c-manifest", type=Path, default=DEFAULT_STAGE3C)
            sub.add_argument("--compiler", default="ee-gcc")
            sub.add_argument("--as", dest="as_tool")
            sub.add_argument("--ld")
            sub.add_argument("--nm")
            sub.add_argument("--input", type=Path, default=DEFAULT_INPUT)
            sub.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
            sub.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args(argv)
    for name, value in vars(args).items():
        if isinstance(value, Path) and not value.is_absolute():
            setattr(args, name, (ROOT / value).resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    try:
        args = parse_args(argv)
        if args.action == "refresh":
            rows, layout = derive_rows(args)
            rows = fingerprint_rows(rows, args.reference, layout)
            args.manifest.parent.mkdir(parents=True, exist_ok=True)
            args.manifest.write_text(render_tsv(rows), encoding="utf-8")
            action = "refreshed"
        else:
            rows, layout = validate_manifest(args)
            action = "validated"
            if args.action in {"verify", "link"}:
                recomputed = fingerprint_rows(rows, args.reference, layout)
                for actual, expected in zip(rows, recomputed):
                    if actual["sha256"] != expected["sha256"]:
                        fail(f"private Stage-3E fingerprint mismatch: {actual['symbol']}")
                if args.action == "link":
                    report = link_exact_providers(args, rows, layout)
                    action = "linked"
                else:
                    report = {"schema": 1, "claim": "stage3e-named-contract-oracle", **summarize(rows)}
                    args.report.parent.mkdir(parents=True, exist_ok=True)
                    args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
                    action = "verified"
    except (
        NamedContractError,
        stage3c.NamedDataError,
        FrontierError,
        AliasError,
        json.JSONDecodeError,
    ) as exc:
        print(f"named-contract gate failed: {exc}", file=sys.stderr)
        return 1

    summary = summarize(rows)
    print(
        f"{action} Stage-3E named contracts: total={summary['total']} "
        f"aliases={summary['text_aliases']} ranges={summary['target_ranges']} "
        f"entries={summary['target_entries']} external={summary['external_addresses']} "
        f"data_aliases={summary['data_aliases']} refactors={summary['source_refactors']}"
    )
    if args.action == "link":
        print(
            f"exact Stage-3C/3E providers: {report['combined_exact_replacements']} names in "
            f"{report['exact_range_clusters']} clusters; compatibility storage "
            f"{report['compatibility_storage_before']} -> {report['compatibility_storage_after']}"
        )
        print(f"partial-link externals: {report['input_external_symbols']} -> {report['output_external_symbols']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
