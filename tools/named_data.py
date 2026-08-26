#!/usr/bin/env python3
"""Audit the original Stage-3C 54-name discrete-data tranche.

The Stage-2 ownership checkpoint partitions its 1,921 externals into the
original small-batch plan.  Stage 3C is exactly the 43 ``named-program-data``
rows, one ``vtable-or-rtti`` row, and ten private-asset rows.  This gate keeps
that historical 54-row definition stable and distinguishes four materially
different claims:

* a private reference range whose bytes and address are proved;
* a named range whose target address, consumed extent, and bytes/zero-fill are
  proved without publishing those bytes;
* an address-only contract whose full object extent is not yet proved; and
* a synthetic behavioral-source adapter that has no single target object and
  must be removed by a source-layout refactor.

The public manifest contains only addresses, extents, classifications, and
SHA-256 fingerprints.  ``verify`` and ``refresh`` require the privately
unpacked 3,304,936-byte reference image.
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

from provider_frontier import (
    ABSOLUTE_ANCHOR,
    COMPAT_STORAGE,
    RUNTIME_SHIM,
    SEMANTIC_ALIAS,
    render_runtime_c,
    render_storage_assembly,
    symbol_map,
    verify_input_frontier,
)
from source_aliases import (
    alloc_section_fingerprints,
    global_symbols,
    resolve_tool,
    run,
    sibling_tool,
)


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_EXTERNAL = ROOT / "analysis" / "source_tree" / "external_symbol_ownership.tsv"
DEFAULT_CONTRACTS = ROOT / "analysis" / "link_identity" / "link_contracts.tsv"
DEFAULT_PRIVATE = ROOT / "analysis" / "link_identity" / "private_asset_providers.tsv"
DEFAULT_FRONTIER = ROOT / "analysis" / "link_identity" / "provider_frontier_closure.tsv"
DEFAULT_LAYOUT = ROOT / "analysis" / "link_identity" / "unpacked_layout.json"
DEFAULT_REVIEWS = ROOT / "analysis" / "link_identity" / "named_data_reviews.tsv"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "named_data.tsv"
DEFAULT_REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
DEFAULT_REPORT = ROOT / "build" / "named-data" / "report.json"
DEFAULT_INPUT = ROOT / "build" / "private-assets" / "source-tree.private-assets.partial.o"
DEFAULT_BUILD = ROOT / "build" / "named-data"
DEFAULT_OUTPUT = DEFAULT_BUILD / "source-tree.named-data.partial.o"

EXTERNAL_FIELDS = (
    "symbol", "category", "provider_kind", "owner", "resolution_gate", "requesters",
)
CONTRACT_FIELDS = (
    "symbol", "target_address", "status", "resolution_kind", "canonical_symbol",
    "evidence", "category", "provider_kind", "canonical_source", "canonical_object",
    "requesters", "detail",
)
PRIVATE_FIELDS = (
    "asset", "data_symbol", "size_symbol", "target_va", "size_hex", "sha256",
    "size_word_va", "padding_hex", "section_name", "section_alignment_hex", "requesters",
)
FRONTIER_FIELDS = (
    "symbol", "category", "provider_kind", "resolution_kind", "target_address",
    "target_symbol", "storage_size_hex", "evidence", "requesters", "detail",
)
REVIEW_FIELDS = (
    "symbol", "decision", "target_address", "extent_hex", "evidence",
    "evidence_path", "evidence_token", "detail",
)
MANIFEST_FIELDS = (
    "symbol", "category", "status", "target_address", "extent_hex", "region",
    "sha256", "evidence", "requesters", "detail",
)

PRIVATE_BYTES = "PRIVATE_BYTES_PROVED"
RANGE_PROVED = "RANGE_PROVED"
ADDRESS_PROVED = "ADDRESS_PROVED"
SOURCE_REFACTOR = "SOURCE_REFACTOR"
VALID_STATUSES = {PRIVATE_BYTES, RANGE_PROVED, ADDRESS_PROVED, SOURCE_REFACTOR}
VALID_DECISIONS = {"target-range", "target-address", "source-refactor"}
SHA_RE = re.compile(r"[0-9a-f]{64}")


class NamedDataError(RuntimeError):
    """A Stage-3C named-data invariant failed."""


def fail(message: str) -> None:
    raise NamedDataError(message)


def read_table(path: Path, fields: Sequence[str]) -> list[dict[str, str]]:
    if not path.is_file():
        fail(f"missing manifest input: {path}")
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != tuple(fields):
            fail(f"unexpected columns in {path}")
        return list(reader)


def parse_hex(value: str, *, field: str, allow_blank: bool = False) -> int | None:
    if not value:
        if allow_blank:
            return None
        fail(f"blank {field}")
    try:
        parsed = int(value, 0)
    except ValueError as exc:
        fail(f"invalid {field}: {value!r}")
        raise AssertionError from exc
    if parsed < 0:
        fail(f"negative {field}: {value!r}")
    return parsed


def canonical_hex(value: int | None) -> str:
    return "" if value is None else f"0x{value:x}"


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def render_tsv(rows: Iterable[dict[str, str]]) -> str:
    output = StringIO(newline="")
    writer = csv.DictWriter(
        output, fieldnames=MANIFEST_FIELDS, delimiter="\t", lineterminator="\n",
    )
    writer.writeheader()
    writer.writerows(rows)
    return output.getvalue()


def load_layout(path: Path) -> dict[str, int | str]:
    if not path.is_file():
        fail(f"missing layout manifest: {path}")
    data = json.loads(path.read_text(encoding="utf-8"))
    try:
        image = data["image"]
        result: dict[str, int | str] = {
            "base": int(image["base_address"]),
            "initialized_end": int(image["initialized_end_address"]),
            "memory_end": int(image["memory_end_address"]),
            "initialized_size": int(image["initialized_size"]),
            "sha256": str(image["sha256"]),
        }
    except (KeyError, TypeError, ValueError) as exc:
        fail(f"malformed layout manifest: {path}")
        raise AssertionError from exc
    if result["initialized_end"] != result["base"] + result["initialized_size"]:
        fail("layout initialized span is inconsistent")
    if result["memory_end"] < result["initialized_end"]:
        fail("layout memory span is inconsistent")
    return result


def read_reviews(path: Path) -> dict[str, dict[str, str]]:
    rows = read_table(path, REVIEW_FIELDS)
    result: dict[str, dict[str, str]] = {}
    for row in rows:
        symbol = row["symbol"]
        if symbol in result:
            fail(f"duplicate named-data review: {symbol}")
        if row["decision"] not in VALID_DECISIONS:
            fail(f"invalid named-data review decision for {symbol}: {row['decision']}")
        address = parse_hex(row["target_address"], field=f"{symbol} target address", allow_blank=True)
        extent = parse_hex(row["extent_hex"], field=f"{symbol} extent", allow_blank=True)
        if row["decision"] == "source-refactor":
            if address is not None:
                fail(f"synthetic adapter review assigns target storage: {symbol}")
        elif address is None:
            fail(f"target review lacks address: {symbol}")
        if row["decision"] == "target-range" and (extent is None or extent == 0):
            fail(f"target-range review lacks nonzero extent: {symbol}")
        if not row["evidence"].startswith("reviewed-"):
            fail(f"review evidence is not labelled for {symbol}")
        relative = Path(row["evidence_path"])
        if relative.is_absolute() or ".." in relative.parts:
            fail(f"review evidence path must be repository-relative: {symbol}")
        evidence_path = ROOT / relative
        if not evidence_path.is_file():
            fail(f"missing review evidence for {symbol}: {relative}")
        evidence_text = evidence_path.read_text(encoding="utf-8", errors="replace")
        if not row["evidence_token"] or row["evidence_token"] not in evidence_text:
            fail(f"review evidence token drift for {symbol}: {row['evidence_token']!r}")
        if not row["detail"]:
            fail(f"review lacks detail: {symbol}")
        result[symbol] = row
    return result


def unique_by_symbol(rows: Sequence[dict[str, str]], label: str) -> dict[str, dict[str, str]]:
    result: dict[str, dict[str, str]] = {}
    for row in rows:
        symbol = row["symbol"]
        if symbol in result:
            fail(f"duplicate {label} symbol: {symbol}")
        result[symbol] = row
    return result


def stage3_partition(external_rows: Sequence[dict[str, str]]) -> dict[str, int]:
    counts = Counter((row["category"], row["provider_kind"]) for row in external_rows)
    partition = {
        "3B": counts[("target-address-symbol", "source-address-alias")]
              + counts[("target-function-alias", "source-address-alias")],
        "3C": counts[("named-program-data", "program-data")]
              + counts[("vtable-or-rtti", "program-data")]
              + counts[("embedded-binary", "private-asset")],
        "3D": counts[("ps2-runtime", "historical-archive")]
              + counts[("c-runtime", "historical-archive")]
              + counts[("compiler-runtime", "historical-archive")],
        "3E": counts[("named-external", "link-contract")]
              + counts[("zlib-peer", "source-or-archive")],
        "3F": counts[("target-address-data", "program-data")],
    }
    expected = {"3B": 337, "3C": 54, "3D": 53, "3E": 212, "3F": 1265}
    if partition != expected or sum(partition.values()) != 1921:
        fail(f"original Stage-3 partition drift: {partition}")
    return partition


def discrete_rows(external_rows: Sequence[dict[str, str]]) -> list[dict[str, str]]:
    rows = [
        row for row in external_rows
        if (row["category"], row["provider_kind"]) in {
            ("named-program-data", "program-data"),
            ("vtable-or-rtti", "program-data"),
            ("embedded-binary", "private-asset"),
        }
    ]
    if len(rows) != 54:
        fail(f"expected exact Stage-3C tranche of 54 rows, found {len(rows)}")
    return sorted(rows, key=lambda row: row["symbol"])


def base_rows(args: argparse.Namespace) -> tuple[list[dict[str, str]], dict[str, int | str]]:
    external_rows = read_table(args.external_map, EXTERNAL_FIELDS)
    stage3_partition(external_rows)
    contracts = unique_by_symbol(read_table(args.contracts, CONTRACT_FIELDS), "contract")
    frontier = unique_by_symbol(read_table(args.frontier_manifest, FRONTIER_FIELDS), "frontier")
    private_rows = read_table(args.private_manifest, PRIVATE_FIELDS)
    reviews = read_reviews(args.reviews)
    layout = load_layout(args.layout_manifest)

    private: dict[str, tuple[dict[str, str], bool]] = {}
    for row in private_rows:
        for symbol, is_data in ((row["data_symbol"], True), (row["size_symbol"], False)):
            if symbol in private:
                fail(f"duplicate private named-data symbol: {symbol}")
            private[symbol] = (row, is_data)

    result: list[dict[str, str]] = []
    used_reviews: set[str] = set()
    for external in discrete_rows(external_rows):
        symbol = external["symbol"]
        review = reviews.get(symbol)
        address: int | None = None
        extent: int | None = None
        status: str
        evidence: str
        detail: str

        if symbol in private:
            provider, is_data = private[symbol]
            address = parse_hex(
                provider["target_va"] if is_data else provider["size_word_va"],
                field=f"{symbol} target address",
            )
            extent = parse_hex(provider["size_hex"], field=f"{symbol} extent") if is_data else 4
            status = PRIVATE_BYTES
            evidence = "v86-private-reference-provider"
            detail = f"asset={provider['asset']};section={provider['section_name']}"
        elif review is not None:
            used_reviews.add(symbol)
            decision = review["decision"]
            address = parse_hex(review["target_address"], field=f"{symbol} target address", allow_blank=True)
            extent = parse_hex(review["extent_hex"], field=f"{symbol} extent", allow_blank=True)
            if decision == "source-refactor":
                status = SOURCE_REFACTOR
            elif extent:
                status = RANGE_PROVED
            else:
                status = ADDRESS_PROVED
            evidence = review["evidence"]
            detail = f"{review['detail']};evidence={review['evidence_path']}#{review['evidence_token']}"
        else:
            contract = contracts.get(symbol)
            provider = frontier.get(symbol)
            if contract is not None and contract["status"] == "RESOLVED" and contract["target_address"]:
                address = parse_hex(contract["target_address"], field=f"{symbol} target address")
                status = ADDRESS_PROVED
                evidence = "v85-absolute-address-anchor"
                detail = contract["detail"]
            elif provider is not None and provider["resolution_kind"] == "compatibility-storage" and provider["target_address"]:
                address = parse_hex(provider["target_address"], field=f"{symbol} target address")
                extent = parse_hex(provider["storage_size_hex"], field=f"{symbol} extent")
                status = RANGE_PROVED
                evidence = "v87-target-address+minimum-consumed-extent"
                detail = "target bytes/zero-fill fingerprinted; extent remains the source-consumed minimum"
            else:
                fail(f"unreviewed Stage-3C data blocker: {symbol}")

        if status == SOURCE_REFACTOR:
            region = "source-refactor"
        elif extent is None or extent == 0:
            region = "address-only"
        else:
            assert address is not None
            end = address + extent
            if address < int(layout["base"]) or end > int(layout["memory_end"]):
                fail(f"named-data range outside frozen memory span: {symbol}")
            if end <= int(layout["initialized_end"]):
                region = "initialized"
            elif address >= int(layout["initialized_end"]):
                region = "zero-fill"
            else:
                region = "initialized+zero-fill"

        result.append({
            "symbol": symbol,
            "category": external["category"],
            "status": status,
            "target_address": canonical_hex(address),
            "extent_hex": canonical_hex(extent),
            "region": region,
            "sha256": "",
            "evidence": evidence,
            "requesters": external["requesters"],
            "detail": detail,
        })

    unused = sorted(set(reviews) - used_reviews)
    if unused:
        fail(f"named-data reviews are not Stage-3C rows: {unused}")
    return result, layout


def range_bytes(reference: bytes, row: dict[str, str], layout: dict[str, int | str]) -> bytes:
    address = parse_hex(row["target_address"], field=f"{row['symbol']} target address")
    extent = parse_hex(row["extent_hex"], field=f"{row['symbol']} extent")
    assert address is not None and extent is not None
    base = int(layout["base"])
    initialized_end = int(layout["initialized_end"])
    end = address + extent
    initialized_part_end = min(end, initialized_end)
    data = b""
    if address < initialized_part_end:
        start_offset = address - base
        data = reference[start_offset:start_offset + initialized_part_end - address]
    if end > initialized_end:
        data += b"\0" * (end - max(address, initialized_end))
    if len(data) != extent:
        fail(f"failed to materialize complete named-data range: {row['symbol']}")
    return data


def fingerprint_rows(
    rows: Sequence[dict[str, str]], reference_path: Path, layout: dict[str, int | str],
) -> list[dict[str, str]]:
    if not reference_path.is_file():
        fail(f"missing private unpacked reference: {reference_path}")
    reference = reference_path.read_bytes()
    if len(reference) != int(layout["initialized_size"]):
        fail(f"private reference size mismatch: {len(reference)}")
    if sha256_bytes(reference) != layout["sha256"]:
        fail("private reference SHA-256 does not match the frozen layout oracle")
    result: list[dict[str, str]] = []
    for source in rows:
        row = dict(source)
        if row["status"] in {PRIVATE_BYTES, RANGE_PROVED}:
            row["sha256"] = sha256_bytes(range_bytes(reference, row, layout))
        result.append(row)
    return result


def read_manifest(path: Path) -> list[dict[str, str]]:
    rows = read_table(path, MANIFEST_FIELDS)
    if len(rows) != 54 or len({row["symbol"] for row in rows}) != 54:
        fail("named-data manifest must contain 54 unique symbols")
    return rows


def validate_manifest(args: argparse.Namespace) -> tuple[list[dict[str, str]], dict[str, int | str]]:
    expected, layout = base_rows(args)
    actual = read_manifest(args.manifest)
    actual_by_symbol = {row["symbol"]: row for row in actual}
    if set(actual_by_symbol) != {row["symbol"] for row in expected}:
        fail("named-data manifest symbol set drift")
    for base in expected:
        row = actual_by_symbol[base["symbol"]]
        for field in MANIFEST_FIELDS:
            if field == "sha256":
                continue
            if row[field] != base[field]:
                fail(f"named-data manifest metadata drift for {base['symbol']}: {field}")
        if row["status"] not in VALID_STATUSES:
            fail(f"invalid named-data status: {row['symbol']}")
        if row["status"] in {PRIVATE_BYTES, RANGE_PROVED}:
            if SHA_RE.fullmatch(row["sha256"]) is None:
                fail(f"range proof lacks SHA-256: {row['symbol']}")
        elif row["sha256"]:
            fail(f"non-range named-data row carries bytes hash: {row['symbol']}")
    return [actual_by_symbol[row["symbol"]] for row in expected], layout


def summarize(rows: Sequence[dict[str, str]]) -> dict[str, object]:
    statuses = Counter(row["status"] for row in rows)
    regions = Counter(row["region"] for row in rows)
    addressed = sum(bool(row["target_address"]) for row in rows)
    fingerprinted = statuses[PRIVATE_BYTES] + statuses[RANGE_PROVED]
    return {
        "total": len(rows),
        "private_bytes": statuses[PRIVATE_BYTES],
        "range_proved": statuses[RANGE_PROVED],
        "address_only": statuses[ADDRESS_PROVED],
        "source_refactor": statuses[SOURCE_REFACTOR],
        "addressed": addressed,
        "fingerprinted": fingerprinted,
        "regions": dict(sorted(regions.items())),
    }


def exact_provider_rows(
    rows: Sequence[dict[str, str]], frontier_rows: Sequence[dict[str, str]],
) -> list[dict[str, str]]:
    """Return V87 compatibility stores replaceable by proved Stage-3C ranges."""
    named = {row["symbol"]: row for row in rows}
    result = [
        named[row["symbol"]]
        for row in frontier_rows
        if row["resolution_kind"] == COMPAT_STORAGE
        and row["symbol"] in named
        and named[row["symbol"]]["status"] == RANGE_PROVED
    ]
    if len(result) != 33:
        fail(f"expected 33 exact named-data provider replacements, found {len(result)}")
    return sorted(result, key=lambda row: (int(row["target_address"], 0), row["symbol"]))


def cluster_ranges(rows: Sequence[dict[str, str]]) -> list[dict[str, object]]:
    intervals = [
        (
            int(row["target_address"], 0),
            int(row["target_address"], 0) + int(row["extent_hex"], 0),
            row,
        )
        for row in rows
    ]
    intervals.sort(key=lambda item: (item[0], item[1], item[2]["symbol"]))
    clusters: list[dict[str, object]] = []
    for start, end, row in intervals:
        if not clusters or start > int(clusters[-1]["end"]):
            clusters.append({"start": start, "end": end, "rows": [row]})
        else:
            clusters[-1]["end"] = max(int(clusters[-1]["end"]), end)
            cast_rows = clusters[-1]["rows"]
            assert isinstance(cast_rows, list)
            cast_rows.append(row)
    if len(clusters) != 9:
        fail(f"expected nine exact named-data range clusters, found {len(clusters)}")
    return clusters


def render_exact_provider_assembly(
    rows: Sequence[dict[str, str]],
    reference: bytes,
    layout: dict[str, int | str],
    build_dir: Path,
) -> tuple[str, dict[str, dict[str, object]]]:
    """Render private target-byte sections and return their expected fingerprints."""
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
        # Keep each private range byte-exact.  The target virtual address is
        # recorded separately; imposing its natural alignment here would make
        # GAS append zero padding to the section and claim bytes past the
        # audited range.  Final placement/alignment belongs to Stage 3G.
        alignment = 1
        zero_fill = start >= int(layout["initialized_end"])
        section = f".bss.stage3c.va_{start:08x}" if zero_fill else f".data.stage3c.va_{start:08x}"
        base_symbol = f"__stage3c_range_{start:08x}"
        if zero_fill:
            lines.append(f'.section {section},"aw",@nobits')
            lines.append(f".balign {alignment}")
            lines.append(f"{base_symbol}:")
            lines.append(f".space 0x{size:x}")
            digest = None
            section_type = 8
        else:
            material = range_bytes(
                reference,
                {
                    "symbol": base_symbol,
                    "target_address": f"0x{start:x}",
                    "extent_hex": f"0x{size:x}",
                },
                layout,
            )
            binary = build_dir / f"range_{start:08x}_{end:08x}.bin"
            binary.write_bytes(material)
            if '"' in str(binary):
                fail(f"unsupported quote in private range path: {binary}")
            lines.append(f'.section {section},"aw",@progbits')
            lines.append(f".balign {alignment}")
            lines.append(f"{base_symbol}:")
            lines.append(f'.incbin "{binary}"')
            digest = sha256_bytes(material)
            section_type = 1
        for row in members:
            offset = int(row["target_address"], 0) - start
            extent = int(row["extent_hex"], 0)
            lines.extend(
                [
                    f".globl {row['symbol']}",
                    f".type {row['symbol']}, @object",
                    f"{row['symbol']} = {base_symbol} + 0x{offset:x}",
                    f".size {row['symbol']}, 0x{extent:x}",
                ]
            )
        lines.append("")
        expected[section] = {
            "type": section_type,
            "flags": 3,
            "size": size,
            "alignment": alignment,
            "sha256": digest,
        }
    return "\n".join(lines), dict(sorted(expected.items()))


def link_exact_providers(
    args: argparse.Namespace,
    named_rows: Sequence[dict[str, str]],
    layout: dict[str, int | str],
) -> dict[str, object]:
    if not args.input.is_file():
        fail(f"missing V86 private-asset aggregate: {args.input}")
    reference = args.reference.read_bytes() if args.reference.is_file() else b""
    if len(reference) != int(layout["initialized_size"]) or sha256_bytes(reference) != layout["sha256"]:
        fail("private unpacked reference is missing or does not match the layout oracle")

    frontier_rows = read_table(args.frontier_manifest, FRONTIER_FIELDS)
    if len(frontier_rows) != 251:
        fail(f"expected V87 frontier of 251 rows, found {len(frontier_rows)}")
    replacements = exact_provider_rows(named_rows, frontier_rows)
    replacement_names = {row["symbol"] for row in replacements}
    remaining_frontier = [
        row for row in frontier_rows
        if not (row["resolution_kind"] == COMPAT_STORAGE and row["symbol"] in replacement_names)
    ]

    compiler = resolve_tool(args.compiler)
    assembler = sibling_tool(compiler, args.as_tool, "as")
    linker = sibling_tool(compiler, args.ld, "ld")
    nm = sibling_tool(compiler, args.nm, "nm")
    args.build_dir.mkdir(parents=True, exist_ok=True)

    exact_source = args.build_dir / "exact_named_data.S"
    exact_object = args.build_dir / "exact_named_data.o"
    storage_source = args.build_dir / "remaining_compatibility_storage.S"
    storage_object = args.build_dir / "remaining_compatibility_storage.o"
    runtime_source = args.build_dir / "runtime_shims.c"
    runtime_object = args.build_dir / "runtime_shims.o"
    provider_object = args.build_dir / "stage3c_providers.o"

    exact_assembly, expected_exact_sections = render_exact_provider_assembly(
        replacements, reference, layout, args.build_dir,
    )
    exact_source.write_text(exact_assembly, encoding="utf-8")
    storage_source.write_text(render_storage_assembly(remaining_frontier), encoding="utf-8")
    runtime_source.write_text(render_runtime_c(), encoding="utf-8")

    run([str(assembler), "-EL", "-o", str(exact_object), str(exact_source)])
    run([str(assembler), "-EL", "-o", str(storage_object), str(storage_source)])
    run(
        [
            str(compiler), "-G0", "-O2", "-EL", "-fomit-frame-pointer", "-fno-common",
            "-ffreestanding", "-fno-builtin", "-fshort-double", "-mlong64",
            "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
            "-ffunction-sections", "-w", "-c", str(runtime_source), "-o", str(runtime_object),
        ]
    )
    run(
        [
            str(linker), "-EL", "-r", "-o", str(provider_object),
            str(exact_object), str(storage_object), str(runtime_object),
        ]
    )

    exact_sections = {
        name: value
        for name, value in alloc_section_fingerprints(exact_object).items()
        if value["size"] != 0
    }
    if exact_sections != expected_exact_sections:
        fail("assembled Stage-3C exact sections differ from private range fingerprints")

    provider_symbols = global_symbols(nm, provider_object)
    provider_by_name = symbol_map(provider_symbols)
    expected_defined = {
        row["symbol"]
        for row in frontier_rows
        if row["resolution_kind"] in {COMPAT_STORAGE, RUNTIME_SHIM}
    }
    provider_defined = {
        name for name, symbol in provider_by_name.items() if not symbol.undefined
    }
    provider_undefined = {
        name for name, symbol in provider_by_name.items() if symbol.undefined
    }
    if provider_defined != expected_defined or provider_undefined != {"ps2lib_vsnprintf_recovered"}:
        fail(
            "Stage-3C provider symbol drift; "
            f"missing={sorted(expected_defined - provider_defined)[:5]} "
            f"extra={sorted(provider_defined - expected_defined)[:5]} "
            f"undefined={sorted(provider_undefined)}"
        )

    input_symbols = global_symbols(nm, args.input)
    input_undefined = verify_input_frontier(input_symbols, frontier_rows)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    map_path = args.output.with_suffix(".map")
    command = [str(linker), "-EL", "-r", "-Map", str(map_path)]
    for row in frontier_rows:
        if row["resolution_kind"] == ABSOLUTE_ANCHOR:
            command.extend(["--defsym", f"{row['symbol']}={row['target_address']}"])
        elif row["resolution_kind"] == SEMANTIC_ALIAS:
            command.extend(["--defsym", f"{row['symbol']}={row['target_symbol']}"])
    command.extend(["-o", str(args.output), str(args.input), str(provider_object)])
    run(command)

    output_symbols = global_symbols(nm, args.output)
    output_undefined = {symbol.name for symbol in output_symbols if symbol.undefined}
    if output_undefined:
        fail(f"Stage-3C aggregate still has externals: {sorted(output_undefined)[:10]}")

    input_sections = alloc_section_fingerprints(args.input)
    output_sections = alloc_section_fingerprints(args.output)
    for name, fingerprint in input_sections.items():
        if output_sections.get(name) != fingerprint:
            fail(f"existing allocated section changed during Stage 3C: {name}")
    added_sections = {
        name: value for name, value in output_sections.items() if name not in input_sections
    }
    provider_sections = {
        name: value
        for name, value in alloc_section_fingerprints(provider_object).items()
        if value["size"] != 0
    }
    if added_sections != provider_sections:
        fail("Stage-3C aggregate did not add exactly the generated provider sections")

    compatibility_remaining = sum(
        row["resolution_kind"] == COMPAT_STORAGE for row in remaining_frontier
    )
    report: dict[str, object] = {
        "schema": 1,
        "claim": "stage3c-private-exact-named-data-providers",
        **summarize(named_rows),
        "exact_provider_symbols": len(replacements),
        "exact_provider_clusters": len(expected_exact_sections),
        "compatibility_storage_before": 44,
        "compatibility_storage_after": compatibility_remaining,
        "input_external_symbols": len(input_undefined),
        "output_external_symbols": len(output_undefined),
        "existing_allocated_sections_unchanged": True,
        "exact_sections": expected_exact_sections,
        "input_sha256": sha256_file(args.input),
        "provider_object_sha256": sha256_file(provider_object),
        "output_sha256": sha256_file(args.output),
        "manifest_sha256": sha256_file(args.manifest),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return report


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="action", required=True)
    for action in ("validate", "verify", "refresh", "link"):
        sub = subparsers.add_parser(action)
        sub.add_argument("--external-map", type=Path, default=DEFAULT_EXTERNAL)
        sub.add_argument("--contracts", type=Path, default=DEFAULT_CONTRACTS)
        sub.add_argument("--private-manifest", type=Path, default=DEFAULT_PRIVATE)
        sub.add_argument("--frontier-manifest", type=Path, default=DEFAULT_FRONTIER)
        sub.add_argument("--layout-manifest", type=Path, default=DEFAULT_LAYOUT)
        sub.add_argument("--reviews", type=Path, default=DEFAULT_REVIEWS)
        sub.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
        if action in {"verify", "refresh", "link"}:
            sub.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
        if action in {"verify", "link"}:
            sub.add_argument("--report", type=Path, default=DEFAULT_REPORT)
        if action == "link":
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
            rows, layout = base_rows(args)
            rows = fingerprint_rows(rows, args.reference, layout)
            args.manifest.parent.mkdir(parents=True, exist_ok=True)
            args.manifest.write_text(render_tsv(rows), encoding="utf-8")
            action = "refreshed"
        else:
            rows, layout = validate_manifest(args)
            action = "validated"
            if args.action in {"verify", "link"}:
                expected = fingerprint_rows(rows, args.reference, layout)
                for actual, recomputed in zip(rows, expected):
                    if actual["sha256"] != recomputed["sha256"]:
                        fail(f"private named-data fingerprint mismatch: {actual['symbol']}")
                if args.action == "link":
                    report = link_exact_providers(args, rows, layout)
                    action = "linked"
                else:
                    report = {
                        "schema": 1,
                        "claim": "stage3c-discrete-data-oracle",
                        **summarize(rows),
                        "reference_sha256": sha256_file(args.reference),
                        "manifest_sha256": sha256_file(args.manifest),
                    }
                    args.report.parent.mkdir(parents=True, exist_ok=True)
                    args.report.write_text(
                        json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8",
                    )
                    action = "verified"
    except (NamedDataError, json.JSONDecodeError) as exc:
        print(f"named-data gate failed: {exc}", file=sys.stderr)
        return 1

    summary = summarize(rows)
    print(
        f"{action} Stage-3C named data: total={summary['total']} "
        f"fingerprinted={summary['fingerprinted']} addressed={summary['addressed']} "
        f"address_only={summary['address_only']} source_refactor={summary['source_refactor']}"
    )
    if args.action == "link":
        print(
            f"exact Stage-3C providers: {report['exact_provider_symbols']} symbols in "
            f"{report['exact_provider_clusters']} clusters; compatibility storage "
            f"{report['compatibility_storage_before']} -> {report['compatibility_storage_after']}"
        )
        print(
            f"partial-link externals: {report['input_external_symbols']} -> "
            f"{report['output_external_symbols']}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
