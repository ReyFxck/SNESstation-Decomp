#!/usr/bin/env python3
"""Freeze the first honest Stage-3G whole-image link/layout diagnostic.

The probe links the current Stage-3F ET_REL aggregate, fixes only the 179
sections whose VMAs and payloads are already independently proved, emits a raw
initialized image, and compares it with the SHA-verified private target.  It
is deliberately diagnostic: it does not substitute missing exact objects,
claim complete data bounds, or emit/pack a replacement emulator ELF.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path
from typing import Sequence

import data_backing
from compare_elf_functions import ELFFile
from source_aliases import resolve_tool, run, sibling_tool

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_BUILD = ROOT / "build/link-layout-probe"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/link_layout_probe.json"
FORMAT = "snesstation-stage3g-clean-link-layout-probe"
SCHEMA = 1

EXPECTED = {
    "target_entry_address": 0x00100008,
    "diagnostic_entry_address": 0x00111F70,
    "target_initialized_size": 3_304_936,
    "diagnostic_unpadded_size": 3_304_836,
    "terminal_zero_padding": 100,
    "fixed_sections": 179,
    "fixed_initialized_sections": 155,
    "fixed_zero_fill_sections": 24,
    "chunk_count": 51,
    "exact_chunks": 12,
    "mismatching_chunks": 39,
    "equal_bytes": 1_421_069,
    "differing_bytes": 1_883_867,
    "diagnostic_unpadded_sha256":
        "f0b35112afa096488b6f3e97ff21e8d8af391764cda0148dc52e869b84664feb",
    "diagnostic_padded_sha256":
        "cf1b7e4003fe1dd5d3e9e5941074cc8c8395bd683be79d9454337838a035bc79",
    "target_sha256":
        "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b",
}


class LinkLayoutProbeError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise LinkLayoutProbeError(message)


def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def file_digest(path: Path) -> str:
    return digest(path.read_bytes())


def load_layout(path: Path) -> dict:
    try:
        layout = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read layout manifest {path}: {exc}")
    image = layout.get("image", {})
    if (
        layout.get("format") != "snesstation-unpacked-layout-oracle"
        or layout.get("schema_version") != 1
        or layout.get("entry_address") != EXPECTED["target_entry_address"]
        or image.get("base_address") != 0x00100000
        or image.get("initialized_size") != EXPECTED["target_initialized_size"]
        or image.get("sha256") != EXPECTED["target_sha256"]
        or image.get("chunk_count") != EXPECTED["chunk_count"]
        or len(image.get("chunks", [])) != EXPECTED["chunk_count"]
    ):
        fail("unpacked layout oracle drift")
    return layout


def load_sections(path: Path) -> list[dict[str, str]]:
    args = data_backing.parse_args(["validate", "--sections", str(path)])
    _rows, sections = data_backing.validate(args)
    initialized = sum(row["region"] == "initialized" for row in sections)
    zero_fill = sum(row["region"] == "zero-fill" for row in sections)
    if (
        len(sections) != EXPECTED["fixed_sections"]
        or initialized != EXPECTED["fixed_initialized_sections"]
        or zero_fill != EXPECTED["fixed_zero_fill_sections"]
    ):
        fail("fixed Stage-3F section roster drift")
    return sections


def source_contract(args: argparse.Namespace, sections: Sequence[dict[str, str]], layout: dict) -> dict:
    return {
        "stage3f_sections_sha256": file_digest(args.sections),
        "unpacked_layout_sha256": file_digest(args.layout),
        "fixed_section_count": len(sections),
        "fixed_initialized_section_count": sum(
            row["region"] == "initialized" for row in sections
        ),
        "fixed_zero_fill_section_count": sum(
            row["region"] == "zero-fill" for row in sections
        ),
        "image_base_address": layout["image"]["base_address"],
        "initialized_end_address": layout["image"]["initialized_end_address"],
        "memory_end_address": layout["image"]["memory_end_address"],
    }


def claims() -> dict:
    return {
        "fixed_section_vmas_and_sizes_proved": True,
        "fixed_initialized_payloads_exact": True,
        "source_relocations_applied": True,
        "complete_object_extents_proved": False,
        "exact_implementation_selection_complete": False,
        "replacement_elf": False,
        "sjcrunch2_packing_reproduced": False,
        "packed_hash_matched": False,
        "unpacked_hash_matched": False,
    }


def render_script(sections: Sequence[dict[str, str]]) -> str:
    lines = [
        "/* Stage-3G diagnostic placement; not the historical final link script. */",
        'OUTPUT_FORMAT("elf32-littlemips")',
        "OUTPUT_ARCH(mips)",
        "ENTRY(snes_p28_00100008)",
        "SECTIONS {",
        "  . = 0x00100000;",
        "  .text : { *(.text) }",
        "  .rodata : { *(.rodata) }",
        "  .data : { *(.data) }",
        "  .bss (NOLOAD) : { *(.bss) *(.bss.stage3.compatibility) }",
    ]
    for row in sections:
        suffix = " (NOLOAD)" if row["region"] == "zero-fill" else ""
        lines.append(
            f"  {row['section']} {row['target_address']}{suffix} : "
            f"{{ KEEP(*({row['section']})) }}"
        )
    lines.extend([
        "  /DISCARD/ : { *(.reginfo) *(.mdebug*) *(.comment) *(.pdr) *(.gnu.attributes) }",
        "}",
    ])
    return "\n".join(lines) + "\n"


def elf_entry(elf: ELFFile) -> int:
    if elf.elf_class != 1:
        fail("diagnostic output is not ELF32")
    return struct.unpack_from(elf.endian + "I", elf.data, 24)[0]


def section_bytes(elf: ELFFile, section) -> bytes:
    if section.type == 8:
        return bytes(section.size)
    return elf.data[section.offset:section.offset + section.size]


def verify_fixed_output(elf: ELFFile, sections: Sequence[dict[str, str]]) -> None:
    by_name = {}
    for section in elf.sections:
        if section.name:
            by_name.setdefault(section.name, []).append(section)
    for row in sections:
        matches = by_name.get(row["section"], [])
        if len(matches) != 1:
            fail(f"missing/duplicate fixed output section: {row['section']}")
        section = matches[0]
        expected_type = 8 if row["region"] == "zero-fill" else 1
        if (
            section.address != int(row["target_address"], 0)
            or section.size != int(row["extent_hex"], 0)
            or section.type != expected_type
        ):
            fail(f"fixed output section geometry drift: {row['section']}")
        if row["region"] == "initialized" and digest(section_bytes(elf, section)) != row["sha256"]:
            fail(f"fixed output section payload drift: {row['section']}")


def compare_chunks(candidate: bytes, layout: dict) -> tuple[list[int], list[int]]:
    exact, different = [], []
    for row in layout["image"]["chunks"]:
        start = row["image_offset"]
        payload = candidate[start:start + row["size"]]
        if len(payload) != row["size"]:
            fail(f"diagnostic image lacks oracle chunk {row['index']}")
        (exact if digest(payload) == row["sha256"] else different).append(row["index"])
    return exact, different


def probe(args: argparse.Namespace, sections: Sequence[dict[str, str]], layout: dict) -> dict:
    if not args.input.is_file():
        fail(f"missing Stage-3F aggregate: {args.input}; run make data-backing-check")
    reference = args.reference.read_bytes()
    if digest(reference) != EXPECTED["target_sha256"]:
        fail("private unpacked reference SHA-256 drift")
    before = ELFFile(args.input)
    if (before.elf_class, before.endian, before.machine, before.file_type) != (1, "<", 8, 1):
        fail("Stage-3F input is not little-endian MIPS ELF32 ET_REL")
    data_backing.check_sections(args.input, sections)

    compiler = resolve_tool(args.compiler)
    linker = sibling_tool(compiler, None, "ld")
    objcopy = sibling_tool(compiler, None, "objcopy")
    args.build_dir.mkdir(parents=True, exist_ok=True)
    script = args.build_dir / "diagnostic-layout.ld"
    output = args.build_dir / "stage3g-diagnostic.elf"
    raw_path = args.build_dir / "stage3g-diagnostic.unpadded.bin"
    padded_path = args.build_dir / "stage3g-diagnostic.padded.bin"
    script.write_text(render_script(sections), encoding="utf-8")
    run([str(linker), "-EL", "-T", str(script), "-o", str(output), str(args.input)])
    after = ELFFile(output)
    if (after.elf_class, after.endian, after.machine, after.file_type) != (1, "<", 8, 2):
        fail("diagnostic link is not little-endian MIPS ELF32 ET_EXEC")
    if any(section.type in (4, 9) and section.size for section in after.sections):
        fail("diagnostic executable still contains relocations")
    verify_fixed_output(after, sections)
    run([str(objcopy), "-O", "binary", str(output), str(raw_path)])
    unpadded = raw_path.read_bytes()
    expected_size = layout["image"]["initialized_size"]
    if len(unpadded) > expected_size:
        fail("diagnostic initialized image exceeds target size")
    padded = unpadded + bytes(expected_size - len(unpadded))
    padded_path.write_bytes(padded)
    exact, different = compare_chunks(padded, layout)
    differing_bytes = sum(left != right for left, right in zip(padded, reference))
    result = {
        "target_entry_address": layout["entry_address"],
        "diagnostic_entry_address": elf_entry(after),
        "target_initialized_size": expected_size,
        "diagnostic_unpadded_size": len(unpadded),
        "terminal_zero_padding": expected_size - len(unpadded),
        "fixed_sections": len(sections),
        "fixed_initialized_sections": sum(row["region"] == "initialized" for row in sections),
        "fixed_zero_fill_sections": sum(row["region"] == "zero-fill" for row in sections),
        "chunk_count": len(exact) + len(different),
        "exact_chunks": len(exact),
        "mismatching_chunks": len(different),
        "exact_chunk_indices": exact,
        "mismatching_chunk_indices": different,
        "equal_bytes": len(padded) - differing_bytes,
        "differing_bytes": differing_bytes,
        "diagnostic_unpadded_sha256": digest(unpadded),
        "diagnostic_padded_sha256": digest(padded),
        "target_sha256": digest(reference),
    }
    for key, expected in EXPECTED.items():
        if result.get(key) != expected:
            fail(f"frozen Stage-3G result drift for {key}: {result.get(key)!r} != {expected!r}")
    return result


def frozen_document(args: argparse.Namespace, sections, layout, result) -> dict:
    return {
        "format": FORMAT,
        "schema_version": SCHEMA,
        "source_contract": source_contract(args, sections, layout),
        "result": result,
        "claims": claims(),
    }


def validate(args: argparse.Namespace) -> dict:
    sections = load_sections(args.sections)
    layout = load_layout(args.layout)
    try:
        document = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read frozen Stage-3G manifest {args.manifest}: {exc}")
    if document.get("format") != FORMAT or document.get("schema_version") != SCHEMA:
        fail("Stage-3G manifest identity drift")
    if document.get("source_contract") != source_contract(args, sections, layout):
        fail("Stage-3G source contract drift")
    result = document.get("result", {})
    for key, expected in EXPECTED.items():
        if result.get(key) != expected:
            fail(f"frozen Stage-3G metric drift: {key}")
    if result.get("exact_chunks") + result.get("mismatching_chunks") != result.get("chunk_count"):
        fail("Stage-3G chunk accounting drift")
    if result.get("equal_bytes") + result.get("differing_bytes") != result.get("target_initialized_size"):
        fail("Stage-3G byte accounting drift")
    if document.get("claims") != claims():
        fail("Stage-3G claim boundary drift")
    return document


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "probe", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--sections", type=Path, default=DEFAULT_SECTIONS)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--compiler", default="ee-gcc")
    args = parser.parse_args(argv)
    for name, value in vars(args).items():
        if isinstance(value, Path):
            setattr(args, name, value.expanduser().resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "validate":
            document = validate(args)
        else:
            sections = load_sections(args.sections)
            layout = load_layout(args.layout)
            result = probe(args, sections, layout)
            document = frozen_document(args, sections, layout, result)
            if args.command == "capture":
                args.manifest.parent.mkdir(parents=True, exist_ok=True)
                args.manifest.write_text(
                    json.dumps(document, indent=2, sort_keys=True) + "\n",
                    encoding="utf-8",
                )
            else:
                if validate(args) != document:
                    fail("private Stage-3G probe differs from frozen manifest")
        result = document["result"]
        print(
            "verified Stage-3G clean link/layout probe: "
            f"fixed={result['fixed_sections']}/{result['fixed_sections']} "
            f"initialized_exact={result['fixed_initialized_sections']}/"
            f"{result['fixed_initialized_sections']} chunks={result['exact_chunks']}/"
            f"{result['chunk_count']} differing_bytes={result['differing_bytes']}"
        )
        print(
            f"diagnostic entry=0x{result['diagnostic_entry_address']:08x}; "
            f"target entry=0x{result['target_entry_address']:08x}; replacement ELF: not yet"
        )
        return 0
    except (
        LinkLayoutProbeError,
        data_backing.DataBackingError,
        OSError,
        ValueError,
        KeyError,
        RuntimeError,
    ) as exc:
        print(f"Stage-3G link/layout probe: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
