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
import struct
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


# Newlib qsort is a complete historical object with one self-call relocation.
# The linker resolves that relocation from the object's own renamed symbol.
DIRECT_SELF_RELOC_OBJECTS = (
    (
        "qsort",
        "build/matching/hunt1000plus-v47-closure/newlib/qsort.o",
        ".text.stage3p.direct.qsort",
        0x001080CC,
        0x0968,
        "qsort",
        0x04A0,
    ),
)


# These complete historical objects have only R_MIPS_26 external calls. Their
# relocation results are checked against the private reference during the
# probe; no reference instruction bytes are committed or copied into an input.
DIRECT_CALL_OBJECTS = (
    ("iop-reset", "build/matching/hunt1000plus-v47-closure/kernel/iop-reset.o", ".text.stage3p.direct.iop_reset", 0x0019D740, 0x010C, "SifIopReset", 11),
    ("calloc", "build/matching/hunt1000plus-v47-closure/ps2lib/calloc.o", ".text.stage3p.direct.calloc", 0x0019E648, 0x0050, "calloc", 2),
    ("memalign", "build/matching/hunt1000plus-v47-closure/ps2lib/memalign.o", ".text.stage3p.direct.memalign", 0x0019E698, 0x00EC, "memalign", 2),
    ("strstr", "build/matching/hunt1000plus-v47-closure/ps2lib/strstr.o", ".text.stage3p.direct.strstr", 0x0019EAF8, 0x0088, "strstr", 2),
)

# Historical code sections with proved MIPS calls and address relocations.
# CPU retains its own code calls; CPU and SETA keep previously proved data
# providers in place instead of linking their data sections a second time.
DIRECT_EXTERNAL_RELOC_OBJECTS = (
    # CPU's unwind data remains at its already proved semantic provider. Its
    # complete code has only external relocations, so the producer's original
    # text section can be linked independently.
    ("cpu", "build/matching/hunt1000plus-v46-closure/snes/CPU.o", 0x001159F4, 0x03BC, 61),
    ("seta", "build/matching/hunt1000plus-v46-closure/snes/seta.o", 0x0016FC48, 0x0048, 4),
    ("fio_write", "kernel/fio-write.o", 0x0019D244, 0x11C, 20),
    ("fio_read_intr", "kernel/fio-read-intr.o", 0x0019D4B0, 0x084, 6),
    ("iop_alloc", "kernel/iop-alloc.o", 0x0019D63C, 0x07C, 6),
    ("iop_free", "kernel/iop-free.o", 0x0019D6B8, 0x088, 6),
    ("free", "ps2lib/free.o", 0x0019E784, 0x0DC, 10),
    ("strtol", "ps2lib/strtol.o", 0x0019EB80, 0x22C, 9),
    ("load_module", "kernel/load-module.o", 0x0019F7E8, 0x10C, 9),
    ("load_buffer", "kernel/load-buffer.o", 0x0019F8F4, 0x0F4, 8),
)


DIRECT_SAI2_OBJECT = (
    "build/matching/hunt1000plus-v46-closure/snes/2XSAI.o",
    ".text.stage3p.direct.sai2",
    0x00108D58,
    0x1AE8,
    ".data.stage3p.direct.sai2",
    0x00335284,
    0x00EC,
)

DIRECT_FXEMU_OBJECT = (
    "build/matching/hunt1000plus-v46-closure/snes/fxemu.o",
    ".text.stage3p.direct.fxemu", 0x0012FE5C, 0x0D80,
    ".data.stage3p.direct.fxemu", 0x00342A38, 0x08E8,
)

DIRECT_FXINST_OBJECT = (
    "build/matching/hunt1000plus-v46-closure/snes/fxinst.o",
    ".text.stage3p.direct.fxinst", 0x00130BDC, 0x11E9C,
    ".data.stage3p.direct.fxinst", 0x00343320, 0x1A10,
    ".rodata.stage3p.direct.fxinst", 0x001B4678, 0x0040,
)

DIRECT_SA1CPU_OBJECT = (
    "build/matching/hunt1000plus-v46-closure/snes/SA1CPU.o",
    ".text.stage3p.direct.sa1cpu", 0x0015F1C8, 0x107E8,
    ".data.stage3p.direct.sa1cpu", 0x003F5040, 0x5570,
)

DIRECT_PPU_OBJECT = (
    "build/matching/hunt1041-v52-closure/objects/ppu-short.o",
    ".text.stage3p.direct.ppu", 0x00159058, 0x4894,
    ".data.stage3p.direct.ppu", 0x003F4BF0, 0x0288,
    ".rodata.stage3p.direct.ppu", 0x001B7318, 0x0AC0,
    ".bss.stage3p.direct.ppu", 0x0042E888, 2,
)

DIRECT_DMA_OBJECT = (
    "build/matching/hunt1041-v72-v53-promotion/objects/dma.o",
    ".text.stage3p.direct.dma", 0x00129AF4, 0x2468,
    ".rodata.stage3p.direct.dma", 0x001B1F20, 0x38,
    ".data.stage3p.unlinked.dma", 0x0033CCC8, 0x104,
)

DIRECT_CPUEXEC_OBJECT = (
    "build/matching/hunt1000plus-v46-closure/snes/CPUEXEC.o",
    ".text.stage3p.direct.cpuexec", 0x00115DB0, 0x990,
    ".data.stage3p.direct.cpuexec", 0x00336784, 0x74,
)

DIRECT_GFX_OBJECT = (
    "build/matching/hunt1041-v52-closure/objects/gfx-short.o",
    ".text.stage3p.direct.gfx", 0x00142A78, 0xDC14,
    ".text.stage3p.direct.gfx_selector", 0x001AC838, 0x15C,
    ".data.stage3p.direct.gfx", 0x00344D30, 0x330,
    ".rodata.stage3p.direct.gfx", 0x001B46B8, 0x1CE0,
)

DIRECT_CPUOPS_OBJECT = (
    "build/matching/hunt1000plus-v46-closure/snes/CPUOPS.o",
    ".text.stage3p.direct.cpuops", 0x00116740, 0x133B4,
    ".text.stage3p.direct.cpuops_shutdown", 0x001AC604, 0x130,
    ".data.stage3p.direct.cpuops", 0x003367F8, 0x518C,
)

DIRECT_TILE_OBJECT = (
    "build/matching/hunt1000plus-v47-closure/snes/tile.o",
    0x00183E04, 0x8320,
    0x004238A8, 0x08AC,
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
    relocation_targets: dict[str, int],
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
    relocation_rules = "\n".join(
        f"  {name} = 0x{address:08x};"
        for name, address in sorted(relocation_targets.items())
    )
    new = f"""  stage3p_target_per_rom_cleanup = 0x00151360;
  stage3p_target_snes_memory_helper = 0x00150f54;
{relocation_rules}
  .text.stage3p.symbols 0x00100114 (NOLOAD) : {{ *(.text) }}
  .rodata.stage3p.symbols 0x0016d5d0 (NOLOAD) : {{ *(.rodata) }}
{code_rules_text}
  /DISCARD/ : {{ *(.text.stage3p.residual.001ab4e4) }}
  .data.stage3p.symbols 0x00170148 (NOLOAD) : {{ *(.data) }}
  .bss.stage3p.symbols 0x001702c0 (NOLOAD) : {{ *(.bss) *(.bss.stage3.compatibility) }}
"""
    if text.count(old) != 1:
        fail("Stage-3O linker-script core drift")
    updated = text.replace(old, new)
    old_sai2 = "  .data.stage3n.source.sai2_data_and_cfi 0x00335284 : { KEEP(*(.data.stage3n.source.sai2_data_and_cfi)) }"
    new_sai2 = "  .data.stage3p.direct.sai2 0x00335284 : { KEEP(*(.data.stage3p.direct.sai2)) }\n  /DISCARD/ : { *(.data.stage3n.source.sai2_data_and_cfi) }"
    if updated.count(old_sai2) != 1:
        fail("historical SAI2 data-provider rule drift")
    updated = updated.replace(old_sai2, new_sai2)
    old_fxemu = "  .data.stage3l.source.fxemu_data 0x00342a38 : { KEEP(*(.data.stage3l.source.fxemu_data)) }"
    new_fxemu = "  .data.stage3p.direct.fxemu 0x00342a38 : { KEEP(*(.data.stage3p.direct.fxemu)) }\n  /DISCARD/ : { *(.data.stage3l.source.fxemu_data) }"
    if updated.count(old_fxemu) != 1:
        fail("historical FXEMU data-provider rule drift")
    updated = updated.replace(old_fxemu, new_fxemu)
    old_fxinst_data = "  .data.stage3l.source.fxinst_data 0x00343320 : { KEEP(*(.data.stage3l.source.fxinst_data)) }"
    new_fxinst_data = "  .data.stage3p.direct.fxinst 0x00343320 : { KEEP(*(.data.stage3p.direct.fxinst)) }\n  /DISCARD/ : { *(.data.stage3l.source.fxinst_data) }"
    old_fxinst_rodata = "  .data.stage3o.source.fxinst 0x001b4678 : { KEEP(*(.data.stage3o.source.fxinst)) }"
    new_fxinst_rodata = "  .rodata.stage3p.direct.fxinst 0x001b4678 : { KEEP(*(.rodata.stage3p.direct.fxinst)) }\n  /DISCARD/ : { *(.data.stage3o.source.fxinst) }"
    if updated.count(old_fxinst_data) != 1 or updated.count(old_fxinst_rodata) != 1:
        fail("historical FXINST data-provider rule drift")
    updated = updated.replace(old_fxinst_data, new_fxinst_data).replace(
        old_fxinst_rodata, new_fxinst_rodata
    )
    old_sa1cpu = "  .data.stage3i.source.sa1cpu 0x003f5040 : { KEEP(*(.data.stage3i.source.sa1cpu)) }"
    new_sa1cpu = "  .data.stage3p.direct.sa1cpu 0x003f5040 : { KEEP(*(.data.stage3p.direct.sa1cpu)) }\n  /DISCARD/ : { *(.data.stage3i.source.sa1cpu) }"
    if updated.count(old_sa1cpu) != 1:
        fail("historical SA1CPU data-provider rule drift")
    updated = updated.replace(old_sa1cpu, new_sa1cpu)
    old_ppu_data = "  .data.stage3i.source.ppu 0x003f4bf0 : { KEEP(*(.data.stage3i.source.ppu)) }"
    new_ppu_data = "  .data.stage3p.direct.ppu 0x003f4bf0 : { KEEP(*(.data.stage3p.direct.ppu)) }\n  /DISCARD/ : { *(.data.stage3i.source.ppu) }"
    old_ppu_rodata = "  .data.stage3o.source.ppu 0x001b7318 : { KEEP(*(.data.stage3o.source.ppu)) }"
    new_ppu_rodata = "  .rodata.stage3p.direct.ppu 0x001b7318 : { KEEP(*(.rodata.stage3p.direct.ppu)) }\n  .bss.stage3p.direct.ppu 0x0042e888 (NOLOAD) : { *(.bss.stage3p.direct.ppu) }\n  /DISCARD/ : { *(.data.stage3o.source.ppu) }"
    if updated.count(old_ppu_data) != 1 or updated.count(old_ppu_rodata) != 1:
        fail("historical PPU data-provider rule drift")
    updated = updated.replace(old_ppu_data, new_ppu_data).replace(
        old_ppu_rodata, new_ppu_rodata
    )
    old_dma_rodata = "  .data.stage3o.source.dma_dispatch 0x001b1f20 : { KEEP(*(.data.stage3o.source.dma_dispatch)) }"
    new_dma_rodata = "  .rodata.stage3p.direct.dma 0x001b1f20 : { KEEP(*(.rodata.stage3p.direct.dma)) }\n  /DISCARD/ : { *(.data.stage3o.source.dma_dispatch) *(.data.stage3p.unlinked.dma) }"
    if updated.count(old_dma_rodata) != 1:
        fail("historical DMA read-only provider rule drift")
    updated = updated.replace(old_dma_rodata, new_dma_rodata)
    old_cpuexec = "  .data.stage3n.source.cpuexec_cfi 0x00336784 : { KEEP(*(.data.stage3n.source.cpuexec_cfi)) }"
    new_cpuexec = "  .data.stage3p.direct.cpuexec 0x00336784 : { KEEP(*(.data.stage3p.direct.cpuexec)) }\n  /DISCARD/ : { *(.data.stage3n.source.cpuexec_cfi) }"
    if updated.count(old_cpuexec) != 1:
        fail("historical CPUEXEC data-provider rule drift")
    updated = updated.replace(old_cpuexec, new_cpuexec)
    old_gfx_data = "  .data.stage3ce.va_00344d30 0x00344d30 : { KEEP(*(.data.stage3ce.va_00344d30)) }"
    new_gfx_data = "  PTR_s_________________00344d30 = 0x00344d30;\n  DAT_00344e08 = 0x00344e08;\n  DAT_00344e0c = 0x00344e0c;\n  .data.stage3p.direct.gfx 0x00344d30 : { KEEP(*(.data.stage3p.direct.gfx)) }\n  /DISCARD/ : { *(.data.stage3ce.va_00344d30) *(.data.stage3f.va_00344e08) *(.data.stage3l.gfx_unwind) }"
    old_gfx_rodata = "  .data.stage3o.source.gfx 0x001b46b8 : { KEEP(*(.data.stage3o.source.gfx)) }"
    new_gfx_rodata = "  .rodata.stage3p.direct.gfx 0x001b46b8 : { KEEP(*(.rodata.stage3p.direct.gfx)) }\n  /DISCARD/ : { *(.data.stage3o.source.gfx) }"
    if updated.count(old_gfx_data) != 1 or updated.count(old_gfx_rodata) != 1:
        fail("historical GFX provider rule drift")
    updated = updated.replace(old_gfx_data, new_gfx_data).replace(
        old_gfx_rodata, new_gfx_rodata
    )
    old_cpuops = "  .data.stage3n.source.cpuops_tables_and_cfi 0x003367f8 : { KEEP(*(.data.stage3n.source.cpuops_tables_and_cfi)) }"
    new_cpuops = "  .data.stage3p.direct.cpuops 0x003367f8 : { KEEP(*(.data.stage3p.direct.cpuops)) }\n  /DISCARD/ : { *(.data.stage3n.source.cpuops_tables_and_cfi) }"
    if updated.count(old_cpuops) != 1:
        fail("historical CPUOPS data-provider rule drift")
    updated = updated.replace(old_cpuops, new_cpuops)
    old_tile = "  .data.stage3j.tile_unwind 0x004238a8 : { KEEP(*(.data.stage3j.tile_unwind)) }"
    new_tile = "  .data.stage3p.direct.tile 0x004238a8 : { KEEP(*(.data.stage3p.direct.tile)) }\n  /DISCARD/ : { *(.data.stage3j.tile_unwind) }"
    if updated.count(old_tile) != 1:
        fail("historical TILE unwind-provider rule drift")
    return updated.replace(old_tile, new_tile)


def link_historical_tile(
    selected: list[dict], reference: bytes, build_dir: Path, objcopy: Path,
    relocation_targets: dict[str, int],
) -> tuple[Path, list[tuple[str, int, int, None]]]:
    """Place the original TILE.CPP code, inline renderers, and unwind data."""
    source, code_address, code_size, data_address, data_size = DIRECT_TILE_OBJECT
    tile = ELFFile(ROOT / source)
    sections = {section.name: section for section in tile.sections}
    rows = [row for row in selected if row.get("object") == source]
    inline = [section for section in tile.sections
              if section.name.startswith(".gnu.linkonce.t.")]
    selected_inline = {
        section.name: row["address"]
        for section in inline
        for row in rows
        if section.name.removeprefix(".gnu.linkonce.t.") == row["symbol"]
        and section.size == row["size"]
    }
    if (tile.elf_class != 1 or tile.endian != "<" or len(rows) != 51
            or len(inline) != 24 or len(selected_inline) != 20
            or sections[".text"].size != code_size
            or sections[".data"].size != data_size
            or sections[".rel.text"].size != 1180 * 8
            or sections[".rel.data"].size != 31 * 8
            or sum(row["size"] for row in rows
                   if row["address"] < code_address + code_size) != code_size):
        fail("historical TILE object or source roster drift")
    placements = {".text": (code_address, code_size)}
    placements.update((name, (address, sections[name].size))
                      for name, address in selected_inline.items())
    excluded = {section.name for section in inline} - set(selected_inline)
    if len(excluded) != 4:
        fail("historical TILE inline-section boundary drift")
    externals: dict[str, int] = {}
    pending: dict[int, list[tuple[str, int]]] = {}
    unpaired_low: set[str] = set()

    def record(name: str, value: int) -> None:
        value &= 0xFFFFFFFF
        if name in externals and externals[name] != value:
            fail(f"historical TILE relocation target ambiguity: {name}")
        externals[name] = value

    for section_name, (address, size) in placements.items():
        section = sections[section_name]
        raw = tile.data[section.offset:section.offset + size]
        expected = reference[address - TARGET_BASE:address - TARGET_BASE + size]
        masked = bytearray(raw)
        relocations = sections[".rel" + section_name]
        for index in range(relocations.size // 8):
            offset, info = struct.unpack_from(
                "<II", tile.data, relocations.offset + index * 8
            )
            symbol_index, kind = info >> 8, info & 0xFF
            if (offset + 4 > size or kind not in (4, 5, 6)
                    or symbol_index >= len(tile.symbols)):
                fail("historical TILE code-relocation drift")
            symbol = tile.symbols[symbol_index]
            target_section = tile.sections[symbol.section_index].name
            if symbol.section_index == 0 or target_section in excluded:
                if not symbol.name:
                    fail("historical TILE unnamed external relocation")
                raw_word = struct.unpack_from("<I", raw, offset)[0]
                target_word = struct.unpack_from("<I", expected, offset)[0]
                if kind == 4:
                    if raw_word & 0xFC000000 != target_word & 0xFC000000:
                        fail("historical TILE call opcode drift")
                    record(symbol.name, ((target_word & 0x03FFFFFF)
                                         - (raw_word & 0x03FFFFFF)) << 2)
                elif kind == 5:
                    pending.setdefault(symbol_index, []).append((section_name, offset))
                else:
                    highs = pending.pop(symbol_index, [])
                    if not highs:
                        unpaired_low.add(symbol.name)
                    for high_name, high_offset in highs:
                        high_section = sections[high_name]
                        raw_high = struct.unpack_from(
                            "<I", tile.data, high_section.offset + high_offset
                        )[0] & 0xFFFF
                        high_address = placements[high_name][0]
                        target_high = struct.unpack_from(
                            "<I", reference,
                            high_address - TARGET_BASE + high_offset,
                        )[0] & 0xFFFF
                        def signed(value: int) -> int:
                            return (value & 0xFFFF) - ((value & 0x8000) << 1)
                        record(symbol.name, (target_high << 16) + signed(target_word)
                               - (raw_high << 16) - signed(raw_word))
            elif target_section not in placements:
                fail("historical TILE unsupported local relocation")
            masked[offset:offset + 4] = expected[offset:offset + 4]
        if masked != expected:
            fail(f"historical TILE {section_name} differs outside relocations")
    if pending or not unpaired_low.issubset(externals):
        fail("historical TILE unpaired external relocation")
    data = sections[".data"]
    raw = tile.data[data.offset:data.offset + data_size]
    expected = reference[data_address - TARGET_BASE:data_address - TARGET_BASE + data_size]
    masked = bytearray(raw)
    rel = sections[".rel.data"]
    for index in range(31):
        offset, info = struct.unpack_from("<II", tile.data, rel.offset + index * 8)
        symbol = tile.symbols[info >> 8]
        if offset + 4 > data_size or info & 0xFF != 2:
            fail("historical TILE data-relocation drift")
        addend = struct.unpack_from("<I", raw, offset)[0]
        actual = struct.unpack_from("<I", expected, offset)[0]
        if symbol.section_index == sections[".text"].index:
            if actual != code_address + addend:
                fail("historical TILE unwind code-pointer drift")
        elif symbol.section_index == 0 and symbol.name == "__gxx_personality_v0":
            record(symbol.name, actual - addend)
        else:
            fail("historical TILE unsupported data relocation")
        masked[offset:offset + 4] = expected[offset:offset + 4]
    if masked != expected:
        fail("historical TILE unwind differs outside relocations")

    # Four original inline functions belong to a later image corridor. Keep
    # their already proved provider there and turn calls to them into external
    # relocations in a derived object; the historical candidate stays intact.
    derived = bytearray(tile.data)
    symbol_table = sections[".symtab"]
    excluded_symbols = set()
    for index, symbol in enumerate(tile.symbols):
        if symbol.section_index < len(tile.sections) and tile.sections[symbol.section_index].name in excluded:
            if not symbol.name:
                continue
            if symbol.name not in externals:
                fail("historical TILE excluded inline target not proved")
            struct.pack_into("<H", derived, symbol_table.offset + index * 16 + 14, 0)
            excluded_symbols.add(symbol.name)
    if len(excluded_symbols) != 4:
        fail("historical TILE excluded-symbol roster drift")
    source_copy = build_dir / "stage3p-tile-selected-source.o"
    source_copy.write_bytes(derived)
    prefix = ".text.stage3p.direct.tile"
    command = [objcopy, "--rename-section", f".text={prefix}.{code_address:08x}",
               "--rename-section", ".data=.data.stage3p.direct.tile"]
    for name, address in sorted(selected_inline.items(), key=lambda item: item[1]):
        command.extend(("--rename-section", f"{name}={prefix}.{address:08x}"))
    for name in sorted(excluded):
        command.extend(("--remove-section", name))
    for symbol, destination in sorted(externals.items()):
        alias = f"stage3p_tile_target_{symbol}"
        command.extend(("--redefine-sym", f"{symbol}={alias}"))
        relocation_targets[alias] = destination
    for index, entry in enumerate(tile.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index < len(tile.sections)
                and tile.sections[entry.section_index].name in placements):
            command.extend(("--redefine-sym", f"{entry.name}=stage3p_tile_direct_{index}"))
    output = build_dir / "stage3p-tile-direct.o"
    run([*command, source_copy, output])
    direct_sections = [
        (prefix, address, size, None)
        for address, size in sorted(placements.values())
    ]
    return output, direct_sections


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
    relocation_targets: dict[str, int] = {}
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
    for key, source_relative, section_prefix, address, size, symbol, reloc_offset in DIRECT_SELF_RELOC_OBJECTS:
        source = ROOT / source_relative
        historical = ELFFile(source)
        sections = [section for section in historical.sections if section.name == ".text"]
        if len(sections) != 1 or sections[0].size != size:
            fail(f"historical self-reloc section drift: {source_relative}")
        section = sections[0]
        symbols = [
            index for index, entry in enumerate(historical.symbols)
            if entry.name == symbol and entry.section_index == section.index
            and entry.value == 0 and entry.size == size
        ]
        relocations = [
            entry for entry in historical.sections
            if entry.name in (".rel.text", ".rela.text")
        ]
        if len(symbols) != 1 or len(relocations) != 1 or relocations[0].size != 8:
            fail(f"historical self-reloc structure drift: {source_relative}")
        relocation = relocations[0]
        offset, info = struct.unpack_from("<II", historical.data, relocation.offset)
        if offset != reloc_offset or info != (symbols[0] << 8) | 4:
            fail(f"historical self-call relocation drift: {source_relative}")
        candidate = historical.data[section.offset:section.offset + size]
        target = reference[address - TARGET_BASE:address - TARGET_BASE + size]
        if (candidate[:offset] != target[:offset]
                or candidate[offset + 4:] != target[offset + 4:]
                or struct.unpack_from("<I", candidate, offset)[0] != 0x0C000000
                or struct.unpack_from("<I", target, offset)[0] !=
                (0x0C000000 | ((address >> 2) & 0x03FFFFFF))):
            fail(f"historical self-reloc object differs from target: {source_relative}")
        output = args.build_dir / f"stage3p-{key}-direct.o"
        run([
            objcopy,
            "--rename-section", f".text={section_prefix}.{address:08x}",
            "--redefine-sym", f"{symbol}=stage3p_{key}_direct",
            source, output,
        ])
        direct_objects.append(output)
        direct_sections.append((section_prefix, address, size, None))
    for key, source_relative, section_prefix, address, size, symbol, reloc_count in DIRECT_CALL_OBJECTS:
        source = ROOT / source_relative
        historical = ELFFile(source)
        sections = [entry for entry in historical.sections if entry.name == ".text"]
        relocations = [entry for entry in historical.sections if entry.name == ".rel.text"]
        if (len(sections) != 1 or sections[0].size != size
                or len(relocations) != 1 or relocations[0].size != reloc_count * 8):
            fail(f"historical call-object layout drift: {source_relative}")
        section, relocation = sections[0], relocations[0]
        if not any(
            entry.name == symbol and entry.section_index == section.index
            and entry.value == 0 and entry.size == size
            for entry in historical.symbols
        ):
            fail(f"historical call-object symbol drift: {source_relative}")
        candidate = historical.data[section.offset:section.offset + size]
        target = reference[address - TARGET_BASE:address - TARGET_BASE + size]
        if len(target) != size:
            fail(f"historical call-object target range drift: {source_relative}")
        masked = bytearray(candidate)
        expected = bytearray(target)
        aliases: dict[str, int] = {}
        for index in range(reloc_count):
            offset, info = struct.unpack_from(
                "<II", historical.data, relocation.offset + index * 8
            )
            symbol_index, kind = info >> 8, info & 0xFF
            if (kind != 4 or offset % 4 or offset + 4 > size
                    or symbol_index >= len(historical.symbols)):
                fail(f"historical call-object relocation drift: {source_relative}")
            callee = historical.symbols[symbol_index]
            if not callee.name or callee.section_index != 0:
                fail(f"historical call-object callee drift: {source_relative}")
            if struct.unpack_from("<I", candidate, offset)[0] != 0x0C000000:
                fail(f"historical call-object instruction drift: {source_relative}")
            target_word = struct.unpack_from("<I", target, offset)[0]
            if target_word & 0xFC000000 != 0x0C000000:
                fail(f"historical call-object target call drift: {source_relative}")
            destination = (target_word & 0x03FFFFFF) << 2
            if callee.name in aliases and aliases[callee.name] != destination:
                fail(f"historical call-object target ambiguity: {source_relative}")
            aliases[callee.name] = destination
            masked[offset:offset + 4] = expected[offset:offset + 4]
        if masked != expected:
            fail(f"historical call-object differs outside relocations: {source_relative}")
        output = args.build_dir / f"stage3p-{key}-direct.o"
        command = [
            objcopy, "--rename-section", f".text={section_prefix}.{address:08x}",
            "--redefine-sym", f"{symbol}=stage3p_{key.replace('-', '_')}_direct",
        ]
        for callee, destination in sorted(aliases.items()):
            alias = f"stage3p_{key.replace('-', '_')}_target_{callee}"
            command.extend(("--redefine-sym", f"{callee}={alias}"))
            relocation_targets[alias] = destination
        run([*command, source, output])
        direct_objects.append(output)
        direct_sections.append((section_prefix, address, size, None))
    (sai2_source, sai2_section, sai2_address, sai2_size,
     sai2_data_section, sai2_data_address, sai2_data_size) = DIRECT_SAI2_OBJECT
    historical = ELFFile(ROOT / sai2_source)
    indexed = {entry.name: entry for entry in historical.sections}
    if (indexed[".text"].size != sai2_size
            or indexed[".data"].size != sai2_data_size
            or indexed[".rel.text"].size != 195 * 8
            or indexed[".rel.data"].size != 4 * 8):
        fail("historical SAI2 object layout drift")
    code = historical.data[indexed[".text"].offset:indexed[".text"].offset + sai2_size]
    data = historical.data[indexed[".data"].offset:indexed[".data"].offset + sai2_data_size]
    expected_code = reference[sai2_address - TARGET_BASE:sai2_address - TARGET_BASE + sai2_size]
    expected_data = reference[sai2_data_address - TARGET_BASE:sai2_data_address - TARGET_BASE + sai2_data_size]
    code_masked, data_masked = bytearray(code), bytearray(data)
    muldi3_destinations: set[int] = set()
    for index in range(195):
        offset, info = struct.unpack_from(
            "<II", historical.data, indexed[".rel.text"].offset + index * 8
        )
        kind, symbol_index = info & 0xFF, info >> 8
        if (offset + 4 > sai2_size or kind not in (4, 5, 6)
                or symbol_index >= len(historical.symbols)):
            fail("historical SAI2 code-relocation drift")
        entry = historical.symbols[symbol_index]
        if entry.name == "__muldi3" and kind == 4:
            target_word = struct.unpack_from("<I", expected_code, offset)[0]
            if (struct.unpack_from("<I", code, offset)[0] != 0x0C000000
                    or target_word & 0xFC000000 != 0x0C000000):
                fail("historical SAI2 external-call drift")
            muldi3_destinations.add((target_word & 0x03FFFFFF) << 2)
        elif not (not entry.name and entry.section_index in
                  (indexed[".text"].index, indexed[".data"].index)):
            fail("historical SAI2 internal-relocation drift")
        code_masked[offset:offset + 4] = expected_code[offset:offset + 4]
    if len(muldi3_destinations) != 1 or code_masked != expected_code:
        fail("historical SAI2 code differs outside relocations")
    for index in range(4):
        offset, info = struct.unpack_from(
            "<II", historical.data, indexed[".rel.data"].offset + index * 8
        )
        entry = historical.symbols[info >> 8]
        if offset + 4 > sai2_data_size or info & 0xFF != 2:
            fail("historical SAI2 data-relocation drift")
        if index == 0:
            if entry.name != "__gxx_personality_v0" or entry.section_index != 0:
                fail("historical SAI2 personality drift")
            relocation_targets["stage3p_sai2_target_personality"] = struct.unpack_from(
                "<I", expected_data, offset
            )[0]
        elif (entry.name or entry.section_index != indexed[".text"].index
              or struct.unpack_from("<I", expected_data, offset)[0] !=
              sai2_address + struct.unpack_from("<I", data, offset)[0]):
            fail("historical SAI2 CFI relocation drift")
        data_masked[offset:offset + 4] = expected_data[offset:offset + 4]
    if data_masked != expected_data:
        fail("historical SAI2 data differs outside relocations")
    relocation_targets["stage3p_sai2_target_muldi3"] = muldi3_destinations.pop()
    sai2_output = args.build_dir / "stage3p-sai2-direct.o"
    sai2_command = [
        objcopy,
        "--rename-section", f".text={sai2_section}.{sai2_address:08x}",
        "--rename-section", f".data={sai2_data_section}",
        "--redefine-sym", "__muldi3=stage3p_sai2_target_muldi3",
        "--redefine-sym", "__gxx_personality_v0=stage3p_sai2_target_personality",
    ]
    for index, entry in enumerate(historical.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index in (indexed[".text"].index, indexed[".data"].index)):
            sai2_command.extend(("--redefine-sym", f"{entry.name}=stage3p_sai2_direct_{index}"))
    run([*sai2_command, ROOT / sai2_source, sai2_output])
    direct_objects.append(sai2_output)
    direct_sections.append((sai2_section, sai2_address, sai2_size, None))
    (fxemu_source, fxemu_section, fxemu_address, fxemu_size,
     fxemu_data_section, fxemu_data_address, fxemu_data_size) = DIRECT_FXEMU_OBJECT
    fxemu = ELFFile(ROOT / fxemu_source)
    fx_sections = {entry.name: entry for entry in fxemu.sections}
    if (fx_sections[".text"].size != fxemu_size
            or fx_sections[".data"].size != fxemu_data_size
            or fx_sections[".rel.text"].size != 168 * 8
            or fx_sections[".rel.data"].size != 8 * 8):
        fail("historical FXEMU object layout drift")
    fx_code = fxemu.data[fx_sections[".text"].offset:fx_sections[".text"].offset + fxemu_size]
    fx_data = fxemu.data[fx_sections[".data"].offset:fx_sections[".data"].offset + fxemu_data_size]
    fx_code_target = reference[fxemu_address - TARGET_BASE:fxemu_address - TARGET_BASE + fxemu_size]
    fx_data_target = reference[fxemu_data_address - TARGET_BASE:fxemu_data_address - TARGET_BASE + fxemu_data_size]
    fx_code_masked, fx_data_masked = bytearray(fx_code), bytearray(fx_data)
    fx_external: dict[str, int] = {}
    for index in range(8):
        offset, info = struct.unpack_from(
            "<II", fxemu.data, fx_sections[".rel.data"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if offset + 4 > fxemu_data_size or kind != 2 or symbol_index >= len(fxemu.symbols):
            fail("historical FXEMU data-relocation drift")
        entry = fxemu.symbols[symbol_index]
        actual = struct.unpack_from("<I", fx_data_target, offset)[0]
        if entry.section_index == 0 and entry.name:
            if entry.name in fx_external and fx_external[entry.name] != actual:
                fail("historical FXEMU external-data ambiguity")
            fx_external[entry.name] = actual
        elif (entry.section_index != fx_sections[".text"].index or entry.name
              or actual != fxemu_address + struct.unpack_from("<I", fx_data, offset)[0]):
            fail("historical FXEMU local-data relocation drift")
        fx_data_masked[offset:offset + 4] = fx_data_target[offset:offset + 4]
    if fx_data_masked != fx_data_target:
        fail("historical FXEMU data differs outside relocations")
    for index in range(168):
        offset, info = struct.unpack_from(
            "<II", fxemu.data, fx_sections[".rel.text"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (offset + 4 > fxemu_size or kind not in (4, 5, 6)
                or symbol_index >= len(fxemu.symbols)):
            fail("historical FXEMU code-relocation drift")
        entry = fxemu.symbols[symbol_index]
        if entry.name == "memset" and entry.section_index == 0 and kind == 4:
            word = struct.unpack_from("<I", fx_code_target, offset)[0]
            if (struct.unpack_from("<I", fx_code, offset)[0] != 0x0C000000
                    or word & 0xFC000000 != 0x0C000000):
                fail("historical FXEMU memset relocation drift")
            destination = (word & 0x03FFFFFF) << 2
            if "memset" in fx_external and fx_external["memset"] != destination:
                fail("historical FXEMU memset target ambiguity")
            fx_external["memset"] = destination
        elif not (entry.section_index in (fx_sections[".text"].index,
                                          fx_sections[".data"].index)
                  or entry.name in fx_external):
            fail("historical FXEMU source-relocation drift")
        fx_code_masked[offset:offset + 4] = fx_code_target[offset:offset + 4]
    if fx_code_masked != fx_code_target:
        fail("historical FXEMU code differs outside relocations")
    fx_output = args.build_dir / "stage3p-fxemu-direct.o"
    fx_command = [
        objcopy,
        "--rename-section", f".text={fxemu_section}.{fxemu_address:08x}",
        "--rename-section", f".data={fxemu_data_section}",
    ]
    for symbol, destination in sorted(fx_external.items()):
        alias = f"stage3p_fxemu_target_{symbol}"
        fx_command.extend(("--redefine-sym", f"{symbol}={alias}"))
        relocation_targets[alias] = destination
    for index, entry in enumerate(fxemu.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index in (fx_sections[".text"].index,
                                            fx_sections[".data"].index)):
            fx_command.extend(("--redefine-sym", f"{entry.name}=stage3p_fxemu_direct_{index}"))
    run([*fx_command, ROOT / fxemu_source, fx_output])
    direct_objects.append(fx_output)
    direct_sections.append((fxemu_section, fxemu_address, fxemu_size, None))
    (fxinst_source, fxinst_section, fxinst_address, fxinst_size,
     fxinst_data_section, fxinst_data_address, fxinst_data_size,
     fxinst_rodata_section, fxinst_rodata_address, fxinst_rodata_size) = DIRECT_FXINST_OBJECT
    fxinst = ELFFile(ROOT / fxinst_source)
    fi_sections = {entry.name: entry for entry in fxinst.sections}
    if (fi_sections[".text"].size != fxinst_size
            or fi_sections[".data"].size != fxinst_data_size
            or fi_sections[".rodata"].size != fxinst_rodata_size
            or fi_sections[".rel.text"].size != 1329 * 8
            or fi_sections[".rel.data"].size != 1049 * 8):
        fail("historical FXINST object layout drift")
    fi_code = fxinst.data[fi_sections[".text"].offset:fi_sections[".text"].offset + fxinst_size]
    fi_data = fxinst.data[fi_sections[".data"].offset:fi_sections[".data"].offset + fxinst_data_size]
    fi_rodata = fxinst.data[fi_sections[".rodata"].offset:fi_sections[".rodata"].offset + fxinst_rodata_size]
    fi_target = reference[fxinst_address - TARGET_BASE:fxinst_address - TARGET_BASE + fxinst_size]
    fi_data_target = reference[fxinst_data_address - TARGET_BASE:fxinst_data_address - TARGET_BASE + fxinst_data_size]
    if fi_rodata != reference[
        fxinst_rodata_address - TARGET_BASE:fxinst_rodata_address - TARGET_BASE + fxinst_rodata_size
    ]:
        fail("historical FXINST rodata differs from target")
    fi_code_masked, fi_data_masked = bytearray(fi_code), bytearray(fi_data)
    fi_external = {
        "GSU": fxemu_data_address + next(
            entry.value for entry in fxemu.symbols if entry.name == "GSU"
        ),
        "fx_ppfOpcodeTable": fxemu_data_address + next(
            entry.value for entry in fxemu.symbols if entry.name == "fx_ppfOpcodeTable"
        ),
    }
    for index in range(1049):
        offset, info = struct.unpack_from(
            "<II", fxinst.data, fi_sections[".rel.data"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if offset + 4 > fxinst_data_size or kind != 2 or symbol_index >= len(fxinst.symbols):
            fail("historical FXINST data-relocation drift")
        symbol = fxinst.symbols[symbol_index]
        actual = struct.unpack_from("<I", fi_data_target, offset)[0]
        if symbol.name == "__gxx_personality_v0" and symbol.section_index == 0:
            fi_external[symbol.name] = actual
        elif (symbol.name or symbol.section_index != fi_sections[".text"].index
              or actual != fxinst_address + struct.unpack_from("<I", fi_data, offset)[0]):
            fail("historical FXINST CFI relocation drift")
        fi_data_masked[offset:offset + 4] = fi_data_target[offset:offset + 4]
    if fi_data_masked != fi_data_target:
        fail("historical FXINST data differs outside relocations")
    for index in range(1329):
        offset, info = struct.unpack_from(
            "<II", fxinst.data, fi_sections[".rel.text"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (offset + 4 > fxinst_size or kind not in (4, 5, 6)
                or symbol_index >= len(fxinst.symbols)):
            fail("historical FXINST code-relocation drift")
        symbol = fxinst.symbols[symbol_index]
        if symbol.section_index == 0 and symbol.name not in fi_external:
            if kind != 4 or symbol.name not in (
                "puts", "_Z13fx_flushCachev", "_Z24fx_computeScreenPointersv"
            ):
                fail("historical FXINST external source drift")
            raw_word = struct.unpack_from("<I", fi_code, offset)[0]
            target_word = struct.unpack_from("<I", fi_target, offset)[0]
            if raw_word != 0x0C000000 or target_word & 0xFC000000 != 0x0C000000:
                fail("historical FXINST external call drift")
            fi_external[symbol.name] = (target_word & 0x03FFFFFF) << 2
        elif (symbol.section_index == 0 and symbol.name in fi_external
              and kind == 4):
            destination = (struct.unpack_from("<I", fi_target, offset)[0]
                           & 0x03FFFFFF) << 2
            if destination != fi_external[symbol.name]:
                fail("historical FXINST call target ambiguity")
        elif not (symbol.name in fi_external
                  or symbol.section_index in (fi_sections[".text"].index,
                                              fi_sections[".rodata"].index)):
            fail("historical FXINST internal source drift")
        fi_code_masked[offset:offset + 4] = fi_target[offset:offset + 4]
    if fi_code_masked != fi_target:
        fail("historical FXINST code differs outside relocations")
    fi_output = args.build_dir / "stage3p-fxinst-direct.o"
    fi_command = [
        objcopy, "--rename-section", f".text={fxinst_section}.{fxinst_address:08x}",
        "--rename-section", f".data={fxinst_data_section}",
        "--rename-section", f".rodata={fxinst_rodata_section}",
    ]
    for symbol, destination in sorted(fi_external.items()):
        alias = f"stage3p_fxinst_target_{symbol}"
        fi_command.extend(("--redefine-sym", f"{symbol}={alias}"))
        relocation_targets[alias] = destination
    for index, entry in enumerate(fxinst.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index in (fi_sections[".text"].index,
                                            fi_sections[".data"].index)):
            fi_command.extend(("--redefine-sym", f"{entry.name}=stage3p_fxinst_direct_{index}"))
    run([*fi_command, ROOT / fxinst_source, fi_output])
    direct_objects.append(fi_output)
    direct_sections.append((fxinst_section, fxinst_address, fxinst_size, None))
    (sa_source, sa_section, sa_address, sa_size,
     sa_data_section, sa_data_address, sa_data_size) = DIRECT_SA1CPU_OBJECT
    sa = ELFFile(ROOT / sa_source)
    sa_sections = {entry.name: entry for entry in sa.sections}
    if (sa_sections[".text"].size != sa_size
            or sa_sections[".data"].size != sa_data_size
            or sa_sections[".rel.text"].size != 5131 * 8
            or sa_sections[".rel.data"].size != 1411 * 8):
        fail("historical SA1CPU object layout drift")
    sa_code = sa.data[sa_sections[".text"].offset:sa_sections[".text"].offset + sa_size]
    sa_data = sa.data[sa_sections[".data"].offset:sa_sections[".data"].offset + sa_data_size]
    sa_target = reference[sa_address - TARGET_BASE:sa_address - TARGET_BASE + sa_size]
    sa_data_target = reference[sa_data_address - TARGET_BASE:sa_data_address - TARGET_BASE + sa_data_size]
    sa_code_masked, sa_data_masked = bytearray(sa_code), bytearray(sa_data)
    sa_external: dict[str, int] = {}
    pending_high: dict[int, list[int]] = {}
    unresolved_low: set[str] = set()

    def signed_short(value: int) -> int:
        value &= 0xFFFF
        return value if value < 0x8000 else value - 0x10000

    def record_sa_target(name: str, value: int) -> None:
        value &= 0xFFFFFFFF
        prior = sa_external.get(name)
        if prior is not None and prior != value:
            fail(f"historical SA1CPU relocation target ambiguity: {name}")
        sa_external[name] = value

    for index in range(5131):
        offset, info = struct.unpack_from(
            "<II", sa.data, sa_sections[".rel.text"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (offset + 4 > sa_size or kind not in (4, 5, 6)
                or symbol_index >= len(sa.symbols)):
            fail("historical SA1CPU code-relocation drift")
        symbol = sa.symbols[symbol_index]
        if symbol.section_index == 0:
            if not symbol.name:
                fail("historical SA1CPU unnamed external relocation")
            raw_word = struct.unpack_from("<I", sa_code, offset)[0]
            target_word = struct.unpack_from("<I", sa_target, offset)[0]
            if kind == 4:
                if raw_word & 0xFC000000 != target_word & 0xFC000000:
                    fail("historical SA1CPU call opcode drift")
                record_sa_target(
                    symbol.name,
                    ((target_word & 0x03FFFFFF) - (raw_word & 0x03FFFFFF)) << 2,
                )
            elif kind == 5:
                pending_high.setdefault(symbol_index, []).append(offset)
            else:
                highs = pending_high.pop(symbol_index, [])
                if not highs:
                    unresolved_low.add(symbol.name)
                for high_offset in highs:
                    raw_high = struct.unpack_from("<I", sa_code, high_offset)[0] & 0xFFFF
                    target_high = struct.unpack_from("<I", sa_target, high_offset)[0] & 0xFFFF
                    record_sa_target(
                        symbol.name,
                        ((target_high << 16) + signed_short(target_word)
                         - (raw_high << 16) - signed_short(raw_word)),
                    )
        elif symbol.section_index not in (sa_sections[".text"].index,
                                         sa_sections[".data"].index):
            fail("historical SA1CPU internal relocation drift")
        sa_code_masked[offset:offset + 4] = sa_target[offset:offset + 4]
    if (pending_high or not unresolved_low.issubset(sa_external)
            or sa_code_masked != sa_target):
        fail("historical SA1CPU code differs outside relocations")
    for index in range(1411):
        offset, info = struct.unpack_from(
            "<II", sa.data, sa_sections[".rel.data"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (offset + 4 > sa_data_size or kind != 2
                or symbol_index >= len(sa.symbols)):
            fail("historical SA1CPU data-relocation drift")
        symbol = sa.symbols[symbol_index]
        actual = struct.unpack_from("<I", sa_data_target, offset)[0]
        if symbol.section_index == 0 and symbol.name == "__gxx_personality_v0":
            record_sa_target(symbol.name, actual - struct.unpack_from("<I", sa_data, offset)[0])
        elif (symbol.section_index != sa_sections[".text"].index
              or actual != sa_address + struct.unpack_from("<I", sa_data, offset)[0]):
            fail("historical SA1CPU CFI relocation drift")
        sa_data_masked[offset:offset + 4] = sa_data_target[offset:offset + 4]
    if sa_data_masked != sa_data_target:
        fail("historical SA1CPU data differs outside relocations")
    sa_output = args.build_dir / "stage3p-sa1cpu-direct.o"
    sa_command = [
        objcopy, "--rename-section", f".text={sa_section}.{sa_address:08x}",
        "--rename-section", f".data={sa_data_section}",
    ]
    for symbol, destination in sorted(sa_external.items()):
        alias = f"stage3p_sa1cpu_target_{symbol}"
        sa_command.extend(("--redefine-sym", f"{symbol}={alias}"))
        relocation_targets[alias] = destination
    for index, entry in enumerate(sa.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index in (sa_sections[".text"].index,
                                            sa_sections[".data"].index)):
            sa_command.extend(("--redefine-sym", f"{entry.name}=stage3p_sa1cpu_direct_{index}"))
    run([*sa_command, ROOT / sa_source, sa_output])
    direct_objects.append(sa_output)
    direct_sections.append((sa_section, sa_address, sa_size, None))
    (ppu_source, ppu_section, ppu_address, ppu_size,
     ppu_data_section, ppu_data_address, ppu_data_size,
     ppu_rodata_section, ppu_rodata_address, ppu_rodata_size,
     ppu_bss_section, ppu_bss_address, ppu_bss_size) = DIRECT_PPU_OBJECT
    ppu = ELFFile(ROOT / ppu_source)
    ppu_sections = {entry.name: entry for entry in ppu.sections}
    if (ppu_sections[".text"].size != ppu_size
            or ppu_sections[".data"].size != ppu_data_size
            or ppu_sections[".rodata"].size != ppu_rodata_size
            or ppu_sections[".bss"].size != ppu_bss_size
            or ppu_sections[".rel.text"].size != 1061 * 8
            or ppu_sections[".rel.data"].size != 14 * 8
            or ppu_sections[".rel.rodata"].size != 688 * 8):
        fail("historical PPU object layout drift")
    ppu_code = ppu.data[ppu_sections[".text"].offset:ppu_sections[".text"].offset + ppu_size]
    ppu_target = reference[ppu_address - TARGET_BASE:ppu_address - TARGET_BASE + ppu_size]
    ppu_code_masked = bytearray(ppu_code)
    ppu_external: dict[str, int] = {}
    ppu_pending_high: dict[int, list[int]] = {}
    ppu_unresolved_low: set[str] = set()
    ppu_bss_targets: set[int] = set()

    def record_ppu_target(name: str, value: int) -> None:
        value &= 0xFFFFFFFF
        prior = ppu_external.get(name)
        if prior is not None and prior != value:
            fail(f"historical PPU relocation target ambiguity: {name}")
        ppu_external[name] = value

    for index in range(1061):
        offset, info = struct.unpack_from(
            "<II", ppu.data, ppu_sections[".rel.text"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (offset + 4 > ppu_size or kind not in (4, 5, 6)
                or symbol_index >= len(ppu.symbols)):
            fail("historical PPU code-relocation drift")
        symbol = ppu.symbols[symbol_index]
        external = symbol.section_index == 0
        local_bss = symbol.section_index == ppu_sections[".bss"].index
        if external or local_bss:
            name = symbol.name if external else "<ppu-bss>"
            if not name:
                fail("historical PPU unnamed external relocation")
            raw_word = struct.unpack_from("<I", ppu_code, offset)[0]
            target_word = struct.unpack_from("<I", ppu_target, offset)[0]
            if kind == 4:
                if raw_word & 0xFC000000 != target_word & 0xFC000000:
                    fail("historical PPU call opcode drift")
                value = ((target_word & 0x03FFFFFF)
                         - (raw_word & 0x03FFFFFF)) << 2
                record_ppu_target(name, value)
            elif kind == 5:
                ppu_pending_high.setdefault(symbol_index, []).append(offset)
            else:
                highs = ppu_pending_high.pop(symbol_index, [])
                if not highs:
                    ppu_unresolved_low.add(name)
                for high_offset in highs:
                    raw_high = struct.unpack_from("<I", ppu_code, high_offset)[0] & 0xFFFF
                    target_high = struct.unpack_from("<I", ppu_target, high_offset)[0] & 0xFFFF
                    record_ppu_target(
                        name,
                        (target_high << 16) + signed_short(target_word)
                        - (raw_high << 16) - signed_short(raw_word),
                    )
            if local_bss and name in ppu_external:
                ppu_bss_targets.add(ppu_external[name])
        elif symbol.section_index not in (
            ppu_sections[".text"].index, ppu_sections[".data"].index,
            ppu_sections[".rodata"].index
        ):
            fail("historical PPU internal relocation drift")
        ppu_code_masked[offset:offset + 4] = ppu_target[offset:offset + 4]
    ppu_external.pop("<ppu-bss>", None)
    if (ppu_pending_high or not ppu_unresolved_low.issubset(
            set(ppu_external) | {"<ppu-bss>"})
            or ppu_bss_targets != {ppu_bss_address}
            or ppu_code_masked != ppu_target):
        fail("historical PPU code differs outside relocations")
    for source_name, address, length, reloc_count in (
        (".data", ppu_data_address, ppu_data_size, 14),
        (".rodata", ppu_rodata_address, ppu_rodata_size, 688),
    ):
        source_section = ppu_sections[source_name]
        content = ppu.data[source_section.offset:source_section.offset + length]
        expected = reference[address - TARGET_BASE:address - TARGET_BASE + length]
        masked = bytearray(content)
        reloc_section = ppu_sections[".rel" + source_name]
        for index in range(reloc_count):
            offset, info = struct.unpack_from(
                "<II", ppu.data, reloc_section.offset + index * 8
            )
            symbol_index, kind = info >> 8, info & 0xFF
            if (offset + 4 > length or kind != 2
                    or symbol_index >= len(ppu.symbols)):
                fail("historical PPU data-relocation drift")
            symbol = ppu.symbols[symbol_index]
            actual = struct.unpack_from("<I", expected, offset)[0]
            if symbol.section_index == 0 and symbol.name == "__gxx_personality_v0":
                record_ppu_target(symbol.name, actual - struct.unpack_from("<I", content, offset)[0])
            elif (symbol.section_index != ppu_sections[".text"].index
                  or actual != ppu_address + struct.unpack_from("<I", content, offset)[0]):
                fail("historical PPU local data relocation drift")
            masked[offset:offset + 4] = expected[offset:offset + 4]
        if masked != expected:
            fail("historical PPU data differs outside relocations")
    ppu_output = args.build_dir / "stage3p-ppu-direct.o"
    ppu_command = [
        objcopy, "--rename-section", f".text={ppu_section}.{ppu_address:08x}",
        "--rename-section", f".data={ppu_data_section}",
        "--rename-section", f".rodata={ppu_rodata_section}",
        "--rename-section", f".bss={ppu_bss_section}",
    ]
    for symbol, destination in sorted(ppu_external.items()):
        alias = f"stage3p_ppu_target_{symbol}"
        ppu_command.extend(("--redefine-sym", f"{symbol}={alias}"))
        relocation_targets[alias] = destination
    for index, entry in enumerate(ppu.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index in (
                    ppu_sections[".text"].index, ppu_sections[".data"].index,
                    ppu_sections[".rodata"].index, ppu_sections[".bss"].index
                )):
            ppu_command.extend(("--redefine-sym", f"{entry.name}=stage3p_ppu_direct_{index}"))
    run([*ppu_command, ROOT / ppu_source, ppu_output])
    direct_objects.append(ppu_output)
    direct_sections.append((ppu_section, ppu_address, ppu_size, None))
    (dma_source, dma_section, dma_address, dma_size,
     dma_rodata_section, dma_rodata_address, dma_rodata_size,
     dma_unlinked_data_section, dma_data_address, dma_data_size) = DIRECT_DMA_OBJECT
    dma = ELFFile(ROOT / dma_source)
    dma_sections = {entry.name: entry for entry in dma.sections}
    if (dma_sections[".text"].size != dma_size
            or dma_sections[".rodata"].size != dma_rodata_size
            or dma_sections[".data"].size != dma_data_size
            or dma_sections[".rel.text"].size != 351 * 8
            or dma_sections[".rel.rodata"].size != 14 * 8
            or dma_sections[".rel.data"].size != 5 * 8):
        fail("historical DMA object layout drift")
    dma_raw = dma.data[dma_sections[".text"].offset:dma_sections[".text"].offset + dma_size]
    dma_target = reference[dma_address - TARGET_BASE:dma_address - TARGET_BASE + dma_size]
    dma_masked = bytearray(dma_raw)
    dma_external: dict[str, int] = {}
    dma_pending: dict[int, list[int]] = {}
    dma_unpaired_low: set[str] = set()

    def record_dma_target(name: str, value: int) -> None:
        value &= 0xFFFFFFFF
        prior = dma_external.get(name)
        if prior is not None and prior != value:
            fail(f"historical DMA relocation target ambiguity: {name}")
        dma_external[name] = value

    for index in range(351):
        offset, info = struct.unpack_from(
            "<II", dma.data, dma_sections[".rel.text"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (offset + 4 > dma_size or kind not in (4, 5, 6)
                or symbol_index >= len(dma.symbols)):
            fail("historical DMA code-relocation drift")
        symbol = dma.symbols[symbol_index]
        if symbol.section_index == 0:
            if not symbol.name:
                fail("historical DMA unnamed external relocation")
            raw_word = struct.unpack_from("<I", dma_raw, offset)[0]
            target_word = struct.unpack_from("<I", dma_target, offset)[0]
            if kind == 4:
                if raw_word & 0xFC000000 != target_word & 0xFC000000:
                    fail("historical DMA call opcode drift")
                record_dma_target(
                    symbol.name,
                    ((target_word & 0x03FFFFFF) - (raw_word & 0x03FFFFFF)) << 2,
                )
            elif kind == 5:
                dma_pending.setdefault(symbol_index, []).append(offset)
            else:
                highs = dma_pending.pop(symbol_index, [])
                if not highs:
                    dma_unpaired_low.add(symbol.name)
                for high_offset in highs:
                    raw_high = struct.unpack_from("<I", dma_raw, high_offset)[0] & 0xFFFF
                    target_high = struct.unpack_from("<I", dma_target, high_offset)[0] & 0xFFFF
                    record_dma_target(
                        symbol.name,
                        (target_high << 16) + signed_short(target_word)
                        - (raw_high << 16) - signed_short(raw_word),
                    )
        elif symbol.section_index not in (
            dma_sections[".text"].index, dma_sections[".rodata"].index
        ):
            fail("historical DMA unsupported internal relocation")
        dma_masked[offset:offset + 4] = dma_target[offset:offset + 4]
    if (dma_pending or not dma_unpaired_low.issubset(dma_external)
            or dma_masked != dma_target):
        fail("historical DMA code differs outside relocations")
    dma_rodata = dma.data[
        dma_sections[".rodata"].offset:dma_sections[".rodata"].offset + dma_rodata_size
    ]
    dma_expected_rodata = reference[
        dma_rodata_address - TARGET_BASE:dma_rodata_address - TARGET_BASE + dma_rodata_size
    ]
    dma_rodata_masked = bytearray(dma_rodata)
    for index in range(14):
        offset, info = struct.unpack_from(
            "<II", dma.data, dma_sections[".rel.rodata"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (kind != 2 or offset + 4 > dma_rodata_size
                or symbol_index >= len(dma.symbols)
                or dma.symbols[symbol_index].section_index != dma_sections[".text"].index
                or struct.unpack_from("<I", dma_expected_rodata, offset)[0]
                != dma_address + struct.unpack_from("<I", dma_rodata, offset)[0]):
            fail("historical DMA read-only relocation drift")
        dma_rodata_masked[offset:offset + 4] = dma_expected_rodata[offset:offset + 4]
    if dma_rodata_masked != dma_expected_rodata:
        fail("historical DMA read-only data differs outside relocations")
    # The original .data has one nonrelocation difference at offset 0x100.
    # Keep the existing proved semantic provider, and discard this .data.
    dma_data = dma.data[
        dma_sections[".data"].offset:dma_sections[".data"].offset + dma_data_size
    ]
    dma_expected_data = reference[
        dma_data_address - TARGET_BASE:dma_data_address - TARGET_BASE + dma_data_size
    ]
    dma_data_reloc_offsets: set[int] = set()
    for index in range(5):
        offset, info = struct.unpack_from(
            "<II", dma.data, dma_sections[".rel.data"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (offset + 4 > dma_data_size or kind != 2
                or symbol_index >= len(dma.symbols)):
            fail("historical DMA discarded-data relocation drift")
        symbol = dma.symbols[symbol_index]
        expected_word = struct.unpack_from("<I", dma_expected_data, offset)[0]
        raw_word = struct.unpack_from("<I", dma_data, offset)[0]
        if symbol.section_index == dma_sections[".text"].index:
            if expected_word != dma_address + raw_word:
                fail("historical DMA discarded-data code pointer drift")
        elif symbol.section_index != 0 or symbol.name != "__gxx_personality_v0":
            fail("historical DMA unexpected discarded-data relocation")
        dma_data_reloc_offsets.add(offset)
    if any(
        dma_data[offset] != dma_expected_data[offset]
        for offset in range(dma_data_size)
        if offset != 0x100
        and all(not start <= offset < start + 4 for start in dma_data_reloc_offsets)
    ) or dma_data[0x100] == dma_expected_data[0x100]:
        fail("historical DMA discarded-data boundary drift")
    dma_output = args.build_dir / "stage3p-dma-direct.o"
    dma_command = [
        objcopy, "--rename-section", f".text={dma_section}.{dma_address:08x}",
        "--rename-section", f".rodata={dma_rodata_section}",
        "--rename-section", f".data={dma_unlinked_data_section}",
    ]
    for symbol, destination in sorted(dma_external.items()):
        alias = f"stage3p_dma_target_{symbol}"
        dma_command.extend(("--redefine-sym", f"{symbol}={alias}"))
        relocation_targets[alias] = destination
    for index, entry in enumerate(dma.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index in (
                    dma_sections[".text"].index, dma_sections[".rodata"].index,
                    dma_sections[".data"].index,
                )):
            dma_command.extend(("--redefine-sym", f"{entry.name}=stage3p_dma_direct_{index}"))
    run([*dma_command, ROOT / dma_source, dma_output])
    direct_objects.append(dma_output)
    direct_sections.append((dma_section, dma_address, dma_size, None))
    (cx_source, cx_section, cx_address, cx_size,
     cx_data_section, cx_data_address, cx_data_size) = DIRECT_CPUEXEC_OBJECT
    cx = ELFFile(ROOT / cx_source)
    cx_sections = {entry.name: entry for entry in cx.sections}
    if (cx_sections[".text"].size != cx_size
            or cx_sections[".data"].size != cx_data_size
            or cx_sections[".rel.text"].size != 201 * 8
            or cx_sections[".rel.data"].size != 3 * 8):
        fail("historical CPUEXEC object layout drift")
    cx_raw = cx.data[cx_sections[".text"].offset:cx_sections[".text"].offset + cx_size]
    cx_target = reference[cx_address - TARGET_BASE:cx_address - TARGET_BASE + cx_size]
    cx_masked = bytearray(cx_raw)
    cx_external: dict[str, int] = {}
    cx_pending: dict[int, list[int]] = {}
    cx_unpaired_low: set[str] = set()

    def record_cpuexec_target(name: str, value: int) -> None:
        value &= 0xFFFFFFFF
        prior = cx_external.get(name)
        if prior is not None and prior != value:
            fail(f"historical CPUEXEC relocation target ambiguity: {name}")
        cx_external[name] = value

    for index in range(201):
        offset, info = struct.unpack_from(
            "<II", cx.data, cx_sections[".rel.text"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (offset + 4 > cx_size or kind not in (4, 5, 6)
                or symbol_index >= len(cx.symbols)):
            fail("historical CPUEXEC code-relocation drift")
        symbol = cx.symbols[symbol_index]
        if symbol.section_index == 0:
            if not symbol.name:
                fail("historical CPUEXEC unnamed external relocation")
            raw_word = struct.unpack_from("<I", cx_raw, offset)[0]
            target_word = struct.unpack_from("<I", cx_target, offset)[0]
            if kind == 4:
                if raw_word & 0xFC000000 != target_word & 0xFC000000:
                    fail("historical CPUEXEC call opcode drift")
                record_cpuexec_target(
                    symbol.name,
                    ((target_word & 0x03FFFFFF) - (raw_word & 0x03FFFFFF)) << 2,
                )
            elif kind == 5:
                cx_pending.setdefault(symbol_index, []).append(offset)
            else:
                highs = cx_pending.pop(symbol_index, [])
                if not highs:
                    cx_unpaired_low.add(symbol.name)
                for high_offset in highs:
                    raw_high = struct.unpack_from("<I", cx_raw, high_offset)[0] & 0xFFFF
                    target_high = struct.unpack_from("<I", cx_target, high_offset)[0] & 0xFFFF
                    record_cpuexec_target(
                        symbol.name,
                        (target_high << 16) + signed_short(target_word)
                        - (raw_high << 16) - signed_short(raw_word),
                    )
        elif symbol.section_index != cx_sections[".text"].index:
            fail("historical CPUEXEC unexpected internal relocation")
        cx_masked[offset:offset + 4] = cx_target[offset:offset + 4]
    if (cx_pending or not cx_unpaired_low.issubset(cx_external)
            or cx_masked != cx_target):
        fail("historical CPUEXEC code differs outside relocations")
    cx_data = cx.data[
        cx_sections[".data"].offset:cx_sections[".data"].offset + cx_data_size
    ]
    cx_expected_data = reference[
        cx_data_address - TARGET_BASE:cx_data_address - TARGET_BASE + cx_data_size
    ]
    cx_data_masked = bytearray(cx_data)
    for index in range(3):
        offset, info = struct.unpack_from(
            "<II", cx.data, cx_sections[".rel.data"].offset + index * 8
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (kind != 2 or offset + 4 > cx_data_size
                or symbol_index >= len(cx.symbols)):
            fail("historical CPUEXEC data-relocation drift")
        symbol = cx.symbols[symbol_index]
        word = struct.unpack_from("<I", cx_expected_data, offset)[0]
        addend = struct.unpack_from("<I", cx_data, offset)[0]
        if symbol.section_index == cx_sections[".text"].index:
            if word != cx_address + addend:
                fail("historical CPUEXEC data code pointer drift")
        elif symbol.section_index == 0 and symbol.name == "__gxx_personality_v0":
            record_cpuexec_target(symbol.name, word - addend)
        else:
            fail("historical CPUEXEC unsupported data relocation")
        cx_data_masked[offset:offset + 4] = cx_expected_data[offset:offset + 4]
    if cx_data_masked != cx_expected_data:
        fail("historical CPUEXEC data differs outside relocations")
    cx_output = args.build_dir / "stage3p-cpuexec-direct.o"
    cx_command = [
        objcopy, "--rename-section", f".text={cx_section}.{cx_address:08x}",
        "--rename-section", f".data={cx_data_section}",
    ]
    for symbol, destination in sorted(cx_external.items()):
        alias = f"stage3p_cpuexec_target_{symbol}"
        cx_command.extend(("--redefine-sym", f"{symbol}={alias}"))
        relocation_targets[alias] = destination
    for index, entry in enumerate(cx.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index in (
                    cx_sections[".text"].index, cx_sections[".data"].index
                )):
            cx_command.extend(("--redefine-sym", f"{entry.name}=stage3p_cpuexec_direct_{index}"))
    run([*cx_command, ROOT / cx_source, cx_output])
    direct_objects.append(cx_output)
    direct_sections.append((cx_section, cx_address, cx_size, None))
    for key, suffix, address, size, reloc_count in DIRECT_EXTERNAL_RELOC_OBJECTS:
        relative = (suffix if suffix.startswith("build/") else
                    f"build/matching/hunt1000plus-v47-closure/{suffix}")
        code_only = key in ("cpu", "seta")
        source_path = ROOT / relative
        if key == "seta":
            # Two local data names occur in code relocations. Redirect them
            # through independently proved absolute addresses while retaining
            # the existing data provider; leave the source ELF untouched.
            original = ELFFile(source_path)
            original_sections = {entry.name: entry for entry in original.sections}
            derived = bytearray(original.data)
            table = original_sections[".symtab"]
            for name, offset in (("SetSETA", 0), ("GetSETA", 4)):
                matches = [index for index, symbol in enumerate(original.symbols)
                           if symbol.name == name
                           and symbol.section_index == original_sections[".data"].index
                           and symbol.value == offset]
                if len(matches) != 1:
                    fail("historical SETA local data-symbol drift")
                position = table.offset + matches[0] * 16
                struct.pack_into("<I", derived, position + 4, 0)
                struct.pack_into("<H", derived, position + 14, 0)
            source_path = args.build_dir / "stage3p-seta-code-source.o"
            source_path.write_bytes(derived)
        candidate = ELFFile(source_path)
        sections = {entry.name: entry for entry in candidate.sections}
        if (sections[".text"].size != size
                or sections[".rel.text"].size != reloc_count * 8
                or sections[".data"].size != (0xCC if key == "cpu" else
                                               0x64 if key == "seta" else 0)
                or (code_only and sections[".rel.data"].size != 5 * 8)
                or sections[".bss"].size != 0):
            fail(f"historical {key} runtime object layout drift")
        raw = candidate.data[sections[".text"].offset:sections[".text"].offset + size]
        expected = reference[address - TARGET_BASE:address - TARGET_BASE + size]
        masked = bytearray(raw)
        external: dict[str, int] = {}
        pending: dict[int, list[int]] = {}
        unpaired_low: set[str] = set()

        def record_runtime_target(name: str, destination: int) -> None:
            destination &= 0xFFFFFFFF
            previous = external.get(name)
            if previous is not None and previous != destination:
                fail(f"historical {key} runtime relocation ambiguity: {name}")
            external[name] = destination

        for index in range(reloc_count):
            offset, info = struct.unpack_from(
                "<II", candidate.data, sections[".rel.text"].offset + index * 8
            )
            symbol_index, kind = info >> 8, info & 0xFF
            if (offset + 4 > size or kind not in (4, 5, 6)
                    or symbol_index >= len(candidate.symbols)):
                fail(f"historical {key} runtime code-relocation drift")
            symbol = candidate.symbols[symbol_index]
            if code_only and kind == 4 and symbol.section_index == sections[".text"].index:
                masked[offset:offset + 4] = expected[offset:offset + 4]
                continue
            if not symbol.name or symbol.section_index != 0:
                fail(f"historical {key} runtime nonexternal relocation")
            raw_word = struct.unpack_from("<I", raw, offset)[0]
            target_word = struct.unpack_from("<I", expected, offset)[0]
            if kind == 4:
                if raw_word & 0xFC000000 != target_word & 0xFC000000:
                    fail(f"historical {key} runtime call opcode drift")
                record_runtime_target(
                    symbol.name,
                    ((target_word & 0x03FFFFFF) - (raw_word & 0x03FFFFFF)) << 2,
                )
            elif kind == 5:
                pending.setdefault(symbol_index, []).append(offset)
            else:
                highs = pending.pop(symbol_index, [])
                if not highs:
                    unpaired_low.add(symbol.name)
                for high_offset in highs:
                    raw_high = struct.unpack_from("<I", raw, high_offset)[0] & 0xFFFF
                    target_high = struct.unpack_from("<I", expected, high_offset)[0] & 0xFFFF
                    record_runtime_target(
                        symbol.name,
                        (target_high << 16) + signed_short(target_word)
                        - (raw_high << 16) - signed_short(raw_word),
                    )
            masked[offset:offset + 4] = expected[offset:offset + 4]
        if pending or not unpaired_low.issubset(external) or masked != expected:
            fail(f"historical {key} runtime code differs outside relocations")
        prefix = f".text.stage3p.direct.{key}"
        output = args.build_dir / f"stage3p-{key}-direct.o"
        command = [
            objcopy, "--rename-section", f".text={prefix}.{address:08x}",
        ]
        if code_only:
            command.extend(("--remove-section", ".data"))
        for symbol, destination in sorted(external.items()):
            alias = f"stage3p_{key}_target_{symbol}"
            command.extend(("--redefine-sym", f"{symbol}={alias}"))
            relocation_targets[alias] = destination
        for index, entry in enumerate(candidate.symbols):
            if (entry.name and entry.info >> 4 == 1
                    and entry.section_index == sections[".text"].index):
                command.extend(("--redefine-sym", f"{entry.name}=stage3p_{key}_direct_{index}"))
        run([*command, source_path, output])
        direct_objects.append(output)
        direct_sections.append((prefix, address, size, None))
    (gfx_source, gfx_section, gfx_address, gfx_size,
     gfx_selector_section, gfx_selector_address, gfx_selector_size,
     gfx_data_section, gfx_data_address, gfx_data_size,
     gfx_rodata_section, gfx_rodata_address, gfx_rodata_size) = DIRECT_GFX_OBJECT
    gfx = ELFFile(ROOT / gfx_source)
    gfx_sections = {entry.name: entry for entry in gfx.sections}
    gfx_selector_name = ".gnu.linkonce.t._Z18SelectTileRendererh"
    gfx_reloc_counts = {
        ".text": 2000, gfx_selector_name: 66,
        ".data": 65, ".rodata": 6,
    }
    if (gfx_sections[".text"].size != gfx_size
            or gfx_sections[gfx_selector_name].size != gfx_selector_size
            or gfx_sections[".data"].size != gfx_data_size
            or gfx_sections[".rodata"].size != gfx_rodata_size
            or any(gfx_sections[".rel" + name].size != count * 8
                   for name, count in gfx_reloc_counts.items())):
        fail("historical GFX object layout drift")
    gfx_external: dict[str, int] = {}
    gfx_pending: dict[int, list[int]] = {}
    gfx_unpaired_low: set[str] = set()

    def record_gfx_target(name: str, value: int) -> None:
        value &= 0xFFFFFFFF
        previous = gfx_external.get(name)
        if previous is not None and previous != value:
            fail(f"historical GFX relocation target ambiguity: {name}")
        gfx_external[name] = value

    for source_name, address, size in (
        (".text", gfx_address, gfx_size),
        (gfx_selector_name, gfx_selector_address, gfx_selector_size),
    ):
        section = gfx_sections[source_name]
        raw = gfx.data[section.offset:section.offset + size]
        expected = reference[address - TARGET_BASE:address - TARGET_BASE + size]
        masked = bytearray(raw)
        rel = gfx_sections[".rel" + source_name]
        for index in range(gfx_reloc_counts[source_name]):
            offset, info = struct.unpack_from("<II", gfx.data, rel.offset + index * 8)
            symbol_index, kind = info >> 8, info & 0xFF
            if (offset + 4 > size or kind not in (4, 5, 6)
                    or symbol_index >= len(gfx.symbols)):
                fail("historical GFX code-relocation drift")
            symbol = gfx.symbols[symbol_index]
            if symbol.section_index == 0:
                if not symbol.name:
                    fail("historical GFX unnamed external relocation")
                raw_word = struct.unpack_from("<I", raw, offset)[0]
                target_word = struct.unpack_from("<I", expected, offset)[0]
                if kind == 4:
                    if raw_word & 0xFC000000 != target_word & 0xFC000000:
                        fail("historical GFX call opcode drift")
                    record_gfx_target(
                        symbol.name,
                        ((target_word & 0x03FFFFFF)
                         - (raw_word & 0x03FFFFFF)) << 2,
                    )
                elif kind == 5:
                    gfx_pending.setdefault(symbol_index, []).append(offset + (0 if source_name == ".text" else gfx_size))
                else:
                    highs = gfx_pending.pop(symbol_index, [])
                    if not highs:
                        gfx_unpaired_low.add(symbol.name)
                    for encoded in highs:
                        high_raw = (
                            gfx.data[gfx_sections[".text"].offset:gfx_sections[".text"].offset + gfx_size]
                            if encoded < gfx_size else
                            gfx.data[gfx_sections[gfx_selector_name].offset:gfx_sections[gfx_selector_name].offset + gfx_selector_size]
                        )
                        high_expected = (
                            reference[gfx_address - TARGET_BASE:gfx_address - TARGET_BASE + gfx_size]
                            if encoded < gfx_size else
                            reference[gfx_selector_address - TARGET_BASE:gfx_selector_address - TARGET_BASE + gfx_selector_size]
                        )
                        high_offset = encoded if encoded < gfx_size else encoded - gfx_size
                        raw_high = struct.unpack_from("<I", high_raw, high_offset)[0] & 0xFFFF
                        target_high = struct.unpack_from("<I", high_expected, high_offset)[0] & 0xFFFF
                        record_gfx_target(
                            symbol.name,
                            (target_high << 16) + signed_short(target_word)
                            - (raw_high << 16) - signed_short(raw_word),
                        )
            elif symbol.section_index not in (
                gfx_sections[".text"].index, gfx_sections[".data"].index,
                gfx_sections[".rodata"].index,
                gfx_sections[gfx_selector_name].index,
            ):
                fail("historical GFX unsupported internal relocation")
            masked[offset:offset + 4] = expected[offset:offset + 4]
        if masked != expected:
            fail("historical GFX code differs outside relocations")
    if gfx_pending or not gfx_unpaired_low.issubset(gfx_external):
        fail("historical GFX unpaired external relocation")
    for source_name, address, size in (
        (".data", gfx_data_address, gfx_data_size),
        (".rodata", gfx_rodata_address, gfx_rodata_size),
    ):
        section = gfx_sections[source_name]
        raw = gfx.data[section.offset:section.offset + size]
        expected = reference[address - TARGET_BASE:address - TARGET_BASE + size]
        masked = bytearray(raw)
        rel = gfx_sections[".rel" + source_name]
        for index in range(gfx_reloc_counts[source_name]):
            offset, info = struct.unpack_from("<II", gfx.data, rel.offset + index * 8)
            symbol_index, kind = info >> 8, info & 0xFF
            if (offset + 4 > size or kind != 2
                    or symbol_index >= len(gfx.symbols)):
                fail("historical GFX data-relocation drift")
            symbol = gfx.symbols[symbol_index]
            word = struct.unpack_from("<I", expected, offset)[0]
            addend = struct.unpack_from("<I", raw, offset)[0]
            if symbol.section_index == gfx_sections[".text"].index:
                if word != gfx_address + addend:
                    fail("historical GFX data code pointer drift")
            elif symbol.section_index == gfx_sections[".rodata"].index:
                if word != gfx_rodata_address + addend:
                    fail("historical GFX data read-only pointer drift")
            elif symbol.section_index == 0 and symbol.name == "__gxx_personality_v0":
                record_gfx_target(symbol.name, word - addend)
            else:
                fail("historical GFX unsupported data relocation")
            masked[offset:offset + 4] = expected[offset:offset + 4]
        if masked != expected:
            fail("historical GFX data differs outside relocations")
    gfx_output = args.build_dir / "stage3p-gfx-direct.o"
    gfx_command = [
        objcopy, "--rename-section", f".text={gfx_section}.{gfx_address:08x}",
        "--rename-section", f"{gfx_selector_name}={gfx_selector_section}.{gfx_selector_address:08x}",
        "--rename-section", f".data={gfx_data_section}",
        "--rename-section", f".rodata={gfx_rodata_section}",
    ]
    for symbol, destination in sorted(gfx_external.items()):
        alias = f"stage3p_gfx_target_{symbol}"
        gfx_command.extend(("--redefine-sym", f"{symbol}={alias}"))
        relocation_targets[alias] = destination
    for index, entry in enumerate(gfx.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index in (
                    gfx_sections[".text"].index,
                    gfx_sections[gfx_selector_name].index,
                    gfx_sections[".data"].index,
                    gfx_sections[".rodata"].index,
                )):
            gfx_command.extend(("--redefine-sym", f"{entry.name}=stage3p_gfx_direct_{index}"))
    run([*gfx_command, ROOT / gfx_source, gfx_output])
    direct_objects.append(gfx_output)
    direct_sections.extend((
        (gfx_section, gfx_address, gfx_size, None),
        (gfx_selector_section, gfx_selector_address, gfx_selector_size, None),
    ))
    (ops_source, ops_section, ops_address, ops_size,
     ops_shutdown_section, ops_shutdown_address, ops_shutdown_size,
     ops_data_section, ops_data_address, ops_data_prefix_size) = DIRECT_CPUOPS_OBJECT
    ops_original = ELFFile(ROOT / ops_source)
    ops_sections = {entry.name: entry for entry in ops_original.sections}
    shutdown_name = ".gnu.linkonce.t._Z11CPUShutdownv"
    if (ops_original.elf_class != 1 or ops_original.endian != "<"
            or ops_sections[".text"].size != ops_size
            or ops_sections[shutdown_name].size != ops_shutdown_size
            or ops_sections[".data"].size != 0x5238
            or ops_sections[".rel.text"].size != 6058 * 8
            or ops_sections[".rel" + shutdown_name].size != 29 * 8
            or ops_sections[".rel.data"].size != 1421 * 8):
        fail("historical CPUOPS object layout drift")
    # The historical data prefix has 1416 original relocations and is exact.
    # Its final 0xac CFI bytes are not exact outside relocations; retain the
    # already proved public semantic CFI provider at the same address.
    ops_data = ops_original.data[
        ops_sections[".data"].offset:
        ops_sections[".data"].offset + ops_sections[".data"].size
    ]
    ops_expected_data = reference[
        ops_data_address - TARGET_BASE:
        ops_data_address - TARGET_BASE + len(ops_data)
    ]
    ops_data_masked = bytearray(ops_data[:ops_data_prefix_size])
    for index in range(1421):
        offset, info = struct.unpack_from(
            "<II", ops_original.data,
            ops_sections[".rel.data"].offset + index * 8,
        )
        symbol_index, kind = info >> 8, info & 0xFF
        if (kind != 2 or symbol_index >= len(ops_original.symbols)
                or (index < 1416 and offset + 4 > ops_data_prefix_size)
                or (index >= 1416 and offset < ops_data_prefix_size)):
            fail("historical CPUOPS data-relocation partition drift")
        if index < 1416:
            ops_data_masked[offset:offset + 4] = ops_expected_data[offset:offset + 4]
    if ops_data_masked != ops_expected_data[:ops_data_prefix_size]:
        fail("historical CPUOPS data prefix differs outside relocations")
    tail_offsets = {
        off + i
        for index in range(1416, 1421)
        for off in (struct.unpack_from(
            "<I", ops_original.data,
            ops_sections[".rel.data"].offset + index * 8,
        )[0],)
        for i in range(4)
    }
    tail_difference_count = sum(
        ops_data[index] != ops_expected_data[index]
        for index in range(ops_data_prefix_size, len(ops_data))
        if index not in tail_offsets
    )
    if tail_difference_count != 77:
        fail("historical CPUOPS semantic CFI boundary drift")
    ops_source_copy = args.build_dir / "stage3p-cpuops-prefix-source.o"
    ops_file = bytearray(ops_original.data)
    section_table = struct.unpack_from("<I", ops_file, 0x20)[0]
    section_entry_size = struct.unpack_from("<H", ops_file, 0x2E)[0]
    if section_entry_size < 40:
        fail("historical CPUOPS ELF section-header drift")
    struct.pack_into(
        "<I", ops_file,
        section_table + ops_sections[".data"].index * section_entry_size + 20,
        ops_data_prefix_size,
    )
    struct.pack_into(
        "<I", ops_file,
        section_table + ops_sections[".rel.data"].index * section_entry_size + 20,
        1416 * 8,
    )
    ops_source_copy.write_bytes(ops_file)
    ops = ELFFile(ops_source_copy)
    if (ops.sections[ops_sections[".data"].index].size != ops_data_prefix_size
            or ops.sections[ops_sections[".rel.data"].index].size != 1416 * 8):
        fail("historical CPUOPS derived prefix-object drift")
    ops_external: dict[str, int] = {}
    ops_pending: dict[int, list[tuple[str, int]]] = {}
    ops_unpaired_low: set[str] = set()

    def record_cpuops_target(name: str, value: int) -> None:
        value &= 0xFFFFFFFF
        previous = ops_external.get(name)
        if previous is not None and previous != value:
            fail(f"historical CPUOPS relocation target ambiguity: {name}")
        ops_external[name] = value

    for source_name, address, size, reloc_count in (
        (".text", ops_address, ops_size, 6058),
        (shutdown_name, ops_shutdown_address, ops_shutdown_size, 29),
    ):
        section = ops_sections[source_name]
        raw = ops_original.data[section.offset:section.offset + size]
        expected = reference[address - TARGET_BASE:address - TARGET_BASE + size]
        masked = bytearray(raw)
        rel = ops_sections[".rel" + source_name]
        for index in range(reloc_count):
            offset, info = struct.unpack_from(
                "<II", ops_original.data, rel.offset + index * 8
            )
            symbol_index, kind = info >> 8, info & 0xFF
            if (offset + 4 > size or kind not in (4, 5, 6)
                    or symbol_index >= len(ops_original.symbols)):
                fail("historical CPUOPS code-relocation drift")
            symbol = ops_original.symbols[symbol_index]
            if symbol.section_index == 0:
                if not symbol.name:
                    fail("historical CPUOPS unnamed external relocation")
                raw_word = struct.unpack_from("<I", raw, offset)[0]
                target_word = struct.unpack_from("<I", expected, offset)[0]
                if kind == 4:
                    if raw_word & 0xFC000000 != target_word & 0xFC000000:
                        fail("historical CPUOPS call opcode drift")
                    record_cpuops_target(
                        symbol.name,
                        ((target_word & 0x03FFFFFF)
                         - (raw_word & 0x03FFFFFF)) << 2,
                    )
                elif kind == 5:
                    ops_pending.setdefault(symbol_index, []).append((source_name, offset))
                else:
                    highs = ops_pending.pop(symbol_index, [])
                    if not highs:
                        ops_unpaired_low.add(symbol.name)
                    for high_name, high_offset in highs:
                        high_section = ops_sections[high_name]
                        high_address = ops_address if high_name == ".text" else ops_shutdown_address
                        raw_high = struct.unpack_from(
                            "<I", ops_original.data, high_section.offset + high_offset
                        )[0] & 0xFFFF
                        target_high = struct.unpack_from(
                            "<I", reference,
                            high_address - TARGET_BASE + high_offset
                        )[0] & 0xFFFF
                        record_cpuops_target(
                            symbol.name,
                            (target_high << 16) + signed_short(target_word)
                            - (raw_high << 16) - signed_short(raw_word),
                        )
            elif symbol.section_index not in (
                ops_sections[".text"].index, ops_sections[".data"].index,
                ops_sections[shutdown_name].index,
            ):
                fail("historical CPUOPS unsupported internal relocation")
            masked[offset:offset + 4] = expected[offset:offset + 4]
        if masked != expected:
            fail("historical CPUOPS code differs outside relocations")
    if ops_pending or not ops_unpaired_low.issubset(ops_external):
        fail("historical CPUOPS unpaired external relocation")
    for index in range(1416):
        offset, info = struct.unpack_from(
            "<II", ops_original.data,
            ops_sections[".rel.data"].offset + index * 8,
        )
        symbol = ops_original.symbols[info >> 8]
        actual = struct.unpack_from("<I", ops_expected_data, offset)[0]
        addend = struct.unpack_from("<I", ops_data, offset)[0]
        if symbol.section_index == ops_sections[".text"].index:
            if actual != ops_address + addend:
                fail("historical CPUOPS data code pointer drift")
        elif symbol.section_index == 0 and symbol.name == "__gxx_personality_v0":
            record_cpuops_target(symbol.name, actual - addend)
        else:
            fail("historical CPUOPS unsupported data relocation")
    ops_output = args.build_dir / "stage3p-cpuops-direct.o"
    ops_command = [
        objcopy, "--rename-section", f".text={ops_section}.{ops_address:08x}",
        "--rename-section", f"{shutdown_name}={ops_shutdown_section}.{ops_shutdown_address:08x}",
        "--rename-section", f".data={ops_data_section}",
    ]
    for symbol, destination in sorted(ops_external.items()):
        alias = f"stage3p_cpuops_target_{symbol}"
        ops_command.extend(("--redefine-sym", f"{symbol}={alias}"))
        relocation_targets[alias] = destination
    for index, entry in enumerate(ops.symbols):
        if (entry.name and entry.info >> 4 == 1
                and entry.section_index in (
                    ops_sections[".text"].index, ops_sections[".data"].index,
                    ops_sections[shutdown_name].index,
                )):
            ops_command.extend(("--redefine-sym", f"{entry.name}=stage3p_cpuops_direct_{index}"))
    run([*ops_command, ops_source_copy, ops_output])
    direct_objects.append(ops_output)
    direct_sections.extend((
        (ops_section, ops_address, ops_size, None),
        (ops_shutdown_section, ops_shutdown_address, ops_shutdown_size, None),
    ))
    tile_output, tile_sections = link_historical_tile(
        selected_sources, reference, args.build_dir, objcopy, relocation_targets,
    )
    direct_objects.append(tile_output)
    direct_sections.extend(tile_sections)
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
            "direct_candidate_object_inputs": len(DIRECT_WHOLE_OBJECTS) + len(DIRECT_SELF_RELOC_OBJECTS) + len(DIRECT_CALL_OBJECTS) + len(DIRECT_EXTERNAL_RELOC_OBJECTS) + 10,
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
            base_script.read_text(encoding="utf-8"), source_paths, direct_sections,
            relocation_targets,
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
