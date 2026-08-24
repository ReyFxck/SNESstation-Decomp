#!/usr/bin/env python3
"""Rebuild and verify the 40 HUNT1000+ V46 closure matches."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
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
SNES_ARCHIVE = Archive(
    name="snes9x-1.41-1-src.tar.gz",
    url="https://www.lysator.liu.se/snes9x/1.41-1/snes9x-1.41-1-src.tar.gz",
    sha256="5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b",
    source_directory="snes9x-1.41-1-src",
    historical_patch="",
)

BUILD = ROOT / "build" / "matching" / "hunt1000plus-v46-closure"
UPSTREAM = ROOT / "build" / "upstream"
SNES_CACHE = UPSTREAM / "hunt1000plus-v46-snes"
PGEN = UPSTREAM / "pgen-403f1710"
PS2DEV = UPSTREAM / "PS2DEV-bac0006c"
EVIDENCE = (
    ROOT / "analysis" / "matching" / "hunt1000plus-v46-validated-42.tsv"
)
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
EVIDENCE_FIELDS = (
    "address", "name", "area", "provenance", "source", "profile",
    "detail", "object", "object_symbol", "object_size", "boundary",
    "result", "differing_bytes", "raw_equal", "normalized_equal",
    "unknown_relocations", "object_sha256", "cache_key",
)


@dataclass(frozen=True)
class Candidate:
    address: int
    expected_name: str
    object_key: str
    symbol: str
    size: int
    boundary: str
    provenance: str
    detail: str


EXACT = "exact-next-boundary"
TERMINAL = "terminal-control-flow-boundary"

CANDIDATES = (
    Candidate(0x001056C0, "snes_dispatch_001056c0", "ps2:libgs", "GsDrawSync", 32, TERMINAL, "ps2dev-bac0006c", "anonymous-unique-strict-fingerprint; demangled=GsDrawSync; identity_candidates=1"),
    Candidate(0x0010773C, "snes_p11_0010773c", "pgen:sjpcm", "SjPCM_Setvol", 100, TERMINAL, "pgen-403f1710", "anchored-object-layout; demangled=SjPCM_Setvol; corridor=sjpcm-rpc"),
    Candidate(0x001077F8, "SjPCM_Quit_001077f8", "pgen:sjpcm", "SjPCM_Init", 256, EXACT, "pgen-403f1710", "anchored-object-layout; demangled=SjPCM_Init; corridor=sjpcm-rpc"),
    Candidate(0x001078F8, "SjPCM_Setvol_001078f8", "pgen:sjpcm", "SjPCM_Enqueue", 332, TERMINAL, "pgen-403f1710", "anchored-object-layout; demangled=SjPCM_Enqueue; corridor=sjpcm-rpc"),
    Candidate(0x00107B1C, "amigaModInit_00107b1c", "pgen:sjpcm", "SjPCM_Quit", 96, EXACT, "pgen-403f1710", "anchored-object-layout; demangled=SjPCM_Quit; corridor=sjpcm-rpc"),
    Candidate(0x00107DB4, "amigaModSetVolume_00107db4", "pgen:amiga", "amigaModPause", 96, EXACT, "pgen-403f1710", "anchored-object-layout; demangled=amigaModPause; corridor=amigamod-rpc"),
    Candidate(0x00107E14, "amigaModGetPosition_00107e14", "pgen:amiga", "amigaModSetVolume", 108, TERMINAL, "pgen-403f1710", "anchored-object-layout; demangled=amigaModSetVolume; corridor=amigamod-rpc"),
    Candidate(0x00107F18, "amigaModQuit_00107f18", "pgen:amiga", "amigaModQuit", 108, TERMINAL, "pgen-403f1710", "historical-symbol-strict; demangled=amigaModQuit"),
    Candidate(0x00108BFC, "snes_p11_00108bfc", "ps2:mtap", "mtapPortOpen", 116, TERMINAL, "ps2dev-bac0006c", "anchored-object-layout; demangled=mtapPortOpen; predecessor=mtapInit@0x00108a9c"),
    Candidate(0x00108CE4, "snes_p11_00108ce4", "ps2:mtap", "mtapGetConnection", 116, TERMINAL, "ps2dev-bac0006c", "anchored-object-layout; demangled=mtapGetConnection; omitted-middle=mtapPortClose"),
    Candidate(0x0010A264, "snes_p16_0010a264", "snes:2XSAI", "_Z9Bilinear4jjjjjj", 372, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=Bilinear4; identity_candidates=1"),
    Candidate(0x00114118, "snes_p11_00114118", "snes:cheats2", "_Z11S9xAddCheathhjh", 180, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xAddCheat; identity_candidates=1"),
    Candidate(0x00115B58, "snes_p16_00115b58", "snes:CPU", "S9xReset", 312, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xReset; identity_candidates=1"),
    Candidate(0x001161A8, "snes_p16_001161a8", "snes:CPUEXEC", "S9xDoHBlankProcessing", 1432, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xDoHBlankProcessing; identity_candidates=1"),
    Candidate(0x00127B78, "snes_p16_00127b78", "snes:CPUOPS", "_Z13S9xOpcode_IRQv", 648, EXACT, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xOpcode_IRQ; identity_candidates=1"),
    Candidate(0x00127E00, "snes_p16_00127e00", "snes:CPUOPS", "_Z13S9xOpcode_NMIv", 648, TERMINAL, "snes9x-1.41-1-official", "anchored-object-layout; demangled=S9xOpcode_NMI; predecessor=S9xOpcode_IRQ@0x00127b78"),
    Candidate(0x0012DEB0, "snes_leaf_0012deb0", "snes:DSP1", "_Z7DSPOp1Bv", 80, TERMINAL, "snes9x-1.41-1-official", "anchored-object-layout; demangled=DSPOp1B; predecessor=DSPOp0B@0x0012de60"),
    Candidate(0x0012E728, "snes_dispatch_0012e728", "snes:DSP1", "S9xSetDSP", 40, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xSetDSP; identity_candidates=1"),
    Candidate(0x001309C4, "snes_p16_001309c4", "snes:fxemu", "_Z9FxEmulatej", 180, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=FxEmulate; identity_candidates=1"),
    Candidate(0x00130C78, "snes_p11_00130c78", "snes:fxinst", "_Z8fx_cachev", 144, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=fx_cache; identity_candidates=1"),
    Candidate(0x0014080C, "snes_dispatch_0014080c", "snes:fxinst", "_Z9fx_dec_r7v", 68, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=fx_dec_r7; identity_candidates=1"),
    Candidate(0x00153608, "CMemory_map_WriteProtectROM_00153608", "snes:MEMMAP", "_ZN7CMemory15WriteProtectROMEv", 108, EXACT, "snes9x-1.41-1-official", "historical-symbol-strict; demangled=CMemory::WriteProtectROM"),
    Candidate(0x001568C4, "snes_leaf_001568c4", "snes:MEMMAP", "_ZN7CMemory10TVStandardEv", 36, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=CMemory::TVStandard; identity_candidates=1"),
    Candidate(0x00156C0C, "CMemory_MapMode_00156c0c", "snes:MEMMAP", "_ZN7CMemory7MapModeEv", 72, TERMINAL, "snes9x-1.41-1-official", "historical-symbol-strict; demangled=CMemory::MapMode"),
    Candidate(0x00159268, "snes_p16_00159268", "snes:ppu", "S9xSetPPU", 5000, EXACT, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xSetPPU; identity_candidates=1"),
    Candidate(0x0015F15C, "snes_p11_0015f15c", "snes:CPUOPS", "_Z12S9xFixCyclesv", 108, TERMINAL, "snes9x-1.41-1-official", "anonymous-strict-fingerprint; demangled=S9xFixCycles; identity_candidates=3; instance=cpuops"),
    Candidate(0x0016DED4, "snes_p16_0016ded4", "snes:SA1CPU", "_Z16S9xSA1Opcode_IRQv", 296, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xSA1Opcode_IRQ; identity_candidates=1"),
    Candidate(0x0016FA7C, "snes_dispatch_0016fa7c", "snes:SDD1", "_Z20S9xSDD1PostLoadStatev", 72, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xSDD1PostLoadState; identity_candidates=1"),
    Candidate(0x0016FC6C, "snes_dispatch_0016fc6c", "snes:seta", "S9xSetSetaDSP", 36, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xSetSetaDSP; identity_candidates=1"),
    Candidate(0x0016FD08, "snes_p13_rotate_pair_0016fd08", "snes:seta010", "_Z12St010_RotatesssRsS_", 248, TERMINAL, "snes9x-1.41-1-official", "historical-symbol-strict; demangled=St010_Rotate"),
    Candidate(0x00171160, "snes_p11_00171160", "snes:snaporig", "_Z12S9xFixCyclesv", 108, TERMINAL, "snes9x-1.41-1-official", "anonymous-strict-fingerprint; demangled=S9xFixCycles; identity_candidates=3; instance=snaporig"),
    Candidate(0x001711E8, "snes_p11_001711e8", "snes:SNAPSHOT", "S9xFreezeGame", 72, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xFreezeGame; identity_candidates=1"),
    Candidate(0x00173C24, "snes_p11_00173c24", "snes:SNAPSHOT", "_Z12S9xFixCyclesv", 108, TERMINAL, "snes9x-1.41-1-official", "anonymous-strict-fingerprint; demangled=S9xFixCycles; identity_candidates=3; instance=snapshot"),
    Candidate(0x00177DB0, "snes_p16_00177db0", "snes:SOUNDUX", "_Z15S9xSetSoundModeii", 172, TERMINAL, "snes9x-1.41-1-official", "anonymous-unique-strict-fingerprint; demangled=S9xSetSoundMode; identity_candidates=1"),
    Candidate(0x001825E4, "snes_p13_days_in_month_001825e4", "snes:spc7110", "_Z17S9xRTCDaysInMonthii", 84, EXACT, "snes9x-1.41-1-official", "anchored-object-layout; demangled=S9xRTCDaysInMonth; predecessor=S9xGetSPC7110Byte@0x0018255c"),
    Candidate(0x00183778, "snes_p13_days_in_month_00183778", "snes:srtc", "_Z19S9xSRTCDaysInMmonthii", 84, EXACT, "snes9x-1.41-1-official", "anchored-object-layout; demangled=S9xSRTCDaysInMmonth; predecessor=S9xSRTCComputeDayOfWeek@0x001836d0"),
    Candidate(0x0019BB68, "gsFont_PrintLine", "pgen:gsfont", "_ZN6gsFont9PrintLineEiiimiPKc", 460, "historical-symbol+target-zero-gap:0x4", "pgen-libgs-a", "historical-symbol-strict; demangled=gsFont::PrintLine"),
    Candidate(0x001A6240, "fde_get_cie_encoding", "gcc:fde", "get_cie_encoding", 224, TERMINAL, "gcc-3.2.2-stage1-libgcc", "historical-symbol-strict; demangled=get_cie_encoding"),
    Candidate(0x001AB440, "iSyncDCache", "ps2:sync", "_SyncDCache", 164, "historical-symbol+target-zero-gap:0x4", "ps2dev-bac0006c", "historical-symbol-strict; demangled=_SyncDCache"),
    Candidate(0x001AC190, "S9xSetWord", "snes:MEMMAP", "_Z10S9xSetWordtj", 1140, EXACT, "snes9x-1.41-1-official", "name-strict; demangled=S9xSetWord"),
    Candidate(0x001AC994, "S9xAPUGetByte", "snes:SPC700", "_Z14S9xAPUGetByteZh", 188, EXACT, "snes9x-1.41-1-official", "historical-symbol-strict; demangled=S9xAPUGetByteZ"),
    Candidate(0x001ACB3C, "S9xAPUGetByteZ", "snes:SPC700", "_Z13S9xAPUGetBytej", 180, EXACT, "snes9x-1.41-1-official", "historical-symbol-strict; demangled=S9xAPUGetByte"),
)

SNES_SOURCES = {
    "2XSAI": "2XSAI.CPP", "CPU": "CPU.CPP", "CPUEXEC": "CPUEXEC.CPP",
    "CPUOPS": "CPUOPS.CPP",
    "DSP1": "DSP1.CPP", "MEMMAP": "MEMMAP.CPP",
    "SA1CPU": "SA1CPU.CPP", "SDD1": "SDD1.CPP",
    "SNAPSHOT": "SNAPSHOT.CPP", "SOUNDUX": "SOUNDUX.CPP",
    "SPC700": "SPC700.CPP", "cheats2": "cheats2.cpp",
    "fxemu": "fxemu.cpp", "fxinst": "fxinst.cpp", "ppu": "ppu.cpp",
    "seta": "seta.cpp", "seta010": "seta010.cpp",
    "snaporig": "snaporig.cpp", "spc7110": "spc7110.cpp",
    "srtc": "srtc.cpp",
}


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT).as_posix()


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


def metadata(
    object_path: Path,
    source: Path,
    profile: str,
    cache_payload: dict[str, object],
) -> dict[str, str]:
    cache_key = hashlib.sha256(
        json.dumps(cache_payload, sort_keys=True, separators=(",", ":")).encode()
    ).hexdigest()
    result = {"cache_key": cache_key, "source": rel(source), "profile": profile}
    atomic_write_text(
        object_path.with_suffix(".json"),
        json.dumps(result, indent=2, sort_keys=True) + "\n",
    )
    return result


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
) -> tuple[Path, dict[str, str]]:
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [str(compiler), *flags, "-c", str(source), "-o", str(output)]
    run(command)
    return output, metadata(
        output,
        source,
        profile,
        {
            "compiler_sha256": sha256_file(compiler),
            "flags": flags,
            "source_sha256": sha256_file(source),
            "upstream": upstream_identity,
        },
    )


def extract_one(
    ar: Path,
    archive: Path,
    member: str,
    output: Path,
    source: Path,
    profile: str,
    upstream_identity: str,
) -> tuple[Path, dict[str, str]]:
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists():
        output.unlink()
    run([str(ar), "x", str(archive.resolve()), member], cwd=output.parent)
    extracted = output.parent / member
    if not extracted.is_file():
        raise SystemExit(f"archive did not contain {member}: {archive}")
    if extracted != output:
        os.replace(extracted, output)
    return output, metadata(
        output,
        source,
        profile,
        {
            "archive_sha256": sha256_file(archive),
            "member": member,
            "source_sha256": sha256_file(source),
            "upstream": upstream_identity,
        },
    )


def build_objects(
    cc: Path,
    cxx: Path,
    ar: Path,
    libgcc: Path,
) -> dict[str, tuple[Path, dict[str, str]]]:
    try:
        archive = download_archive(SNES_ARCHIVE, SNES_CACHE)
        snes_root = safe_extract_archive(
            archive, SNES_CACHE / "source", SNES_ARCHIVE.source_directory
        )
    except BuildFailure as exc:
        raise SystemExit(str(exc)) from exc
    snes = snes_root / "snes9x"
    newlib = (
        PS2DEV / "ps2toolchain" / "soft" / "newlib-1.10.0"
        / "newlib" / "libc" / "include"
    )
    for path, label in ((snes, "Snes9x source"), (newlib, "Newlib headers")):
        if not path.is_dir():
            raise SystemExit(f"missing {label}: {path}")

    compat = BUILD / "compat"
    atomic_write_text(
        compat / "memory.h",
        "#ifndef HUNT1000PLUS_V46_MEMORY_H\n"
        "#define HUNT1000PLUS_V46_MEMORY_H\n"
        "#include <string.h>\n"
        "#endif\n",
    )
    snes_flags = [
        *COMMON_FLAGS, "-Os", *SNES_DEFINES,
        *include_args([compat, newlib, snes, snes / "unzip", snes_root / "zlib"]),
        "-x", "c++",
    ]
    objects: dict[str, tuple[Path, dict[str, str]]] = {}
    for key, name in SNES_SOURCES.items():
        source = snes / name
        objects[f"snes:{key}"] = compile_one(
            cxx,
            source,
            BUILD / "snes" / f"{key}.o",
            "snes9x-1.41-1-os-short",
            snes_flags,
            f"lysator:{SNES_ARCHIVE.sha256}",
        )

    ps2_includes = sorted(
        path for path in (PS2DEV / "ps2sdk").rglob("include") if path.is_dir()
    )
    pgen_includes = [
        PGEN, PGEN / "lib", PGEN / "lib" / "gslib051" / "include",
        PGEN / "unzip", PGEN / "zlib", *ps2_includes,
    ]
    pgen_flags = [*COMMON_FLAGS, "-Os", *PS2_DEFINES, *include_args(pgen_includes)]
    for key, name in (("sjpcm", "sjpcm_rpc.c"), ("amiga", "amigamod_rpc.c")):
        objects[f"pgen:{key}"] = compile_one(
            cc,
            PGEN / "lib" / name,
            BUILD / "pgen" / f"{key}.o",
            "pgen-os",
            pgen_flags,
            PGEN_COMMIT,
        )

    ps2_flags = [*COMMON_FLAGS, *PS2_DEFINES, *include_args(ps2_includes)]
    objects["ps2:libgs"] = compile_one(
        cc,
        PS2DEV / "ps2sdk" / "ee" / "libgs" / "src" / "libgs.c",
        BUILD / "ps2dev" / "libgs.o",
        "hist-tu-o2",
        [*ps2_flags, "-O2"],
        PS2DEV_COMMIT,
    )
    objects["ps2:mtap"] = compile_one(
        cc,
        PS2DEV / "ps2sdk" / "ee" / "rpc" / "multitap" / "src" / "libmtap.c",
        BUILD / "ps2dev" / "libmtap.o",
        "hist-tu-os",
        [*ps2_flags, "-Os"],
        PS2DEV_COMMIT,
    )
    objects["ps2:sync"] = compile_one(
        cc,
        PS2DEV / "ps2sdk" / "ee" / "kernel" / "src" / "kernel.S",
        BUILD / "ps2dev" / "SyncDCache.o",
        "hist-o2",
        [*ps2_flags, "-O2", "-DF__SyncDCache"],
        PS2DEV_COMMIT,
    )

    gs_archive = PGEN / "lib" / "gslib051" / "lib" / "libgs.a"
    objects["pgen:gsfont"] = extract_one(
        ar, gs_archive, "gsFont.o", BUILD / "pgen" / "gsFont.o",
        gs_archive, "prebuilt-archive", PGEN_COMMIT,
    )
    gcc_source = cc.parents[2] / "source" / "gcc-3.2.2" / "gcc" / "unwind-dw2-fde.c"
    if not gcc_source.is_file():
        raise SystemExit(f"missing GCC source for libgcc evidence: {gcc_source}")
    objects["gcc:fde"] = extract_one(
        ar, libgcc, "unwind-dw2-fde.o", BUILD / "gcc" / "unwind-dw2-fde.o",
        gcc_source, "prebuilt-libgcc-archive", "gcc-3.2.2-stage1",
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
    for spec in CANDIDATES:
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
        if spec.boundary.startswith("historical-symbol+target-zero-gap:"):
            gap = int(spec.boundary.rsplit(":", 1)[1], 0)
            if symbol.size + gap != span:
                raise SystemExit(f"0x{spec.address:08x}: zero-gap boundary changed")
            start = spec.address - TARGET_BASE + symbol.size
            if reference[start : start + gap] != b"\0" * gap:
                raise SystemExit(f"0x{spec.address:08x}: target padding is not zero")

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
    if len(CANDIDATES) != 42 or len(rows) != 42:
        raise SystemExit("V46 evidence cardinality gate failed")
    if {row["address"] for row in rows} != expected:
        raise SystemExit("V46 evidence address gate failed")
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
    parser.add_argument("--ar", type=Path)
    parser.add_argument("--libgcc", type=Path)
    args = parser.parse_args()

    cc = args.cc.expanduser().resolve()
    cxx = args.cxx.expanduser().resolve()
    ar = args.ar.expanduser().resolve() if args.ar else cc.with_name("ee-ar")
    work = cc.parents[2]
    libgcc = (
        args.libgcc.expanduser().resolve()
        if args.libgcc
        else work / "build" / "gcc-ee-stage1" / "gcc" / "libgcc.a"
    )
    reference_path = ROOT / "build" / "SNES_EMU.unpacked.bin"
    for path, label in (
        (cc, "C compiler"), (cxx, "C++ compiler"), (ar, "archiver"),
        (libgcc, "libgcc archive"), (reference_path, "reference"),
    ):
        if not path.is_file():
            raise SystemExit(f"missing {label}: {path}")
    if sha256_file(reference_path) != TARGET_SHA256:
        raise SystemExit("unpacked target SHA-256 mismatch")
    run([sys.executable, str(ROOT / "tools" / "probe_ee_toolchain.py"), "--compiler", str(cc)])
    run([sys.executable, str(ROOT / "tools" / "probe_ee_toolchain.py"), "--compiler", str(cxx)])

    ensure_git_commit(PGEN, PGEN_REPO, PGEN_COMMIT)
    ensure_git_commit(PS2DEV, PS2DEV_REPO, PS2DEV_COMMIT)
    objects = build_objects(cc, cxx, ar, libgcc)
    progress, next_address = load_progress()
    rows = make_evidence(
        reference_path.read_bytes(), progress, next_address, objects
    )
    write_evidence(rows)
    exact = sum(row["boundary"] == EXACT for row in rows)
    terminal = sum(row["boundary"] == TERMINAL for row in rows)
    zero_gap = len(rows) - exact - terminal
    print(
        f"V46 strict matches: {len(rows)} "
        f"(exact={exact}, terminal={terminal}, zero-gap={zero_gap})"
    )
    print(f"evidence: {EVIDENCE.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
