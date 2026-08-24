#!/usr/bin/env python3
"""Rebuild, verify, and optionally promote the 25 HUNT1041 V48 matches.

The preferred target gate is the hash-pinned unpacked executable.  When that
private reference is unavailable, the runner fails closed to bytes present in
the committed objdump listings.  Two eight-byte zero runs inside inflate_fast
are admitted only through explicit, address-bounded listing-gap assertions.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "research"))

from compare_elf_functions import ELFFile  # noqa: E402
from run_match_miner import (  # noqa: E402
    compile_source,
    compiler_identity,
    dependency_identity,
)
import hunt1000plus_v47_closure as v47  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
BUILD = ROOT / "build" / "matching" / "hunt1041-v48-closure"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v48-validated-25.tsv"
ZERO_EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v48-inferred-zero-ranges.tsv"
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"

INSTRUCTION_RE = re.compile(
    r"^\s*([0-9A-Fa-f]+):\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})\s+"
    r"([0-9A-Fa-f]{2})\s+([0-9A-Fa-f]{2})(?:\s|$)"
)

# These are the only absent listing bytes accepted by the fallback gate.  Each
# gap is bracketed by explicit sequential instructions in the same continuous
# objdump listing, is word aligned, and corresponds to objdump-suppressed zero
# words in the matching historical object.
BOUNDED_ZERO_GAPS = (
    ("analysis/functions/zlib_infcodes_0019533c.asm", 0x00195F1C, 0x00195F24),
    ("analysis/functions/zlib_infcodes_0019533c.asm", 0x00195F5C, 0x00195F64),
)

EVIDENCE_FIELDS = (
    "address", "name", "area", "provenance", "source", "profile",
    "detail", "object", "object_symbol", "object_symbol_size",
    "object_offset", "object_size", "boundary", "result",
    "differing_bytes", "raw_equal", "normalized_equal",
    "unknown_relocations", "object_sha256", "cache_key", "target_gate",
    "target_span_sha256", "target_sources",
)


@dataclass(frozen=True)
class Candidate:
    address: int
    expected_name: str
    object_key: str
    symbol: str
    symbol_size: int
    object_offset: int
    size: int
    provenance: str
    detail: str


@dataclass(frozen=True)
class ObjectBuild:
    path: Path
    metadata: dict[str, object]


@dataclass(frozen=True)
class TargetImage:
    data: bytes
    known: frozenset[int] | None
    sources: dict[int, frozenset[str]]
    gate: str


CANDIDATES = (
    Candidate(0x001005B0, "snes_p11_001005b0", "app-o2", "v48_001005b0", 60, 0, 60, "snesstation-v0.23-recovered", "recovered-source-strict"),
    Candidate(0x00101924, "snes_p13_path_wrapper_00101924", "app-o2", "v48_00101924", 124, 0, 124, "snesstation-v0.23-recovered", "recovered-source-strict"),
    Candidate(0x00105750, "build_sram_path_00105750", "path-os", "build_sram_path_00105750", 172, 0, 172, "snesstation-v0.23-recovered", "recovered-source-strict"),
    Candidate(0x001059CC, "_makepath", "path-os", "_makepath", 284, 0, 284, "snesstation-v0.23-recovered", "recovered-source-strict; historical-identity=_makepath"),
    Candidate(0x00105CB8, "snes_p11_00105cb8", "app-os", "v48_00105cb8", 120, 0, 120, "snesstation-v0.23-recovered", "recovered-source-strict"),
    Candidate(0x00105D78, "snes_dispatch_00105d78", "app-os", "v48_00105d78", 140, 0, 140, "snesstation-v0.23-recovered", "recovered-source-strict"),
    Candidate(0x00106054, "snes_p11_00106054", "app-os", "v48_00106054", 136, 0, 136, "snesstation-v0.23-recovered", "recovered-source-strict"),
    Candidate(0x0014308C, "snes_p11_0014308c", "app-os", "v48_0014308c", 144, 0, 144, "snesstation-v0.23-recovered", "recovered-source-strict"),
    Candidate(0x001591A8, "snes_p13_palette_expand_001591a8", "snes-ppu", "S9xFixColourBrightness", 192, 0, 192, "snes9x-1.41-1-official", "historical-symbol-strict; historical-identity=S9xFixColourBrightness; pixel-format=BGR555"),
    Candidate(0x00172174, "snes_p12_00172174", "app-o2", "v48_00172174", 120, 0, 120, "snesstation-v0.23-recovered", "recovered-source-strict"),
    Candidate(0x00183678, "snes_dispatch_00183678", "snes-srtc", "_Z16S9xHardResetSRTCv", 88, 0, 88, "snes9x-1.41-1-official", "historical-symbol-strict; historical-identity=S9xHardResetSRTC; target-time=int32-at-0x14; wall-clock=zero"),

    # The original compiler emitted useful setup instructions before the
    # conventional stack prologues.  Earlier analysis treated those addresses
    # as separate functions; the two rows below each cover one full historical
    # symbol without overlap or omission.
    Candidate(0x0018EA58, "snes_p16_0018ea58", "pgen-unshrink", "unShrink", 1116, 0, 12, "pgen-403f1710", "historical-symbol-slice-strict"),
    Candidate(0x0018EA64, "unShrink", "pgen-unshrink", "unShrink", 1116, 12, 1104, "pgen-403f1710", "historical-symbol-slice-strict"),
    Candidate(0x0018FAA4, "snes_dispatch_0018faa4", "pgen-unzip", "unzGetCurrentFileInfo", 72, 0, 16, "pgen-403f1710", "historical-symbol-slice-strict"),
    Candidate(0x0018FAB4, "unzGetCurrentFileInfo", "pgen-unzip", "unzGetCurrentFileInfo", 72, 16, 56, "pgen-403f1710", "historical-symbol-slice-strict"),
    Candidate(0x00193A34, "gzread", "pgen-gzio", "gzread", 676, 0, 676, "pgen-403f1710", "historical-symbol-strict; abi=32-bit-memcpy-length"),
    Candidate(0x00195B3C, "snes_p16_00195b3c", "pgen-inffast", "inflate_fast", 1092, 0, 4, "pgen-403f1710", "historical-symbol-slice-strict"),
    Candidate(0x00195B40, "inflate_fast", "pgen-inffast", "inflate_fast", 1092, 4, 1088, "pgen-403f1710", "historical-symbol-slice-strict"),

    Candidate(0x0019D558, "fioGets", "fileio-os", "fioGets_0019d558", 168, 0, 168, "snesstation-v0.23-recovered", "recovered-source-strict; historical-identity=fioGets"),
    Candidate(0x001A086C, "mcStoreDir", "ps2lib-libmc", "mcStoreDir", 128, 0, 128, "ps2sdk-694100b", "historical-symbol-strict; corrected-entry=0x001a086c"),
    Candidate(0x001837CC, "snes_p17_001837cc", "snes-srtc", "_Z17S9xUpdateSrtcTimev", 728, 0, 728, "snes9x-1.41-1-official", "historical-symbol-strict; target-time=int32-at-0x14; wall-clock=zero"),
    Candidate(0x00183AA4, "snes_p16_00183aa4", "snes-srtc", "_Z10S9xSetSRTCht", 312, 0, 312, "snes9x-1.41-1-official", "historical-symbol-strict; target-time=int32-at-0x14; wall-clock=zero"),
    Candidate(0x00183C58, "snes_p12_00183c58", "snes-srtc", "_Z19S9xSRTCPreSaveStatev", 232, 0, 232, "snes9x-1.41-1-official", "historical-symbol-strict; target-time=int32-at-0x14; fixed-copy=8"),
    Candidate(0x00183D40, "snes_p12_00183d40", "snes-srtc", "_Z20S9xSRTCPostLoadStatev", 196, 0, 196, "snes9x-1.41-1-official", "historical-symbol-strict; target-time=int32-at-0x14; fixed-copy=8"),
    Candidate(0x001ABC28, "S9xGetWord", "snes-memmap", "_Z10S9xGetWordj", 1020, 0, 1020, "snes9x-1.41-1-official", "historical-symbol-strict; target-access=packed-unaligned-u32-low16"),
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


def listing_bytes() -> tuple[dict[int, int], dict[int, frozenset[str]]]:
    values: dict[int, int] = {}
    mutable_sources: dict[int, set[str]] = {}
    parsed_by_path: dict[str, tuple[list[str], list[tuple[int, int]]]] = {}

    for path in sorted((ROOT / "analysis" / "functions").glob("*.asm")):
        source = rel(path)
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
        explicit: list[tuple[int, int]] = []
        for line_number, line in enumerate(lines):
            match = INSTRUCTION_RE.match(line)
            if match is None:
                continue
            address = int(match.group(1), 16)
            explicit.append((line_number, address))
            raw = bytes(int(match.group(index), 16) for index in range(2, 6))
            for offset, value in enumerate(raw):
                absolute = address + offset
                previous = values.get(absolute)
                if previous is not None and previous != value:
                    raise SystemExit(
                        f"conflicting committed listing byte at 0x{absolute:08x}"
                    )
                values[absolute] = value
                mutable_sources.setdefault(absolute, set()).add(source)
        parsed_by_path[source] = (lines, explicit)

    for source, start, end in BOUNDED_ZERO_GAPS:
        if start % 4 or end <= start or (end - start) % 4:
            raise SystemExit(f"invalid bounded zero range: 0x{start:08x}-0x{end:08x}")
        lines, explicit = parsed_by_path.get(source, ([], []))
        if not lines:
            raise SystemExit(f"missing zero-gap listing: {source}")
        pair = None
        for left, right in zip(explicit, explicit[1:]):
            if left[1] == start - 4 and right[1] == end:
                pair = (left, right)
                break
        if pair is None:
            raise SystemExit(
                f"zero gap is no longer bracketed in {source}: "
                f"0x{start:08x}-0x{end:08x}"
            )
        (left_line, _), (right_line, _) = pair
        between = [line.strip() for line in lines[left_line + 1:right_line]]
        if any(line not in ("", "...") for line in between):
            raise SystemExit(f"unexpected content inside bounded zero gap in {source}")
        if any(address in values for address in range(start, end)):
            raise SystemExit(f"bounded zero gap now has explicit bytes in {source}")
        for address in range(start, end):
            values[address] = 0
            mutable_sources.setdefault(address, set()).add(
                source + "#bounded-zero-gap"
            )

    sources = {
        address: frozenset(paths) for address, paths in mutable_sources.items()
    }
    return values, sources


def write_zero_evidence() -> None:
    rows = []
    for source, start, end in BOUNDED_ZERO_GAPS:
        path = ROOT / source
        rows.append(
            {
                "start": f"0x{start:08x}",
                "end_exclusive": f"0x{end:08x}",
                "bytes": str(end - start),
                "source": source,
                "source_sha256": sha256_file(path),
                "proof": "bounded-contiguous-objdump-address-gap; inferred-word=0x00000000",
            }
        )
    write_csv_atomic(
        ZERO_EVIDENCE,
        ["start", "end_exclusive", "bytes", "source", "source_sha256", "proof"],
        rows,
        delimiter="\t",
    )


def load_target() -> TargetImage:
    listed, listed_sources = listing_bytes()
    write_zero_evidence()
    if REFERENCE.is_file() and sha256_file(REFERENCE) == TARGET_SHA256:
        return TargetImage(
            REFERENCE.read_bytes(), None, {},
            f"formal-unpacked-elf:{TARGET_SHA256}",
        )
    if not listed:
        raise SystemExit("verified target and committed listing bytes are unavailable")
    end = max(listed) + 1
    image = bytearray(end - TARGET_BASE)
    for address, value in listed.items():
        if address >= TARGET_BASE:
            image[address - TARGET_BASE] = value
    return TargetImage(
        bytes(image), frozenset(listed), listed_sources,
        "committed-objdump-listings+bounded-zero-gaps",
    )


def recovered_object(
    compiler: Path,
    compiler_id: str,
    dependency_id: str,
    source: Path,
    profile: str,
) -> ObjectBuild:
    result = compile_source(
        compiler, compiler_id, dependency_id, BUILD / "recovered",
        profile, source,
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
    extra_inputs: tuple[Path, ...] = (),
) -> ObjectBuild:
    path, metadata = v47.compile_one(
        compiler, source, output, profile, flags, upstream,
        extra_inputs=extra_inputs,
    )
    return ObjectBuild(path, metadata)


def build_objects(compiler: Path, cxx: Path) -> dict[str, ObjectBuild]:
    compiler_id = compiler_identity(compiler)
    dependency_id = dependency_identity()
    app = ROOT / "matching" / "candidates" / "hunt1041_v48.c"
    path_helpers = ROOT / "src" / "ps2" / "path_helpers_recovered.c"
    fileio = ROOT / "src" / "ps2" / "fileio_recovered.c"

    objects = {
        "app-o2": recovered_object(compiler, compiler_id, dependency_id, app, "o2"),
        "app-os": recovered_object(compiler, compiler_id, dependency_id, app, "os"),
        "path-os": recovered_object(compiler, compiler_id, dependency_id, path_helpers, "os"),
        "fileio-os": recovered_object(compiler, compiler_id, dependency_id, fileio, "os"),
    }

    v47.ensure_git_commit(v47.PGEN, v47.PGEN_REPO, v47.PGEN_COMMIT)
    v47.ensure_git_commit(v47.PS2DEV, v47.PS2DEV_REPO, v47.PS2DEV_COMMIT)
    v47.ensure_git_commit(
        v47.PS2SDK_20040415, v47.PS2SDK_REPO, v47.PS2SDK_20040415_COMMIT
    )

    newlib = (
        v47.PS2DEV / "ps2toolchain" / "soft" / "newlib-1.10.0"
        / "newlib" / "libc" / "include"
    )
    modern_ps2_includes = sorted(
        path for path in (v47.PS2DEV / "ps2sdk").rglob("include")
        if path.is_dir()
    )
    pgen_includes = [
        v47.PGEN, v47.PGEN / "ps2", v47.PGEN / "unzip",
        v47.PGEN / "zlib", newlib, *modern_ps2_includes,
    ]
    pgen_flags = [
        *v47.COMMON_FLAGS, "-Os", *v47.PS2_DEFINES,
        *v47.include_args(pgen_includes),
    ]
    for key, source, profile in (
        ("pgen-unshrink", v47.PGEN / "unzip" / "unshrink.c", "pgen-os-unshrink"),
        ("pgen-unzip", v47.PGEN / "unzip" / "unzip.c", "pgen-os-unzip"),
        ("pgen-inffast", v47.PGEN / "zlib" / "inffast.c", "pgen-os-inffast"),
    ):
        objects[key] = historical_object(
            compiler, source, BUILD / "historical" / f"{key}.o",
            profile, pgen_flags, v47.PGEN_COMMIT,
        )

    gzio_compat = BUILD / "compat" / "pgen-gzio"
    v47.atomic_write_text(
        gzio_compat / "string.h",
        "#ifndef HUNT1041_V48_GZIO_STRING_H\n"
        "#define HUNT1041_V48_GZIO_STRING_H\n"
        "#include \"_ansi.h\"\n#include <sys/reent.h>\n"
        "#define __need_size_t\n#include <stddef.h>\n"
        "#ifndef NULL\n#define NULL 0\n#endif\n"
        "_PTR memchr(const _PTR, int, size_t);\n"
        "int memcmp(const _PTR, const _PTR, size_t);\n"
        "_PTR memcpy(_PTR, const _PTR, int);\n"
        "_PTR memmove(_PTR, const _PTR, size_t);\n"
        "_PTR memset(_PTR, int, size_t);\n"
        "char *strcat(char *, const char *);\n"
        "char *strchr(const char *, int);\n"
        "int strcmp(const char *, const char *);\n"
        "char *strcpy(char *, const char *);\n"
        "int strlen(const char *);\n"
        "char *strncpy(char *, const char *, size_t);\n"
        "char *strerror(int);\n#endif\n",
    )
    objects["pgen-gzio"] = historical_object(
        compiler,
        v47.PGEN / "zlib" / "gzio.c",
        BUILD / "historical" / "pgen-gzio.o",
        "pgen-os-int-strlen-int-memcpy",
        [
            *v47.COMMON_FLAGS, "-Os", *v47.PS2_DEFINES,
            *v47.include_args([gzio_compat, *pgen_includes]),
        ],
        v47.PGEN_COMMIT,
        extra_inputs=(gzio_compat / "string.h",),
    )

    old_libc_includes = [
        v47.PS2SDK_20040415 / "ee" / "kernel" / "include",
        v47.PS2SDK_20040415 / "ee" / "libc" / "include",
        v47.PS2DEV / "ps2sdk" / "common" / "include",
        v47.PS2DEV / "ps2sdk" / "ee" / "kernel" / "include",
        v47.PS2DEV / "ps2sdk" / "ee" / "libc" / "include",
    ]
    libmc_source = (
        v47.PS2SDK_20040415 / "ee" / "rpc" / "memorycard"
        / "src" / "libmc.c"
    )
    libmc_includes = [
        v47.PS2SDK_20040415 / "ee" / "rpc" / "memorycard" / "include",
        *old_libc_includes,
    ]
    objects["ps2lib-libmc"] = historical_object(
        compiler, libmc_source, BUILD / "historical" / "ps2lib-libmc.o",
        "ps2lib-20040415-os-tu",
        [*v47.PS2LIB_FLAGS, *v47.include_args(libmc_includes)],
        v47.PS2SDK_20040415_COMMIT,
    )

    try:
        archive = v47.download_archive(v47.SNES_141_1_ARCHIVE, v47.SNES_CACHE)
        snes_root = v47.safe_extract_archive(
            archive,
            v47.SNES_CACHE / "source-1.41-1",
            v47.SNES_141_1_ARCHIVE.source_directory,
        )
    except v47.BuildFailure as exc:
        raise SystemExit(str(exc)) from exc
    original_snes = snes_root / "snes9x"
    target_layout_snes = BUILD / "historical" / "snes9x-target-layout"
    if target_layout_snes.exists():
        shutil.rmtree(target_layout_snes)
    shutil.copytree(original_snes, target_layout_snes)
    srtc_header = target_layout_snes / "srtc.h"
    header_text = srtc_header.read_text(encoding="latin-1")
    marker = "    time_t system_timestamp;\t// Of latest RTC load time"
    replacement = "    int32 system_timestamp;\t// Of latest RTC load time"
    if header_text.count(marker) != 1:
        raise SystemExit("Snes9x 1.41-1 SRTC layout patch context changed")
    v47.atomic_write_text(srtc_header, header_text.replace(marker, replacement))

    srtc_source = target_layout_snes / "srtc.cpp"
    srtc_text = srtc_source.read_text(encoding="latin-1")
    local_time_marker = "\ttime_t\tcur_systime;"
    if srtc_text.count(local_time_marker) != 1:
        raise SystemExit("Snes9x 1.41-1 SRTC local-time patch context changed")
    srtc_text = srtc_text.replace(local_time_marker, "\tint32\tcur_systime;")
    source_lines = []
    wall_clock_replacements = 0
    for source_line in srtc_text.splitlines(keepends=True):
        if (
            "time (NULL)" in source_line
            and not source_line.lstrip().startswith("*")
        ):
            source_line = source_line.replace("time (NULL)", "0")
            wall_clock_replacements += 1
        source_lines.append(source_line)
    if wall_clock_replacements != 3:
        raise SystemExit("Snes9x 1.41-1 SRTC wall-clock patch context changed")
    v47.atomic_write_text(srtc_source, "".join(source_lines))

    getset_header = target_layout_snes / "getset.h"
    getset_text = getset_header.read_text(encoding="latin-1")
    function_marker = "INLINE uint16 S9xGetWord (uint32 Address)"
    access_marker = (
        "#ifdef FAST_LSB_WORD_ACCESS\n"
        "\t\treturn (*(uint16 *) (GetAddress + (Address & 0xffff)));\n"
        "#else"
    )
    if getset_text.count(function_marker) != 1 or getset_text.count(access_marker) != 1:
        raise SystemExit("Snes9x 1.41-1 S9xGetWord access patch context changed")
    getset_text = getset_text.replace(
        function_marker,
        "struct Hunt1041UnalignedUint32\n"
        "{\n"
        "    uint32 value;\n"
        "} __attribute__((packed));\n\n"
        + function_marker,
    )
    getset_text = getset_text.replace(
        access_marker,
        "#ifdef FAST_LSB_WORD_ACCESS\n"
        "\t\tregister uint32 value __asm__ (\"$16\") =\n"
        "\t\t\t((Hunt1041UnalignedUint32 *)\n"
        "\t\t\t (GetAddress + (Address & 0xffff)))->value;\n"
        "\t\tvalue &= 0xffff;\n"
        "\t\t__asm__ (\"\" : \"+r\" (value));\n"
        "\t\treturn value;\n"
        "#else",
    )
    v47.atomic_write_text(getset_header, getset_text)

    port_header = target_layout_snes / "port.h"
    port_text = port_header.read_text(encoding="latin-1")
    pixel_marker = "/* #define PIXEL_FORMAT RGB565 */\n#define GFX_MULTI_FORMAT"
    if port_text.count(pixel_marker) != 1:
        raise SystemExit("Snes9x 1.41-1 BGR555 patch context changed")
    v47.atomic_write_text(
        port_header,
        port_text.replace(
            pixel_marker,
            "#define PIXEL_FORMAT BGR555\n"
            "/* Fixed 15-bit pixel profile used by the PS2 frontend. */",
        ),
    )

    snes_compat = BUILD / "compat" / "snes"
    v47.atomic_write_text(
        snes_compat / "memory.h",
        "#ifndef HUNT1041_V48_MEMORY_H\n#define HUNT1041_V48_MEMORY_H\n"
        "#include <string.h>\n#endif\n",
    )
    snes_flags = [
        *v47.COMMON_FLAGS, "-Os", *v47.SNES_DEFINES,
        *v47.include_args([
            snes_compat, newlib, target_layout_snes,
            target_layout_snes / "unzip", snes_root / "zlib",
        ]),
        "-x", "c++",
    ]
    srtc_path, srtc_metadata = v47.compile_one(
        cxx,
        srtc_source,
        BUILD / "historical" / "snes-srtc.o",
        "snes9x-1.41-1-os-short-ps2-int32-no-wall-clock",
        snes_flags,
        f"lysator:{v47.SNES_141_1_ARCHIVE.sha256}",
        evidence_source=original_snes / "srtc.cpp",
        extra_inputs=(srtc_header, snes_compat / "memory.h"),
    )
    objects["snes-srtc"] = ObjectBuild(srtc_path, srtc_metadata)
    memmap_path, memmap_metadata = v47.compile_one(
        cxx,
        target_layout_snes / "MEMMAP.CPP",
        BUILD / "historical" / "snes-memmap.o",
        "snes9x-1.41-1-os-short-fast-lsb-target-access",
        [*snes_flags, "-DFAST_LSB_WORD_ACCESS"],
        f"lysator:{v47.SNES_141_1_ARCHIVE.sha256}",
        evidence_source=original_snes / "MEMMAP.CPP",
        extra_inputs=(getset_header, snes_compat / "memory.h"),
    )
    objects["snes-memmap"] = ObjectBuild(memmap_path, memmap_metadata)
    ppu_path, ppu_metadata = v47.compile_one(
        cxx,
        target_layout_snes / "ppu.cpp",
        BUILD / "historical" / "snes-ppu.o",
        "snes9x-1.41-1-os-short-bgr555",
        snes_flags,
        f"lysator:{v47.SNES_141_1_ARCHIVE.sha256}",
        evidence_source=original_snes / "ppu.cpp",
        extra_inputs=(port_header, snes_compat / "memory.h"),
    )
    objects["snes-ppu"] = ObjectBuild(ppu_path, ppu_metadata)
    return objects


def compare_slice(
    target: bytes,
    address: int,
    elf: ELFFile,
    symbol_name: str,
    object_offset: int,
    size: int,
) -> dict[str, object]:
    symbol = elf.find_symbol(symbol_name)
    full_candidate = elf.symbol_bytes(symbol, symbol.size)
    candidate = full_candidate[object_offset:object_offset + size]
    target_offset = address - TARGET_BASE
    expected = target[target_offset:target_offset + size]
    if len(candidate) != size or len(expected) != size:
        raise SystemExit(f"0x{address:08x}: truncated comparison range")

    ignored_bits = bytearray(size)
    relocation_count = 0
    unknown: set[int] = set()
    for relocation in elf.relocation_masks(symbol, 4):
        overlap_start = max(relocation.start, object_offset)
        overlap_end = min(relocation.end, object_offset + size)
        if overlap_start >= overlap_end:
            continue
        relocation_count += 1
        if not relocation.known:
            unknown.add(relocation.relocation_type)
        for absolute in range(overlap_start, overlap_end):
            segment_index = absolute - object_offset
            mask_index = absolute - relocation.start
            ignored_bits[segment_index] |= relocation.mask_bytes[mask_index]

    differences = [
        index for index, (left, right) in enumerate(zip(expected, candidate))
        if ((left ^ right) & (~ignored_bits[index] & 0xFF)) != 0
    ]
    return {
        "raw_equal": expected == candidate,
        "normalized_equal": not differences,
        "differing_bytes": len(differences),
        "first_differences": tuple(differences[:8]),
        "unknown": tuple(sorted(unknown)),
        "relocations": relocation_count,
    }


def make_evidence(
    target: TargetImage,
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
        if target_row is None or end is None:
            raise SystemExit(f"0x{spec.address:08x}: missing audited target boundary")
        if target_row["name"] != spec.expected_name:
            raise SystemExit(f"0x{spec.address:08x}: manifest identity changed")
        if target_row["status"] not in {"RECONSTRUCTED", "MATCHING"}:
            raise SystemExit(f"0x{spec.address:08x}: unexpected manifest status")
        if end - spec.address != spec.size:
            raise SystemExit(
                f"0x{spec.address:08x}: expected exact span {spec.size}, got "
                f"{end - spec.address}"
            )
        if target.known is not None:
            missing = [
                address for address in range(spec.address, end)
                if address not in target.known
            ]
            if missing:
                raise SystemExit(
                    f"0x{spec.address:08x}: committed listing coverage missing at "
                    f"0x{missing[0]:08x}"
                )

        built = objects[spec.object_key]
        elf = ELFFile(built.path)
        symbol = elf.find_symbol(spec.symbol)
        if symbol.size != spec.symbol_size:
            raise SystemExit(
                f"0x{spec.address:08x}: {spec.symbol} size changed from "
                f"{spec.symbol_size} to {symbol.size}"
            )
        if spec.object_offset + spec.size > symbol.size:
            raise SystemExit(f"0x{spec.address:08x}: object slice exceeds symbol")
        result = compare_slice(
            target.data, spec.address, elf, spec.symbol,
            spec.object_offset, spec.size,
        )
        if not result["normalized_equal"] or result["differing_bytes"]:
            first = ",".join(
                f"+0x{value:x}" for value in result["first_differences"]
            )
            raise SystemExit(
                f"0x{spec.address:08x}: strict comparison failed ({first})"
            )
        if result["unknown"]:
            raise SystemExit(f"0x{spec.address:08x}: unknown relocation type")

        target_offset = spec.address - TARGET_BASE
        target_span = target.data[target_offset:target_offset + spec.size]
        if target.known is None:
            target_sources = "build/SNES_EMU.unpacked.bin"
        else:
            paths: set[str] = set()
            for address in range(spec.address, end):
                paths.update(target.sources.get(address, ()))
            target_sources = ";".join(sorted(paths))
        metadata = built.metadata
        detail = (
            f"{spec.detail}; symbol-range=0x{spec.object_offset:x}"
            f"..0x{spec.object_offset + spec.size:x}; "
            f"full-symbol-size={spec.symbol_size}; "
            f"relocations={result['relocations']}"
        )
        rows.append(
            {
                "address": f"0x{spec.address:08x}",
                "name": target_row["name"],
                "area": target_row["area"],
                "provenance": spec.provenance,
                "source": str(metadata["source"]),
                "profile": str(metadata["profile"]),
                "detail": detail,
                "object": rel(built.path),
                "object_symbol": spec.symbol,
                "object_symbol_size": str(symbol.size),
                "object_offset": str(spec.object_offset),
                "object_size": str(spec.size),
                "boundary": "exact-next-boundary",
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(result["raw_equal"]),
                "normalized_equal": "True",
                "unknown_relocations": "",
                "object_sha256": sha256_file(built.path),
                "cache_key": str(metadata["cache_key"]),
                "target_gate": target.gate,
                "target_span_sha256": hashlib.sha256(target_span).hexdigest(),
                "target_sources": target_sources,
            }
        )

    expected = {f"0x{candidate.address:08x}" for candidate in CANDIDATES}
    if len(CANDIDATES) != 25 or len(rows) != 25:
        raise SystemExit("V48 evidence cardinality gate failed")
    if {row["address"] for row in rows} != expected:
        raise SystemExit("V48 evidence address gate failed")
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
            "HUNT1041 V48 closure strict MATCH; "
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
            # Refresh this round's generated suffix when the evidence bundle
            # or a private historical profile improves during an idempotent run.
            marker = "HUNT1041 V48 closure strict MATCH;"
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
        "--apply", action="store_true",
        help="promote the 25 validated rows after writing evidence",
    )
    parser.add_argument(
        "--cxx", type=Path,
        default=ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-cxx-stage1"
        / "prefix" / "bin" / "ee-g++",
    )
    args = parser.parse_args()
    compiler = args.cc.expanduser().resolve()
    cxx = args.cxx.expanduser().resolve()
    for tool, label in ((compiler, "EE C compiler"), (cxx, "EE C++ compiler")):
        if not tool.is_file():
            raise SystemExit(f"missing {label}: {tool}")
        run([
            sys.executable, str(ROOT / "tools" / "probe_ee_toolchain.py"),
            "--compiler", str(tool),
        ])

    target = load_target()
    objects = build_objects(compiler, cxx)
    rows = make_evidence(target, objects)
    write_evidence(rows)
    print(f"V48 strict matches: {len(rows)} (exact boundaries: {len(rows)})")
    print(f"target gate: {target.gate}")
    print(f"evidence: {rel(EVIDENCE)}")
    if args.apply:
        print(f"promoted strict matches: {promote(rows)}")
    else:
        print("dry promotion; pass --apply to update the manifests")


if __name__ == "__main__":
    main()
