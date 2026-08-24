#!/usr/bin/env python3
"""Rebuild, verify, and optionally promote the 16 HUNT1041 V51 matches."""
from __future__ import annotations

import argparse
import csv
import hashlib
import os
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "research"))

from compare_elf_functions import ELFFile, compare_function  # noqa: E402
from run_match_miner import COMMON_FLAGS as MINER_FLAGS, PROFILES  # noqa: E402
import hunt1000plus_v47_closure as v47  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
BUILD = ROOT / "build" / "matching" / "hunt1041-v51-closure"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v51-validated-16.tsv"
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
class ObjectBuild:
    path: Path
    metadata: dict[str, str]


@dataclass(frozen=True)
class Candidate:
    address: int
    expected_name: str
    area: str
    object_key: str
    symbol: str
    size: int
    provenance: str
    detail: str


CANDIDATES = (
    Candidate(
        0x00101890, "snes_dispatch_00101890", "dispatch-leaf",
        "local:small-o2", "snes_dispatch_00101890", 80,
        "snesstation-v0.23-recovered",
        "recovered-source-strict; corrected-identity=S9xOpenSnapshotFile",
    ),
    Candidate(
        0x00103CD4, "snes_p16_00103cd4", "frontend-core",
        "local:p21-os", "snes_p21_00103cd4", 188,
        "snesstation-v0.23-recovered",
        "recovered-source-strict; identity=toc-display-name-normalizer",
    ),
    Candidate(
        0x00106C08, "snes_p16_00106c08", "frontend-core",
        "local:p21-o2", "snes_p21_00106c08", 152,
        "snesstation-v0.23-recovered",
        "recovered-source-strict; identity=save-state-slot-parser",
    ),
    Candidate(
        0x00107B7C, "amigaModPause_00107b7c", "audio-rpc",
        "candidate:amiga-os", "amigaModInit", 232,
        "pgen-403f1710-snesstation-delta",
        "historical-source-variant-strict; corrected-identity=amigaModInit; delta=u128-buffer-copy",
    ),
    Candidate(
        0x00107C64, "amigaModLoad_00107c64", "audio-rpc",
        "candidate:amiga-os", "amigaModLoad", 224,
        "pgen-403f1710-snesstation-delta",
        "historical-source-variant-strict; identity=amigaModLoad; delta=caller-owned-iop-heap-init",
    ),
    Candidate(
        0x0012BF5C, "snes_p16_0012bf5c", "snes-core-dsp",
        "candidate:dsp-o2", "snes_p28_0012bf5c", 208,
        "snes9x-1.41-1-ps2-float-variant",
        "historical-source-variant-strict; corrected-identity=Atan-float",
    ),
    Candidate(
        0x0012C02C, "snes_p16_0012c02c", "snes-core-dsp",
        "candidate:dsp-os", "snes_p26_0012c02c", 272,
        "snes9x-1.41-1-ps2-float-variant",
        "historical-source-variant-strict; corrected-identity=InitDSP",
    ),
    Candidate(
        0x00150B1C, "ForceInterleave1OverrideSnes9x", "memory-ppu",
        "snes:memmap", "_Z30ForceInterleave1OverrideSnes9xi", 372,
        "snes9x-1.41-1-ps2-settings-layout",
        "historical-symbol-strict; target-layout-slot=pre-game-fixes-bool8",
    ),
    Candidate(
        0x001520B8, "snes_p16_001520b8", "memory-ppu",
        "snes:memmap", "S9xDeinterleaveMode2", 544,
        "snes9x-1.41-1-ps2-settings-layout",
        "historical-symbol-strict; corrected-identity=S9xDeinterleaveMode2",
    ),
    Candidate(
        0x001541E0, "CMemory_HiROMMap", "memory-ppu",
        "snes:memmap", "_ZN7CMemory8HiROMMapEv", 940,
        "snes9x-1.41-1-ps2-settings-layout",
        "historical-symbol-strict; identity=CMemory::HiROMMap",
    ),
    Candidate(
        0x0015458C, "CMemory_TalesROMMap", "memory-ppu",
        "snes:memmap", "_ZN7CMemory11TalesROMMapEh", 1176,
        "snes9x-1.41-1-ps2-settings-layout",
        "historical-symbol-strict; identity=CMemory::TalesROMMap",
    ),
    Candidate(
        0x00156A04, "snes_p17_00156a04", "memory-ppu",
        "snes:memmap", "_ZN7CMemory12KartContentsEv", 520,
        "snes9x-1.41-1-ps2-settings-layout",
        "historical-symbol-strict; corrected-identity=CMemory::KartContents",
    ),
    Candidate(
        0x0015D0BC, "snes_p16_0015d0bc", "memory-ppu",
        "snes:ppu", "S9xUpdateJoypads", 632,
        "snes9x-1.41-1-ps2-settings-layout",
        "historical-symbol-strict; corrected-identity=S9xUpdateJoypads",
    ),
    Candidate(
        0x0015D334, "snes_p16_0015d334", "memory-ppu",
        "local:p22-os", "snes_p22_0015d334", 184,
        "snes9x-1.41-1-ps2-settings-layout",
        "recovered-source-strict; corrected-identity=S9xSuperFXExec",
    ),
    Candidate(
        0x0016FB04, "snes_p12_0016fb04", "persistence",
        "local:p12-os", "snes_p12_0016fb04", 176,
        "snes9x-1.41-1-snesstation-fio",
        "historical-source-variant-strict; corrected-identity=S9xSDD1SaveLoggedData",
    ),
    Candidate(
        0x0016FBB4, "snes_p12_0016fbb4", "persistence",
        "local:p12-o2", "snes_p12_0016fbb4", 148,
        "snes9x-1.41-1-snesstation-fio",
        "historical-source-variant-strict; corrected-identity=S9xSDD1LoadLoggedData",
    ),
)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def rel(path: Path) -> str:
    try:
        return path.resolve().relative_to(ROOT).as_posix()
    except ValueError:
        return str(path.resolve())


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
        return list(reader.fieldnames or ()), list(reader)


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


def compile_one(
    compiler: Path,
    source: Path,
    output: Path,
    profile: str,
    flags: list[str],
    provenance: str,
    *,
    evidence_source: Path | None = None,
    extra_inputs: tuple[Path, ...] = (),
) -> ObjectBuild:
    path, metadata = v47.compile_one(
        compiler, source, output, profile, flags, provenance,
        evidence_source=evidence_source, extra_inputs=extra_inputs,
    )
    return ObjectBuild(path, metadata)


def replace_once(path: Path, marker: str, replacement: str, label: str) -> None:
    text = path.read_text(encoding="latin-1")
    if text.count(marker) != 1:
        raise SystemExit(f"Snes9x 1.41-1 {label} patch context changed")
    v47.atomic_write_text(path, text.replace(marker, replacement))


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
    marker = (
        "#ifdef FAST_LSB_WORD_ACCESS\n"
        "\t\treturn (*(uint16 *) (GetAddress + (Address & 0xffff)));\n"
        "#else"
    )
    replacement = (
        "#ifdef FAST_LSB_WORD_ACCESS\n"
        "\t\tregister uint32 value __asm__ (\"$16\") =\n"
        "\t\t\t((Hunt1041UnalignedUint32 *)\n"
        "\t\t\t (GetAddress + (Address & 0xffff)))->value;\n"
        "\t\tvalue &= 0xffff;\n"
        "\t\t__asm__ (\"\" : \"+r\" (value));\n"
        "\t\treturn value;\n"
        "#else"
    )
    replace_once(getset, marker, replacement, "S9xGetWord access")

    port = layout / "port.h"
    replace_once(
        port,
        "/* #define PIXEL_FORMAT RGB565 */\n#define GFX_MULTI_FORMAT",
        "#define PIXEL_FORMAT BGR555\n"
        "/* Fixed 15-bit pixel profile used by the PS2 frontend. */",
        "BGR555",
    )

    settings = layout / "snes9x.h"
    replace_once(
        settings,
        "  /*   uint32 StrikeGunnerOffsetHack; */\n"
        "    bool8  ChuckRock;",
        "  /*   uint32 StrikeGunnerOffsetHack; */\n"
        "    bool8  PS2PortLayoutByte;\n"
        "    bool8  ChuckRock;",
        "Settings one-byte port layout",
    )
    return source_root, original, layout


def build_objects(cc: Path, cxx: Path) -> dict[str, ObjectBuild]:
    objects: dict[str, ObjectBuild] = {}
    local_sources = {
        "small": ROOT / "src" / "ps2" / "small_dispatch_recovered.c",
        "p21": ROOT / "src" / "ps2" / "progress21_small_helpers_recovered.c",
        "p22": ROOT / "src" / "ps2" / "progress22_more_small_helpers_recovered.c",
        "p12": ROOT / "src" / "ps2" / "progress12_io_state_recovered.c",
    }
    local_profiles = {
        "local:small-o2": ("small", "o2"),
        "local:p21-o2": ("p21", "o2"),
        "local:p21-os": ("p21", "os"),
        "local:p22-os": ("p22", "os"),
        "local:p12-o2": ("p12", "o2"),
        "local:p12-os": ("p12", "os"),
    }
    for key, (source_key, profile) in local_profiles.items():
        source = local_sources[source_key]
        objects[key] = compile_one(
            cc, source, BUILD / "local" / f"{source_key}-{profile}.o",
            f"ee-gcc-3.2.2-{profile}",
            [*MINER_FLAGS, *PROFILES[profile]],
            "snesstation-v0.23-recovered",
        )

    v47.ensure_git_commit(v47.PGEN, v47.PGEN_REPO, v47.PGEN_COMMIT)
    amiga_source = ROOT / "matching" / "candidates" / "hunt1041_v51_amigamod.c"
    objects["candidate:amiga-os"] = compile_one(
        cc, amiga_source, BUILD / "candidate" / "amigamod-os.o",
        "pgen-os-snesstation-delta",
        [*v47.COMMON_FLAGS, "-Os", *v47.PS2_DEFINES,
         *v47.include_args([
             ROOT / "include", ROOT / "include" / "ee_stage1_compat",
             ROOT / "matching" / "ee_abi_compat",
         ])],
        v47.PGEN_COMMIT,
        extra_inputs=(v47.PGEN / "lib" / "amigamod_rpc.c",),
    )

    dsp_source = ROOT / "matching" / "candidates" / "hunt1041_v51_dsp.c"
    normal_double_flags = [
        flag for flag in v47.COMMON_FLAGS if flag != "-fshort-double"
    ]
    for profile, option in (("o2", "-O2"), ("os", "-Os")):
        objects[f"candidate:dsp-{profile}"] = compile_one(
            cc, dsp_source, BUILD / "candidate" / f"dsp-{profile}.o",
            f"snes9x-1.41-1-{profile}-normal-double-ps2-float",
            [*normal_double_flags, option, *v47.PS2_DEFINES],
            v47.SNES_141_1_ARCHIVE.sha256,
        )

    v47.ensure_git_commit(v47.PS2DEV, v47.PS2DEV_REPO, v47.PS2DEV_COMMIT)
    source_root, original, layout = prepare_snes_layout()
    newlib = (
        v47.PS2DEV / "ps2toolchain" / "soft" / "newlib-1.10.0"
        / "newlib" / "libc" / "include"
    )
    compat = BUILD / "compat"
    v47.atomic_write_text(
        compat / "memory.h",
        "#ifndef HUNT1041_V51_MEMORY_H\n#define HUNT1041_V51_MEMORY_H\n"
        "#include <string.h>\n#endif\n",
    )
    snes_flags = [
        *v47.COMMON_FLAGS, "-Os", *v47.SNES_DEFINES,
        *v47.include_args([
            compat, newlib, layout, layout / "unzip", source_root / "zlib",
        ]),
        "-x", "c++",
    ]
    settings = layout / "snes9x.h"
    getset = layout / "getset.h"
    port = layout / "port.h"
    objects["snes:memmap"] = compile_one(
        cxx, layout / "MEMMAP.CPP", BUILD / "historical" / "memmap.o",
        "snes9x-1.41-1-os-short-fast-lsb-ps2-settings-layout",
        [*snes_flags, "-DFAST_LSB_WORD_ACCESS"],
        v47.SNES_141_1_ARCHIVE.sha256,
        evidence_source=original / "MEMMAP.CPP",
        extra_inputs=(settings, getset, compat / "memory.h"),
    )
    objects["snes:ppu"] = compile_one(
        cxx, layout / "ppu.cpp", BUILD / "historical" / "ppu.o",
        "snes9x-1.41-1-os-short-bgr555-ps2-settings-layout",
        snes_flags,
        v47.SNES_141_1_ARCHIVE.sha256,
        evidence_source=original / "ppu.cpp",
        extra_inputs=(settings, port, compat / "memory.h"),
    )
    return objects


def make_evidence(
    target: bytes, objects: dict[str, ObjectBuild]
) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    elf_cache: dict[Path, ELFFile] = {}
    for candidate in CANDIDATES:
        built = objects[candidate.object_key]
        elf = elf_cache.setdefault(built.path, ELFFile(built.path))
        comparison = compare_function(
            target, candidate.address - TARGET_BASE, candidate.size,
            elf, candidate.symbol,
        )
        if not comparison.matching:
            offsets = ",".join(f"+0x{x:x}" for x in comparison.first_differences)
            raise SystemExit(
                f"0x{candidate.address:08x} {candidate.symbol}: DIFF "
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
                "object": rel(built.path),
                "object_symbol": candidate.symbol,
                "object_size": str(comparison.candidate_size),
                "boundary": EXACT,
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(comparison.raw_equal),
                "normalized_equal": str(comparison.normalized_equal),
                "unknown_relocations": "",
                "object_sha256": sha256_file(built.path),
                "cache_key": str(built.metadata["cache_key"]),
                "target_gate": f"formal-unpacked-elf:{TARGET_SHA256}",
                "target_span_sha256": hashlib.sha256(span).hexdigest(),
            }
        )
    if len(rows) != 16 or len({row["address"] for row in rows}) != 16:
        raise SystemExit("V51 evidence cardinality gate failed")
    return rows


def promote(rows: list[dict[str, str]]) -> int:
    target_fields, target_rows = read_csv(TARGETS)
    symbol_fields, symbol_rows = read_csv(SYMBOLS)
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
            "HUNT1041 V51 closure strict MATCH; "
            f"mode={evidence['detail'].split(';', 1)[0]}; "
            f"provenance={evidence['provenance']}; "
            f"source={evidence['source']}; profile={evidence['profile']}; "
            f"object_symbol={evidence['object_symbol']}; boundary={EXACT}; "
            f"target_gate={evidence['target_gate']}; differing_bytes=0; "
            "normalized_equal=True; unknown_relocations=none; "
            f"evidence={rel(EVIDENCE)}"
        )
        if target["status"] == "MATCHING":
            marker = "HUNT1041 V51 closure strict MATCH;"
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
        help="promote the 16 validated rows after writing evidence",
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
    write_csv_atomic(EVIDENCE, EVIDENCE_FIELDS, rows, delimiter="\t")
    print(f"V51 strict matches: {len(rows)} (exact={len(rows)})")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print(f"evidence: {rel(EVIDENCE)}")
    if args.apply:
        print(f"promoted strict matches: {promote(rows)}")
    else:
        print("dry promotion; pass --apply to update the manifests")


if __name__ == "__main__":
    main()
