#!/usr/bin/env python3
"""Prove and apply zero-byte aliases for recovered target-address symbols.

Stage 2 intentionally leaves names such as ``FUN_00101858`` undefined even
when the same target function is already exported under a readable recovered
name.  This gate only binds an alias when the frozen manifests prove a unique
global text definition.  It never assigns a raw virtual address and never
emits a trampoline.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import shlex
import shutil
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass
from io import StringIO
from pathlib import Path
from typing import Iterable, Sequence


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_EXTERNAL = ROOT / "analysis" / "source_tree" / "external_symbol_ownership.tsv"
DEFAULT_DEFINED = ROOT / "analysis" / "source_tree" / "defined_symbol_ownership.tsv"
DEFAULT_PROGRESS = ROOT / "analysis" / "progress_targets.csv"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "source_address_aliases.tsv"
DEFAULT_INPUT = ROOT / "build" / "source-tree" / "source-tree.partial.o"
DEFAULT_BUILD = ROOT / "build" / "source-aliases"
DEFAULT_OUTPUT = DEFAULT_BUILD / "source-tree.alias-resolved.partial.o"
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
MANIFEST_FIELDS = (
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

ADDRESS_RE = re.compile(r"(?:^|_)([0-9a-fA-F]{8})$")
PROVED = "PROVED"
BLOCKED = "BLOCKED"


class AliasError(RuntimeError):
    """A source-address alias invariant failed."""


@dataclass(frozen=True)
class NmSymbol:
    name: str
    type_code: str
    value: str = ""
    size: str = ""

    @property
    def undefined(self) -> bool:
        return self.type_code.upper() == "U"


def fail(message: str) -> None:
    raise AliasError(message)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def read_table(path: Path, fields: Sequence[str], *, delimiter: str) -> list[dict[str, str]]:
    if not path.is_file():
        fail(f"missing manifest input: {path}")
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter=delimiter)
        if tuple(reader.fieldnames or ()) != tuple(fields):
            fail(f"unexpected columns in {path}")
        rows = list(reader)
    return rows


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


def symbol_address(symbol: str) -> int:
    match = ADDRESS_RE.search(symbol)
    if match is None:
        fail(f"source-address alias has no hexadecimal suffix: {symbol}")
    return int(match.group(1), 16)


def derive_rows(
    external_rows: Sequence[dict[str, str]],
    defined_rows: Sequence[dict[str, str]],
    progress_rows: Sequence[dict[str, str]],
) -> list[dict[str, str]]:
    """Classify every frozen source-address alias using strict public evidence."""
    global_text = [
        row
        for row in defined_rows
        if row["binding"] == "global" and row["section_class"] == "text"
    ]
    by_name: dict[str, dict[str, str]] = {}
    by_suffix: dict[int, list[dict[str, str]]] = defaultdict(list)
    for row in global_text:
        name = row["symbol"]
        if name in by_name:
            fail(f"duplicate global text definition in ownership map: {name}")
        by_name[name] = row
        match = ADDRESS_RE.search(name)
        if match is not None:
            by_suffix[int(match.group(1), 16)].append(row)

    progress_by_address: dict[int, dict[str, str]] = {}
    for row in progress_rows:
        try:
            address = int(row["address"], 16)
        except ValueError as exc:
            fail(f"invalid progress address: {row['address']!r}")
            raise AssertionError from exc
        if address in progress_by_address:
            fail(f"duplicate progress address: 0x{address:08x}")
        progress_by_address[address] = row

    aliases = [
        row for row in external_rows if row["provider_kind"] == "source-address-alias"
    ]
    if len({row["symbol"] for row in aliases}) != len(aliases):
        fail("duplicate source-address alias in external ownership map")

    result: list[dict[str, str]] = []
    for external in sorted(aliases, key=lambda row: row["symbol"]):
        alias = external["symbol"]
        address = symbol_address(alias)
        progress = progress_by_address.get(address)
        exact = by_name.get(progress["name"]) if progress is not None else None
        suffix_candidates = [
            row for row in by_suffix.get(address, []) if row["symbol"] != alias
        ]

        target: dict[str, str] | None = None
        evidence: str
        detail = ""
        if exact is not None:
            target = exact
            evidence = "progress-name-global-text"
        elif len(suffix_candidates) == 1:
            target = suffix_candidates[0]
            evidence = "unique-address-suffix-global-text"
        elif len(suffix_candidates) > 1:
            evidence = "ambiguous-address-suffix-global-text"
            detail = ";".join(sorted(row["symbol"] for row in suffix_candidates))
        elif progress is not None:
            evidence = "progress-target-not-exported"
            detail = f"expected={progress['name']}"
        else:
            evidence = "address-outside-progress-manifest"
            detail = "no audited target row"

        if target is not None:
            owners = external["owner"].split(";")
            if target["source"] not in owners:
                fail(
                    f"candidate owner mismatch for {alias}: "
                    f"{target['source']} not in {external['owner']}"
                )
            status = PROVED
            canonical_symbol = target["symbol"]
            canonical_source = target["source"]
            canonical_object = target["object"]
        else:
            status = BLOCKED
            canonical_symbol = ""
            canonical_source = ""
            canonical_object = ""

        result.append(
            {
                "alias": alias,
                "target_address": f"0x{address:08x}",
                "status": status,
                "canonical_symbol": canonical_symbol,
                "evidence": evidence,
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
    rows = derive_rows(external_rows, defined_rows, progress_rows)
    return rows, render_tsv(rows)


def validate_frozen_manifest(args: argparse.Namespace) -> list[dict[str, str]]:
    rows, expected = expected_manifest(args)
    if not args.manifest.is_file():
        fail(f"missing source-address alias manifest: {args.manifest}")
    actual = args.manifest.read_text(encoding="utf-8")
    if actual != expected:
        fail(
            "source-address alias manifest is stale; review inputs and run "
            "tools/source_aliases.py refresh"
        )
    return rows


def resolve_tool(value: str) -> Path:
    candidate = Path(value)
    if candidate.is_file():
        return candidate.resolve()
    discovered = shutil.which(value)
    if discovered is None:
        fail(f"required tool not found: {value}")
    return Path(discovered).resolve()


def sibling_tool(compiler: Path, explicit: str | None, suffix: str) -> Path:
    if explicit:
        return resolve_tool(explicit)
    if not compiler.name.endswith("gcc"):
        fail(f"cannot derive {suffix} from compiler {compiler}; pass --{suffix}")
    return resolve_tool(str(compiler.with_name(compiler.name[:-3] + suffix)))


def run(command: Sequence[str]) -> str:
    result = subprocess.run(
        list(command),
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    if result.returncode != 0:
        rendered = shlex.join(str(part) for part in command)
        details = (result.stdout or "").strip()
        if len(details) > 4000:
            details = details[-4000:]
        fail(f"command failed ({result.returncode}): {rendered}\n{details}")
    return result.stdout or ""


def parse_nm(text: str) -> list[NmSymbol]:
    symbols: list[NmSymbol] = []
    for raw in text.splitlines():
        line = raw.strip()
        if not line or line.endswith(":") or ": no symbols" in line:
            continue
        fields = line.split()
        if len(fields) < 2 or len(fields[1]) != 1:
            fail(f"cannot parse nm output: {raw!r}")
        symbols.append(
            NmSymbol(
                name=fields[0],
                type_code=fields[1],
                value=fields[2] if len(fields) >= 3 else "",
                size=fields[3] if len(fields) >= 4 else "",
            )
        )
    return symbols


def global_symbols(nm: Path, obj: Path) -> list[NmSymbol]:
    return parse_nm(run([str(nm), "-g", "-P", "-S", str(obj)]))


def alloc_section_fingerprints(path: Path) -> dict[str, dict[str, object]]:
    """Return exact fingerprints for every SHF_ALLOC section in an ELF32 file."""
    data = path.read_bytes()
    if len(data) < 52 or data[:6] != b"\x7fELF\x01\x01":
        fail(f"not an ELF32 little-endian object: {path}")
    header = struct.unpack_from("<HHIIIIIHHHHHH", data, 16)
    section_offset = header[5]
    section_entry_size = header[10]
    section_count = header[11]
    string_index = header[12]
    if section_entry_size < 40 or string_index >= section_count:
        fail(f"malformed ELF section table: {path}")
    if section_offset + section_entry_size * section_count > len(data):
        fail(f"truncated ELF section table: {path}")

    headers = [
        struct.unpack_from("<IIIIIIIIII", data, section_offset + i * section_entry_size)
        for i in range(section_count)
    ]
    strings_header = headers[string_index]
    strings_offset, strings_size = strings_header[4], strings_header[5]
    if strings_offset + strings_size > len(data):
        fail(f"truncated ELF section-name table: {path}")
    strings = data[strings_offset:strings_offset + strings_size]

    result: dict[str, dict[str, object]] = {}
    for section in headers:
        name_offset, section_type, flags = section[0], section[1], section[2]
        file_offset, size, alignment = section[4], section[5], section[8]
        if not flags & 0x2:  # SHF_ALLOC
            continue
        if name_offset >= len(strings):
            fail(f"invalid ELF section name offset: {path}")
        name_end = strings.find(b"\0", name_offset)
        if name_end < 0:
            fail(f"unterminated ELF section name: {path}")
        name = strings[name_offset:name_end].decode("ascii")
        if name in result:
            fail(f"duplicate allocated section {name!r}: {path}")
        if section_type == 8:  # SHT_NOBITS
            digest = None
        else:
            if file_offset + size > len(data):
                fail(f"truncated allocated section {name!r}: {path}")
            digest = hashlib.sha256(data[file_offset:file_offset + size]).hexdigest()
        result[name] = {
            "type": section_type,
            "flags": flags,
            "size": size,
            "alignment": alignment,
            "sha256": digest,
        }
    return dict(sorted(result.items()))


def verify_link_result(
    input_symbols: Sequence[NmSymbol],
    output_symbols: Sequence[NmSymbol],
    rows: Sequence[dict[str, str]],
    external_names: set[str],
) -> tuple[int, int]:
    input_undefined = {symbol.name for symbol in input_symbols if symbol.undefined}
    if input_undefined != external_names:
        missing = sorted(external_names - input_undefined)[:5]
        extra = sorted(input_undefined - external_names)[:5]
        fail(f"input aggregate/external map drift; missing={missing} extra={extra}")

    proved = [row for row in rows if row["status"] == PROVED]
    proved_names = {row["alias"] for row in proved}
    output_undefined = {symbol.name for symbol in output_symbols if symbol.undefined}
    expected_undefined = input_undefined - proved_names
    if output_undefined != expected_undefined:
        missing = sorted(expected_undefined - output_undefined)[:5]
        extra = sorted(output_undefined - expected_undefined)[:5]
        fail(f"alias-resolved undefined set is wrong; missing={missing} extra={extra}")

    output_defined: dict[str, NmSymbol] = {}
    for symbol in output_symbols:
        if symbol.undefined:
            continue
        if symbol.name in output_defined:
            fail(f"duplicate global output symbol: {symbol.name}")
        output_defined[symbol.name] = symbol
    for row in proved:
        alias = output_defined.get(row["alias"])
        target = output_defined.get(row["canonical_symbol"])
        if alias is None or target is None:
            fail(f"linker did not define both names for {row['alias']}")
        if alias.value != target.value or alias.type_code.upper() != target.type_code.upper():
            fail(
                f"alias value/type differs from canonical symbol: "
                f"{row['alias']} != {row['canonical_symbol']}"
            )
    return len(input_undefined), len(output_undefined)


def summarize(rows: Sequence[dict[str, str]]) -> dict[str, object]:
    status_counts = Counter(row["status"] for row in rows)
    evidence_counts = Counter(row["evidence"] for row in rows)
    proved_targets = {
        row["canonical_symbol"] for row in rows if row["status"] == PROVED
    }
    return {
        "aliases_total": len(rows),
        "proved": status_counts[PROVED],
        "blocked": status_counts[BLOCKED],
        "canonical_targets": len(proved_targets),
        "evidence_counts": dict(sorted(evidence_counts.items())),
    }


def link_aliases(args: argparse.Namespace, rows: Sequence[dict[str, str]]) -> dict[str, object]:
    if not args.input.is_file():
        fail(f"missing Stage-2 aggregate: {args.input}")
    compiler = resolve_tool(args.compiler)
    linker = sibling_tool(compiler, args.ld, "ld")
    nm = sibling_tool(compiler, args.nm, "nm")

    external_rows = read_table(args.external_map, EXTERNAL_FIELDS, delimiter="\t")
    external_names = {row["symbol"] for row in external_rows}
    input_symbols = global_symbols(nm, args.input)
    proved = [row for row in rows if row["status"] == PROVED]

    args.output.parent.mkdir(parents=True, exist_ok=True)
    map_path = args.output.with_suffix(".map")
    command = [str(linker), "-EL", "-r", "-Map", str(map_path)]
    for row in proved:
        command.extend(
            ["--defsym", f"{row['alias']}={row['canonical_symbol']}"]
        )
    command.extend(["-o", str(args.output), str(args.input)])
    run(command)

    output_symbols = global_symbols(nm, args.output)
    input_external_count, output_external_count = verify_link_result(
        input_symbols, output_symbols, rows, external_names
    )
    input_sections = alloc_section_fingerprints(args.input)
    output_sections = alloc_section_fingerprints(args.output)
    if output_sections != input_sections:
        fail("allocated sections changed while applying zero-byte aliases")

    summary = summarize(rows)
    report: dict[str, object] = {
        "schema": 1,
        "claim": "source-address-alias-resolution",
        **summary,
        "input_external_symbols": input_external_count,
        "output_external_symbols": output_external_count,
        "remaining_source_address_aliases": summary["blocked"],
        "allocated_sections_unchanged": True,
        "emitted_code_bytes": 0,
        "input_sha256": sha256_file(args.input),
        "output_sha256": sha256_file(args.output),
        "manifest_sha256": sha256_file(args.manifest),
        "allocated_sections": input_sections,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    markdown = [
        "# Source-address alias partial-link report",
        "",
        f"- Address aliases proved: **{summary['proved']}/{summary['aliases_total']}**",
        f"- Canonical global text targets: **{summary['canonical_targets']}**",
        f"- Address aliases still blocked: **{summary['blocked']}**",
        f"- Aggregate externals: **{input_external_count} -> {output_external_count}**",
        "- Allocated sections changed: **no**",
        "- Code/data bytes emitted by aliases: **0**",
        "",
        "This is a relocatable link-identity checkpoint, not a replacement ELF.",
        "",
    ]
    args.report.with_suffix(".md").write_text("\n".join(markdown), encoding="utf-8")
    return report


def add_manifest_arguments(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--external-map", type=Path, default=DEFAULT_EXTERNAL)
    parser.add_argument("--defined-map", type=Path, default=DEFAULT_DEFINED)
    parser.add_argument("--progress-manifest", type=Path, default=DEFAULT_PROGRESS)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="action", required=True)
    validate_parser = subparsers.add_parser("validate", help="verify the public alias manifest")
    add_manifest_arguments(validate_parser)
    refresh_parser = subparsers.add_parser("refresh", help="refresh the reviewed alias manifest")
    add_manifest_arguments(refresh_parser)
    link_parser = subparsers.add_parser("link", help="apply proved aliases to the Stage-2 aggregate")
    add_manifest_arguments(link_parser)
    link_parser.add_argument("--compiler", default="ee-gcc")
    link_parser.add_argument("--ld")
    link_parser.add_argument("--nm")
    link_parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    link_parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    link_parser.add_argument("--report", type=Path, default=DEFAULT_REPORT)
    args = parser.parse_args(argv)
    for name in ("external_map", "defined_map", "progress_manifest", "manifest"):
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
                report = link_aliases(args, rows)
                action = "linked"
    except AliasError as exc:
        print(f"source-address alias gate failed: {exc}", file=sys.stderr)
        return 1

    print(
        f"{action} source-address aliases: "
        f"proved={report['proved']}/{report['aliases_total']} "
        f"blocked={report['blocked']} "
        f"canonical_targets={report['canonical_targets']}"
    )
    if args.action == "link":
        print(
            f"partial-link externals: {report['input_external_symbols']} -> "
            f"{report['output_external_symbols']} (allocated bytes unchanged)"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
