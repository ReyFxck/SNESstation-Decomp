#!/usr/bin/env python3
"""Rebuild and verify the four HUNT1000+ V45 historical-source matches."""
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

from bootstrap_ee_gcc_stage1 import atomic_write_text  # noqa: E402
from compare_elf_functions import ELFFile, compare_function  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
PS2DEV_REPO = "https://github.com/duduclx/PS2DEV.git"
PS2DEV_COMMIT = "bac0006c6302edcf1bdae253799484497b4e5032"
PGEN_REPO = "https://github.com/ps2homebrew/pgen.git"
PGEN_COMMIT = "403f1710e5eacb7d04e5031e1cb0a40435ff9d33"

BUILD = ROOT / "build" / "matching" / "hunt1000plus-v45-historical"
PS2DEV = ROOT / "build" / "upstream" / "PS2DEV-bac0006c"
PGEN = ROOT / "build" / "upstream" / "pgen-403f1710"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1000plus-v45-validated-historical.tsv"

COMMON_FLAGS = (
    "-G0", "-EL", "-pipe", "-w", "-fomit-frame-pointer",
    "-fstrict-aliasing", "-fno-common", "-fshort-double", "-mlong64",
    "-mhard-float", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
    "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD",
    "-DCODE_PLATFORM=3",
)
EVIDENCE_FIELDS = (
    "address", "name", "area", "provenance", "source", "profile",
    "detail", "object", "object_symbol", "object_size", "boundary",
    "result", "differing_bytes", "raw_equal", "normalized_equal",
    "unknown_relocations", "object_sha256", "cache_key",
)


@dataclass(frozen=True)
class CandidateSpec:
    address: int
    expected_name: str
    provenance: str
    source_rel: str
    profile: str
    object_name: str
    symbol: str
    object_size: int
    gap: int
    checkout: str
    optimization: str = ""
    macro: str = ""
    archive_member: str = ""


CANDIDATES = (
    CandidateSpec(
        0x00198AC0, "adler32", "pgen-403f1710",
        "zlib/adler32.c", "pgen-os", "adler32.o", "adler32", 404, 4,
        "pgen", optimization="-Os",
    ),
    CandidateSpec(
        0x0019BAD0, "gsFont_GetCurrLineLength", "pgen-libgs-a",
        "lib/gslib051/lib/libgs.a", "prebuilt-archive", "gsFont.o",
        "_ZN6gsFont17GetCurrLineLengthEPKciRiS2_", 148, 4, "pgen",
        archive_member="gsFont.o",
    ),
    CandidateSpec(
        0x0019CF10, "SifWriteBackDCache", "ps2dev-per-object",
        "ps2sdk/ee/kernel/src/kernel.S", "hist-o2", "SifWriteBackDCache.o",
        "SifWriteBackDCache", 172, 4, "ps2dev", optimization="-O2",
        macro="F_SifWriteBackDCache",
    ),
    CandidateSpec(
        0x0019F57C, "SifGetSreg", "ps2dev-per-object",
        "ps2sdk/ee/kernel/src/sifcmd.c", "hist-os", "SifGetSreg.o",
        "SifGetSreg", 24, 12, "ps2dev", optimization="-Os",
        macro="F_sif_sreg_get",
    ),
)


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT).as_posix()


def run(
    command: list[str], *, cwd: Path = ROOT, allow_failure: bool = False
) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(
        command,
        cwd=cwd,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    if result.returncode and not allow_failure:
        joined = " ".join(command)
        raise SystemExit(
            f"command failed ({result.returncode}): {joined}\n{result.stdout[-6000:]}"
        )
    return result


def ensure_git_commit(path: Path, repository: str, commit: str) -> None:
    git_dir = path / ".git"
    if not git_dir.exists():
        if path.exists() and any(path.iterdir()):
            raise SystemExit(f"refusing non-Git upstream directory: {path}")
        path.mkdir(parents=True, exist_ok=True)
        run(["git", "init", "-q", str(path)])
        run(["git", "-C", str(path), "remote", "add", "origin", repository])

    current = run(
        ["git", "-C", str(path), "rev-parse", "HEAD"], allow_failure=True
    ).stdout.strip()
    if current != commit:
        run(
            [
                "git", "-C", str(path), "fetch", "-q", "--depth=1",
                "origin", commit,
            ]
        )
        run(["git", "-C", str(path), "checkout", "-q", "--detach", "FETCH_HEAD"])
    actual = run(["git", "-C", str(path), "rev-parse", "HEAD"]).stdout.strip()
    if actual != commit:
        raise SystemExit(f"upstream checkout mismatch: wanted {commit}, got {actual}")
    dirty = run(
        [
            "git", "-C", str(path), "status", "--porcelain",
            "--untracked-files=no",
        ]
    ).stdout.strip()
    if dirty:
        raise SystemExit(f"pinned upstream checkout has tracked changes: {path}")


def include_flags(source: Path, *, pgen: bool) -> list[str]:
    historical = sorted(
        path for path in (PS2DEV / "ps2sdk").rglob("include") if path.is_dir()
    )
    includes: list[Path] = []
    if pgen:
        includes.extend(
            (
                PGEN,
                PGEN / "lib",
                PGEN / "lib" / "gslib051" / "include",
                PGEN / "unzip",
                PGEN / "zlib",
            )
        )
    else:
        for path in (
            source.parent / "include",
            source.parent.parent / "include",
            source.parent.parent.parent / "include",
        ):
            if path.is_dir() and path not in includes:
                includes.append(path)
    includes.extend(historical)
    flags: list[str] = []
    for path in includes:
        flags.extend(("-I", str(path)))
    return flags


def write_metadata(
    object_path: Path,
    source: Path,
    profile: str,
    cache_payload: dict[str, object],
) -> dict[str, str]:
    cache_key = hashlib.sha256(
        json.dumps(
            cache_payload, sort_keys=True, separators=(",", ":")
        ).encode("utf-8")
    ).hexdigest()
    metadata = {"cache_key": cache_key, "source": rel(source), "profile": profile}
    atomic_write_text(
        object_path.with_suffix(".json"),
        json.dumps(metadata, indent=2, sort_keys=True) + "\n",
    )
    return metadata


def compile_candidate(spec: CandidateSpec, compiler: Path) -> tuple[Path, dict[str, str]]:
    checkout = PGEN if spec.checkout == "pgen" else PS2DEV
    source = checkout / spec.source_rel
    if not source.is_file():
        raise SystemExit(f"missing pinned source: {source}")
    output = BUILD / spec.object_name
    command = [
        str(compiler),
        *COMMON_FLAGS,
        spec.optimization,
        *([f"-D{spec.macro}"] if spec.macro else []),
        *include_flags(source, pgen=spec.checkout == "pgen"),
        "-c", str(source), "-o", str(output),
    ]
    run(command)
    metadata = write_metadata(
        output,
        source,
        spec.profile,
        {
            "compiler_sha256": sha256_file(compiler),
            "command": command[1:-3],
            "source_sha256": sha256_file(source),
            "upstream_commit": PGEN_COMMIT if spec.checkout == "pgen" else PS2DEV_COMMIT,
        },
    )
    return output, metadata


def extract_candidate(spec: CandidateSpec, ar: Path) -> tuple[Path, dict[str, str]]:
    source = PGEN / spec.source_rel
    if not source.is_file():
        raise SystemExit(f"missing pinned archive: {source}")
    output = BUILD / spec.object_name
    if output.exists():
        output.unlink()
    run([str(ar), "x", str(source.resolve()), spec.archive_member], cwd=BUILD)
    if not output.is_file():
        raise SystemExit(f"archive did not contain {spec.archive_member}")
    metadata = write_metadata(
        output,
        source,
        spec.profile,
        {
            "archive_sha256": sha256_file(source),
            "member": spec.archive_member,
            "upstream_commit": PGEN_COMMIT,
        },
    )
    return output, metadata


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
    built: dict[int, tuple[Path, dict[str, str]]],
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
            raise SystemExit(f"0x{spec.address:08x}: unexpected status")
        if end - spec.address != spec.object_size + spec.gap:
            raise SystemExit(f"0x{spec.address:08x}: target boundary changed")

        object_path, metadata = built[spec.address]
        elf = ELFFile(object_path)
        symbol = elf.find_symbol(spec.symbol)
        if symbol.size != spec.object_size:
            raise SystemExit(
                f"0x{spec.address:08x}: expected object size {spec.object_size}, "
                f"got {symbol.size}"
            )
        comparison = compare_function(
            reference,
            spec.address - TARGET_BASE,
            symbol.size,
            elf,
            spec.symbol,
        )
        if not comparison.matching or comparison.differing_bytes:
            raise SystemExit(f"0x{spec.address:08x}: strict object comparison failed")
        if comparison.unknown_relocation_types:
            raise SystemExit(f"0x{spec.address:08x}: unknown relocations")
        gap_start = spec.address - TARGET_BASE + symbol.size
        if reference[gap_start : gap_start + spec.gap] != b"\0" * spec.gap:
            raise SystemExit(f"0x{spec.address:08x}: target gap is not zero padding")

        commit = PGEN_COMMIT if spec.checkout == "pgen" else PS2DEV_COMMIT
        rows.append(
            {
                "address": f"0x{spec.address:08x}",
                "name": manifest["name"],
                "area": manifest["area"],
                "provenance": spec.provenance,
                "source": metadata["source"],
                "profile": spec.profile,
                "detail": (
                    f"historical-symbol-strict; commit={commit}; "
                    f"historical-symbol-size={symbol.size}; "
                    f"relocations={len(comparison.relocation_ranges)}"
                ),
                "object": rel(object_path),
                "object_symbol": spec.symbol,
                "object_size": str(symbol.size),
                "boundary": f"historical-symbol+target-zero-gap:0x{spec.gap:x}",
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(comparison.raw_equal),
                "normalized_equal": "True",
                "unknown_relocations": "",
                "object_sha256": sha256_file(object_path),
                "cache_key": metadata["cache_key"],
            }
        )
    return rows


def write_evidence(rows: list[dict[str, str]]) -> None:
    if len(rows) != 4 or {row["address"] for row in rows} != {
        f"0x{spec.address:08x}" for spec in CANDIDATES
    }:
        raise SystemExit("historical evidence cardinality/identity gate failed")
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
        "--compiler",
        type=Path,
        default=ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-stage1" / "prefix" / "bin" / "ee-gcc",
    )
    parser.add_argument("--ar", type=Path)
    args = parser.parse_args()

    compiler = args.compiler.expanduser().resolve()
    ar = (
        args.ar.expanduser().resolve()
        if args.ar
        else compiler.with_name("ee-ar")
    )
    reference_path = ROOT / "build" / "SNES_EMU.unpacked.bin"
    for path, label in ((compiler, "compiler"), (ar, "archiver"), (reference_path, "reference")):
        if not path.is_file():
            raise SystemExit(f"missing {label}: {path}")
    if sha256_file(reference_path) != TARGET_SHA256:
        raise SystemExit("unpacked target SHA-256 mismatch")
    run(
        [
            sys.executable,
            str(ROOT / "tools" / "probe_ee_toolchain.py"),
            "--compiler", str(compiler),
        ]
    )

    BUILD.mkdir(parents=True, exist_ok=True)
    ensure_git_commit(PS2DEV, PS2DEV_REPO, PS2DEV_COMMIT)
    ensure_git_commit(PGEN, PGEN_REPO, PGEN_COMMIT)

    built: dict[int, tuple[Path, dict[str, str]]] = {}
    for spec in CANDIDATES:
        built[spec.address] = (
            extract_candidate(spec, ar)
            if spec.archive_member
            else compile_candidate(spec, compiler)
        )

    progress, next_address = load_progress()
    rows = make_evidence(
        reference_path.read_bytes(), progress, next_address, built
    )
    write_evidence(rows)
    print(f"historical strict matches: {len(rows)}")
    print(f"evidence: {EVIDENCE.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
