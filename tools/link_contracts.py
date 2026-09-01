#!/usr/bin/env python3
"""Freeze and apply the zero-byte whole-program link-contract frontier.

This gate starts from the V84 source-address-alias aggregate.  It may resolve
an external in exactly two byte-free ways:

* assign a target-address data name an absolute value already encoded in its
  audited symbol spelling; or
* bind a uniquely identified call contract to an existing recovered global
  text definition.

Neither operation allocates storage, selects an archive member, or claims a
replacement ELF.  Everything not proved by those rules remains an explicit
provider frontier for the next Stage-3 batch.
"""
from __future__ import annotations

import argparse
import csv
import json
import re
import sys
from collections import Counter, defaultdict, deque
from io import StringIO
from pathlib import Path
from typing import Iterable, Sequence

from source_aliases import (
    AliasError as SourceAliasError,
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
DEFAULT_EXTERNAL = ROOT / "analysis" / "source_tree" / "external_symbol_ownership.tsv"
DEFAULT_DEFINED = ROOT / "analysis" / "source_tree" / "defined_symbol_ownership.tsv"
DEFAULT_PROGRESS = ROOT / "analysis" / "progress_targets.csv"
DEFAULT_SOURCE_ALIASES = ROOT / "analysis" / "link_identity" / "source_address_aliases.tsv"
DEFAULT_LAYOUT = ROOT / "analysis" / "link_identity" / "unpacked_layout.json"
DEFAULT_REVIEWS = ROOT / "analysis" / "link_identity" / "link_contract_reviews.tsv"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "link_contracts.tsv"
DEFAULT_INPUT = ROOT / "build" / "source-aliases" / "source-tree.alias-resolved.partial.o"
DEFAULT_BUILD = ROOT / "build" / "link-contracts"
DEFAULT_OUTPUT = DEFAULT_BUILD / "source-tree.link-contracts.partial.o"
DEFAULT_REPORT = DEFAULT_BUILD / "report.json"

EXTERNAL_FIELDS = (
    "symbol",
    "category",
    "provider_kind",
    "owner",
    "resolution_gate",
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
PROGRESS_FIELDS = ("address", "name", "area", "status", "confidence", "notes")
SOURCE_ALIAS_FIELDS = (
    "alias",
    "target_address",
    "status",
    "canonical_symbol",
    "evidence",
    "canonical_source",
    "canonical_object",
    "requesters",
    "detail",
)
REVIEW_FIELDS = (
    "symbol",
    "canonical_symbol",
    "target_address",
    "evidence_path",
    "evidence_token",
    "detail",
)
MANIFEST_FIELDS = (
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

ADDRESS_RE = re.compile(r"(?:^|_)([0-9a-fA-F]{8})$")
TRAILING_ADDRESS_RE = re.compile(r"_[0-9a-fA-F]{8}$")
RESOLVED = "RESOLVED"
BLOCKED = "BLOCKED"
ABSOLUTE_ANCHOR = "absolute-address-anchor"
SEMANTIC_ALIAS = "semantic-text-alias"
SEMANTIC_PROVIDERS = {"historical-archive", "link-contract", "source-or-archive", "recovered-runtime"}


class ContractError(RuntimeError):
    """A frozen link-contract invariant failed."""


def fail(message: str) -> None:
    raise ContractError(message)


def parse_address(value: str, *, context: str) -> int:
    try:
        return int(value, 16)
    except ValueError as exc:
        fail(f"invalid address for {context}: {value!r}")
        raise AssertionError from exc


def suffix_address(symbol: str) -> int | None:
    match = ADDRESS_RE.search(symbol)
    return int(match.group(1), 16) if match is not None else None


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


def read_layout_bounds(path: Path) -> tuple[int, int]:
    if not path.is_file():
        fail(f"missing unpacked layout manifest: {path}")
    try:
        payload = json.loads(path.read_text(encoding="utf-8"))
        image = payload["image"]
        base = int(image["base_address"])
        end = int(image["memory_end_address"])
    except (json.JSONDecodeError, KeyError, TypeError, ValueError) as exc:
        fail(f"invalid unpacked layout bounds: {path}")
        raise AssertionError from exc
    if base < 0 or end <= base:
        fail(f"invalid unpacked layout interval: 0x{base:x}..0x{end:x}")
    return base, end


def read_reviews(path: Path) -> list[dict[str, str]]:
    rows = read_table(path, REVIEW_FIELDS, delimiter="\t")
    seen: set[str] = set()
    for row in rows:
        symbol = row["symbol"]
        if not symbol or symbol in seen:
            fail(f"duplicate or empty reviewed symbol: {symbol!r}")
        seen.add(symbol)
        if not row["canonical_symbol"] or not row["detail"] or not row["evidence_token"]:
            fail(f"incomplete reviewed contract: {symbol}")
        parse_address(row["target_address"], context=f"review {symbol}")
        relative = Path(row["evidence_path"])
        if relative.is_absolute() or ".." in relative.parts:
            fail(f"review evidence path must stay repository-relative: {symbol}")
        evidence_path = ROOT / relative
        if not evidence_path.is_file():
            fail(f"missing reviewed evidence for {symbol}: {relative}")
        evidence_text = evidence_path.read_text(encoding="utf-8", errors="replace")
        address = f"0x{parse_address(row['target_address'], context=symbol):08x}"
        if address not in evidence_text or row["evidence_token"] not in evidence_text:
            fail(
                f"review evidence drift for {symbol}: expected {address} and "
                f"{row['evidence_token']!r} in {relative}"
            )
    return rows


def normalized_names(symbol: str) -> list[tuple[str, str]]:
    """Return deterministic spellings reached by conservative suffix removal."""
    queue: deque[tuple[str, tuple[str, ...]]] = deque([(symbol, ())])
    best: dict[str, tuple[str, ...]] = {}
    while queue:
        name, transforms = queue.popleft()
        previous = best.get(name)
        if previous is not None and (len(previous), previous) <= (len(transforms), transforms):
            continue
        best[name] = transforms
        if TRAILING_ADDRESS_RE.search(name):
            queue.append((TRAILING_ADDRESS_RE.sub("", name), transforms + ("strip-address",)))
        for suffix, label in (("_like", "strip-like"), ("_recovered", "strip-recovered")):
            if name.endswith(suffix):
                queue.append((name[: -len(suffix)], transforms + (label,)))

    result: list[tuple[str, str]] = []
    for name, transforms in sorted(best.items(), key=lambda item: (len(item[1]), item[1], item[0])):
        evidence = "exact-progress-name" if not transforms else "+".join(transforms)
        result.append((name, evidence))
    return result


def canonical_candidates(
    progress: dict[str, str],
    global_text_by_name: dict[str, dict[str, str]],
    global_text_by_suffix: dict[int, list[dict[str, str]]],
    source_alias_targets: dict[int, set[str]],
) -> list[dict[str, str]]:
    address = parse_address(progress["address"], context=progress["name"])
    names = {
        progress["name"],
        progress["name"] + "_recovered",
        "snes_" + progress["name"],
        "snes_" + progress["name"].lstrip("_"),
    }
    names.update(source_alias_targets.get(address, set()))
    candidates: dict[str, dict[str, str]] = {}
    for name in names:
        row = global_text_by_name.get(name)
        if row is not None:
            candidates[name] = row
    for row in global_text_by_suffix.get(address, []):
        candidates[row["symbol"]] = row
    return [candidates[name] for name in sorted(candidates)]


def blocker_for(external: dict[str, str], source_block: dict[str, str] | None) -> tuple[str, str]:
    provider = external["provider_kind"]
    if provider == "source-address-alias":
        evidence = source_block["evidence"] if source_block else "source-address-alias-unproved"
        detail = source_block["detail"] if source_block else "not proved by the V84 address-alias gate"
        return "blocked-source-address-alias", f"{evidence};{detail}".rstrip(";")
    if provider == "program-data":
        return "blocked-program-data-storage", "named program data still needs storage, bytes, size, and section ownership"
    if provider == "private-asset":
        return "blocked-private-asset", "private asset bytes and extraction provenance are intentionally absent"
    if provider == "historical-archive":
        return "blocked-historical-archive", "historical archive revision and member identity are not yet proved"
    if provider == "recovered-runtime":
        return "deferred-runtime-alias", "target-selected override is bound by the reviewed provider-frontier alias gate"
    if provider == "source-or-archive":
        return "blocked-source-or-archive", "source definition versus historical archive provider is not yet proved"
    if provider == "link-contract":
        return "blocked-link-contract", "no unique recovered global text contract is proved"
    return "blocked-unclassified-provider", f"unclassified provider kind: {provider}"


def derive_rows(
    external_rows: Sequence[dict[str, str]],
    defined_rows: Sequence[dict[str, str]],
    progress_rows: Sequence[dict[str, str]],
    source_alias_rows: Sequence[dict[str, str]],
    review_rows: Sequence[dict[str, str]],
    layout_base: int,
    layout_end: int,
) -> list[dict[str, str]]:
    external_by_name: dict[str, dict[str, str]] = {}
    for row in external_rows:
        symbol = row["symbol"]
        if symbol in external_by_name:
            fail(f"duplicate external ownership symbol: {symbol}")
        external_by_name[symbol] = row

    global_text_by_name: dict[str, dict[str, str]] = {}
    global_text_by_suffix: dict[int, list[dict[str, str]]] = defaultdict(list)
    for row in defined_rows:
        if row["binding"] != "global" or row["section_class"] != "text":
            continue
        symbol = row["symbol"]
        if symbol in global_text_by_name:
            fail(f"duplicate global text definition: {symbol}")
        global_text_by_name[symbol] = row
        address = suffix_address(symbol)
        if address is not None:
            global_text_by_suffix[address].append(row)

    proved_source_aliases: set[str] = set()
    source_blockers: dict[str, dict[str, str]] = {}
    source_alias_targets: dict[int, set[str]] = defaultdict(set)
    seen_source_aliases: set[str] = set()
    for row in source_alias_rows:
        alias = row["alias"]
        if alias in seen_source_aliases or alias not in external_by_name:
            fail(f"invalid source-address alias manifest member: {alias}")
        seen_source_aliases.add(alias)
        if row["status"] == "PROVED":
            proved_source_aliases.add(alias)
            address = parse_address(row["target_address"], context=alias)
            source_alias_targets[address].add(row["canonical_symbol"])
        elif row["status"] == BLOCKED:
            source_blockers[alias] = row
        else:
            fail(f"invalid source-address alias status for {alias}: {row['status']}")

    remaining = {
        name: row for name, row in external_by_name.items() if name not in proved_source_aliases
    }

    progress_by_name: dict[str, list[dict[str, str]]] = defaultdict(list)
    progress_by_address: dict[int, dict[str, str]] = {}
    for row in progress_rows:
        if row["status"] != "MATCHING":
            continue
        address = parse_address(row["address"], context=row["name"])
        if address in progress_by_address:
            fail(f"duplicate MATCHING progress address: 0x{address:08x}")
        progress_by_address[address] = row
        progress_by_name[row["name"]].append(row)

    reviews: dict[str, dict[str, str]] = {}
    for row in review_rows:
        symbol = row["symbol"]
        if symbol in reviews or symbol not in remaining:
            fail(f"reviewed contract is not a remaining external: {symbol}")
        external = remaining[symbol]
        if external["provider_kind"] not in SEMANTIC_PROVIDERS:
            fail(f"reviewed semantic contract has incompatible provider: {symbol}")
        canonical = global_text_by_name.get(row["canonical_symbol"])
        if canonical is None:
            fail(f"reviewed canonical symbol is not global text: {row['canonical_symbol']}")
        address = parse_address(row["target_address"], context=symbol)
        if not layout_base <= address < layout_end:
            fail(f"reviewed target is outside the unpacked layout: {symbol}")
        progress = progress_by_address.get(address)
        if progress is None:
            fail(f"reviewed target is not a MATCHING progress row: {symbol}")
        encoded = suffix_address(symbol)
        if encoded is not None and encoded != address:
            fail(f"review target conflicts with address-qualified symbol: {symbol}")
        reviews[symbol] = row

    result: list[dict[str, str]] = []
    for symbol, external in sorted(remaining.items()):
        target_address = ""
        status = BLOCKED
        resolution_kind = ""
        canonical_symbol = ""
        canonical_source = ""
        canonical_object = ""
        evidence = ""
        detail = ""
        encoded = suffix_address(symbol)

        if external["provider_kind"] == "program-data" and encoded is not None:
            if not layout_base <= encoded < layout_end:
                fail(f"target-address data anchor is outside the unpacked layout: {symbol}")
            target_address = f"0x{encoded:08x}"
            status = RESOLVED
            resolution_kind = ABSOLUTE_ANCHOR
            evidence = "address-suffix-inside-frozen-layout"
            detail = "absolute value only; no storage, section bytes, size, or alignment emitted"
        elif symbol in reviews:
            review = reviews[symbol]
            canonical = global_text_by_name[review["canonical_symbol"]]
            target_address = f"0x{parse_address(review['target_address'], context=symbol):08x}"
            status = RESOLVED
            resolution_kind = SEMANTIC_ALIAS
            canonical_symbol = canonical["symbol"]
            canonical_source = canonical["source"]
            canonical_object = canonical["object"]
            evidence = "reviewed-semantic-contract"
            detail = (
                f"{review['detail']};evidence={review['evidence_path']}"
                f"#{review['evidence_token']}"
            )
        elif external["provider_kind"] in SEMANTIC_PROVIDERS:
            progress_matches: dict[tuple[int, str], tuple[dict[str, str], str]] = {}
            for name, name_evidence in normalized_names(symbol):
                for progress in progress_by_name.get(name, []):
                    address = parse_address(progress["address"], context=name)
                    if encoded is not None and encoded != address:
                        continue
                    key = (address, progress["name"])
                    previous = progress_matches.get(key)
                    if previous is None or name_evidence < previous[1]:
                        progress_matches[key] = (progress, name_evidence)

            if len(progress_matches) == 1:
                progress, name_evidence = next(iter(progress_matches.values()))
                candidates = canonical_candidates(
                    progress,
                    global_text_by_name,
                    global_text_by_suffix,
                    source_alias_targets,
                )
                target_address = f"0x{parse_address(progress['address'], context=symbol):08x}"
                if len(candidates) == 1:
                    canonical = candidates[0]
                    status = RESOLVED
                    resolution_kind = SEMANTIC_ALIAS
                    canonical_symbol = canonical["symbol"]
                    canonical_source = canonical["source"]
                    canonical_object = canonical["object"]
                    evidence = f"{name_evidence}+unique-recovered-global-text"
                    detail = f"progress={progress['name']}"
                elif len(candidates) > 1:
                    resolution_kind = "blocked-ambiguous-global-text"
                    evidence = "multiple-recovered-global-text-candidates"
                    detail = ";".join(row["symbol"] for row in candidates)
                else:
                    resolution_kind = "blocked-unexported-progress-target"
                    evidence = "matching-progress-target-not-exported"
                    detail = f"progress={progress['name']}"
            elif len(progress_matches) > 1:
                resolution_kind = "blocked-ambiguous-progress-name"
                evidence = "multiple-matching-progress-targets"
                detail = ";".join(
                    f"0x{address:08x}:{name}" for address, name in sorted(progress_matches)
                )
            else:
                resolution_kind, detail = blocker_for(external, source_blockers.get(symbol))
                evidence = "no-unique-matching-progress-contract"
        else:
            resolution_kind, detail = blocker_for(external, source_blockers.get(symbol))
            evidence = "provider-frontier"

        result.append(
            {
                "symbol": symbol,
                "target_address": target_address,
                "status": status,
                "resolution_kind": resolution_kind,
                "canonical_symbol": canonical_symbol,
                "evidence": evidence,
                "category": external["category"],
                "provider_kind": external["provider_kind"],
                "canonical_source": canonical_source,
                "canonical_object": canonical_object,
                "requesters": external["requesters"],
                "detail": detail,
            }
        )
    return result


def expected_manifest(args: argparse.Namespace) -> tuple[list[dict[str, str]], str]:
    external_rows = read_table(args.external_map, EXTERNAL_FIELDS, delimiter="\t")
    defined_rows = read_table(args.defined_map, DEFINED_FIELDS, delimiter="\t")
    progress_rows = read_table(args.progress_manifest, PROGRESS_FIELDS, delimiter=",")
    source_alias_rows = read_table(args.source_alias_manifest, SOURCE_ALIAS_FIELDS, delimiter="\t")
    review_rows = read_reviews(args.reviews)
    layout_base, layout_end = read_layout_bounds(args.layout_manifest)
    rows = derive_rows(
        external_rows,
        defined_rows,
        progress_rows,
        source_alias_rows,
        review_rows,
        layout_base,
        layout_end,
    )
    return rows, render_tsv(rows)


def validate_frozen_manifest(args: argparse.Namespace) -> list[dict[str, str]]:
    rows, expected = expected_manifest(args)
    if not args.manifest.is_file():
        fail(f"missing link-contract manifest: {args.manifest}")
    if args.manifest.read_text(encoding="utf-8") != expected:
        fail("link-contract manifest is stale; review inputs and run tools/link_contracts.py refresh")
    return rows


def summarize(rows: Sequence[dict[str, str]]) -> dict[str, object]:
    status = Counter(row["status"] for row in rows)
    kinds = Counter(row["resolution_kind"] for row in rows)
    evidence = Counter(row["evidence"] for row in rows)
    providers = Counter(row["provider_kind"] for row in rows if row["status"] == BLOCKED)
    return {
        "contracts_total": len(rows),
        "resolved": status[RESOLVED],
        "blocked": status[BLOCKED],
        "address_anchors": kinds[ABSOLUTE_ANCHOR],
        "semantic_aliases": kinds[SEMANTIC_ALIAS],
        "resolution_kind_counts": dict(sorted(kinds.items())),
        "evidence_counts": dict(sorted(evidence.items())),
        "blocked_provider_counts": dict(sorted(providers.items())),
    }


def verify_link_result(
    input_symbols: Sequence[NmSymbol],
    output_symbols: Sequence[NmSymbol],
    rows: Sequence[dict[str, str]],
) -> tuple[int, int]:
    expected_input = {row["symbol"] for row in rows}
    input_undefined = {symbol.name for symbol in input_symbols if symbol.undefined}
    if input_undefined != expected_input:
        missing = sorted(expected_input - input_undefined)[:5]
        extra = sorted(input_undefined - expected_input)[:5]
        fail(f"V84 aggregate/contract manifest drift; missing={missing} extra={extra}")

    blocked = {row["symbol"] for row in rows if row["status"] == BLOCKED}
    output_undefined = {symbol.name for symbol in output_symbols if symbol.undefined}
    if output_undefined != blocked:
        missing = sorted(blocked - output_undefined)[:5]
        extra = sorted(output_undefined - blocked)[:5]
        fail(f"link-contract undefined set is wrong; missing={missing} extra={extra}")

    output_defined: dict[str, NmSymbol] = {}
    for symbol in output_symbols:
        if symbol.undefined:
            continue
        if symbol.name in output_defined:
            fail(f"duplicate global output symbol: {symbol.name}")
        output_defined[symbol.name] = symbol

    for row in rows:
        if row["status"] != RESOLVED:
            continue
        symbol = output_defined.get(row["symbol"])
        if symbol is None:
            fail(f"linker did not define resolved contract: {row['symbol']}")
        if row["resolution_kind"] == ABSOLUTE_ANCHOR:
            if symbol.type_code.upper() != "A":
                fail(f"address anchor is not absolute: {row['symbol']} ({symbol.type_code})")
            if int(symbol.value, 16) != parse_address(row["target_address"], context=row["symbol"]):
                fail(f"address anchor value drift: {row['symbol']}")
        elif row["resolution_kind"] == SEMANTIC_ALIAS:
            canonical = output_defined.get(row["canonical_symbol"])
            if canonical is None:
                fail(f"semantic canonical symbol is absent: {row['canonical_symbol']}")
            if symbol.value != canonical.value or symbol.type_code.upper() != canonical.type_code.upper():
                fail(f"semantic alias differs from canonical: {row['symbol']} != {row['canonical_symbol']}")
        else:
            fail(f"unknown resolved contract kind: {row['resolution_kind']}")
    return len(input_undefined), len(output_undefined)


def link_contracts(args: argparse.Namespace, rows: Sequence[dict[str, str]]) -> dict[str, object]:
    if not args.input.is_file():
        fail(f"missing V84 alias-resolved aggregate: {args.input}")
    compiler = resolve_tool(args.compiler)
    linker = sibling_tool(compiler, args.ld, "ld")
    nm = sibling_tool(compiler, args.nm, "nm")
    input_symbols = global_symbols(nm, args.input)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    map_path = args.output.with_suffix(".map")
    command = [str(linker), "-EL", "-r", "-Map", str(map_path)]
    for row in rows:
        if row["status"] != RESOLVED:
            continue
        target = row["canonical_symbol"]
        if row["resolution_kind"] == ABSOLUTE_ANCHOR:
            target = row["target_address"]
        command.extend(["--defsym", f"{row['symbol']}={target}"])
    command.extend(["-o", str(args.output), str(args.input)])
    run(command)

    output_symbols = global_symbols(nm, args.output)
    input_count, output_count = verify_link_result(input_symbols, output_symbols, rows)
    input_sections = alloc_section_fingerprints(args.input)
    output_sections = alloc_section_fingerprints(args.output)
    if output_sections != input_sections:
        fail("allocated sections changed while applying zero-byte link contracts")

    summary = summarize(rows)
    report: dict[str, object] = {
        "schema": 1,
        "claim": "zero-byte-whole-program-link-contract-frontier",
        **summary,
        "input_external_symbols": input_count,
        "output_external_symbols": output_count,
        "allocated_sections_unchanged": True,
        "emitted_code_bytes": 0,
        "emitted_data_bytes": 0,
        "input_sha256": sha256_file(args.input),
        "output_sha256": sha256_file(args.output),
        "manifest_sha256": sha256_file(args.manifest),
        "review_manifest_sha256": sha256_file(args.reviews),
        "layout_manifest_sha256": sha256_file(args.layout_manifest),
        "allocated_sections": input_sections,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    markdown = [
        "# Zero-byte link-contract partial-link report",
        "",
        f"- Contracts resolved: **{summary['resolved']}/{summary['contracts_total']}**",
        f"- Absolute target-address anchors: **{summary['address_anchors']}**",
        f"- Semantic text aliases: **{summary['semantic_aliases']}**",
        f"- Provider frontier: **{summary['blocked']}**",
        f"- Aggregate externals: **{input_count} -> {output_count}**",
        "- Allocated sections changed: **no**",
        "- Code/data bytes emitted: **0**",
        "",
        "This is a relocatable link-contract checkpoint, not a replacement ELF.",
        "",
    ]
    args.report.with_suffix(".md").write_text("\n".join(markdown), encoding="utf-8")
    return report


def add_manifest_arguments(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--external-map", type=Path, default=DEFAULT_EXTERNAL)
    parser.add_argument("--defined-map", type=Path, default=DEFAULT_DEFINED)
    parser.add_argument("--progress-manifest", type=Path, default=DEFAULT_PROGRESS)
    parser.add_argument("--source-alias-manifest", type=Path, default=DEFAULT_SOURCE_ALIASES)
    parser.add_argument("--layout-manifest", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reviews", type=Path, default=DEFAULT_REVIEWS)


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="action", required=True)
    validate_parser = subparsers.add_parser("validate", help="verify the public contract frontier")
    add_manifest_arguments(validate_parser)
    refresh_parser = subparsers.add_parser("refresh", help="refresh the public contract frontier")
    add_manifest_arguments(refresh_parser)
    link_parser = subparsers.add_parser("link", help="apply resolved zero-byte contracts")
    add_manifest_arguments(link_parser)
    link_parser.add_argument("--compiler", default="ee-gcc")
    link_parser.add_argument("--ld")
    link_parser.add_argument("--nm")
    link_parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    link_parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    link_parser.add_argument("--report", type=Path, default=DEFAULT_REPORT)
    args = parser.parse_args(argv)
    for name in (
        "external_map",
        "defined_map",
        "progress_manifest",
        "source_alias_manifest",
        "layout_manifest",
        "manifest",
        "reviews",
    ):
        value = getattr(args, name)
        if not value.is_absolute():
            setattr(args, name, (ROOT / value).resolve())
    if args.action == "link":
        for name in ("input", "output", "report"):
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
            action = "refreshed"
            report = summarize(rows)
        else:
            rows = validate_frozen_manifest(args)
            action = "verified"
            report = summarize(rows)
            if args.action == "link":
                report = link_contracts(args, rows)
                action = "linked"
    except (ContractError, SourceAliasError) as exc:
        print(f"link-contract gate failed: {exc}", file=sys.stderr)
        return 1

    print(
        f"{action} link contracts: resolved={report['resolved']}/{report['contracts_total']} "
        f"blocked={report['blocked']} address_anchors={report['address_anchors']} "
        f"semantic_aliases={report['semantic_aliases']}"
    )
    if args.action == "link":
        print(
            f"partial-link externals: {report['input_external_symbols']} -> "
            f"{report['output_external_symbols']} (allocated bytes unchanged)"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
