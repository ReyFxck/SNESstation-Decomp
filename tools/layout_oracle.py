#!/usr/bin/env python3
"""Freeze and compare the private unpacked-image layout without publishing it."""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import sys
from pathlib import Path
from typing import Sequence

from sjuncrunch import SJCrunchError, decode_container


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_PACKED = ROOT / "original" / "SNES_EMU.ELF"
DEFAULT_UNPACKED = ROOT / "build" / "SNES_EMU.unpacked.bin"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "unpacked_layout.json"
DEFAULT_REPORT = ROOT / "build" / "layout-oracle" / "comparison.json"
DEFAULT_CHUNK_SIZE = 0x10000
SCHEMA_VERSION = 1
FORMAT_NAME = "snesstation-unpacked-layout-oracle"
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")


class OracleError(ValueError):
    """A layout-oracle input or invariant failed."""


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def require_bytes(path: Path, label: str) -> bytes:
    if not path.is_file():
        raise OracleError(f"missing {label}: {path}")
    return path.read_bytes()


def image_address(sections: Sequence[dict[str, object]], offset: int) -> int | None:
    """Map a concatenated unpacked-image offset back to its load address."""
    for section in sections:
        start = int(section["image_offset"])
        size = int(section["initialized_size"])
        if start <= offset < start + size:
            return int(section["virtual_address"]) + offset - start
    final = sections[-1]
    final_start = int(final["image_offset"])
    final_size = int(final["initialized_size"])
    if offset == final_start + final_size:
        return int(final["virtual_address"]) + final_size
    return None


def first_byte_difference(reference: bytes, candidate: bytes) -> int | None:
    common = min(len(reference), len(candidate))
    for offset in range(common):
        if reference[offset] != candidate[offset]:
            return offset
    if len(reference) != len(candidate):
        return common
    return None


def public_sections(decoded: dict[str, object]) -> list[dict[str, object]]:
    raw_sections = decoded["sections"]
    if not isinstance(raw_sections, list):
        raise OracleError("decoded section metadata is not a list")

    sections: list[dict[str, object]] = []
    for raw_section in raw_sections:
        if not isinstance(raw_section, dict):
            raise OracleError("decoded section metadata is malformed")
        raw_blocks = raw_section["blocks"]
        if not isinstance(raw_blocks, list):
            raise OracleError("decoded block metadata is not a list")
        blocks = [dict(block) for block in raw_blocks]
        section = {key: value for key, value in raw_section.items() if key != "blocks"}
        section["block_count"] = len(blocks)
        section["blocks"] = blocks
        sections.append(section)
    return sections


def build_manifest(
    packed: bytes,
    unpacked: bytes,
    *,
    chunk_size: int = DEFAULT_CHUNK_SIZE,
) -> dict[str, object]:
    if chunk_size <= 0 or chunk_size % 0x1000:
        raise OracleError("chunk size must be a positive multiple of 0x1000")

    try:
        decoded = decode_container(packed, native_lzo=None)
    except SJCrunchError as exc:
        raise OracleError(f"cannot decode packed reference: {exc}") from exc

    decoded_image = decoded["image"]
    if not isinstance(decoded_image, bytes):
        raise OracleError("decoder did not return a byte image")
    if decoded_image != unpacked:
        offset = first_byte_difference(decoded_image, unpacked)
        assert offset is not None
        raise OracleError(
            "supplied unpacked image differs from the SJCRUNCH2 payload at "
            f"offset 0x{offset:08x}"
        )

    sections = public_sections(decoded)
    if not sections:
        raise OracleError("decoded image has no sections")
    base_address = min(int(section["virtual_address"]) for section in sections)
    initialized_end = max(
        int(section["virtual_address"]) + int(section["initialized_size"])
        for section in sections
    )
    memory_end = max(
        int(section["virtual_address"]) + int(section["memory_size"])
        for section in sections
    )

    chunks: list[dict[str, object]] = []
    for index, offset in enumerate(range(0, len(unpacked), chunk_size)):
        chunk = unpacked[offset:offset + chunk_size]
        chunks.append(
            {
                "index": index,
                "image_offset": offset,
                "virtual_address": image_address(sections, offset),
                "size": len(chunk),
                "sha256": sha256_bytes(chunk),
            }
        )

    container_end = int(decoded["container_end_offset"])
    manifest: dict[str, object] = {
        "schema_version": SCHEMA_VERSION,
        "format": FORMAT_NAME,
        "packed": {
            "size": len(packed),
            "sha256": sha256_bytes(packed),
            "sjcrunch2_header_offset": int(decoded["header_offset"]),
            "sjcrunch2_end_offset": container_end,
            "trailing_size": len(packed) - container_end,
            "section_count": len(sections),
        },
        "entry_address": int(decoded["entry_address"]),
        "image": {
            "base_address": base_address,
            "initialized_size": len(unpacked),
            "initialized_end_address": initialized_end,
            "zero_fill_size": sum(int(section["zero_fill_size"]) for section in sections),
            "memory_end_address": memory_end,
            "loaded_span_size": memory_end - base_address,
            "sha256": sha256_bytes(unpacked),
            "chunk_size": chunk_size,
            "chunk_count": len(chunks),
            "chunks": chunks,
        },
        "sections": sections,
    }
    validate_manifest(manifest)
    return manifest


def require_int(mapping: dict[str, object], key: str, minimum: int = 0) -> int:
    value = mapping.get(key)
    if not isinstance(value, int) or isinstance(value, bool) or value < minimum:
        raise OracleError(f"manifest field {key!r} must be an integer >= {minimum}")
    return value


def require_sha256(mapping: dict[str, object], key: str) -> str:
    value = mapping.get(key)
    if not isinstance(value, str) or not SHA256_RE.fullmatch(value):
        raise OracleError(f"manifest field {key!r} is not a lowercase SHA-256")
    return value


def validate_manifest(manifest: dict[str, object]) -> None:
    if manifest.get("schema_version") != SCHEMA_VERSION:
        raise OracleError(f"unsupported layout-oracle schema: {manifest.get('schema_version')!r}")
    if manifest.get("format") != FORMAT_NAME:
        raise OracleError(f"unexpected layout-oracle format: {manifest.get('format')!r}")

    packed = manifest.get("packed")
    image = manifest.get("image")
    sections = manifest.get("sections")
    if not isinstance(packed, dict) or not isinstance(image, dict) or not isinstance(sections, list):
        raise OracleError("manifest packed/image/sections fields are malformed")
    if not sections:
        raise OracleError("manifest must describe at least one section")

    packed_size = require_int(packed, "size", 1)
    require_sha256(packed, "sha256")
    header_offset = require_int(packed, "sjcrunch2_header_offset")
    container_end = require_int(packed, "sjcrunch2_end_offset", header_offset)
    trailing_size = require_int(packed, "trailing_size")
    if container_end + trailing_size != packed_size:
        raise OracleError("packed container end plus trailing size does not equal packed size")
    if require_int(packed, "section_count", 1) != len(sections):
        raise OracleError("packed section count does not match sections array")

    image_size = require_int(image, "initialized_size", 1)
    base_address = require_int(image, "base_address")
    initialized_end = require_int(image, "initialized_end_address", base_address)
    memory_end = require_int(image, "memory_end_address", initialized_end)
    require_int(image, "zero_fill_size")
    if require_int(image, "loaded_span_size", 1) != memory_end - base_address:
        raise OracleError("loaded span size does not match the recorded address interval")
    require_sha256(image, "sha256")
    chunk_size = require_int(image, "chunk_size", 0x1000)
    if chunk_size % 0x1000:
        raise OracleError("manifest chunk size is not page aligned")
    chunks = image.get("chunks")
    if not isinstance(chunks, list) or not chunks:
        raise OracleError("manifest chunks field must be a non-empty list")
    if require_int(image, "chunk_count", 1) != len(chunks):
        raise OracleError("manifest chunk count does not match chunks array")

    covered = 0
    for index, chunk in enumerate(chunks):
        if not isinstance(chunk, dict):
            raise OracleError("manifest chunk row is malformed")
        if require_int(chunk, "index") != index:
            raise OracleError("manifest chunk indexes are not contiguous")
        if require_int(chunk, "image_offset") != covered:
            raise OracleError("manifest chunks do not form a contiguous image")
        size = require_int(chunk, "size", 1)
        if size > chunk_size:
            raise OracleError("manifest chunk exceeds the frozen chunk size")
        require_int(chunk, "virtual_address")
        require_sha256(chunk, "sha256")
        covered += size
    if covered != image_size:
        raise OracleError("manifest chunks do not cover the initialized image")

    section_image_offset = 0
    total_zero_fill = 0
    block_total = 0
    for section_index, section in enumerate(sections):
        if not isinstance(section, dict):
            raise OracleError("manifest section row is malformed")
        if require_int(section, "index") != section_index:
            raise OracleError("manifest section indexes are not contiguous")
        if require_int(section, "image_offset") != section_image_offset:
            raise OracleError("manifest sections do not form a contiguous unpacked image")
        initialized_size = require_int(section, "initialized_size", 1)
        zero_fill_size = require_int(section, "zero_fill_size")
        if require_int(section, "memory_size", initialized_size) != initialized_size + zero_fill_size:
            raise OracleError("manifest section memory size is inconsistent")
        require_int(section, "virtual_address")
        require_sha256(section, "sha256")
        block_stream_offset = require_int(section, "packed_block_stream_offset")
        block_stream_end = require_int(section, "packed_block_stream_end", block_stream_offset)
        block_stream_size = require_int(section, "block_stream_size")
        if block_stream_end - block_stream_offset != block_stream_size:
            raise OracleError("manifest block-stream span is inconsistent")
        packed_end = require_int(section, "packed_end_offset", block_stream_end)
        file_size = require_int(section, "file_size", block_stream_size)
        if packed_end - block_stream_offset != file_size:
            raise OracleError("manifest packed section span is inconsistent")
        if require_int(section, "padding_size") != file_size - block_stream_size:
            raise OracleError("manifest packed section padding is inconsistent")
        if not isinstance(section.get("padding_is_zero"), bool):
            raise OracleError("manifest padding_is_zero field must be boolean")

        blocks = section.get("blocks")
        if not isinstance(blocks, list) or not blocks:
            raise OracleError("manifest section must contain block rows")
        if require_int(section, "block_count", 1) != len(blocks):
            raise OracleError("manifest block count does not match blocks array")
        section_covered = 0
        compressed_total = 0
        for block_index, block in enumerate(blocks):
            if not isinstance(block, dict):
                raise OracleError("manifest block row is malformed")
            if require_int(block, "index") != block_index:
                raise OracleError("manifest block indexes are not contiguous")
            if require_int(block, "image_offset") != section_image_offset + section_covered:
                raise OracleError("manifest blocks do not form a contiguous section")
            size = require_int(block, "initialized_size", 1)
            compressed_total += require_int(block, "packed_data_size", 1)
            require_int(block, "packed_header_offset")
            require_int(block, "packed_data_offset")
            require_int(block, "virtual_address")
            if block.get("encoding") not in {"stored", "lzo1x"}:
                raise OracleError("manifest block encoding is invalid")
            require_sha256(block, "sha256")
            section_covered += size
        if section_covered != initialized_size:
            raise OracleError("manifest blocks do not cover their initialized section")
        if compressed_total != require_int(section, "compressed_data_size", 1):
            raise OracleError("manifest compressed block sizes do not match their section")

        section_image_offset += initialized_size
        total_zero_fill += zero_fill_size
        block_total += len(blocks)

    if section_image_offset != image_size:
        raise OracleError("manifest sections do not cover the initialized image")
    if total_zero_fill != require_int(image, "zero_fill_size"):
        raise OracleError("manifest section BSS total does not match image metadata")
    if block_total < 1:
        raise OracleError("manifest does not contain any SJCRUNCH2 blocks")


def load_manifest(path: Path) -> dict[str, object]:
    if not path.is_file():
        raise OracleError(f"missing layout manifest: {path}")
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise OracleError(f"cannot read layout manifest {path}: {exc}") from exc
    if not isinstance(value, dict):
        raise OracleError("layout manifest root must be a JSON object")
    validate_manifest(value)
    return value


def render_json(value: dict[str, object]) -> str:
    return json.dumps(value, indent=2, sort_keys=True) + "\n"


def first_structure_difference(expected: object, actual: object, path: str = "$") -> str | None:
    if type(expected) is not type(actual):
        return f"{path}: expected type {type(expected).__name__}, got {type(actual).__name__}"
    if isinstance(expected, dict):
        expected_keys = set(expected)
        actual_keys = set(actual)  # type: ignore[arg-type]
        if expected_keys != actual_keys:
            return f"{path}: expected keys {sorted(expected_keys)!r}, got {sorted(actual_keys)!r}"
        for key in sorted(expected_keys):
            difference = first_structure_difference(
                expected[key], actual[key], f"{path}.{key}"  # type: ignore[index]
            )
            if difference is not None:
                return difference
        return None
    if isinstance(expected, list):
        if len(expected) != len(actual):  # type: ignore[arg-type]
            return f"{path}: expected {len(expected)} rows, got {len(actual)}"  # type: ignore[arg-type]
        for index, expected_item in enumerate(expected):
            difference = first_structure_difference(
                expected_item, actual[index], f"{path}[{index}]"  # type: ignore[index]
            )
            if difference is not None:
                return difference
        return None
    if expected != actual:
        return f"{path}: expected {expected!r}, got {actual!r}"
    return None


def compare_images(
    reference: bytes,
    candidate: bytes,
    manifest: dict[str, object],
) -> dict[str, object]:
    validate_manifest(manifest)
    image = manifest["image"]
    sections = manifest["sections"]
    assert isinstance(image, dict)
    assert isinstance(sections, list)
    expected_size = int(image["initialized_size"])
    expected_sha = str(image["sha256"])
    if len(reference) != expected_size or sha256_bytes(reference) != expected_sha:
        raise OracleError("private reference image does not match the frozen layout manifest")

    chunk_size = int(image["chunk_size"])
    expected_chunks = image["chunks"]
    assert isinstance(expected_chunks, list)
    candidate_chunk_count = (len(candidate) + chunk_size - 1) // chunk_size
    mismatching_chunks: list[dict[str, object]] = []
    for index in range(max(len(expected_chunks), candidate_chunk_count)):
        offset = index * chunk_size
        reference_chunk = reference[offset:offset + chunk_size]
        candidate_chunk = candidate[offset:offset + chunk_size]
        if reference_chunk == candidate_chunk:
            continue
        mismatching_chunks.append(
            {
                "index": index,
                "image_offset": offset,
                "virtual_address": image_address(sections, offset),
                "reference_size": len(reference_chunk),
                "candidate_size": len(candidate_chunk),
                "reference_sha256": sha256_bytes(reference_chunk),
                "candidate_sha256": sha256_bytes(candidate_chunk),
            }
        )

    first = first_byte_difference(reference, candidate)
    common = min(len(reference), len(candidate))
    different_bytes = sum(
        reference[offset] != candidate[offset] for offset in range(common)
    ) + abs(len(reference) - len(candidate))
    matching_prefix = len(reference) if first is None else first
    matching_suffix = 0
    while (
        matching_suffix < common - matching_prefix
        and reference[len(reference) - matching_suffix - 1]
        == candidate[len(candidate) - matching_suffix - 1]
    ):
        matching_suffix += 1

    first_difference: dict[str, object] | None = None
    if first is not None:
        first_difference = {
            "image_offset": first,
            "virtual_address": image_address(sections, first),
            "chunk_index": first // chunk_size,
            "reference_byte": reference[first] if first < len(reference) else None,
            "candidate_byte": candidate[first] if first < len(candidate) else None,
        }

    return {
        "schema_version": 1,
        "status": "exact" if first is None else "mismatch",
        "reference": {
            "size": len(reference),
            "sha256": sha256_bytes(reference),
        },
        "candidate": {
            "size": len(candidate),
            "sha256": sha256_bytes(candidate),
        },
        "matching_prefix_size": matching_prefix,
        "matching_suffix_size": matching_suffix,
        "different_byte_count": different_bytes,
        "mismatching_chunk_count": len(mismatching_chunks),
        "mismatching_chunks": mismatching_chunks,
        "first_difference": first_difference,
    }


def add_reference_arguments(parser: argparse.ArgumentParser) -> None:
    parser.add_argument("--packed", type=Path, default=DEFAULT_PACKED)
    parser.add_argument("--unpacked", type=Path, default=DEFAULT_UNPACKED)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)

    capture = commands.add_parser("capture", help="refresh the public hash-only manifest")
    add_reference_arguments(capture)
    capture.add_argument("--chunk-size", type=lambda value: int(value, 0), default=DEFAULT_CHUNK_SIZE)

    check = commands.add_parser("check", help="compare the private reference to the frozen manifest")
    add_reference_arguments(check)

    validate = commands.add_parser("validate", help="validate the public manifest without private bytes")
    validate.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)

    compare = commands.add_parser("compare", help="report the first rebuilt-image difference")
    compare.add_argument("--reference", type=Path, default=DEFAULT_UNPACKED)
    compare.add_argument("--candidate", type=Path, required=True)
    compare.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    compare.add_argument("--report", type=Path, default=DEFAULT_REPORT)
    return parser.parse_args(argv)


def manifest_summary(manifest: dict[str, object]) -> str:
    packed = manifest["packed"]
    image = manifest["image"]
    sections = manifest["sections"]
    assert isinstance(packed, dict)
    assert isinstance(image, dict)
    assert isinstance(sections, list)
    blocks = sum(len(section["blocks"]) for section in sections)  # type: ignore[index]
    return (
        f"sections={packed['section_count']} blocks={blocks} "
        f"chunks={image['chunk_count']} size={image['initialized_size']} "
        f"sha256={image['sha256']}"
    )


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "validate":
            manifest = load_manifest(args.manifest)
            print(f"public layout manifest: OK ({manifest_summary(manifest)})")
            return 0

        if args.command in {"capture", "check"}:
            packed = require_bytes(args.packed, "packed reference ELF")
            unpacked = require_bytes(args.unpacked, "unpacked reference image")
            if args.command == "capture":
                manifest = build_manifest(packed, unpacked, chunk_size=args.chunk_size)
                args.manifest.parent.mkdir(parents=True, exist_ok=True)
                args.manifest.write_text(render_json(manifest), encoding="utf-8")
                print(f"refreshed unpacked layout oracle: {manifest_summary(manifest)}")
                return 0

            expected = load_manifest(args.manifest)
            image = expected["image"]
            assert isinstance(image, dict)
            actual = build_manifest(packed, unpacked, chunk_size=int(image["chunk_size"]))
            difference = first_structure_difference(expected, actual)
            if difference is not None:
                raise OracleError(f"frozen layout manifest differs: {difference}")
            print(f"unpacked layout oracle: OK ({manifest_summary(expected)})")
            return 0

        if args.command == "compare":
            reference = require_bytes(args.reference, "private unpacked reference")
            candidate = require_bytes(args.candidate, "rebuilt candidate image")
            manifest = load_manifest(args.manifest)
            report = compare_images(reference, candidate, manifest)
            args.report.parent.mkdir(parents=True, exist_ok=True)
            args.report.write_text(render_json(report), encoding="utf-8")
            if report["status"] == "exact":
                print(
                    "unpacked candidate: EXACT "
                    f"size={report['candidate']['size']} "  # type: ignore[index]
                    f"sha256={report['candidate']['sha256']}"  # type: ignore[index]
                )
                return 0
            first = report["first_difference"]
            assert isinstance(first, dict)
            address = first["virtual_address"]
            address_text = "unknown" if address is None else f"0x{address:08x}"
            print(
                "unpacked candidate: MISMATCH "
                f"offset=0x{first['image_offset']:08x} address={address_text} "
                f"reference={first['reference_byte']!r} candidate={first['candidate_byte']!r} "
                f"different_bytes={report['different_byte_count']} "
                f"report={args.report}"
            )
            return 1

        raise AssertionError(f"unhandled command: {args.command}")
    except (OSError, OracleError) as exc:
        print(f"layout oracle failed: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
