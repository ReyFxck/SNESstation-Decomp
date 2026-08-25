#!/usr/bin/env python3
"""Rebuild, verify, and optionally promote the strict V78 C4BitPlaneWave match.

The runner starts from the hash-pinned official Snes9x 1.41-1 source, applies
the already-proven SNES Station PS2 C4 access forms, and compiles with the
portable EE GCC 3.2.2 C++ bootstrap.  An isolated cc1plus profile changes only
the target-proven local-allocation tie-break ($t5 before $t4); the canonical
compiler and its build tree are left untouched.  Only precise MIPS relocation
fields are normalized by the final comparison.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))

from build_ee_gcc_regalloc_profile import build_profile  # noqa: E402
from compare_elf_functions import ELFFile, compare_function  # noqa: E402
import hunt1000plus_v47_closure as v47  # noqa: E402
import hunt1041_v51_closure as v51  # noqa: E402
import hunt1041_v77_c4draw as v77  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
TARGET_SPAN_SHA256 = (
    "238f6f0603598494941793416566184c83843985afe3bd3913b0df34b83e4c00"
)
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
BUILD = ROOT / "build" / "matching" / "hunt1041-v78-c4bit"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v78-validated-c4bit-1.tsv"
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"

ADDRESS = 0x0010D2A8
END_ADDRESS = 0x0010D4F0
EXPECTED_NAME = "snes_p16_0010d2a8"
HISTORICAL_IDENTITY = "C4BitPlaneWave"
AREA = "frontend-core"
OBJECT_SYMBOL = "_Z14C4BitPlaneWavev"
PROVENANCE = (
    "snes9x-1.41-1-official-plus-snesstation-ps2-packed-c4-wave-access"
)
PROFILE = (
    "snes9x-1.41-1-os-normal-double-ps2-packed-wave-fixed-loads-"
    "local-t5-before-t4-fno-builtin"
)

PATCHED_SOURCE_SHA256 = (
    "23827b996c2f4817456f299f2d6fef58309dbd89ec388dda6b2026e98717964e"
)
PATCHED_MEMMAP_SHA256 = v77.PATCHED_MEMMAP_SHA256
COMPAT_MEMORY_SHA256 = v77.COMPAT_MEMORY_SHA256
EVIDENCE_FIELDS = v77.EVIDENCE_FIELDS

READ_WAVE_WORD_REG = r'''#define READ_WAVE_WORD_REG(s, regname) ({ \
    register uint8 *source = (uint8 *) (s); \
    __asm__ volatile ("" : "+r" (source)); \
    register uint32 value __asm__(regname) = ((Hunt1041PackedU32 *) source)->value; \
    value &= 0xffff; \
    __asm__ volatile ("" : "+r" (value)); \
    register uint32 result = value & 0xffff; \
    __asm__ volatile ("" : "+r" (value), "+r" (result)); \
    result; \
})'''


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT.resolve()).as_posix()


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def checked_replace(text: str, old: str, new: str, label: str) -> str:
    if text.count(old) != 1:
        raise SystemExit(f"Snes9x 1.41-1 {label} patch context changed")
    return text.replace(old, new, 1)


def patch_c4bit(path: Path) -> None:
    text = path.read_text(encoding="latin-1")
    start = text.index("static void C4BitPlaneWave()")
    end = text.index("static void C4SprDisintegrate()", start)
    before, body, after = text[:start], text[start:end], text[end:]
    before = before + READ_WAVE_WORD_REG + "\n"
    body = checked_replace(
        body,
        "    for(int j=0; j<0x10; j++){",
        '    register int j __asm__("$16");\n    for(j=0; j<0x10; j++){',
        "C4Bit outer-loop allocation",
    )
    dst_read = "READ_WORD(dst+bmpdata[i])"
    if body.count(dst_read) != 2:
        raise SystemExit("Snes9x 1.41-1 C4Bit destination-read contexts changed")
    body = body.replace(
        dst_read, 'READ_WAVE_WORD_REG(dst+bmpdata[i], "$14")', 1
    )
    body = body.replace(
        dst_read, 'READ_WAVE_WORD_REG(dst+bmpdata[i], "$24")', 1
    )
    body = checked_replace(
        body,
        "READ_WORD(Memory.C4RAM+0xa00+height*2)",
        'READ_WAVE_WORD_REG(Memory.C4RAM+0xa00+height*2, "$15")',
        "C4Bit first wave read",
    )
    body = checked_replace(
        body,
        "READ_WORD(Memory.C4RAM+0xa10+height*2)",
        'READ_WAVE_WORD_REG(Memory.C4RAM+0xa10+height*2, "$25")',
        "C4Bit second wave read",
    )
    body = checked_replace(
        body,
        'uint16 tmp=READ_WAVE_WORD_REG(dst+bmpdata[i], "$24") & mask2;',
        'uint32 tmp=mask2 & READ_WAVE_WORD_REG(dst+bmpdata[i], "$24");',
        "C4Bit second-plane temporary",
    )
    v47.atomic_write_text(path, before + body + after)
    actual = v51.sha256_file(path)
    if actual != PATCHED_SOURCE_SHA256:
        raise SystemExit(f"patched c4emu.cpp SHA-256 mismatch: {actual}")


def prepare_layout() -> tuple[Path, Path, Path, Path]:
    previous_build = v77.BUILD
    v77.BUILD = BUILD
    try:
        source_root, original, layout, newlib = v77.prepare_layout()
    finally:
        v77.BUILD = previous_build
    patch_c4bit(layout / "c4emu.cpp")
    return source_root, original, layout, newlib


def include_args(paths: tuple[Path, ...]) -> list[str]:
    result: list[str] = []
    for path in paths:
        result.extend(("-I", rel(path)))
    return result


def build_object(cxx: Path) -> v51.ObjectBuild:
    try:
        regalloc_profile = build_profile(cxx)
    except RuntimeError as exc:
        raise SystemExit(str(exc)) from exc
    source_root, original, layout, newlib = prepare_layout()
    memory = BUILD / "compat" / "memory.h"
    common_flags = [flag for flag in v47.COMMON_FLAGS if flag != "-fshort-double"]
    flags = [
        f"-B{regalloc_profile.as_posix()}/",
        *common_flags,
        "-Os",
        "-fno-builtin",
        *v47.SNES_DEFINES,
        *include_args(
            (BUILD / "compat", newlib, layout, layout / "unzip", source_root / "zlib")
        ),
        "-x",
        "c++",
    ]
    output = BUILD / "objects" / "c4emu.o"
    output.parent.mkdir(parents=True, exist_ok=True)
    v47.run(
        [str(cxx), *flags, "-c", rel(layout / "c4emu.cpp"), "-o", rel(output)],
        cwd=ROOT,
    )
    profile_cc1plus = regalloc_profile / "cc1plus"
    extra_inputs = (
        layout / "MEMMAP.H",
        layout / "c4.h",
        layout / "snes9x.h",
        memory,
        ROOT / "tools" / "build_ee_gcc_regalloc_profile.py",
    )
    payload = {
        "compiler_sha256": v51.sha256_file(cxx),
        "profile_cc1plus_sha256": v51.sha256_file(profile_cc1plus),
        "flags": flags,
        "source_sha256": v51.sha256_file(layout / "c4emu.cpp"),
        "extra_inputs": {rel(path): v51.sha256_file(path) for path in extra_inputs},
        "upstream": v47.SNES_141_1_ARCHIVE.sha256,
    }
    metadata = {
        "cache_key": hashlib.sha256(
            json.dumps(payload, sort_keys=True, separators=(",", ":")).encode()
        ).hexdigest(),
        "source": rel(original / "c4emu.cpp"),
        "profile": PROFILE,
    }
    v47.atomic_write_text(
        output.with_suffix(".json"),
        json.dumps(metadata, indent=2, sort_keys=True) + "\n",
    )
    return v51.ObjectBuild(output, metadata)


def make_evidence(target: bytes, built: v51.ObjectBuild) -> dict[str, str]:
    _fields, rows = v51.read_csv(TARGETS)
    by_address = {int(row["address"], 0): row for row in rows}
    starts = sorted(by_address)
    index = starts.index(ADDRESS)
    manifest_next = starts[index + 1]
    row = by_address[ADDRESS]
    if (
        row["name"] != EXPECTED_NAME
        or row["area"] != AREA
        or row["status"] not in {"RECONSTRUCTED", "MATCHING"}
        or manifest_next != END_ADDRESS
    ):
        raise SystemExit("0x0010d2a8: manifest identity or boundary changed")
    elf = ELFFile(built.path)
    symbol = elf.find_symbol(OBJECT_SYMBOL)
    size = END_ADDRESS - ADDRESS
    if symbol.size != size:
        raise SystemExit(f"0x0010d2a8: object size changed from {size} to {symbol.size}")
    comparison = compare_function(
        target, ADDRESS - TARGET_BASE, size, elf, OBJECT_SYMBOL, 4
    )
    if not comparison.matching or comparison.differing_bytes:
        first = ",".join(f"+0x{x:x}" for x in comparison.first_differences)
        raise SystemExit(
            "0x0010d2a8: strict comparison failed "
            f"({comparison.differing_bytes}; {first or 'size'})"
        )
    if comparison.unknown_relocation_types:
        raise SystemExit(
            "0x0010d2a8: unknown relocation types "
            f"{comparison.unknown_relocation_types}"
        )
    span = target[ADDRESS - TARGET_BASE : END_ADDRESS - TARGET_BASE]
    actual_span_hash = sha256_bytes(span)
    if actual_span_hash != TARGET_SPAN_SHA256:
        raise SystemExit(f"0x0010d2a8: target span SHA-256 mismatch: {actual_span_hash}")
    return {
        "address": f"0x{ADDRESS:08x}",
        "end_address": f"0x{END_ADDRESS:08x}",
        "manifest_next": f"0x{manifest_next:08x}",
        "name": EXPECTED_NAME,
        "historical_identity": HISTORICAL_IDENTITY,
        "area": AREA,
        "provenance": PROVENANCE,
        "source": str(built.metadata["source"]),
        "profile": str(built.metadata["profile"]),
        "detail": (
            "historical-symbol-strict; packed unaligned wave reads retain four "
            "distinct target load registers; the isolated compiler profile "
            "reproduces the target-proven local $t5-before-$t4 allocation tie-break"
        ),
        "object": rel(built.path),
        "object_symbol": OBJECT_SYMBOL,
        "object_size": str(symbol.size),
        "boundary": "exact-next-boundary",
        "result": "MATCH",
        "differing_bytes": "0",
        "raw_equal": str(comparison.raw_equal),
        "normalized_equal": "True",
        "unknown_relocations": "",
        "relocation_count": str(len(comparison.relocation_ranges)),
        "object_sha256": v51.sha256_file(built.path),
        "cache_key": str(built.metadata["cache_key"]),
        "promotion_scope": "formal-manifest",
        "target_gate": f"formal-unpacked-elf:{TARGET_SHA256}",
        "target_span_sha256": actual_span_hash,
    }


def promote(evidence: dict[str, str]) -> tuple[int, int]:
    target_fields, target_rows = v51.read_csv(TARGETS)
    symbol_fields, symbol_rows = v51.read_csv(SYMBOLS)
    targets = {row["address"].lower(): row for row in target_rows}
    symbols = {row["address"].lower(): row for row in symbol_rows}
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
    marker = "HUNT1041 V78 strict MATCH;"
    provisional_note = "address label retained because the historical symbol is unproven"
    proven_note = "manifest address label retained; historical identity is proven below"
    note = (
        f"{marker} historical_identity={evidence['historical_identity']}; "
        f"provenance={evidence['provenance']}; source={evidence['source']}; "
        f"profile={evidence['profile']}; object_symbol={evidence['object_symbol']}; "
        f"object_size={evidence['object_size']}; boundary={evidence['boundary']}; "
        f"target_gate={evidence['target_gate']}; differing_bytes=0; "
        "normalized_equal=True; unknown_relocations=none; "
        f"evidence={rel(EVIDENCE)}"
    )
    for manifest_row in (target, symbol):
        manifest_row["notes"] = manifest_row["notes"].replace(
            provisional_note, proven_note
        )
        prefix, separator, _old = manifest_row["notes"].partition(marker)
        manifest_row["notes"] = (
            prefix.rstrip("; ") + "; " + note
            if separator
            else manifest_row["notes"].rstrip("; ") + "; " + note
        )
        manifest_row["status"] = "MATCHING"
        manifest_row["confidence"] = "very-high"
    v51.write_csv_atomic(TARGETS, target_fields, target_rows)
    v51.write_csv_atomic(SYMBOLS, symbol_fields, symbol_rows)
    v51.run([sys.executable, str(ROOT / "tools" / "audit_source_completeness.py")])
    v51.run([sys.executable, str(ROOT / "tools" / "update_frontier_map.py")])
    v51.run([sys.executable, str(ROOT / "tools" / "update_progress.py")])
    _fields, updated = v51.read_csv(TARGETS)
    formal = sum(row["status"] == "MATCHING" for row in updated)
    return int(old_status != "MATCHING"), formal


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
    built = build_object(cxx)
    evidence = make_evidence(target, built)
    v51.write_csv_atomic(EVIDENCE, EVIDENCE_FIELDS, [evidence], delimiter="\t")
    print("V78 strict C4BitPlaneWave formal match: 1/1")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print(
        f"object_size={END_ADDRESS - ADDRESS}; differing_bytes=0; "
        "unknown_relocations=none"
    )
    print(f"evidence: {rel(EVIDENCE)}")
    if args.apply:
        changed, formal = promote(evidence)
        print(f"promoted rows: {changed}; formal MATCHING now {formal}/1041")
    else:
        print("dry promotion; pass --apply to update the authoritative manifests")


if __name__ == "__main__":
    main()
