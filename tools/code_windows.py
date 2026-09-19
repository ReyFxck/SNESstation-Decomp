#!/usr/bin/env python3
"""Integrate the eleven proved Stage-3P code windows without freezing private payloads.

The gate assembles each 64 KiB window from historical/recovered objects and
explicit assembly proofs already accepted by the matching ledgers. Only
relocation-controlled instruction bits may be filled from the private unpacked
reference. Labelled assembly residuals cover old GCC schedules that are not
reproduced by the pinned source builds. Generated window payloads stay below
``build/``.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
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
WINDOW0_START = 0x00100114
WINDOW0_END = 0x00110000
WINDOW0_SIZE = WINDOW0_END - WINDOW0_START
WINDOW_START = 0x00110000
WINDOW_END = 0x001B0000
WINDOW_SIZE = 0x10000
WINDOW_COUNT = (WINDOW_END - WINDOW_START) // WINDOW_SIZE
TARGET_SHA256 = stage3o.TARGET_SHA256
LISTING_INSTRUCTION_RE = re.compile(
    r"^\s*([0-9a-fA-F]+):\s+"
    r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})\s+"
    r"([0-9a-fA-F]{2})\s+([0-9a-fA-F]{2})(?:\s|$)"
)

EVIDENCE_MANIFESTS = (
    "analysis/matching/hunt1000plus-v46-validated-42.tsv",
    "analysis/matching/hunt1000plus-v47-validated-79.tsv",
    "analysis/matching/hunt1041-v48-validated-25.tsv",
    "analysis/matching/hunt1041-v49-validated-20.tsv",
    "analysis/matching/hunt1041-v51-validated-16.tsv",
    "analysis/matching/hunt1041-v52-validated-17.tsv",
    "analysis/matching/hunt1041-v72-validated-v53-6.tsv",
    "analysis/matching/hunt1041-v73-validated-2.tsv",
    "analysis/matching/hunt1041-v75-validated-c4-5.tsv",
    "analysis/matching/hunt1041-v75-c4-companion-1.tsv",
    "analysis/matching/hunt1041-v76-validated-c4spr-1.tsv",
    "analysis/matching/hunt1041-v77-validated-c4draw-1.tsv",
    "analysis/matching/hunt1041-v78-validated-c4bit-1.tsv",
    "analysis/matching/hunt1041-v79-validated-c4conv-1.tsv",
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
    "hunt1041-v75-evidence",
    "hunt1041-v76-evidence",
    "hunt1041-v77-evidence",
    "hunt1041-v78-evidence",
    "hunt1041-v79-evidence",
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

# Window 0 combines exact recovered functions with complete historical object
# corridors.  The hashed recovered-object paths are deterministic products of
# the evidence rebuild; recording them here makes stale ledger paths fail
# closed instead of silently selecting a different compiler candidate.
WINDOW0_FIXED_SLICES = (
    SliceSpec("frontend_v48_001005b0", "build/matching/hunt1041-v48-closure/recovered/objects/o2/matching_candidates_hunt1041_v48.c-7406be02c459577a.o", 0x001005B0, 0x003C, symbol="v48_001005b0"),
    SliceSpec("frontend_v48_00101924", "build/matching/hunt1041-v48-closure/recovered/objects/o2/matching_candidates_hunt1041_v48.c-7406be02c459577a.o", 0x00101924, 0x007C, symbol="v48_00101924"),
    SliceSpec("frontend_swap_00101e8c", "build/matching/hunt1041-v49-closure/recovered/objects/os/src_ps2_progress11_frontend_recovered.c-19d249f9d6808a7c.o", 0x00101E8C, 0x0064, symbol="snes_p20_00101e8c"),
    SliceSpec("frontend_sram_path", "build/matching/hunt1041-v48-closure/recovered/objects/os/src_ps2_path_helpers_recovered.c-83f874a273f6f677.o", 0x00105750, 0x00AC, symbol="build_sram_path_00105750"),
    SliceSpec("frontend_makepath", "build/matching/hunt1041-v48-closure/recovered/objects/os/src_ps2_path_helpers_recovered.c-83f874a273f6f677.o", 0x001059CC, 0x011C, symbol="_makepath"),
    SliceSpec("frontend_v48_00105cb8", "build/matching/hunt1041-v48-closure/recovered/objects/os/matching_candidates_hunt1041_v48.c-94d08b9934d7796d.o", 0x00105CB8, 0x0078, symbol="v48_00105cb8"),
    SliceSpec("frontend_v48_00105d78", "build/matching/hunt1041-v48-closure/recovered/objects/os/matching_candidates_hunt1041_v48.c-94d08b9934d7796d.o", 0x00105D78, 0x008C, symbol="v48_00105d78"),
    SliceSpec("frontend_v48_00106054", "build/matching/hunt1041-v48-closure/recovered/objects/os/matching_candidates_hunt1041_v48.c-94d08b9934d7796d.o", 0x00106054, 0x0088, symbol="v48_00106054"),
    SliceSpec("frontend_srm_extension", "build/matching/hunt1041-v49-closure/recovered/objects/os/src_ps2_audio_rpc_recovered.c-bb25fcb1215428d2.o", 0x00106BCC, 0x003C, symbol="is_sram_extension_00106bcc"),
    SliceSpec("sjpcm_exact_suffix", "build/matching/hunt1000plus-v46-closure/pgen/sjpcm.o", 0x0010768C, 0x04F0, source_offset=0x00C0),
    SliceSpec("amigamod_exact_suffix", "build/matching/hunt1000plus-v46-closure/pgen/amiga.o", 0x00107D44, 0x0240, source_offset=0x01EC),
    SliceSpec("qsort", "build/matching/hunt1000plus-v47-closure/newlib/qsort.o", 0x001080CC, 0x0968),
    SliceSpec("libmtap", "build/matching/hunt1000plus-v46-closure/ps2dev/libmtap.o", 0x00108A9C, 0x02BC),
    SliceSpec("2xsai", "build/matching/hunt1000plus-v46-closure/snes/2XSAI.o", 0x00108D58, 0x1AE8),
    SliceSpec("apu", "build/matching/hunt1041-v52-closure/objects/apu-short.o", 0x0010A840, 0x1064),
    SliceSpec("c4", "build/matching/hunt1041-v75-c4/objects/c4.o", 0x0010B8A4, 0x0A5C),
    SliceSpec("cheats_prefix", "build/window11-rodata/source-objects/snes-CHEATS.o", 0x0010E360, 0x1CA0, source_offset=0x0400),
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

WINDOW0_RESIDUAL_SYMBOLS = (
    ("stage3p_frontend_leaf_cluster_00101838", 0x00101838, 0x0058),
    ("stage3p_frontend_dispatch_cluster_001018e0", 0x001018E0, 0x0044),
    ("stage3p_frontend_leaf_001019a0", 0x001019A0, 0x0008),
    ("stage3p_frontend_regalloc_001029c4", 0x001029C4, 0x00EC),
    ("stage3p_frontend_control_001041e4", 0x001041E4, 0x0050),
    ("stage3p_frontend_module_bridge_00104e48", 0x00104E48, 0x00D0),
    ("stage3p_frontend_drawsync_leaf_001056b0", 0x001056B0, 0x0010),
    ("stage3p_frontend_drawsync_tail_001056e0", 0x001056E0, 0x0038),
    ("stage3p_frontend_audio_tail_00105e38", 0x00105E38, 0x0010),
    ("stage3p_sjpcm_puts_bridge_00107578", 0x00107578, 0x0114),
    ("stage3p_amigamod_log_tail_00107f84", 0x00107F84, 0x0148),
    ("stage3p_c4_leaf_cluster_0010e33c", 0x0010E33C, 0x0024),
)

# Exact instruction schedules and alignment gaps not emitted by the available
# historical object set.  These are explicit EE assembly reconstructions in
# DEFAULT_RESIDUAL; no private binary payload is checked into the repository.
TAIL_RESIDUAL_SYMBOLS = (
    ("stage3p_tail_gap_00170000", 0x00170000, 0x022C),
    ("stage3p_tail_gap_001711cc", 0x001711CC, 0x001C),
    ("stage3p_tail_gap_00171230", 0x00171230, 0x001C),
    ("stage3p_tail_gap_00173c90", 0x00173C90, 0x016C),
    ("stage3p_tail_gap_00177e5c", 0x00177E5C, 0x0010),
    ("stage3p_tail_gap_0017ec4c", 0x0017EC4C, 0x1A58),
    ("stage3p_tail_gap_0018c1a8", 0x0018C1A8, 0x000C),
    ("stage3p_tail_gap_0018e130", 0x0018E130, 0x0008),
    ("stage3p_tail_gap_0018e164", 0x0018E164, 0x0010),
    ("stage3p_tail_gap_0019272c", 0x0019272C, 0x0B6C),
    ("stage3p_tail_gap_00196370", 0x00196370, 0x0010),
    ("stage3p_tail_gap_00196a10", 0x00196A10, 0x0010),
    ("stage3p_tail_gap_00196a38", 0x00196A38, 0x0010),
    ("stage3p_tail_gap_00196a60", 0x00196A60, 0x0010),
    ("stage3p_tail_gap_00196c08", 0x00196C08, 0x0010),
    ("stage3p_tail_gap_00196d68", 0x00196D68, 0x0010),
    ("stage3p_tail_gap_00197000", 0x00197000, 0x1A70),
    ("stage3p_tail_gap_00198bfc", 0x00198BFC, 0x0008),
    ("stage3p_tail_gap_00199170", 0x00199170, 0x0008),
    ("stage3p_tail_gap_00199288", 0x00199288, 0x0008),
    ("stage3p_tail_gap_0019bd5c", 0x0019BD5C, 0x0008),
    ("stage3p_tail_gap_0019bd70", 0x0019BD70, 0x0008),
    ("stage3p_tail_gap_0019be18", 0x0019BE18, 0x0008),
    ("stage3p_tail_gap_0019be5c", 0x0019BE5C, 0x0008),
    ("stage3p_tail_gap_0019beb4", 0x0019BEB4, 0x0018),
    ("stage3p_tail_gap_0019c050", 0x0019C050, 0x000C),
    ("stage3p_tail_gap_0019c0b4", 0x0019C0B4, 0x000C),
    ("stage3p_tail_gap_0019cfb8", 0x0019CFB8, 0x0008),
    ("stage3p_tail_gap_0019e364", 0x0019E364, 0x00B0),
    ("stage3p_tail_gap_0019f014", 0x0019F014, 0x0004),
    ("stage3p_tail_gap_0019f594", 0x0019F594, 0x003C),
    ("stage3p_tail_gap_001a0694", 0x001A0694, 0x000C),
    ("stage3p_tail_gap_001a06a8", 0x001A06A8, 0x0008),
    ("stage3p_tail_gap_001a06b8", 0x001A06B8, 0x0008),
    ("stage3p_tail_gap_001a06d4", 0x001A06D4, 0x0008),
    ("stage3p_tail_gap_001a0700", 0x001A0700, 0x0008),
    ("stage3p_tail_gap_001a1c90", 0x001A1C90, 0x0008),
    ("stage3p_tail_gap_001a3464", 0x001A3464, 0x000C),
    ("stage3p_tail_gap_001a3498", 0x001A3498, 0x0008),
    ("stage3p_tail_gap_001a3bc4", 0x001A3BC4, 0x000C),
    ("stage3p_tail_gap_001a3e28", 0x001A3E28, 0x0008),
    ("stage3p_tail_gap_001a4100", 0x001A4100, 0x1B58),
    ("stage3p_tail_gap_001a5c7c", 0x001A5C7C, 0x0044),
    ("stage3p_tail_gap_001a5d2c", 0x001A5D2C, 0x0004),
    ("stage3p_tail_gap_001a5d6c", 0x001A5D6C, 0x0004),
    ("stage3p_tail_gap_001a5f7c", 0x001A5F7C, 0x0004),
    ("stage3p_tail_gap_001a5fec", 0x001A5FEC, 0x0004),
    ("stage3p_tail_gap_001a6034", 0x001A6034, 0x0004),
    ("stage3p_tail_gap_001a608c", 0x001A608C, 0x0004),
    ("stage3p_tail_gap_001a6184", 0x001A6184, 0x0004),
    ("stage3p_tail_gap_001a6320", 0x001A6320, 0x01C8),
    ("stage3p_tail_gap_001a6a6c", 0x001A6A6C, 0x0004),
    ("stage3p_tail_gap_001a7434", 0x001A7434, 0x0004),
    ("stage3p_tail_gap_001a757c", 0x001A757C, 0x0004),
    ("stage3p_tail_gap_001a7630", 0x001A7630, 0x0800),
    ("stage3p_tail_gap_001a7e90", 0x001A7E90, 0x0010),
    ("stage3p_tail_gap_001a80c8", 0x001A80C8, 0x0008),
    ("stage3p_tail_gap_001a8130", 0x001A8130, 0x0010),
    ("stage3p_tail_gap_001a8330", 0x001A8330, 0x0008),
    ("stage3p_tail_gap_001a84d4", 0x001A84D4, 0x0014),
    ("stage3p_tail_gap_001a84f0", 0x001A84F0, 0x000C),
    ("stage3p_tail_gap_001a8534", 0x001A8534, 0x0014),
    ("stage3p_tail_gap_001a8550", 0x001A8550, 0x000C),
    ("stage3p_tail_gap_001a9c18", 0x001A9C18, 0x0008),
    ("stage3p_tail_gap_001a9c80", 0x001A9C80, 0x0008),
    ("stage3p_tail_gap_001a9e80", 0x001A9E80, 0x0008),
    ("stage3p_tail_gap_001a9f68", 0x001A9F68, 0x0040),
    ("stage3p_tail_gap_001aade0", 0x001AADE0, 0x0008),
    ("stage3p_tail_gap_001ab078", 0x001AB078, 0x0008),
    ("stage3p_tail_gap_001ab4e4", 0x001AB4E4, 0x0004),
)


def residual_ranges() -> list[tuple[str, int, int]]:
    """Return the labelled residual spans in target-address order.

    The source file is intentionally one readable assembly unit, but a single
    input section cannot represent the historical gaps between these symbols.
    The probe therefore gives every labelled span its own output section before
    handing the object to the real linker.
    """
    ranges: dict[tuple[str, int], int] = {}
    for name, address, size in (
        *RESIDUAL_SYMBOLS,
        *WINDOW0_RESIDUAL_SYMBOLS,
        *TAIL_RESIDUAL_SYMBOLS,
    ):
        key = (name, address)
        prior = ranges.get(key)
        if prior is not None and prior != size:
            fail(f"conflicting residual span: {name}")
        ranges[key] = size
    return sorted(
        [(name, address, size) for (name, address), size in ranges.items()],
        key=lambda item: item[1],
    )


def split_symbol_source(
    source: Path,
    output: Path,
    spans: list[tuple[str, int, int]],
    section_prefix: str,
) -> list[tuple[str, int, int]]:
    """Compile labelled assembly functions as addressable ELF sections."""
    by_name = {name: (address, size) for name, address, size in spans}
    symbol_re = re.compile(r"^\s*\.globl\s+(\S+)")
    lines = source.read_text(encoding="utf-8").splitlines()
    transformed: list[str] = []
    seen: set[str] = set()
    for line in lines:
        match = symbol_re.match(line)
        if match and match.group(1) in by_name:
            name = match.group(1)
            address, _size = by_name[name]
            if name in seen:
                fail(f"duplicate residual symbol: {name}")
            seen.add(name)
            if transformed and transformed[-1].strip() == ".balign 4":
                transformed.pop()
                transformed.append(
                    f'    .section {section_prefix}.{address:08x},"ax",@progbits'
                )
                transformed.append("    .balign 4")
            else:
                transformed.append(
                    f'    .section {section_prefix}.{address:08x},"ax",@progbits'
                )
        transformed.append(line)
    expected = set(by_name)
    if seen != expected:
        missing = ", ".join(sorted(expected - seen))
        extra = ", ".join(sorted(seen - expected))
        fail(f"residual section roster drift: missing={missing}; extra={extra}")
    output.write_text("\n".join(transformed) + "\n", encoding="utf-8")
    return spans


def split_residual_source(source: Path, output: Path) -> list[tuple[str, int, int]]:
    """Compile the labelled residual source as addressable ELF sections."""
    return split_symbol_source(
        source, output, residual_ranges(), ".text.stage3p.residual"
    )


DIRECT_ASSEMBLY_GROUPS = (
    (
        "v80",
        "matching/candidates/hunt1041_v80_quickwins_exact.S",
        "hunt1041_v80_quickwins_exact.o",
        ".text.stage3p.direct.v80",
    ),
    (
        "v81",
        "matching/candidates/hunt1041_v81_final20_exact.S",
        "hunt1041_v81_final20_exact.o",
        ".text.stage3p.direct.v81",
    ),
    (
        "v79-c4conv",
        "matching/candidates/c4convoam_exact.S",
        "c4convoam_exact.o",
        ".text.stage3p.direct.v79_c4conv",
    ),
)


# This recovered source translation unit contains a proved 0x30-byte function
# at 0x00151330. Compile just that function so its two external calls can be
# bound to their proved target addresses without duplicating source-tree
# symbols in the compatibility input.
DIRECT_SOURCE_OBJECTS = (
    (
        "rom-cleanup",
        "src/snes9x/memory_cleanup_recovered.c",
        ".text.stage3p.direct.rom_cleanup",
        0x00151330,
        0x0030,
    ),
)


# The historical PS2SDK assembly object contains the complete function and
# its four-byte alignment gap. Keep the source object intact in the final link.
DIRECT_WHOLE_OBJECTS = (
    (
        "syncdcache",
        "build/matching/hunt1000plus-v46-closure/ps2dev/SyncDCache.o",
        ".text.stage3p.direct.syncdcache",
        0x001AB440,
        0x00A8,
        "_SyncDCache",
    ),
)


def selected_symbol_spans(
    selected: list[dict], object_name: str
) -> list[tuple[str, int, int]]:
    """Extract the proved symbol roster for one exact assembly object."""
    rows = [row for row in selected if object_name in str(row.get("object", ""))]
    spans: dict[tuple[str, int], int] = {}
    for row in rows:
        symbol = str(row.get("symbol", ""))
        if not symbol:
            fail(f"exact assembly row has no symbol: {object_name}")
        key = (symbol, int(row["address"]))
        size = int(row["size"])
        prior = spans.get(key)
        if prior is not None and prior != size:
            fail(f"conflicting exact assembly span: {symbol}")
        spans[key] = size
    return sorted(
        [(symbol, address, size) for (symbol, address), size in spans.items()],
        key=lambda item: item[1],
    )

EXPECTED: dict[str, object] = {
    "chunk_count": 51,
    "code_windows": 11,
    "differences_removed": 618_286,
    "differing_bytes": 0,
    "exact_chunks": 51,
    "mismatching_chunks": 0,
    "first_differing_address": None,
    "historical_recovered_bytes": 525_460,
    "integrated_padded_sha256": TARGET_SHA256,
    "listing_bytes": 73_192,
    "prior_differing_bytes": 618_286,
    "prior_exact_assembly_bytes": 85_628,
    "relocation_fields": 39_226,
    "residual_bytes": 36_340,
    "selected_source_contract_sha256": "520cf28fe8c51b9cdeb845bd6b74a808bbd9fb283fd1148b3b23e233f7d9ab87",
    "selected_source_slices": 568,
    "source_bytes": 720_620,
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
        "windows_0_through_6_exact": True,
        "windows_0_through_10_exact": True,
        "windows_1_through_6_exact": True,
        "replacement_elf": False,
        "unpacked_hash_matched": True,
        "packed_hash_matched": False,
    }


def source_contract() -> dict:
    return {
        "evidence_manifests": list(EVIDENCE_MANIFESTS),
        "evidence_targets": list(EVIDENCE_TARGETS),
        "fixed_slices": [spec.__dict__ for spec in FIXED_SLICES],
        "window0_fixed_slices": [spec.__dict__ for spec in WINDOW0_FIXED_SLICES],
        "extra_symbols": [spec.__dict__ for spec in EXTRA_SYMBOLS],
        "residual_source": str(DEFAULT_RESIDUAL.relative_to(ROOT)),
        "residual_symbols": [
            {"symbol": name, "address": address, "size": size}
            for name, address, size in RESIDUAL_SYMBOLS
        ],
        "window0_residual_symbols": [
            {"symbol": name, "address": address, "size": size}
            for name, address, size in WINDOW0_RESIDUAL_SYMBOLS
        ],
        "tail_residual_symbols": [
            {"symbol": name, "address": address, "size": size}
            for name, address, size in TAIL_RESIDUAL_SYMBOLS
        ],
        "public_listings": public_listing_contract(),
    }


def public_listing_contract() -> list[dict[str, object]]:
    result: list[dict[str, object]] = []
    for path in sorted((ROOT / "analysis/functions").glob("*.asm")):
        count = 0
        for line in path.read_text(encoding="utf-8").splitlines():
            match = LISTING_INSTRUCTION_RE.match(line)
            if match and WINDOW_START <= int(match.group(1), 16) < WINDOW_END:
                count += 1
        if count:
            result.append({
                "path": str(path.relative_to(ROOT)),
                "sha256": digest(path.read_bytes()),
                "instruction_words": count,
            })
    return result


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
    if result.get("exact_chunk_indices") != list(range(51)):
        fail("exact-window roster drift")
    if result.get("source_bytes") != WINDOW0_SIZE + WINDOW_COUNT * WINDOW_SIZE:
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
        # Match-miner cache filenames include a checkout-local cache key.  The
        # committed ledger intentionally freezes the proved object identity,
        # but a clean checkout can rebuild the same bytes under a different
        # suffix when the compiler's absolute path changes.  Resolve only a
        # unique sibling with the same source/profile prefix; raw bytes and
        # relocation masks are still checked against the frozen proof below.
        match = re.fullmatch(r"(.+)-[0-9a-f]{16}\.o", path.name)
        siblings = sorted(path.parent.glob(f"{match.group(1)}-*.o")) if match else []
        if len(siblings) != 1:
            fail(f"missing proved candidate object: {spec.object}")
        path = siblings[0]
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


def tsv_specs(start: int = WINDOW_START, end: int = WINDOW_END) -> list[SliceSpec]:
    rows: list[tuple[int, int, int, str, str]] = []
    object_pool: set[str] = {
        spec.object for spec in (*FIXED_SLICES, *WINDOW0_FIXED_SLICES, *EXTRA_SYMBOLS)
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
                if symbol and size > 0 and start <= address and address + size <= end:
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


def add_public_listings(buffers: list[bytearray], covered: list[bytearray],
                        reference: bytes) -> tuple[list[dict], int]:
    """Fill uncovered words from committed objdump listings.

    Listings are public textual disassembly evidence.  Every byte is checked
    against overlaps and, in the private gate, against the unpacked oracle.
    """
    selected: list[dict] = []
    total_new = 0
    for item in public_listing_contract():
        path = ROOT / str(item["path"])
        newly = 0
        first: int | None = None
        last: int | None = None
        for line in path.read_text(encoding="utf-8").splitlines():
            match = LISTING_INSTRUCTION_RE.match(line)
            if match is None:
                continue
            address = int(match.group(1), 16)
            if not (WINDOW_START <= address and address + 4 <= WINDOW_END):
                continue
            raw = bytes(int(match.group(index), 16) for index in range(2, 6))
            target_offset = address - TARGET_BASE
            if reference[target_offset:target_offset + 4] != raw:
                fail(f"public listing differs from target: {item['path']} @ 0x{address:08x}")
            for index, value in enumerate(raw):
                absolute = address + index
                window = (absolute - WINDOW_START) // WINDOW_SIZE
                offset = (absolute - WINDOW_START) % WINDOW_SIZE
                if covered[window][offset] and buffers[window][offset] != value:
                    fail(f"conflicting public listing overlap: {item['path']} @ 0x{absolute:08x}")
                if not covered[window][offset]:
                    covered[window][offset] = 1
                    buffers[window][offset] = value
                    newly += 1
                    total_new += 1
                    first = absolute if first is None else min(first, absolute)
                    last = absolute if last is None else max(last, absolute)
        if newly:
            selected.append({
                "name": f"listing_{path.stem}",
                "object": str(item["path"]),
                "section": "objdump-byte-columns",
                "source_offset": 0,
                "symbol": "",
                "address": first,
                "size": (last - first + 1) if first is not None and last is not None else 0,
                "new_bytes": newly,
                "relocations": 0,
                "raw_sha256": item["sha256"],
            })
    return selected, total_new


def assemble_windows(reference: bytes, residual_object: Path) -> tuple[list[bytes], dict]:
    buffers = [bytearray(WINDOW_SIZE) for _ in range(WINDOW_COUNT)]
    covered = [bytearray(WINDOW_SIZE) for _ in range(WINDOW_COUNT)]
    selected: list[dict] = []
    relocation_fields = 0

    specs = [*FIXED_SLICES, *tsv_specs(WINDOW_START, WINDOW_END), *EXTRA_SYMBOLS]
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

    listing_selected, listing_bytes = add_public_listings(buffers, covered, reference)
    selected.extend(listing_selected)

    tail_specs = [
        SliceSpec(name, str(residual_object.relative_to(ROOT)), address, size, symbol=name)
        for name, address, size in TAIL_RESIDUAL_SYMBOLS
    ]
    for spec in tail_specs:
        newly, relocations, raw_hash = add_slice(buffers, covered, reference, spec)
        relocation_fields += relocations
        if newly:
            selected.append({
                "name": spec.name,
                "object": str(DEFAULT_RESIDUAL.relative_to(ROOT)),
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
    residual_bytes = sum(size for _name, _address, size in (*RESIDUAL_SYMBOLS, *TAIL_RESIDUAL_SYMBOLS))
    historical_recovered_bytes = sum(map(len, buffers)) - prior_assembly_bytes - residual_bytes - listing_bytes
    return [bytes(item) for item in buffers], {
        "selected_source_slices": len(selected),
        "relocation_fields": relocation_fields,
        "source_bytes": sum(map(len, buffers)),
        "historical_recovered_bytes": historical_recovered_bytes,
        "prior_exact_assembly_bytes": prior_assembly_bytes,
        "residual_bytes": residual_bytes,
        "listing_bytes": listing_bytes,
        "selected_source_contract_sha256": digest(json.dumps(selected, sort_keys=True).encode()),
        "selected_sources": selected,
    }


def add_window0_slice(buffer: bytearray, covered: bytearray, reference: bytes,
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
        if not (WINDOW0_START <= absolute < WINDOW0_END):
            fail(f"source slice crosses window 0 code boundary: {spec.name}")
        offset = absolute - WINDOW0_START
        if covered[offset] and buffer[offset] != value:
            fail(f"conflicting exact source overlap: {spec.name}")
        if not covered[offset]:
            newly += 1
        buffer[offset] = value
        covered[offset] = 1
    return newly, relocations, digest(raw)


def assemble_window0(reference: bytes, residual_object: Path) -> tuple[bytes, dict]:
    buffer = bytearray(WINDOW0_SIZE)
    covered = bytearray(WINDOW0_SIZE)
    selected: list[dict] = []
    relocation_fields = 0
    specs = [*WINDOW0_FIXED_SLICES, *tsv_specs(WINDOW0_START, WINDOW0_END)]
    specs.extend(
        SliceSpec(name, str(residual_object.relative_to(ROOT)), address, size, symbol=name)
        for name, address, size in WINDOW0_RESIDUAL_SYMBOLS
    )
    for spec in specs:
        try:
            newly, relocations, raw_hash = add_window0_slice(buffer, covered, reference, spec)
        except (CodeWindowsError, ValueError):
            if spec in WINDOW0_FIXED_SLICES or spec.symbol.startswith("stage3p_"):
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
    start = None
    for index, bit in enumerate([*covered, 1]):
        if not bit and start is None:
            start = index
        elif bit and start is not None:
            gaps.append((WINDOW0_START + start, WINDOW0_START + index))
            start = None
    if gaps:
        shown = ", ".join(f"0x{start:08x}-0x{end:08x}" for start, end in gaps[:24])
        fail(f"uncovered window 0 source bytes: {shown}")
    prior_assembly_bytes = sum(
        row["new_bytes"] for row in selected
        if "v80" in row["object"] or "v81" in row["object"]
    )
    residual_bytes = sum(size for _name, _address, size in WINDOW0_RESIDUAL_SYMBOLS)
    return bytes(buffer), {
        "selected_source_slices": len(selected),
        "relocation_fields": relocation_fields,
        "source_bytes": WINDOW0_SIZE,
        "historical_recovered_bytes": WINDOW0_SIZE - prior_assembly_bytes - residual_bytes,
        "prior_exact_assembly_bytes": prior_assembly_bytes,
        "residual_bytes": residual_bytes,
        "selected_sources": selected,
    }


def payload_segments(
    window0: bytes,
    windows: list[bytes],
    spans: list[tuple[str, int, int]],
    build_dir: Path,
) -> list[tuple[str, Path, int]]:
    """Write payload gaps while leaving labelled object spans to ``ee-ld``."""
    regions = [(WINDOW0_START, window0)] + [
        (WINDOW_START + index * WINDOW_SIZE, data)
        for index, data in enumerate(windows)
    ]
    ranges = [(address, address + size) for _name, address, size in spans]
    if any(end <= start for start, end in ranges):
        fail("empty residual span")
    if any(left + size > right
           for (_name, left, size), (_other, right, _other_size)
           in zip(spans, spans[1:])):
        fail("overlapping residual spans")
    result: list[tuple[str, Path, int]] = []
    counter = 0
    for region_start, data in regions:
        region_end = region_start + len(data)
        cursor = region_start
        for span_start, span_end in ranges:
            if span_end <= region_start:
                continue
            if span_start >= region_end:
                break
            overlap_start = max(span_start, region_start)
            overlap_end = min(span_end, region_end)
            if overlap_start > cursor:
                section = f".text.stage3p.payload.{counter:04d}"
                path = build_dir / f"payload-{counter:04d}.bin"
                path.write_bytes(
                    data[cursor - region_start:overlap_start - region_start]
                )
                result.append((section, path, cursor))
                counter += 1
            cursor = max(cursor, overlap_end)
        if cursor < region_end:
            section = f".text.stage3p.payload.{counter:04d}"
            path = build_dir / f"payload-{counter:04d}.bin"
            path.write_bytes(data[cursor - region_start:])
            result.append((section, path, cursor))
            counter += 1
    return result


def render_payload_source(paths: list[tuple[str, Path, int]]) -> str:
    lines = ["/* Generated Stage-3P payload; ignored build artifact. */", "    .set noreorder"]
    for section, path, _address in paths:
        lines.extend([
            f'    .section {section},"ax",@progbits',
            "    .balign 4",
            f'    .incbin "{path}"',
        ])
    return "\n".join(lines) + "\n"


def update_linker_script(
    text: str,
    payload_parts: list[tuple[str, Path, int]],
    direct_sections: list[tuple[str, int, int, tuple[str, str] | None]],
) -> str:
    old = """  .text 0x00100114 : { *(.text) }
  .rodata : { *(.rodata) }
  .data : { *(.data) }
  .bss (NOLOAD) : { *(.bss) *(.bss.stage3.compatibility) }
"""
    code_rules = [
        (
            address,
            f'  {section} 0x{address:08x} : {{ KEEP(*({section})) }}',
        )
        for section, _path, address in payload_parts
    ]
    code_rules.extend(
        (
            address,
            f'  {prefix}.{address:08x} 0x{address:08x} : {{ KEEP('
            f'{selector[0] + "(" + selector[1] + ")" if selector else "*(" + prefix + "." + format(address, "08x") + ")"}'
            f') }}',
        )
        for prefix, address, _size, selector in direct_sections
    )
    code_rules.sort(key=lambda item: item[0])
    code_rules_text = "\n".join(rule for _address, rule in code_rules)
    new = f"""  stage3p_target_per_rom_cleanup = 0x00151360;
  stage3p_target_snes_memory_helper = 0x00150f54;
  .text.stage3p.symbols 0x00100114 (NOLOAD) : {{ *(.text) }}
  .rodata.stage3p.symbols 0x0016d5d0 (NOLOAD) : {{ *(.rodata) }}
{code_rules_text}
  /DISCARD/ : {{ *(.text.stage3p.residual.001ab4e4) }}
  .data.stage3p.symbols 0x00170148 (NOLOAD) : {{ *(.data) }}
  .bss.stage3p.symbols 0x001702c0 (NOLOAD) : {{ *(.bss) *(.bss.stage3.compatibility) }}
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

    residual_source = args.build_dir / "stage3p-residual-sections.S"
    residual_spans = split_residual_source(args.residual, residual_source)
    residual_object = args.build_dir / "stage3p-residual.o"
    stage3o.stage3i.compile_one(
        cxx, ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"),
        residual_source, residual_object,
    )
    window0, window0_metrics = assemble_window0(reference, residual_object)
    windows, metrics = assemble_windows(reference, residual_object)
    selected_sources = [*window0_metrics["selected_sources"], *metrics["selected_sources"]]
    metrics = {
        "selected_source_slices": len(selected_sources),
        "relocation_fields": window0_metrics["relocation_fields"] + metrics["relocation_fields"],
        "source_bytes": window0_metrics["source_bytes"] + metrics["source_bytes"],
        "historical_recovered_bytes": window0_metrics["historical_recovered_bytes"] + metrics["historical_recovered_bytes"],
        "prior_exact_assembly_bytes": window0_metrics["prior_exact_assembly_bytes"] + metrics["prior_exact_assembly_bytes"],
        "residual_bytes": window0_metrics["residual_bytes"] + metrics["residual_bytes"],
        "listing_bytes": metrics["listing_bytes"],
        "selected_source_contract_sha256": digest(json.dumps(selected_sources, sort_keys=True).encode()),
        "selected_sources": selected_sources,
    }

    prior_padded = args.stage3o_build / "stage3o-window11-rodata-integrated.padded.bin"
    base_script = args.stage3o_build / "window11-rodata.ld"
    if not prior_padded.is_file() or not base_script.is_file():
        fail("missing Stage-3O dependency; run make window11-rodata")
    direct_sections = [
        (".text.stage3p.residual", address, size, None)
        for _name, address, size in residual_spans
        if address != 0x001AB4E4
    ]
    direct_objects = [residual_object]
    for key, source_relative, object_name, section_prefix in DIRECT_ASSEMBLY_GROUPS:
        spans = selected_symbol_spans(selected_sources, object_name)
        if not spans:
            continue
        source = ROOT / source_relative
        transformed = args.build_dir / f"stage3p-{key}-sections.S"
        split_symbol_source(source, transformed, spans, section_prefix)
        output = args.build_dir / f"stage3p-{key}-sections.o"
        stage3o.stage3i.compile_one(
            cxx, ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"),
            transformed, output,
        )
        direct_objects.append(output)
        direct_sections.extend(
            (section_prefix, address, size, None)
            for _name, address, size in spans
        )
    for key, source_relative, section_prefix, address, size in DIRECT_SOURCE_OBJECTS:
        source = ROOT / source_relative
        compiled = args.build_dir / f"stage3p-{key}-compiled.o"
        stage3o.stage3i.compile_one(
            cxx,
            (
                "-G0", "-O2", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
                "-ffunction-sections", "-DSNESSTATION_STAGE3P_ROM_ONLY",
                "-I", ROOT / "include", "-I",
                ROOT / "include/ee_stage1_compat",
            ),
            source,
            compiled,
        )
        output = args.build_dir / f"stage3p-{key}-direct.o"
        run([
            objcopy,
            "--rename-section", f".text.rom_cleanup_00151330={section_prefix}.{address:08x}",
            "--redefine-sym", "rom_cleanup_00151330=stage3p_rom_cleanup_direct",
            "--redefine-sym", "per_rom_buffer_cleanup_00151360=stage3p_target_per_rom_cleanup",
            "--redefine-sym", "snes_memory_helper_00150f54=stage3p_target_snes_memory_helper",
            compiled, output,
        ])
        direct_objects.append(output)
        direct_sections.append((section_prefix, address, size, None))
    for key, source_relative, section_prefix, address, size, symbol in DIRECT_WHOLE_OBJECTS:
        source = ROOT / source_relative
        historical = ELFFile(source)
        sections = [section for section in historical.sections if section.name == ".text"]
        if len(sections) != 1 or sections[0].size != size:
            fail(f"historical whole object section drift: {source_relative}")
        section = sections[0]
        if not any(
            entry.name == symbol and entry.section_index == section.index
            and entry.value == 0 and entry.size == size - 4
            for entry in historical.symbols
        ):
            fail(f"historical whole object symbol drift: {source_relative}")
        if any(item.name in (".rel.text", ".rela.text") for item in historical.sections):
            fail(f"historical whole object has relocations: {source_relative}")
        if historical.data[section.offset:section.offset + size] != reference[
            address - TARGET_BASE:address - TARGET_BASE + size
        ]:
            fail(f"historical whole object differs from target: {source_relative}")
        output = args.build_dir / f"stage3p-{key}-direct.o"
        run([
            objcopy,
            "--rename-section", f".text={section_prefix}.{address:08x}",
            "--redefine-sym", f"{symbol}=stage3p_{key}_direct",
            source, output,
        ])
        direct_objects.append(output)
        direct_sections.append((section_prefix, address, size, None))
    direct_sections.sort(key=lambda item: item[1])
    direct_spans = [
        (f"direct_{address:08x}", address, size)
        for _prefix, address, size, _selector in direct_sections
    ]
    source_paths = payload_segments(window0, windows, direct_spans, args.build_dir)
    metrics.update(
        {
            "direct_object_bytes": sum(
                size for _prefix, _address, size, _selector in direct_sections
            ),
            "direct_object_inputs": len(direct_objects),
            "direct_candidate_object_inputs": len(DIRECT_WHOLE_OBJECTS),
            "direct_object_sections": len(direct_sections),
            "incbin_payload_bytes": sum(
                path.stat().st_size for _section, path, _address in source_paths
            ),
            "incbin_payload_sections": len(source_paths),
        }
    )
    payload_source = args.build_dir / "code-windows.S"
    payload_source.write_text(render_payload_source(source_paths), encoding="utf-8")
    payload_object = args.build_dir / "code-windows.o"
    stage3o.stage3i.compile_one(
        cxx, ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"),
        payload_source, payload_object,
    )

    linker_script = args.build_dir / "code-windows.ld"
    linker_script.write_text(
        update_linker_script(
            base_script.read_text(encoding="utf-8"), source_paths, direct_sections
        ),
        encoding="utf-8",
    )
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
        args.stage3o_build / "window11-semantics.o",
        *direct_objects,
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
    if padded[0x114:0x10000] != window0:
        fail("linked code window 0 differs from source reconstruction")
    for index, expected in enumerate(windows, start=1):
        if padded[index * WINDOW_SIZE:(index + 1) * WINDOW_SIZE] != expected:
            fail(f"linked code window {index} differs from source reconstruction")

    _sections, layout = startup.load_inputs(startup.parse_args(["validate", "--layout", str(args.layout)]))
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [index for index, (actual, target) in enumerate(zip(padded, reference)) if actual != target]
    return {
        **metrics,
        "code_windows": 11,
        "exact_chunks": len(exact),
        "mismatching_chunks": len(different),
        "exact_chunk_indices": exact,
        "mismatching_chunk_indices": different,
        "chunk_count": len(exact) + len(different),
        "differing_bytes": len(differences),
        "prior_differing_bytes": prior["result"]["differing_bytes"],
        "differences_removed": prior["result"]["differing_bytes"] - len(differences),
        "first_differing_address": TARGET_BASE + differences[0] if differences else None,
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
            f"differing_bytes={result['differing_bytes']}; complete ELF verified separately"
        )
        return 0
    except (CodeWindowsError, stage3o.Window11RodataError, OSError, ValueError, KeyError, RuntimeError) as exc:
        print(f"Stage-3P code windows: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
