#!/usr/bin/env python3
"""Verify the seven historical Stage-3D libgcc source contracts.

Four live contracts select exact GCC 3.2.2 libgcc archive members.  Three
other names were introduced only by the reconstructed C aggregate and are
closed as source refactors.  The private gate compares complete member .text
sections while masking only relocation-controlled MIPS bits; no target or
archive bytes are committed.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import shutil
import subprocess
from dataclasses import dataclass
from io import StringIO
from pathlib import Path
from typing import Sequence

from compare_elf_functions import ELFFile, Symbol


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_EXTERNAL = ROOT / "analysis" / "source_tree" / "external_symbol_ownership.tsv"
DEFAULT_CONTRACTS = ROOT / "analysis" / "link_identity" / "link_contracts.tsv"
DEFAULT_FRONTIER = ROOT / "analysis" / "link_identity" / "provider_frontier_closure.tsv"
DEFAULT_LAYOUT = ROOT / "analysis" / "link_identity" / "unpacked_layout.json"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "libgcc_contracts.tsv"
DEFAULT_REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
DEFAULT_BUILD = ROOT / "build" / "libgcc-contracts"
DEFAULT_REPORT = DEFAULT_BUILD / "report.json"

TARGET_BASE = 0x00100000
TARGET_SIZE = 3_304_936
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

EXTERNAL_FIELDS = (
    "symbol", "category", "provider_kind", "owner", "resolution_gate", "requesters",
)
CONTRACT_FIELDS = (
    "symbol", "target_address", "status", "resolution_kind", "canonical_symbol",
    "evidence", "category", "provider_kind", "canonical_source", "canonical_object",
    "requesters", "detail",
)
FRONTIER_FIELDS = (
    "symbol", "category", "provider_kind", "resolution_kind", "target_address",
    "target_symbol", "storage_size_hex", "evidence", "requesters", "detail",
)
MANIFEST_FIELDS = (
    "symbol", "status", "archive_member", "target_address", "extent_hex",
    "member_text_size_hex", "member_text_sha256", "target_sha256",
    "normalized_sha256", "relocation_count", "evidence", "requesters", "detail",
)

ARCHIVE_TEXT_EXACT = "ARCHIVE_TEXT_EXACT"
SOURCE_REFACTOR_CLOSED = "SOURCE_REFACTOR_CLOSED"
SHA_RE = re.compile(r"[0-9a-f]{64}")


class LibgccContractError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise LibgccContractError(message)


@dataclass(frozen=True)
class ContractSpec:
    symbol: str
    status: str
    member: str
    address: int | None
    end: int | None
    text_size: int
    relocations: int
    evidence: str
    detail: str
    historical_requesters: str = ""


SPECS = (
    ContractSpec(
        "__ashlti3", SOURCE_REFACTOR_CLOSED, "ashlti3.o", None, None, 0, 0,
        "empty-archive-member+compiled-source-refactor",
        "GCC 3.2.2 emits an empty TImode member for EE; byte-wise lift helpers remove the synthetic libcall",
        "ps2/progress28_structural_lift_recovered.o",
    ),
    ContractSpec(
        "__fixunssfdi", SOURCE_REFACTOR_CLOSED, "_fixunssfdi.o", None, None, 0x130, 12,
        "target-wide-negative-scan+compiled-source-refactor",
        "the archive body has zero relocation-normalized occurrences in the target; integer/float lift conversions are now explicit",
        "ps2/libgcc_runtime_recovered.o;ps2/progress28_structural_lift_recovered.o",
    ),
    ContractSpec(
        "__floatdisf", ARCHIVE_TEXT_EXACT, "_floatdisf.o", 0x001A1B98, 0x001A1C98, 0x100, 7,
        "complete-member-text-relocation-normalized",
        "complete _floatdisf.o .text including the four-byte terminal gap",
    ),
    ContractSpec(
        "__lshrti3", SOURCE_REFACTOR_CLOSED, "lshrti3.o", None, None, 0, 0,
        "empty-archive-member+compiled-source-refactor",
        "GCC 3.2.2 emits an empty TImode member for EE; narrowed 64-bit expressions remove the synthetic libcall",
        "ps2/progress28_structural_lift_recovered.o",
    ),
    ContractSpec(
        "__muldi3", ARCHIVE_TEXT_EXACT, "_muldi3.o", 0x001A1B20, 0x001A1B98, 0x78, 0,
        "complete-member-text-raw-exact",
        "complete _muldi3.o .text including the four-byte terminal gap",
    ),
    ContractSpec(
        "__udivdi3", ARCHIVE_TEXT_EXACT, "_udivdi3.o", 0x001A25B0, 0x001A2C78, 0x6C8, 7,
        "complete-member-text-relocation-normalized",
        "public wrapper, internal __udivmoddi4 body and terminal gap all agree",
    ),
    ContractSpec(
        "__umoddi3", ARCHIVE_TEXT_EXACT, "_umoddi3.o", 0x001A2C78, 0x001A3340, 0x6C8, 7,
        "complete-member-text-relocation-normalized",
        "public wrapper, internal __udivmoddi4 body and terminal gap all agree",
    ),
)
SPEC_BY_SYMBOL = {spec.symbol: spec for spec in SPECS}
EXACT_SYMBOLS = {spec.symbol for spec in SPECS if spec.status == ARCHIVE_TEXT_EXACT}
REFACTOR_SYMBOLS = {spec.symbol for spec in SPECS if spec.status == SOURCE_REFACTOR_CLOSED}
SOURCE_MARKERS = {
    "__ashlti3": ("src/ps2/progress28_structural_lift_recovered.c", "(uint64_t)CONCAT44"),
    "__fixunssfdi": ("src/ps2/progress28_structural_lift_recovered.c", "p28_float_to_u64"),
    "__lshrti3": ("src/ps2/progress28_structural_lift_recovered.c", "(uint64_t)CONCAT44"),
}


def read_table(path: Path, fields: Sequence[str]) -> list[dict[str, str]]:
    if not path.is_file():
        fail(f"missing table: {path}")
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != tuple(fields):
            fail(f"unexpected fields in {path}: {reader.fieldnames}")
        rows = list(reader)
    if any(None in row for row in rows):
        fail(f"malformed row in {path}")
    return rows


def render_tsv(rows: Sequence[dict[str, str]]) -> str:
    stream = StringIO()
    writer = csv.DictWriter(stream, fieldnames=MANIFEST_FIELDS, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    return stream.getvalue()


def resolve_tool(value: str) -> Path:
    candidate = Path(value)
    if candidate.is_file():
        return candidate.resolve()
    discovered = shutil.which(value)
    if discovered is None:
        fail(f"required tool not found: {value}")
    return Path(discovered).resolve()


def run(command: Sequence[str], *, binary: bool = False) -> bytes | str:
    result = subprocess.run(
        list(command), cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        text=not binary, check=False,
    )
    if result.returncode:
        details = result.stderr if binary else (result.stderr or result.stdout)
        if isinstance(details, bytes):
            details = details.decode(errors="replace")
        fail(f"command failed ({result.returncode}): {' '.join(command)}\n{str(details)[-4000:]}")
    return result.stdout


def compiler_archive(compiler_value: str, ar_value: str | None) -> tuple[Path, Path]:
    compiler = resolve_tool(compiler_value)
    raw = str(run([str(compiler), "-print-libgcc-file-name"])).strip()
    archive = Path(raw)
    if not archive.is_absolute():
        archive = (ROOT / archive).resolve()
    if not archive.is_file():
        fail(f"compiler-reported libgcc archive is missing: {archive}")
    if ar_value:
        ar = resolve_tool(ar_value)
    else:
        if not compiler.name.endswith("gcc"):
            fail(f"cannot derive ar from compiler {compiler}; pass --ar")
        ar = resolve_tool(str(compiler.with_name(compiler.name[:-3] + "ar")))
    return archive, ar


def text_image(path: Path) -> tuple[bytes, tuple[object, ...], bytes]:
    elf = ELFFile(path)
    sections = [section for section in elf.sections if section.name == ".text"]
    if len(sections) != 1:
        fail(f"expected one .text section in {path}, found {len(sections)}")
    section = sections[0]
    span = Symbol(".text", section.address, section.size, section.index, 2)
    image = elf.symbol_bytes(span, section.size)
    masks = elf.relocation_masks(span, 4)
    unknown = sorted({mask.relocation_type for mask in masks if not mask.known})
    if unknown:
        fail(f"unknown MIPS relocation types in {path}: {unknown}")
    normalized = bytearray(image)
    for relocation in masks:
        for index, mask in enumerate(relocation.mask_bytes):
            offset = relocation.start + index
            normalized[offset] &= ~mask & 0xFF
    return image, tuple(masks), bytes(normalized)


def normalize_target(image: bytes, masks: Sequence[object]) -> bytes:
    normalized = bytearray(image)
    for relocation in masks:
        for index, mask in enumerate(relocation.mask_bytes):
            offset = relocation.start + index
            normalized[offset] &= ~mask & 0xFF
    return bytes(normalized)


def differing_unmasked(left: bytes, right: bytes, masks: Sequence[object]) -> int:
    if len(left) != len(right):
        return abs(len(left) - len(right)) + min(len(left), len(right))
    ignored = bytearray(len(left))
    for relocation in masks:
        for index, mask in enumerate(relocation.mask_bytes):
            ignored[relocation.start + index] |= mask
    return sum(
        ((a ^ b) & (~ignored[index] & 0xFF)) != 0
        for index, (a, b) in enumerate(zip(left, right))
    )


def normalized_occurrences(target: bytes, candidate: bytes, masks: Sequence[object]) -> list[int]:
    if not candidate:
        return []
    masked = bytearray(len(candidate))
    for relocation in masks:
        for index, mask in enumerate(relocation.mask_bytes):
            if mask:
                masked[relocation.start + index] = 1
    best_start = best_size = run_start = run_size = 0
    for index, is_masked in enumerate((*masked, 1)):
        if not is_masked:
            if run_size == 0:
                run_start = index
            run_size += 1
        else:
            if run_size > best_size:
                best_start, best_size = run_start, run_size
            run_size = 0
    if best_size < 8:
        fail("archive member lacks an eight-byte relocation-free scan anchor")
    needle = candidate[best_start:best_start + best_size]
    hits: list[int] = []
    offset = target.find(needle)
    while offset >= 0:
        start = offset - best_start
        if 0 <= start and start + len(candidate) <= len(target):
            window = target[start:start + len(candidate)]
            if differing_unmasked(window, candidate, masks) == 0:
                hits.append(start)
        offset = target.find(needle, offset + 1)
    return hits


def extract_members(compiler: str, ar_value: str | None, build_dir: Path) -> dict[str, Path]:
    archive, ar = compiler_archive(compiler, ar_value)
    build_dir.mkdir(parents=True, exist_ok=True)
    result: dict[str, Path] = {}
    for member in sorted({spec.member for spec in SPECS}):
        payload = run([str(ar), "p", str(archive), member], binary=True)
        assert isinstance(payload, bytes)
        if not payload:
            fail(f"archive member not found or empty ELF payload: {member}")
        path = build_dir / member
        path.write_bytes(payload)
        result[member] = path
    return result


def load_reference(path: Path) -> bytes:
    if not path.is_file():
        fail(f"missing private unpacked reference: {path}")
    data = path.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if len(data) != TARGET_SIZE or digest != TARGET_SHA256:
        fail(f"private reference identity mismatch: size={len(data)} sha256={digest}")
    return data


def build_rows(args: argparse.Namespace) -> tuple[list[dict[str, str]], dict[str, object]]:
    target = load_reference(args.reference)
    external = read_table(args.external_map, EXTERNAL_FIELDS)
    external_by_name = {row["symbol"]: row for row in external}
    members = extract_members(args.compiler, args.ar, args.build_dir / "members")
    rows: list[dict[str, str]] = []
    total_exact_bytes = 0
    total_relocations = 0
    negative_scan_hits = -1

    for spec in SPECS:
        image, masks, normalized = text_image(members[spec.member])
        if len(image) != spec.text_size or len(masks) != spec.relocations:
            fail(
                f"archive geometry drift for {spec.symbol}: text=0x{len(image):x} "
                f"relocations={len(masks)}"
            )
        target_sha = ""
        if spec.status == ARCHIVE_TEXT_EXACT:
            assert spec.address is not None and spec.end is not None
            expected = target[spec.address - TARGET_BASE:spec.end - TARGET_BASE]
            if len(expected) != len(image):
                fail(f"target/member extent mismatch for {spec.symbol}")
            differences = differing_unmasked(expected, image, masks)
            if differences:
                fail(f"{spec.symbol} differs outside relocation-controlled bits: {differences}")
            normalized_target = normalize_target(expected, masks)
            if normalized_target != normalized:
                fail(f"normalized target/member mismatch for {spec.symbol}")
            target_sha = hashlib.sha256(expected).hexdigest()
            total_exact_bytes += len(image)
            total_relocations += len(masks)
        elif spec.symbol == "__fixunssfdi":
            hits = normalized_occurrences(target, image, masks)
            negative_scan_hits = len(hits)
            if hits:
                rendered = ", ".join(f"0x{TARGET_BASE + hit:08x}" for hit in hits[:4])
                fail(f"__fixunssfdi unexpectedly occurs in target at {rendered}")

        requester = external_by_name.get(spec.symbol, {}).get("requesters", "")
        if spec.status == SOURCE_REFACTOR_CLOSED:
            requester = spec.historical_requesters
        rows.append(
            {
                "symbol": spec.symbol,
                "status": spec.status,
                "archive_member": spec.member,
                "target_address": "" if spec.address is None else f"0x{spec.address:08x}",
                "extent_hex": "" if spec.end is None else f"0x{spec.end - spec.address:x}",
                "member_text_size_hex": f"0x{len(image):x}",
                "member_text_sha256": hashlib.sha256(image).hexdigest(),
                "target_sha256": target_sha,
                "normalized_sha256": hashlib.sha256(normalized).hexdigest(),
                "relocation_count": str(len(masks)),
                "evidence": spec.evidence,
                "requesters": requester,
                "detail": spec.detail,
            }
        )
    return rows, {
        "contracts_total": len(rows),
        "archive_text_exact": len(EXACT_SYMBOLS),
        "source_refactors_closed": len(REFACTOR_SYMBOLS),
        "exact_text_bytes": total_exact_bytes,
        "relocations_normalized": total_relocations,
        "fixunssfdi_target_occurrences": negative_scan_hits,
    }


def validate_manifest(args: argparse.Namespace) -> list[dict[str, str]]:
    rows = read_table(args.manifest, MANIFEST_FIELDS)
    if len(rows) != 7 or [row["symbol"] for row in rows] != sorted(SPEC_BY_SYMBOL):
        fail("libgcc manifest must contain the sorted historical seven-row ledger")
    by_name = {row["symbol"]: row for row in rows}
    for symbol, spec in SPEC_BY_SYMBOL.items():
        row = by_name[symbol]
        expected_address = "" if spec.address is None else f"0x{spec.address:08x}"
        expected_extent = "" if spec.end is None else f"0x{spec.end - spec.address:x}"
        fixed = {
            "status": spec.status,
            "archive_member": spec.member,
            "target_address": expected_address,
            "extent_hex": expected_extent,
            "member_text_size_hex": f"0x{spec.text_size:x}",
            "relocation_count": str(spec.relocations),
            "evidence": spec.evidence,
            "detail": spec.detail,
        }
        for field, expected in fixed.items():
            if row[field] != expected:
                fail(f"{symbol} {field} drift: {row[field]!r} != {expected!r}")
        for field in ("member_text_sha256", "normalized_sha256"):
            if not SHA_RE.fullmatch(row[field]):
                fail(f"{symbol} lacks valid {field}")
        if spec.status == ARCHIVE_TEXT_EXACT:
            if not SHA_RE.fullmatch(row["target_sha256"]):
                fail(f"{symbol} lacks valid target_sha256")
        elif row["target_sha256"]:
            fail(f"source refactor {symbol} must not claim target bytes")

    external = read_table(args.external_map, EXTERNAL_FIELDS)
    active = {
        row["symbol"] for row in external
        if row["category"] == "compiler-runtime" and row["provider_kind"] == "historical-archive"
    }
    if active != EXACT_SYMBOLS:
        fail(f"active compiler-runtime set drift: {sorted(active)} != {sorted(EXACT_SYMBOLS)}")
    if len(external) != 1892:
        fail(f"expected 1,892 live source externals after runtime refactors, found {len(external)}")

    contracts = {row["symbol"]: row for row in read_table(args.contracts, CONTRACT_FIELDS)}
    for spec in SPECS:
        if spec.status == ARCHIVE_TEXT_EXACT:
            contract = contracts.get(spec.symbol)
            if not contract or contract["status"] != "RESOLVED":
                fail(f"live libgcc contract is not resolved: {spec.symbol}")
            if contract["target_address"] != f"0x{spec.address:08x}":
                fail(f"live libgcc address drift: {spec.symbol}")
        elif spec.symbol in contracts:
            fail(f"source-refactored libgcc name returned to link contracts: {spec.symbol}")

    frontier = read_table(args.frontier_manifest, FRONTIER_FIELDS)
    if len(frontier) != 223:
        fail(f"expected 223 post-snprintf provider rows, found {len(frontier)}")
    runtime_shims = {
        row["symbol"] for row in frontier if row["resolution_kind"] == "compatibility-runtime-shim"
    }
    if runtime_shims:
        fail(f"unexpected remaining runtime shims: {sorted(runtime_shims)}")
    if set(SPEC_BY_SYMBOL) & {row["symbol"] for row in frontier}:
        fail("closed libgcc names remain in the compatibility-provider frontier")

    layout = json.loads(args.layout_manifest.read_text(encoding="utf-8"))
    image = layout.get("image", {})
    if image.get("initialized_size") != TARGET_SIZE or image.get("sha256") != TARGET_SHA256:
        fail("public layout manifest identity drift")
    for symbol, (source_name, marker) in SOURCE_MARKERS.items():
        text = (ROOT / source_name).read_text(encoding="utf-8")
        if marker not in text:
            fail(f"source-refactor evidence missing for {symbol}: {marker}")
    return rows


def verify_frozen(args: argparse.Namespace) -> dict[str, object]:
    frozen = validate_manifest(args)
    expected, report = build_rows(args)
    if render_tsv(frozen) != render_tsv(expected):
        fail("private libgcc proof differs from frozen manifest; review and refresh")
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return report


def summary(rows: Sequence[dict[str, str]]) -> str:
    exact = sum(row["status"] == ARCHIVE_TEXT_EXACT for row in rows)
    refactors = sum(row["status"] == SOURCE_REFACTOR_CLOSED for row in rows)
    return f"total={len(rows)} archive_exact={exact} source_refactors_closed={refactors} runtime_shims=0"


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", choices=("validate", "refresh", "verify"))
    parser.add_argument("--external-map", type=Path, default=DEFAULT_EXTERNAL)
    parser.add_argument("--contracts", type=Path, default=DEFAULT_CONTRACTS)
    parser.add_argument("--frontier-manifest", type=Path, default=DEFAULT_FRONTIER)
    parser.add_argument("--layout-manifest", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    parser.add_argument("--compiler", default="ee-gcc")
    parser.add_argument("--ar")
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--report", type=Path, default=DEFAULT_REPORT)
    return parser.parse_args(argv)


def main() -> None:
    args = parse_args()
    try:
        if args.action == "refresh":
            rows, report = build_rows(args)
            args.manifest.parent.mkdir(parents=True, exist_ok=True)
            args.manifest.write_text(render_tsv(rows), encoding="utf-8")
            validate_manifest(args)
            print(f"refreshed Stage-3D libgcc contracts: {summary(rows)}")
            print(
                f"exact libgcc member text: {report['exact_text_bytes']} bytes; "
                f"relocations={report['relocations_normalized']}"
            )
        elif args.action == "verify":
            report = verify_frozen(args)
            rows = read_table(args.manifest, MANIFEST_FIELDS)
            print(f"verified Stage-3D libgcc contracts: {summary(rows)}")
            print(
                f"exact libgcc member text: {report['exact_text_bytes']} bytes; "
                f"relocations={report['relocations_normalized']}; "
                f"__fixunssfdi target hits={report['fixunssfdi_target_occurrences']}"
            )
        else:
            rows = validate_manifest(args)
            print(f"validated Stage-3D libgcc contracts: {summary(rows)}")
    except (LibgccContractError, OSError, ValueError, json.JSONDecodeError) as exc:
        raise SystemExit(f"Stage-3D libgcc gate failed: {exc}") from exc


if __name__ == "__main__":
    main()
