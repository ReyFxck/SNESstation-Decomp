#!/usr/bin/env python3
"""Rebuild and integrate the historical source-data corridor in image window 35.

Stage 3N combines freshly compiled Snes9x 1.41-1 data with explicit GCC 3.2.2
call-frame semantics. The private reference supplies only verified R_MIPS_32
results for source-owned pointer fields and comparison answers; no target
payload is committed.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import shlex
import subprocess
from pathlib import Path
from typing import Sequence

import data_backing
import historical_data
import historical_tail_data as stage3i
import link_layout_probe
import media_assets as stage3m
import startup_integration as startup
import window36_data as stage3l
from compare_elf_functions import ELFFile
from source_aliases import resolve_tool


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/window35_data.json"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_STARTUP = ROOT / "build/startup-integration/crt0-stage3g.o"
DEFAULT_STAGE3I = ROOT / "build/historical-tail-data"
DEFAULT_STAGE3J = ROOT / "build/runtime-tail-data"
DEFAULT_STAGE3K = ROOT / "build/tail-metadata"
DEFAULT_STAGE3L = ROOT / "build/window36-data"
DEFAULT_STAGE3M = ROOT / "build/media-assets"
DEFAULT_BUILD = ROOT / "build/window35-data"

FORMAT = "snesstation-stage3n-window35-source-data"
SCHEMA = 1
TARGET_BASE = 0x00100000
TARGET_SHA256 = stage3m.TARGET_SHA256
PERSONALITY = 0x001A9728

SOURCE_HASHES = {
    "2XSAI.CPP": "db89b9b61f75b68f63135e408103b64463a3c5367c156ff5c81886e2e079a47f",
    "APU.CPP": "84e40e72f8fcca594e4086cad61d65c6703a0297d2e54b1ab6a823168cdf8ded",
    "c4.cpp": "f8ba4f3c2f67d217caac7369111746f596bb6d264cc4a1ddd9edd58416d4d627",
    "c4emu.cpp": "a06866949a1c2ac1d0396178023ee1f1daa3e65a1355af7cf294ecd947da800d",
    "CPUEXEC.CPP": "5d17165342ec705cd80f5a61e6149d1b2ba3fb47095a6a01ec5069f38fa9a2c2",
    "CPUOPS.CPP": "85b8849cb5f2b930564f84e82e9fadca427f52f79c05bf0d12e64d62899e45a1",
    "data.cpp": "9d034dd8777df0faeee73e10233046a55722437d1c2b986091a3e49d8afef973",
}


def provider(name: str, filename: str, address: int, size: int, full_size: int,
             relocations: int, full_sha256: str, raw_sha256: str,
             linked_sha256: str) -> dict:
    return {
        "name": name,
        "filename": filename,
        "source_section": ".data",
        "source_offset": 0,
        "section": f".data.stage3n.source.{name}",
        "address": address,
        "size": size,
        "full_size": full_size,
        "relocations": relocations,
        "full_sha256": full_sha256,
        "raw_sha256": raw_sha256,
        "linked_sha256": linked_sha256,
    }


SOURCE_SECTIONS = (
    provider(
        "sai2_data_and_cfi", "2XSAI.CPP", 0x00335284, 0xEC, 0xEC, 4,
        "06e808735fb9f82c5e3644df39acdccb85d52144e728a8a3e6a60af170e6933d",
        "06e808735fb9f82c5e3644df39acdccb85d52144e728a8a3e6a60af170e6933d",
        "144bfbba9ce14e83d0b67cfac00626a62f561c608d4a9239cb543e73fd76323f",
    ),
    provider(
        "apu_data_and_cfi", "APU.CPP", 0x00335370, 0x5C8, 0x5C8, 6,
        "0ee7a3e80b07c6183ea655e87115f3f1390f14ac9f1a99965166acd588256456",
        "0ee7a3e80b07c6183ea655e87115f3f1390f14ac9f1a99965166acd588256456",
        "5d1bce6ceaa15b4b311735d6e093f9060ee35f58a6da13d55cd4e1c831282309",
    ),
    provider(
        "c4_state", "c4.cpp", 0x00335938, 0x18, 0x18, 0,
        "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0",
        "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0",
        "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0",
    ),
    provider(
        "c4emu_tables", "c4emu.cpp", 0x003359D0, 0x880, 0x9E8, 0,
        "dc8e48ddd7f11a640fddac4c6f66c683d8190546b362573791f661d2c72ab03f",
        "0d02e06b760365d1028fec081e17082fbb54da13f85599069bfa10a20ea96764",
        "0d02e06b760365d1028fec081e17082fbb54da13f85599069bfa10a20ea96764",
    ),
    provider(
        "cpuexec_cfi", "CPUEXEC.CPP", 0x00336784, 0x74, 0x74, 3,
        "598f19851acea3623d038980e47b722a02088e451306bc78c86f1f422a917675",
        "598f19851acea3623d038980e47b722a02088e451306bc78c86f1f422a917675",
        "0c4740ac67d0b983a08b01c65358c3726f54eda64fa8ef4915628bec56f5a48b",
    ),
    provider(
        "cpuops_tables_and_cfi", "CPUOPS.CPP", 0x003367F8, 0x518C, 0x5238, 1416,
        "a3e4198c49d90f7b95e7ca1ad145fa0c1259d2cd2e0c7f6f2014d33a65473064",
        "dd2f164fcd870d0373bde084d818d425fdbc6e87d15595ec81adef814964f2c7",
        "a8dc70ca6783d1a4404ae919b70b28b7f6c454dbc5e32703ddd1bfe3679216b4",
    ),
    provider(
        "color_math_tables", "data.cpp", 0x0033BAC8, 0x1200, 0x1200, 0,
        "b9777e8c09650d936e0a09af6506a492784000792fa6582a53bdd2b1ff4337eb",
        "b9777e8c09650d936e0a09af6506a492784000792fa6582a53bdd2b1ff4337eb",
        "b9777e8c09650d936e0a09af6506a492784000792fa6582a53bdd2b1ff4337eb",
    ),
)


def advance(value: int) -> tuple[str, int]:
    return ("advance_loc4", value)


def cfa(value: int) -> tuple[str, int]:
    return ("def_cfa_offset", value)


def saved(reg: int, value: int) -> tuple[str, int, int]:
    return ("offset_extended_sf", reg, value)


def fde(pc: int, size: int, *ops: tuple) -> dict:
    return {"pc": pc, "size": size, "ops": ops}


A, C, S, F = advance, cfa, saved, fde

SEMANTIC_GROUPS = (
    {
        "name": "c4", "section": ".data.stage3n.semantic.c4",
        "address": 0x00335950, "size": 0x80, "zero_words": 1,
        "linked_sha256": "dd95b4c750aa95c0ade462223be1b76c39ffd05b3ef83e2f21940c52e4f55f0f",
        "fdes": (
            F(0x0010BECC, 0x1C8, A(4), C(64), A(12), S(17, -12), S(64, -8), A(8), S(16, -16), A(8), S(52, -4)),
            F(0x0010C094, 0xE0, A(4), C(48), A(4), S(17, -8), A(12), S(16, -12), A(12), S(64, -4)),
        ),
    },
    {
        "name": "c4emu", "section": ".data.stage3n.semantic.c4emu",
        "address": 0x00336250, "size": 0x198, "zero_words": 2,
        "linked_sha256": "5bd85653df743b2f763ddc09eeb2f61dd7bbaa6b311692a77b12fd7548e5ce57",
        "fdes": (
            F(0x0010C340, 0x3B8, A(4), C(208), A(44), S(16, -40), S(18, -32), S(19, -28), S(20, -24), S(21, -20), S(22, -16), S(23, -12), S(30, -8), S(64, -4), S(17, -36)),
            F(0x0010CBB0, 0x21C, A(4), C(176), A(16), S(30, -8), S(64, -4), A(12), S(22, -16), S(23, -12), A(8), S(21, -20), A(8), S(20, -24), A(8), S(19, -28), A(8), S(18, -32), A(8), S(17, -36), A(8), S(16, -40)),
            F(0x0010CDCC, 0x1D8, A(4), C(64), A(4), S(16, -16), A(20), S(17, -12), S(18, -8), S(64, -4)),
            F(0x0010CFA4, 0x304, A(4), C(176), A(44), S(16, -40), S(17, -36), S(19, -28), S(20, -24), S(21, -20), S(22, -16), S(23, -12), S(30, -8), S(64, -4), S(18, -32)),
            F(0x0010D734, 0xA8, A(4), C(16), A(8), S(64, -4)),
            F(0x0010D7DC, 0xB0C, A(4), C(64), A(16), S(64, -4), S(18, -8), A(16), S(16, -16), S(17, -12)),
            F(0x001AB4E8, 0x154, A(4), C(16), A(12), S(64, -4)),
        ),
    },
    {
        "name": "cheats", "section": ".data.stage3n.semantic.cheats",
        "address": 0x003363E8, "size": 0x228, "zero_words": 1,
        "linked_sha256": "e4586ad8f66280ffeed491948258c647daa1b47092efb5d1a23e631e34ea0312",
        "fdes": (
            F(0x00114118, 0xB4, A(4), C(64), A(8), S(17, -12), A(24), S(64, -4), S(18, -8), A(12), S(16, -16)),
            F(0x001141CC, 0x94, A(4), C(80), A(8), S(18, -12), A(12), S(19, -8), A(16), S(16, -20), S(17, -16), A(8), S(64, -4)),
            F(0x00114260, 0x24, A(4), C(16), A(4), S(64, -4)),
            F(0x00114284, 0x4C, A(4), C(16), A(8), S(64, -4)),
            F(0x001142D0, 0x58, A(4), C(32), A(16), S(64, -4), S(16, -8)),
            F(0x00114328, 0x74, A(4), C(16), A(12), S(64, -4)),
            F(0x0011439C, 0xB0, A(4), C(64), A(16), S(17, -12), S(18, -8), A(16), S(16, -16), S(64, -4)),
            F(0x0011444C, 0x84, A(4), C(48), A(16), S(16, -12), S(17, -8), S(64, -4)),
            F(0x001144D0, 0x74, A(4), C(48), A(20), S(17, -8), S(64, -4), S(16, -12)),
            F(0x00114544, 0x130, A(4), C(80), A(20), S(16, -12), S(64, -4), S(17, -8)),
            F(0x00114674, 0x154, A(4), C(96), A(12), S(64, -4), S(17, -12), A(12), S(16, -16), S(18, -8)),
            F(0x001AB63C, 0x2C4, A(12), C(16), A(8), S(64, -4)),
            F(0x001AB900, 0x328, A(12), C(16), A(16), S(64, -4)),
        ),
    },
    {
        "name": "clip", "section": ".data.stage3n.semantic.clip",
        "address": 0x00336610, "size": 0x5C, "zero_words": 1,
        "linked_sha256": "37feecf8604f293c177e20156364f566b04532ad666f079999c13eb42f765b91",
        "fdes": (
            F(0x00114818, 0x11DC, A(4), C(304), A(36), S(22, -12), S(16, -36), S(17, -32), S(18, -28), S(19, -24), S(21, -16), S(23, -8), S(64, -4), A(8), S(20, -20)),
        ),
    },
    {
        "name": "cpu", "section": ".data.stage3n.semantic.cpu",
        "address": 0x0033666C, "size": 0x118, "zero_words": 1,
        "linked_sha256": "5984715bd1ddd650460f207778727d2d34c42b10c0f17ab004f4eea319bcf15b",
        "fdes": (
            F(0x001159F4, 0x24, A(8), C(16), A(8), S(64, -4)),
            F(0x00115A18, 0x140, A(4), C(64), A(8), S(16, -16), A(12), S(18, -8), S(64, -4), A(8), S(17, -12)),
            F(0x00115B58, 0x138, A(4), C(64), A(8), S(18, -8), A(24), S(16, -16), S(17, -12), S(64, -4)),
            F(0x00115C90, 0x120, A(4), C(48), A(8), S(17, -8), A(20), S(16, -12), S(64, -4)),
            F(0x001ABC28, 0x3FC, A(4), C(48), A(16), S(64, -4), S(17, -8), A(12), S(16, -12)),
            F(0, 0x2C4, A(12), C(16), A(8), S(64, -4)),
        ),
    },
    {
        "name": "cpuops_tail", "section": ".data.stage3n.semantic.cpuops_tail",
        "address": 0x0033B984, "size": 0x144, "zero_words": 1,
        "external_cie": 0x003377F8,
        "linked_sha256": "0fb40add4bf36f4d71cdb5906a5c37bbbc865c9d55ba099a349014190a9b9dad",
        "fdes": (
            F(0, 0x2C4, A(12), C(16), A(8), S(64, -4)),
            F(0, 0x3FC, A(4), C(48), A(16), S(64, -4), S(17, -8), A(12), S(16, -12)),
            F(0, 0x328, A(12), C(16), A(16), S(64, -4)),
            F(0x001AC190, 0x474, A(4), C(48), A(16), S(16, -12), S(17, -8), A(8), S(64, -4)),
            F(0x00129250, 0x1D0, A(4), C(16), A(8), S(64, -4)),
            F(0x00129420, 0x2A4, A(4), C(32), A(12), S(16, -8), S(64, -4)),
            F(0x001296C4, 0x278, A(4), C(16), A(8), S(64, -4)),
            F(0x0012993C, 0x1B8, A(4), C(16), A(8), S(64, -4)),
            F(0x001AC604, 0x130, A(4), C(32), A(16), S(16, -8), S(64, -4)),
        ),
    },
    {
        "name": "dma", "section": ".data.stage3n.semantic.dma",
        "address": 0x0033CCC8, "size": 0x1B0, "zero_words": 1,
        "linked_sha256": "1acd1f0bb80c268bf8e625b90bb206a54fc1643c8e47930c581d03ba17d9862c",
        "fdes": (
            F(0x00129AF4, 0x18F4, A(8), C(224), A(44), S(16, -40), S(17, -36), S(18, -32), S(19, -28), S(20, -24), S(21, -20), S(22, -16), S(23, -12), S(30, -8), S(64, -4)),
            F(0x0012B498, 0x50C, A(4), C(112), A(20), S(21, -8), S(16, -28), S(17, -24), S(64, -4), A(8), S(20, -12), A(8), S(19, -16), A(8), S(18, -20)),
            F(0x0012BA5C, 0x2EC, A(4), C(128), A(8), S(17, -28), A(8), S(18, -24), A(40), S(16, -32), S(19, -20), S(20, -16), S(21, -12), S(22, -8), S(64, -4)),
            F(0x0012BD48, 0x214, A(4), C(48), A(20), S(17, -8), S(64, -4), S(16, -12)),
            F(0, 0x2C4, A(12), C(16), A(8), S(64, -4)),
            F(0, 0x3FC, A(4), C(48), A(16), S(64, -4), S(17, -8), A(12), S(16, -12)),
            F(0, 0x328, A(12), C(16), A(16), S(64, -4)),
            F(0x001AC734, 0x104, A(4), C(16), A(12), S(64, -4)),
            F(0, 0x154, A(4), C(16), A(12), S(64, -4)),
        ),
    },
)

INTERNAL_FDES = (
    {"name": "S9xDeleteCheat", "address": 0x001141CC, "size": 0x94,
     "sha256": "e2e34a7c031d6307d39e812ee07f6873ceb51c841c4367e27123d5dd4b610239"},
    {"name": "S9xDeleteCheats", "address": 0x00114260, "size": 0x24,
     "sha256": "4a32cd5594f1fac39e6a0bd973ffa148cd360a8e486c57caf02963900d22d5fb"},
    {"name": "S9xEnableCheat", "address": 0x00114284, "size": 0x4C,
     "sha256": "9ec3f9d32a90d4e38fa9b2016a72c46de9c09dbbcb1e2683663b991b9cccea26"},
    {"name": "S9xDisableCheat", "address": 0x001142D0, "size": 0x58,
     "sha256": "ecae823bb01b6983dc733b734e38a6904810d12083ce68101c1bc2ed8b861d15"},
    {"name": "S9xSaveCheatFile", "address": 0x00114674, "size": 0x154,
     "sha256": "250d60be27e340ccf717ecfbc8206e1cc1e88c2b9f1165768c369a17b4eecdf3"},
    {"name": "S9xSoftReset", "address": 0x00115C90, "size": 0x120,
     "sha256": "21d6c8defce0faccb2633846a0923e70f2e4a852db3b78267dc1666a4aa4b278"},
)

ABSORBED_FIXED = (
    ".data.stage3f.history.va_00335370", ".data.stage3f.history.va_00335374",
    ".data.stage3f.history.va_00335378", ".data.stage3f.history.va_00335478",
    ".data.stage3f.history.va_003354f8", ".data.stage3f.history.va_00335538",
    ".data.stage3f.history.va_00335638", ".data.stage3f.history.va_00335738",
    ".data.stage3f.history.va_00335938", ".data.stage3f.history.va_0033593a",
    ".data.stage3f.history.va_0033593c", ".data.stage3f.history.va_0033593e",
    ".data.stage3f.history.va_00335940", ".data.stage3f.history.va_00335942",
    ".data.stage3f.history.va_00335944", ".data.stage3f.history.va_00335946",
    ".data.stage3f.history.va_00335948", ".data.stage3f.history.va_0033594a",
    ".data.stage3f.history.va_0033594c", ".data.stage3ce.va_0033594e",
    ".data.stage3f.recovered.va_003359d0", ".data.stage3f.recovered.va_00335a50",
    ".data.stage3f.recovered.va_00335e50", ".data.stage3ce.va_003367f8",
)

EXACT_CHUNKS = list(range(12, 51))
EXPECTED = {
    "source_sections": 7,
    "source_bytes": 29_516,
    "source_relocations": 1_429,
    "semantic_sections": 7,
    "semantic_bytes": 2_216,
    "semantic_fdes": 47,
    "semantic_relocations": 45,
    "absorbed_fixed_sections": 24,
    "window35_differing_bytes": 0,
    "exact_chunks": 39,
    "mismatching_chunks": 12,
    "differing_bytes": 644_215,
    "prior_differing_bytes": 661_433,
    "differences_removed": 17_218,
    "chunk_count": 51,
    "target_initialized_size": 3_304_936,
    "first_differing_address": 0x00100114,
    "target_sha256": TARGET_SHA256,
}


class Window35Error(RuntimeError):
    """The Stage-3N public contract or private integration drifted."""


def fail(message: str) -> None:
    raise Window35Error(message)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def run(command: Sequence[str | Path], cwd: Path = ROOT) -> str:
    result = subprocess.run(
        [str(item) for item in command], cwd=cwd, text=True,
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    if result.returncode:
        fail(f"command failed: {shlex.join(map(str, command))}\n{result.stdout[-5000:]}")
    return result.stdout.strip()


def section(elf: ELFFile, name: str):
    found = [item for item in elf.sections if item.name == name]
    if len(found) != 1:
        fail(f"missing/duplicate section: {name}")
    return found[0]


def source_document() -> list[dict]:
    return [dict(row) for row in SOURCE_SECTIONS]


def semantic_document() -> list[dict]:
    return [
        {
            **row,
            "fdes": [
                {**entry, "ops": [list(op) for op in entry["ops"]]}
                for entry in row["fdes"]
            ],
        }
        for row in SEMANTIC_GROUPS
    ]


def internal_document() -> list[dict]:
    return [dict(row) for row in INTERNAL_FDES]


def claims() -> dict[str, bool]:
    return {
        "window_35_exact": True,
        "historical_source_sections_rebuilt": True,
        "non_relocation_bytes_raw_exact": True,
        "private_oracle_limited_to_r_mips_32_results": True,
        "unwind_semantics_reconstructed": True,
        "private_target_bytes_stored": False,
        "replacement_elf": False,
        "unpacked_hash_matched": False,
        "packed_hash_matched": False,
    }


def frozen_document(result: dict) -> dict:
    return {
        "format": FORMAT,
        "schema_version": SCHEMA,
        "prior_manifest_sha256": digest(stage3m.DEFAULT_MANIFEST.read_bytes()),
        "source_sections": source_document(),
        "semantic_groups": semantic_document(),
        "internal_fdes": internal_document(),
        "absorbed_fixed_sections": list(ABSORBED_FIXED),
        "result": result,
        "claims": claims(),
    }


def validate_fdes() -> None:
    with (ROOT / "analysis/progress_targets.csv").open(
        newline="", encoding="utf-8"
    ) as handle:
        rows = {int(row["address"], 0): row for row in csv.DictReader(handle)}
    internal = {(row["address"], row["size"]) for row in INTERNAL_FDES}
    seen_internal = set()
    for group in SEMANTIC_GROUPS:
        for entry in group["fdes"]:
            pc = entry["pc"]
            if pc == 0:
                continue
            key = (pc, entry["size"])
            if key in internal:
                seen_internal.add(key)
            elif pc not in rows or rows[pc]["status"] != "MATCHING":
                fail(f"semantic FDE lacks exact function evidence at 0x{pc:08x}")
    if seen_internal != internal:
        fail("internal FDE roster drift")


def validate(args: argparse.Namespace) -> dict:
    stage3m.validate(stage3m.parse_args(["validate"]))
    validate_fdes()
    try:
        document = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read window-35 manifest: {exc}")
    if document.get("format") != FORMAT or document.get("schema_version") != SCHEMA:
        fail("window-35 manifest identity drift")
    if document.get("prior_manifest_sha256") != digest(stage3m.DEFAULT_MANIFEST.read_bytes()):
        fail("prior checkpoint drift")
    if document.get("source_sections") != source_document():
        fail("source-section contract drift")
    if document.get("semantic_groups") != semantic_document():
        fail("semantic contract drift")
    if document.get("internal_fdes") != internal_document():
        fail("internal FDE contract drift")
    if document.get("absorbed_fixed_sections") != list(ABSORBED_FIXED):
        fail("absorbed roster drift")
    for key, value in EXPECTED.items():
        if document.get("result", {}).get(key) != value:
            fail(f"frozen metric drift: {key}")
    if document.get("result", {}).get("exact_chunk_indices") != EXACT_CHUNKS:
        fail("exact-window roster drift")
    if document.get("claims") != claims():
        fail("claim boundary drift")
    return document


def prepare_sources(args: argparse.Namespace, compiler: Path) -> dict[str, Path]:
    stage3i.v47.ensure_git_commit(
        stage3i.v47.PS2DEV, stage3i.v47.PS2DEV_REPO, stage3i.v47.PS2DEV_COMMIT
    )
    newlib = stage3i.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    gcc_include = Path(run([compiler, "-print-file-name=include"])).resolve()
    old_build = stage3i.v52.BUILD
    try:
        stage3i.v52.BUILD = args.build_dir / "v52-rebuild"
        source_root, _original, layout = stage3i.v52.prepare_snes_layout()
        stage3i.v52.patch_sources(layout)
        compat = args.build_dir / "compat-v52"
        stage3i.v52.write_compat_headers(compat)
        flags = [
            *stage3i.v47.COMMON_FLAGS,
            "-Os",
            *stage3i.v47.SNES_DEFINES,
            "-DZLIB",
            "-nostdinc",
            *stage3i.v47.include_args(
                (compat, newlib, layout, layout / "unzip", source_root / "zlib", gcc_include)
            ),
            "-x",
            "c++",
        ]
        result = {}
        output = args.build_dir / "source-objects"
        for filename, expected_hash in SOURCE_HASHES.items():
            source = layout / filename
            if digest(source.read_bytes()) != expected_hash:
                fail(f"historical source drift: {filename}")
            obj = output / f"{Path(filename).stem}.o"
            stage3i.compile_one(compiler, flags, source, obj)
            result[filename] = obj
        return result
    finally:
        stage3i.v52.BUILD = old_build


def rebuild_payloads(
    objects: dict[str, Path], reference: bytes, build_dir: Path
) -> list[dict]:
    rows = []
    for spec in SOURCE_SECTIONS:
        elf = ELFFile(objects[spec["filename"]])
        item = section(elf, spec["source_section"])
        full = stage3i.section_bytes(elf, item)
        if len(full) != spec["full_size"] or digest(full) != spec["full_sha256"]:
            fail(f"full source section drift: {spec['name']}")
        start, size = spec["source_offset"], spec["size"]
        source = full[start:start + size]
        if len(source) != size or digest(source) != spec["raw_sha256"]:
            fail(f"source slice drift: {spec['name']}")
        relocations = [
            (offset - start, kind, name)
            for offset, kind, name in historical_data.relocations(elf, item.index)
            if start <= offset and offset + 4 <= start + size
        ]
        if len(relocations) != spec["relocations"] or any(
            kind != 2 for _offset, kind, _name in relocations
        ):
            fail(f"R_MIPS_32 roster drift: {spec['name']}")
        target_start = spec["address"] - TARGET_BASE
        target = reference[target_start:target_start + size]
        patched, mask = bytearray(source), bytearray(size)
        for offset, _kind, _name in relocations:
            mask[offset:offset + 4] = b"\1" * 4
            patched[offset:offset + 4] = target[offset:offset + 4]
        if any(left != right for left, right, marked in zip(source, target, mask) if not marked):
            fail(f"source differs outside relocations: {spec['name']}")
        if bytes(patched) != target or digest(target) != spec["linked_sha256"]:
            fail(f"linked source payload drift: {spec['name']}")
        path = build_dir / f"payloads/{spec['name']}.bin"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(patched)
        rows.append({**spec, "payload": path})
    return rows


def render_payload_source(rows: Sequence[dict]) -> str:
    lines = ["/* Fresh source bytes plus verified R_MIPS_32 results. */"]
    for row in rows:
        lines.extend(
            [
                f'.section {row["section"]},"aw",@progbits',
                ".align 2",
                f'.incbin "{row["payload"].resolve()}"',
            ]
        )
    return "\n".join(lines) + "\n"


def emit_ops(lines: list[str], entry: dict) -> None:
    for op in entry["ops"]:
        if op[0] == "advance_loc4":
            lines.extend([".byte 4", f".4byte {op[1]}"])
        elif op[0] == "def_cfa_offset":
            lines.extend([".byte 14", f".uleb128 {op[1]}"])
        elif op[0] == "offset_extended_sf":
            lines.extend([".byte 17", f".uleb128 {op[1]}", f".sleb128 {op[2]}"])
        else:
            fail(f"unknown CFI operation: {op[0]}")


def encoded_uleb_size(value: int) -> int:
    size = 1
    while value >= 0x80:
        value >>= 7
        size += 1
    return size


def encoded_sleb_size(value: int) -> int:
    size = 0
    while True:
        byte = value & 0x7F
        value >>= 7
        size += 1
        if (value == 0 and not byte & 0x40) or (value == -1 and byte & 0x40):
            return size


def fde_entry_size(entry: dict) -> int:
    op_size = 0
    for op in entry["ops"]:
        if op[0] == "advance_loc4":
            op_size += 5
        elif op[0] == "def_cfa_offset":
            op_size += 1 + encoded_uleb_size(op[1])
        else:
            op_size += 1 + encoded_uleb_size(op[1]) + encoded_sleb_size(op[2])
    return (17 + op_size + 3) & ~3


def render_semantic_source() -> str:
    lines = ["/* Explicit DWARF semantics; contains no private target payload. */"]
    for group_index, group in enumerate(SEMANTIC_GROUPS):
        frame = f".Lst3n_frame_{group_index}"
        lines.extend([f'.section {group["section"]},"aw",@progbits', ".align 2"])
        if "external_cie" not in group:
            lines.extend(
                [
                    f"{frame}:",
                    f".4byte .Lst3n_cie_end_{group_index}-.Lst3n_cie_body_{group_index}",
                    f".Lst3n_cie_body_{group_index}:",
                    ".4byte 0",
                    ".byte 1",
                    '.ascii "zP\\0"',
                    ".uleb128 1",
                    ".sleb128 4",
                    ".byte 64",
                    ".uleb128 5",
                    ".byte 0",
                    ".4byte stage3n_gxx_personality",
                    ".byte 12",
                    ".uleb128 29",
                    ".uleb128 0",
                    f".Lst3n_cie_end_{group_index}:",
                ]
            )
        offset = 0
        for entry_index, entry in enumerate(group["fdes"]):
            body = f".Lst3n_fde_body_{group_index}_{entry_index}"
            end = f".Lst3n_fde_end_{group_index}_{entry_index}"
            if "external_cie" in group:
                cie_pointer = group["address"] + offset + 4 - group["external_cie"]
                pointer = f"0x{cie_pointer:x}"
            else:
                pointer = f"{body}-{frame}"
            pc = "0" if entry["pc"] == 0 else f"stage3n_pc_{entry['pc']:08x}"
            lines.extend(
                [
                    f".4byte {end}-{body}",
                    f"{body}:",
                    f".4byte {pointer}",
                    f".4byte {pc}",
                    f".4byte 0x{entry['size']:x}",
                    ".uleb128 0",
                ]
            )
            emit_ops(lines, entry)
            lines.extend([".align 2", f"{end}:"])
            offset += fde_entry_size(entry)
        lines.extend([".4byte 0"] * group["zero_words"])
    return "\n".join(lines) + "\n"


def relocation_count(elf: ELFFile, item) -> int:
    return sum(
        other.size // other.entry_size
        for other in elf.sections
        if other.type in (4, 9) and other.info == item.index and other.entry_size
    )


def verify_generated_object(path: Path, rows: Sequence[dict]) -> int:
    elf = ELFFile(path)
    total = 0
    for row in rows:
        item = section(elf, row["section"])
        if item.type != 1 or item.size != row["size"]:
            fail(f"generated geometry drift: {row['name']}")
        total += relocation_count(elf, item)
    return total


def update_linker_script(
    base_script: str, input_path: Path, sections: Sequence[dict]
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
    placements = sorted(
        [*SOURCE_SECTIONS, *SEMANTIC_GROUPS], key=lambda row: row["address"]
    )
    insertion = "\n".join(
        f"  {row['section']} 0x{row['address']:08x} : {{ KEEP(*({row['section']})) }}"
        for row in placements
    )
    if script.count(marker) != 1:
        fail("Stage-3M linker insertion marker drift")
    script = script.replace(marker, insertion + "\n" + marker, 1)

    discard_marker = "*(.data.stage3g.crt0)"
    if script.count(discard_marker) != 1:
        fail("Stage-3M discard marker drift")
    discarded_text = " ".join(f"*({name})" for name in ABSORBED_FIXED)
    script = script.replace(discard_marker, f"{discarded_text} {discard_marker}", 1)

    aliases = stage3l.absorbed_aliases(input_path, sections, ABSORBED_FIXED)
    for name in aliases:
        if f"{name} =" in script:
            fail(f"new absorbed alias already exists: {name}")
    assignments = "".join(
        f"{name} = 0x{address:08x};\n" for name, address in sorted(aliases.items())
    )
    discarded = (
        set(stage3l.stage3k.ABSORBED_FIXED)
        | set(stage3l.ABSORBED_FIXED)
        | set(stage3m.ABSORBED_FIXED)
        | set(ABSORBED_FIXED)
    )
    retained = [row for row in sections if row["section"] not in discarded]
    return assignments + script, retained


def probe(args: argparse.Namespace) -> dict:
    prior = stage3m.validate(stage3m.parse_args(["validate"]))
    sections, layout = startup.load_inputs(
        startup.parse_args(
            ["validate", "--sections", str(args.sections), "--layout", str(args.layout)]
        )
    )
    reference = args.reference.read_bytes()
    if len(reference) != EXPECTED["target_initialized_size"] or digest(reference) != TARGET_SHA256:
        fail("private unpacked reference identity drift")
    data_backing.check_sections(args.input, sections)
    for row in INTERNAL_FDES:
        start = row["address"] - TARGET_BASE
        if digest(reference[start:start + row["size"]]) != row["sha256"]:
            fail(f"internal FDE target extent drift: {row['name']}")

    compiler = resolve_tool(args.compiler)
    if run([compiler, "-dumpversion"]) != "3.2.2" or run([compiler, "-dumpmachine"]) != "ee":
        fail("Stage-3N requires EE GCC 3.2.2")
    linker = resolve_tool(args.ld) if args.ld else compiler.with_name("ee-ld")
    objcopy = resolve_tool(args.objcopy) if args.objcopy else compiler.with_name("ee-objcopy")

    prior_inputs = [
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
        args.stage3m_build / "media-assets.o",
    ]
    base_script = args.stage3m_build / "media-assets.ld"
    if not all(path.is_file() for path in [*prior_inputs, base_script]):
        fail("missing Stage-3M dependency; run make media-assets")

    args.build_dir.mkdir(parents=True, exist_ok=True)
    objects = prepare_sources(args, compiler)
    payloads = rebuild_payloads(objects, reference, args.build_dir)
    payload_source = args.build_dir / "window35-payloads.S"
    payload_source.write_text(render_payload_source(payloads), encoding="utf-8")
    payload_object = args.build_dir / "window35-payloads.o"
    stage3i.compile_one(
        compiler,
        ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"),
        payload_source,
        payload_object,
    )
    if verify_generated_object(payload_object, SOURCE_SECTIONS) != 0:
        fail("generated source payload unexpectedly retained relocations")

    semantic_source = args.build_dir / "window35-semantics.S"
    semantic_source.write_text(render_semantic_source(), encoding="utf-8")
    semantic_object = args.build_dir / "window35-semantics.o"
    stage3i.compile_one(
        compiler,
        ("-G0", "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900"),
        semantic_source,
        semantic_object,
    )
    semantic_relocations = verify_generated_object(semantic_object, SEMANTIC_GROUPS)
    if semantic_relocations != EXPECTED["semantic_relocations"]:
        fail("semantic relocation roster drift")

    script, retained = update_linker_script(
        base_script.read_text(encoding="utf-8"), args.input, sections
    )
    for entry in (
        entry
        for group in SEMANTIC_GROUPS
        for entry in group["fdes"]
        if entry["pc"]
    ):
        script = f"stage3n_pc_{entry['pc']:08x} = 0x{entry['pc']:08x};\n" + script
    script = f"stage3n_gxx_personality = 0x{PERSONALITY:08x};\n" + script
    linker_script = args.build_dir / "window35-data.ld"
    linker_script.write_text(script, encoding="utf-8")
    output = args.build_dir / "stage3n-window35-integrated.elf"
    run(
        [
            linker,
            "-EL",
            "-T",
            linker_script,
            "-o",
            output,
            *prior_inputs,
            payload_object,
            semantic_object,
        ]
    )

    elf = ELFFile(output)
    startup.verify_symbols(elf)
    stage3i.verify_fixed_output(elf, retained)
    for row in [*SOURCE_SECTIONS, *SEMANTIC_GROUPS]:
        item = section(elf, row["section"])
        start = row["address"] - TARGET_BASE
        target = reference[start:start + row["size"]]
        actual = stage3i.section_bytes(elf, item)
        if (
            item.address != row["address"]
            or item.size != row["size"]
            or actual != target
            or digest(actual) != row["linked_sha256"]
        ):
            fail(f"linked window-35 section differs: {row['name']}")

    raw_path = args.build_dir / "stage3n-window35-integrated.unpadded.bin"
    padded_path = args.build_dir / "stage3n-window35-integrated.padded.bin"
    run([objcopy, "-O", "binary", output, raw_path])
    raw = raw_path.read_bytes()
    if len(raw) > len(reference):
        fail("integrated diagnostic exceeds target size")
    padded = raw + bytes(len(reference) - len(raw))
    padded_path.write_bytes(padded)
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [
        index for index, (left, right) in enumerate(zip(padded, reference))
        if left != right
    ]
    result = {
        "source_sections": len(SOURCE_SECTIONS),
        "source_bytes": sum(row["size"] for row in SOURCE_SECTIONS),
        "source_relocations": sum(row["relocations"] for row in SOURCE_SECTIONS),
        "semantic_sections": len(SEMANTIC_GROUPS),
        "semantic_bytes": sum(row["size"] for row in SEMANTIC_GROUPS),
        "semantic_fdes": sum(len(row["fdes"]) for row in SEMANTIC_GROUPS),
        "semantic_relocations": semantic_relocations,
        "absorbed_fixed_sections": len(ABSORBED_FIXED),
        "window35_differing_bytes": sum(
            left != right
            for left, right in zip(
                padded[35 * 65536:36 * 65536], reference[35 * 65536:36 * 65536]
            )
        ),
        "exact_chunks": len(exact),
        "mismatching_chunks": len(different),
        "exact_chunk_indices": exact,
        "mismatching_chunk_indices": different,
        "differing_bytes": len(differences),
        "prior_differing_bytes": prior["result"]["differing_bytes"],
        "differences_removed": prior["result"]["differing_bytes"] - len(differences),
        "chunk_count": len(exact) + len(different),
        "target_initialized_size": len(reference),
        "first_differing_address": TARGET_BASE + differences[0],
        "integrated_padded_sha256": digest(padded),
        "target_sha256": digest(reference),
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
    parser.add_argument("--stage3m-build", type=Path, default=DEFAULT_STAGE3M)
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
            "verified window-35 source data: "
            f"source={result['source_sections']} sections/{result['source_bytes']} bytes; "
            f"semantic={result['semantic_fdes']} FDEs/{result['semantic_bytes']} bytes"
        )
        print(
            f"whole-image chunks={result['exact_chunks']}/51 "
            f"remaining={result['mismatching_chunks']} "
            f"differing_bytes={result['differing_bytes']}; replacement ELF: not yet"
        )
        return 0
    except (
        Window35Error,
        stage3m.MediaAssetsError,
        stage3l.Window36Error,
        stage3i.HistoricalTailError,
        data_backing.DataBackingError,
        OSError,
        ValueError,
        KeyError,
        RuntimeError,
    ) as exc:
        print(f"window-35 source data: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
