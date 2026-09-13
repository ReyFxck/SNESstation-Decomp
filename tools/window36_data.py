#!/usr/bin/env python3
"""Rebuild and integrate the source/CFI containers that close image window 36.

Stage 3L promotes four historical Snes9x initialized-data ranges and two
semantic GCC 3.2.2 unwind groups.  The private reference may provide only
final R_MIPS_32 relocation results and comparison answers; generated payloads
stay below ignored build storage and no target bytes enter the repository.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import shlex
import struct
import subprocess
from pathlib import Path
from typing import Sequence

import data_backing
import historical_data
import historical_tail_data as stage3i
import link_layout_probe
import startup_integration as startup
import tail_metadata as stage3k
from compare_elf_functions import ELFFile, compare_function
from source_aliases import resolve_tool

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/window36_data.json"
DEFAULT_REFERENCE = ROOT / "build/SNES_EMU.unpacked.bin"
DEFAULT_LAYOUT = ROOT / "analysis/link_identity/unpacked_layout.json"
DEFAULT_SECTIONS = ROOT / "analysis/link_identity/data_backing_sections.tsv"
DEFAULT_INPUT = ROOT / "build/data-backing/source-tree.data-backed.partial.o"
DEFAULT_STARTUP = ROOT / "build/startup-integration/crt0-stage3g.o"
DEFAULT_STAGE3I = ROOT / "build/historical-tail-data"
DEFAULT_STAGE3J = ROOT / "build/runtime-tail-data"
DEFAULT_STAGE3K = ROOT / "build/tail-metadata"
DEFAULT_BUILD = ROOT / "build/window36-data"

FORMAT = "snesstation-stage3l-window36-data"
SCHEMA = 1
TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
PERSONALITY = 0x001A9728

SOURCE_HASHES = {
    "DSP1.CPP": "4c0046c4bc05565b0638defc450e635a55403ce7d15be536d1a738312b337045",
    "CPUOPS.CPP": "85b8849cb5f2b930564f84e82e9fadca427f52f79c05bf0d12e64d62899e45a1",
    "fxemu.cpp": "021c8b181f3cc5967955a837ffc916f7b30c43adcfef801ee5289e4684ace08e",
    "fxinst.cpp": "f427773319dfc6412642a160b199d73b1a72203503a391d60b7e425d10e6e801",
}


def P(name: str, filename: str, address: int, size: int, full_size: int,
      relocations: int, full_sha256: str, raw_sha256: str,
      linked_sha256: str) -> dict:
    return {
        "name": name,
        "filename": filename,
        "source_section": ".data",
        "source_offset": 0,
        "section": f".data.stage3l.source.{name}",
        "address": address,
        "size": size,
        "full_size": full_size,
        "relocations": relocations,
        "full_sha256": full_sha256,
        "raw_sha256": raw_sha256,
        "linked_sha256": linked_sha256,
    }


SOURCE_SECTIONS = (
    P("dsp1_prefix", "DSP1.CPP", 0x0033CE78, 0x47EC, 0x4B88, 2,
      "487b90039e0a84011da12c1dd25f7c260b8448cd30112572a35689a315dd4c1f",
      "2fe5d36180798587ee09150c313e639b212e4c01c02fe7aa8bbc332ff32ba8b8",
      "a04af52a6e6aec67e49abf77aa14140e5b749743ffcd5a1518cfa4b0380b981b"),
    P("cpu_opcode_tables", "CPUOPS.CPP", 0x00341A38, 0x1000, 0x5238, 1024,
      "a3e4198c49d90f7b95e7ca1ad145fa0c1259d2cd2e0c7f6f2014d33a65473064",
      "a3a1d9b5ffa4fac7ea4ebaa2483f3cf547bb314db75c19534ee2d72610a72d78",
      "30beb2e2de4eee974731ecfa3e3f7b63b9ec38ba0e78cee9ae53f124bb5d94f4"),
    P("fxemu_data", "fxemu.cpp", 0x00342A38, 0x8E8, 0x8E8, 8,
      "8511be0bbfef2cc57b50c744a607816859b2e7f460ea6260efc27e759d1334ed",
      "8511be0bbfef2cc57b50c744a607816859b2e7f460ea6260efc27e759d1334ed",
      "8d739827be081d1cf7155d825fbd808227a75560ac6ba6afef247902505b7470"),
    P("fxinst_data", "fxinst.cpp", 0x00343320, 0x1A10, 0x1A10, 1049,
      "a16f5c06dd2daa0edb6af293c87369ff97093ec4e93566698695a639b7fd4471",
      "a16f5c06dd2daa0edb6af293c87369ff97093ec4e93566698695a639b7fd4471",
      "66298bc08fb0aa5fadf68ef4b7ada795a3b019c7ff6efab0ca93b5cf51c97483"),
)


def A(value: int) -> tuple[str, int]: return ("advance_loc4", value)
def C(value: int) -> tuple[str, int]: return ("def_cfa_offset", value)
def S(reg: int, value: int) -> tuple[str, int, int]: return ("offset_extended_sf", reg, value)
def F(pc: int, size: int, *ops: tuple) -> dict: return {"pc": pc, "size": size, "ops": ops}


DSP1_FDES = (
    F(0x0012BF5C, 0xD0, A(12), C(64), A(4), S(52,-4), A(24), S(16,-16), S(17,-12), S(64,-8)),
    F(0x0012C2DC, 0xBC, A(8), C(16), A(12), S(64,-4)),
    F(0x0012C444, 0x64, A(4), C(48), A(4), S(16,-12), A(16), S(64,-4), S(17,-8)),
    F(0x0012C4A8, 0xB0, A(4), C(96), A(4), S(19,-12), A(12), S(20,-8), S(64,-4), A(12), S(17,-20), S(18,-16), A(8), S(16,-24)),
    F(0x0012C558, 0x680, A(4), C(176), A(76), S(16,-44), S(17,-40), S(18,-36), S(19,-32), S(20,-28), S(21,-24), S(22,-20), S(23,-16), S(30,-12), S(64,-8), S(52,-4), S(54,-3)),
    F(0x0012CBD8, 0x240, A(4), C(160), A(4), S(23,-12), A(48), S(52,-4), S(16,-40), S(17,-36), S(18,-32), S(19,-28), S(20,-24), S(21,-20), S(22,-16), S(64,-8)),
    F(0x0012CE18, 0x244, A(4), C(112), A(44), S(52,-8), S(54,-7), S(56,-6), S(58,-5), S(60,-4), S(16,-28), S(17,-24), S(18,-20), S(19,-16), S(64,-12)),
    F(0x0012D334, 0x178, A(4), C(112), A(8), S(21,-8), S(64,-4), A(16), S(18,-20), S(19,-16), S(20,-12), A(8), S(17,-24), A(8), S(16,-28)),
    F(0x0012D4AC, 0x178, A(4), C(112), A(8), S(21,-8), S(64,-4), A(16), S(18,-20), S(19,-16), S(20,-12), A(8), S(17,-24), A(8), S(16,-28)),
    F(0x0012D624, 0x178, A(4), C(112), A(8), S(21,-8), S(64,-4), A(16), S(18,-20), S(19,-16), S(20,-12), A(8), S(17,-24), A(8), S(16,-28)),
    F(0x0012DC1C, 0x1C4, A(4), C(176), A(8), S(30,-8), S(64,-4), A(8), S(23,-12), A(8), S(22,-16), A(8), S(21,-20), A(8), S(20,-24), A(8), S(19,-28), A(16), S(16,-40), S(17,-36), S(18,-32)),
    F(0x0012DDE0, 0x80, A(4), C(16), A(8), S(64,-4)),
    F(0x0012E0CC, 0x1F8, A(4), C(160), A(4), S(19,-28), A(12), S(30,-8), S(64,-4), A(8), S(23,-12), A(8), S(22,-16), A(8), S(21,-20), A(8), S(20,-24), A(12), S(16,-40), S(18,-32), A(8), S(17,-36)),
    F(0x0012E704, 0x24, A(4), C(16), A(8), S(64,-4)),
    F(0x0012E728, 0x28, A(4), C(16), A(8), S(64,-4)),
    F(0x0012E750, 0xFF4, A(8), C(32), A(24), S(16,-8), S(64,-4)),
    F(0x0012F744, 0x164, A(8), C(48), A(24), S(16,-12), S(17,-8), S(64,-4)),
)

GFX_FDES = (
    F(0x00142A78, 0x59C, A(4), C(48), A(24), S(16,-12), S(17,-8), S(64,-4)),
    F(0x0014311C, 0x274, A(4), C(32), A(16), S(16,-8), S(64,-4)),
    F(0x001434AC, 0x1C8, A(4), C(48), A(4), S(16,-12), A(16), S(17,-8), S(64,-4)),
    F(0x001437D8, 0x6A4, A(4), C(224), A(80), S(16,-40), S(17,-36), S(18,-32), S(19,-28), S(20,-24), S(21,-20), S(22,-16), S(23,-12), S(30,-8), S(64,-4)),
    F(0x00143E7C, 0x570, A(4), C(240), A(40), S(16,-40), S(17,-36), S(18,-32), S(19,-28), S(20,-24), S(21,-20), S(22,-16), S(23,-12), S(30,-8), S(64,-4)),
    F(0x001443EC, 0x6EC, A(8), C(288), A(72), S(16,-40), S(17,-36), S(18,-32), S(19,-28), S(20,-24), S(21,-20), S(22,-16), S(23,-12), S(64,-4), S(30,-8)),
    F(0x00144AD8, 0x7D8, A(4), C(240), A(48), S(16,-40), S(17,-36), S(18,-32), S(19,-28), S(20,-24), S(21,-20), S(22,-16), S(23,-12), S(30,-8), S(64,-4)),
    F(0x001452B0, 0x958, A(20), C(240), A(8), S(21,-20), A(56), S(20,-24), S(22,-16), S(23,-12), S(30,-8), S(64,-4), S(17,-36), A(24), S(16,-40), S(18,-32), S(19,-28)),
    F(0x0014E140, 0x6E8, A(4), C(128), A(8), S(17,-28), A(24), S(64,-4), S(16,-32), S(18,-24), A(24), S(19,-20), S(20,-16), S(21,-12), S(22,-8)),
    F(0x0014EC54, 0x1A38, A(4), C(144), A(16), S(64,-4), S(23,-8), A(36), S(16,-36), S(17,-32), S(18,-28), S(19,-24), S(20,-20), S(21,-16), S(22,-12)),
)

SEMANTIC_SECTIONS = (
    {"name": "dsp1_unwind", "section": ".data.stage3l.dsp1_unwind",
     "address": 0x00341664, "size": 0x3D4, "linked_sha256":
     "5de44b838c8884ffc2326d4bb5683b21a964c49f6534fcd5065b95be38f13ca5",
     "zero_words": 2, "fdes": DSP1_FDES},
    {"name": "gfx_unwind", "section": ".data.stage3l.gfx_unwind",
     "address": 0x00344E10, "size": 0x250, "linked_sha256":
     "20f597f0863376774e17e6f063ce375b335cd29fe8d2cf79cf4d1cb91ebe6deb",
     "zero_words": 2, "fdes": GFX_FDES},
)

# These two indirect-only DSP entries sit outside the JAL-derived function
# universe.  Their source symbols, exact extents and normalized target bytes
# are therefore checked explicitly by the private gate.
INTERNAL_FUNCTIONS = (
    {"symbol": "_Z11DSP1SetByteht", "address": 0x0012E750, "size": 0xFF4,
     "sha256": "56232184ca0d814f5f2da53bc866a6d6e6a5d754340f9ed700fa78859c8164b6"},
    {"symbol": "_Z11DSP1GetBytet", "address": 0x0012F744, "size": 0x164,
     "sha256": "05506efd8bd9527d8fb3acfd869c9cc11b9dedca7f71aada0954159d20bdaf37"},
)

ABSORBED_FIXED = (
    ".data.stage3f.history.va_0033ce78", ".data.stage3f.history.va_0033ee78",
    ".data.stage3f.va_00340f90", ".data.stage3f.history.va_00340f92",
    ".data.stage3f.va_00340f94", ".data.stage3f.va_00341398",
    ".data.stage3f.history.va_0034139a", ".data.stage3f.va_0034139c",
    ".data.stage3f.history.va_003413b2", ".data.stage3f.va_003413b4",
    ".data.stage3f.va_003413cc", ".data.stage3f.va_003413f0",
    ".data.stage3f.va_00341434", ".data.stage3f.va_0034143c",
    ".data.stage3f.history.va_0034144c", ".data.stage3f.va_00341450",
    ".data.stage3f.history.va_0034145c", ".data.stage3f.va_00341460",
    ".data.stage3f.va_003414b0", ".data.stage3f.va_003414fc",
    ".data.stage3f.va_003415b0", ".data.stage3f.history.va_003415b4",
    ".data.stage3f.history.va_003415b6", ".data.stage3f.history.va_003415b8",
    ".data.stage3f.va_003415ba", ".data.stage3f.va_0034160c",
    ".data.stage3f.history.va_00341616", ".data.stage3f.va_00341618",
    ".data.stage3f.history.va_0034161c", ".data.stage3f.va_0034161e",
    ".data.stage3f.history.va_00341626", ".data.stage3f.va_00341628",
    ".data.stage3f.history.va_0034162a", ".data.stage3f.va_0034162c",
    ".data.stage3f.history.va_0034162e", ".data.stage3f.va_00341630",
    ".data.stage3ce.va_00341658", ".data.stage3f.history.va_00342a38",
    ".data.stage3f.va_0034323c", ".data.stage3f.history.va_00343240",
    ".data.stage3f.history.va_00343250", ".data.stage3ce.va_00343b30",
    ".data.stage3ce.va_00343b44",
)

EXACT_CHUNKS = [12, 13, 14, 36, *range(37, 51)]
EXPECTED = {
    "source_sections": 4, "source_bytes": 31460, "source_relocations": 2083,
    "semantic_sections": 2, "semantic_bytes": 1572, "semantic_fdes": 27,
    "semantic_relocations": 29, "internal_functions": 2,
    "absorbed_fixed_sections": 43, "window36_differing_bytes": 0,
    "exact_chunks": 18, "mismatching_chunks": 33, "differing_bytes": 1845637,
    "chunk_count": 51, "target_initialized_size": 3304936,
    "first_differing_address": 0x00100114, "target_sha256": TARGET_SHA256,
    "prior_differing_bytes": 1854246,
}


class Window36Error(RuntimeError): pass
def fail(message: str) -> None: raise Window36Error(message)
def digest(data: bytes) -> str: return hashlib.sha256(data).hexdigest()


def run(command: Sequence[str | Path], cwd: Path = ROOT) -> str:
    result = subprocess.run([str(x) for x in command], cwd=cwd, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        fail(f"command failed: {shlex.join(map(str, command))}\n{result.stdout[-5000:]}")
    return result.stdout.strip()


def section(elf: ELFFile, name: str):
    found = [item for item in elf.sections if item.name == name]
    if len(found) != 1: fail(f"missing/duplicate section: {name}")
    return found[0]


def source_document() -> list[dict]: return [dict(row) for row in SOURCE_SECTIONS]


def semantic_document() -> list[dict]:
    return [{**row, "fdes": [{**fde, "ops": [list(op) for op in fde["ops"]]}
                              for fde in row["fdes"]]}
            for row in SEMANTIC_SECTIONS]


def internal_document() -> list[dict]: return [dict(row) for row in INTERNAL_FUNCTIONS]


def claims() -> dict[str, bool]:
    return {
        "window_36_exact": True,
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
        "format": FORMAT, "schema_version": SCHEMA,
        "prior_manifest_sha256": digest(stage3k.DEFAULT_MANIFEST.read_bytes()),
        "historical_data_manifest_sha256": digest(historical_data.DEFAULT_MANIFEST.read_bytes()),
        "source_hashes": SOURCE_HASHES, "source_sections": source_document(),
        "semantic_sections": semantic_document(), "internal_functions": internal_document(),
        "absorbed_fixed_sections": list(ABSORBED_FIXED), "result": result, "claims": claims(),
    }


def validate_fde_evidence() -> None:
    with (ROOT / "analysis/progress_targets.csv").open(newline="", encoding="utf-8") as handle:
        rows = {int(row["address"], 0): row for row in csv.DictReader(handle)}
    internal = {(row["address"], row["size"]) for row in INTERNAL_FUNCTIONS}
    seen_internal = set()
    for group in SEMANTIC_SECTIONS:
        for fde in group["fdes"]:
            row = rows.get(fde["pc"])
            if row is not None and row["status"] == "MATCHING":
                continue
            key = (fde["pc"], fde["size"])
            if key not in internal: fail(f"unproved semantic FDE at 0x{fde['pc']:08x}")
            seen_internal.add(key)
    if seen_internal != internal: fail("internal DSP function evidence roster drift")


def validate(args: argparse.Namespace) -> dict:
    stage3k.validate(stage3k.parse_args(["validate"]))
    validate_fde_evidence()
    try: doc = json.loads(args.manifest.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc: fail(f"cannot read window-36 manifest: {exc}")
    if doc.get("format") != FORMAT or doc.get("schema_version") != SCHEMA: fail("window-36 identity drift")
    if doc.get("prior_manifest_sha256") != digest(stage3k.DEFAULT_MANIFEST.read_bytes()): fail("prior checkpoint drift")
    if doc.get("historical_data_manifest_sha256") != digest(historical_data.DEFAULT_MANIFEST.read_bytes()): fail("historical data contract drift")
    if doc.get("source_hashes") != SOURCE_HASHES or doc.get("source_sections") != source_document(): fail("source contract drift")
    if doc.get("semantic_sections") != semantic_document(): fail("semantic contract drift")
    if doc.get("internal_functions") != internal_document(): fail("internal function contract drift")
    if doc.get("absorbed_fixed_sections") != list(ABSORBED_FIXED): fail("absorbed section roster drift")
    for key, value in EXPECTED.items():
        if doc.get("result", {}).get(key) != value: fail(f"frozen window-36 metric drift: {key}")
    if doc.get("result", {}).get("exact_chunk_indices") != EXACT_CHUNKS: fail("exact-window roster drift")
    if doc.get("claims") != claims(): fail("window-36 claim boundary drift")
    return doc


def prepare_sources(args: argparse.Namespace, compiler: Path) -> dict[str, Path]:
    stage3i.v47.ensure_git_commit(stage3i.v47.PS2DEV, stage3i.v47.PS2DEV_REPO, stage3i.v47.PS2DEV_COMMIT)
    newlib = stage3i.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    gcc_include = Path(run([compiler, "-print-file-name=include"])).resolve()
    old_build = stage3i.v52.BUILD
    try:
        stage3i.v52.BUILD = args.build_dir / "v52-rebuild"
        source_root, _original, layout = stage3i.v52.prepare_snes_layout()
        stage3i.v52.patch_sources(layout)
        compat = args.build_dir / "compat-v52"
        stage3i.v52.write_compat_headers(compat)
        flags = [*stage3i.v47.COMMON_FLAGS, "-Os", *stage3i.v47.SNES_DEFINES,
                 "-DZLIB", "-nostdinc",
                 *stage3i.v47.include_args([compat, newlib, layout, layout / "unzip",
                                            source_root / "zlib", gcc_include]), "-x", "c++"]
        result = {}
        output = args.build_dir / "source-objects"
        for filename in SOURCE_HASHES:
            source = layout / filename
            if digest(source.read_bytes()) != SOURCE_HASHES[filename]: fail(f"historical source hash drift: {filename}")
            obj = output / f"{Path(filename).stem}.o"
            stage3i.compile_one(compiler, flags, source, obj)
            result[filename] = obj
        return result
    finally:
        stage3i.v52.BUILD = old_build


def verify_internal_functions(obj: Path, reference: bytes) -> None:
    elf = ELFFile(obj)
    for row in INTERNAL_FUNCTIONS:
        symbol = elf.find_symbol(row["symbol"])
        if symbol.size != row["size"]: fail(f"internal DSP extent drift: {row['symbol']}")
        start = row["address"] - TARGET_BASE
        target = reference[start:start + row["size"]]
        if digest(target) != row["sha256"]: fail(f"internal DSP target fingerprint drift: {row['symbol']}")
        check = compare_function(reference, start, row["size"], elf, row["symbol"])
        if not check.matching or check.unknown_relocation_types:
            fail(f"internal DSP source comparison failed: {row['symbol']}")


def rebuild_payloads(objects: dict[str, Path], reference: bytes, build_dir: Path) -> list[dict]:
    rows = []
    for spec in SOURCE_SECTIONS:
        elf = ELFFile(objects[spec["filename"]]); item = section(elf, spec["source_section"])
        full = stage3i.section_bytes(elf, item)
        if len(full) != spec["full_size"] or digest(full) != spec["full_sha256"]:
            fail(f"full source section drift: {spec['name']}")
        start, size = spec["source_offset"], spec["size"]
        source = full[start:start + size]
        if len(source) != size or digest(source) != spec["raw_sha256"]: fail(f"source slice drift: {spec['name']}")
        relocs = [(off-start, kind, name) for off, kind, name in historical_data.relocations(elf, item.index)
                  if start <= off and off + 4 <= start + size]
        if len(relocs) != spec["relocations"] or any(kind != 2 for _off, kind, _name in relocs):
            fail(f"R_MIPS_32 roster drift: {spec['name']}")
        target = reference[spec["address"]-TARGET_BASE:spec["address"]-TARGET_BASE+size]
        patched, mask = bytearray(source), bytearray(size)
        for off, _kind, _name in relocs:
            mask[off:off+4] = b"\1" * 4
            patched[off:off+4] = target[off:off+4]
        if any(a != b for a, b, marked in zip(source, target, mask) if not marked):
            fail(f"source differs outside relocations: {spec['name']}")
        if bytes(patched) != target or digest(target) != spec["linked_sha256"]:
            fail(f"linked source payload drift: {spec['name']}")
        if spec["name"] == "cpu_opcode_tables":
            words = struct.unpack("<1024I", target)
            if any(word % 4 or not 0x00100000 <= word < 0x0033CE78 for word in words):
                fail("CPU opcode relocation result outside executable text")
        path = build_dir / f"payloads/{spec['name']}.bin"
        path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(patched)
        rows.append({**spec, "payload": path})
    return rows


def render_payload_source(rows: Sequence[dict]) -> str:
    lines = ["/* Generated from public source plus verified R_MIPS_32 results. */"]
    for row in rows:
        lines += [f'.section {row["section"]},"aw",@progbits', ".align 2", f'.incbin "{row["payload"]}"']
    return "\n".join(lines) + "\n"


def emit_ops(lines: list[str], fde: dict) -> None:
    for op in fde["ops"]:
        if op[0] == "advance_loc4": lines += [".byte 4", f".4byte {op[1]}"]
        elif op[0] == "def_cfa_offset": lines += [".byte 14", f".uleb128 {op[1]}"]
        elif op[0] == "offset_extended_sf": lines += [".byte 17", f".uleb128 {op[1]}", f".sleb128 {op[2]}"]
        else: fail(f"unknown CFI operation: {op[0]}")


def render_semantic_source() -> str:
    lines = ["/* Explicit DWARF semantics; contains no private target payload. */"]
    for group_index, group in enumerate(SEMANTIC_SECTIONS):
        frame = f".Lst3l_frame_{group_index}"
        lines += [f'.section {group["section"]},"aw",@progbits', ".align 2", f"{frame}:",
                  f".4byte .Lst3l_cie_end_{group_index}-.Lst3l_cie_body_{group_index}",
                  f".Lst3l_cie_body_{group_index}:", ".4byte 0", ".byte 1", '.ascii "zP\\0"',
                  ".uleb128 1", ".sleb128 4", ".byte 64", ".uleb128 5", ".byte 0",
                  ".4byte stage3l_gxx_personality", ".byte 12", ".uleb128 29", ".uleb128 0",
                  f".Lst3l_cie_end_{group_index}:"]
        for index, fde in enumerate(group["fdes"]):
            body = f".Lst3l_fde_body_{group_index}_{index}"
            end = f".Lst3l_fde_end_{group_index}_{index}"
            lines += [f".4byte {end}-{body}", f"{body}:", f".4byte {body}-{frame}",
                      f".4byte stage3l_pc_{fde['pc']:08x}", f".4byte 0x{fde['size']:x}", ".uleb128 0"]
            emit_ops(lines, fde); lines += [".align 2", f"{end}:"]
        lines += [".4byte 0"] * group["zero_words"]
    return "\n".join(lines) + "\n"


def verify_semantic_object(path: Path) -> int:
    elf = ELFFile(path); total = 0
    for row in SEMANTIC_SECTIONS:
        item = section(elf, row["section"])
        if item.type != 1 or item.size != row["size"]: fail(f"semantic section geometry drift: {row['name']}")
        total += sum(other.size // other.entry_size for other in elf.sections
                     if other.type in (4, 9) and other.info == item.index and other.entry_size)
    if total != EXPECTED["semantic_relocations"]: fail("semantic relocation count drift")
    return total


def absorbed_aliases(input_path: Path, sections: Sequence[dict], names: Sequence[str]) -> dict[str, int]:
    elf, result, seen = ELFFile(input_path), {}, set()
    by_name = {row["section"]: row for row in sections}
    for name in names:
        item = section(elf, name); seen.add(name); base = int(by_name[name]["target_address"], 0)
        for symbol in elf.symbols:
            if symbol.section_index != item.index or not symbol.name or symbol.info >> 4 not in (1, 2): continue
            address = base + symbol.value
            previous = result.setdefault(symbol.name, address)
            if previous != address: fail(f"absorbed symbol address conflict: {symbol.name}")
    if seen != set(names): fail("absorbed fixed-section roster incomplete")
    return result


def probe(args: argparse.Namespace) -> dict:
    prior = stage3k.validate(stage3k.parse_args(["validate"]))
    sections, layout = startup.load_inputs(startup.parse_args([
        "validate", "--sections", str(args.sections), "--layout", str(args.layout)]))
    reference = args.reference.read_bytes()
    if digest(reference) != TARGET_SHA256: fail("private unpacked reference SHA-256 drift")
    compiler = resolve_tool(args.compiler)
    if run([compiler, "-dumpversion"]) != "3.2.2" or run([compiler, "-dumpmachine"]) != "ee":
        fail("Stage-3L requires EE GCC 3.2.2 C++")
    linker = resolve_tool(args.ld) if args.ld else compiler.with_name("ee-ld")
    objcopy = resolve_tool(args.objcopy) if args.objcopy else compiler.with_name("ee-objcopy")
    required = [args.input, args.startup_object, args.stage3i_build/"frontend-eh-frames.o",
                args.stage3i_build/"providers.o", args.stage3i_build/"semantic-cfi.o",
                args.stage3j_build/"runtime-tail.o", args.stage3k_build/"tail-payloads.o",
                args.stage3k_build/"tail-semantics.o"]
    if not all(path.is_file() for path in required): fail("missing Stage-3I/J/K dependency; run make tail-metadata")
    data_backing.check_sections(args.input, sections); args.build_dir.mkdir(parents=True, exist_ok=True)
    objects = prepare_sources(args, compiler); verify_internal_functions(objects["DSP1.CPP"], reference)
    rows = rebuild_payloads(objects, reference, args.build_dir)
    payload_source = args.build_dir / "window36-payloads.S"
    payload_source.write_text(render_payload_source(rows), encoding="utf-8")
    payload_obj = args.build_dir / "window36-payloads.o"
    stage3i.compile_one(compiler, ("-G0","-EL","-mno-abicalls","-march=r5900","-mtune=r5900"), payload_source, payload_obj)
    semantic_source = args.build_dir / "window36-semantics.S"
    semantic_source.write_text(render_semantic_source(), encoding="utf-8")
    semantic_obj = args.build_dir / "window36-semantics.o"
    stage3i.compile_one(compiler, ("-G0","-EL","-mno-abicalls","-march=r5900","-mtune=r5900"), semantic_source, semantic_obj)
    verify_semantic_object(semantic_obj)

    aliases = stage3i.absorbed_symbol_aliases(args.input, sections)
    aliases.update(stage3k.absorbed_aliases(args.input, sections))
    aliases.update(absorbed_aliases(args.input, sections, ABSORBED_FIXED))
    prior_providers = [{"address": row["target_address"], "section": f".data.stage3i.source.{row['name']}"}
                       for row in stage3i.PROVIDERS]
    discarded = set(stage3k.ABSORBED_FIXED) | set(ABSORBED_FIXED)
    retained = [row for row in sections if row["section"] not in discarded]
    script = stage3i.render_linker_script(retained, prior_providers, aliases)
    marker = "  .bss.stage3g.crt0 0x00426e80"
    placements = [
        *({"section": row["section"], "address": row["address"]} for row in stage3k.stage3j.SECTIONS),
        *({"section": row["section"], "address": row["address"]} for row in stage3k.SOURCE_SECTIONS),
        *stage3k.SEMANTIC_SECTIONS,
        *({"section": row["section"], "address": row["address"]} for row in rows),
        *SEMANTIC_SECTIONS,
    ]
    insertion = "\n".join(f"  {row['section']} 0x{row['address']:08x} : {{ KEEP(*({row['section']})) }}"
                            for row in sorted(placements, key=lambda item: item["address"]))
    script = script.replace(marker, insertion + "\n" + marker, 1)
    discard_text = " ".join(f"*({name})" for name in sorted(discarded))
    script = script.replace("*(.data.stage3g.crt0)", f"{discard_text} *(.data.stage3g.crt0)", 1)
    for fde in stage3k.SPC_FDES:
        script = f"stage3k_pc_{fde['pc']:08x} = 0x{fde['pc']:08x};\n" + script
    script = f"stage3k_end_of_heap = 0x00450c18;\nstage3k_gxx_personality = 0x{stage3i.PERSONALITY:08x};\n" + script
    for group in SEMANTIC_SECTIONS:
        for fde in group["fdes"]:
            script = f"stage3l_pc_{fde['pc']:08x} = 0x{fde['pc']:08x};\n" + script
    script = f"stage3l_gxx_personality = 0x{PERSONALITY:08x};\n" + script
    linker_script = args.build_dir / "window36-data.ld"
    linker_script.write_text(script, encoding="utf-8")
    output = args.build_dir / "stage3l-window36-integrated.elf"
    run([linker, "-EL", "-T", linker_script, "-o", output, args.startup_object, args.input,
         args.stage3i_build/"frontend-eh-frames.o", args.stage3i_build/"providers.o",
         args.stage3i_build/"semantic-cfi.o", args.stage3j_build/"runtime-tail.o",
         args.stage3k_build/"tail-payloads.o", args.stage3k_build/"tail-semantics.o",
         payload_obj, semantic_obj])
    elf = ELFFile(output); startup.verify_symbols(elf); stage3i.verify_fixed_output(elf, retained)
    for row in placements:
        item = section(elf, row["section"])
        target = reference[row["address"]-TARGET_BASE:row["address"]-TARGET_BASE+item.size]
        if item.address != row["address"] or stage3i.section_bytes(elf, item) != target:
            fail(f"linked Stage-3L section differs: {row['section']}")
    raw_path = args.build_dir / "stage3l-window36-integrated.unpadded.bin"
    padded_path = args.build_dir / "stage3l-window36-integrated.padded.bin"
    run([objcopy, "-O", "binary", output, raw_path]); raw = raw_path.read_bytes()
    if len(raw) > len(reference): fail("integrated diagnostic exceeds target image size")
    padded = raw + bytes(len(reference)-len(raw)); padded_path.write_bytes(padded)
    exact, different = link_layout_probe.compare_chunks(padded, layout)
    differences = [index for index, (left, right) in enumerate(zip(padded, reference)) if left != right]
    result = {
        "source_sections": len(rows), "source_bytes": sum(row["size"] for row in rows),
        "source_relocations": sum(row["relocations"] for row in rows),
        "semantic_sections": len(SEMANTIC_SECTIONS),
        "semantic_bytes": sum(row["size"] for row in SEMANTIC_SECTIONS),
        "semantic_fdes": sum(len(row["fdes"]) for row in SEMANTIC_SECTIONS),
        "semantic_relocations": verify_semantic_object(semantic_obj),
        "internal_functions": len(INTERNAL_FUNCTIONS), "absorbed_fixed_sections": len(ABSORBED_FIXED),
        "chunk_count": len(exact)+len(different), "target_initialized_size": len(reference),
        "exact_chunks": len(exact), "mismatching_chunks": len(different),
        "exact_chunk_indices": exact, "mismatching_chunk_indices": different,
        "differing_bytes": len(differences),
        "window36_differing_bytes": sum(a != b for a, b in zip(padded[36*65536:37*65536], reference[36*65536:37*65536])),
        "first_differing_address": TARGET_BASE+differences[0],
        "integrated_padded_sha256": digest(padded), "target_sha256": digest(reference),
        "prior_differing_bytes": prior["result"]["differing_bytes"],
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
    parser.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    parser.add_argument("--compiler", default="ee-g++"); parser.add_argument("--ld"); parser.add_argument("--objcopy")
    args = parser.parse_args(argv)
    for name, value in vars(args).items():
        if isinstance(value, Path): setattr(args, name, value.expanduser().resolve())
    return args


def main(argv: Sequence[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "validate": doc = validate(args)
        else:
            result = probe(args); doc = frozen_document(result)
            if args.command == "capture":
                args.manifest.parent.mkdir(parents=True, exist_ok=True)
                args.manifest.write_text(json.dumps(doc, indent=2, sort_keys=True) + "\n", encoding="utf-8")
            elif validate(args) != doc: fail("private result differs from frozen manifest")
        result = doc["result"]
        print(f"verified window-36 data: source={result['source_sections']} sections/{result['source_bytes']} bytes/{result['source_relocations']} relocations; semantic={result['semantic_fdes']} FDEs")
        print(f"whole-image chunks={result['exact_chunks']}/51 remaining={result['mismatching_chunks']} differing_bytes={result['differing_bytes']} window36_differences={result['window36_differing_bytes']}; complete ELF verified separately")
        return 0
    except (Window36Error, stage3k.TailMetadataError, stage3i.HistoricalTailError,
            data_backing.DataBackingError, OSError, ValueError, KeyError, RuntimeError) as exc:
        print(f"window-36 data: FAIL -- {exc}"); return 1


if __name__ == "__main__": raise SystemExit(main())
