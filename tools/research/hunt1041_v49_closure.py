#!/usr/bin/env python3
"""Rebuild, verify, and optionally promote the 20 HUNT1041 V49 matches.

V49 deliberately requires the hash-pinned unpacked original.  Several rows
begin inside compiler symbols or end before the next audited manifest entry;
the private reference is therefore required to prove their complete historical
symbol context instead of inferring bytes from incomplete listings.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "research"))

from compare_elf_functions import ELFFile  # noqa: E402
from run_match_miner import (  # noqa: E402
    compile_source,
    compiler_identity,
    dependency_identity,
    has_terminal_control_flow,
)
import hunt1000plus_v46_closure as v46  # noqa: E402
import hunt1000plus_v47_closure as v47  # noqa: E402
from hunt1041_v48_closure import compare_slice  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
BUILD = ROOT / "build" / "matching" / "hunt1041-v49-closure"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v49-validated-20.tsv"
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"
MAX_TERMINAL_SIZE = 4096

EXACT = "exact-next-boundary"
TERMINAL = "terminal-control-flow-boundary"

EVIDENCE_FIELDS = (
    "address", "name", "area", "provenance", "source", "profile",
    "detail", "object", "object_symbol", "object_symbol_size",
    "object_offset", "object_size", "boundary", "result",
    "differing_bytes", "raw_equal", "normalized_equal",
    "unknown_relocations", "object_sha256", "cache_key", "target_gate",
    "target_span_sha256", "target_sources",
)


@dataclass(frozen=True)
class Piece:
    symbol: str
    symbol_size: int
    object_offset: int
    target_offset: int
    size: int
    full_target: int | None


@dataclass(frozen=True)
class Candidate:
    address: int
    expected_name: str
    object_key: str
    pieces: tuple[Piece, ...]
    boundary: str
    provenance: str
    detail: str


@dataclass(frozen=True)
class ObjectBuild:
    path: Path
    metadata: dict[str, object]


def one(
    symbol: str,
    symbol_size: int,
    object_offset: int,
    size: int,
    full_target: int | None,
) -> tuple[Piece, ...]:
    return (Piece(symbol, symbol_size, object_offset, 0, size, full_target),)


CANDIDATES = (
    Candidate(
        0x001000E0, "snes_p17_001000e0", "ps2lib:crt0",
        (
            Piece("_exit", 44, 0, 0, 44, 0x001000E0),
            Piece("_root", 8, 0, 44, 8, 0x0010010C),
        ),
        EXACT, "ps2sdk-694100b",
        "historical-object-symbol-partition-strict; identities=_exit+_root",
    ),
    Candidate(
        0x00101E8C, "snes_p20_00101e8c", "recovered:frontend",
        one("snes_p20_00101e8c", 100, 0, 100, None), EXACT,
        "snesstation-v0.23-recovered",
        "recovered-source-strict; identity=gs-vsint-wait-and-buffer-swap",
    ),
    Candidate(
        0x00104E58, "frontend_shutdown_00104e58", "recovered:shutdown",
        one("frontend_shutdown_00104e58", 36, 0, 36, None), EXACT,
        "snesstation-v0.23-recovered",
        "recovered-source-strict; corrected-identity=frontend-shutdown",
    ),
    Candidate(
        0x00106BCC, "is_sram_extension_00106bcc", "recovered:sram-extension",
        one("is_sram_extension_00106bcc", 60, 0, 60, None), EXACT,
        "snesstation-v0.23-recovered",
        "recovered-source-strict; corrected-identity=srm-extension-test",
    ),
    Candidate(
        0x0011C050, "snes_dispatch_0011c050", "snes:cpuops",
        one("_Z6OpD6M0v", 236, 176, 60, 0x0011BFA0), TERMINAL,
        "snes9x-1.41-1-official",
        "historical-symbol-tail-strict; historical-identity=OpD6M0",
    ),
    Candidate(
        0x0012BD48, "snes_p16_0012bd48", "snes:dma",
        one("_Z13REGISTER_2122h", 532, 0, 532, 0x0012BD48), EXACT,
        "snes9x-1.41-1-official",
        "historical-symbol-strict; historical-identity=REGISTER_2122@DMA",
    ),
    Candidate(
        0x0013E014, "snes_dispatch_0013e014", "snes:fxinst",
        one("_Z10fx_from_r6v", 152, 24, 16, 0x0013DFFC), EXACT,
        "snes9x-1.41-1-official",
        "historical-symbol-slice-strict; historical-identity=fx_from_r6",
    ),
    Candidate(
        0x0013E024, "snes_dispatch_0013e024", "snes:fxinst",
        one("_Z10fx_from_r6v", 152, 40, 112, 0x0013DFFC), TERMINAL,
        "snes9x-1.41-1-official",
        "historical-symbol-tail-strict; historical-identity=fx_from_r6",
    ),
    Candidate(
        0x0013E3A8, "snes_dispatch_0013e3a8", "snes:fxinst",
        one("_Z11fx_from_r12v", 152, 28, 124, 0x0013E38C), TERMINAL,
        "snes9x-1.41-1-official",
        "historical-symbol-tail-strict; historical-identity=fx_from_r12",
    ),
    Candidate(
        0x0013FC10, "snes_dispatch_0013fc10", "snes:fxinst",
        one("_Z9fx_xor_i4v", 112, 72, 16, 0x0013FBC8), TERMINAL,
        "snes9x-1.41-1-official",
        "historical-symbol-closed-terminal-prefix-strict; historical-identity=fx_xor_i4",
    ),
    Candidate(
        0x00140040, "snes_dispatch_00140040", "snes:fxinst",
        one("_Z10fx_xor_i14v", 112, 24, 88, 0x00140028), TERMINAL,
        "snes9x-1.41-1-official",
        "historical-symbol-tail-strict; historical-identity=fx_xor_i14",
    ),
    Candidate(
        0x00140C30, "snes_dispatch_00140c30", "snes:fxinst",
        one("_Z9fx_iwt_r0v", 116, 80, 36, 0x00140BE0), TERMINAL,
        "snes9x-1.41-1-official",
        "historical-symbol-tail-strict; historical-identity=fx_iwt_r0",
    ),
    Candidate(
        0x00142A78, "tile_lookup_table_init", "snes:gfx",
        one("S9xGraphicsInit", 1436, 0, 1436, 0x00142A78), TERMINAL,
        "snes9x-1.41-1-official",
        "historical-symbol-strict; corrected-identity=S9xGraphicsInit",
    ),
    Candidate(
        0x0014368C, "S9xSetupOBJ", "snes:gfx",
        one("_Z11S9xSetupOBJv", 332, 0, 244, 0x0014368C), EXACT,
        "snes9x-1.41-1-official",
        "historical-symbol-slice-strict; historical-identity=S9xSetupOBJ",
    ),
    Candidate(
        0x00143780, "renderer_4bpp_setup", "snes:gfx",
        one("_Z11S9xSetupOBJv", 332, 244, 88, 0x0014368C), EXACT,
        "snes9x-1.41-1-official",
        "historical-symbol-slice-strict; historical-identity=S9xSetupOBJ",
    ),
    Candidate(
        0x00144AD8, "DrawBackgroundMode5", "snes:gfx",
        one("_Z19DrawBackgroundMode5jjhh", 2008, 0, 1896, 0x00144AD8), EXACT,
        "snes9x-1.41-1-official",
        "historical-symbol-slice-strict; historical-identity=DrawBackgroundMode5",
    ),
    Candidate(
        0x00145240, "renderer_cache_select", "snes:gfx",
        one("_Z19DrawBackgroundMode5jjhh", 2008, 1896, 112, 0x00144AD8), EXACT,
        "snes9x-1.41-1-official",
        "historical-symbol-slice-strict; historical-identity=DrawBackgroundMode5",
    ),
    Candidate(
        0x0015D6D8, "snes_p16_0015d6d8", "snes:ppu-bgr555",
        one("_Z13REGISTER_2122h", 532, 0, 532, 0x0015D6D8), EXACT,
        "snes9x-1.41-1-official",
        "historical-symbol-strict; historical-identity=REGISTER_2122@PPU; pixel-format=BGR555",
    ),
    Candidate(
        0x0017EC24, "snes_leaf_0017ec24", "snes:spc700",
        one("_Z5Apu9Fv", 56, 16, 40, 0x0017EC14), TERMINAL,
        "snes9x-1.41-1-official",
        "historical-symbol-tail-strict; historical-identity=Apu9F",
    ),
    Candidate(
        0x001B0790, "gsDriver_getTexSizeFromInt", "pgen:gsfont",
        one("_ZN8gsDriver17getTexSizeFromIntEi", 68, 0, 68, 0x001B0790), TERMINAL,
        "pgen-libgs-a",
        "historical-symbol-strict; historical-identity=gsDriver::getTexSizeFromInt",
    ),
)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def rel(path: Path) -> str:
    return path.absolute().relative_to(ROOT.absolute()).as_posix()


def run(command: list[str]) -> None:
    result = subprocess.run(
        command, cwd=ROOT, text=True, stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT, check=False,
    )
    if result.returncode:
        raise SystemExit(
            f"command failed ({result.returncode}): {' '.join(command)}\n"
            f"{result.stdout[-6000:]}"
        )


def read_csv(path: Path) -> tuple[list[str], list[dict[str, str]]]:
    with path.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream)
        rows = list(reader)
        fields = list(reader.fieldnames or ())
    return fields, rows


def write_csv_atomic(
    path: Path,
    fields: list[str] | tuple[str, ...],
    rows: list[dict[str, str]],
    *,
    delimiter: str = ",",
) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + ".tmp")
    with temporary.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(
            stream, fieldnames=fields, delimiter=delimiter, lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(rows)
    os.replace(temporary, path)


def recovered_object(
    compiler: Path,
    compiler_id: str,
    dependency_id: str,
    source: Path,
) -> ObjectBuild:
    result = compile_source(
        compiler, compiler_id, dependency_id, BUILD / "recovered", "os", source
    )
    if result.returncode:
        output = result.log_path.read_text(encoding="utf-8", errors="replace")
        raise SystemExit(f"recovered candidate compile failed:\n{output[-6000:]}")
    metadata = json.loads(result.object_path.with_suffix(".json").read_text())
    return ObjectBuild(result.object_path, metadata)


def historical_object(
    compiler: Path,
    source: Path,
    output: Path,
    profile: str,
    flags: list[str],
    upstream: str,
    *,
    evidence_source: Path | None = None,
    extra_inputs: tuple[Path, ...] = (),
) -> ObjectBuild:
    path, metadata = v47.compile_one(
        compiler, source, output, profile, flags, upstream,
        evidence_source=evidence_source, extra_inputs=extra_inputs,
    )
    return ObjectBuild(path, metadata)


def build_objects(cc: Path, cxx: Path) -> dict[str, ObjectBuild]:
    compiler_id = compiler_identity(cc)
    dependency_id = dependency_identity()
    objects = {
        "recovered:frontend": recovered_object(
            cc, compiler_id, dependency_id,
            ROOT / "src" / "ps2" / "progress11_frontend_recovered.c",
        ),
        "recovered:shutdown": recovered_object(
            cc, compiler_id, dependency_id,
            ROOT / "src" / "ps2" / "libmtap_recovered.c",
        ),
        "recovered:sram-extension": recovered_object(
            cc, compiler_id, dependency_id,
            ROOT / "src" / "ps2" / "audio_rpc_recovered.c",
        ),
    }

    v47.ensure_git_commit(v47.PGEN, v47.PGEN_REPO, v47.PGEN_COMMIT)
    v47.ensure_git_commit(v47.PS2DEV, v47.PS2DEV_REPO, v47.PS2DEV_COMMIT)
    v47.ensure_git_commit(
        v47.PS2SDK_20040415, v47.PS2SDK_REPO, v47.PS2SDK_20040415_COMMIT
    )
    try:
        archive = v46.download_archive(v46.SNES_ARCHIVE, v46.SNES_CACHE)
        snes_root = v46.safe_extract_archive(
            archive, v46.SNES_CACHE / "source", v46.SNES_ARCHIVE.source_directory
        )
    except v46.BuildFailure as exc:
        raise SystemExit(str(exc)) from exc

    snes = snes_root / "snes9x"
    newlib = (
        v47.PS2DEV / "ps2toolchain" / "soft" / "newlib-1.10.0"
        / "newlib" / "libc" / "include"
    )
    compat = BUILD / "compat" / "snes"
    v47.atomic_write_text(
        compat / "memory.h",
        "#ifndef HUNT1041_V49_MEMORY_H\n#define HUNT1041_V49_MEMORY_H\n"
        "#include <string.h>\n#endif\n",
    )
    snes_flags = [
        *v46.COMMON_FLAGS, "-Os", *v46.SNES_DEFINES,
        *v46.include_args([compat, newlib, snes, snes / "unzip", snes_root / "zlib"]),
        "-x", "c++",
    ]
    for key, filename in (
        ("snes:cpuops", "CPUOPS.CPP"),
        ("snes:fxinst", "fxinst.cpp"),
        ("snes:spc700", "SPC700.CPP"),
    ):
        objects[key] = historical_object(
            cxx, snes / filename,
            BUILD / "historical" / f"{key.split(':')[1]}.o",
            "snes9x-1.41-1-os-short", snes_flags,
            f"lysator:{v46.SNES_ARCHIVE.sha256}",
            extra_inputs=(compat / "memory.h",),
        )

    bgr_snes = BUILD / "historical" / "snes9x-1.41-1-bgr555"
    if bgr_snes.exists():
        shutil.rmtree(bgr_snes)
    shutil.copytree(snes, bgr_snes)
    port = bgr_snes / "port.h"
    port_text = port.read_text(encoding="latin-1")
    marker = "/* #define PIXEL_FORMAT RGB565 */\n#define GFX_MULTI_FORMAT"
    if port_text.count(marker) != 1:
        raise SystemExit("Snes9x 1.41-1 BGR555 patch context changed")
    v47.atomic_write_text(
        port,
        port_text.replace(
            marker,
            "#define PIXEL_FORMAT BGR555\n"
            "/* Fixed 15-bit pixel profile used by the PS2 frontend. */",
        ),
    )
    bgr_flags = [
        *v46.COMMON_FLAGS, "-Os", *v46.SNES_DEFINES,
        *v46.include_args([
            compat, newlib, bgr_snes, bgr_snes / "unzip", snes_root / "zlib"
        ]),
        "-x", "c++",
    ]
    for key, filename in (
        ("snes:dma", "DMA.CPP"),
        ("snes:gfx", "GFX.CPP"),
        ("snes:ppu-bgr555", "ppu.cpp"),
    ):
        objects[key] = historical_object(
            cxx, bgr_snes / filename,
            BUILD / "historical" / f"{key.split(':')[1]}.o",
            "snes9x-1.41-1-os-short-bgr555", bgr_flags,
            f"lysator:{v46.SNES_ARCHIVE.sha256}",
            evidence_source=snes / filename,
            extra_inputs=(port, compat / "memory.h"),
        )

    ar = cc.with_name("ee-ar")
    if not ar.is_file():
        raise SystemExit(f"missing EE archiver: {ar}")
    gs_archive = v47.PGEN / "lib" / "gslib051" / "lib" / "libgs.a"
    gs_path, gs_metadata = v46.extract_one(
        ar, gs_archive, "gsFont.o", BUILD / "historical" / "gsFont.o",
        gs_archive, "prebuilt-archive", v47.PGEN_COMMIT,
    )
    objects["pgen:gsfont"] = ObjectBuild(gs_path, gs_metadata)

    crt0 = v47.PS2SDK_20040415 / "ee" / "startup" / "src" / "crt0.s"
    objects["ps2lib:crt0"] = historical_object(
        cc, crt0, BUILD / "historical" / "crt0.o",
        "ps2lib-20040415-startup", ["-G0", "-EL"],
        v47.PS2SDK_20040415_COMMIT,
    )
    return objects


def closed_terminal_prefix(data: bytes, endian: str) -> bool:
    if len(data) < 8 or len(data) % 4:
        return False
    byte_order = "little" if endian == "<" else "big"
    words = [
        int.from_bytes(data[offset:offset + 4], byte_order)
        for offset in range(0, len(data), 4)
    ]
    if words[-2] != 0x03E00008:  # jr $ra; final word is its delay slot
        return False
    for word in words[:-2]:
        opcode = word >> 26
        funct = word & 0x3F
        if opcode in {1, 2, 3, 4, 5, 6, 7}:
            return False
        if opcode == 0 and funct in {8, 9}:  # jr/jalr
            return False
    return True


def make_evidence(
    target: bytes,
    objects: dict[str, ObjectBuild],
) -> list[dict[str, str]]:
    _, manifest_rows = read_csv(TARGETS)
    manifest = {int(row["address"], 0): row for row in manifest_rows}
    starts = sorted(manifest)
    next_address = {start: end for start, end in zip(starts, starts[1:])}
    rows: list[dict[str, str]] = []

    for spec in sorted(CANDIDATES, key=lambda item: item.address):
        target_row = manifest.get(spec.address)
        end = next_address.get(spec.address)
        if target_row is None:
            raise SystemExit(f"0x{spec.address:08x}: missing audited target")
        if target_row["name"] != spec.expected_name:
            raise SystemExit(f"0x{spec.address:08x}: manifest identity changed")
        if target_row["status"] not in {"RECONSTRUCTED", "MATCHING"}:
            raise SystemExit(f"0x{spec.address:08x}: unexpected manifest status")

        ordered = sorted(spec.pieces, key=lambda piece: piece.target_offset)
        cursor = 0
        for piece in ordered:
            if piece.target_offset != cursor or piece.size <= 0 or piece.size % 4:
                raise SystemExit(f"0x{spec.address:08x}: non-contiguous piece partition")
            cursor += piece.size
        span = None if end is None else end - spec.address
        if spec.boundary == EXACT:
            if span is None or cursor != span:
                raise SystemExit(f"0x{spec.address:08x}: exact boundary changed")
        elif spec.boundary == TERMINAL:
            if cursor > MAX_TERMINAL_SIZE or (span is not None and cursor >= span):
                raise SystemExit(f"0x{spec.address:08x}: invalid terminal range")
        else:
            raise SystemExit(f"0x{spec.address:08x}: unsupported boundary mode")

        built = objects[spec.object_key]
        elf = ELFFile(built.path)
        candidate_parts: list[bytes] = []
        raw_equal = True
        relocation_count = 0
        unknown: set[int] = set()
        for piece in ordered:
            symbol = elf.find_symbol(piece.symbol)
            if symbol.size != piece.symbol_size:
                raise SystemExit(
                    f"0x{spec.address:08x}: {piece.symbol} size changed from "
                    f"{piece.symbol_size} to {symbol.size}"
                )
            if piece.object_offset + piece.size > symbol.size:
                raise SystemExit(f"0x{spec.address:08x}: object slice exceeds symbol")
            piece_address = spec.address + piece.target_offset
            result = compare_slice(
                target, piece_address, elf, piece.symbol,
                piece.object_offset, piece.size,
            )
            if not result["normalized_equal"] or result["differing_bytes"]:
                first = ",".join(
                    f"+0x{value:x}" for value in result["first_differences"]
                )
                raise SystemExit(
                    f"0x{piece_address:08x}: strict comparison failed ({first})"
                )
            if result["unknown"]:
                unknown.update(result["unknown"])
            raw_equal = raw_equal and bool(result["raw_equal"])
            relocation_count += int(result["relocations"])
            full = elf.symbol_bytes(symbol, symbol.size)
            candidate_parts.append(
                full[piece.object_offset:piece.object_offset + piece.size]
            )

            if piece.full_target is not None:
                if piece.full_target + piece.object_offset != piece_address:
                    raise SystemExit(f"0x{piece_address:08x}: full-symbol anchor changed")
                context = compare_slice(
                    target, piece.full_target, elf, piece.symbol, 0, symbol.size
                )
                if not context["normalized_equal"] or context["differing_bytes"]:
                    raise SystemExit(
                        f"0x{piece.full_target:08x}: full historical symbol context failed"
                    )
                if context["unknown"]:
                    unknown.update(context["unknown"])

        if unknown:
            raise SystemExit(f"0x{spec.address:08x}: unknown relocation type")
        candidate_bytes = b"".join(candidate_parts)
        if spec.boundary == TERMINAL:
            only = ordered[0]
            reaches_symbol_end = only.object_offset + only.size == only.symbol_size
            if reaches_symbol_end:
                if not has_terminal_control_flow(candidate_bytes, elf.endian):
                    raise SystemExit(f"0x{spec.address:08x}: terminal flow missing")
            elif "closed-terminal-prefix" in spec.detail:
                if not closed_terminal_prefix(candidate_bytes, elf.endian):
                    raise SystemExit(f"0x{spec.address:08x}: open terminal prefix")
            else:
                raise SystemExit(f"0x{spec.address:08x}: terminal range is not a symbol tail")

        metadata = built.metadata
        target_offset = spec.address - TARGET_BASE
        target_span = target[target_offset:target_offset + cursor]
        piece_detail = "+".join(
            f"{piece.symbol}[0x{piece.object_offset:x}:0x{piece.object_offset + piece.size:x}]"
            for piece in ordered
        )
        rows.append(
            {
                "address": f"0x{spec.address:08x}",
                "name": target_row["name"],
                "area": target_row["area"],
                "provenance": spec.provenance,
                "source": str(metadata["source"]),
                "profile": str(metadata["profile"]),
                "detail": (
                    f"{spec.detail}; pieces={piece_detail}; "
                    f"relocations={relocation_count}; formal-full-symbol-context=True"
                ),
                "object": rel(built.path),
                "object_symbol": "+".join(piece.symbol for piece in ordered),
                "object_symbol_size": "+".join(str(piece.symbol_size) for piece in ordered),
                "object_offset": "+".join(str(piece.object_offset) for piece in ordered),
                "object_size": str(cursor),
                "boundary": spec.boundary if end is not None else "terminal-control-flow-final-manifest",
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(raw_equal),
                "normalized_equal": "True",
                "unknown_relocations": "",
                "object_sha256": sha256_file(built.path),
                "cache_key": str(metadata["cache_key"]),
                "target_gate": f"formal-unpacked-elf:{TARGET_SHA256}",
                "target_span_sha256": hashlib.sha256(target_span).hexdigest(),
                "target_sources": "build/SNES_EMU.unpacked.bin",
            }
        )

    expected = {f"0x{candidate.address:08x}" for candidate in CANDIDATES}
    if len(CANDIDATES) != 20 or len(rows) != 20:
        raise SystemExit("V49 evidence cardinality gate failed")
    if {row["address"] for row in rows} != expected:
        raise SystemExit("V49 evidence address gate failed")
    return rows


def write_evidence(rows: list[dict[str, str]]) -> None:
    write_csv_atomic(EVIDENCE, EVIDENCE_FIELDS, rows, delimiter="\t")


def promote(rows: list[dict[str, str]]) -> int:
    target_fields, target_rows = read_csv(TARGETS)
    symbol_fields, symbol_rows = read_csv(SYMBOLS)
    targets = {row["address"].lower(): row for row in target_rows}
    symbols = {row["address"].lower(): row for row in symbol_rows}
    if len(targets) != len(target_rows) or len(symbols) != len(symbol_rows):
        raise SystemExit("duplicate address in manifest")
    if set(targets) != set(symbols):
        raise SystemExit("target and symbol manifests have different address sets")

    changed = 0
    for evidence in rows:
        address = evidence["address"]
        target = targets.get(address)
        symbol = symbols.get(address)
        if target is None or symbol is None:
            raise SystemExit(f"{address}: evidence address absent from manifests")
        for field in ("name", "status", "confidence", "notes"):
            if target[field] != symbol[field]:
                raise SystemExit(f"{address}: manifest mismatch for {field}")
        if evidence["name"] != target["name"] or evidence["area"] != target["area"]:
            raise SystemExit(f"{address}: evidence identity mismatch")
        mode = evidence["detail"].split(";", 1)[0]
        note = (
            "HUNT1041 V49 closure strict MATCH; "
            f"mode={mode}; provenance={evidence['provenance']}; "
            f"source={evidence['source']}; profile={evidence['profile']}; "
            f"object_symbol={evidence['object_symbol']}; "
            f"object_offset={evidence['object_offset']}; "
            f"boundary={evidence['boundary']}; target_gate={evidence['target_gate']}; "
            "differing_bytes=0; normalized_equal=True; "
            "unknown_relocations=none; "
            f"evidence={rel(EVIDENCE)}"
        )
        if target["status"] == "MATCHING":
            marker = "HUNT1041 V49 closure strict MATCH;"
            for manifest_row in (target, symbol):
                prefix, separator, _old_suffix = manifest_row["notes"].partition(marker)
                if separator:
                    manifest_row["notes"] = prefix.rstrip("; ") + "; " + note
            continue
        if target["status"] != "RECONSTRUCTED":
            raise SystemExit(f"{address}: unexpected status {target['status']}")
        for manifest_row in (target, symbol):
            manifest_row["status"] = "MATCHING"
            manifest_row["confidence"] = "very-high"
            manifest_row["notes"] = manifest_row["notes"].rstrip("; ") + "; " + note
        changed += 1

    write_csv_atomic(TARGETS, target_fields, target_rows)
    write_csv_atomic(SYMBOLS, symbol_fields, symbol_rows)
    run([sys.executable, str(ROOT / "tools" / "audit_source_completeness.py")])
    run([sys.executable, str(ROOT / "tools" / "update_progress.py")])
    return changed


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--cc", type=Path,
        default=ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-stage1"
        / "prefix" / "bin" / "ee-gcc",
    )
    parser.add_argument(
        "--cxx", type=Path,
        default=ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-cxx-stage1"
        / "prefix" / "bin" / "ee-g++",
    )
    parser.add_argument(
        "--apply", action="store_true",
        help="promote the 20 validated rows after writing evidence",
    )
    args = parser.parse_args()
    cc = args.cc.expanduser().resolve()
    cxx = args.cxx.expanduser().resolve()
    for path, label in ((cc, "EE C compiler"), (cxx, "EE C++ compiler")):
        if not path.is_file():
            raise SystemExit(f"missing {label}: {path}")
        run([
            sys.executable, str(ROOT / "tools" / "probe_ee_toolchain.py"),
            "--compiler", str(path),
        ])
    if not REFERENCE.is_file():
        raise SystemExit("missing formal unpacked reference; run make reference")
    if sha256_file(REFERENCE) != TARGET_SHA256:
        raise SystemExit("unpacked target SHA-256 mismatch")

    objects = build_objects(cc, cxx)
    rows = make_evidence(REFERENCE.read_bytes(), objects)
    write_evidence(rows)
    exact = sum(row["boundary"] == EXACT for row in rows)
    terminal = len(rows) - exact
    print(f"V49 strict matches: {len(rows)} (exact={exact}, terminal={terminal})")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print(f"evidence: {rel(EVIDENCE)}")
    if args.apply:
        print(f"promoted strict matches: {promote(rows)}")
    else:
        print("dry promotion; pass --apply to update the manifests")


if __name__ == "__main__":
    main()
