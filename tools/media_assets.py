#!/usr/bin/env python3
"""Integrate the six remaining verified private media containers.

Stage 3M closes image windows 15 through 34.  The tracked contract stores only
public geometry and hashes.  The assembler reads asset bytes from a verified,
user-supplied unpacked reference into ignored build products; no graphics,
audio, icon or other target payload is stored in the repository.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import shlex
import subprocess
from pathlib import Path
from typing import Sequence

import data_backing
import extract_embedded_assets as assets
import historical_tail_data as stage3i
import link_layout_probe
import startup_integration as startup
import tail_metadata as stage3k
import window36_data as stage3l
from compare_elf_functions import ELFFile
from source_aliases import resolve_tool


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/media_assets.json"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_STARTUP = ROOT / "build/startup-integration/crt0-stage3g.o"
DEFAULT_STAGE3I = ROOT / "build/historical-tail-data"
DEFAULT_STAGE3J = ROOT / "build/runtime-tail-data"
DEFAULT_STAGE3K = ROOT / "build/tail-metadata"
DEFAULT_STAGE3L = ROOT / "build/window36-data"
DEFAULT_BUILD = ROOT / "build/media-assets"

FORMAT = "snesstation-stage3m-embedded-media"
SCHEMA = 1
TARGET_BASE = 0x00100000
TARGET_SHA256 = assets.REFERENCE_SHA256

MEDIA_NAMES = (
    "frontend_background_iif",
    "frontend_logo_iif",
    "frontend_panel_corner_iif",
    "frontend_font_bfnt",
    "azazel_mod",
    "memory_card_icon",
)


def M(name: str, envelope_sha256: str) -> dict:
    item = assets.ASSET_BY_NAME[name]
    if item.size_word_va is None:
        raise RuntimeError(f"{name}: missing public size-word identity")
    return {
        "name": name,
        "kind": item.kind,
        "section": f".data.stage3m.asset.{name}",
        "address": item.va,
        "asset_size": item.size,
        "asset_sha256": item.sha256,
        "size_word_address": item.size_word_va,
        "envelope_size": item.size_word_va + 4 - item.va,
        "envelope_sha256": envelope_sha256,
    }


MEDIA_SECTIONS = (
    M("frontend_background_iif", "d31a77cfd3704af1108435b8eff377ab60ea92b1e96013307a450f9079d58fa7"),
    M("frontend_logo_iif", "bf585495bf24550639183763744551542a82c26cdc4fdc8f7e2ce9313b4219ff"),
    M("frontend_panel_corner_iif", "0c25c442442e604a9ca76dc4c822c9130b0f6ef1e9dc089d104944186020721c"),
    M("frontend_font_bfnt", "525cc3e74eff187ce5455c4271b85c3e5f9f4799d7671f06ab391d3550bbb630"),
    M("azazel_mod", "a58f0105057a054f469a6dd9bf27d5527022662af415b21f53e94485f92595e2"),
    M("memory_card_icon", "71b0443e1dd7169f83611138d0bd676ec3fbe604252135c19c9230aee82c3715"),
)

ABSORBED_FIXED = (
    ".data.stage3ce.va_001fc752",
    ".data.stage3f.va_00335278",
)
EXACT_CHUNKS = [*range(12, 35), *range(36, 51)]
EXPECTED = {
    "media_sections": 6,
    "media_asset_bytes": 1_284_388,
    "media_envelope_bytes": 1_284_412,
    "size_words": 6,
    "absorbed_fixed_sections": 2,
    "closed_media_windows": 20,
    "windows15_34_differing_bytes": 0,
    "exact_chunks": 38,
    "mismatching_chunks": 13,
    "differing_bytes": 661_433,
    "prior_differing_bytes": 1_845_637,
    "media_differing_bytes_removed": 1_184_204,
    "chunk_count": 51,
    "target_initialized_size": 3_304_936,
    "first_differing_address": 0x00100114,
    "target_sha256": TARGET_SHA256,
}


class MediaAssetsError(RuntimeError):
    """The Stage-3M public contract or private integration drifted."""


def fail(message: str) -> None:
    raise MediaAssetsError(message)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def run(command: Sequence[str | Path], cwd: Path = ROOT) -> str:
    result = subprocess.run(
        [str(item) for item in command],
        cwd=cwd,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    if result.returncode:
        fail(f"command failed: {shlex.join(map(str, command))}\n{result.stdout[-5000:]}")
    return result.stdout.strip()


def section(elf: ELFFile, name: str):
    found = [item for item in elf.sections if item.name == name]
    if len(found) != 1:
        fail(f"missing/duplicate section: {name}")
    return found[0]


def media_document() -> list[dict]:
    return [dict(row) for row in MEDIA_SECTIONS]


def claims() -> dict:
    return {
        "media_ranges_hash_verified": True,
        "size_words_verified": True,
        "windows_15_through_34_exact": True,
        "private_target_bytes_stored": False,
        "replacement_elf": False,
        "unpacked_hash_matched": False,
        "packed_hash_matched": False,
    }


def frozen_document(result: dict) -> dict:
    return {
        "format": FORMAT,
        "schema_version": SCHEMA,
        "prior_manifest_sha256": digest(stage3l.DEFAULT_MANIFEST.read_bytes()),
        "asset_catalog_sha256": digest(assets.ROOT.joinpath("analysis/embedded_assets.csv").read_bytes()),
        "media_sections": media_document(),
        "absorbed_fixed_sections": list(ABSORBED_FIXED),
        "result": result,
        "claims": claims(),
    }


def validate_catalog() -> None:
    if tuple(item["name"] for item in MEDIA_SECTIONS) != MEDIA_NAMES:
        fail("media roster drift")
    for row in MEDIA_SECTIONS:
        item = assets.ASSET_BY_NAME.get(row["name"])
        if item is None:
            fail(f"missing asset catalog entry: {row['name']}")
        if item.size_word_va != item.end_va:
            fail(f"{row['name']}: size word no longer follows the asset")
        if row["address"] != item.va or row["asset_size"] != item.size:
            fail(f"{row['name']}: asset geometry drift")
        if row["envelope_size"] != item.size + 4:
            fail(f"{row['name']}: envelope geometry drift")


def validate(args: argparse.Namespace) -> dict:
    stage3l.validate(stage3l.parse_args(["validate"]))
    validate_catalog()
    try:
        document = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read media manifest: {exc}")
    if document.get("format") != FORMAT or document.get("schema_version") != SCHEMA:
        fail("media identity drift")
    if document.get("prior_manifest_sha256") != digest(stage3l.DEFAULT_MANIFEST.read_bytes()):
        fail("prior checkpoint drift")
    catalog = assets.ROOT / "analysis/embedded_assets.csv"
    if document.get("asset_catalog_sha256") != digest(catalog.read_bytes()):
        fail("embedded-asset catalog drift")
    if document.get("media_sections") != media_document():
        fail("media section contract drift")
    if document.get("absorbed_fixed_sections") != list(ABSORBED_FIXED):
        fail("absorbed fixed-section roster drift")
    for key, value in EXPECTED.items():
        if document.get("result", {}).get(key) != value:
            fail(f"frozen media metric drift: {key}")
    if document.get("result", {}).get("exact_chunk_indices") != EXACT_CHUNKS:
        fail("exact-window roster drift")
    if document.get("claims") != claims():
        fail("media claim boundary drift")
    return document


def verify_reference(reference: bytes) -> None:
    if len(reference) != assets.REFERENCE_SIZE or digest(reference) != TARGET_SHA256:
        fail("private unpacked reference identity drift")
    for row in MEDIA_SECTIONS:
        item = assets.ASSET_BY_NAME[row["name"]]
        payload = assets.image_slice(reference, item)
        if assets.validate_size_word(reference, item) != item.size:
            fail(f"{item.name}: size-word value drift")
        start = item.va - TARGET_BASE
        envelope = reference[start:start + row["envelope_size"]]
        if len(payload) != item.size or digest(envelope) != row["envelope_sha256"]:
            fail(f"{item.name}: media envelope drift")


def quote_assembly_path(path: Path) -> str:
    value = str(path.resolve())
    if "\n" in value or "\r" in value:
        fail("reference path cannot contain a newline")
    return value.replace("\\", "\\\\").replace('"', '\\"')


def render_assembly(reference: Path) -> str:
    quoted = quote_assembly_path(reference)
    lines = [
        "/* Generated privately from a hash-verified user-supplied image. */",
        "/* The tracked source contains no media payload bytes. */",
    ]
    for row in MEDIA_SECTIONS:
        lines.extend(
            [
                f'.section {row["section"]},"aw",@progbits',
                ".align 2",
                f'.incbin "{quoted}",{row["address"] - TARGET_BASE},{row["asset_size"]}',
                f'.4byte 0x{row["asset_size"]:x}',
            ]
        )
    return "\n".join(lines) + "\n"


def verify_media_object(path: Path, reference: bytes) -> None:
    elf = ELFFile(path)
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, 1):
        fail("generated media object is not MIPS ELF32 ET_REL")
    for row in MEDIA_SECTIONS:
        item = section(elf, row["section"])
        start = row["address"] - TARGET_BASE
        target = reference[start:start + row["envelope_size"]]
        if item.type != 1 or item.size != row["envelope_size"]:
            fail(f"generated media geometry drift: {row['name']}")
        if stage3i.section_bytes(elf, item) != target:
            fail(f"generated media payload drift: {row['name']}")


def update_linker_script(
    base_script: str,
    input_path: Path,
    sections: Sequence[dict],
) -> tuple[str, list[dict]]:
    by_name = {row["section"]: row for row in sections}
    script = base_script
    for name in ABSORBED_FIXED:
        row = by_name.get(name)
        if row is None:
            fail(f"missing fixed section selected for absorption: {name}")
        address = int(row["target_address"], 0)
        placement = f"  {name} 0x{address:08x} : {{ KEEP(*({name})) }}\n"
        if script.count(placement) != 1:
            fail(f"cannot remove prior placement for {name}")
        script = script.replace(placement, "", 1)

    marker = "  .bss.stage3g.crt0 0x00426e80"
    if script.count(marker) != 1:
        fail("Stage-3L linker insertion marker drift")
    insertion = "\n".join(
        f"  {row['section']} 0x{row['address']:08x} : {{ KEEP(*({row['section']})) }}"
        for row in MEDIA_SECTIONS
    )
    script = script.replace(marker, insertion + "\n" + marker, 1)

    discard_marker = "*(.data.stage3g.crt0)"
    if script.count(discard_marker) != 1:
        fail("Stage-3L discard marker drift")
    discarded = " ".join(f"*({name})" for name in ABSORBED_FIXED)
    script = script.replace(discard_marker, f"{discarded} {discard_marker}", 1)

    aliases = stage3l.absorbed_aliases(input_path, sections, ABSORBED_FIXED)
    for name in aliases:
        if f"{name} =" in script:
            fail(f"new absorbed alias already exists: {name}")
    assignments = "".join(
        f"{name} = 0x{address:08x};\n" for name, address in sorted(aliases.items())
    )
    discarded_names = set(stage3k.ABSORBED_FIXED) | set(stage3l.ABSORBED_FIXED) | set(ABSORBED_FIXED)
    retained = [row for row in sections if row["section"] not in discarded_names]
    return assignments + script, retained


def probe(args: argparse.Namespace) -> dict:
    prior = stage3l.validate(stage3l.parse_args(["validate"]))
    sections, layout = startup.load_inputs(
        startup.parse_args(
            ["validate", "--sections", str(args.sections), "--layout", str(args.layout)]
        )
    )
    reference = args.reference.read_bytes()
    verify_reference(reference)
    data_backing.check_sections(args.input, sections)

    compiler = resolve_tool(args.compiler)
    if run([compiler, "-dumpversion"]) != "3.2.2" or run([compiler, "-dumpmachine"]) != "ee":
        fail("Stage-3M requires EE GCC 3.2.2")
    linker = resolve_tool(args.ld) if args.ld else compiler.with_name("ee-ld")
    objcopy = resolve_tool(args.objcopy) if args.objcopy else compiler.with_name("ee-objcopy")

    inputs = [
        args.startup_object,
        args.input,
        args.stage3i_build / "frontend-eh-frames.o",
        args.stage3i_build / "providers.o",
        args.stage3i_build / "semantic-cfi.o",
        args.stage3j_build / "runtime-tail.o",
        args.stage3k_build / "tail-payloads.o",
        args.stage3k_build / "tail-semantics.o",
        args.stage3l_build / "window36-payloads.o",
        args.stage3l_build / "window36-semantics.o",
    ]
    base_script_path = args.stage3l_build / "window36-data.ld"
    if not all(path.is_file() for path in [*inputs, base_script_path]):
        fail("missing Stage-3L dependency; run make window36-data")

    args.build_dir.mkdir(parents=True, exist_ok=True)
    source = args.build_dir / "media-assets.S"
    media_object = args.build_dir / "media-assets.o"
    source.write_text(render_assembly(args.reference), encoding="utf-8")
    stage3i.compile_one(
        compiler,
        ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"),
        source,
        media_object,
    )
    verify_media_object(media_object, reference)

    script, retained = update_linker_script(
        base_script_path.read_text(encoding="utf-8"), args.input, sections
    )
    linker_script = args.build_dir / "media-assets.ld"
    linker_script.write_text(script, encoding="utf-8")
    output = args.build_dir / "stage3m-media-integrated.elf"
    run([linker, "-EL", "-T", linker_script, "-o", output, *inputs, media_object])

    elf = ELFFile(output)
    startup.verify_symbols(elf)
    stage3i.verify_fixed_output(elf, retained)
    for row in MEDIA_SECTIONS:
        item = section(elf, row["section"])
        start = row["address"] - TARGET_BASE
        target = reference[start:start + row["envelope_size"]]
        if item.address != row["address"] or stage3i.section_bytes(elf, item) != target:
            fail(f"linked media section differs: {row['name']}")

    raw_path = args.build_dir / "stage3m-media-integrated.unpadded.bin"
    padded_path = args.build_dir / "stage3m-media-integrated.padded.bin"
    run([objcopy, "-O", "binary", output, raw_path])
    raw = raw_path.read_bytes()
    if len(raw) > len(reference):
        fail("integrated diagnostic exceeds target image size")
    padded = raw + bytes(len(reference) - len(raw))
    padded_path.write_bytes(padded)
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [
        index for index, (left, right) in enumerate(zip(padded, reference)) if left != right
    ]
    differing_bytes = len(differences)
    result = {
        "media_sections": len(MEDIA_SECTIONS),
        "media_asset_bytes": sum(row["asset_size"] for row in MEDIA_SECTIONS),
        "media_envelope_bytes": sum(row["envelope_size"] for row in MEDIA_SECTIONS),
        "size_words": len(MEDIA_SECTIONS),
        "absorbed_fixed_sections": len(ABSORBED_FIXED),
        "closed_media_windows": sum(index in exact for index in range(15, 35)),
        "windows15_34_differing_bytes": sum(
            left != right
            for left, right in zip(
                padded[15 * 65536:35 * 65536],
                reference[15 * 65536:35 * 65536],
            )
        ),
        "chunk_count": len(exact) + len(different),
        "target_initialized_size": len(reference),
        "exact_chunks": len(exact),
        "mismatching_chunks": len(different),
        "exact_chunk_indices": exact,
        "mismatching_chunk_indices": different,
        "differing_bytes": differing_bytes,
        "first_differing_address": TARGET_BASE + differences[0],
        "integrated_padded_sha256": digest(padded),
        "target_sha256": digest(reference),
        "prior_differing_bytes": prior["result"]["differing_bytes"],
        "media_differing_bytes_removed": prior["result"]["differing_bytes"] - differing_bytes,
    }
    return result


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "probe", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    parser.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--sections", type=Path, default=DEFAULT_SECTIONS)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--startup-object", type=Path, default=DEFAULT_STARTUP)
    parser.add_argument("--stage3i-build", type=Path, default=DEFAULT_STAGE3I)
    parser.add_argument("--stage3j-build", type=Path, default=DEFAULT_STAGE3J)
    parser.add_argument("--stage3k-build", type=Path, default=DEFAULT_STAGE3K)
    parser.add_argument("--stage3l-build", type=Path, default=DEFAULT_STAGE3L)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--compiler", default="ee-g++")
    parser.add_argument("--ld")
    parser.add_argument("--objcopy")
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
            result = probe(args)
            document = frozen_document(result)
            if args.command == "capture":
                args.manifest.parent.mkdir(parents=True, exist_ok=True)
                args.manifest.write_text(
                    json.dumps(document, indent=2, sort_keys=True) + "\n",
                    encoding="utf-8",
                )
            elif validate(args) != document:
                fail("private result differs from frozen manifest")
        result = document["result"]
        print(
            "verified embedded media: "
            f"sections={result['media_sections']} assets={result['media_asset_bytes']} "
            f"envelope={result['media_envelope_bytes']} bytes"
        )
        print(
            f"whole-image chunks={result['exact_chunks']}/51 "
            f"remaining={result['mismatching_chunks']} "
            f"differing_bytes={result['differing_bytes']} "
            f"windows15_34_differences={result['windows15_34_differing_bytes']}; "
            "replacement ELF: not yet"
        )
        return 0
    except (
        MediaAssetsError,
        stage3l.Window36Error,
        stage3k.TailMetadataError,
        stage3i.HistoricalTailError,
        data_backing.DataBackingError,
        OSError,
        ValueError,
        KeyError,
        RuntimeError,
    ) as exc:
        print(f"embedded media: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
