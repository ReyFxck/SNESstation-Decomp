#!/usr/bin/env python3
"""Rebuild and verify the five strict C4 matches promoted in V75.

The runner starts from the hash-pinned official Snes9x 1.41-1 source, applies
the target-proven SNES Station C4 float/math adaptation, compiles with the
historical EE GCC 3.2.2 C++ frontend, and compares against the private unpacked
ELF. Relocations are masked by their precise MIPS field masks; no other target
bytes are ignored. The exact unlisted C4Op15 body between two audited manifest
rows is recorded separately as an auxiliary boundary companion.
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
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))

from compare_elf_functions import ELFFile, compare_function  # noqa: E402
import hunt1000plus_v47_closure as v47  # noqa: E402
import hunt1041_v51_closure as v51  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
BUILD = ROOT / "build" / "matching" / "hunt1041-v75-c4"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v75-validated-c4-5.tsv"
COMPANION_EVIDENCE = (
    ROOT / "analysis" / "matching" / "hunt1041-v75-c4-companion-1.tsv"
)
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"
PROVENANCE = "snes9x-1.41-1-official-plus-snesstation-c4-float-math"
PATCHED_SOURCE_SHA256 = (
    "d33244713aa55d3f01a2a3a9a86db09228849908163f361bbfa37eefd4901e13"
)

EVIDENCE_FIELDS = (
    "address",
    "end_address",
    "manifest_next",
    "name",
    "historical_identity",
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
    "relocation_count",
    "object_sha256",
    "cache_key",
    "promotion_scope",
    "target_gate",
    "target_span_sha256",
)


@dataclass(frozen=True)
class Candidate:
    address: int
    end_address: int
    expected_name: str
    historical_identity: str
    area: str
    object_key: str
    symbol: str
    boundary: str
    detail: str
    formal_manifest: bool = True

    @property
    def size(self) -> int:
        return self.end_address - self.address


CANDIDATES = (
    Candidate(
        0x0010B8A4,
        0x0010BBCC,
        "snes_p17_0010b8a4",
        "C4TransfWireFrame",
        "frontend-core",
        "c4",
        "C4TransfWireFrame",
        "exact-next-boundary",
        "historical-symbol-strict; C4 globals=float; math=cosf/sinf; "
        "double-abi=normal; builtin-folding=disabled",
    ),
    Candidate(
        0x0010BBCC,
        0x0010BECC,
        "snes_p17_0010bbcc",
        "C4TransfWireFrame2",
        "frontend-core",
        "c4",
        "C4TransfWireFrame2",
        "exact-next-boundary",
        "historical-symbol-strict; C4 globals=float; math=cosf/sinf; "
        "double-abi=normal; builtin-folding=disabled",
    ),
    Candidate(
        0x0010BECC,
        0x0010C094,
        "snes_p17_0010becc",
        "C4CalcWireFrame",
        "frontend-core",
        "c4",
        "C4CalcWireFrame",
        "exact-next-boundary",
        "historical-symbol-strict; integer abs adapted to external fabsf; "
        "builtin-folding=disabled",
    ),
    Candidate(
        0x0010C094,
        0x0010C174,
        "snes_p16_0010c094",
        "C4Op1F",
        "frontend-core",
        "c4",
        "C4Op1F",
        "exact-object-symbol-boundary+exact-companion",
        "historical-symbol-strict; angle math=atanf; following exact "
        "C4Op15 companion pins the full gap",
    ),
    Candidate(
        0x0010C174,
        0x0010C1F8,
        "C4Op15",
        "C4Op15",
        "frontend-core",
        "c4",
        "C4Op15",
        "exact-gap-companion",
        "auxiliary historical symbol absent from the audited 1041-row "
        "manifest; float sqrt implementation pins the C4Op1F gap",
        False,
    ),
    Candidate(
        0x0010C1F8,
        0x0010C300,
        "snes_p16_0010c1f8",
        "C4Op0D",
        "frontend-core",
        "c4",
        "C4Op0D",
        "exact-next-boundary",
        "historical-symbol-strict; corrected identity=C4Op0D; "
        "math=__builtin_sqrtf; C4 globals=float",
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


def patch_c4(source: Path) -> None:
    text = source.read_text(encoding="latin-1")
    replacements = (
        ("static double tanval;", "static float tanval;", 1),
        ("static double c4x, c4y, c4z;", "static float c4x, c4y, c4z;", 1),
        ("static double c4x2, c4y2, c4z2;", "static float c4x2, c4y2, c4z2;", 1),
        ("(double)", "(float)", 23),
        ("cos (", "cosf (", 12),
        ("sin (", "sinf (", 12),
        ("abs (", "fabsf (", 5),
        ("abs(", "fabsf(", 1),
        ("atan (", "atanf (", 1),
        ("sqrt (", "__builtin_sqrtf (", 2),
    )
    for marker, replacement, expected in replacements:
        if text.count(marker) != expected:
            raise SystemExit(
                f"Snes9x 1.41-1 C4 patch context changed for {marker!r}"
            )
        text = text.replace(marker, replacement)
    v47.atomic_write_text(source, text)
    actual = v51.sha256_file(source)
    if actual != PATCHED_SOURCE_SHA256:
        raise SystemExit(f"patched C4 source SHA-256 mismatch: {actual}")


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
    original = source_root / "snes9x"
    newlib = (
        v47.PS2DEV / "ps2toolchain" / "soft" / "newlib-1.10.0"
        / "newlib" / "libc" / "include"
    )
    for path, label in ((original, "Snes9x source"), (newlib, "Newlib headers")):
        if not path.is_dir():
            raise SystemExit(f"missing {label}: {path}")
    layout = BUILD / "historical" / "snes9x-target-layout"
    if layout.exists():
        shutil.rmtree(layout)
    shutil.copytree(original, layout)
    patch_c4(layout / "c4.cpp")
    return source_root, original, layout, newlib


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
    v47.run([str(compiler), *flags, "-c", rel(source), "-o", rel(output)], cwd=ROOT)
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
    memory = compat / "memory.h"
    v47.atomic_write_text(
        memory,
        "#ifndef HUNT1041_V75_MEMORY_H\n#define HUNT1041_V75_MEMORY_H\n"
        "#include <string.h>\n#endif\n",
    )
    common_flags = [flag for flag in v47.COMMON_FLAGS if flag != "-fshort-double"]
    flags = [
        *common_flags,
        "-Os",
        "-fno-builtin",
        *v47.SNES_DEFINES,
        *include_args((compat, newlib, layout, layout / "unzip", source_root / "zlib")),
        "-x",
        "c++",
    ]
    shared = (
        layout / "c4.h",
        layout / "snes9x.h",
        layout / "MEMMAP.H",
        memory,
    )
    builds = ((
        "c4",
        "c4.cpp",
        "snes9x-1.41-1-os-normal-double-ps2-c4-float-math-fno-builtin",
    ),)
    result: dict[str, v51.ObjectBuild] = {}
    for key, filename, profile in builds:
        result[key] = compile_one(
            cxx,
            layout / filename,
            BUILD / "objects" / f"{key}.o",
            profile,
            flags,
            original / filename,
            shared,
        )
    return result


def read_manifest() -> tuple[list[dict[str, str]], dict[int, dict[str, str]], dict[int, int]]:
    _fields, rows = v51.read_csv(TARGETS)
    by_address = {int(row["address"], 0): row for row in rows}
    starts = sorted(by_address)
    next_address = {left: right for left, right in zip(starts, starts[1:])}
    return rows, by_address, next_address


def make_evidence(
    target: bytes, objects: dict[str, v51.ObjectBuild]
) -> tuple[list[dict[str, str]], list[dict[str, str]]]:
    _rows, manifest, next_address = read_manifest()
    elf_cache: dict[Path, ELFFile] = {}
    evidence: list[dict[str, str]] = []
    for candidate in CANDIDATES:
        address = f"0x{candidate.address:08x}"
        target_row = manifest.get(candidate.address)
        if candidate.formal_manifest:
            manifest_next = next_address.get(candidate.address)
            if target_row is None or manifest_next is None:
                raise SystemExit(f"{address}: missing audited target boundary")
            if (
                target_row["name"] != candidate.expected_name
                or target_row["area"] != candidate.area
            ):
                raise SystemExit(f"{address}: manifest identity changed")
            if target_row["status"] not in {"RECONSTRUCTED", "MATCHING"}:
                raise SystemExit(f"{address}: unexpected manifest status")
        else:
            if target_row is not None:
                raise SystemExit(f"{address}: auxiliary companion entered manifest")
            manifest_next = candidate.end_address
            if manifest_next not in manifest:
                raise SystemExit(f"{address}: companion does not end at manifest row")

        if candidate.boundary == "exact-next-boundary":
            if manifest_next != candidate.end_address:
                raise SystemExit(f"{address}: exact boundary changed")
        elif candidate.boundary == "exact-object-symbol-boundary+exact-companion":
            if candidate.end_address >= manifest_next:
                raise SystemExit(f"{address}: object companion boundary changed")
        elif candidate.boundary == "exact-gap-companion":
            if candidate.formal_manifest or manifest_next != candidate.end_address:
                raise SystemExit(f"{address}: auxiliary companion boundary changed")
        else:
            raise SystemExit(f"{address}: unsupported boundary mode")

        built = objects[candidate.object_key]
        elf = elf_cache.setdefault(built.path, ELFFile(built.path))
        symbol = elf.find_symbol(candidate.symbol)
        if symbol.size != candidate.size:
            raise SystemExit(
                f"{address}: object size changed from {candidate.size} to {symbol.size}"
            )
        comparison = compare_function(
            target,
            candidate.address - TARGET_BASE,
            candidate.size,
            elf,
            candidate.symbol,
            4,
        )
        if not comparison.matching or comparison.differing_bytes:
            first = ",".join(f"+0x{x:x}" for x in comparison.first_differences)
            raise SystemExit(
                f"{address}: strict comparison failed "
                f"({comparison.differing_bytes}; {first or 'size'})"
            )
        if comparison.unknown_relocation_types:
            raise SystemExit(
                f"{address}: unknown relocation types "
                f"{comparison.unknown_relocation_types}"
            )
        span = target[
            candidate.address - TARGET_BASE : candidate.end_address - TARGET_BASE
        ]
        evidence.append(
            {
                "address": address,
                "end_address": f"0x{candidate.end_address:08x}",
                "manifest_next": f"0x{manifest_next:08x}",
                "name": candidate.expected_name,
                "historical_identity": candidate.historical_identity,
                "area": candidate.area,
                "provenance": PROVENANCE,
                "source": str(built.metadata["source"]),
                "profile": str(built.metadata["profile"]),
                "detail": candidate.detail,
                "object": rel(built.path),
                "object_symbol": candidate.symbol,
                "object_size": str(symbol.size),
                "boundary": candidate.boundary,
                "result": "MATCH",
                "differing_bytes": "0",
                "raw_equal": str(comparison.raw_equal),
                "normalized_equal": "True",
                "unknown_relocations": "",
                "relocation_count": str(len(comparison.relocation_ranges)),
                "object_sha256": v51.sha256_file(built.path),
                "cache_key": str(built.metadata["cache_key"]),
                "promotion_scope": (
                    "formal-manifest"
                    if candidate.formal_manifest
                    else "auxiliary-companion"
                ),
                "target_gate": f"formal-unpacked-elf:{TARGET_SHA256}",
                "target_span_sha256": sha256_bytes(span),
            }
        )
    formal = [row for row in evidence if row["promotion_scope"] == "formal-manifest"]
    companion = [
        row for row in evidence if row["promotion_scope"] == "auxiliary-companion"
    ]
    if len(formal) != 5 or len({row["address"] for row in formal}) != 5:
        raise SystemExit("V75 formal evidence cardinality gate failed")
    if len(companion) != 1 or companion[0]["address"] != "0x0010c174":
        raise SystemExit("V75 companion evidence cardinality gate failed")
    op1f = next(row for row in formal if row["address"] == "0x0010c094")
    op15 = companion[0]
    op0d = next(row for row in formal if row["address"] == "0x0010c1f8")
    if not (
        op1f["end_address"] == op15["address"]
        and op15["end_address"] == op0d["address"]
        and op1f["manifest_next"] == op0d["address"]
    ):
        raise SystemExit("V75 C4Op1F/C4Op15/C4Op0D corridor gate failed")
    return formal, companion


def promote(evidence_rows: list[dict[str, str]]) -> tuple[int, int]:
    target_fields, target_rows = v51.read_csv(TARGETS)
    symbol_fields, symbol_rows = v51.read_csv(SYMBOLS)
    targets = {row["address"].lower(): row for row in target_rows}
    symbols = {row["address"].lower(): row for row in symbol_rows}
    if set(targets) != set(symbols):
        raise SystemExit("target and symbol manifests have different address sets")
    changed = 0
    marker = "HUNT1041 V75 strict MATCH;"
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
            f"{marker} historical_identity={evidence['historical_identity']}; "
            f"provenance={evidence['provenance']}; source={evidence['source']}; "
            f"profile={evidence['profile']}; object_symbol={evidence['object_symbol']}; "
            f"object_size={evidence['object_size']}; boundary={evidence['boundary']}; "
            f"target_gate={evidence['target_gate']}; differing_bytes=0; "
            "normalized_equal=True; unknown_relocations=none; "
            f"evidence={rel(EVIDENCE)}"
        )
        for row in (target, symbol):
            prefix, separator, _old = row["notes"].partition(marker)
            row["notes"] = (
                prefix.rstrip("; ") + "; " + note if separator
                else row["notes"].rstrip("; ") + "; " + note
            )
            row["status"] = "MATCHING"
            row["confidence"] = "very-high"
        changed += int(old_status != "MATCHING")
    v51.write_csv_atomic(TARGETS, target_fields, target_rows)
    v51.write_csv_atomic(SYMBOLS, symbol_fields, symbol_rows)
    v51.run([sys.executable, str(ROOT / "tools" / "audit_source_completeness.py")])
    v51.run([sys.executable, str(ROOT / "tools" / "update_progress.py")])
    _fields, updated = v51.read_csv(TARGETS)
    formal = sum(row["status"] == "MATCHING" for row in updated)
    return changed, formal


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, default=REFERENCE)
    parser.add_argument(
        "--cxx",
        type=Path,
        default=(
            ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-cxx-stage1"
            / "prefix" / "bin" / "ee-g++"
        ),
    )
    parser.add_argument("--apply", action="store_true")
    args = parser.parse_args()
    reference = args.reference.expanduser().resolve()
    cxx = args.cxx.expanduser().resolve()
    if not cxx.is_file():
        raise SystemExit(f"missing EE C++ compiler: {cxx}")
    v51.run(
        [sys.executable, str(ROOT / "tools" / "probe_ee_toolchain.py"),
         "--compiler", str(cxx)]
    )
    if not reference.is_file():
        raise SystemExit(f"missing formal unpacked reference: {reference}; run make reference")
    target = reference.read_bytes()
    actual_sha = sha256_bytes(target)
    if actual_sha != TARGET_SHA256:
        raise SystemExit(f"unpacked target SHA-256 mismatch: {actual_sha}")
    objects = build_objects(cxx)
    rows, companion = make_evidence(target, objects)
    v51.write_csv_atomic(EVIDENCE, EVIDENCE_FIELDS, rows, delimiter="\t")
    v51.write_csv_atomic(
        COMPANION_EVIDENCE, EVIDENCE_FIELDS, companion, delimiter="\t"
    )
    print("V75 strict C4 formal matches: 5/5")
    print("V75 strict C4 auxiliary companion: 1/1 (not a manifest row)")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print("differing_bytes=0; unknown_relocations=none")
    print(f"evidence: {rel(EVIDENCE)}")
    print(f"companion evidence: {rel(COMPANION_EVIDENCE)}")
    if args.apply:
        changed, formal = promote(rows)
        print(f"promoted rows: {changed}; formal MATCHING now {formal}/1041")
    else:
        print("dry promotion; pass --apply to update the authoritative manifests")


if __name__ == "__main__":
    main()
