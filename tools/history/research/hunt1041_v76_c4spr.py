#!/usr/bin/env python3
"""Rebuild and verify the strict V76 C4SprDisintegrate match.

The runner starts from the hash-pinned official Snes9x 1.41-1 source, applies
the target-proven SNES Station PS2 unaligned-word/codegen adaptation, compiles
with the historical EE GCC 3.2.2 C++ frontend, and compares the complete
function against the private unpacked ELF. Only precise MIPS relocation fields
are normalized; no instruction byte is otherwise ignored.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import shutil
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))

from compare_elf_functions import ELFFile, compare_function  # noqa: E402
import hunt1000plus_v47_closure as v47  # noqa: E402
import hunt1041_v51_closure as v51  # noqa: E402


TARGET_BASE = 0x00100000
TARGET_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
TARGET_SPAN_SHA256 = (
    "c7390c51c88e66701aff3338d7a421055d8cb6dd950dfaffcb0371574bc263b1"
)
REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
BUILD = ROOT / "build" / "matching" / "hunt1041-v76-c4spr"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v76-validated-c4spr-1.tsv"
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"

ADDRESS = 0x0010D4F0
END_ADDRESS = 0x0010D734
EXPECTED_NAME = "snes_p16_0010d4f0"
HISTORICAL_IDENTITY = "C4SprDisintegrate"
AREA = "frontend-core"
OBJECT_SYMBOL = "_Z17C4SprDisintegratev"
PROVENANCE = "snes9x-1.41-1-official-plus-snesstation-ps2-c4emu-codegen"
PROFILE = "snes9x-1.41-1-os-normal-double-ps2-packed-readword-a3-signed-v0-fno-builtin"

PATCHED_SOURCE_SHA256 = (
    "e0c8151906f06ff51d433ac04427f5d826485f76782dfba3fd2acfc98da3c5fe"
)
PATCHED_MEMMAP_SHA256 = (
    "d9c51acdcb3d7df710895206e9e5ccca88f3da0f768e4aef18c0c8f9a7c9c201"
)
COMPAT_MEMORY_SHA256 = (
    "5c3dcaf026069bf47ccab0f15f0ab623f5e6033dbceae4bceda12ec8a093a492"
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

PACKED_READ_WORD = r'''struct Hunt1041PackedU32
{
    uint32 value;
} __attribute__((packed));

#define READ_WORD(s) ({ \
    register uint8 *source = (uint8 *) (s); \
    __asm__ volatile ("" : "+r" (source)); \
    register uint32 value = ((Hunt1041PackedU32 *) source)->value; \
    value &= 0xffff; \
    __asm__ volatile ("" : "+r" (value)); \
    (uint16) (value); \
})

/*
 * EE GCC 3.2.2 otherwise coalesces the final sign-extension back into $a0.
 * Keeping this single non-volatile shift as a code-generation constraint
 * reproduces the target's $a0 -> $v0 -> $s4 allocation without constraining
 * the surrounding C implementation or copying target instructions wholesale.
 */
#define READ_SIGNED_WORD_V0(s) ({ \
    register uint8 *source __asm__("$2") = (uint8 *) (s); \
    __asm__ volatile ("" : "+r" (source)); \
    register uint32 value __asm__("$4") = ((Hunt1041PackedU32 *) source)->value; \
    value &= 0xffff; \
    __asm__ volatile ("" : "+r" (value)); \
    register uint32 shifted __asm__("$2"); \
    __asm__ ("sll %0,%1,16" : "=r" (shifted) : "r" (value)); \
    ((int32) shifted) >> 16; \
})
'''

COMPAT_MEMORY = (
    '#ifndef _STRING_H_\n#define _STRING_H_\nextern "C" {\n'
    'void *memset(void *, int, unsigned int);\n'
    'void *memmove(void *, const void *, unsigned int);\n}\n#endif\n'
)


def rel(path: Path) -> str:
    return path.resolve().relative_to(ROOT.resolve()).as_posix()


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def checked_replace(text: str, old: str, new: str, label: str) -> str:
    if text.count(old) != 1:
        raise SystemExit(f"Snes9x 1.41-1 {label} patch context changed")
    return text.replace(old, new, 1)


def patch_memmap(path: Path) -> None:
    text = path.read_text(encoding="latin-1")
    include = '#include "snes9x.h"\n'
    declaration = PACKED_READ_WORD.split("#define READ_WORD", 1)[0]
    text = checked_replace(
        text, include, include + "\n" + declaration.rstrip("\n") + "\n",
        "MEMMAP packed-read declaration",
    )
    macros = "#define READ_WORD" + PACKED_READ_WORD.split("#define READ_WORD", 1)[1]
    pattern = re.compile(r'#else\n#define READ_WORD\(s\).*?(?=#define READ_DWORD)', re.S)
    text, count = pattern.subn("#else\n" + macros, text, count=1)
    if count != 1:
        raise SystemExit("Snes9x 1.41-1 MEMMAP READ_WORD patch context changed")
    v47.atomic_write_text(path, text)
    actual = v51.sha256_file(path)
    if actual != PATCHED_MEMMAP_SHA256:
        raise SystemExit(f"patched MEMMAP.H SHA-256 mismatch: {actual}")


def patch_c4emu(path: Path) -> None:
    text = path.read_text(encoding="latin-1")
    start = text.index("static void C4SprDisintegrate()")
    end = text.index("static void S9xC4ProcessSprites()", start)
    before, body, after = text[:start], text[start:end], text[end:]
    replacements = (
        (
            "    uint8 *src;\n",
            "    uint8 *src;\n"
            '    register uint8 *ram __asm__("$7")=Memory.C4RAM;\n',
        ),
        ("width=Memory.C4RAM[0x1f89];", "width=ram[0x1f89];"),
        ("height=Memory.C4RAM[0x1f8c];", "height=ram[0x1f8c];"),
        ("READ_WORD(Memory.C4RAM+0x1f80)", "READ_WORD(ram+0x1f80)"),
        ("READ_WORD(Memory.C4RAM+0x1f83)", "READ_WORD(ram+0x1f83)"),
        ("READ_WORD(Memory.C4RAM+0x1f86)", "READ_WORD(ram+0x1f86)"),
        (
            "scaleY=(int16)READ_WORD(Memory.C4RAM+0x1f8f);",
            "scaleY=READ_SIGNED_WORD_V0(ram+0x1f8f);",
        ),
        ("src=Memory.C4RAM+0x600;", "src=ram+0x600;"),
        (
            "memset(Memory.C4RAM, 0, width*height/2);",
            "memset(ram, 0, width*height/2);",
        ),
    )
    for old, new in replacements:
        if body.count(old) != 1:
            raise SystemExit(f"Snes9x 1.41-1 C4Spr patch context changed: {old!r}")
        body = body.replace(old, new, 1)
    v47.atomic_write_text(path, before + body + after)
    actual = v51.sha256_file(path)
    if actual != PATCHED_SOURCE_SHA256:
        raise SystemExit(f"patched c4emu.cpp SHA-256 mismatch: {actual}")


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
    compat = BUILD / "compat"
    compat.mkdir(parents=True, exist_ok=True)
    memory = compat / "memory.h"
    v47.atomic_write_text(memory, COMPAT_MEMORY)
    if v51.sha256_file(memory) != COMPAT_MEMORY_SHA256:
        raise SystemExit("compat memory.h SHA-256 mismatch")
    patch_memmap(layout / "MEMMAP.H")
    patch_c4emu(layout / "c4emu.cpp")
    return source_root, original, layout, newlib


def include_args(paths: tuple[Path, ...]) -> list[str]:
    result: list[str] = []
    for path in paths:
        result.extend(("-I", rel(path)))
    return result


def build_object(cxx: Path) -> v51.ObjectBuild:
    source_root, original, layout, newlib = prepare_layout()
    memory = BUILD / "compat" / "memory.h"
    common_flags = [flag for flag in v47.COMMON_FLAGS if flag != "-fshort-double"]
    flags = [
        *common_flags,
        "-Os",
        "-fno-builtin",
        *v47.SNES_DEFINES,
        *include_args((BUILD / "compat", newlib, layout, layout / "unzip", source_root / "zlib")),
        "-x",
        "c++",
    ]
    output = BUILD / "objects" / "c4emu.o"
    output.parent.mkdir(parents=True, exist_ok=True)
    v47.run([str(cxx), *flags, "-c", rel(layout / "c4emu.cpp"), "-o", rel(output)], cwd=ROOT)
    extra_inputs = (layout / "MEMMAP.H", layout / "c4.h", layout / "snes9x.h", memory)
    payload = {
        "compiler_sha256": v51.sha256_file(cxx),
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
        raise SystemExit("0x0010d4f0: manifest identity or boundary changed")
    elf = ELFFile(built.path)
    symbol = elf.find_symbol(OBJECT_SYMBOL)
    size = END_ADDRESS - ADDRESS
    if symbol.size != size:
        raise SystemExit(f"0x0010d4f0: object size changed from {size} to {symbol.size}")
    comparison = compare_function(
        target, ADDRESS - TARGET_BASE, size, elf, OBJECT_SYMBOL, 4
    )
    if not comparison.matching or comparison.differing_bytes:
        first = ",".join(f"+0x{x:x}" for x in comparison.first_differences)
        raise SystemExit(
            "0x0010d4f0: strict comparison failed "
            f"({comparison.differing_bytes}; {first or 'size'})"
        )
    if comparison.unknown_relocation_types:
        raise SystemExit(
            "0x0010d4f0: unknown relocation types "
            f"{comparison.unknown_relocation_types}"
        )
    span = target[ADDRESS - TARGET_BASE : END_ADDRESS - TARGET_BASE]
    actual_span_hash = sha256_bytes(span)
    if actual_span_hash != TARGET_SPAN_SHA256:
        raise SystemExit(f"0x0010d4f0: target span SHA-256 mismatch: {actual_span_hash}")
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
            "historical-symbol-strict; packed 32-bit unaligned READ_WORD masked to "
            "uint16; C4RAM fixed in $a3; 32-bit memset length; one non-volatile "
            "inline sll pins the target-proven $a0->$v0 sign-extension allocation"
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
    marker = "HUNT1041 V76 strict MATCH;"
    provisional_note = (
        "address label retained because the historical symbol is unproven"
    )
    proven_note = (
        "manifest address label retained; historical identity is proven below"
    )
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
        [sys.executable, str(ROOT / "tools" / "probe_ee_toolchain.py"),
         "--compiler", str(cxx)]
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
    print("V76 strict C4SprDisintegrate formal match: 1/1")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print("object_size=580; differing_bytes=0; unknown_relocations=none")
    print(f"evidence: {rel(EVIDENCE)}")
    if args.apply:
        changed, formal = promote(evidence)
        print(f"promoted rows: {changed}; formal MATCHING now {formal}/1041")
    else:
        print("dry promotion; pass --apply to update the authoritative manifests")


if __name__ == "__main__":
    main()
