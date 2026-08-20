#!/usr/bin/env python3
"""Cache, compile and strictly compare address-anchored EE source functions."""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import os
import re
import shlex
import subprocess
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass
from pathlib import Path

from compare_elf_functions import Comparison, ELFFile, compare_function


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_COMPILER = (
    ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-stage1" / "prefix" / "bin" / "ee-gcc"
)
DEFAULT_TARGET = ROOT / "build" / "SNES_EMU.unpacked.bin"
DEFAULT_OUTPUT = ROOT / "build" / "match-miner"
DEFAULT_MANIFEST = ROOT / "analysis" / "progress_targets.csv"
EXPECTED_TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
ADDRESS_RE = re.compile(r"(?<![0-9a-fA-F])(001[0-9a-fA-F]{5})(?![0-9a-fA-F])")

COMMON_FLAGS = (
    "-G0",
    "-EL",
    "-pipe",
    "-fomit-frame-pointer",
    "-ffreestanding",
    "-fno-builtin",
    "-fshort-double",
    "-mlong64",
    "-mhard-float",
    "-mno-abicalls",
    "-march=r5900",
    "-mtune=r5900",
    "-DPS2_EE",
    "-D_EE",
    "-DLSB_FIRST",
    "-DALIGN_DWORD",
    "-DCODE_PLATFORM=3",
    "-Iinclude",
    "-Iinclude/ee_stage1_compat",
    "-Imatching/ee_abi_compat",
    "-w",
)

# Keep the default narrow.  ``--full`` enables the complete deterministic
# matrix, while ``--profiles`` lets a resumed hunt select only useful rows.
PROFILES: dict[str, tuple[str, ...]] = {
    "o0": ("-O0", "-fstrict-aliasing", "-fno-common"),
    "o1": ("-O1", "-fstrict-aliasing", "-fno-common"),
    "o2": ("-O2", "-fstrict-aliasing", "-fno-common"),
    "o3": ("-O3", "-fstrict-aliasing", "-fno-common"),
    "os": ("-Os", "-fstrict-aliasing", "-fno-common"),
    "o2-nostrict": ("-O2", "-fno-strict-aliasing", "-fno-common"),
    "os-nostrict": ("-Os", "-fno-strict-aliasing", "-fno-common"),
    "o2-noinline": ("-O2", "-fstrict-aliasing", "-fno-common", "-fno-inline"),
    "os-noinline": ("-Os", "-fstrict-aliasing", "-fno-common", "-fno-inline"),
    "o2-nosched1": (
        "-O2",
        "-fstrict-aliasing",
        "-fno-common",
        "-fno-schedule-insns",
    ),
    "o2-nosched2": (
        "-O2",
        "-fstrict-aliasing",
        "-fno-common",
        "-fno-schedule-insns2",
    ),
    "o2-nodelay": (
        "-O2",
        "-fstrict-aliasing",
        "-fno-common",
        "-fno-delayed-branch",
    ),
    "o2-noreorder": (
        "-O2",
        "-fstrict-aliasing",
        "-fno-common",
        "-fno-reorder-blocks",
    ),
    "o2-noalignj": (
        "-O2",
        "-fstrict-aliasing",
        "-fno-common",
        "-fno-align-jumps",
    ),
    "o2-frame": ("-O2", "-fstrict-aliasing", "-fno-common", "-fno-omit-frame-pointer"),
    "os-noalignj": (
        "-Os",
        "-fstrict-aliasing",
        "-fno-common",
        "-fno-align-jumps",
    ),
}
DEFAULT_PROFILES = ("o2", "os", "o2-nosched1")


@dataclass(frozen=True)
class CompileResult:
    profile: str
    source: Path
    object_path: Path
    log_path: Path
    cache_key: str
    cached: bool
    returncode: int


@dataclass(frozen=True)
class Hit:
    address: int
    target_name: str
    area: str
    source: Path
    profile: str
    flags: tuple[str, ...]
    object_path: Path
    object_symbol: str
    identity_mode: str
    comparison: Comparison
    boundary_mode: str
    zero_gap: int
    object_sha256: str
    cache_key: str


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def relative(path: Path) -> str:
    try:
        return path.resolve().relative_to(ROOT).as_posix()
    except ValueError:
        return str(path.resolve())


def compiler_identity(compiler: Path) -> str:
    digest = hashlib.sha256()
    digest.update(sha256_file(compiler).encode("ascii"))
    manifest = compiler.parents[2] / "bootstrap-manifest.json"
    if manifest.is_file():
        digest.update(manifest.read_bytes())
    for option in ("--version", "-dumpmachine", "-dumpversion"):
        result = subprocess.run(
            [str(compiler), option],
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )
        if result.returncode != 0:
            raise SystemExit(f"compiler probe failed: {compiler} {option}")
        digest.update(result.stdout)
    return digest.hexdigest()


def dependency_identity() -> str:
    digest = hashlib.sha256()
    roots = (ROOT / "include", ROOT / "matching" / "ee_abi_compat")
    paths: list[Path] = []
    for directory in roots:
        if directory.is_dir():
            paths.extend(path for path in directory.rglob("*") if path.is_file())
    for path in sorted(paths):
        digest.update(relative(path).encode("utf-8"))
        digest.update(b"\0")
        digest.update(path.read_bytes())
        digest.update(b"\0")
    return digest.hexdigest()


def cache_key(
    compiler_id: str,
    dependency_id: str,
    source: Path,
    flags: tuple[str, ...],
) -> str:
    digest = hashlib.sha256()
    for value in (compiler_id, dependency_id, relative(source), "\0".join(flags)):
        digest.update(value.encode("utf-8"))
        digest.update(b"\0")
    digest.update(source.read_bytes())
    return digest.hexdigest()


def compile_source(
    compiler: Path,
    compiler_id: str,
    dependency_id: str,
    output_dir: Path,
    profile: str,
    source: Path,
) -> CompileResult:
    flags = COMMON_FLAGS + PROFILES[profile]
    key = cache_key(compiler_id, dependency_id, source, flags)
    profile_dir = output_dir / "objects" / profile
    profile_dir.mkdir(parents=True, exist_ok=True)
    stem = re.sub(r"[^A-Za-z0-9_.-]+", "_", relative(source))[-96:]
    object_path = profile_dir / f"{stem}-{key[:16]}.o"
    log_path = object_path.with_suffix(".log")
    metadata_path = object_path.with_suffix(".json")
    failure_path = object_path.with_suffix(".failure.json")
    expected_metadata = {
        "schema": 1,
        "cache_key": key,
        "compiler_identity": compiler_id,
        "dependency_identity": dependency_id,
        "profile": profile,
        "source": relative(source),
        "flags": list(flags),
    }
    if object_path.is_file() and metadata_path.is_file():
        try:
            current = json.loads(metadata_path.read_text(encoding="utf-8"))
        except (OSError, ValueError):
            current = None
        if current == expected_metadata:
            return CompileResult(profile, source, object_path, log_path, key, True, 0)
    if failure_path.is_file():
        try:
            failure = json.loads(failure_path.read_text(encoding="utf-8"))
        except (OSError, ValueError):
            failure = None
        if (
            isinstance(failure, dict)
            and failure.get("metadata") == expected_metadata
            and isinstance(failure.get("returncode"), int)
            and failure["returncode"] != 0
        ):
            return CompileResult(
                profile,
                source,
                object_path,
                log_path,
                key,
                True,
                failure["returncode"],
            )

    command = [str(compiler), *flags, "-c", str(source), "-o", str(object_path)]
    result = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )
    log_path.write_text(
        "$ " + shlex.join(command) + "\n" + result.stdout,
        encoding="utf-8",
    )
    if result.returncode == 0:
        temporary = metadata_path.with_suffix(".json.tmp")
        temporary.write_text(
            json.dumps(expected_metadata, indent=2, sort_keys=True) + "\n",
            encoding="utf-8",
        )
        os.replace(temporary, metadata_path)
        if failure_path.exists():
            failure_path.unlink()
    elif object_path.exists():
        object_path.unlink()
    if result.returncode != 0:
        temporary = failure_path.with_name(failure_path.name + ".tmp")
        temporary.write_text(
            json.dumps(
                {"metadata": expected_metadata, "returncode": result.returncode},
                indent=2,
                sort_keys=True,
            )
            + "\n",
            encoding="utf-8",
        )
        os.replace(temporary, failure_path)
    return CompileResult(
        profile, source, object_path, log_path, key, False, result.returncode
    )


def load_manifest(path: Path) -> tuple[list[dict[str, str]], dict[int, int]]:
    with path.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream))
    required = {"address", "name", "area", "status"}
    if not rows or not required <= set(rows[0]):
        raise SystemExit(f"manifest must contain {sorted(required)}: {path}")
    addresses = sorted(int(row["address"], 0) for row in rows)
    if len(addresses) != len(set(addresses)):
        raise SystemExit(f"manifest contains duplicate addresses: {path}")
    return rows, {start: end for start, end in zip(addresses, addresses[1:])}


def identity_candidates(
    symbol_name: str,
    address_by_name: dict[str, int],
    known_addresses: set[int],
) -> dict[int, str]:
    candidates: dict[int, str] = {}
    if symbol_name in address_by_name:
        candidates[address_by_name[symbol_name]] = "name-strict"
    for match in ADDRESS_RE.finditer(symbol_name):
        address = int(match.group(1), 16)
        if address in known_addresses:
            candidates.setdefault(address, "address-anchored-strict")
    return candidates


def has_terminal_control_flow(candidate: bytes, endian: str) -> bool:
    """Return true for a complete MIPS leaf return or closed self-loop tail."""
    if len(candidate) < 8 or len(candidate) % 4:
        return False
    byte_order = "little" if endian == "<" else "big"
    instruction = int.from_bytes(candidate[-8:-4], byte_order)
    if instruction == 0x03E00008:  # jr ra; final word is its delay slot
        return True
    opcode = instruction >> 26
    rs = (instruction >> 21) & 0x1F
    rt = (instruction >> 16) & 0x1F
    immediate = instruction & 0xFFFF
    if immediate & 0x8000:
        immediate -= 0x10000
    # beq zero,zero,target with a target inside the same candidate.  This
    # proves a closed tail such as the fatal-spin leaf without treating the
    # later bytes before the next sparse manifest entry as part of it.
    branch_offset = len(candidate) - 8
    branch_target = branch_offset + 4 + immediate * 4
    return opcode == 4 and rs == 0 and rt == 0 and 0 <= branch_target < len(candidate)


def rank_hit(
    hit: Hit, profile_order: dict[str, int]
) -> tuple[int, int, int, int, int, str]:
    boundary_rank = {
        "exact-next-boundary": 0,
        "target-zero-gap": 1,
        "terminal-control-flow-boundary": 2,
    }[hit.boundary_mode]
    return (
        boundary_rank,
        0 if hit.comparison.raw_equal else 1,
        0 if hit.identity_mode == "name-strict" else 1,
        len(hit.comparison.relocation_ranges),
        profile_order[hit.profile],
        relative(hit.source),
    )


def write_hits(path: Path, hits: list[Hit]) -> None:
    fields = [
        "address",
        "name",
        "area",
        "provenance",
        "source",
        "profile",
        "detail",
        "object",
        "object_symbol",
        "object_size",
        "boundary",
        "result",
        "differing_bytes",
        "raw_equal",
        "normalized_equal",
        "unknown_relocations",
        "object_sha256",
        "cache_key",
    ]
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        for hit in hits:
            result = hit.comparison
            writer.writerow(
                {
                    "address": f"0x{hit.address:08x}",
                    "name": hit.target_name,
                    "area": hit.area,
                    "provenance": "cached-address-match-miner",
                    "source": relative(hit.source),
                    "profile": hit.profile,
                    "detail": (
                        f"{hit.identity_mode}; compiler=EE-GCC-3.2.2; "
                        f"relocations={len(result.relocation_ranges)}"
                    ),
                    "object": relative(hit.object_path),
                    "object_symbol": hit.object_symbol,
                    "object_size": result.candidate_size,
                    "boundary": (
                        "exact-next-boundary"
                        if hit.boundary_mode == "exact-next-boundary"
                        else (
                            f"historical-symbol+target-zero-gap:0x{hit.zero_gap:x}"
                            if hit.boundary_mode == "target-zero-gap"
                            else "terminal-control-flow-boundary"
                        )
                    ),
                    "result": "MATCH",
                    "differing_bytes": result.differing_bytes,
                    "raw_equal": result.raw_equal,
                    "normalized_equal": result.normalized_equal,
                    "unknown_relocations": ",".join(
                        str(value) for value in result.unknown_relocation_types
                    ),
                    "object_sha256": hit.object_sha256,
                    "cache_key": hit.cache_key,
                }
            )


def write_near_misses(path: Path, rows: list[dict[str, object]]) -> None:
    fields = [
        "address",
        "name",
        "source",
        "profile",
        "object_symbol",
        "target_size",
        "object_size",
        "differing_bytes",
        "first_differences",
    ]
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)


def select_profiles(args: argparse.Namespace) -> tuple[str, ...]:
    if args.full:
        return tuple(PROFILES)
    names = tuple(part.strip() for part in args.profiles.split(",") if part.strip())
    unknown = sorted(set(names) - set(PROFILES))
    if unknown:
        raise SystemExit(f"unknown profile(s): {', '.join(unknown)}")
    if not names:
        raise SystemExit("at least one profile is required")
    return names


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", type=Path, default=DEFAULT_COMPILER)
    parser.add_argument("--target", type=Path, default=DEFAULT_TARGET)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--output-dir", type=Path, default=DEFAULT_OUTPUT)
    parser.add_argument("--report", type=Path)
    parser.add_argument("--profiles", default=",".join(DEFAULT_PROFILES))
    parser.add_argument("--full", action="store_true")
    parser.add_argument("--jobs", type=int, default=min(8, os.cpu_count() or 1))
    parser.add_argument("--include-matching", action="store_true")
    parser.add_argument("--expect-min", type=int, default=0)
    parser.add_argument(
        "--max-zero-gap",
        type=int,
        default=16,
        help="accept an otherwise exact named symbol followed only by this many target zero bytes",
    )
    parser.add_argument(
        "--max-terminal-size",
        type=int,
        default=512,
        help="maximum exact prefix size accepted with a terminal control-flow boundary proof",
    )
    args = parser.parse_args()

    compiler = args.compiler.expanduser().resolve()
    target_path = args.target.expanduser().resolve()
    manifest_path = args.manifest.expanduser().resolve()
    output_dir = args.output_dir.expanduser().resolve()
    if not compiler.is_file() or not os.access(compiler, os.X_OK):
        raise SystemExit(f"missing executable EE compiler: {compiler}")
    if not target_path.is_file():
        raise SystemExit(f"missing unpacked target: {target_path}; run make reference")
    if not 1 <= args.jobs <= 64:
        raise SystemExit("--jobs must be between 1 and 64")
    if args.max_zero_gap < 0 or args.max_zero_gap > 64:
        raise SystemExit("--max-zero-gap must be between 0 and 64")
    if args.max_terminal_size < 8 or args.max_terminal_size > 4096:
        raise SystemExit("--max-terminal-size must be between 8 and 4096")
    target_sha256 = sha256_file(target_path)
    if target_sha256 != EXPECTED_TARGET_SHA256:
        raise SystemExit(
            "refusing unexpected target image\n"
            f"expected SHA-256 {EXPECTED_TARGET_SHA256}\nactual   SHA-256 {target_sha256}"
        )

    profiles = select_profiles(args)
    sources = sorted((ROOT / "src").rglob("*.c"))
    sources += sorted((ROOT / "matching" / "candidates").glob("*.c"))
    if not sources:
        raise SystemExit("no source translation units found")
    rows, end_by_address = load_manifest(manifest_path)
    row_by_address = {int(row["address"], 0): row for row in rows}
    address_by_name = {row["name"]: int(row["address"], 0) for row in rows}
    known_addresses = set(row_by_address)
    target = target_path.read_bytes()
    compiler_id = compiler_identity(compiler)
    dependency_id = dependency_identity()

    jobs: list[tuple[str, Path]] = [
        (profile, source) for profile in profiles for source in sources
    ]
    compiled: list[CompileResult] = []
    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        futures = [
            executor.submit(
                compile_source,
                compiler,
                compiler_id,
                dependency_id,
                output_dir,
                profile,
                source,
            )
            for profile, source in jobs
        ]
        for index, future in enumerate(as_completed(futures), 1):
            compiled.append(future.result())
            if index % 100 == 0 or index == len(futures):
                print(f"compile progress: {index}/{len(futures)}", flush=True)

    failures = [result for result in compiled if result.returncode != 0]
    usable = [result for result in compiled if result.returncode == 0]
    cached = sum(result.cached for result in usable)
    print(
        f"compiled objects: {len(usable)}/{len(compiled)} "
        f"(cache hits={cached}, failures={len(failures)})"
    )
    if failures:
        failure_report = output_dir / "compile-failures.tsv"
        failure_report.parent.mkdir(parents=True, exist_ok=True)
        with failure_report.open("w", encoding="utf-8", newline="") as stream:
            writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
            writer.writerow(("profile", "source", "log"))
            for result in sorted(failures, key=lambda item: (item.profile, str(item.source))):
                writer.writerow((result.profile, relative(result.source), relative(result.log_path)))

    profile_order = {name: index for index, name in enumerate(profiles)}
    hits_by_address: dict[int, list[Hit]] = {}
    near_by_address: dict[int, tuple[tuple[int, int, int], dict[str, object]]] = {}
    object_hashes: dict[Path, str] = {}
    for result in usable:
        try:
            elf = ELFFile(result.object_path)
        except (OSError, ValueError):
            continue
        flags = COMMON_FLAGS + PROFILES[result.profile]
        for symbol in elf.symbols:
            if symbol.section_index == 0 or (symbol.info & 0xF) != 2 or symbol.size <= 0:
                continue
            candidates = identity_candidates(symbol.name, address_by_name, known_addresses)
            for address, identity_mode in candidates.items():
                row = row_by_address[address]
                if address not in end_by_address:
                    continue
                if row["status"] == "MATCHING" and not args.include_matching:
                    continue
                expected_size = end_by_address[address] - address
                try:
                    comparison = compare_function(
                        target,
                        address - 0x00100000,
                        expected_size,
                        elf,
                        symbol.name,
                    )
                except ValueError:
                    continue
                accepted_comparison = comparison
                boundary_mode = "exact-next-boundary"
                zero_gap = 0
                if not comparison.matching and symbol.size < expected_size:
                    candidate_gap = expected_size - symbol.size
                    gap_start = address - 0x00100000 + symbol.size
                    gap_end = address - 0x00100000 + expected_size
                    target_gap = target[gap_start:gap_end]
                    if (
                        candidate_gap <= args.max_zero_gap
                        and candidate_gap % 4 == 0
                        and target_gap == b"\0" * candidate_gap
                    ):
                        try:
                            prefix = compare_function(
                                target,
                                address - 0x00100000,
                                symbol.size,
                                elf,
                                symbol.name,
                            )
                        except ValueError:
                            prefix = comparison
                        if prefix.matching and not prefix.unknown_relocation_types:
                            accepted_comparison = prefix
                            boundary_mode = "target-zero-gap"
                            zero_gap = candidate_gap
                if (
                    not accepted_comparison.matching
                    and symbol.size < expected_size
                    and symbol.size <= args.max_terminal_size
                ):
                    try:
                        prefix = compare_function(
                            target,
                            address - 0x00100000,
                            symbol.size,
                            elf,
                            symbol.name,
                        )
                        candidate_bytes = elf.symbol_bytes(symbol, symbol.size)
                    except ValueError:
                        prefix = comparison
                        candidate_bytes = b""
                    if (
                        prefix.matching
                        and not prefix.unknown_relocation_types
                        and has_terminal_control_flow(candidate_bytes, elf.endian)
                    ):
                        accepted_comparison = prefix
                        boundary_mode = "terminal-control-flow-boundary"
                        zero_gap = 0
                if (
                    accepted_comparison.matching
                    and not accepted_comparison.unknown_relocation_types
                ):
                    object_sha = object_hashes.setdefault(
                        result.object_path, sha256_file(result.object_path)
                    )
                    hit = Hit(
                        address=address,
                        target_name=row["name"],
                        area=row["area"],
                        source=result.source,
                        profile=result.profile,
                        flags=flags,
                        object_path=result.object_path,
                        object_symbol=symbol.name,
                        identity_mode=identity_mode,
                        comparison=accepted_comparison,
                        boundary_mode=boundary_mode,
                        zero_gap=zero_gap,
                        object_sha256=object_sha,
                        cache_key=result.cache_key,
                    )
                    hits_by_address.setdefault(address, []).append(hit)
                    continue
                near = {
                    "address": f"0x{address:08x}",
                    "name": row["name"],
                    "source": relative(result.source),
                    "profile": result.profile,
                    "object_symbol": symbol.name,
                    "target_size": comparison.expected_size,
                    "object_size": comparison.candidate_size,
                    "differing_bytes": comparison.differing_bytes,
                    "first_differences": ",".join(
                        f"+0x{offset:x}" for offset in comparison.first_differences
                    ),
                }
                score = (
                    comparison.differing_bytes,
                    abs(comparison.expected_size - comparison.candidate_size),
                    profile_order[result.profile],
                )
                if address not in near_by_address or score < near_by_address[address][0]:
                    near_by_address[address] = (score, near)

    selected = [
        min(values, key=lambda hit: rank_hit(hit, profile_order))
        for _, values in sorted(hits_by_address.items())
    ]
    report_path = (
        args.report.expanduser().resolve()
        if args.report
        else output_dir / "strict-hits.tsv"
    )
    write_hits(report_path, selected)
    near_path = output_dir / "near-misses.tsv"
    write_near_misses(
        near_path,
        [value[1] for _, value in sorted(near_by_address.items())],
    )
    summary = {
        "schema": 1,
        "target_sha256": target_sha256,
        "compiler_identity": compiler_id,
        "profiles": list(profiles),
        "sources": len(sources),
        "compile_jobs": len(compiled),
        "compiled": len(usable),
        "compile_failures": len(failures),
        "cache_hits": cached,
        "strict_direct_matches": len(selected),
        "strict_report": relative(report_path),
        "near_miss_report": relative(near_path),
    }
    summary_path = output_dir / "summary.json"
    summary_path.write_text(json.dumps(summary, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    print(f"strict direct matches: {len(selected)}")
    print(f"strict report: {relative(report_path)}")
    print(f"near misses: {relative(near_path)}")
    if len(selected) < args.expect_min:
        raise SystemExit(
            f"strict match gate failed: expected at least {args.expect_min}, got {len(selected)}"
        )


if __name__ == "__main__":
    main()
