#!/usr/bin/env python3
"""Rebuild, verify, and optionally promote the 17 HUNT1041 V52 matches."""
from __future__ import annotations

import argparse
import hashlib
import os
import shutil
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "research"))

from compare_elf_functions import (  # noqa: E402
    Comparison,
    ELFFile,
    compare_function,
)
import hunt1000plus_v47_closure as v47  # noqa: E402
import hunt1041_v51_closure as v51  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
BUILD = ROOT / "build" / "matching" / "hunt1041-v52-closure"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v52-validated-17.tsv"
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"
EXACT = "exact-next-boundary"

EVIDENCE_FIELDS = (
    "address", "name", "area", "provenance", "source", "profile",
    "detail", "object", "object_symbol", "object_size", "boundary",
    "result", "differing_bytes", "raw_equal", "normalized_equal",
    "unknown_relocations", "object_sha256", "cache_key", "target_gate",
    "target_span_sha256",
)


@dataclass(frozen=True)
class Candidate:
    address: int
    expected_name: str
    area: str
    object_key: str
    symbols: tuple[str, ...]
    size: int
    provenance: str
    detail: str


CANDIDATES = (
    Candidate(
        0x0010A934, "snes_p16_0010a934", "frontend-core", "apu-short",
        ("S9xResetAPU",), 1044, "snes9x-1.41-1-ps2-abi32-zlib",
        "historical-symbol-strict; corrected-identity=S9xResetAPU",
    ),
    Candidate(
        0x001434AC, "S9xEndScreenRefresh", "renderer", "gfx-short",
        ("S9xEndScreenRefresh", "S9xSetInfoString"), 480,
        "snes9x-1.41-1-snesstation-renderer",
        "historical-contiguous-symbols-strict; delta=display-frame-rate-overlay-removed",
    ),
    Candidate(
        0x0014EC54, "S9xUpdateScreen", "renderer", "gfx-short",
        ("S9xUpdateScreen",), 6712,
        "snes9x-1.41-1-ps2-abi32-zlib",
        "historical-symbol-strict; identity=S9xUpdateScreen; byte-count-abi=uint32",
    ),
    Candidate(
        0x001513BC, "CMemory_LoadROM", "memory", "memmap-short",
        ("_ZN7CMemory7LoadROMEPKc",), 3324,
        "snes9x-1.41-1-snesstation-memory",
        "historical-symbol-strict; identity=CMemory::LoadROM; stream-profile=zlib",
    ),
    Candidate(
        0x001522D8, "CMemory_InitROM", "memory-ppu", "memmap-short",
        ("_ZN7CMemory7InitROMEh",), 4220,
        "snes9x-1.41-1-ps2-abi32-zlib",
        "historical-symbol-strict; identity=CMemory::InitROM; byte-count-abi=uint32",
    ),
    Candidate(
        0x00153354, "CMemory_LoadSRAM", "memory", "memmap-short",
        ("_ZN7CMemory8LoadSRAMEPKc",), 356,
        "snes9x-1.41-1-snesstation-fio",
        "historical-source-variant-strict; identity=CMemory::LoadSRAM; io=fioOpen/fioRead/fioClose",
    ),
    Candidate(
        0x001534B8, "CMemory_SaveSRAM", "memory", "memmap-short",
        ("_ZN7CMemory8SaveSRAMEPKc",), 264,
        "snes9x-1.41-1-snesstation-fio",
        "historical-source-variant-strict; identity=CMemory::SaveSRAM; io=fioOpen/fioWrite/fioClose",
    ),
    Candidate(
        0x00156C60, "CMemory_ApplyROMFixes", "memory-ppu", "memmap-short",
        ("_ZN7CMemory13ApplyROMFixesEv",), 6256,
        "snes9x-1.41-1-snesstation-memory",
        "historical-source-variant-strict; identity=CMemory::ApplyROMFixes; delta=fixed-22-byte-copy",
    ),
    Candidate(
        0x0015A5F0, "snes_p16_0015a5f0", "memory-ppu", "ppu-short",
        ("S9xGetPPU",), 1916, "snes9x-1.41-1-snesstation-ppu",
        "historical-source-variant-strict; corrected-identity=S9xGetPPU; delta=diagnostic-fprintf-removed",
    ),
    Candidate(
        0x0015AD6C, "snes_p17_0015ad6c", "memory-ppu", "ppu-short",
        ("S9xSetCPU",), 3844, "snes9x-1.41-1-snesstation-ppu",
        "historical-source-variant-strict; corrected-identity=S9xSetCPU; delta=diagnostic-fprintf-removed",
    ),
    Candidate(
        0x0017028C, "snes_p16_0017028c", "cpu-audio-runtime", "snaporig-short",
        ("_Z9ReadBlockPKcPviS1_",), 268,
        "snes9x-1.41-1-snesstation-snapshot",
        "historical-source-variant-strict; corrected-identity=ReadBlock; numeric-parser=strtol-base10",
    ),
    Candidate(
        0x00170398, "snapshot_Freeze", "cpu-audio-runtime", "snaporig-short",
        ("_Z16ReadOrigSnapshotPv",), 3528,
        "snes9x-1.41-1-snesstation-snapshot",
        "historical-source-variant-strict; corrected-identity=ReadOrigSnapshot; numeric-parser=strtol-base10",
    ),
    Candidate(
        0x00171348, "snes_p16_00171348", "cpu-audio-runtime", "snapshot-short",
        ("_Z6FreezePv",), 952, "snes9x-1.41-1-ps2-abi32-zlib",
        "historical-symbol-strict; corrected-identity=Freeze",
    ),
    Candidate(
        0x00171700, "snes_p16_00171700", "cpu-audio-runtime", "snapshot-short",
        ("_Z8UnfreezePv",), 1768,
        "snes9x-1.41-1-snesstation-snapshot",
        "historical-source-variant-strict; corrected-identity=Unfreeze; numeric-parser=strtol-base10",
    ),
    Candidate(
        0x001725AC, "snes_p16_001725ac", "cpu-audio-runtime", "snapshot-short",
        ("_Z13UnfreezeBlockPvPcPhi",), 320,
        "snes9x-1.41-1-snesstation-snapshot",
        "historical-source-variant-strict; corrected-identity=UnfreezeBlock; numeric-parser=strtol-base10",
    ),
    Candidate(
        0x00174728, "snes_p16_00174728", "cpu-audio-runtime", "sound-normal",
        ("_Z20S9xSetSoundFrequencyii",), 264,
        "snes9x-1.41-1-snesstation-audio",
        "historical-source-variant-strict; corrected-identity=S9xSetSoundFrequency; conversion=double-of-float",
    ),
    Candidate(
        0x00174FE4, "snes_p17_00174fe4", "cpu-audio-runtime", "sound-short",
        ("_Z11DecodeBlockP7Channel",), 796,
        "snes9x-1.41-1-ps2-abi32-zlib",
        "historical-symbol-strict; corrected-identity=DecodeBlock",
    ),
)


def replace_once(path: Path, marker: str, replacement: str, label: str) -> None:
    text = path.read_text(encoding="latin-1")
    if text.count(marker) != 1:
        raise SystemExit(f"Snes9x 1.41-1 {label} patch context changed")
    temporary = path.with_name(path.name + ".tmp")
    temporary.write_text(
        text.replace(marker, replacement), encoding="latin-1", newline=""
    )
    os.replace(temporary, path)


def prepare_snes_layout() -> tuple[Path, Path, Path]:
    try:
        archive = v47.download_archive(v47.SNES_141_1_ARCHIVE, v47.SNES_CACHE)
        source_root = v47.safe_extract_archive(
            archive,
            v47.SNES_CACHE / "source-1.41-1",
            v47.SNES_141_1_ARCHIVE.source_directory,
        )
    except v47.BuildFailure as exc:
        raise SystemExit(str(exc)) from exc

    original = source_root / "snes9x"
    layout = BUILD / "historical" / "snes9x-target-layout"
    if layout.exists():
        shutil.rmtree(layout)
    shutil.copytree(original, layout)

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
        "S9xGetWord access",
    )

    replace_once(
        layout / "port.h",
        "/* #define PIXEL_FORMAT RGB565 */\n#define GFX_MULTI_FORMAT",
        "#define PIXEL_FORMAT BGR555\n"
        "/* Fixed 15-bit pixel profile used by the PS2 frontend. */",
        "BGR555",
    )
    replace_once(
        layout / "snes9x.h",
        "  /*   uint32 StrikeGunnerOffsetHack; */\n"
        "    bool8  ChuckRock;",
        "  /*   uint32 StrikeGunnerOffsetHack; */\n"
        "    bool8  PS2PortLayoutByte;\n"
        "    bool8  ChuckRock;",
        "Settings one-byte port layout",
    )
    return source_root, original, layout


def patch_sources(layout: Path) -> None:
    replace_once(
        layout / "GFX.CPP",
        "\t\tif (Settings.DisplayFrameRate)\n\t    S9xDisplayFrameRate ();\n",
        "",
        "renderer frame-rate overlay",
    )

    memmap = layout / "MEMMAP.CPP"
    replace_once(
        memmap,
        "#include \"fxemu.h\"",
        "#include \"fxemu.h\"\n"
        "extern \"C\" int fioOpen_like(const char *, int);\n"
        "extern \"C\" int fioClose_like(int);\n"
        "extern \"C\" int fioRead_like(int, void *, int);\n"
        "extern \"C\" int fioWrite_like(int, const void *, int);",
        "PS2 fio declarations",
    )
    replace_once(
        memmap,
        "\t\tFILE *file;\n"
        "\t\tif ((file = fopen (filename, \"rb\")))\n"
        "\t\t{\n"
        "\t\t\tint len = fread ((char*) ::SRAM, 1, 0x20000, file);\n"
        "\t\t\tfclose (file);",
        "\t\tint file;\n"
        "\t\tif ((file = fioOpen_like (filename, 1)))\n"
        "\t\t{\n"
        "\t\t\tint len = fioRead_like (file, (char*) ::SRAM, 0x20000);\n"
        "\t\t\tfioClose_like (file);",
        "LoadSRAM fio calls",
    )
    replace_once(
        memmap,
        "\t\tFILE *file;\n"
        "\t\tif ((file = fopen (filename, \"wb\")))\n"
        "\t\t{\n"
        "\t\t\tfwrite ((char *) ::SRAM, size, 1, file);\n"
        "\t\t\tfclose (file);",
        "\t\tint file;\n"
        "\t\tif ((file = fioOpen_like (filename, 0x203)))\n"
        "\t\t{\n"
        "\t\t\tfioWrite_like (file, (char *) ::SRAM, size);\n"
        "\t\t\tfioClose_like (file);",
        "SaveSRAM fio calls",
    )
    replace_once(
        memmap,
        "strncpy(ROMName, \"THIS SCRIPT WAS STOLEN\", 22);",
        "__builtin_memcpy(ROMName, \"THIS SCRIPT WAS STOLEN\", 22);",
        "ApplyROMFixes fixed-size copy",
    )

    for filename in ("SNAPSHOT.CPP", "snaporig.cpp"):
        source = layout / filename
        replace_once(
            source,
            "(len = atoi (&buffer [4])) == 0",
            "(len = strtol (&buffer [4], NULL, 10)) == 0",
            f"{filename} block-length parser",
        )
        replace_once(
            source,
            "version = atoi (&buffer [strlen (SNAPSHOT_MAGIC) + 1])",
            "version = strtol (&buffer [strlen (SNAPSHOT_MAGIC) + 1], NULL, 10)",
            f"{filename} version parser",
        )

    ppu = layout / "ppu.cpp"
    for index, line in enumerate(
        (
            '\t\t\t\tfprintf(stderr, "Read from $21c2!\\n");\n',
            '\t\t\t\tfprintf(stderr, "Read from $21c3!\\n");\n',
            '\t\t\tfprintf(stderr, "Write %02x to %04x!\\n", byte, Address);\n',
        ),
        start=1,
    ):
        replace_once(ppu, line, "", f"PPU diagnostic print {index}")

    replace_once(
        layout / "SOUNDUX.CPP",
        "((double)  SoundData.channels[channel].frequency * 0.980)",
        "((double) (float) SoundData.channels[channel].frequency * 0.980)",
        "sound-frequency float-to-double conversion",
    )


def write_compat_headers(compat: Path) -> tuple[Path, Path, Path]:
    memory = compat / "memory.h"
    string = compat / "string.h"
    stdio = compat / "stdio.h"
    v47.atomic_write_text(
        memory,
        "#ifndef HUNT1041_V52_MEMORY_H\n#define HUNT1041_V52_MEMORY_H\n"
        "#include <string.h>\n#endif\n",
    )
    v47.atomic_write_text(
        string,
        "#ifndef HUNT1041_V52_STRING_H\n#define HUNT1041_V52_STRING_H\n"
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
        "#ifndef HUNT1041_V52_STDIO_H\n#define HUNT1041_V52_STDIO_H\n"
        "#define fread snes_hidden_fread\n#define fwrite snes_hidden_fwrite\n"
        "#include_next <stdio.h>\n#undef fread\n#undef fwrite\n"
        "extern \"C\" {\n"
        "unsigned int fread(void *, unsigned int, unsigned int, FILE *);\n"
        "unsigned int fwrite(const void *, unsigned int, unsigned int, FILE *);\n"
        "}\n#endif\n",
    )
    return memory, string, stdio


def build_objects(cxx: Path) -> dict[str, v51.ObjectBuild]:
    v47.ensure_git_commit(v47.PS2DEV, v47.PS2DEV_REPO, v47.PS2DEV_COMMIT)
    source_root, original, layout = prepare_snes_layout()
    patch_sources(layout)
    newlib = (
        v47.PS2DEV / "ps2toolchain" / "soft" / "newlib-1.10.0"
        / "newlib" / "libc" / "include"
    )
    compat = BUILD / "compat"
    memory, string, stdio = write_compat_headers(compat)
    includes = v47.include_args(
        [compat, newlib, layout, layout / "unzip", source_root / "zlib"]
    )
    short_flags = [
        *v47.COMMON_FLAGS, "-Os", *v47.SNES_DEFINES, "-DZLIB",
        *includes, "-x", "c++",
    ]
    normal_flags = [
        *(flag for flag in v47.COMMON_FLAGS if flag != "-fshort-double"),
        "-Os", *v47.SNES_DEFINES, "-DZLIB", *includes, "-x", "c++",
    ]
    shared_inputs = (
        layout / "snes9x.h", layout / "getset.h", layout / "port.h",
        memory, string, stdio,
    )
    objects: dict[str, v51.ObjectBuild] = {}
    builds = (
        ("apu-short", "APU.CPP", short_flags, "os-short-abi32-zlib"),
        ("gfx-short", "GFX.CPP", short_flags, "os-short-abi32-zlib-nofps"),
        ("memmap-short", "MEMMAP.CPP", [*short_flags, "-DFAST_LSB_WORD_ACCESS"],
         "os-short-fast-lsb-abi32-zlib-fio"),
        ("ppu-short", "ppu.cpp", short_flags, "os-short-abi32-zlib-noprints"),
        ("snapshot-short", "SNAPSHOT.CPP", short_flags, "os-short-abi32-zlib-strtol"),
        ("snaporig-short", "snaporig.cpp", short_flags, "os-short-abi32-zlib-strtol"),
        ("sound-short", "SOUNDUX.CPP", short_flags, "os-short-abi32-zlib"),
        ("sound-normal", "SOUNDUX.CPP", normal_flags,
         "os-normal-double-abi32-zlib-float-cast"),
    )
    for key, filename, flags, profile in builds:
        objects[key] = v51.compile_one(
            cxx,
            layout / filename,
            BUILD / "objects" / f"{key}.o",
            f"snes9x-1.41-1-{profile}",
            list(flags),
            v47.SNES_141_1_ARCHIVE.sha256,
            evidence_source=original / filename,
            extra_inputs=shared_inputs,
        )
    return objects


def compare_symbol_sequence(
    target: bytes,
    target_offset: int,
    expected_size: int,
    elf: ELFFile,
    symbol_names: tuple[str, ...],
) -> Comparison:
    if len(symbol_names) == 1:
        return compare_function(
            target, target_offset, expected_size, elf, symbol_names[0]
        )
    symbols = tuple(elf.find_symbol(name) for name in symbol_names)
    for previous, current in zip(symbols, symbols[1:]):
        if previous.section_index != current.section_index:
            raise ValueError("composite symbols are in different sections")
        if previous.value + previous.size != current.value:
            raise ValueError("composite symbols are not contiguous")

    candidate = bytearray()
    masks = []
    ranges = []
    base = 0
    for symbol in symbols:
        candidate.extend(elf.symbol_bytes(symbol, symbol.size))
        for relocation in elf.relocation_masks(symbol, 4):
            masks.append((base + relocation.start, relocation.mask_bytes,
                          relocation.relocation_type, relocation.known))
            ranges.append((base + relocation.start, base + relocation.end))
        base += symbol.size
    expected = target[target_offset:target_offset + expected_size]
    overlap = min(len(expected), len(candidate))
    ignored_bits = bytearray(overlap)
    for start, mask_bytes, _kind, _known in masks:
        for index, mask in enumerate(mask_bytes):
            absolute = start + index
            if absolute < overlap:
                ignored_bits[absolute] |= mask
    differences = [
        index for index in range(overlap)
        if ((expected[index] ^ candidate[index]) &
            (~ignored_bits[index] & 0xFF)) != 0
    ]
    unknown = tuple(sorted({kind for _, _, kind, known in masks if not known}))
    return Comparison(
        symbol="+".join(symbol_names),
        expected_size=len(expected),
        candidate_size=len(candidate),
        relocation_ranges=tuple(sorted(set(ranges))),
        raw_equal=expected == candidate,
        normalized_equal=not differences and len(expected) == len(candidate),
        differing_bytes=len(differences) + abs(len(expected) - len(candidate)),
        first_differences=tuple(differences[:8]),
        unknown_relocation_types=unknown,
    )


def make_evidence(
    target: bytes, objects: dict[str, v51.ObjectBuild]
) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    elf_cache: dict[Path, ELFFile] = {}
    for candidate in CANDIDATES:
        built = objects[candidate.object_key]
        elf = elf_cache.setdefault(built.path, ELFFile(built.path))
        comparison = compare_symbol_sequence(
            target,
            candidate.address - TARGET_BASE,
            candidate.size,
            elf,
            candidate.symbols,
        )
        if not comparison.matching:
            offsets = ",".join(f"+0x{x:x}" for x in comparison.first_differences)
            raise SystemExit(
                f"0x{candidate.address:08x} {'+'.join(candidate.symbols)}: DIFF "
                f"({comparison.differing_bytes}; {offsets or 'size'})"
            )
        if comparison.unknown_relocation_types:
            raise SystemExit(
                f"0x{candidate.address:08x}: unknown relocations "
                f"{comparison.unknown_relocation_types}"
            )
        span = target[
            candidate.address - TARGET_BASE:
            candidate.address - TARGET_BASE + candidate.size
        ]
        rows.append(
            {
                "address": f"0x{candidate.address:08x}",
                "name": candidate.expected_name,
                "area": candidate.area,
                "provenance": candidate.provenance,
                "source": str(built.metadata["source"]),
                "profile": str(built.metadata["profile"]),
                "detail": candidate.detail,
                "object": v47.rel(built.path),
                "object_symbol": "+".join(candidate.symbols),
                "object_size": str(comparison.candidate_size),
                "boundary": EXACT,
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(comparison.raw_equal),
                "normalized_equal": str(comparison.normalized_equal),
                "unknown_relocations": "",
                "object_sha256": v51.sha256_file(built.path),
                "cache_key": str(built.metadata["cache_key"]),
                "target_gate": f"formal-unpacked-elf:{TARGET_SHA256}",
                "target_span_sha256": hashlib.sha256(span).hexdigest(),
            }
        )
    if len(rows) != 17 or len({row["address"] for row in rows}) != 17:
        raise SystemExit("V52 evidence cardinality gate failed")
    return rows


def promote(rows: list[dict[str, str]]) -> int:
    target_fields, target_rows = v51.read_csv(TARGETS)
    symbol_fields, symbol_rows = v51.read_csv(SYMBOLS)
    targets = {row["address"].lower(): row for row in target_rows}
    symbols = {row["address"].lower(): row for row in symbol_rows}
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
        note = (
            "HUNT1041 V52 closure strict MATCH; "
            f"mode={evidence['detail'].split(';', 1)[0]}; "
            f"provenance={evidence['provenance']}; "
            f"source={evidence['source']}; profile={evidence['profile']}; "
            f"object_symbol={evidence['object_symbol']}; boundary={EXACT}; "
            f"target_gate={evidence['target_gate']}; differing_bytes=0; "
            "normalized_equal=True; unknown_relocations=none; "
            f"evidence={v47.rel(EVIDENCE)}"
        )
        if target["status"] == "MATCHING":
            marker = "HUNT1041 V52 closure strict MATCH;"
            for manifest_row in (target, symbol):
                prefix, separator, _suffix = manifest_row["notes"].partition(marker)
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

    v51.write_csv_atomic(TARGETS, target_fields, target_rows)
    v51.write_csv_atomic(SYMBOLS, symbol_fields, symbol_rows)
    v51.run([sys.executable, str(ROOT / "tools" / "audit_source_completeness.py")])
    v51.run([sys.executable, str(ROOT / "tools" / "update_progress.py")])
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
        help="promote the 17 validated rows after writing evidence",
    )
    args = parser.parse_args()
    cc = args.cc.expanduser().resolve()
    cxx = args.cxx.expanduser().resolve()
    for path, label in ((cc, "EE C compiler"), (cxx, "EE C++ compiler")):
        if not path.is_file():
            raise SystemExit(f"missing {label}: {path}")
        v51.run([
            sys.executable, str(ROOT / "tools" / "probe_ee_toolchain.py"),
            "--compiler", str(path),
        ])
    if not REFERENCE.is_file():
        raise SystemExit("missing formal unpacked reference; run make reference")
    if v51.sha256_file(REFERENCE) != TARGET_SHA256:
        raise SystemExit("unpacked target SHA-256 mismatch")

    objects = build_objects(cxx)
    rows = make_evidence(REFERENCE.read_bytes(), objects)
    v51.write_csv_atomic(EVIDENCE, EVIDENCE_FIELDS, rows, delimiter="\t")
    print(f"V52 strict matches: {len(rows)} (exact={len(rows)})")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print(f"evidence: {v47.rel(EVIDENCE)}")
    if args.apply:
        print(f"promoted strict matches: {promote(rows)}")
    else:
        print("dry promotion; pass --apply to update the manifests")


if __name__ == "__main__":
    main()
