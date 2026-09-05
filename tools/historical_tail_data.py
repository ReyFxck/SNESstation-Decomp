#!/usr/bin/env python3
"""Rebuild and integrate exact historical C++ tail data for Stage 3I.

The target's late writable-data corridor contains ordinary Snes9x objects and
GCC 3.2.2 C++ unwind records.  This gate rebuilds six public-source data
sections, permits the private reference to supply *only* their R_MIPS_32
relocation results, and assembles the remaining CFI from explicit DWARF
semantics.  No target payload is written to the repository and this remains a
diagnostic image, not a replacement-ELF claim.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import shlex
import shutil
import struct
import subprocess
import sys
from pathlib import Path
from typing import Sequence

import data_backing
import frontend_eh_frames as frontend
import historical_data
import link_layout_probe
import startup_integration as startup
from compare_elf_functions import ELFFile
from source_aliases import resolve_tool, sibling_tool

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools/history/research"))
import hunt1000plus_v46_closure as v46  # noqa: E402
import hunt1000plus_v47_closure as v47  # noqa: E402
import hunt1041_v52_closure as v52  # noqa: E402

DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_STARTUP_OBJECT = ROOT / "build/startup-integration/crt0-stage3g.o"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_FRONTEND_MANIFEST = ROOT / "analysis/link_identity/frontend_eh_frames.json"
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/historical_tail_data.json"
DEFAULT_BUILD = ROOT / "build/historical-tail-data"

FORMAT = "snesstation-stage3i-historical-tail-data"
SCHEMA = 1
TARGET_BASE = 0x00100000
PERSONALITY = 0x001A9728
SOURCE_ARCHIVE_SHA256 = v46.SNES_ARCHIVE.sha256
TIME32_PATCH = ("\ttime_t last_used;", "\tint last_used;")

# The complete source sections are fingerprinted before any relocation is
# applied.  Slice boundaries exclude only records reconstructed below.
PROVIDERS = (
    {
        "name": "ppu",
        "profile": "v52-time32",
        "filename": "ppu.cpp",
        "target_address": 0x003F4BF0,
        "source_offset": 0,
        "size": 0x288,
        "full_size": 0x288,
        "full_sha256": "d6c09da603a6d3c27bf8b92f70f11477a7a84df63fee051acda89ca0df645fe2",
        "slice_sha256": "d6c09da603a6d3c27bf8b92f70f11477a7a84df63fee051acda89ca0df645fe2",
        "linked_sha256": "fcb0379aaab43e7e0331dbe287881167031ac64b9dd4e4fec6ab5678619518f6",
        "relocations": 14,
    },
    {
        "name": "sa1cpu",
        "profile": "v46-official",
        "filename": "SA1CPU.CPP",
        "target_address": 0x003F5040,
        "source_offset": 0,
        "size": 0x5570,
        "full_size": 0x5570,
        "full_sha256": "be8548846f24b7a7bc2ab6d16eaa0ed930b13f1a3ecc05640efd9d440b23c9e6",
        "slice_sha256": "be8548846f24b7a7bc2ab6d16eaa0ed930b13f1a3ecc05640efd9d440b23c9e6",
        "linked_sha256": "bb4e64679d46f3cc1fa54fc08ccaee1dc1102e8a910a039116a9583dd52d7a64",
        "relocations": 1411,
    },
    {
        "name": "snaporig",
        "profile": "v52-time32",
        "filename": "snaporig.cpp",
        "target_address": 0x003FA6C0,
        "source_offset": 0,
        "size": 0x11988,
        "full_size": 0x11988,
        "full_sha256": "eddf16c3e8c7579f8dd42dc77ea7408ae984051fe57268a2a0bb7072c866f373",
        "slice_sha256": "eddf16c3e8c7579f8dd42dc77ea7408ae984051fe57268a2a0bb7072c866f373",
        "linked_sha256": "b5917c0c2f23956886f36365de8edebe2aa540a3918c157ce9e483d6cfbbbdf3",
        "relocations": 4,
    },
    {
        "name": "snapshot_prefix",
        "profile": "v52-time32",
        "filename": "SNAPSHOT.CPP",
        "target_address": 0x0040C048,
        "source_offset": 0,
        "size": 0x4DE4,
        "full_size": 0x4E68,
        "full_sha256": "bb45476f1d49865948850230d8e6ff1829e5e52a44c13d11ef582124d60cb072",
        "slice_sha256": "6f10b6787d0abc3989d459cb7cff9ddc1cb1967402f947bcf411336e15d61989",
        "linked_sha256": "0a341a059a856337a6e63a6a8fcad31f750c5a36629c1def719b3b61586a64a2",
        "relocations": 11,
    },
    {
        "name": "sound_prefix",
        "profile": "v52-time32",
        "filename": "SOUNDUX.CPP",
        "target_address": 0x00410F00,
        "source_offset": 0,
        "size": 0xC8,
        "full_size": 0x108,
        "full_sha256": "a720400359963e03539f48cdf01d6001a25bca29735d6452303b630ef834f8c7",
        "slice_sha256": "e2929a351838ddde7e139358a8c579c36f545e94be70faa51ce22db703b555bc",
        "linked_sha256": "20d429db408fbc9ff591d40c92c0b807a0da5324c1340572414c67b8b5867ad2",
        "relocations": 4,
    },
    {
        "name": "spc700_suffix",
        "profile": "v46-official",
        "filename": "SPC700.CPP",
        "target_address": 0x00411410,
        "source_offset": 0x400,
        "size": 0x20D8,
        "full_size": 0x24D8,
        "full_sha256": "951690e4a850a3a7835410d08a25f27da07f12fc2c7148b7d5694f88f4392e2b",
        "slice_sha256": "a38c99a63a0ef48c5ded7e204aadf2c9a95589a48288df06171d5f4ee2f1a422",
        "linked_sha256": "7dc3c58ee92c41b8ab05e2cda8baf981a49283125651ac204d67adb5fb687e8f",
        "relocations": 179,
    },
)

# Compact semantic notation for DWARF call-frame instructions.
def A(count: int) -> tuple[str, int]:
    return ("advance_loc4", count)


def C(size: int) -> tuple[str, int]:
    return ("def_cfa_offset", size)


def S(register: int, factor: int) -> tuple[str, int, int]:
    return ("offset_extended_sf", register, factor)


def F(pc: int, size: int, *ops: tuple) -> dict:
    return {"pc": pc, "size": size, "ops": ops}


SEMANTIC_GROUPS = (
    {
        "name": "memmap_pre",
        "section": ".data.stage3i.eh.va_003f48a8",
        "address": 0x003F48A8,
        "size": 0x70,
        "cie": True,
        "zero_words": 3,
        "fdes": (
            F(0x0015068C, 0x490, A(8), C(592), A(8), S(23, -12), A(8),
              S(22, -16), A(32), S(17, -36), S(18, -32), S(20, -24),
              S(21, -20), S(30, -8), S(64, -4), S(19, -28), A(8),
              S(16, -40)),
        ),
    },
    {
        "name": "memmap",
        "section": ".data.stage3i.eh.va_003f4964",
        "address": 0x003F4964,
        "size": 0x284,
        "cie": True,
        "zero_words": 2,
        "fdes": (
            F(0x00151074, 0x2BC, A(4), C(80), A(20), S(17, -16), S(18, -12), S(19, -8), S(64, -4), S(16, -20)),
            F(0x00151330, 0x30, A(4), C(32), A(8), S(64, -4), S(16, -8)),
            F(0x001513BC, 0xCFC, A(4), C(4608), A(8), S(30, -8), A(8), S(23, -12), A(8), S(22, -16), A(32), S(16, -40), S(17, -36), S(18, -32), S(20, -24), S(21, -20), S(64, -4), S(19, -28)),
            F(0x001520B8, 0x220, A(4), C(368), A(40), S(16, -28), S(17, -24), S(18, -20), S(19, -16), S(20, -12), S(21, -8), S(64, -4)),
            F(0x001522D8, 0x107C, A(4), C(192), A(12), S(18, -28), S(21, -16), A(24), S(23, -8), S(64, -4), S(16, -36), A(24), S(17, -32), S(19, -24), S(20, -20), S(22, -12)),
            F(0x00153354, 0x164, A(4), C(80), A(24), S(17, -16), S(18, -12), S(19, -8), S(64, -4), S(16, -20)),
            F(0x001534B8, 0x108, A(4), C(64), A(20), S(17, -12), S(18, -8), S(64, -4), S(16, -16)),
            F(0x00156514, 0x370, A(4), C(32), A(16), S(16, -8), S(64, -4)),
            F(0x00156C60, 0x1870, A(4), C(80), A(12), S(16, -20), S(18, -12), A(24), S(17, -16), S(19, -8), S(64, -4)),
            F(0x001584D0, 0x4A4, A(4), C(4304), A(16), S(17, -28), S(18, -24), A(24), S(20, -16), S(21, -12), S(22, -8), A(8), S(19, -20), A(36), S(16, -32), S(64, -4)),
            F(0x00158974, 0xE4, A(8), C(16), A(12), S(64, -4)),
            F(0x00158A58, 0xE4, A(4), C(80), A(12), S(19, -8), S(16, -20), S(64, -4), A(8), S(18, -12), A(8), S(17, -16)),
        ),
    },
    {
        "name": "gfx",
        "section": ".data.stage3i.eh.va_003f4e78",
        "address": 0x003F4E78,
        "size": 0x1C8,
        "cie": True,
        "zero_words": 2,
        "fdes": (
            F(0x0015D9AC, 0x108, A(4), C(96), A(8), S(20, -8), S(64, -4), A(8), S(19, -12), A(8), S(18, -16), A(16), S(16, -24), S(17, -20)),
            F(0x0015DB74, 0x110, A(4), C(64), A(8), S(18, -8), S(64, -4), A(8), S(17, -12), A(8), S(16, -16)),
            F(0x0015DC84, 0x190, A(4), C(16), A(12), S(64, -4)),
            F(0x0015DE14, 0x54, A(4), C(48), A(4), S(17, -8), A(8), S(64, -4), A(12), S(16, -12)),
            F(0x0015DE68, 0x1CC, A(4), C(16), A(12), S(64, -4)),
            F(0x0015E034, 0x48, A(4), C(48), A(4), S(17, -8), A(8), S(16, -12), A(12), S(64, -4)),
            F(0x0015E298, 0x174, A(8), C(32), A(12), S(64, -4), S(16, -8)),
            F(0x0015E40C, 0x8E0, A(4), C(80), A(4), S(19, -8), A(16), S(17, -16), S(18, -12), A(20), S(16, -20), S(64, -4)),
            F(0x0015F030, 0x12C, A(4), C(112), A(32), S(18, -20), S(19, -16), S(20, -12), S(21, -8), A(16), S(16, -28), S(64, -4), S(17, -24)),
        ),
    },
    {
        "name": "sdd1",
        "section": ".data.stage3i.eh.va_003fa5b0",
        "address": 0x003FA5B0,
        "size": 0x78,
        "cie": True,
        "zero_words": 1,
        "fdes": (
            F(0x0016FB04, 0xB0, A(4), C(64), A(8), S(18, -8), A(24), S(16, -16), S(64, -4), S(17, -12)),
            F(0x0016FBB4, 0x94, A(8), C(48), A(12), S(17, -8), S(64, -4), A(8), S(16, -12)),
        ),
    },
    {
        "name": "srtc",
        "section": ".data.stage3i.eh.va_003fa630",
        "address": 0x003FA630,
        "size": 0x58,
        "cie": True,
        "zero_words": 0,
        "fdes": (
            F(0x0016FC48, 0x24, A(4), C(16), A(8), S(64, -4)),
            F(0x0016FC6C, 0x24, A(4), C(16), A(8), S(64, -4)),
        ),
    },
    {
        "name": "snapshot_tail",
        "section": ".data.stage3i.eh.va_00410e2c",
        "address": 0x00410E2C,
        "size": 0x84,
        "cie": False,
        "zero_words": 1,
        "fdes": (
            {**F(0x001726EC, 0x1E8, A(4), C(96), A(4), S(16, -24), A(24), S(18, -16), S(19, -12), S(20, -8), S(64, -4), A(8), S(17, -20)), "cie_pointer": 0x250},
            {**F(0x001728D4, 0x1350, A(4), C(4352), A(40), S(17, -36), S(18, -32), S(20, -24), S(21, -20), S(22, -16), S(23, -12), S(30, -8), S(64, -4), S(19, -28), A(8), S(16, -40)), "cie_pointer": 0x28C},
        ),
    },
    {
        "name": "snapshot_gap",
        "section": ".data.stage3i.eh.va_00410eb0",
        "address": 0x00410EB0,
        "size": 0x50,
        "cie": True,
        "zero_words": 2,
        "fdes": (
            F(0x00173C90, 0x16C, A(4), C(4288), A(4), S(16, -16), A(32), S(17, -12), S(18, -8), S(64, -4)),
        ),
    },
    {
        "name": "sound_tail",
        "section": ".data.stage3i.eh.va_00410fc8",
        "address": 0x00410FC8,
        "size": 0x48,
        "cie": False,
        "zero_words": 1,
        "fdes": (
            {**F(0x00177E6C, 0x290, A(8), C(112), A(8), S(17, -24), A(28), S(18, -20), S(20, -12), S(21, -8), A(16), S(64, -4), S(16, -28), A(8), S(19, -16)), "cie_pointer": 0xA4},
        ),
    },
    {
        "name": "cache_megs",
        "section": ".data.stage3i.source.va_004134e8",
        "address": 0x004134E8,
        "size": 4,
        "literal": (5,),
        "source_identity": "Snes9x 1.41-1 spc7110.cpp: uint16 cacheMegs=5",
    },
)

# This compiler-emitted FDE describes a local SNAPSHOT.CPP helper that has no
# public JAL-target row.  Its boundary is kept explicit instead of pretending
# it belongs to the 1,041-entry function denominator.
INTERNAL_FDES = {(0x00173C90, 0x16C)}

ABSORBED_FIXED = {
    ".data.stage3f.va_003f4bf0",
    ".data.stage3ce.va_003f4bf8",
    ".data.stage3f.history.va_003f4c00",
    ".data.stage3f.history.va_003fa6c0",
    ".data.stage3f.history.va_003fb2f8",
    ".data.stage3f.history.va_003fb3b8",
    ".data.stage3f.history.va_003fb3c8",
    ".data.stage3f.history.va_003fb428",
    ".data.stage3f.history.va_003fb508",
    ".data.stage3f.history.va_0040bb78",
}

EXACT_CHUNKS = [12, 13, 14, *range(37, 50)]
EXPECTED = {
    "target_entry_address": 0x00100008,
    "integrated_entry_address": 0x00100008,
    "source_providers": 6,
    "source_bytes": 123140,
    "source_relocations": 1623,
    "semantic_groups": 9,
    "semantic_fdes": 30,
    "semantic_bytes": 1708,
    "semantic_relocations": 36,
    "absorbed_fixed_sections": 10,
    "absorbed_symbol_aliases": 157,
    "target_initialized_size": 3_304_936,
    "integrated_unpadded_size": 3_304_836,
    "terminal_zero_padding": 100,
    "chunk_count": 51,
    "exact_chunks": 16,
    "mismatching_chunks": 35,
    "equal_bytes": 1_445_164,
    "differing_bytes": 1_859_772,
    "first_differing_address": 0x00100114,
    "integrated_unpadded_sha256": "4270e812ae4a32b9085e33a63cd407fd7a6551697313ac5bb1f9180dd7be4f45",
    "integrated_padded_sha256": "f1b99419c6a4c433b6e3564646aa2a210e6ed64d270f6fb4461c0b241546f0d4",
    "target_sha256": "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b",
    "prior_exact_chunks": 13,
    "prior_differing_bytes": 1_883_635,
}


class HistoricalTailError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise HistoricalTailError(message)


def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def run(command: Sequence[str | Path]) -> str:
    result = subprocess.run(
        [str(item) for item in command], cwd=ROOT, text=True,
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT, check=False,
    )
    if result.returncode:
        fail(f"command failed: {shlex.join(map(str, command))}\n{result.stdout[-5000:]}")
    return result.stdout.strip()


def section(elf: ELFFile, name: str):
    found = [item for item in elf.sections if item.name == name]
    if len(found) != 1:
        fail(f"missing/duplicate section: {name}")
    return found[0]


def section_bytes(elf: ELFFile, item) -> bytes:
    return elf.data[item.offset:item.offset + item.size]


def provider_document() -> list[dict]:
    return [dict(row) for row in PROVIDERS]


def semantic_document() -> list[dict]:
    result = []
    for row in SEMANTIC_GROUPS:
        item = {key: value for key, value in row.items() if key != "fdes"}
        if "fdes" in row:
            item["fdes"] = [
                {**fde, "ops": [list(op) for op in fde["ops"]]}
                for fde in row["fdes"]
            ]
        if "literal" in item:
            item["literal"] = list(item["literal"])
        result.append(item)
    return result


def claims() -> dict[str, bool]:
    return {
        "historical_source_sections_rebuilt": True,
        "non_relocation_bytes_raw_exact": True,
        "private_oracle_limited_to_r_mips_32_results": True,
        "tail_unwind_semantics_reconstructed": True,
        "chunks_47_48_49_raw_exact": True,
        "private_target_bytes_stored": False,
        "historical_link_order_complete": False,
        "replacement_elf": False,
        "sjcrunch2_packing_reproduced": False,
        "unpacked_hash_matched": False,
        "packed_hash_matched": False,
    }


def source_contract(args: argparse.Namespace) -> dict:
    return {
        "archive_sha256": SOURCE_ARCHIVE_SHA256,
        "compiler_base_version": "3.2.2",
        "compiler_target": "ee",
        "compile_profile": "historical -Os EE C++ with explicit time32 layout patch",
        "time32_patch": list(TIME32_PATCH),
        "frontend_manifest_sha256": digest(args.frontend_manifest.read_bytes()),
        "historical_data_manifest_sha256": digest((ROOT / "analysis/link_identity/historical_data.json").read_bytes()),
        "progress_targets_sha256": digest((ROOT / "analysis/progress_targets.csv").read_bytes()),
        "private_byte_fallback": False,
    }


def frozen_document(args: argparse.Namespace, result: dict) -> dict:
    return {
        "format": FORMAT,
        "schema_version": SCHEMA,
        "source_contract": source_contract(args),
        "providers": provider_document(),
        "semantic_groups": semantic_document(),
        "result": result,
        "claims": claims(),
    }


def validate_fde_evidence() -> None:
    with (ROOT / "analysis/progress_targets.csv").open(newline="", encoding="utf-8") as handle:
        rows = {int(row["address"], 0): row for row in csv.DictReader(handle)}
    internal_seen: set[tuple[int, int]] = set()
    for group in SEMANTIC_GROUPS:
        for fde in group.get("fdes", ()):
            row = rows.get(fde["pc"])
            if row is None and (fde["pc"], fde["size"]) in INTERNAL_FDES:
                internal_seen.add((fde["pc"], fde["size"]))
                continue
            if row is None or row["status"] != "MATCHING":
                fail(f"semantic FDE lacks frozen function evidence at 0x{fde['pc']:08x}")
    if internal_seen != INTERNAL_FDES:
        fail("internal semantic FDE boundary roster drift")


def validate(args: argparse.Namespace) -> dict:
    frontend.validate(frontend.parse_args([
        "validate", "--manifest", str(args.frontend_manifest),
    ]))
    historical_data.validate()
    validate_fde_evidence()
    try:
        document = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read historical-tail manifest: {exc}")
    if document.get("format") != FORMAT or document.get("schema_version") != SCHEMA:
        fail("historical-tail manifest identity drift")
    if document.get("source_contract") != source_contract(args):
        fail("historical-tail source contract drift")
    if document.get("providers") != provider_document():
        fail("historical-tail provider roster drift")
    if document.get("semantic_groups") != semantic_document():
        fail("historical-tail semantic roster drift")
    result = document.get("result", {})
    for key, value in EXPECTED.items():
        if result.get(key) != value:
            fail(f"frozen historical-tail metric drift: {key}")
    if result.get("exact_chunk_indices") != EXACT_CHUNKS:
        fail("historical-tail exact-chunk roster drift")
    if sorted(result.get("exact_chunk_indices", []) + result.get("mismatching_chunk_indices", [])) != list(range(51)):
        fail("historical-tail chunk accounting drift")
    if result["equal_bytes"] + result["differing_bytes"] != result["target_initialized_size"]:
        fail("historical-tail byte accounting drift")
    if document.get("claims") != claims():
        fail("historical-tail claim boundary drift")
    return document


def compile_one(compiler: Path, flags: Sequence[str], source: Path, output: Path) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    run([compiler, *flags, "-c", source, "-o", output])


def build_source_objects(args: argparse.Namespace, compiler: Path) -> dict[str, Path]:
    v47.ensure_git_commit(v47.PS2DEV, v47.PS2DEV_REPO, v47.PS2DEV_COMMIT)
    newlib = v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    gcc_include = Path(run([compiler, "-print-file-name=include"])).resolve()
    if not newlib.is_dir() or not gcc_include.is_dir():
        fail("historical Newlib/GCC include roots are missing")

    try:
        archive = v46.download_archive(v46.SNES_ARCHIVE, v46.SNES_CACHE)
        original_root = v46.safe_extract_archive(
            archive, v46.SNES_CACHE / "source", v46.SNES_ARCHIVE.source_directory,
        )
    except v46.BuildFailure as exc:
        fail(str(exc))
    original = original_root / "snes9x"
    v46_compat = args.build_dir / "compat-v46"
    v47.atomic_write_text(
        v46_compat / "memory.h",
        "#ifndef STAGE3I_MEMORY_H\n#define STAGE3I_MEMORY_H\n#include <string.h>\n#endif\n",
    )
    v46_flags = [
        *v46.COMMON_FLAGS, "-Os", *v46.SNES_DEFINES, "-nostdinc",
        *v46.include_args([v46_compat, newlib, original, original / "unzip", original_root / "zlib", gcc_include]),
        "-x", "c++",
    ]

    old_build = v52.BUILD
    try:
        v52.BUILD = args.build_dir / "v52-rebuild"
        source_root, _unpatched, layout = v52.prepare_snes_layout()
        v52.patch_sources(layout)
        v52.replace_once(layout / "spc7110.h", *TIME32_PATCH, "32-bit target time_t layout")
        compat = args.build_dir / "compat-v52"
        v52.write_compat_headers(compat)
        v52_flags = [
            *v47.COMMON_FLAGS, "-Os", *v47.SNES_DEFINES, "-DZLIB", "-nostdinc",
            *v47.include_args([compat, newlib, layout, layout / "unzip", source_root / "zlib", gcc_include]),
            "-x", "c++",
        ]
        built: dict[str, Path] = {}
        for row in PROVIDERS:
            name = row["name"]
            output = args.build_dir / "source-objects" / f"{name}.o"
            if row["profile"] == "v46-official":
                compile_one(compiler, v46_flags, original / row["filename"], output)
            else:
                compile_one(compiler, v52_flags, layout / row["filename"], output)
            built[name] = output
        return built
    finally:
        v52.BUILD = old_build


def rebuild_provider_payloads(
    objects: dict[str, Path], reference: bytes, build_dir: Path,
) -> tuple[list[dict], int]:
    documents = []
    total_relocations = 0
    for row in PROVIDERS:
        elf = ELFFile(objects[row["name"]])
        item = section(elf, ".data")
        full = section_bytes(elf, item)
        if item.type != 1 or len(full) != row["full_size"] or digest(full) != row["full_sha256"]:
            fail(f"historical source section drift: {row['name']}")
        relocs = historical_data.relocations(elf, item.index)
        start, size = row["source_offset"], row["size"]
        selected = [rel for rel in relocs if start <= rel[0] and rel[0] + 4 <= start + size]
        if len(selected) != row["relocations"] or any(kind != 2 for _off, kind, _name in selected):
            fail(f"historical relocation roster drift: {row['name']}")
        source = bytearray(full[start:start + size])
        if digest(source) != row["slice_sha256"]:
            fail(f"historical source slice drift: {row['name']}")
        address = row["target_address"]
        target = reference[address - TARGET_BASE:address - TARGET_BASE + size]
        mask = bytearray(size)
        resolved_values = set()
        for offset, _kind, _symbol in selected:
            relative = offset - start
            mask[relative:relative + 4] = b"\x01" * 4
            source_word = struct.unpack_from("<I", source, relative)[0]
            target_word = struct.unpack_from("<I", target, relative)[0]
            resolved_values.add((target_word - source_word) & 0xFFFFFFFF)
            source[relative:relative + 4] = target[relative:relative + 4]
        outside = sum(
            left != right for index, (left, right) in enumerate(zip(source, target))
            if not mask[index]
        )
        if outside or bytes(source) != target or digest(source) != row["linked_sha256"]:
            fail(f"source-derived provider differs outside R_MIPS_32 relocations: {row['name']}")
        payload = build_dir / "provider-payloads" / f"{row['name']}.bin"
        payload.parent.mkdir(parents=True, exist_ok=True)
        payload.write_bytes(source)
        documents.append({
            "name": row["name"],
            "section": f".data.stage3i.source.{row['name']}",
            "address": address,
            "size": size,
            "payload": payload,
            "relocations": len(selected),
            "resolved_values": len(resolved_values),
        })
        total_relocations += len(selected)
    return documents, total_relocations


def render_provider_source(providers: Sequence[dict]) -> str:
    lines = ["/* Generated only from rebuilt public-source payloads. */"]
    for row in providers:
        path = str(row["payload"]).replace("\\", "\\\\").replace('"', '\\"')
        lines.extend([
            f'.section {row["section"]},"aw",@progbits',
            ".align 2",
            f'.incbin "{path}"',
        ])
    return "\n".join(lines) + "\n"


def render_semantic_source() -> str:
    lines = [
        "/* Generated from explicit DWARF semantics; contains no target payload. */",
        ".macro ST3I_A bytes",
        ".byte 0x04",
        ".4byte \\bytes",
        ".endm",
        ".macro ST3I_C bytes",
        ".byte 0x0e",
        ".uleb128 \\bytes",
        ".endm",
        ".macro ST3I_S reg, factor",
        ".byte 0x11",
        ".uleb128 \\reg",
        ".sleb128 \\factor",
        ".endm",
    ]
    for group_index, group in enumerate(SEMANTIC_GROUPS):
        lines.extend([f'.section {group["section"]},"aw",@progbits', ".align 2"])
        if "literal" in group:
            lines.extend(f".2byte {value}" for value in group["literal"])
            continue
        frame = f".Lst3i_frame_{group_index}"
        if group["cie"]:
            lines.extend([
                f"{frame}:",
                f".4byte .Lst3i_cie_end_{group_index}-.Lst3i_cie_body_{group_index}",
                f".Lst3i_cie_body_{group_index}:",
                ".4byte 0",
                ".byte 1",
                '.ascii "zP\\0"',
                ".uleb128 1",
                ".sleb128 4",
                ".byte 64",
                ".uleb128 5",
                ".byte 0",
                ".4byte stage3i_gxx_personality",
                ".byte 0x0c",
                ".uleb128 29",
                ".uleb128 0",
                f".Lst3i_cie_end_{group_index}:",
            ])
        for fde_index, fde in enumerate(group["fdes"]):
            tag = f"{group_index}_{fde_index}"
            body = f".Lst3i_fde_body_{tag}"
            lines.extend([
                f".4byte .Lst3i_fde_end_{tag}-{body}",
                f"{body}:",
                f".4byte {body}-{frame}" if group["cie"] else f".4byte 0x{fde['cie_pointer']:x}",
                f".4byte stage3i_pc_{fde['pc']:08x}",
                f".4byte 0x{fde['size']:x}",
                ".uleb128 0",
            ])
            for op in fde["ops"]:
                if op[0] == "advance_loc4":
                    lines.append(f"ST3I_A {op[1]}")
                elif op[0] == "def_cfa_offset":
                    lines.append(f"ST3I_C {op[1]}")
                elif op[0] == "offset_extended_sf":
                    lines.append(f"ST3I_S {op[1]}, {op[2]}")
                else:
                    fail(f"unknown semantic CFI opcode: {op[0]}")
            lines.extend([".align 2", f".Lst3i_fde_end_{tag}:"])
        lines.extend(".4byte 0" for _ in range(group["zero_words"]))
    return "\n".join(lines) + "\n"


def verify_generated_object(path: Path, rows: Sequence[dict], reference: bytes) -> tuple[int, int]:
    elf = ELFFile(path)
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, 1):
        fail("generated historical-tail object is not MIPS ELF32 ET_REL")
    total = 0
    for row in rows:
        item = section(elf, row["section"])
        if item.type != 1 or item.size != row["size"]:
            fail(f"generated section geometry drift: {row['section']}")
        target = reference[row["address"] - TARGET_BASE:row["address"] - TARGET_BASE + row["size"]]
        if not any(other.info == item.index and other.type in (4, 9) for other in elf.sections):
            # Provider sections are already resolved; semantic sections retain address relocations.
            if section_bytes(elf, item) != target:
                fail(f"generated relocation-free section differs: {row['section']}")
        total += item.size
    relocations = sum(
        item.size // item.entry_size for item in elf.sections
        if item.type in (4, 9) and item.entry_size
    )
    return total, relocations


def absorbed_symbol_aliases(input_path: Path, sections: Sequence[dict]) -> dict[str, int]:
    """Preserve global names while larger exact providers absorb old slices."""
    elf = ELFFile(input_path)
    rows = {row["section"]: row for row in sections}
    result: dict[str, int] = {}
    found_sections: set[str] = set()
    for item in elf.sections:
        if item.name not in ABSORBED_FIXED:
            continue
        found_sections.add(item.name)
        base = int(rows[item.name]["target_address"], 0)
        for symbol in elf.symbols:
            binding = symbol.info >> 4
            if symbol.section_index != item.index or not symbol.name or binding not in (1, 2):
                continue
            if symbol.value > item.size:
                fail(f"absorbed symbol falls outside its section: {symbol.name}")
            address = base + symbol.value
            previous = result.setdefault(symbol.name, address)
            if previous != address:
                fail(f"absorbed symbol has conflicting addresses: {symbol.name}")
    if found_sections != ABSORBED_FIXED:
        missing = ", ".join(sorted(ABSORBED_FIXED - found_sections))
        fail(f"absorbed fixed section missing from input: {missing}")
    return result


def render_linker_script(
    sections: Sequence[dict], providers: Sequence[dict], aliases: dict[str, int],
) -> str:
    symbols = {**startup.LINK_SYMBOLS, **frontend.EH_LINK_SYMBOLS, "stage3i_gxx_personality": PERSONALITY}
    for group in SEMANTIC_GROUPS:
        for fde in group.get("fdes", ()):
            symbols[f"stage3i_pc_{fde['pc']:08x}"] = fde["pc"]
    for name, address in aliases.items():
        if name in symbols and symbols[name] != address:
            fail(f"absorbed symbol conflicts with an integration symbol: {name}")
        symbols[name] = address
    lines = ['OUTPUT_FORMAT("elf32-littlemips")', "OUTPUT_ARCH(mips)", "ENTRY(_start)"]
    lines.extend(f"{name} = 0x{address:08x};" for name, address in sorted(symbols.items()))
    lines.extend([
        "SECTIONS {",
        "  .startup 0x00100000 : { *(.text.stage3g.crt0) }",
        "  .text 0x00100114 : { *(.text) }",
        "  .rodata : { *(.rodata) }",
        "  .data : { *(.data) }",
        "  .bss (NOLOAD) : { *(.bss) *(.bss.stage3.compatibility) }",
    ])
    placements = []
    for row in sections:
        if row["section"] not in startup.ABSORBED_ZERO_FILL and row["section"] not in ABSORBED_FIXED:
            placements.append((int(row["target_address"], 0), row["section"], row["region"] == "zero-fill"))
    placements.extend((row["address"], row["section"], False) for row in frontend.GROUPS)
    placements.extend((row["address"], row["section"], False) for row in providers)
    placements.extend((row["address"], row["section"], False) for row in SEMANTIC_GROUPS)
    for address, name, no_load in sorted(placements):
        suffix = " (NOLOAD)" if no_load else ""
        lines.append(f"  {name} 0x{address:08x}{suffix} : {{ KEEP(*({name})) }}")
    lines.append("  .bss.stage3g.crt0 0x00426e80 (NOLOAD) : { KEEP(*(.bss.stage3g.crt0)) }")
    discarded = set(startup.ABSORBED_ZERO_FILL) | ABSORBED_FIXED
    lines.append(
        "  /DISCARD/ : { " + " ".join(f"*({name})" for name in sorted(discarded))
        + " *(.data.stage3g.crt0) *(.reginfo) *(.mdebug*) *(.comment)"
        + " *(.pdr) *(.gnu.attributes) }"
    )
    lines.append("}")
    return "\n".join(lines) + "\n"


def verify_fixed_output(elf: ELFFile, sections: Sequence[dict]) -> None:
    by_name = {item.name: item for item in elf.sections if item.name}
    for row in sections:
        name = row["section"]
        if name in startup.ABSORBED_ZERO_FILL or name in ABSORBED_FIXED:
            if name in by_name:
                fail(f"absorbed fixed section survived: {name}")
            continue
        item = by_name.get(name)
        expected_type = 8 if row["region"] == "zero-fill" else 1
        if item is None or item.address != int(row["target_address"], 0) or item.size != int(row["extent_hex"], 0) or item.type != expected_type:
            fail(f"preserved fixed-section geometry drift: {name}")
        if row["region"] == "initialized" and digest(section_bytes(elf, item)) != row["sha256"]:
            fail(f"preserved fixed-section payload drift: {name}")


def probe(args: argparse.Namespace) -> dict:
    prior = frontend.validate(frontend.parse_args([
        "validate", "--manifest", str(args.frontend_manifest),
        "--sections", str(args.sections), "--layout", str(args.layout),
    ]))
    sections, layout = startup.load_inputs(startup.parse_args([
        "validate", "--sections", str(args.sections), "--layout", str(args.layout),
    ]))
    reference = args.reference.read_bytes()
    if digest(reference) != EXPECTED["target_sha256"]:
        fail("private unpacked reference SHA-256 drift")
    if not args.input.is_file() or not args.startup_object.is_file():
        fail("missing Stage-3F/startup build output")
    data_backing.check_sections(args.input, sections)
    startup.verify_relocatable(args.startup_object)
    compiler = resolve_tool(args.compiler)
    if run([compiler, "-dumpversion"]) != "3.2.2" or run([compiler, "-dumpmachine"]) != "ee":
        fail("historical-tail integration requires EE GCC 3.2.2 C++")
    if args.ld:
        linker = resolve_tool(args.ld)
    elif compiler.name.endswith("g++"):
        linker = compiler.with_name(compiler.name[:-3] + "ld")
    else:
        linker = sibling_tool(compiler, None, "ld")
    if args.objcopy:
        objcopy = resolve_tool(args.objcopy)
    elif compiler.name.endswith("g++"):
        objcopy = compiler.with_name(compiler.name[:-3] + "objcopy")
    else:
        objcopy = sibling_tool(compiler, None, "objcopy")
    if not linker.is_file() or not objcopy.is_file():
        fail("cannot derive historical linker/objcopy from C++ compiler")
    args.build_dir.mkdir(parents=True, exist_ok=True)

    source_objects = build_source_objects(args, compiler)
    providers, relocation_count = rebuild_provider_payloads(source_objects, reference, args.build_dir)
    provider_source = args.build_dir / "providers.S"
    provider_object = args.build_dir / "providers.o"
    provider_source.write_text(render_provider_source(providers), encoding="utf-8")
    compile_one(compiler, ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"), provider_source, provider_object)
    verify_generated_object(provider_object, providers, reference)

    semantic_source = args.build_dir / "semantic-cfi.S"
    semantic_object = args.build_dir / "semantic-cfi.o"
    semantic_source.write_text(render_semantic_source(), encoding="utf-8")
    compile_one(compiler, ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"), semantic_source, semantic_object)
    semantic_bytes, semantic_relocations = verify_generated_object(semantic_object, SEMANTIC_GROUPS, reference)

    frontend_object = args.build_dir / "frontend-eh-frames.o"
    compile_one(compiler, frontend.COMPILE_FLAGS, frontend.DEFAULT_SOURCE, frontend_object)
    frontend.verify_relocatable(frontend_object)
    linker_script = args.build_dir / "historical-tail.ld"
    output = args.build_dir / "stage3i-historical-tail-integrated.elf"
    raw_path = args.build_dir / "stage3i-historical-tail-integrated.unpadded.bin"
    padded_path = args.build_dir / "stage3i-historical-tail-integrated.padded.bin"
    aliases = absorbed_symbol_aliases(args.input, sections)
    linker_script.write_text(render_linker_script(sections, providers, aliases), encoding="utf-8")
    run([linker, "-EL", "-T", linker_script, "-o", output,
         args.startup_object, args.input, frontend_object, provider_object, semantic_object])
    elf = ELFFile(output)
    if (elf.elf_class, elf.endian, elf.machine, elf.file_type) != (1, "<", 8, 2):
        fail("historical-tail output is not MIPS ELF32 ET_EXEC")
    if any(item.type in (4, 9) and item.size for item in elf.sections):
        fail("historical-tail output retains relocations")
    startup.verify_symbols(elf)
    verify_fixed_output(elf, sections)
    startup_bss = section(elf, ".bss.stage3g.crt0")
    if startup_bss.address != startup.STARTUP_BSS or startup_bss.size != startup.STARTUP_BSS_SIZE or startup_bss.type != 8:
        fail("startup BSS geometry regressed")
    if section_bytes(elf, section(elf, ".startup")) != reference[:startup.STARTUP_SIZE]:
        fail("startup corridor regressed")
    for row in (*frontend.GROUPS, *providers, *SEMANTIC_GROUPS):
        item = section(elf, row["section"])
        expected = reference[row["address"] - TARGET_BASE:row["address"] - TARGET_BASE + row["size"]]
        if item.address != row["address"] or item.size != row["size"] or section_bytes(elf, item) != expected:
            fail(f"linked historical-tail range differs: {row['section']}")

    run([objcopy, "-O", "binary", output, raw_path])
    unpadded = raw_path.read_bytes()
    if len(unpadded) > len(reference):
        fail("historical-tail image exceeds target initialized size")
    padded = unpadded + bytes(len(reference) - len(unpadded))
    padded_path.write_bytes(padded)
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [index for index, (left, right) in enumerate(zip(padded, reference)) if left != right]
    if not differences:
        fail("unexpected complete match; this gate is not a replacement-ELF claim")
    return {
        "target_entry_address": layout["entry_address"],
        "integrated_entry_address": struct.unpack_from("<I", elf.data, 24)[0],
        "source_providers": len(providers),
        "source_bytes": sum(row["size"] for row in providers),
        "source_relocations": relocation_count,
        "semantic_groups": len(SEMANTIC_GROUPS),
        "semantic_fdes": sum(len(row.get("fdes", ())) for row in SEMANTIC_GROUPS),
        "semantic_bytes": semantic_bytes,
        "semantic_relocations": semantic_relocations,
        "absorbed_fixed_sections": len(ABSORBED_FIXED),
        "absorbed_symbol_aliases": len(aliases),
        "target_initialized_size": len(reference),
        "integrated_unpadded_size": len(unpadded),
        "terminal_zero_padding": len(reference) - len(unpadded),
        "chunk_count": len(exact) + len(different),
        "exact_chunks": len(exact),
        "mismatching_chunks": len(different),
        "exact_chunk_indices": exact,
        "mismatching_chunk_indices": different,
        "equal_bytes": len(reference) - len(differences),
        "differing_bytes": len(differences),
        "first_differing_address": TARGET_BASE + differences[0],
        "integrated_unpadded_sha256": digest(unpadded),
        "integrated_padded_sha256": digest(padded),
        "target_sha256": digest(reference),
        "prior_exact_chunks": prior["result"]["exact_chunks"],
        "prior_differing_bytes": prior["result"]["differing_bytes"],
    }


def parse_args(argv: Sequence[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("validate", "probe", "capture"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--frontend-manifest", type=Path, default=DEFAULT_FRONTEND_MANIFEST)
    parser.add_argument("--input", type=Path, default=DEFAULT_INPUT)
    parser.add_argument("--startup-object", type=Path, default=DEFAULT_STARTUP_OBJECT)
    parser.add_argument("--sections", type=Path, default=DEFAULT_SECTIONS)
    parser.add_argument("--layout", type=Path, default=DEFAULT_LAYOUT)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
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
            document = frozen_document(args, result)
            if args.command == "capture":
                args.manifest.parent.mkdir(parents=True, exist_ok=True)
                args.manifest.write_text(json.dumps(document, indent=2, sort_keys=True) + "\n", encoding="utf-8")
            elif validate(args) != document:
                fail("private historical-tail result differs from frozen manifest")
        result = document["result"]
        print(
            "verified historical C++ tail data: "
            f"source_providers={result['source_providers']} source_bytes={result['source_bytes']} "
            f"relocations={result['source_relocations']} semantic_fdes={result['semantic_fdes']}"
        )
        print(
            f"whole-image chunks={result['exact_chunks']}/{result['chunk_count']} "
            f"remaining={result['mismatching_chunks']} differing_bytes={result['differing_bytes']}; "
            "replacement ELF: not yet"
        )
        return 0
    except (
        HistoricalTailError, frontend.FrontendEhError, startup.StartupIntegrationError,
        data_backing.DataBackingError, historical_data.HistoricalDataError,
        link_layout_probe.LinkLayoutProbeError, OSError, ValueError, KeyError, RuntimeError,
    ) as exc:
        print(f"historical C++ tail data: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
