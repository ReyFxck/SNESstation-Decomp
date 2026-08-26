#!/usr/bin/env python3
"""Validate and privately materialize the five embedded-asset providers.

The tracked manifest contains only names, target ranges and hashes.  The
``link`` action reads a user-supplied unpacked reference image, verifies it
against the public layout oracle, and emits all private bytes below ``build/``.
No extracted asset or generated provider object belongs in the public tree.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import struct
import sys
from dataclasses import dataclass
from io import StringIO
from pathlib import Path
from typing import Iterable, Sequence

from source_aliases import (
    AliasError,
    NmSymbol,
    alloc_section_fingerprints,
    global_symbols,
    resolve_tool,
    run,
    sha256_file,
    sibling_tool,
)


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_ASSETS = ROOT / "analysis" / "embedded_assets.csv"
DEFAULT_CONTRACTS = ROOT / "analysis" / "link_identity" / "link_contracts.tsv"
DEFAULT_LAYOUT = ROOT / "analysis" / "link_identity" / "unpacked_layout.json"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "private_asset_providers.tsv"
DEFAULT_REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
DEFAULT_INPUT = ROOT / "build" / "link-contracts" / "source-tree.link-contracts.partial.o"
DEFAULT_BUILD = ROOT / "build" / "private-assets"
DEFAULT_OUTPUT = DEFAULT_BUILD / "source-tree.private-assets.partial.o"
DEFAULT_REPORT = DEFAULT_BUILD / "report.json"

ASSET_FIELDS = (
    "name",
    "kind",
    "va_start",
    "va_end",
    "size_hex",
    "size_dec",
    "sha256",
    "decoded_sha256",
    "size_word_va",
    "consumer",
)
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
MANIFEST_FIELDS = (
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

# Mapping a private range to the two source-level names that consume it is the
# reviewed identity decision in this checkpoint.  All range/hash details are
# independently derived from analysis/embedded_assets.csv.
ASSET_SYMBOLS = (
    ("cdvd_irx", "embedded_cdvd_irx", "embedded_cdvd_irx_size"),
    ("sjpcm_irx", "embedded_sjpcm_irx", "embedded_sjpcm_irx_size"),
    ("amigamod_irx", "embedded_amigamod_irx", "embedded_amigamod_irx_size"),
    ("credits_text", "embedded_credits_xor", "embedded_credits_bytes"),
    ("disclaimer_text", "embedded_disclaimer_xor", "embedded_disclaimer_bytes"),
)
BLOCKED = "BLOCKED"
PRIVATE_ASSET = "private-asset"
# All five data ranges and their 32-bit size words satisfy four-byte alignment.
# A stronger input-section alignment would be an unproved historical-linker
# claim; exact target placement remains a later linker-script gate.
SECTION_ALIGNMENT = 0x4


class ProviderError(RuntimeError):
    """A private-asset provider invariant failed."""


def fail(message: str) -> None:
    raise ProviderError(message)


@dataclass(frozen=True)
class Layout:
    base: int
    size: int
    sha256: str

    @property
    def end(self) -> int:
        return self.base + self.size


def parse_int(value: str, *, context: str) -> int:
    try:
        return int(value, 0)
    except (TypeError, ValueError) as exc:
        fail(f"invalid integer for {context}: {value!r}")
        raise AssertionError from exc


def read_table(path: Path, fields: Sequence[str], *, delimiter: str) -> list[dict[str, str]]:
    if not path.is_file():
        fail(f"missing manifest input: {path}")
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter=delimiter)
        if tuple(reader.fieldnames or ()) != tuple(fields):
            fail(f"unexpected columns in {path}: {reader.fieldnames}")
        return list(reader)


def read_layout(path: Path) -> Layout:
    if not path.is_file():
        fail(f"missing unpacked layout manifest: {path}")
    try:
        image = json.loads(path.read_text(encoding="utf-8"))["image"]
        layout = Layout(
            base=int(image["base_address"]),
            size=int(image["initialized_size"]),
            sha256=str(image["sha256"]),
        )
    except (json.JSONDecodeError, KeyError, TypeError, ValueError) as exc:
        fail(f"invalid unpacked layout manifest: {path}")
        raise AssertionError from exc
    if (
        layout.base < 0
        or layout.size <= 0
        or len(layout.sha256) != 64
        or any(ch not in "0123456789abcdef" for ch in layout.sha256)
    ):
        fail(f"invalid unpacked image identity in {path}")
    return layout


def render_tsv(rows: Iterable[dict[str, str]]) -> str:
    output = StringIO(newline="")
    writer = csv.DictWriter(
        output,
        fieldnames=MANIFEST_FIELDS,
        delimiter="\t",
        lineterminator="\n",
    )
    writer.writeheader()
    writer.writerows(rows)
    return output.getvalue()


def derive_rows(
    asset_rows: Sequence[dict[str, str]],
    contract_rows: Sequence[dict[str, str]],
    layout: Layout,
) -> list[dict[str, str]]:
    assets: dict[str, dict[str, str]] = {}
    for row in asset_rows:
        name = row["name"]
        if not name or name in assets:
            fail(f"duplicate or empty embedded asset: {name!r}")
        assets[name] = row

    contracts: dict[str, dict[str, str]] = {}
    for row in contract_rows:
        symbol = row["symbol"]
        if not symbol or symbol in contracts:
            fail(f"duplicate or empty link contract: {symbol!r}")
        contracts[symbol] = row

    expected_private = {
        row["symbol"]
        for row in contract_rows
        if row["status"] == BLOCKED and row["provider_kind"] == PRIVATE_ASSET
    }
    mapped_private = {
        symbol
        for _asset, data_symbol, size_symbol in ASSET_SYMBOLS
        for symbol in (data_symbol, size_symbol)
    }
    if expected_private != mapped_private:
        missing = sorted(expected_private - mapped_private)
        extra = sorted(mapped_private - expected_private)
        fail(f"private-asset contract mapping drift; missing={missing} extra={extra}")

    result: list[dict[str, str]] = []
    for asset_name, data_symbol, size_symbol in ASSET_SYMBOLS:
        asset = assets.get(asset_name)
        if asset is None:
            fail(f"missing embedded-asset row: {asset_name}")
        data_contract = contracts.get(data_symbol)
        size_contract = contracts.get(size_symbol)
        if data_contract is None or size_contract is None:
            fail(f"missing private contracts for {asset_name}")
        for contract in (data_contract, size_contract):
            if contract["status"] != BLOCKED or contract["provider_kind"] != PRIVATE_ASSET:
                fail(f"{contract['symbol']}: no longer a blocked private-asset contract")
        if data_contract["requesters"] != size_contract["requesters"]:
            fail(f"{asset_name}: data/size requester mismatch")

        va = parse_int(asset["va_start"], context=f"{asset_name} start")
        end = parse_int(asset["va_end"], context=f"{asset_name} end")
        size = parse_int(asset["size_hex"], context=f"{asset_name} size")
        size_word_va = parse_int(asset["size_word_va"], context=f"{asset_name} size word")
        size_dec = parse_int(asset["size_dec"], context=f"{asset_name} decimal size")
        if end != va + size or size_dec != size:
            fail(f"{asset_name}: inconsistent range/size metadata")
        if not (layout.base <= va < end <= size_word_va < size_word_va + 4 <= layout.end):
            fail(f"{asset_name}: provider range lies outside the unpacked image")
        padding = size_word_va - end
        if padding < 0 or padding >= SECTION_ALIGNMENT:
            fail(f"{asset_name}: unsupported padding 0x{padding:x}")
        if va % SECTION_ALIGNMENT:
            fail(f"{asset_name}: target start is not 0x{SECTION_ALIGNMENT:x}-aligned")
        digest = asset["sha256"].lower()
        if len(digest) != 64 or any(ch not in "0123456789abcdef" for ch in digest):
            fail(f"{asset_name}: invalid SHA-256")

        result.append(
            {
                "asset": asset_name,
                "data_symbol": data_symbol,
                "size_symbol": size_symbol,
                "target_va": f"0x{va:08x}",
                "size_hex": f"0x{size:x}",
                "sha256": digest,
                "size_word_va": f"0x{size_word_va:08x}",
                "padding_hex": f"0x{padding:x}",
                # Credits/disclaimer are decoded in place, so the conservative
                # provider envelope must be writable.  Exact historical output
                # section ownership remains a separate final-link proof.
                "section_name": f".data.private_asset.{asset_name}",
                "section_alignment_hex": f"0x{SECTION_ALIGNMENT:x}",
                "requesters": data_contract["requesters"],
            }
        )
    return result


def expected_manifest(args: argparse.Namespace) -> tuple[list[dict[str, str]], str, Layout]:
    assets = read_table(args.assets, ASSET_FIELDS, delimiter=",")
    contracts = read_table(args.contracts, CONTRACT_FIELDS, delimiter="\t")
    layout = read_layout(args.layout_manifest)
    rows = derive_rows(assets, contracts, layout)
    return rows, render_tsv(rows), layout


def validate_frozen_manifest(args: argparse.Namespace) -> tuple[list[dict[str, str]], Layout]:
    rows, expected, layout = expected_manifest(args)
    if not args.manifest.is_file():
        fail(f"missing private-asset provider manifest: {args.manifest}")
    if args.manifest.read_text(encoding="utf-8") != expected:
        fail(
            "private-asset provider manifest is stale; review the identity map and run "
            "tools/private_asset_providers.py refresh"
        )
    return rows, layout


def summarize(rows: Sequence[dict[str, str]], contracts: Sequence[dict[str, str]]) -> dict[str, int]:
    frontier = sum(row["status"] == BLOCKED for row in contracts)
    provider_symbols = len(rows) * 2
    provider_bytes = sum(
        parse_int(row["size_hex"], context=row["asset"])
        + parse_int(row["padding_hex"], context=row["asset"])
        + 4
        for row in rows
    )
    return {
        "bundles": len(rows),
        "provider_symbols": provider_symbols,
        "provider_bytes": provider_bytes,
        "input_frontier": frontier,
        "output_frontier": frontier - provider_symbols,
    }


def validate_reference(
    image: bytes,
    layout: Layout,
    rows: Sequence[dict[str, str]],
) -> dict[str, bytes]:
    if len(image) != layout.size:
        fail(f"unpacked reference size mismatch: expected {layout.size}, got {len(image)}")
    digest = hashlib.sha256(image).hexdigest()
    if digest != layout.sha256:
        fail(f"unpacked reference SHA-256 mismatch: expected {layout.sha256}, got {digest}")

    payloads: dict[str, bytes] = {}
    for row in rows:
        va = parse_int(row["target_va"], context=row["asset"])
        size = parse_int(row["size_hex"], context=row["asset"])
        padding = parse_int(row["padding_hex"], context=row["asset"])
        size_word_va = parse_int(row["size_word_va"], context=row["asset"])
        start = va - layout.base
        size_offset = size_word_va - layout.base
        if (
            start < 0
            or start + size > size_offset
            or size_offset - (start + size) != padding
            or size_offset + 4 > len(image)
        ):
            fail(f"{row['asset']}: provider range lies outside the private reference")
        data = image[start:start + size]
        if hashlib.sha256(data).hexdigest() != row["sha256"]:
            fail(f"{row['asset']}: private asset SHA-256 mismatch")
        if image[start + size:size_offset] != b"\0" * padding:
            fail(f"{row['asset']}: nonzero target padding before size word")
        stored_size = struct.unpack_from("<I", image, size_offset)[0]
        if stored_size != size:
            fail(
                f"{row['asset']}: size word mismatch: expected 0x{size:x}, "
                f"got 0x{stored_size:x}"
            )
        payloads[row["section_name"]] = image[start:size_offset + 4]
    return payloads


def quote_assembly_path(path: Path) -> str:
    value = str(path.resolve())
    if "\n" in value or "\r" in value:
        fail("reference path cannot contain a newline")
    return value.replace("\\", "\\\\").replace('"', '\\"')


def render_assembly(reference: Path, layout: Layout, rows: Sequence[dict[str, str]]) -> str:
    quoted = quote_assembly_path(reference)
    lines = [
        "/* Generated privately by tools/private_asset_providers.py. */",
        "/* Contains no bytes itself; .incbin reads the verified local image. */",
        "",
    ]
    for row in rows:
        va = parse_int(row["target_va"], context=row["asset"])
        size = parse_int(row["size_hex"], context=row["asset"])
        padding = parse_int(row["padding_hex"], context=row["asset"])
        lines.extend(
            [
                f'.section {row["section_name"]},"aw",@progbits',
                f".balign {SECTION_ALIGNMENT}",
                f'.globl {row["data_symbol"]}',
                f'.type {row["data_symbol"]},@object',
                f'{row["data_symbol"]}:',
                f'.incbin "{quoted}",{va - layout.base},{size}',
                f'.size {row["data_symbol"]},{size}',
            ]
        )
        if padding:
            lines.append(f".space {padding},0")
        lines.extend(
            [
                f'.globl {row["size_symbol"]}',
                f'.type {row["size_symbol"]},@object',
                f'{row["size_symbol"]}:',
                f'.word {size}',
                f'.size {row["size_symbol"]},4',
                "",
            ]
        )
    return "\n".join(lines)


def symbol_map(symbols: Sequence[NmSymbol]) -> dict[str, NmSymbol]:
    result: dict[str, NmSymbol] = {}
    for symbol in symbols:
        if symbol.name in result:
            fail(f"duplicate global symbol: {symbol.name}")
        result[symbol.name] = symbol
    return result


def verify_provider_object(
    symbols: Sequence[NmSymbol],
    sections: dict[str, dict[str, object]],
    rows: Sequence[dict[str, str]],
    payloads: dict[str, bytes],
) -> None:
    expected_names = {
        name
        for row in rows
        for name in (row["data_symbol"], row["size_symbol"])
    }
    by_name = symbol_map(symbols)
    if set(by_name) != expected_names:
        fail(
            "provider object global-symbol drift; "
            f"missing={sorted(expected_names - set(by_name))[:5]} "
            f"extra={sorted(set(by_name) - expected_names)[:5]}"
        )
    expected_sections: dict[str, dict[str, object]] = {}
    for row in rows:
        size = parse_int(row["size_hex"], context=row["asset"])
        padding = parse_int(row["padding_hex"], context=row["asset"])
        data_symbol = by_name[row["data_symbol"]]
        size_symbol = by_name[row["size_symbol"]]
        if data_symbol.undefined or size_symbol.undefined:
            fail(f"{row['asset']}: provider symbol remained undefined")
        if data_symbol.type_code.upper() != "D" or size_symbol.type_code.upper() != "D":
            fail(f"{row['asset']}: provider symbols are not initialized writable data")
        if int(data_symbol.value, 16) != 0 or int(data_symbol.size, 16) != size:
            fail(f"{row['data_symbol']}: value/size mismatch")
        if int(size_symbol.value, 16) != size + padding or int(size_symbol.size, 16) != 4:
            fail(f"{row['size_symbol']}: value/size mismatch")
        payload = payloads[row["section_name"]]
        expected_sections[row["section_name"]] = {
            "type": 1,
            "flags": 3,
            "size": len(payload),
            "alignment": SECTION_ALIGNMENT,
            "sha256": hashlib.sha256(payload).hexdigest(),
        }
    # GNU as emits conventional zero-sized .text/.data/.bss sections even for
    # this data-only object.  They carry no bytes and disappear into the
    # aggregate's existing sections; reject every unexpected nonempty section.
    material_sections = {
        name: value for name, value in sections.items() if value["size"] != 0
    }
    if material_sections != dict(sorted(expected_sections.items())):
        fail("provider object allocated-section fingerprints differ from the private oracle")


def verify_link_result(
    input_symbols: Sequence[NmSymbol],
    output_symbols: Sequence[NmSymbol],
    contract_rows: Sequence[dict[str, str]],
    rows: Sequence[dict[str, str]],
) -> tuple[int, int]:
    expected_input = {row["symbol"] for row in contract_rows if row["status"] == BLOCKED}
    providers = {
        name
        for row in rows
        for name in (row["data_symbol"], row["size_symbol"])
    }
    input_undefined = {symbol.name for symbol in input_symbols if symbol.undefined}
    output_undefined = {symbol.name for symbol in output_symbols if symbol.undefined}
    if input_undefined != expected_input:
        fail("V85 aggregate/private-provider manifest drift")
    if output_undefined != expected_input - providers:
        fail("private-provider output frontier is not the exact V85 frontier minus ten symbols")
    output_by_name = symbol_map(output_symbols)
    for name in providers:
        symbol = output_by_name.get(name)
        if symbol is None or symbol.undefined or symbol.type_code.upper() != "D":
            fail(f"linked private provider is absent or has the wrong type: {name}")
    return len(input_undefined), len(output_undefined)


def link_providers(
    args: argparse.Namespace,
    rows: Sequence[dict[str, str]],
    layout: Layout,
) -> dict[str, object]:
    if not args.reference.is_file():
        fail(f"missing private unpacked reference: {args.reference}")
    if not args.input.is_file():
        fail(f"missing V85 link-contract aggregate: {args.input}")
    compiler = resolve_tool(args.compiler)
    assembler = sibling_tool(compiler, args.as_tool, "as")
    linker = sibling_tool(compiler, args.ld, "ld")
    nm = sibling_tool(compiler, args.nm, "nm")

    image = args.reference.read_bytes()
    payloads = validate_reference(image, layout, rows)
    contracts = read_table(args.contracts, CONTRACT_FIELDS, delimiter="\t")
    summary = summarize(rows, contracts)

    args.build_dir.mkdir(parents=True, exist_ok=True)
    assembly_path = args.build_dir / "private_asset_providers.S"
    provider_object = args.build_dir / "private_asset_providers.o"
    assembly_path.write_text(render_assembly(args.reference, layout, rows), encoding="utf-8")
    run([str(assembler), "-EL", "-o", str(provider_object), str(assembly_path)])

    provider_symbols = global_symbols(nm, provider_object)
    provider_sections = alloc_section_fingerprints(provider_object)
    verify_provider_object(provider_symbols, provider_sections, rows, payloads)

    input_symbols = global_symbols(nm, args.input)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    map_path = args.output.with_suffix(".map")
    run(
        [
            str(linker),
            "-EL",
            "-r",
            "-Map",
            str(map_path),
            "-o",
            str(args.output),
            str(args.input),
            str(provider_object),
        ]
    )
    output_symbols = global_symbols(nm, args.output)
    input_count, output_count = verify_link_result(input_symbols, output_symbols, contracts, rows)

    input_sections = alloc_section_fingerprints(args.input)
    output_sections = alloc_section_fingerprints(args.output)
    for name, fingerprint in input_sections.items():
        if output_sections.get(name) != fingerprint:
            fail(f"existing allocated section changed while adding private assets: {name}")
    added_sections = {name: value for name, value in output_sections.items() if name not in input_sections}
    provider_material_sections = {
        name: value for name, value in provider_sections.items() if value["size"] != 0
    }
    if added_sections != provider_material_sections:
        fail("linked aggregate did not add exactly the five private provider sections")

    report: dict[str, object] = {
        "schema": 1,
        "claim": "verified-private-asset-provider-batch",
        **summary,
        "input_external_symbols": input_count,
        "output_external_symbols": output_count,
        "existing_allocated_sections_unchanged": True,
        "added_allocated_sections": provider_material_sections,
        "input_sha256": sha256_file(args.input),
        "output_sha256": sha256_file(args.output),
        "reference_sha256": layout.sha256,
        "manifest_sha256": sha256_file(args.manifest),
        "contract_manifest_sha256": sha256_file(args.contracts),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    markdown = [
        "# Private-asset provider partial-link report",
        "",
        f"- Provider bundles: **{summary['bundles']}**",
        f"- Provider symbols: **{summary['provider_symbols']}**",
        f"- Verified private bytes emitted: **{summary['provider_bytes']:,}**",
        f"- Aggregate externals: **{input_count} -> {output_count}**",
        "- Existing allocated sections changed: **no**",
        "",
        "All generated assembly, objects and reports are ignored private build products.",
        "",
    ]
    args.report.with_suffix(".md").write_text("\n".join(markdown), encoding="utf-8")
    return report


def add_public_arguments(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--assets", type=Path, default=DEFAULT_ASSETS)
    parser.add_argument("--contracts", type=Path, default=DEFAULT_CONTRACTS)
    parser.add_argument("--layout-manifest", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="action", required=True)
    validate_parser = subparsers.add_parser("validate", help="verify public provider metadata")
    add_public_arguments(validate_parser)
    refresh_parser = subparsers.add_parser("refresh", help="refresh public provider metadata")
    add_public_arguments(refresh_parser)
    link_parser = subparsers.add_parser("link", help="materialize and link private providers")
    add_public_arguments(link_parser)
    link_parser.add_argument("--compiler", default="ee-gcc")
    link_parser.add_argument("--as", dest="as_tool")
    link_parser.add_argument("--ld")
    link_parser.add_argument("--nm")
    link_parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    link_parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    link_parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    link_parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    link_parser.add_argument("--report", type=Path, default=DEFAULT_REPORT)
    args = parser.parse_args(argv)
    for name in ("assets", "contracts", "layout_manifest", "manifest"):
        value = getattr(args, name)
        if not value.is_absolute():
            setattr(args, name, (ROOT / value).resolve())
    if args.action == "link":
        for name in ("reference", "input", "build_dir", "output", "report"):
            value = getattr(args, name)
            if not value.is_absolute():
                setattr(args, name, (ROOT / value).resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    try:
        args = parse_args(argv)
        if args.action == "refresh":
            rows, rendered, _layout = expected_manifest(args)
            args.manifest.parent.mkdir(parents=True, exist_ok=True)
            args.manifest.write_text(rendered, encoding="utf-8")
            contracts = read_table(args.contracts, CONTRACT_FIELDS, delimiter="\t")
            report: dict[str, object] = summarize(rows, contracts)
            action = "refreshed"
        else:
            rows, layout = validate_frozen_manifest(args)
            contracts = read_table(args.contracts, CONTRACT_FIELDS, delimiter="\t")
            report = summarize(rows, contracts)
            action = "verified"
            if args.action == "link":
                report = link_providers(args, rows, layout)
                action = "linked"
    except (ProviderError, AliasError) as exc:
        print(f"private-asset provider gate failed: {exc}", file=sys.stderr)
        return 1

    print(
        f"{action} private-asset providers: bundles={report['bundles']} "
        f"symbols={report['provider_symbols']} bytes={report['provider_bytes']} "
        f"frontier={report['input_frontier']}->{report['output_frontier']}"
    )
    if args.action == "link":
        print(
            f"partial-link externals: {report['input_external_symbols']} -> "
            f"{report['output_external_symbols']} (five verified private sections added)"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
