#!/usr/bin/env python3
"""Rebuild and verify the 79 strict matches in the HUNT1000+ V47 closure."""
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

from bootstrap_ee_gcc_stage1 import (  # noqa: E402
    Archive,
    BuildFailure,
    atomic_write_text,
    download_archive,
    safe_extract_archive,
)
from compare_elf_functions import ELFFile, compare_function  # noqa: E402
from run_match_miner import has_terminal_control_flow  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

PGEN_REPO = "https://github.com/ps2homebrew/pgen.git"
PGEN_COMMIT = "403f1710e5eacb7d04e5031e1cb0a40435ff9d33"
PS2DEV_REPO = "https://github.com/duduclx/PS2DEV.git"
PS2DEV_COMMIT = "bac0006c6302edcf1bdae253799484497b4e5032"
PS2SDK_REPO = "https://github.com/ps2dev/ps2sdk.git"
PS2SDK_20040415_COMMIT = "694100b78ad5bc8f8248a1138143860af4f8435f"
PS2SDK_20040418_COMMIT = "a80df908256955382f102278400b5d713552dbce"

SNES_141_1_ARCHIVE = Archive(
    name="snes9x-1.41-1-src.tar.gz",
    url="https://www.lysator.liu.se/snes9x/1.41-1/snes9x-1.41-1-src.tar.gz",
    sha256="5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b",
    source_directory="snes9x-1.41-1-src",
    historical_patch="",
)
SNES_141_ARCHIVE = Archive(
    name="snes9x-1.41-src.tar.gz",
    url="https://www.lysator.liu.se/snes9x/1.41/snes9x-1.41-src.tar.gz",
    sha256="f24e5761fd91078c124241e8631370a6bd182b8dde9661d24e9761898d1838f3",
    source_directory="snes9x-1.41-src",
    historical_patch="",
)

BUILD = ROOT / "build" / "matching" / "hunt1000plus-v47-closure"
UPSTREAM = ROOT / "build" / "upstream"
SNES_CACHE = UPSTREAM / "hunt1000plus-v47-snes"
PGEN = UPSTREAM / "pgen-403f1710"
PS2DEV = UPSTREAM / "PS2DEV-bac0006c"
PS2SDK_20040415 = UPSTREAM / "ps2sdk-20040415"
PS2SDK_20040418 = UPSTREAM / "ps2sdk-20040418"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1000plus-v47-validated-79.tsv"
MAX_TERMINAL_SIZE = 4096

COMMON_FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
)
PS2_DEFINES = (
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD",
    "-DCODE_PLATFORM=3",
)
SNES_DEFINES = (
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DVAR_CYCLES",
    "-DCPU_SHUTDOWN", "-DSPC700_SHUTDOWN",
    "-DEXECUTE_SUPERFX_PER_LINE", "-DSPC700_C", "-DUNZIP_SUPPORT",
    "-DNO_INLINE_SET_GET",
)
PS2LIB_FLAGS = ("-D_EE", "-DPS2_EE", "-G0", "-EL", "-pipe", "-w", "-Os")
EVIDENCE_FIELDS = (
    "address", "name", "area", "provenance", "source", "profile",
    "detail", "object", "object_symbol", "object_size", "boundary",
    "result", "differing_bytes", "raw_equal", "normalized_equal",
    "unknown_relocations", "object_sha256", "cache_key",
)

EXACT = "exact-next-boundary"
TERMINAL = "terminal-control-flow-boundary"


@dataclass(frozen=True)
class Candidate:
    address: int
    expected_name: str
    object_key: str
    symbol: str
    size: int
    boundary: str
    provenance: str
    detail: str = "historical-symbol-strict"


CANDIDATES = (
    # Small application/frontend bodies reconstructed from the target.
    Candidate(0x00101814, "snes_leaf_00101814", "app", "v47_leaf_control", 36, TERMINAL, "snesstation-v0.23-recovered", "recovered-source-strict; identity=control-clear-leaf"),
    Candidate(0x00101B04, "snes_p11_00101b04", "app", "v47_gs_clear", 96, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=gslib-clear"),
    Candidate(0x00103C7C, "snes_dispatch_00103c7c", "app", "v47_cdvd_getdir", 52, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=cdvd-getdir-wrapper"),
    Candidate(0x00103CB0, "snes_leaf_00103cb0", "app", "v47_record_is_file", 36, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=file-record-test"),
    Candidate(0x00103D90, "snes_p12_00103d90", "app", "v47_cdfs_path", 68, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=cdfs-path-builder"),
    Candidate(0x001041C0, "snes_leaf_001041c0", "app", "v47_leaf_control", 36, TERMINAL, "snesstation-v0.23-recovered", "recovered-source-strict; identity=control-clear-leaf-copy"),
    Candidate(0x00104998, "mtapInit_00104998", "app", "v47_pad_wait", 88, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=pad-stable-state-wait"),
    Candidate(0x001049F0, "mtapPortOpen_001049f0", "app", "v47_pad_open", 100, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=pad-main-mode-init"),
    Candidate(0x00105718, "snes_dispatch_00105718", "app", "v47_snapshot_path", 56, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=snapshot-path"),
    Candidate(0x00105D30, "snes_dispatch_00105d30", "app", "v47_save_sram", 72, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=conditional-sram-save"),
    Candidate(0x00105E04, "snes_dispatch_00105e04", "app", "v47_audio_shutdown", 52, TERMINAL, "snesstation-v0.23-recovered", "recovered-source-strict; identity=audio-shutdown"),
    Candidate(0x00106824, "snes_p12_00106824", "app", "v47_mc_probe", 120, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=memory-card-probe"),
    Candidate(0x001080CC, "snes_qsort_001080cc", "newlib:qsort", "qsort", 2408, EXACT, "newlib-1.10.0-ps2dev-bac0006c"),
    Candidate(0x00108A34, "snes_rand_00108a34", "app", "v47_snes_rand", 104, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=lcg-rand"),

    # Snes9x 1.41-1 fixed RGB555 renderer profile.
    Candidate(0x00146720, "DrawBGMode7Background16Add", "snes:gfx", "_Z26DrawBGMode7Background16AddPhi", 1868, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x00146E6C, "DrawBGMode7Background16Add1_2", "snes:gfx", "_Z29DrawBGMode7Background16Add1_2Phi", 1804, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x00147578, "DrawBGMode7Background16Sub", "snes:gfx", "_Z26DrawBGMode7Background16SubPhi", 1776, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x00147C68, "DrawBGMode7Background16Sub1_2", "snes:gfx", "_Z29DrawBGMode7Background16Sub1_2Phi", 1744, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x00148338, "snes_leaf_00148338", "snes:gfx", "_Z13Q_INTERPOLATEjjjj", 92, EXACT, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=Q_INTERPOLATE"),
    Candidate(0x00148394, "DrawBGMode7Background16_i", "snes:gfx", "_Z25DrawBGMode7Background16_iPhi", 4116, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001493A8, "DrawBGMode7Background16Add_i", "snes:gfx", "_Z28DrawBGMode7Background16Add_iPhi", 5084, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x0014A784, "DrawBGMode7Background16Add1_2_i", "snes:gfx", "_Z31DrawBGMode7Background16Add1_2_iPhi", 4920, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x0014BABC, "DrawBGMode7Background16Sub_i", "snes:gfx", "_Z28DrawBGMode7Background16Sub_iPhi", 4968, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x0014CE24, "DrawBGMode7Background16Sub1_2_i", "snes:gfx", "_Z31DrawBGMode7Background16Sub1_2_iPhi", 4892, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x0014E828, "DisplayChar", "snes:gfx", "_Z11DisplayCharPhh", 420, TERMINAL, "snes9x-1.41-1-official"),
    Candidate(0x0018A6D4, "DrawLargePixel16Add", "snes:tile", "_Z19DrawLargePixel16Addjjjjjj", 1764, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x0018ADB8, "DrawLargePixel16Add1_2", "snes:tile", "_Z22DrawLargePixel16Add1_2jjjjjj", 1668, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x0018B43C, "DrawLargePixel16Sub", "snes:tile", "_Z19DrawLargePixel16Subjjjjjj", 1668, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x0018BAC0, "DrawLargePixel16Sub1_2", "snes:tile", "_Z22DrawLargePixel16Sub1_2jjjjjj", 1636, EXACT, "snes9x-1.41-1-official"),

    # PGEN unzip/zlib overlay and the official Snes9x 1.41 zlib leaf.
    Candidate(0x0018DC60, "ReadByte", "pgen:explode", "ReadByte", 268, EXACT, "pgen-403f1710"),
    Candidate(0x0018F010, "unzlocal_getByte", "pgen:unzip", "unzlocal_getByte", 96, EXACT, "pgen-403f1710"),
    Candidate(0x0018F27C, "unzlocal_SearchCentralDir", "pgen:unzip", "unzlocal_SearchCentralDir", 396, EXACT, "pgen-403f1710"),
    Candidate(0x0018F408, "unzOpen", "pgen:unzip", "unzOpen", 472, EXACT, "pgen-403f1710"),
    Candidate(0x0018F6CC, "unzlocal_GetCurrentFileInfoInternal", "pgen:unzip", "unzlocal_GetCurrentFileInfoInternal", 984, EXACT, "pgen-403f1710"),
    Candidate(0x0018FCEC, "unzlocal_CheckCurrentFileCoherencyHeader", "pgen:unzip", "unzlocal_CheckCurrentFileCoherencyHeader", 640, EXACT, "pgen-403f1710"),
    Candidate(0x001900D4, "unzReadCurrentFile", "pgen:unzip", "unzReadCurrentFile", 900, EXACT, "pgen-403f1710"),
    Candidate(0x001904A0, "unzGetLocalExtrafield", "pgen:unzip", "unzGetLocalExtrafield", 216, EXACT, "pgen-403f1710"),
    Candidate(0x00190628, "unzGetGlobalComment", "pgen:unzip", "unzGetGlobalComment", 216, EXACT, "pgen-403f1710"),
    Candidate(0x0019165C, "read_buf", "snes:zlib141", "read_buf", 176, EXACT, "snes9x-1.41-official"),
    Candidate(0x00193298, "gz_open", "pgen:gzio", "gz_open", 716, EXACT, "pgen-403f1710"),
    Candidate(0x001940EC, "gzseek", "pgen:gzio", "gzseek", 488, EXACT, "pgen-403f1710"),

    # Pre-2.0 PS2SDK/PS2LIB split translation units.
    Candidate(0x0019D244, "fioWrite", "kernel:fio-write", "fioWrite", 284, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019D4B0, "_fio_read_intr", "kernel:fio-read-intr", "_fio_read_intr", 132, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019D63C, "SifAllocIopHeap", "kernel:iop-alloc", "SifAllocIopHeap", 124, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019D6B8, "SifFreeIopHeap", "kernel:iop-free", "SifFreeIopHeap", 136, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019D740, "SifIopReset", "kernel:iop-reset", "SifIopReset", 268, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019D84C, "fmtint", "libc:xprintf", "append_number", 860, EXACT, "ps2sdk-694100b", "anchored-object-layout; historical=append_number"),
    Candidate(0x0019DBA8, "fmtstr", "libc:xprintf", "append_string", 384, EXACT, "ps2sdk-694100b", "anchored-object-layout; historical=append_string"),
    Candidate(0x0019DD28, "fmtchar", "libc:xprintf", "append_char", 232, EXACT, "ps2sdk-694100b", "anchored-object-layout; historical=append_char"),
    Candidate(0x0019DE10, "dopr", "libc:xprintf", "xyzprintf", 1124, EXACT, "ps2sdk-694100b", "anchored-object-layout; historical=xyzprintf"),
    Candidate(0x0019E2E0, "vsnprintf", "libc:xprintf", "vsnprintf", 132, EXACT, "ps2sdk-694100b"),
    Candidate(0x0019E414, "puts_like", "app", "v47_puts_like", 96, EXACT, "snesstation-v0.23-recovered", "recovered-source-strict; identity=fioWrite-puts"),
    Candidate(0x0019E4B4, "malloc", "libc:malloc", "malloc", 404, EXACT, "ps2sdk-694100b"),
    Candidate(0x0019E648, "calloc", "libc:calloc", "calloc", 80, EXACT, "ps2sdk-694100b"),
    Candidate(0x0019E698, "memalign", "libc:memalign", "memalign", 236, EXACT, "ps2sdk-694100b"),
    Candidate(0x0019E784, "free", "libc:free", "free", 220, EXACT, "ps2sdk-694100b"),
    Candidate(0x0019EAF8, "strstr", "libc:strstr", "strstr", 136, EXACT, "ps2sdk-694100b"),
    Candidate(0x0019EB80, "strtol", "libc:strtol", "strtol", 556, EXACT, "ps2sdk-694100b"),
    Candidate(0x0019F078, "ps2_sbrk", "libc:sbrk", "ps2_sbrk", 192, EXACT, "ps2sdk-694100b"),
    Candidate(0x0019F304, "SifInitCmd", "kernel:sif-init", "SifInitCmd", 524, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019F600, "fioInit", "kernel:fio-main", "fioInit", 232, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019F7E8, "_SifLoadModule", "kernel:load-module", "_SifLoadModule", 268, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019F8F4, "_SifLoadModuleBuffer", "kernel:load-buffer", "_SifLoadModuleBuffer", 244, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019F9E8, "SifInitIopHeap", "kernel:iop-init", "SifInitIopHeap", 192, EXACT, "ps2sdk-a80df908"),
    Candidate(0x0019FD20, "SifLoadFileInit", "kernel:load-init", "SifLoadFileInit", 188, EXACT, "ps2sdk-a80df908"),
    Candidate(0x001A07C8, "mcReadFixAlign", "rpc:libmc", "mcReadFixAlign", 164, TERMINAL, "ps2sdk-694100b"),
    Candidate(0x001A130C, "mcSetFileInfo", "rpc:libmc", "mcSetFileInfo", 368, EXACT, "ps2sdk-694100b"),

    # RGB555 tile template instantiations.
    Candidate(0x001ADE2C, "WRITE_4PIXELS16_ADD", "snes:tile", "_Z19WRITE_4PIXELS16_ADDjPh", 1172, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001AE2C0, "WRITE_4PIXELS16_FLIPPED_ADD", "snes:tile", "_Z27WRITE_4PIXELS16_FLIPPED_ADDjPh", 1172, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001AE754, "WRITE_4PIXELS16_ADD1_2", "snes:tile", "_Z22WRITE_4PIXELS16_ADD1_2jPh", 1028, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001AEB58, "WRITE_4PIXELS16_FLIPPED_ADD1_2", "snes:tile", "_Z30WRITE_4PIXELS16_FLIPPED_ADD1_2jPh", 1028, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001AEF5C, "WRITE_4PIXELS16_SUB", "snes:tile", "_Z19WRITE_4PIXELS16_SUBjPh", 948, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001AF310, "WRITE_4PIXELS16_FLIPPED_SUB", "snes:tile", "_Z27WRITE_4PIXELS16_FLIPPED_SUBjPh", 948, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001AF6C4, "WRITE_4PIXELS16_SUB1_2", "snes:tile", "_Z22WRITE_4PIXELS16_SUB1_2jPh", 964, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001AFA88, "WRITE_4PIXELS16_FLIPPED_SUB1_2", "snes:tile", "_Z30WRITE_4PIXELS16_FLIPPED_SUB1_2jPh", 964, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001AFE4C, "WRITE_4PIXELS16_ADDF1_2", "snes:tile", "_Z23WRITE_4PIXELS16_ADDF1_2jPh", 576, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001B008C, "WRITE_4PIXELS16_FLIPPED_ADDF1_2", "snes:tile", "_Z31WRITE_4PIXELS16_FLIPPED_ADDF1_2jPh", 576, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001B02CC, "WRITE_4PIXELS16_SUBF1_2", "snes:tile", "_Z23WRITE_4PIXELS16_SUBF1_2jPh", 608, EXACT, "snes9x-1.41-1-official"),
    Candidate(0x001B052C, "WRITE_4PIXELS16_FLIPPED_SUBF1_2", "snes:tile", "_Z31WRITE_4PIXELS16_FLIPPED_SUBF1_2jPh", 608, TERMINAL, "snes9x-1.41-1-official"),
)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def rel(path: Path) -> str:
    # Keep the repository-relative spelling even when a developer reuses a
    # large build directory through a local symlink.
    return path.absolute().relative_to(ROOT.absolute()).as_posix()


def run(command: list[str], *, cwd: Path = ROOT, allow_failure: bool = False) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(
        command, cwd=cwd, text=True, stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT, check=False,
    )
    if result.returncode and not allow_failure:
        raise SystemExit(
            f"command failed ({result.returncode}): {' '.join(command)}\n"
            f"{result.stdout[-6000:]}"
        )
    return result


def ensure_git_commit(path: Path, repository: str, commit: str) -> None:
    if not (path / ".git").exists():
        if path.exists() and any(path.iterdir()):
            raise SystemExit(f"refusing non-Git upstream directory: {path}")
        path.mkdir(parents=True, exist_ok=True)
        run(["git", "init", "-q", str(path)])
        run(["git", "-C", str(path), "remote", "add", "origin", repository])
    current = run(
        ["git", "-C", str(path), "rev-parse", "HEAD"], allow_failure=True
    ).stdout.strip()
    if current != commit:
        run(["git", "-C", str(path), "fetch", "-q", "--depth=1", "origin", commit])
        run(["git", "-C", str(path), "checkout", "-q", "--detach", "FETCH_HEAD"])
    actual = run(["git", "-C", str(path), "rev-parse", "HEAD"]).stdout.strip()
    if actual != commit:
        raise SystemExit(f"upstream checkout mismatch: wanted {commit}, got {actual}")
    dirty = run(
        ["git", "-C", str(path), "status", "--porcelain", "--untracked-files=no"]
    ).stdout.strip()
    if dirty:
        raise SystemExit(f"pinned upstream checkout has tracked changes: {path}")


def include_args(paths: list[Path]) -> list[str]:
    result: list[str] = []
    for path in paths:
        result.extend(("-I", str(path)))
    return result


def compile_one(
    compiler: Path,
    source: Path,
    output: Path,
    profile: str,
    flags: list[str],
    upstream_identity: str,
    *,
    evidence_source: Path | None = None,
    extra_inputs: tuple[Path, ...] = (),
) -> tuple[Path, dict[str, str]]:
    output.parent.mkdir(parents=True, exist_ok=True)
    run([str(compiler), *flags, "-c", str(source), "-o", str(output)])
    payload = {
        "compiler_sha256": sha256_file(compiler),
        "flags": flags,
        "source_sha256": sha256_file(source),
        "extra_inputs": {rel(path): sha256_file(path) for path in extra_inputs},
        "upstream": upstream_identity,
    }
    metadata = {
        "cache_key": hashlib.sha256(
            json.dumps(payload, sort_keys=True, separators=(",", ":")).encode()
        ).hexdigest(),
        "source": rel(evidence_source or source),
        "profile": profile,
    }
    atomic_write_text(
        output.with_suffix(".json"),
        json.dumps(metadata, indent=2, sort_keys=True) + "\n",
    )
    return output, metadata


def prepare_archives() -> tuple[Path, Path]:
    try:
        archive_141_1 = download_archive(SNES_141_1_ARCHIVE, SNES_CACHE)
        root_141_1 = safe_extract_archive(
            archive_141_1,
            SNES_CACHE / "source-1.41-1",
            SNES_141_1_ARCHIVE.source_directory,
        )
        archive_141 = download_archive(SNES_141_ARCHIVE, SNES_CACHE)
        root_141 = safe_extract_archive(
            archive_141,
            SNES_CACHE / "source-1.41",
            SNES_141_ARCHIVE.source_directory,
        )
    except BuildFailure as exc:
        raise SystemExit(str(exc)) from exc
    return root_141_1, root_141


def build_objects(cc: Path, cxx: Path) -> dict[str, tuple[Path, dict[str, str]]]:
    snes_141_1, snes_141 = prepare_archives()
    newlib = (
        PS2DEV / "ps2toolchain" / "soft" / "newlib-1.10.0"
        / "newlib" / "libc" / "include"
    )
    for path, label in (
        (newlib, "Newlib headers"),
        (snes_141_1 / "snes9x", "Snes9x 1.41-1 source"),
        (snes_141 / "zlib", "Snes9x 1.41 zlib source"),
    ):
        if not path.is_dir():
            raise SystemExit(f"missing {label}: {path}")

    objects: dict[str, tuple[Path, dict[str, str]]] = {}

    # Recovered application candidates.
    app_source = ROOT / "matching" / "candidates" / "hunt1000plus_v47.c"
    objects["app"] = compile_one(
        cc, app_source, BUILD / "app" / "hunt1000plus_v47.o",
        "recovered-os-r5900", [*COMMON_FLAGS, "-Os"], "target-recovery-v47",
    )

    # Newlib qsort needs the historical 32-bit size_t declaration used by this TU.
    qsort_compat = BUILD / "compat" / "qsort"
    atomic_write_text(
        qsort_compat / "_ansi.h",
        "#ifndef HUNT1000PLUS_V47_ANSI_H\n#define HUNT1000PLUS_V47_ANSI_H\n"
        "#define _PARAMS(parameters) parameters\n"
        "#define _DEFUN(name, arglist, args) name(args)\n"
        "#define _DEFUN_VOID(name) name(void)\n#define _AND ,\n#endif\n",
    )
    atomic_write_text(
        qsort_compat / "stdlib.h",
        "#ifndef HUNT1000PLUS_V47_STDLIB_H\n#define HUNT1000PLUS_V47_STDLIB_H\n"
        "#define size_t int\n#endif\n",
    )
    qsort_source = (
        PS2DEV / "ps2toolchain" / "soft" / "newlib-1.10.0" / "newlib"
        / "libc" / "stdlib" / "qsort.c"
    )
    objects["newlib:qsort"] = compile_one(
        cc, qsort_source, BUILD / "newlib" / "qsort.o",
        "newlib-1.10-os-size_t-int",
        [*COMMON_FLAGS, "-Os", *include_args([qsort_compat])],
        PS2DEV_COMMIT, extra_inputs=(qsort_compat / "_ansi.h", qsort_compat / "stdlib.h"),
    )

    # The PS2 renderer selected a fixed RGB555 format instead of the desktop
    # multi-format dispatcher. Patch a private build copy, never the archive.
    original_snes = snes_141_1 / "snes9x"
    fixed_snes = BUILD / "upstream-profile" / "snes9x-1.41-1-rgb555"
    shutil.copytree(original_snes, fixed_snes, dirs_exist_ok=True)
    port = fixed_snes / "port.h"
    port_text = port.read_text(encoding="latin-1")
    marker = "/* #define PIXEL_FORMAT RGB565 */\n#define GFX_MULTI_FORMAT"
    if port_text.count(marker) != 1:
        raise SystemExit("Snes9x 1.41-1 RGB555 patch context changed")
    atomic_write_text(
        port,
        port_text.replace(
            marker,
            "#define PIXEL_FORMAT RGB555\n"
            "/* Fixed 15-bit renderer profile used by the PS2 frontend. */",
        ),
    )
    snes_compat = BUILD / "compat" / "snes"
    atomic_write_text(
        snes_compat / "memory.h",
        "#ifndef HUNT1000PLUS_V47_MEMORY_H\n#define HUNT1000PLUS_V47_MEMORY_H\n"
        "#include <string.h>\n#endif\n",
    )
    snes_flags = [
        *COMMON_FLAGS, "-Os", *SNES_DEFINES,
        *include_args([
            snes_compat, newlib, fixed_snes, fixed_snes / "unzip",
            snes_141_1 / "zlib",
        ]),
        "-x", "c++",
    ]
    for key, filename in (("gfx", "GFX.CPP"), ("tile", "TILE.CPP")):
        objects[f"snes:{key}"] = compile_one(
            cxx, fixed_snes / filename, BUILD / "snes" / f"{key}.o",
            "snes9x-1.41-1-fixed-rgb555-os-short", snes_flags,
            f"lysator:{SNES_141_1_ARCHIVE.sha256}",
            evidence_source=original_snes / filename,
            extra_inputs=(port, snes_compat / "memory.h"),
        )

    # The read_buf leaf is from the immediately preceding official 1.41 tarball.
    zlib_141_flags = [
        "-G0", "-EL", "-pipe", "-w", "-Os", "-fshort-double",
        *include_args([newlib, snes_141 / "snes9x", snes_141 / "snes9x" / "unzip", snes_141 / "zlib"]),
    ]
    objects["snes:zlib141"] = compile_one(
        cc, snes_141 / "zlib" / "deflate.c", BUILD / "snes" / "deflate-1.41.o",
        "snes9x-1.41-zlib-os-short", zlib_141_flags,
        f"lysator:{SNES_141_ARCHIVE.sha256}",
    )

    # PGEN carries the matching unzip overlay. A narrow compatibility header
    # preserves the old int strlen declaration that shaped gz_open.
    pgen_compat = BUILD / "compat" / "pgen"
    atomic_write_text(
        pgen_compat / "string.h",
        "#ifndef HUNT1000PLUS_V47_STRING_H\n#define HUNT1000PLUS_V47_STRING_H\n"
        "#include \"_ansi.h\"\n#include <sys/reent.h>\n"
        "#define __need_size_t\n#include <stddef.h>\n"
        "#ifndef NULL\n#define NULL 0\n#endif\n"
        "_PTR memchr(const _PTR, int, size_t);\n"
        "int memcmp(const _PTR, const _PTR, size_t);\n"
        "_PTR memcpy(_PTR, const _PTR, size_t);\n"
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
    modern_ps2_includes = sorted(
        path for path in (PS2DEV / "ps2sdk").rglob("include") if path.is_dir()
    )
    pgen_includes = [
        PGEN, PGEN / "ps2", PGEN / "unzip", PGEN / "zlib", newlib,
        *modern_ps2_includes,
    ]
    pgen_flags = [*COMMON_FLAGS, "-Os", *PS2_DEFINES, *include_args(pgen_includes)]
    for key, source in (
        ("pgen:explode", PGEN / "unzip" / "explode.c"),
        ("pgen:unzip", PGEN / "unzip" / "unzip.c"),
    ):
        objects[key] = compile_one(
            cc, source, BUILD / "pgen" / f"{key.split(':')[1]}.o",
            "pgen-os-short", pgen_flags, PGEN_COMMIT,
        )
    objects["pgen:gzio"] = compile_one(
        cc, PGEN / "zlib" / "gzio.c", BUILD / "pgen" / "gzio.o",
        "pgen-os-short-int-strlen",
        [*COMMON_FLAGS, "-Os", *PS2_DEFINES, *include_args([pgen_compat, *pgen_includes])],
        PGEN_COMMIT, extra_inputs=(pgen_compat / "string.h",),
    )

    # Old PS2LIB libc split objects from 15 April 2004.
    old_libc_includes = [
        PS2SDK_20040415 / "ee" / "kernel" / "include",
        PS2SDK_20040415 / "ee" / "libc" / "include",
        PS2DEV / "ps2sdk" / "common" / "include",
        PS2DEV / "ps2sdk" / "ee" / "kernel" / "include",
        PS2DEV / "ps2sdk" / "ee" / "libc" / "include",
    ]
    old_libc_flags = [*PS2LIB_FLAGS, *include_args(old_libc_includes)]
    libc_builds = (
        ("libc:xprintf", "xprintf.c", "F_vsnprintf"),
        ("libc:malloc", "alloc.c", "F_malloc"),
        ("libc:calloc", "alloc.c", "F_calloc"),
        ("libc:memalign", "alloc.c", "F_memalign"),
        ("libc:free", "alloc.c", "F_free"),
        ("libc:strstr", "string.c", "F_strstr"),
        ("libc:strtol", "string.c", "F_strtol"),
        ("libc:sbrk", "sbrk.c", ""),
    )
    for key, filename, macro in libc_builds:
        source = PS2SDK_20040415 / "ee" / "libc" / "src" / filename
        if key == "libc:strtol":
            # This snapshot's limits contract comes from the first shared
            # common/include tree, added three days after the libc source.
            flags = [
                *PS2LIB_FLAGS,
                *include_args([
                    PS2SDK_20040418 / "common" / "include",
                    PS2SDK_20040415 / "ee" / "kernel" / "include",
                    PS2SDK_20040415 / "ee" / "libc" / "include",
                ]),
            ]
        else:
            flags = [*old_libc_flags]
        if macro:
            flags.insert(len(PS2LIB_FLAGS), f"-D{macro}")
        objects[key] = compile_one(
            cc, source, BUILD / "ps2lib" / f"{key.split(':')[1]}.o",
            "ps2lib-20040415-os-split", flags, PS2SDK_20040415_COMMIT,
        )

    # Old memory-card TU uses the pre-mlong64 default ABI.
    libmc_source = PS2SDK_20040415 / "ee" / "rpc" / "memorycard" / "src" / "libmc.c"
    libmc_includes = [
        PS2SDK_20040415 / "ee" / "rpc" / "memorycard" / "include",
        *old_libc_includes,
    ]
    objects["rpc:libmc"] = compile_one(
        cc, libmc_source, BUILD / "ps2lib" / "libmc.o",
        "ps2lib-20040415-os-tu",
        [*PS2LIB_FLAGS, *include_args(libmc_includes)],
        PS2SDK_20040415_COMMIT,
    )

    # Kernel split objects from 18 April 2004, with historical headers first.
    old_kernel_includes = [
        PS2SDK_20040418 / "common" / "include",
        PS2SDK_20040418 / "ee" / "kernel" / "include",
        PS2SDK_20040418 / "ee" / "libc" / "include",
        *sorted(
            path for path in (PS2SDK_20040418 / "ee" / "rpc").rglob("include")
            if path.is_dir()
        ),
        *modern_ps2_includes,
    ]
    kernel_builds = (
        ("kernel:fio-write", "fileio.c", "F_fio_write"),
        ("kernel:fio-read-intr", "fileio.c", "F__fio_read_intr"),
        ("kernel:fio-main", "fileio.c", "F_fio_main"),
        ("kernel:iop-reset", "iopcontrol.c", "F_SifIopReset"),
        ("kernel:iop-alloc", "iopheap.c", "F_SifAllocIopHeap"),
        ("kernel:iop-free", "iopheap.c", "F_SifFreeIopHeap"),
        ("kernel:iop-init", "iopheap.c", "F_SifInitIopHeap"),
        ("kernel:load-module", "loadfile.c", "F__SifLoadModule"),
        ("kernel:load-buffer", "loadfile.c", "F__SifLoadModuleBuffer"),
        ("kernel:load-init", "loadfile.c", "F_SifLoadFileInit"),
        ("kernel:sif-init", "sifcmd.c", "F_sif_cmd_main"),
    )
    for key, filename, macro in kernel_builds:
        source = PS2SDK_20040418 / "ee" / "kernel" / "src" / filename
        objects[key] = compile_one(
            cc, source, BUILD / "kernel" / f"{key.split(':')[1]}.o",
            "ps2lib-20040418-os-split",
            [*PS2LIB_FLAGS, f"-D{macro}", *include_args(old_kernel_includes)],
            PS2SDK_20040418_COMMIT,
        )
    return objects


def load_progress() -> tuple[dict[int, dict[str, str]], dict[int, int]]:
    with (ROOT / "analysis" / "progress_targets.csv").open(
        encoding="utf-8", newline=""
    ) as stream:
        rows = list(csv.DictReader(stream))
    progress = {int(row["address"], 0): row for row in rows}
    starts = sorted(progress)
    return progress, {start: end for start, end in zip(starts, starts[1:])}


def make_evidence(
    reference: bytes,
    progress: dict[int, dict[str, str]],
    next_address: dict[int, int],
    objects: dict[str, tuple[Path, dict[str, str]]],
) -> list[dict[str, str]]:
    rows: list[dict[str, str]] = []
    for spec in sorted(CANDIDATES, key=lambda item: item.address):
        manifest = progress.get(spec.address)
        end = next_address.get(spec.address)
        if manifest is None or end is None:
            raise SystemExit(f"0x{spec.address:08x}: missing audited boundary")
        if manifest["name"] != spec.expected_name:
            raise SystemExit(f"0x{spec.address:08x}: manifest identity changed")
        if manifest["status"] not in {"RECONSTRUCTED", "MATCHING"}:
            raise SystemExit(f"0x{spec.address:08x}: unexpected manifest status")
        object_path, object_metadata = objects[spec.object_key]
        elf = ELFFile(object_path)
        symbol = elf.find_symbol(spec.symbol)
        if symbol.size != spec.size:
            raise SystemExit(
                f"0x{spec.address:08x}: expected size {spec.size}, got {symbol.size}"
            )
        span = end - spec.address
        candidate_bytes = elf.symbol_bytes(symbol, symbol.size)
        if spec.boundary == EXACT and symbol.size != span:
            raise SystemExit(f"0x{spec.address:08x}: exact boundary changed")
        if spec.boundary == TERMINAL:
            if symbol.size >= span or symbol.size > MAX_TERMINAL_SIZE:
                raise SystemExit(f"0x{spec.address:08x}: invalid terminal prefix")
            if not has_terminal_control_flow(candidate_bytes, elf.endian):
                raise SystemExit(f"0x{spec.address:08x}: terminal control flow missing")

        comparison = compare_function(
            reference, spec.address - TARGET_BASE, symbol.size, elf, symbol.name
        )
        if not comparison.matching or comparison.differing_bytes:
            raise SystemExit(f"0x{spec.address:08x}: strict comparison failed")
        if comparison.unknown_relocation_types:
            raise SystemExit(f"0x{spec.address:08x}: unknown relocation type")
        rows.append(
            {
                "address": f"0x{spec.address:08x}",
                "name": manifest["name"],
                "area": manifest["area"],
                "provenance": spec.provenance,
                "source": object_metadata["source"],
                "profile": object_metadata["profile"],
                "detail": (
                    f"{spec.detail}; object-size={symbol.size}; "
                    f"relocations={len(comparison.relocation_ranges)}"
                ),
                "object": rel(object_path),
                "object_symbol": symbol.name,
                "object_size": str(symbol.size),
                "boundary": spec.boundary,
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(comparison.raw_equal),
                "normalized_equal": "True",
                "unknown_relocations": "",
                "object_sha256": sha256_file(object_path),
                "cache_key": object_metadata["cache_key"],
            }
        )
    return rows


def write_evidence(rows: list[dict[str, str]]) -> None:
    expected = {f"0x{candidate.address:08x}" for candidate in CANDIDATES}
    if len(CANDIDATES) != 79 or len(rows) != 79:
        raise SystemExit("V47 evidence cardinality gate failed")
    if {row["address"] for row in rows} != expected:
        raise SystemExit("V47 evidence address gate failed")
    if any(
        row["result"] != "MATCH"
        or row["differing_bytes"] != "0"
        or row["normalized_equal"] != "True"
        or row["unknown_relocations"]
        for row in rows
    ):
        raise SystemExit("V47 strict evidence gate failed")
    EVIDENCE.parent.mkdir(parents=True, exist_ok=True)
    temporary = EVIDENCE.with_suffix(EVIDENCE.suffix + ".tmp")
    with temporary.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(
            stream, fieldnames=EVIDENCE_FIELDS, delimiter="\t", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(rows)
    os.replace(temporary, EVIDENCE)


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
    args = parser.parse_args()
    cc = args.cc.expanduser().resolve()
    cxx = args.cxx.expanduser().resolve()
    reference_path = ROOT / "build" / "SNES_EMU.unpacked.bin"
    for path, label in ((cc, "C compiler"), (cxx, "C++ compiler"), (reference_path, "reference")):
        if not path.is_file():
            raise SystemExit(f"missing {label}: {path}")
    if sha256_file(reference_path) != TARGET_SHA256:
        raise SystemExit("unpacked target SHA-256 mismatch")
    run([sys.executable, str(ROOT / "tools" / "probe_ee_toolchain.py"), "--compiler", str(cc)])
    run([sys.executable, str(ROOT / "tools" / "probe_ee_toolchain.py"), "--compiler", str(cxx)])

    ensure_git_commit(PGEN, PGEN_REPO, PGEN_COMMIT)
    ensure_git_commit(PS2DEV, PS2DEV_REPO, PS2DEV_COMMIT)
    ensure_git_commit(PS2SDK_20040415, PS2SDK_REPO, PS2SDK_20040415_COMMIT)
    ensure_git_commit(PS2SDK_20040418, PS2SDK_REPO, PS2SDK_20040418_COMMIT)
    objects = build_objects(cc, cxx)
    progress, next_address = load_progress()
    rows = make_evidence(reference_path.read_bytes(), progress, next_address, objects)
    write_evidence(rows)
    exact = sum(row["boundary"] == EXACT for row in rows)
    terminal = len(rows) - exact
    print(f"V47 strict matches: {len(rows)} (exact={exact}, terminal={terminal})")
    print(f"evidence: {EVIDENCE.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
