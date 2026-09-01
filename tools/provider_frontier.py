#!/usr/bin/env python3
"""Close the post-snprintf-refactor 223-name source-link provider frontier.

This checkpoint has a deliberately narrow claim: the complete recovered EE
source aggregate can be partially linked with no undefined global symbols.
It combines four audited mechanisms:

* target addresses already encoded in low-level lifted names;
* reviewed aliases to recovered global text definitions;
* typed compatibility storage for source-model globals.

The shims and compatibility storage are generated below ignored ``build/``.
They close the source-link namespace, but they do not claim original data
initializers, historical archive membership, final section placement, object
order, or a replacement ELF.
"""
from __future__ import annotations

import argparse
import csv
import json
import re
import sys
from collections import Counter
from io import StringIO
from pathlib import Path
from typing import Iterable, Sequence

from source_aliases import (
    AliasError,
    NmSymbol,
    alloc_section_fingerprints,
    global_symbols,
    read_table,
    resolve_tool,
    run,
    sha256_file,
    sibling_tool,
)


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_CONTRACTS = ROOT / "analysis" / "link_identity" / "link_contracts.tsv"
DEFAULT_PRIVATE = ROOT / "analysis" / "link_identity" / "private_asset_providers.tsv"
DEFAULT_DEFINED = ROOT / "analysis" / "source_tree" / "defined_symbol_ownership.tsv"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "provider_frontier_closure.tsv"
DEFAULT_INPUT = ROOT / "build" / "private-assets" / "source-tree.private-assets.partial.o"
DEFAULT_BUILD = ROOT / "build" / "provider-frontier"
DEFAULT_OUTPUT = DEFAULT_BUILD / "source-tree.provider-closed.partial.o"
DEFAULT_REPORT = DEFAULT_BUILD / "report.json"

CONTRACT_FIELDS = (
    "symbol",
    "target_address",
    "status",
    "resolution_kind",
    "canonical_symbol",
    "evidence",
    "category",
    "provider_kind",
    "canonical_source",
    "canonical_object",
    "requesters",
    "detail",
)
PRIVATE_FIELDS = (
    "asset",
    "data_symbol",
    "size_symbol",
    "target_va",
    "size_hex",
    "sha256",
    "size_word_va",
    "padding_hex",
    "section_name",
    "section_alignment_hex",
    "requesters",
)
DEFINED_FIELDS = (
    "symbol",
    "binding",
    "section_class",
    "size_hex",
    "source",
    "object",
)
MANIFEST_FIELDS = (
    "symbol",
    "category",
    "provider_kind",
    "resolution_kind",
    "target_address",
    "target_symbol",
    "storage_size_hex",
    "evidence",
    "requesters",
    "detail",
)

BLOCKED = "BLOCKED"
ABSOLUTE_ANCHOR = "absolute-target-anchor"
SEMANTIC_ALIAS = "semantic-text-alias"
COMPAT_STORAGE = "compatibility-storage"
RUNTIME_SHIM = "compatibility-runtime-shim"

SUFFIX_ADDRESS_RE = re.compile(r"(?:^|_)([0-9a-fA-F]{8})$")
RAM_ADDRESS_RE = re.compile(r"(?:[bcfiu]|pu)Ram([0-9a-fA-F]{8})$")
STACK_ADDRESS_RE = re.compile(r"stack0x([0-9a-fA-F]{8})$")


class FrontierError(RuntimeError):
    """A frozen provider-frontier invariant failed."""


def fail(message: str) -> None:
    raise FrontierError(message)


# Reviewed aliases whose target identities are already independently frozen in
# analysis/progress_targets.csv and the Stage-2 defined-symbol ownership map.
SEMANTIC_ALIASES = {
    "S9xSync_AudioStep": "snes_dispatch_00176578",
    "S9xSync_SetVolume": "SjPCM_Setvol_001078f8",
    "abort": "snes_fatal_spin_00107578",
    "operator_new_u32": "snes_p13_operator_new",
    "snes_p12_fio_close": "fioClose_0019d090",
    "snes_p12_fio_open": "fioOpen_0019cfc0",
    "snes_p12_fio_read": "fioRead_0019d120",
    "snes_p12_fio_write": "fioWrite_0019d244",
    "snes_p12_qsort": "snes_qsort_001080cc",
}

# Two low-level adapters call exact target entries for which the buildable
# source model does not export a signature-compatible canonical definition.
# REG_GS_CSR is the documented PS2 privileged GS register, not image storage.
EXPLICIT_ANCHORS = {
    "REG_GS_CSR": 0x12001000,
    "snes_p12_compare_sdd1_entries": 0x0016FAC4,
    "snes_p12_get_filename": 0x00101924,
}

# Real storage required by the buildable behavioral source.  Sizes are the
# narrowest complete types/arrays consumed by the requesting sources.  Initial
# target bytes and final placement intentionally remain later data-layout gates.
STORAGE_SIZES = {
    "IPPU_FrameSync": 0x1C,
    "Memory_ROMFramesPerSecond": 0x4,
    "S9xSync_AudioActive": 0x1,
    "S9xSync_AudioArg0": 0x4,
    "S9xSync_AudioArg1": 0x4,
    "S9xSync_AutoFrameLatch": 0x4,
    "S9xSync_PeriodCounter": 0x8,
    "S9xSync_PeriodPrevious": 0x4,
    "S9xSync_PeriodValue": 0x4,
    "Settings_SkipFrames": 0x4,
    "g_Settings_blob": 0x148,
    "g_bg_bitshift": 0x4,
    "g_bg_blob": 0x30,
    "g_direct_colour_maps": 0x1000,
    "g_direct_colour_maps_need_rebuild": 0x1,
    "g_even_high": 0x100,
    "g_even_low": 0x100,
    "g_gfx_blob": 0x50,
    "g_head_mask": 0x10,
    "g_loaded_rom_path": 0x800,
    "g_memory_card_available": 0x4,
    "g_memory_vram": 0x4,
    "g_odd_high": 0x100,
    "g_odd_low": 0x100,
    "g_p12_memory": 0x1B078,
    "g_p12_sdd1_dat_extension": 0x5,
    "g_screen_colors": 0x200,
    "g_selected_rom_path": 0x400,
    "g_settings_54": 0x1,
    "g_settings_64": 0x1,
    "g_settings_66": 0x1,
    "g_shrink_workspace_recovered": 0x6000,
    "g_special_load_flag": 0x1,
    "g_special_save_enable": 0x1,
    "g_sram": 0x4,
    "g_sram_initial_value": 0x1,
    "g_sram_size": 0x1,
    "g_tail_mask": 0x14,
    "g_tile_cache_state": 0x34,
}

RUNTIME_SHIMS: set[str] = set()

KNOWN_DATA_ADDRESSES = {
    "IPPU_FrameSync": 0x0035C268,
    "Memory_ROMFramesPerSecond": 0x003592FC,
    "Settings_SkipFrames": 0x0034551C,
    "g_Settings_blob": 0x003454E0,
    "g_bg_bitshift": 0x0035D454,
    "g_bg_blob": 0x0035D450,
    "g_direct_colour_maps": 0x003F2F80,
    "g_direct_colour_maps_need_rebuild": 0x0035C26F,
    "g_even_high": 0x0035FBA0,
    "g_even_low": 0x0035FCA0,
    "g_gfx_blob": 0x0035D480,
    "g_head_mask": 0x003F4040,
    "g_loaded_rom_path": 0x0035B328,
    "g_memory_vram": 0x0034E2B8,
    "g_odd_high": 0x0035F9A0,
    "g_odd_low": 0x0035FAA0,
    "g_p12_memory": 0x0034E2B0,
    "g_p12_sdd1_dat_extension": 0x001B8050,
    "g_screen_colors": 0x0035CEB0,
    "g_selected_rom_path": 0x00428180,
    "g_settings_54": 0x00345534,
    "g_settings_64": 0x00345544,
    "g_settings_66": 0x00345546,
    "g_special_load_flag": 0x00345544,
    "g_special_save_enable": 0x0035B328,
    "g_sram": 0x0034E298,
    "g_sram_initial_value": 0x0035B73C,
    "g_sram_size": 0x0034E2D4,
    "g_tail_mask": 0x003F4050,
    "g_tile_cache_state": 0x0035C268,
    "S9xSync_AudioActive": 0x001EBD40,
    "S9xSync_AudioArg0": 0x001EBAE4,
    "S9xSync_AudioArg1": 0x001EBAE0,
    "S9xSync_AutoFrameLatch": 0x001BAF18,
    "S9xSync_PeriodCounter": 0x001BBD48,
    "S9xSync_PeriodPrevious": 0x001BBD50,
    "S9xSync_PeriodValue": 0x001BBD54,
}


def encoded_address(symbol: str) -> int | None:
    for pattern in (SUFFIX_ADDRESS_RE, RAM_ADDRESS_RE, STACK_ADDRESS_RE):
        match = pattern.search(symbol)
        if match is not None:
            return int(match.group(1), 16)
    return None


def render_tsv(rows: Iterable[dict[str, str]]) -> str:
    output = StringIO(newline="")
    writer = csv.DictWriter(
        output, fieldnames=MANIFEST_FIELDS, delimiter="\t", lineterminator="\n"
    )
    writer.writeheader()
    writer.writerows(rows)
    return output.getvalue()


def derive_rows(
    contract_rows: Sequence[dict[str, str]],
    private_rows: Sequence[dict[str, str]],
    defined_rows: Sequence[dict[str, str]],
) -> list[dict[str, str]]:
    private_names = {
        name
        for row in private_rows
        for name in (row["data_symbol"], row["size_symbol"])
    }
    active = {
        row["symbol"]: row
        for row in contract_rows
        if row["status"] == BLOCKED and row["symbol"] not in private_names
    }
    if len(active) != 223:
        fail(f"expected exact post-snprintf-refactor frontier of 223 symbols, found {len(active)}")

    canonical_text = {
        row["symbol"]
        for row in defined_rows
        if row["binding"] == "global" and row["section_class"] == "text"
    }
    missing_canonical = sorted(set(SEMANTIC_ALIASES.values()) - canonical_text)
    if missing_canonical:
        fail(f"semantic canonical text drift: {missing_canonical}")

    classified = (
        set(SEMANTIC_ALIASES)
        | set(EXPLICIT_ANCHORS)
        | set(STORAGE_SIZES)
        | set(RUNTIME_SHIMS)
    )
    unknown_explicit = sorted(classified - set(active))
    if unknown_explicit:
        fail(f"reviewed provider map contains inactive symbols: {unknown_explicit}")

    rows: list[dict[str, str]] = []
    for symbol, contract in sorted(active.items()):
        address = encoded_address(symbol)
        target_symbol = ""
        storage_size = ""
        detail = ""
        if address is not None:
            kind = ABSOLUTE_ANCHOR
            evidence = "address-encoded-low-level-name"
            detail = "zero-byte address contract retained from the structural lift"
        elif symbol in EXPLICIT_ANCHORS:
            kind = ABSOLUTE_ANCHOR
            address = EXPLICIT_ANCHORS[symbol]
            evidence = "reviewed-target-or-hardware-address"
            detail = "reviewed low-level address binding; no storage bytes emitted"
        elif symbol in SEMANTIC_ALIASES:
            kind = SEMANTIC_ALIAS
            target_symbol = SEMANTIC_ALIASES[symbol]
            evidence = "reviewed-existing-global-text"
            detail = "source-model contract bound to an already recovered target entry"
        elif symbol in STORAGE_SIZES:
            kind = COMPAT_STORAGE
            address = KNOWN_DATA_ADDRESSES.get(symbol)
            storage_size = f"0x{STORAGE_SIZES[symbol]:x}"
            evidence = "requester-type-and-minimum-complete-extent"
            detail = (
                "zero-initialized compatibility storage only; exact initializer and "
                "final placement remain data-layout gates"
            )
        elif symbol in RUNTIME_SHIMS:
            kind = RUNTIME_SHIM
            evidence = "deterministic-ee-source-model-shim"
            detail = (
                "linkable compatibility implementation; historical archive-member "
                "identity is not claimed"
            )
        else:
            fail(f"unclassified V86 provider contract: {symbol}")

        rows.append(
            {
                "symbol": symbol,
                "category": contract["category"],
                "provider_kind": contract["provider_kind"],
                "resolution_kind": kind,
                "target_address": "" if address is None else f"0x{address:08x}",
                "target_symbol": target_symbol,
                "storage_size_hex": storage_size,
                "evidence": evidence,
                "requesters": contract["requesters"],
                "detail": detail,
            }
        )

    counts = Counter(row["resolution_kind"] for row in rows)
    expected = {
        ABSOLUTE_ANCHOR: 175,
        SEMANTIC_ALIAS: 9,
        COMPAT_STORAGE: 39,
    }
    if dict(counts) != expected:
        fail(f"provider classification count drift: {dict(counts)} != {expected}")
    return rows


def expected_manifest(args: argparse.Namespace) -> tuple[list[dict[str, str]], str]:
    contracts = read_table(args.contracts, CONTRACT_FIELDS, delimiter="\t")
    private = read_table(args.private_manifest, PRIVATE_FIELDS, delimiter="\t")
    defined = read_table(args.defined_map, DEFINED_FIELDS, delimiter="\t")
    rows = derive_rows(contracts, private, defined)
    return rows, render_tsv(rows)


def validate_frozen_manifest(args: argparse.Namespace) -> list[dict[str, str]]:
    rows, expected = expected_manifest(args)
    if not args.manifest.is_file():
        fail(f"missing provider-frontier manifest: {args.manifest}")
    if args.manifest.read_text(encoding="utf-8") != expected:
        fail("provider-frontier manifest is stale; review inputs and run refresh")
    return rows


def summarize(rows: Sequence[dict[str, str]]) -> dict[str, int]:
    counts = Counter(row["resolution_kind"] for row in rows)
    return {
        "frontier_total": len(rows),
        "absolute_anchors": counts[ABSOLUTE_ANCHOR],
        "semantic_aliases": counts[SEMANTIC_ALIAS],
        "compatibility_storage_symbols": counts[COMPAT_STORAGE],
        "runtime_shims": counts[RUNTIME_SHIM],
        "compatibility_storage_bytes": sum(
            int(row["storage_size_hex"], 0)
            for row in rows
            if row["resolution_kind"] == COMPAT_STORAGE
        ),
    }


def render_storage_assembly(rows: Sequence[dict[str, str]]) -> str:
    lines = [
        "/* Generated by tools/provider_frontier.py; ignored build product. */",
        '.section .bss.stage3.compatibility,"aw",@nobits',
        ".balign 16",
    ]
    for row in rows:
        if row["resolution_kind"] != COMPAT_STORAGE:
            continue
        symbol = row["symbol"]
        size = int(row["storage_size_hex"], 0)
        lines.extend(
            (
                f".globl {symbol}",
                f".type {symbol},@object",
                f"{symbol}:",
                f".space {size}",
                f".size {symbol},{size}",
                ".balign 4",
            )
        )
    return "\n".join(lines) + "\n"


def render_runtime_c() -> str:
    return "/* No Stage-3 compatibility runtime shims remain. */\n"


def symbol_map(symbols: Sequence[NmSymbol]) -> dict[str, NmSymbol]:
    result: dict[str, NmSymbol] = {}
    for symbol in symbols:
        if symbol.name in result:
            fail(f"duplicate global symbol: {symbol.name}")
        result[symbol.name] = symbol
    return result


def verify_input_frontier(
    input_symbols: Sequence[NmSymbol], rows: Sequence[dict[str, str]]
) -> set[str]:
    expected = {row["symbol"] for row in rows}
    actual = {symbol.name for symbol in input_symbols if symbol.undefined}
    if actual != expected:
        fail(
            "V86 aggregate/provider manifest drift; "
            f"missing={sorted(expected - actual)[:5]} extra={sorted(actual - expected)[:5]}"
        )
    return actual


def link_frontier(args: argparse.Namespace, rows: Sequence[dict[str, str]]) -> dict[str, object]:
    if not args.input.is_file():
        fail(f"missing V86 private-asset aggregate: {args.input}")
    compiler = resolve_tool(args.compiler)
    assembler = sibling_tool(compiler, args.as_tool, "as")
    linker = sibling_tool(compiler, args.ld, "ld")
    nm = sibling_tool(compiler, args.nm, "nm")

    args.build_dir.mkdir(parents=True, exist_ok=True)
    storage_source = args.build_dir / "compatibility_storage.S"
    runtime_source = args.build_dir / "runtime_shims.c"
    storage_object = args.build_dir / "compatibility_storage.o"
    runtime_object = args.build_dir / "runtime_shims.o"
    provider_object = args.build_dir / "provider_frontier.o"
    storage_source.write_text(render_storage_assembly(rows), encoding="utf-8")
    runtime_source.write_text(render_runtime_c(), encoding="utf-8")

    run([str(assembler), "-EL", "-o", str(storage_object), str(storage_source)])
    run(
        [
            str(compiler),
            "-G0",
            "-O2",
            "-EL",
            "-fomit-frame-pointer",
            "-fno-common",
            "-ffreestanding",
            "-fno-builtin",
            "-fshort-double",
            "-mlong64",
            "-mhard-float",
            "-mno-abicalls",
            "-march=r5900",
            "-mtune=r5900",
            "-ffunction-sections",
            "-w",
            "-c",
            str(runtime_source),
            "-o",
            str(runtime_object),
        ]
    )
    run(
        [
            str(linker),
            "-EL",
            "-r",
            "-o",
            str(provider_object),
            str(storage_object),
            str(runtime_object),
        ]
    )

    provider_symbols = global_symbols(nm, provider_object)
    provider_by_name = symbol_map(provider_symbols)
    emitted = {
        row["symbol"]
        for row in rows
        if row["resolution_kind"] in {COMPAT_STORAGE, RUNTIME_SHIM}
    }
    provider_defined = {name for name, symbol in provider_by_name.items() if not symbol.undefined}
    provider_undefined = {name for name, symbol in provider_by_name.items() if symbol.undefined}
    if provider_defined != emitted or provider_undefined:
        fail(
            "generated provider symbol drift; "
            f"defined_missing={sorted(emitted - provider_defined)[:5]} "
            f"defined_extra={sorted(provider_defined - emitted)[:5]} "
            f"undefined={sorted(provider_undefined)}"
        )

    input_symbols = global_symbols(nm, args.input)
    input_undefined = verify_input_frontier(input_symbols, rows)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    map_path = args.output.with_suffix(".map")
    command = [str(linker), "-EL", "-r", "-Map", str(map_path)]
    for row in rows:
        kind = row["resolution_kind"]
        if kind == ABSOLUTE_ANCHOR:
            command.extend(["--defsym", f"{row['symbol']}={row['target_address']}"])
        elif kind == SEMANTIC_ALIAS:
            command.extend(["--defsym", f"{row['symbol']}={row['target_symbol']}"])
    command.extend(["-o", str(args.output), str(args.input), str(provider_object)])
    run(command)

    output_symbols = global_symbols(nm, args.output)
    output_undefined = {symbol.name for symbol in output_symbols if symbol.undefined}
    if output_undefined:
        fail(f"provider-closed aggregate still has externals: {sorted(output_undefined)[:10]}")
    output_by_name = symbol_map(output_symbols)
    for row in rows:
        symbol = output_by_name.get(row["symbol"])
        if symbol is None or symbol.undefined:
            fail(f"resolved provider symbol absent from output: {row['symbol']}")
        if row["resolution_kind"] == ABSOLUTE_ANCHOR:
            if symbol.type_code.upper() != "A" or int(symbol.value, 16) != int(
                row["target_address"], 0
            ):
                fail(f"absolute target anchor drift: {row['symbol']}")
        elif row["resolution_kind"] == SEMANTIC_ALIAS:
            target = output_by_name.get(row["target_symbol"])
            if target is None or target.undefined or symbol.value != target.value:
                fail(f"semantic provider alias drift: {row['symbol']}")

    input_sections = alloc_section_fingerprints(args.input)
    output_sections = alloc_section_fingerprints(args.output)
    for name, fingerprint in input_sections.items():
        if output_sections.get(name) != fingerprint:
            fail(f"existing allocated section changed: {name}")
    added_sections = {
        name: value for name, value in output_sections.items() if name not in input_sections
    }
    provider_sections = {
        name: value
        for name, value in alloc_section_fingerprints(provider_object).items()
        if value["size"] != 0
    }
    if added_sections != provider_sections:
        fail("provider-closed aggregate did not add exactly the generated shim sections")

    summary = summarize(rows)
    report: dict[str, object] = {
        "schema": 1,
        "claim": "complete-source-link-provider-namespace-closure",
        **summary,
        "input_external_symbols": len(input_undefined),
        "output_external_symbols": len(output_undefined),
        "existing_allocated_sections_unchanged": True,
        "added_allocated_sections": provider_sections,
        "input_sha256": sha256_file(args.input),
        "provider_object_sha256": sha256_file(provider_object),
        "output_sha256": sha256_file(args.output),
        "manifest_sha256": sha256_file(args.manifest),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return report


def add_public_arguments(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--contracts", type=Path, default=DEFAULT_CONTRACTS)
    parser.add_argument("--private-manifest", type=Path, default=DEFAULT_PRIVATE)
    parser.add_argument("--defined-map", type=Path, default=DEFAULT_DEFINED)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="action", required=True)
    validate_parser = subparsers.add_parser("validate", help="verify public closure metadata")
    add_public_arguments(validate_parser)
    refresh_parser = subparsers.add_parser("refresh", help="refresh public closure metadata")
    add_public_arguments(refresh_parser)
    link_parser = subparsers.add_parser("link", help="generate providers and close the aggregate")
    add_public_arguments(link_parser)
    link_parser.add_argument("--compiler", default="ee-gcc")
    link_parser.add_argument("--as", dest="as_tool")
    link_parser.add_argument("--ld")
    link_parser.add_argument("--nm")
    link_parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    link_parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    link_parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    link_parser.add_argument("--report", type=Path, default=DEFAULT_REPORT)
    args = parser.parse_args(argv)
    for name in ("contracts", "private_manifest", "defined_map", "manifest"):
        value = getattr(args, name)
        if not value.is_absolute():
            setattr(args, name, (ROOT / value).resolve())
    if args.action == "link":
        for name in ("input", "build_dir", "output", "report"):
            value = getattr(args, name)
            if not value.is_absolute():
                setattr(args, name, (ROOT / value).resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    try:
        args = parse_args(argv)
        if args.action == "refresh":
            rows, rendered = expected_manifest(args)
            args.manifest.parent.mkdir(parents=True, exist_ok=True)
            args.manifest.write_text(rendered, encoding="utf-8")
            report: dict[str, object] = summarize(rows)
            action = "refreshed"
        else:
            rows = validate_frozen_manifest(args)
            report = summarize(rows)
            action = "verified"
            if args.action == "link":
                report = link_frontier(args, rows)
                action = "linked"
    except (FrontierError, AliasError) as exc:
        print(f"provider-frontier gate failed: {exc}", file=sys.stderr)
        return 1

    print(
        f"{action} provider frontier: total={report['frontier_total']} "
        f"anchors={report['absolute_anchors']} aliases={report['semantic_aliases']} "
        f"storage={report['compatibility_storage_symbols']} shims={report['runtime_shims']}"
    )
    if args.action == "link":
        print(
            f"partial-link externals: {report['input_external_symbols']} -> "
            f"{report['output_external_symbols']}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
