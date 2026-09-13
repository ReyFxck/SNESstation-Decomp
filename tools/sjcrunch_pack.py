#!/usr/bin/env python3
"""Rebuild and verify the public SJCRUNCH2 compressed container.

The loader stub and outer ELF are deliberately outside this tool.  A private
check may compare the generated container with a legally obtained reference,
but no reference payload is copied into the repository.
"""
from __future__ import annotations

import argparse
import ctypes
import ctypes.util
import hashlib
import json
import os
import re
import struct
import sys
from pathlib import Path
from typing import Callable, Sequence

from layout_oracle import load_manifest as load_layout_manifest


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_LAYOUT = ROOT / "analysis" / "link_identity" / "unpacked_layout.json"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "sjcrunch_packing.json"
DEFAULT_IMAGE = ROOT / "build" / "code-windows" / "stage3p-code-windows-integrated.padded.bin"
DEFAULT_PACKED = ROOT / "original" / "SNES_EMU.ELF"
DEFAULT_OUTPUT = ROOT / "build" / "sjcrunch-packing" / "container.bin"
FORMAT = "snesstation-sjcrunch2-packing"
SCHEMA_VERSION = 1
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")
LZO_WORK_MEMORY = 14 * 16384 * 2


class PackingError(ValueError):
    """An SJCRUNCH2 packing input or invariant failed."""


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def require_int(mapping: dict[str, object], key: str, minimum: int = 0) -> int:
    value = mapping.get(key)
    if not isinstance(value, int) or isinstance(value, bool) or value < minimum:
        raise PackingError(f"manifest field {key!r} must be an integer >= {minimum}")
    return value


def require_sha256(mapping: dict[str, object], key: str) -> str:
    value = mapping.get(key)
    if not isinstance(value, str) or not SHA256_RE.fullmatch(value):
        raise PackingError(f"manifest field {key!r} is not a lowercase SHA-256")
    return value


def load_packing_manifest(path: Path = DEFAULT_MANIFEST) -> dict[str, object]:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise PackingError(f"cannot read packing manifest {path}: {exc}") from exc
    if not isinstance(value, dict):
        raise PackingError("packing manifest root must be an object")
    validate_packing_manifest(value)
    return value


def validate_packing_manifest(manifest: dict[str, object]) -> None:
    if manifest.get("schema_version") != SCHEMA_VERSION:
        raise PackingError("unsupported SJCRUNCH2 packing schema")
    if manifest.get("format") != FORMAT:
        raise PackingError("unexpected SJCRUNCH2 packing format")
    if manifest.get("algorithm") != "lzo1x_999":
        raise PackingError("packing algorithm must be lzo1x_999")
    if require_int(manifest, "compression_level", 1) != 8:
        raise PackingError("the frozen SJCRUNCH2 compression level must be 8")
    if require_int(manifest, "block_size", 1) != 0x40000:
        raise PackingError("the frozen SJCRUNCH2 block size must be 256 KiB")

    container = manifest.get("container")
    outer_elf = manifest.get("outer_elf")
    blocks = manifest.get("blocks")
    evidence = manifest.get("version_evidence")
    if not isinstance(container, dict) or not isinstance(outer_elf, dict):
        raise PackingError("container/outer_elf packing metadata is malformed")
    if not isinstance(blocks, list) or not blocks:
        raise PackingError("packing manifest must contain block rows")
    if not isinstance(evidence, dict):
        raise PackingError("packing version evidence is malformed")

    header_offset = require_int(container, "packed_offset")
    size = require_int(container, "size", 1)
    if require_int(container, "packed_end_offset", header_offset) != header_offset + size:
        raise PackingError("packed container offsets do not match its size")
    require_sha256(container, "sha256")
    if require_int(container, "section_count", 1) != 1:
        raise PackingError("the frozen image must contain one SJCRUNCH2 section")
    if require_int(container, "block_count", 1) != len(blocks):
        raise PackingError("container block count does not match block rows")

    prefix_size = require_int(outer_elf, "prefix_size", 1)
    trailing_size = require_int(outer_elf, "trailing_size", 1)
    if prefix_size != header_offset:
        raise PackingError("outer ELF prefix does not end at the container")
    if require_int(outer_elf, "unreproduced_size", 1) != prefix_size + trailing_size:
        raise PackingError("outer ELF unreproduced byte count is inconsistent")
    require_sha256(outer_elf, "prefix_sha256")
    require_sha256(outer_elf, "trailing_sha256")
    if outer_elf.get("reproduced") is not False:
        raise PackingError("outer ELF must remain explicitly unreproduced")

    if evidence.get("embedded_minilzo_version") != "1.08":
        raise PackingError("embedded miniLZO version evidence must remain 1.08")
    if evidence.get("embedded_minilzo_date") != "12-Jul-2002":
        raise PackingError("embedded miniLZO date evidence must remain 12-Jul-2002")

    image_offset = 0
    compressed_total = 0
    for index, block in enumerate(blocks):
        if not isinstance(block, dict):
            raise PackingError("packing block row is malformed")
        if require_int(block, "index") != index:
            raise PackingError("packing block indexes are not contiguous")
        if require_int(block, "image_offset") != image_offset:
            raise PackingError("packing block image offsets are not contiguous")
        image_offset += require_int(block, "initialized_size", 1)
        compressed_total += require_int(block, "packed_data_size", 1)
        require_sha256(block, "compressed_sha256")
    if compressed_total != require_int(container, "compressed_data_size", 1):
        raise PackingError("compressed block sizes do not match the container total")

    forbidden = ("source_path", "packed_bytes", "unpacked_bytes", "data_base64")
    rendered = json.dumps(manifest, sort_keys=True)
    if any(key in rendered for key in forbidden):
        raise PackingError("packing manifest contains a forbidden private field")


def validate_against_layout(
    packing: dict[str, object], layout: dict[str, object]
) -> None:
    container = packing["container"]
    blocks = packing["blocks"]
    packed_layout = layout["packed"]
    sections = layout["sections"]
    assert isinstance(container, dict) and isinstance(blocks, list)
    assert isinstance(packed_layout, dict) and isinstance(sections, list)
    if len(sections) != int(container["section_count"]):
        raise PackingError("packing and layout section counts differ")
    if int(packed_layout["sjcrunch2_header_offset"]) != int(container["packed_offset"]):
        raise PackingError("packing and layout container offsets differ")
    if int(packed_layout["sjcrunch2_end_offset"]) != int(container["packed_end_offset"]):
        raise PackingError("packing and layout container ends differ")

    layout_blocks = [block for section in sections for block in section["blocks"]]
    if len(layout_blocks) != len(blocks):
        raise PackingError("packing and layout block counts differ")
    for packed_block, layout_block in zip(blocks, layout_blocks):
        for key in ("index", "image_offset", "initialized_size", "packed_data_size"):
            if int(packed_block[key]) != int(layout_block[key]):
                raise PackingError(f"packing and layout block field {key!r} differs")


class LZO999:
    """Small ctypes adapter for the public liblzo2 high-compression API."""

    def __init__(self, library: str | None = None):
        candidates = [
            library,
            os.environ.get("SNESSTATION_LZO_LIBRARY"),
            ctypes.util.find_library("lzo2"),
            "liblzo2.so.2",
            "liblzo2.so",
        ]
        last_error: OSError | None = None
        self.library_name = ""
        self.lib: ctypes.CDLL | None = None
        for candidate in candidates:
            if not candidate:
                continue
            try:
                self.lib = ctypes.CDLL(candidate)
                self.library_name = candidate
                break
            except OSError as exc:
                last_error = exc
        if self.lib is None:
            detail = f": {last_error}" if last_error is not None else ""
            raise PackingError(
                "liblzo2 with lzo1x_999_compress_level is required"
                f"{detail}; install liblzo2 or set SNESSTATION_LZO_LIBRARY"
            )

        try:
            self.fn = self.lib.lzo1x_999_compress_level
        except AttributeError as exc:
            raise PackingError("selected LZO library lacks lzo1x_999_compress_level") from exc
        self.fn.argtypes = [
            ctypes.c_void_p,
            ctypes.c_size_t,
            ctypes.c_void_p,
            ctypes.POINTER(ctypes.c_size_t),
            ctypes.c_void_p,
            ctypes.c_void_p,
            ctypes.c_size_t,
            ctypes.c_void_p,
            ctypes.c_int,
        ]
        self.fn.restype = ctypes.c_int
        version_fn = getattr(self.lib, "lzo_version_string", None)
        if version_fn is not None:
            version_fn.restype = ctypes.c_char_p
            raw_version = version_fn()
            self.version = raw_version.decode("ascii") if raw_version else "unknown"
        else:
            self.version = "unknown"

    def __call__(self, data: bytes, level: int) -> bytes:
        capacity = len(data) + len(data) // 16 + 64 + 3
        output = (ctypes.c_ubyte * capacity)()
        output_size = ctypes.c_size_t(capacity)
        work = (ctypes.c_ubyte * LZO_WORK_MEMORY)()
        result = self.fn(
            data,
            len(data),
            output,
            ctypes.byref(output_size),
            work,
            None,
            0,
            None,
            level,
        )
        if result != 0:
            raise PackingError(f"lzo1x_999 compression failed with status {result}")
        return bytes(output[: output_size.value])


def build_container(
    image: bytes,
    layout: dict[str, object],
    packing: dict[str, object],
    compressor: Callable[[bytes, int], bytes],
) -> bytes:
    validate_packing_manifest(packing)
    validate_against_layout(packing, layout)
    image_meta = layout["image"]
    sections = layout["sections"]
    packed_meta = layout["packed"]
    assert isinstance(image_meta, dict) and isinstance(sections, list)
    assert isinstance(packed_meta, dict)
    if len(image) != int(image_meta["initialized_size"]):
        raise PackingError("candidate image size does not match the frozen layout")
    if sha256_bytes(image) != str(image_meta["sha256"]):
        raise PackingError("candidate image hash does not match the frozen layout")

    header_offset = int(packed_meta["sjcrunch2_header_offset"])
    result = bytearray(struct.pack("<II", int(layout["entry_address"]), len(sections)))
    flat_packing_blocks = iter(packing["blocks"])
    level = int(packing["compression_level"])

    for section in sections:
        assert isinstance(section, dict)
        expected_section_header = int(section["packed_header_offset"]) - header_offset
        if len(result) != expected_section_header:
            raise PackingError("generated section header offset differs from the layout")
        stream = bytearray()
        compressed_size = 0
        for block in section["blocks"]:
            assert isinstance(block, dict)
            packed_block = next(flat_packing_blocks)
            assert isinstance(packed_block, dict)
            offset = int(block["image_offset"])
            size = int(block["initialized_size"])
            chunk = image[offset : offset + size]
            if sha256_bytes(chunk) != str(block["sha256"]):
                raise PackingError(f"candidate block {block['index']} hash differs")
            compressed = chunk if block["encoding"] == "stored" else compressor(chunk, level)
            if len(compressed) >= len(chunk):
                compressed = chunk
            if len(compressed) != int(block["packed_data_size"]):
                raise PackingError(
                    f"block {block['index']} packed size differs: "
                    f"expected {block['packed_data_size']}, got {len(compressed)}"
                )
            if sha256_bytes(compressed) != str(packed_block["compressed_sha256"]):
                raise PackingError(f"block {block['index']} compressed hash differs")

            absolute_header = header_offset + len(result) + 20 + len(stream)
            if absolute_header != int(block["packed_header_offset"]):
                raise PackingError(f"block {block['index']} header offset differs")
            stream.extend(struct.pack("<II", size, len(compressed)))
            stream.extend(compressed)
            compressed_size += len(compressed)

        if compressed_size != int(section["compressed_data_size"]):
            raise PackingError("generated compressed section size differs")
        padding_size = int(section["padding_size"])
        stream.extend(b"\0" * padding_size)
        if len(stream) != int(section["file_size"]):
            raise PackingError("generated section file size differs")
        result.extend(
            struct.pack(
                "<IIIII",
                compressed_size,
                int(section["initialized_size"]),
                int(section["zero_fill_size"]),
                int(section["virtual_address"]),
                len(stream),
            )
        )
        result.extend(stream)

    container = packing["container"]
    assert isinstance(container, dict)
    if len(result) != int(container["size"]):
        raise PackingError("generated SJCRUNCH2 container size differs")
    if sha256_bytes(result) != str(container["sha256"]):
        raise PackingError("generated SJCRUNCH2 container hash differs")
    return bytes(result)


def first_difference(expected: bytes, actual: bytes) -> int | None:
    for index, (left, right) in enumerate(zip(expected, actual)):
        if left != right:
            return index
    return min(len(expected), len(actual)) if len(expected) != len(actual) else None


def run_pack(args: argparse.Namespace, *, compare_reference: bool) -> None:
    layout = load_layout_manifest(args.layout)
    packing = load_packing_manifest(args.manifest)
    compressor = LZO999(args.lzo_library)
    try:
        image = args.image.read_bytes()
    except OSError as exc:
        raise PackingError(f"cannot read candidate image {args.image}: {exc}") from exc
    container = build_container(image, layout, packing, compressor)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(container)

    if compare_reference:
        try:
            reference = args.packed.read_bytes()
        except OSError as exc:
            raise PackingError(f"cannot read packed reference {args.packed}: {exc}") from exc
        packed_meta = layout["packed"]
        packing_container = packing["container"]
        assert isinstance(packed_meta, dict) and isinstance(packing_container, dict)
        if len(reference) != int(packed_meta["size"]):
            raise PackingError("packed reference size differs from the frozen layout")
        if sha256_bytes(reference) != str(packed_meta["sha256"]):
            raise PackingError("packed reference hash differs from the frozen layout")
        start = int(packing_container["packed_offset"])
        expected = reference[start : start + len(container)]
        difference = first_difference(expected, container)
        if difference is not None:
            raise PackingError(f"generated container first differs at +0x{difference:08x}")

    print(f"compressor=liblzo {compressor.version} level={packing['compression_level']}")
    print(f"blocks={len(packing['blocks'])}/{len(packing['blocks'])} exact")
    print(f"container={len(container)}/{len(container)} bytes exact")
    print(f"container_sha256={sha256_bytes(container)}")
    outer = packing["outer_elf"]
    assert isinstance(outer, dict)
    print(f"outer_elf_remaining={outer['unreproduced_size']} bytes (stub and ELF metadata)")


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Reproduce the SJCRUNCH2 compressed container")
    subparsers = parser.add_subparsers(dest="command", required=True)
    validate = subparsers.add_parser("validate", help="validate public hash-only metadata")
    validate.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
    validate.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    for name in ("pack", "check"):
        command = subparsers.add_parser(name)
        command.add_argument("--image", type=Path, default=DEFAULT_IMAGE)
        command.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
        command.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
        command.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
        command.add_argument("--lzo-library")
        if name == "check":
            command.add_argument("--packed", type=Path, default=DEFAULT_PACKED)
    return parser.parse_args(argv)


def main(argv: Sequence[str] | None = None) -> None:
    args = parse_args(argv)
    try:
        layout = load_layout_manifest(args.layout)
        packing = load_packing_manifest(args.manifest)
        validate_against_layout(packing, layout)
        if args.command == "validate":
            print(
                f"SJCRUNCH2 packing manifest: OK "
                f"({len(packing['blocks'])} blocks; level {packing['compression_level']})"
            )
            return
        run_pack(args, compare_reference=args.command == "check")
    except (PackingError, ValueError) as exc:
        raise SystemExit(str(exc)) from exc


if __name__ == "__main__":
    main()
