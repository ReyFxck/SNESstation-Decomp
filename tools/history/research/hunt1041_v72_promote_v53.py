#!/usr/bin/env python3
"""Rebuild, verify, and optionally promote the six recovered V53 results.

The target gate is the hash-pinned unpacked SNES Station v0.23 image.  Every
row must cover one exact audited boundary, compare with zero non-relocation
differences, and contain no unknown MIPS relocation type.  The two DMA rows
are explicit partitions of one contiguous historical S9xDoDMA symbol.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import shutil
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "research"))
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))

from compare_elf_functions import ELFFile  # noqa: E402
import hunt1000plus_v47_closure as v47  # noqa: E402
import hunt1041_v51_closure as v51  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
BUILD = ROOT / "build" / "matching" / "hunt1041-v72-v53-promotion"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v72-validated-v53-6.tsv"
RECOVERED_SPANS = (
    ROOT / "analysis" / "matching" / "hunt1041-v53-recovered-target-spans.tsv"
)
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"
EXACT = "exact-next-boundary"
PROVENANCE = "snes9x-1.41-1-official"

EVIDENCE_FIELDS = (
    "address",
    "name",
    "historical_identity",
    "area",
    "provenance",
    "source",
    "profile",
    "detail",
    "object",
    "object_symbol",
    "object_symbol_size",
    "object_offset",
    "object_size",
    "boundary",
    "result",
    "differing_bytes",
    "raw_equal",
    "normalized_equal",
    "unknown_relocations",
    "relocation_count",
    "object_sha256",
    "cache_key",
    "target_gate",
    "target_span_sha256",
)


@dataclass(frozen=True)
class Candidate:
    address: int
    expected_name: str
    historical_identity: str
    area: str
    object_key: str
    symbol: str
    symbol_size: int
    object_offset: int
    size: int
    detail: str


CANDIDATES = (
    Candidate(
        0x00129AF4,
        "snes_p17_00129af4",
        "S9xDoDMA partition 1/2",
        "snes-core-dsp",
        "dma",
        "S9xDoDMA",
        6388,
        0,
        2316,
        "historical-symbol-partition-strict; historical-identity=S9xDoDMA; partition=1/2",
    ),
    Candidate(
        0x0012A400,
        "ppu_vram_cache_invalidate",
        "S9xDoDMA partition 2/2",
        "ppu",
        "dma",
        "S9xDoDMA",
        6388,
        2316,
        4072,
        "historical-symbol-partition-strict; historical-identity=S9xDoDMA; partition=2/2",
    ),
    Candidate(
        0x0015068C,
        "snes_p17_0015068c",
        "LoadZip",
        "renderer",
        "loadzip",
        "_Z7LoadZipPKcPiS1_",
        1168,
        0,
        1168,
        "historical-symbol-strict; historical-identity=LoadZip; assert-profile=printf-plus-fatal",
    ),
    Candidate(
        0x00158B74,
        "snes_p16_00158b74",
        "SetOBC1",
        "memory-ppu",
        "obc1",
        "SetOBC1",
        1116,
        0,
        1116,
        "historical-symbol-strict; historical-identity=SetOBC1; access=packed-lwl-lwr; write=bytewise",
    ),
    Candidate(
        0x00181BAC,
        "snes_p17_00181bac",
        "S9xSetSPC7110",
        "cpu-audio-runtime",
        "spc7110",
        "_Z13S9xSetSPC7110ht",
        2480,
        0,
        2480,
        "historical-symbol-strict; historical-identity=S9xSetSPC7110; target-time=int32; wall-clock=zero",
    ),
    Candidate(
        0x00182638,
        "snes_p17_00182638",
        "S9xUpdateRTC",
        "cpu-audio-runtime",
        "spc7110",
        "_Z12S9xUpdateRTCv",
        728,
        0,
        728,
        "historical-symbol-strict; historical-identity=S9xUpdateRTC; target-time=int32; wall-clock=zero",
    ),
)


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT.resolve()).as_posix()


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def replace_once(path: Path, marker: str, replacement: str, label: str) -> None:
    text = path.read_text(encoding="latin-1")
    if text.count(marker) != 1:
        raise SystemExit(f"Snes9x 1.41-1 {label} patch context changed")
    v47.atomic_write_text(path, text.replace(marker, replacement))


def patch_base_layout(layout: Path) -> None:
    """Apply previously proven PS2 layout choices needed by the V53 objects."""
    getset = layout / "getset.h"
    function_marker = "INLINE uint16 S9xGetWord (uint32 Address)"
    replace_once(
        getset,
        function_marker,
        "struct Hunt1041UnalignedUint32\n"
        "{\n"
        "    uint32 value;\n"
        "} __attribute__((packed));\n\n"
        + function_marker,
        "S9xGetWord packed access type",
    )
    replace_once(
        getset,
        "#ifdef FAST_LSB_WORD_ACCESS\n"
        "\t\treturn (*(uint16 *) (GetAddress + (Address & 0xffff)));\n"
        "#else",
        "#ifdef FAST_LSB_WORD_ACCESS\n"
        "\t\tregister uint32 value __asm__ (\"$16\") =\n"
        "\t\t\t((Hunt1041UnalignedUint32 *)\n"
        "\t\t\t (GetAddress + (Address & 0xffff)))->value;\n"
        "\t\tvalue &= 0xffff;\n"
        "\t\t__asm__ (\"\" : \"+r\" (value));\n"
        "\t\treturn value;\n"
        "#else",
        "S9xGetWord target access",
    )
    replace_once(
        layout / "port.h",
        "/* #define PIXEL_FORMAT RGB565 */\n#define GFX_MULTI_FORMAT",
        "#define PIXEL_FORMAT BGR555\n"
        "/* Fixed 15-bit pixel profile used by the PS2 frontend. */",
        "BGR555 pixel format",
    )
    replace_once(
        layout / "snes9x.h",
        "  /*   uint32 StrikeGunnerOffsetHack; */\n"
        "    bool8  ChuckRock;",
        "  /*   uint32 StrikeGunnerOffsetHack; */\n"
        "    bool8  PS2PortLayoutByte;\n"
        "    bool8  ChuckRock;",
        "Settings one-byte PS2 layout",
    )


def patch_v53_sources(layout: Path) -> None:
    memmap = layout / "MEMMAP.H"
    replace_once(
        memmap,
        '#include "snes9x.h"\n',
        '#include "snes9x.h"\n\n'
        "struct Hunt1041V72UnalignedWord\n"
        "{\n"
        "    uint32 value;\n"
        "} __attribute__((packed));\n",
        "OBC1 packed access type",
    )
    replace_once(
        memmap,
        "#else\n"
        "#define READ_WORD(s) ( *(uint8 *) (s) |\\\n"
        "\t\t      (*((uint8 *) (s) + 1) << 8))",
        "#else\n"
        "#define READ_WORD(s) ({ \\\n"
        "    register uint8 *source = (uint8 *) (s); \\\n"
        "    __asm__ (\"\" : \"+r\" (source)); \\\n"
        "    register uint32 value __asm__ (\"$6\") = \\\n"
        "        ((Hunt1041V72UnalignedWord *) source)->value; \\\n"
        "    value &= 0xffff; \\\n"
        "    __asm__ (\"\" : \"+r\" (value) : : \"memory\"); \\\n"
        "    register uint32 result __asm__ (\"$3\"); \\\n"
        "    __asm__ (\"andi %0,%1,0xffff\" : \"=r\" (result) : \"r\" (value)); \\\n"
        "    result; \\\n"
        "})",
        "OBC1 target unaligned read",
    )

    spc_header = layout / "spc7110.h"
    replace_once(
        spc_header,
        "\ttime_t last_used;",
        "\tint32 last_used;",
        "SPC7110 persisted clock width",
    )
    spc_source = layout / "spc7110.cpp"
    replace_once(
        spc_source,
        "\ttime_t\tcur_systime;",
        "\tint32\tcur_systime;",
        "SPC7110 local clock width",
    )
    replace_once(
        spc_source,
        "        cur_systime = time (NULL);",
        "        cur_systime = 0;",
        "SPC7110 RTC wall clock",
    )
    lines = spc_source.read_text(encoding="latin-1").splitlines(keepends=True)
    replacements = 0
    for index, line in enumerate(lines):
        if "time(NULL)" in line and not line.lstrip().startswith("//"):
            lines[index] = line.replace("time(NULL)", "0")
            replacements += 1
    if replacements != 8:
        raise SystemExit(
            "Snes9x 1.41-1 SPC7110 register clock patch context changed "
            f"(expected 8, got {replacements})"
        )
    v47.atomic_write_text(spc_source, "".join(lines))


def prepare_layout() -> tuple[Path, Path, Path, Path]:
    try:
        archive = v47.download_archive(v47.SNES_141_1_ARCHIVE, v47.SNES_CACHE)
        source_root = v47.safe_extract_archive(
            archive,
            v47.SNES_CACHE / "source-1.41-1",
            v47.SNES_141_1_ARCHIVE.source_directory,
        )
    except v47.BuildFailure as exc:
        raise SystemExit(str(exc)) from exc

    v47.ensure_git_commit(v47.PS2DEV, v47.PS2DEV_REPO, v47.PS2DEV_COMMIT)
    newlib = (
        v47.PS2DEV
        / "ps2toolchain"
        / "soft"
        / "newlib-1.10.0"
        / "newlib"
        / "libc"
        / "include"
    )
    original = source_root / "snes9x"
    for path, label in ((original, "Snes9x source"), (newlib, "Newlib headers")):
        if not path.is_dir():
            raise SystemExit(f"missing {label}: {path}")

    layout = BUILD / "historical" / "snes9x-target-layout"
    if layout.exists():
        shutil.rmtree(layout)
    shutil.copytree(original, layout)
    patch_base_layout(layout)
    patch_v53_sources(layout)
    return source_root, original, layout, newlib


def write_compat_headers(compat: Path) -> tuple[Path, ...]:
    memory = compat / "memory.h"
    string = compat / "string.h"
    stdio = compat / "stdio.h"
    v47.atomic_write_text(
        memory,
        "#ifndef HUNT1041_V72_MEMORY_H\n#define HUNT1041_V72_MEMORY_H\n"
        "#include <string.h>\n#endif\n",
    )
    v47.atomic_write_text(
        string,
        "#ifndef HUNT1041_V72_STRING_H\n#define HUNT1041_V72_STRING_H\n"
        "#define memchr snes_hidden_memchr\n#define memcmp snes_hidden_memcmp\n"
        "#define memcpy snes_hidden_memcpy\n#define memmove snes_hidden_memmove\n"
        "#define memset snes_hidden_memset\n#define strcat snes_hidden_strcat\n"
        "#define strchr snes_hidden_strchr\n#define strcmp snes_hidden_strcmp\n"
        "#define strcpy snes_hidden_strcpy\n#define strlen snes_hidden_strlen\n"
        "#define strncat snes_hidden_strncat\n#define strncmp snes_hidden_strncmp\n"
        "#define strncpy snes_hidden_strncpy\n#define strrchr snes_hidden_strrchr\n"
        "#include_next <string.h>\n"
        "#undef memchr\n#undef memcmp\n#undef memcpy\n#undef memmove\n#undef memset\n"
        "#undef strcat\n#undef strchr\n#undef strcmp\n#undef strcpy\n#undef strlen\n"
        "#undef strncat\n#undef strncmp\n#undef strncpy\n#undef strrchr\n"
        "extern \"C\" {\n"
        "void *memchr(const void *, int, unsigned int);\n"
        "int memcmp(const void *, const void *, unsigned int);\n"
        "void *memcpy(void *, const void *, unsigned int);\n"
        "void *memmove(void *, const void *, unsigned int);\n"
        "void *memset(void *, int, unsigned int);\n"
        "char *strcat(char *, const char *);\nchar *strchr(const char *, int);\n"
        "int strcmp(const char *, const char *);\nchar *strcpy(char *, const char *);\n"
        "unsigned int strlen(const char *);\n"
        "char *strncat(char *, const char *, unsigned int);\n"
        "int strncmp(const char *, const char *, unsigned int);\n"
        "char *strncpy(char *, const char *, unsigned int);\n"
        "char *strrchr(const char *, int);\n}\n#endif\n",
    )
    v47.atomic_write_text(
        stdio,
        "#ifndef HUNT1041_V72_STDIO_H\n#define HUNT1041_V72_STDIO_H\n"
        "#define fread snes_hidden_fread\n#define fwrite snes_hidden_fwrite\n"
        "#include_next <stdio.h>\n#undef fread\n#undef fwrite\n"
        "extern \"C\" {\n"
        "unsigned int fread(void *, unsigned int, unsigned int, FILE *);\n"
        "unsigned int fwrite(const void *, unsigned int, unsigned int, FILE *);\n"
        "}\n#endif\n",
    )
    assertion = compat / "assert.h"
    v47.atomic_write_text(
        assertion,
        "#ifndef HUNT1041_V72_ASSERT_H\n#define HUNT1041_V72_ASSERT_H\n\n"
        "#include <stdio.h>\n\n"
        "extern void S9xExit() __attribute__((noreturn));\n\n"
        "#ifdef NDEBUG\n"
        "#define assert(condition) ((void) 0)\n"
        "#else\n"
        "#define assert(condition) \\\n"
        "    ((condition) ? (void) 0 : \\\n"
        "     (printf(\"%s:%u: failed assertion\\n\", __FILE__, __LINE__ + 8), S9xExit()))\n"
        "#endif\n\n#endif\n",
    )
    return memory, string, stdio, assertion


def include_args(paths: tuple[Path, ...]) -> list[str]:
    result: list[str] = []
    for path in paths:
        result.extend(("-I", rel(path)))
    return result


def compile_one(
    compiler: Path,
    source: Path,
    output: Path,
    profile: str,
    flags: list[str],
    evidence_source: Path,
    extra_inputs: tuple[Path, ...],
) -> v51.ObjectBuild:
    output.parent.mkdir(parents=True, exist_ok=True)
    v47.run(
        [str(compiler), *flags, "-c", rel(source), "-o", rel(output)],
        cwd=ROOT,
    )
    payload = {
        "compiler_sha256": v51.sha256_file(compiler),
        "flags": flags,
        "source_sha256": v51.sha256_file(source),
        "extra_inputs": {rel(path): v51.sha256_file(path) for path in extra_inputs},
        "upstream": v47.SNES_141_1_ARCHIVE.sha256,
    }
    metadata = {
        "cache_key": hashlib.sha256(
            json.dumps(payload, sort_keys=True, separators=(",", ":")).encode()
        ).hexdigest(),
        "source": rel(evidence_source),
        "profile": profile,
    }
    v47.atomic_write_text(
        output.with_suffix(".json"),
        json.dumps(metadata, indent=2, sort_keys=True) + "\n",
    )
    return v51.ObjectBuild(output, metadata)


def build_objects(cxx: Path) -> dict[str, v51.ObjectBuild]:
    source_root, original, layout, newlib = prepare_layout()
    compat = BUILD / "compat"
    compat_inputs = write_compat_headers(compat)
    includes = (compat, newlib, layout, layout / "unzip", source_root / "zlib")
    flags = [
        *v47.COMMON_FLAGS,
        "-Os",
        *v47.SNES_DEFINES,
        "-DZLIB",
        *include_args(includes),
        "-x",
        "c++",
    ]
    shared_inputs = (
        layout / "snes9x.h",
        layout / "getset.h",
        layout / "port.h",
        layout / "MEMMAP.H",
        layout / "spc7110.h",
        *compat_inputs,
    )
    builds = (
        ("dma", "DMA.CPP", "snes9x-1.41-1-os-short-ps2-layout"),
        ("loadzip", "LOADZIP.CPP", "snes9x-1.41-1-os-short-ps2-assert-zlib"),
        ("obc1", "obc1.cpp", "snes9x-1.41-1-os-short-ps2-unaligned-byte-write"),
        ("spc7110", "spc7110.cpp", "snes9x-1.41-1-os-short-ps2-int32-no-wall-clock"),
    )
    objects: dict[str, v51.ObjectBuild] = {}
    for key, filename, profile in builds:
        objects[key] = compile_one(
            cxx,
            layout / filename,
            BUILD / "objects" / f"{key}.o",
            profile,
            flags,
            original / filename,
            shared_inputs,
        )
    return objects


def compare_slice(
    target: bytes,
    candidate: Candidate,
    elf: ELFFile,
) -> dict[str, object]:
    symbol = elf.find_symbol(candidate.symbol)
    full = elf.symbol_bytes(symbol, symbol.size)
    body = full[candidate.object_offset:candidate.object_offset + candidate.size]
    target_offset = candidate.address - TARGET_BASE
    expected = target[target_offset:target_offset + candidate.size]
    if len(body) != candidate.size or len(expected) != candidate.size:
        raise SystemExit(f"0x{candidate.address:08x}: truncated comparison range")

    ignored_bits = bytearray(candidate.size)
    relocation_count = 0
    unknown: set[int] = set()
    for relocation in elf.relocation_masks(symbol, 4):
        overlap_start = max(relocation.start, candidate.object_offset)
        overlap_end = min(
            relocation.end, candidate.object_offset + candidate.size
        )
        if overlap_start >= overlap_end:
            continue
        relocation_count += 1
        if not relocation.known:
            unknown.add(relocation.relocation_type)
        for absolute in range(overlap_start, overlap_end):
            output_index = absolute - candidate.object_offset
            mask_index = absolute - relocation.start
            ignored_bits[output_index] |= relocation.mask_bytes[mask_index]

    differences = [
        index
        for index, (left, right) in enumerate(zip(expected, body))
        if ((left ^ right) & (~ignored_bits[index] & 0xFF)) != 0
    ]
    return {
        "raw_equal": expected == body,
        "normalized_equal": not differences,
        "differing_bytes": len(differences),
        "first_differences": tuple(differences[:8]),
        "unknown": tuple(sorted(unknown)),
        "relocations": relocation_count,
    }


def load_recovered_spans() -> dict[str, dict[str, str]]:
    with RECOVERED_SPANS.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    if len(rows) != len(CANDIDATES):
        raise SystemExit("V53 recovered-span cardinality changed")
    result = {row["address"].lower(): row for row in rows}
    if len(result) != len(rows):
        raise SystemExit("duplicate V53 recovered-span address")
    for candidate in CANDIDATES:
        address = f"0x{candidate.address:08x}"
        row = result.get(address)
        if row is None:
            raise SystemExit(f"{address}: missing frozen V53 target span")
        if int(row["bytes"]) != candidate.size:
            raise SystemExit(f"{address}: frozen V53 span size changed")
        if row.get("recovered_exact_match_fact", "").lower() != "yes":
            raise SystemExit(f"{address}: frozen V53 recovery fact changed")
    return result


def make_evidence(
    target: bytes,
    objects: dict[str, v51.ObjectBuild],
) -> list[dict[str, str]]:
    recovered = load_recovered_spans()
    _, manifest_rows = v51.read_csv(TARGETS)
    manifest = {int(row["address"], 0): row for row in manifest_rows}
    starts = sorted(manifest)
    next_address = {left: right for left, right in zip(starts, starts[1:])}
    elf_cache: dict[Path, ELFFile] = {}
    rows: list[dict[str, str]] = []

    for candidate in CANDIDATES:
        address = f"0x{candidate.address:08x}"
        target_row = manifest.get(candidate.address)
        boundary = next_address.get(candidate.address)
        if target_row is None or boundary is None:
            raise SystemExit(f"{address}: missing audited target boundary")
        if target_row["name"] != candidate.expected_name:
            raise SystemExit(f"{address}: manifest identity changed")
        if target_row["area"] != candidate.area:
            raise SystemExit(f"{address}: manifest area changed")
        if target_row["status"] not in {"RECONSTRUCTED", "MATCHING"}:
            raise SystemExit(f"{address}: unexpected manifest status")
        if boundary - candidate.address != candidate.size:
            raise SystemExit(
                f"{address}: exact boundary changed from {candidate.size} to "
                f"{boundary - candidate.address} bytes"
            )

        built = objects[candidate.object_key]
        elf = elf_cache.setdefault(built.path, ELFFile(built.path))
        symbol = elf.find_symbol(candidate.symbol)
        if symbol.size != candidate.symbol_size:
            raise SystemExit(
                f"{address}: {candidate.symbol} size changed from "
                f"{candidate.symbol_size} to {symbol.size}"
            )
        if candidate.object_offset + candidate.size > symbol.size:
            raise SystemExit(f"{address}: object slice exceeds historical symbol")
        comparison = compare_slice(target, candidate, elf)
        if not comparison["normalized_equal"] or comparison["differing_bytes"]:
            first = ",".join(
                f"+0x{offset:x}" for offset in comparison["first_differences"]
            )
            raise SystemExit(
                f"{address}: strict comparison failed "
                f"({comparison['differing_bytes']}; {first or 'size'})"
            )
        if comparison["unknown"]:
            raise SystemExit(
                f"{address}: unknown relocation types {comparison['unknown']}"
            )

        target_offset = candidate.address - TARGET_BASE
        span = target[target_offset:target_offset + candidate.size]
        span_sha = sha256_bytes(span)
        recovered_row = recovered[address]
        if span_sha != recovered_row["target_span_sha256"]:
            raise SystemExit(f"{address}: frozen target-span SHA-256 mismatch")
        rows.append(
            {
                "address": address,
                "name": candidate.expected_name,
                "historical_identity": candidate.historical_identity,
                "area": candidate.area,
                "provenance": PROVENANCE,
                "source": str(built.metadata["source"]),
                "profile": str(built.metadata["profile"]),
                "detail": candidate.detail,
                "object": rel(built.path),
                "object_symbol": candidate.symbol,
                "object_symbol_size": str(symbol.size),
                "object_offset": str(candidate.object_offset),
                "object_size": str(candidate.size),
                "boundary": EXACT,
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(comparison["raw_equal"]),
                "normalized_equal": "True",
                "unknown_relocations": "",
                "relocation_count": str(comparison["relocations"]),
                "object_sha256": v51.sha256_file(built.path),
                "cache_key": str(built.metadata["cache_key"]),
                "target_gate": f"formal-unpacked-elf:{TARGET_SHA256}",
                "target_span_sha256": span_sha,
            }
        )

    if len(rows) != 6 or len({row["address"] for row in rows}) != 6:
        raise SystemExit("V72 evidence cardinality gate failed")
    dma = CANDIDATES[:2]
    if (
        dma[0].address + dma[0].size != dma[1].address
        or dma[0].object_offset + dma[0].size != dma[1].object_offset
        or dma[1].object_offset + dma[1].size != dma[1].symbol_size
    ):
        raise SystemExit("S9xDoDMA partition coverage is no longer contiguous")
    return rows


def promote(evidence_rows: list[dict[str, str]]) -> tuple[int, int]:
    target_fields, target_rows = v51.read_csv(TARGETS)
    symbol_fields, symbol_rows = v51.read_csv(SYMBOLS)
    targets = {row["address"].lower(): row for row in target_rows}
    symbols = {row["address"].lower(): row for row in symbol_rows}
    if set(targets) != set(symbols):
        raise SystemExit("target and symbol manifests have different address sets")

    changed = 0
    marker = "HUNT1041 V72 strict MATCH;"
    for evidence in evidence_rows:
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
        old_status = target["status"]
        if old_status not in {"RECONSTRUCTED", "MATCHING"}:
            raise SystemExit(f"{address}: unexpected status {old_status}")
        note = (
            f"{marker} mode={evidence['detail'].split(';', 1)[0]}; "
            f"historical_identity={evidence['historical_identity']}; "
            f"provenance={evidence['provenance']}; source={evidence['source']}; "
            f"profile={evidence['profile']}; object_symbol={evidence['object_symbol']}; "
            f"object_symbol_size={evidence['object_symbol_size']}; "
            f"object_offset={evidence['object_offset']}; object_size={evidence['object_size']}; "
            f"boundary={EXACT}; target_gate={evidence['target_gate']}; "
            "differing_bytes=0; normalized_equal=True; unknown_relocations=none; "
            f"evidence={rel(EVIDENCE)}"
        )
        for manifest_row in (target, symbol):
            prefix, separator, _old = manifest_row["notes"].partition(marker)
            if separator:
                manifest_row["notes"] = prefix.rstrip("; ") + "; " + note
            else:
                manifest_row["notes"] = manifest_row["notes"].rstrip("; ") + "; " + note
            manifest_row["status"] = "MATCHING"
            manifest_row["confidence"] = "very-high"
        if target["status"] != "MATCHING":
            raise SystemExit(f"{address}: promotion failed")
        changed += int(old_status != "MATCHING")

    v51.write_csv_atomic(TARGETS, target_fields, target_rows)
    v51.write_csv_atomic(SYMBOLS, symbol_fields, symbol_rows)
    v51.run([sys.executable, str(ROOT / "tools" / "audit_source_completeness.py")])
    v51.run([sys.executable, str(ROOT / "tools" / "update_progress.py")])
    _, updated = v51.read_csv(TARGETS)
    formal = sum(row["status"] == "MATCHING" for row in updated)
    return changed, formal


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, default=REFERENCE)
    parser.add_argument(
        "--cxx",
        type=Path,
        default=(
            ROOT
            / "build"
            / "toolchains"
            / "ee-gcc-3.2.2-cxx-stage1"
            / "prefix"
            / "bin"
            / "ee-g++"
        ),
    )
    parser.add_argument(
        "--apply",
        action="store_true",
        help="promote the six strictly validated rows and regenerate status files",
    )
    args = parser.parse_args()

    reference = args.reference.expanduser().resolve()
    cxx = args.cxx.expanduser().resolve()
    if not cxx.is_file():
        raise SystemExit(f"missing EE C++ compiler: {cxx}")
    v51.run(
        [
            sys.executable,
            str(ROOT / "tools" / "probe_ee_toolchain.py"),
            "--compiler",
            str(cxx),
        ]
    )
    if not reference.is_file():
        raise SystemExit(f"missing formal unpacked reference: {reference}; run make reference")
    target = reference.read_bytes()
    actual_sha = sha256_bytes(target)
    if actual_sha != TARGET_SHA256:
        raise SystemExit(f"unpacked target SHA-256 mismatch: {actual_sha}")

    objects = build_objects(cxx)
    rows = make_evidence(target, objects)
    v51.write_csv_atomic(EVIDENCE, EVIDENCE_FIELDS, rows, delimiter="\t")
    print(f"V72 strict V53 matches: {len(rows)}/6")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print("boundaries: exact-next-boundary; differing_bytes=0; unknown_relocations=none")
    print(f"evidence: {rel(EVIDENCE)}")
    if args.apply:
        changed, formal = promote(rows)
        print(f"promoted rows: {changed}; formal MATCHING now {formal}/1041")
    else:
        print("dry promotion; pass --apply to update the authoritative manifests")


if __name__ == "__main__":
    main()
