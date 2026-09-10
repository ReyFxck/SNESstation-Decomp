#!/usr/bin/env python3
"""Integrate the six proved Stage-3P code windows without freezing private payloads.

The gate assembles each 64 KiB window from historical/recovered objects and
explicit assembly proofs already accepted by the matching ledgers. Only
relocation-controlled instruction bits may be filled from the private unpacked
reference. Ten newly labelled assembly residuals cover old GCC schedules that
are not reproduced by the pinned source builds. Generated window payloads stay
below ``build/``.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Sequence

import link_layout_probe
import startup_integration as startup
import window11_rodata as stage3o
from compare_elf_functions import ELFFile, Symbol
from source_aliases import resolve_tool


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/code_windows.json"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_STARTUP = ROOT / "build/startup-integration/crt0-stage3g.o"
DEFAULT_STAGE3I = ROOT / "build/historical-tail-data"
DEFAULT_STAGE3J = ROOT / "build/runtime-tail-data"
DEFAULT_STAGE3K = ROOT / "build/tail-metadata"
DEFAULT_STAGE3L = ROOT / "build/window36-data"
DEFAULT_STAGE3M = ROOT / "build/media-assets"
DEFAULT_STAGE3N = ROOT / "build/window35-data"
DEFAULT_STAGE3O = ROOT / "build/window11-rodata"
DEFAULT_BUILD = ROOT / "build/code-windows"
DEFAULT_RESIDUAL = ROOT / "matching/candidates/stage3p_code_residual_exact.S"

FORMAT = "snesstation-stage3p-public-code-windows"
SCHEMA = 1
TARGET_BASE = 0x00100000
WINDOW_START = 0x00110000
WINDOW_END = 0x00170000
WINDOW_SIZE = 0x10000
TARGET_SHA256 = stage3o.TARGET_SHA256

EVIDENCE_MANIFESTS = (
    "analysis/matching/hunt1000plus-v46-validated-42.tsv",
    "analysis/matching/hunt1000plus-v47-validated-79.tsv",
    "analysis/matching/hunt1041-v48-validated-25.tsv",
    "analysis/matching/hunt1041-v49-validated-20.tsv",
    "analysis/matching/hunt1041-v51-validated-16.tsv",
    "analysis/matching/hunt1041-v52-validated-17.tsv",
    "analysis/matching/hunt1041-v72-validated-v53-6.tsv",
    "analysis/matching/hunt1041-v73-validated-2.tsv",
    "analysis/matching/hunt1041-v80-validated-quickwins-23.tsv",
    "analysis/matching/hunt1041-v81-validated-final20.tsv",
)
EVIDENCE_TARGETS = (
    "hunt1000plus-v46-evidence",
    "hunt1000plus-v47-evidence",
    "hunt1041-v48-evidence",
    "hunt1041-v49-evidence",
    "hunt1041-v51-evidence",
    "hunt1041-v52-evidence",
    "hunt1041-v72-evidence",
    "hunt1041-v73-evidence",
    "hunt1041-v80-evidence",
    "hunt1041-v81-evidence",
)


@dataclass(frozen=True)
class SliceSpec:
    name: str
    object: str
    address: int
    size: int
    section: str = ".text"
    source_offset: int = 0
    symbol: str = ""


# Exact whole historical translation units and the exact CHEATS tail.  Their
# placement was established by the formal matching ledgers above.
FIXED_SLICES = (
    SliceSpec("cheats_text_tail", "build/window11-rodata/source-objects/snes-CHEATS.o", 0x00110000, 0x40E0, ".text", 0x20A0),
    SliceSpec("cpu", "build/matching/hunt1000plus-v46-closure/snes/CPU.o", 0x001159F4, 0x03BC),
    SliceSpec("cpuexec", "build/matching/hunt1000plus-v46-closure/snes/CPUEXEC.o", 0x00115DB0, 0x0990),
    SliceSpec("cpuops", "build/matching/hunt1000plus-v46-closure/snes/CPUOPS.o", 0x00116740, 0x133B4),
    SliceSpec("dma", "build/matching/hunt1041-v72-v53-promotion/objects/dma.o", 0x00129AF4, 0x2468),
    SliceSpec("fxemu", "build/matching/hunt1000plus-v46-closure/snes/fxemu.o", 0x0012FE5C, 0x0D80),
    SliceSpec("fxinst", "build/matching/hunt1000plus-v46-closure/snes/fxinst.o", 0x00130BDC, 0x11E9C),
    SliceSpec("gfx", "build/matching/hunt1041-v52-closure/objects/gfx-short.o", 0x00142A78, 0x0DC14),
    SliceSpec("loadzip", "build/matching/hunt1041-v72-v53-promotion/objects/loadzip.o", 0x0015068C, 0x0490),
    SliceSpec("obc1", "build/matching/hunt1041-v72-v53-promotion/objects/obc1.o", 0x00158B5C, 0x04FC),
    SliceSpec("ppu", "build/matching/hunt1041-v52-closure/objects/ppu-short.o", 0x00159058, 0x4894),
    SliceSpec("sa1", "build/window11-rodata/source-objects/snes-sa1.o", 0x0015D8EC, 0x1870),
    SliceSpec("sa1cpu", "build/matching/hunt1000plus-v46-closure/snes/SA1CPU.o", 0x0015F1C8, 0x107E8),
    SliceSpec("seta", "build/matching/hunt1000plus-v46-closure/snes/seta.o", 0x0016FC48, 0x0048),
    SliceSpec("seta010_prefix", "build/matching/hunt1000plus-v46-closure/snes/seta010.o", 0x0016FC90, 0x0370),
)

# Exact functions that were proved during the same hunts but are not selected
# by every historical TSV schema (or use a recovered local object).
EXTRA_SYMBOLS = (
    SliceSpec("sdd1_compare", "build/matching/hunt1000plus-v46-closure/snes/SDD1.o", 0x0016FAC4, 0x40, symbol="_Z31S9xCompareSDD1LoggedDataEntriesPKvS0_"),
    SliceSpec("memory_speed", "build/matching/hunt1000plus-v46-closure/snes/MEMMAP.o", 0x001568E8, 0x2C, symbol="_ZN7CMemory5SpeedEv"),
    SliceSpec("memory_romid", "build/matching/hunt1000plus-v46-closure/snes/MEMMAP.o", 0x00156C54, 0x0C, symbol="_ZN7CMemory5ROMIDEv"),
    SliceSpec("dsp_op2b", "build/matching/hunt1000plus-v46-closure/snes/DSP1.o", 0x0012DF00, 0x50, symbol="_Z7DSPOp2Bv"),
    SliceSpec("dsp_set_byte", "build/matching/hunt1000plus-v46-closure/snes/DSP1.o", 0x0012E750, 0xFF4, symbol="_Z11DSP1SetByteht"),
    SliceSpec("dsp_get_byte", "build/matching/hunt1000plus-v46-closure/snes/DSP1.o", 0x0012F744, 0x164, symbol="_Z11DSP1GetBytet"),
    SliceSpec("rom_cleanup", "build/source-tree/objects/snes9x/memory_cleanup_recovered.o", 0x00151330, 0x30, symbol="rom_cleanup_00151330"),
)

RESIDUAL_SYMBOLS = (
    ("stage3p_delete_cheat", 0x001141CC, 148),
    ("stage3p_delete_cheats", 0x00114260, 36),
    ("stage3p_enable_cheat", 0x00114284, 76),
    ("stage3p_disable_cheat", 0x001142D0, 88),
    ("stage3p_save_cheat_file", 0x00114674, 420),
    ("stage3p_compute_clip_windows", 0x00114818, 4572),
    ("stage3p_dsp2_set_byte", 0x0012F8A8, 732),
    ("stage3p_dsp2_get_byte", 0x0012FB84, 100),
    ("stage3p_fx_pipe_string", 0x0012FBE8, 628),
    ("stage3p_memory_safe", 0x00150F54, 288),
)

EXPECTED: dict[str, object] = {
    "chunk_count": 51,
    "code_windows": 6,
    "differences_removed": 356_981,
    "differing_bytes": 263_765,
    "exact_chunks": 45,
    "mismatching_chunks": 6,
    "first_differing_address": 0x00100114,
    "historical_recovered_bytes": 381_272,
    "integrated_padded_sha256": "2e85d2d3db52ea3657120ab21a9b6f88cac16f77ef975ad2a40ba959d3db6b44",
    "prior_differing_bytes": 620_746,
    "prior_exact_assembly_bytes": 4_856,
    "relocation_fields": 31_010,
    "residual_bytes": 7_088,
    "selected_source_contract_sha256": "d32efd597da34e9012f45e0d61ab6a34036cf7611616e4492a679c299b4dc317",
    "selected_source_slices": 127,
    "source_bytes": 393_216,
    "target_initialized_size": 3_304_936,
    "target_sha256": TARGET_SHA256,
}
_ELF_CACHE: dict[Path, ELFFile] = {}


class CodeWindowsError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise CodeWindowsError(message)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def run(command: Sequence[str | Path]) -> str:
    process = subprocess.run(
        [str(item) for item in command], cwd=ROOT, text=True,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
    )
    if process.returncode:
        detail = process.stderr.strip() or process.stdout.strip()
        fail(f"command failed ({' '.join(map(str, command))}): {detail}")
    return process.stdout.strip()


def claims() -> dict[str, bool]:
    return {
        "public_proof_sources_rebuilt": True,
        "non_relocation_bits_raw_exact": True,
        "private_oracle_limited_to_relocation_results_and_verification": True,
        "private_target_bytes_stored": False,
        "windows_1_through_6_exact": True,
        "replacement_elf": False,
        "unpacked_hash_matched": False,
        "packed_hash_matched": False,
    }


def source_contract() -> dict:
    return {
        "evidence_manifests": list(EVIDENCE_MANIFESTS),
        "evidence_targets": list(EVIDENCE_TARGETS),
        "fixed_slices": [spec.__dict__ for spec in FIXED_SLICES],
        "extra_symbols": [spec.__dict__ for spec in EXTRA_SYMBOLS],
        "residual_source": str(DEFAULT_RESIDUAL.relative_to(ROOT)),
        "residual_symbols": [
            {"symbol": name, "address": address, "size": size}
            for name, address, size in RESIDUAL_SYMBOLS
        ],
    }


def frozen_document(result: dict) -> dict:
    return {
        "format": FORMAT,
        "schema_version": SCHEMA,
        "prior_manifest_sha256": digest(stage3o.DEFAULT_MANIFEST.read_bytes()),
        "source_contract": source_contract(),
        "result": result,
        "claims": claims(),
    }


def prepare_evidence(args: argparse.Namespace) -> None:
    """Rebuild candidate objects while preserving committed/user ledger bytes."""
    paths = [ROOT / relative for relative in EVIDENCE_MANIFESTS]
    snapshots = {path: path.read_bytes() for path in paths}
    cxx = resolve_tool(args.compiler)
    cc = resolve_tool(args.c_compiler) if args.c_compiler else cxx.with_name("ee-gcc")
    try:
        process = subprocess.run(
            ["make", *EVIDENCE_TARGETS, f"EE_CC={cc}", f"EE_STAGE1_CXX={cxx}"],
            cwd=ROOT,
        )
        if process.returncode:
            fail(f"historical evidence rebuild failed with status {process.returncode}")
    finally:
        for path, data in snapshots.items():
            path.write_bytes(data)
    print("rebuilt Stage-3P historical candidate objects; evidence ledgers preserved")


def validate(args: argparse.Namespace) -> dict:
    document = json.loads(args.manifest.read_text(encoding="utf-8"))
    if document.get("format") != FORMAT or document.get("schema_version") != SCHEMA:
        fail("code-window identity drift")
    if document.get("prior_manifest_sha256") != digest(stage3o.DEFAULT_MANIFEST.read_bytes()):
        fail("prior checkpoint drift")
    if document.get("source_contract") != source_contract():
        fail("public source contract drift")
    if document.get("claims") != claims():
        fail("claim boundary drift")
    if EXPECTED:
        for key, value in EXPECTED.items():
            if document.get("result", {}).get(key) != value:
                fail(f"frozen metric drift: {key}")
    result = document.get("result", {})
    if result.get("exact_chunk_indices") != [*range(1, 7), *range(12, 51)]:
        fail("exact-window roster drift")
    if result.get("source_bytes") != 6 * WINDOW_SIZE:
        fail("code-window byte coverage drift")
    selected = result.get("selected_sources", [])
    if len(selected) != result.get("selected_source_slices"):
        fail("selected source roster length drift")
    if digest(json.dumps(selected, sort_keys=True).encode()) != result.get("selected_source_contract_sha256"):
        fail("selected source roster hash drift")
    return document


def section_symbol(elf: ELFFile, section_name: str, offset: int, size: int, name: str) -> Symbol:
    matches = [item for item in elf.sections if item.name == section_name]
    if len(matches) != 1:
        fail(f"{elf.path}: expected one {section_name} section")
    item = matches[0]
    if offset < 0 or offset + size > item.size:
        fail(f"{elf.path}: slice {name} exceeds {section_name}")
    return Symbol(name, item.address + offset, size, item.index, 2)


def candidate(spec: SliceSpec) -> tuple[bytes, bytes, int]:
    path = ROOT / spec.object
    if not path.is_file():
        fail(f"missing proved candidate object: {spec.object}")
    elf = _ELF_CACHE.get(path)
    if elf is None:
        elf = ELFFile(path)
        _ELF_CACHE[path] = elf
    if spec.symbol:
        found = elf.find_symbol(spec.symbol)
        symbol = Symbol(found.name, found.value + spec.source_offset, spec.size, found.section_index, found.info)
    else:
        symbol = section_symbol(elf, spec.section, spec.source_offset, spec.size, spec.name)
    raw = elf.symbol_bytes(symbol, spec.size)
    mask = bytearray(spec.size)
    relocs = elf.relocation_masks(symbol, 4)
    for reloc in relocs:
        if not reloc.known:
            fail(f"unknown MIPS relocation in {spec.name}: {reloc.relocation_type}")
        for index, bits in enumerate(reloc.mask_bytes):
            mask[reloc.start + index] |= bits
    return raw, bytes(mask), len(relocs)


def tsv_specs() -> list[SliceSpec]:
    rows: list[tuple[int, int, int, str, str]] = []
    object_pool: set[str] = {
        spec.object for spec in (*FIXED_SLICES, *EXTRA_SYMBOLS)
    }
    for path in sorted((ROOT / "analysis/matching").glob("*.tsv")):
        with path.open(encoding="utf-8", newline="") as stream:
            for row in csv.DictReader(stream, delimiter="\t"):
                result = row.get("result", "").strip().lower()
                normalized = row.get("normalized_equal", "").strip().lower()
                accepted_without_result = (
                    not result and ("validated" in path.name or path.name.startswith("progress54-"))
                )
                if (result not in {"match", "matching", "pass", "exact"}
                        and normalized != "true" and not accepted_without_result):
                    continue
                symbol = row.get("object_symbol", "")
                try:
                    address = int(row["address"], 0)
                    if row.get("end_address"):
                        size = int(row["end_address"], 0) - address
                    else:
                        size = int(row.get("object_size") or row.get("object_symbol_size") or "0", 0)
                    source_offset = int(row.get("object_offset") or "0", 0)
                except (KeyError, ValueError):
                    continue
                if symbol and size > 0 and WINDOW_START <= address and address + size <= WINDOW_END:
                    rows.append((address, size, source_offset, symbol, row.get("object", "")))
                obj = row.get("object", "")
                if obj and (ROOT / obj).is_file():
                    object_pool.add(obj)

    specs: list[SliceSpec] = []
    seen: set[tuple[str, int, int, int]] = set()
    for address, size, source_offset, symbol, preferred in rows:
        pool = ([preferred] if preferred and preferred in object_pool else []) + sorted(object_pool)
        for obj in pool:
            key = (obj, address, size, source_offset)
            if key in seen:
                continue
            seen.add(key)
            specs.append(SliceSpec(f"tsv_{address:08x}_{symbol}", obj, address, size, source_offset=source_offset, symbol=symbol))
    return sorted(specs, key=lambda item: (item.address, -item.size, item.object, item.symbol))


def add_slice(buffers: list[bytearray], covered: list[bytearray], reference: bytes,
              spec: SliceSpec) -> tuple[int, int, str]:
    raw, masks, relocations = candidate(spec)
    target_offset = spec.address - TARGET_BASE
    target = reference[target_offset:target_offset + spec.size]
    if len(target) != spec.size:
        fail(f"target slice outside image: {spec.name}")
    for index, (source_byte, target_byte, mask) in enumerate(zip(raw, target, masks)):
        if ((source_byte ^ target_byte) & (~mask & 0xFF)) != 0:
            fail(f"non-relocation mismatch in {spec.name} +0x{index:x}")
    patched = bytes((source & (~mask & 0xFF)) | (target_byte & mask)
                    for source, target_byte, mask in zip(raw, target, masks))
    if patched != target:
        fail(f"relocation patch failed: {spec.name}")
    newly = 0
    for index, value in enumerate(patched):
        absolute = spec.address + index
        if not (WINDOW_START <= absolute < WINDOW_END):
            fail(f"source slice crosses code-window boundary: {spec.name}")
        window = (absolute - WINDOW_START) // WINDOW_SIZE
        offset = (absolute - WINDOW_START) % WINDOW_SIZE
        if covered[window][offset] and buffers[window][offset] != value:
            fail(f"conflicting exact source overlap: {spec.name}")
        if not covered[window][offset]:
            newly += 1
        buffers[window][offset] = value
        covered[window][offset] = 1
    return newly, relocations, digest(raw)


def assemble_windows(reference: bytes, residual_object: Path) -> tuple[list[bytes], dict]:
    buffers = [bytearray(WINDOW_SIZE) for _ in range(6)]
    covered = [bytearray(WINDOW_SIZE) for _ in range(6)]
    selected: list[dict] = []
    relocation_fields = 0

    specs = [*FIXED_SLICES, *tsv_specs(), *EXTRA_SYMBOLS]
    specs.extend(
        SliceSpec(name, str(residual_object.relative_to(ROOT)), address, size, symbol=name)
        for name, address, size in RESIDUAL_SYMBOLS
    )
    for spec in specs:
        try:
            newly, relocations, raw_hash = add_slice(buffers, covered, reference, spec)
        except (CodeWindowsError, ValueError) as exc:
            # A TSV may point at an older duplicate object while another proved
            # object supplies the same range. Fixed and explicit specs fail closed.
            if spec in FIXED_SLICES or spec in EXTRA_SYMBOLS or spec.symbol.startswith("stage3p_"):
                raise
            continue
        relocation_fields += relocations
        if newly:
            selected.append({
                "name": spec.name,
                "object": (
                    str(DEFAULT_RESIDUAL.relative_to(ROOT))
                    if spec.symbol.startswith("stage3p_") else spec.object
                ),
                "section": spec.section,
                "source_offset": spec.source_offset,
                "symbol": spec.symbol,
                "address": spec.address,
                "size": spec.size,
                "new_bytes": newly,
                "relocations": relocations,
                "raw_sha256": raw_hash,
            })

    gaps = []
    for window, bitmap in enumerate(covered, start=1):
        start = None
        for index, bit in enumerate([*bitmap, 1]):
            if not bit and start is None:
                start = index
            elif bit and start is not None:
                gaps.append((WINDOW_START + (window - 1) * WINDOW_SIZE + start,
                             WINDOW_START + (window - 1) * WINDOW_SIZE + index))
                start = None
    if gaps:
        shown = ", ".join(f"0x{start:08x}-0x{end:08x}" for start, end in gaps[:12])
        fail(f"uncovered code-window source bytes: {shown}")
    prior_assembly_bytes = sum(
        row["new_bytes"] for row in selected
        if "v80" in row["object"] or "v81" in row["object"]
    )
    residual_bytes = sum(size for _name, _address, size in RESIDUAL_SYMBOLS)
    historical_recovered_bytes = sum(map(len, buffers)) - prior_assembly_bytes - residual_bytes
    return [bytes(item) for item in buffers], {
        "selected_source_slices": len(selected),
        "relocation_fields": relocation_fields,
        "source_bytes": sum(map(len, buffers)),
        "historical_recovered_bytes": historical_recovered_bytes,
        "prior_exact_assembly_bytes": prior_assembly_bytes,
        "residual_bytes": residual_bytes,
        "selected_source_contract_sha256": digest(json.dumps(selected, sort_keys=True).encode()),
        "selected_sources": selected,
    }


def render_payload_source(paths: list[tuple[str, Path]]) -> str:
    lines = ["/* Generated Stage-3P payload; ignored build artifact. */", "    .set noreorder"]
    for section, path in paths:
        lines.extend([
            f'    .section {section},"ax",@progbits',
            "    .balign 4",
            f'    .incbin "{path}"',
        ])
    return "\n".join(lines) + "\n"


def update_linker_script(text: str) -> str:
    old = """  .text 0x00100114 : { *(.text) }
  .rodata : { *(.rodata) }
  .data : { *(.data) }
  .bss (NOLOAD) : { *(.bss) *(.bss.stage3.compatibility) }
"""
    new = """  .text.stage3p.symbols 0x00100114 (NOLOAD) : { *(.text) }
  .rodata.stage3p.symbols 0x0016d5d0 (NOLOAD) : { *(.rodata) }
  .text.stage3p.prefix 0x00100114 : { KEEP(*(.text.stage3p.prefix)) }
  .text.stage3p.window1 0x00110000 : { KEEP(*(.text.stage3p.window1)) }
  .text.stage3p.window2 0x00120000 : { KEEP(*(.text.stage3p.window2)) }
  .text.stage3p.window3 0x00130000 : { KEEP(*(.text.stage3p.window3)) }
  .text.stage3p.window4 0x00140000 : { KEEP(*(.text.stage3p.window4)) }
  .text.stage3p.window5 0x00150000 : { KEEP(*(.text.stage3p.window5)) }
  .text.stage3p.window6 0x00160000 : { KEEP(*(.text.stage3p.window6)) }
  .text.stage3p.tail 0x00170000 : { KEEP(*(.text.stage3p.tail)) }
  .data 0x00170148 : { *(.data) }
  .bss 0x001702c0 (NOLOAD) : { *(.bss) *(.bss.stage3.compatibility) }
"""
    if text.count(old) != 1:
        fail("Stage-3O linker-script core drift")
    return text.replace(old, new)


def probe(args: argparse.Namespace) -> dict:
    prior = stage3o.validate(stage3o.parse_args(["validate"]))
    reference = args.reference.read_bytes()
    if len(reference) != prior["result"]["target_initialized_size"] or digest(reference) != TARGET_SHA256:
        fail("private unpacked reference identity drift")
    cxx = resolve_tool(args.compiler)
    if run([cxx, "-dumpversion"]) != "3.2.2" or run([cxx, "-dumpmachine"]) != "ee":
        fail("Stage-3P requires EE GCC 3.2.2")
    linker = resolve_tool(args.ld) if args.ld else cxx.with_name("ee-ld")
    objcopy = resolve_tool(args.objcopy) if args.objcopy else cxx.with_name("ee-objcopy")
    args.build_dir.mkdir(parents=True, exist_ok=True)

    residual_object = args.build_dir / "stage3p-residual.o"
    stage3o.stage3i.compile_one(
        cxx, ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"),
        args.residual, residual_object,
    )
    windows, metrics = assemble_windows(reference, residual_object)

    prior_padded = args.stage3o_build / "stage3o-window11-rodata-integrated.padded.bin"
    base_script = args.stage3o_build / "window11-rodata.ld"
    if not prior_padded.is_file() or not base_script.is_file():
        fail("missing Stage-3O dependency; run make window11-rodata")
    prior_bytes = prior_padded.read_bytes()
    payload_parts = [
        (".text.stage3p.prefix", prior_bytes[0x114:0x10000]),
        *[(f".text.stage3p.window{index}", data) for index, data in enumerate(windows, start=1)],
        (".text.stage3p.tail", prior_bytes[0x70000:0x70148]),
    ]
    source_paths = []
    for section_name, data in payload_parts:
        path = args.build_dir / f"{section_name.rsplit('.', 1)[-1]}.bin"
        path.write_bytes(data)
        source_paths.append((section_name, path))
    payload_source = args.build_dir / "code-windows.S"
    payload_source.write_text(render_payload_source(source_paths), encoding="utf-8")
    payload_object = args.build_dir / "code-windows.o"
    stage3o.stage3i.compile_one(
        cxx, ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"),
        payload_source, payload_object,
    )

    linker_script = args.build_dir / "code-windows.ld"
    linker_script.write_text(update_linker_script(base_script.read_text(encoding="utf-8")), encoding="utf-8")
    prior_inputs = [
        args.startup_object, args.input,
        args.stage3i_build / "frontend-eh-frames.o",
        args.stage3i_build / "providers.o",
        args.stage3i_build / "semantic-cfi.o",
        args.stage3j_build / "runtime-tail.o",
        args.stage3k_build / "tail-payloads.o",
        args.stage3k_build / "tail-semantics.o",
        args.stage3l_build / "window36-payloads.o",
        args.stage3l_build / "window36-semantics.o",
        args.stage3m_build / "media-assets.o",
        args.stage3n_build / "window35-payloads.o",
        args.stage3n_build / "window35-semantics.o",
        args.stage3o_build / "window11-rodata.o",
    ]
    missing = [str(path) for path in prior_inputs if not path.is_file()]
    if missing:
        fail("missing prior link inputs: " + ", ".join(missing))
    output = args.build_dir / "stage3p-code-windows-integrated.elf"
    run([linker, "-EL", "-T", linker_script, "-o", output, *prior_inputs, payload_object])

    elf = ELFFile(output)
    startup.verify_symbols(elf)
    raw_path = args.build_dir / "stage3p-code-windows-integrated.unpadded.bin"
    padded_path = args.build_dir / "stage3p-code-windows-integrated.padded.bin"
    run([objcopy, "-O", "binary", output, raw_path])
    raw = raw_path.read_bytes()
    if len(raw) > len(reference):
        fail("integrated diagnostic exceeds target size")
    padded = raw + bytes(len(reference) - len(raw))
    padded_path.write_bytes(padded)
    for index, expected in enumerate(windows, start=1):
        if padded[index * WINDOW_SIZE:(index + 1) * WINDOW_SIZE] != expected:
            fail(f"linked code window {index} differs from source reconstruction")

    _sections, layout = startup.load_inputs(startup.parse_args(["validate", "--layout", str(args.layout)]))
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [index for index, (actual, target) in enumerate(zip(padded, reference)) if actual != target]
    return {
        **metrics,
        "code_windows": 6,
        "exact_chunks": len(exact),
        "mismatching_chunks": len(different),
        "exact_chunk_indices": exact,
        "mismatching_chunk_indices": different,
        "chunk_count": len(exact) + len(different),
        "differing_bytes": len(differences),
        "prior_differing_bytes": prior["result"]["differing_bytes"],
        "differences_removed": prior["result"]["differing_bytes"] - len(differences),
        "first_differing_address": TARGET_BASE + differences[0],
        "target_initialized_size": len(reference),
        "integrated_padded_sha256": digest(padded),
        "target_sha256": digest(reference),
    }


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "prepare", "probe", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    parser.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--startup-object", type=Path, default=DEFAULT_STARTUP)
    parser.add_argument("--stage3i-build", type=Path, default=DEFAULT_STAGE3I)
    parser.add_argument("--stage3j-build", type=Path, default=DEFAULT_STAGE3J)
    parser.add_argument("--stage3k-build", type=Path, default=DEFAULT_STAGE3K)
    parser.add_argument("--stage3l-build", type=Path, default=DEFAULT_STAGE3L)
    parser.add_argument("--stage3m-build", type=Path, default=DEFAULT_STAGE3M)
    parser.add_argument("--stage3n-build", type=Path, default=DEFAULT_STAGE3N)
    parser.add_argument("--stage3o-build", type=Path, default=DEFAULT_STAGE3O)
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--residual", type=Path, default=DEFAULT_RESIDUAL)
    parser.add_argument("--compiler", default="ee-g++")
    parser.add_argument("--c-compiler")
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
        if args.command == "prepare":
            prepare_evidence(args)
            return 0
        if args.command == "validate":
            document = validate(args)
        else:
            result = probe(args)
            document = frozen_document(result)
            if args.command == "capture":
                args.manifest.parent.mkdir(parents=True, exist_ok=True)
                args.manifest.write_text(json.dumps(document, indent=2, sort_keys=True) + "\n", encoding="utf-8")
            elif validate(args) != document:
                fail("private result differs from frozen manifest")
        result = document["result"]
        print(
            f"verified Stage-3P code windows: exact={result['exact_chunks']}/51; "
            f"source={result['source_bytes']} bytes; residual={result['residual_bytes']} bytes; "
            f"differing_bytes={result['differing_bytes']}; replacement ELF: not yet"
        )
        return 0
    except (CodeWindowsError, stage3o.Window11RodataError, OSError, ValueError, KeyError, RuntimeError) as exc:
        print(f"Stage-3P code windows: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
