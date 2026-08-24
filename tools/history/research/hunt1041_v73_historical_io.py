#!/usr/bin/env python3
"""Rebuild and verify the two strict PS2-I/O historical matches from V73.

The runner starts from the hash-pinned official Snes9x 1.41-1 source, applies
only the SNES Station file-I/O adaptations proven by the target, compiles with
the historical EE GCC 3.2.2 C++ frontend, and compares against the private
unpacked ELF.  Relocations are masked by their precise MIPS field masks; no
other target bytes are ignored.
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
BUILD = ROOT / "build" / "matching" / "hunt1041-v73-historical-io"
EVIDENCE = ROOT / "analysis" / "matching" / "hunt1041-v73-validated-2.tsv"
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"
PROVENANCE = "snes9x-1.41-1-official-plus-snesstation-ps2-io"

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
    next_instruction: int | None = None

    @property
    def size(self) -> int:
        return self.end_address - self.address


CANDIDATES = (
    Candidate(
        0x00114544,
        0x00114674,
        "snes_p16_00114544",
        "S9xLoadCheatFile",
        "snes-core-dsp",
        "cheats2",
        "_Z16S9xLoadCheatFilePKc",
        "terminal-control-flow-boundary",
        "historical-symbol-strict; historical-identity=S9xLoadCheatFile; "
        "io=fioOpen/fioRead/fioClose; terminal=jr-ra; next=S9xSaveCheatFile",
        0x27BDFFA0,
    ),
    Candidate(
        0x001726EC,
        0x001728D4,
        "snes_p16_001726ec",
        "S9xSPCDump",
        "cpu-audio-runtime",
        "snapshot",
        "S9xSPCDump",
        "exact-next-boundary",
        "historical-symbol-strict; historical-identity=S9xSPCDump; "
        "io=PS2-fio; dsp-tail-seek=-128; write-result=one",
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


def patch_cheats2(source: Path) -> None:
    replace_once(
        source,
        '#include "MEMMAP.H"\n',
        '#include "MEMMAP.H"\n\n'
        'extern "C" int fioOpen (const char *, int);\n'
        'extern "C" int fioClose (int);\n'
        'extern "C" int fioRead (int, void *, int);\n',
        "cheat fio declarations",
    )
    start = "bool8 S9xLoadCheatFile (const char *filename)\n{"
    end = "\nbool8 S9xSaveCheatFile (const char *filename)"
    text = source.read_text(encoding="latin-1")
    left = text.find(start)
    right = text.find(end, left)
    if left < 0 or right < 0 or text.find(start, left + 1) >= 0:
        raise SystemExit("Snes9x 1.41-1 cheat loader patch context changed")
    replacement = """bool8 S9xLoadCheatFile (const char *filename)
{
    Cheat.num_cheats = 0;

    int fs = fioOpen (filename, 1);
    uint8 data [28];

    if (fs < 0)
        return (FALSE);

    while (fioRead (fs, (void *) data, 28) == 28)
    {
        Cheat.c [Cheat.num_cheats].enabled = (data [0] & 4) == 0;
        Cheat.c [Cheat.num_cheats].byte = data [1];
        Cheat.c [Cheat.num_cheats].address =
            data [2] | (data [3] << 8) | (data [4] << 16);
        Cheat.c [Cheat.num_cheats].saved_byte = data [5];
        Cheat.c [Cheat.num_cheats].saved = (data [0] & 8) != 0;
        memmove (Cheat.c [Cheat.num_cheats].name, &data [8], 20);
        Cheat.c [Cheat.num_cheats++].name [20] = 0;
    }
    fioClose (fs);

    return (TRUE);
}
"""
    v47.atomic_write_text(source, text[:left] + replacement + text[right:])


def patch_snapshot(source: Path) -> None:
    replace_once(
        source,
        '#include "spc7110.h"\n',
        '#include "spc7110.h"\n\n'
        'extern "C" int fioOpen (const char *, int);\n'
        'extern "C" int fioClose (int);\n'
        'extern "C" int fioWrite (int, const void *, int);\n'
        'extern "C" int fioPutc (int, int);\n'
        'extern "C" int fioLseek (int, int, int);\n',
        "snapshot fio declarations",
    )
    start = "bool8 S9xSPCDump (const char *filename)\n{"
    end = "\nbool8 S9xUnfreezeZSNES (const char *filename)"
    text = source.read_text(encoding="latin-1")
    left = text.find(start)
    right = text.find(end, left)
    if left < 0 or right < 0 or text.find(start, left + 1) >= 0:
        raise SystemExit("Snes9x 1.41-1 SPC dump patch context changed")
    replacement = """bool8 S9xSPCDump (const char *filename)
{
    static uint8 header [] = {
        'S', 'N', 'E', 'S', '-', 'S', 'P', 'C', '7', '0', '0', ' ',
        'S', 'o', 'u', 'n', 'd', ' ', 'F', 'i', 'l', 'e', ' ',
        'D', 'a', 't', 'a', ' ', 'v', '0', '.', '3', '0', 26, 26, 26
    };
    static uint8 version = { 0x1e };
    int fs;

    S9xSetSoundMute (TRUE);
    if (!(fs = fioOpen (filename, 3)))
        return (FALSE);

    if (fioWrite (fs, header, sizeof (header)) != 1 ||
        fioPutc (fs, version) == EOF ||
        fioLseek (fs, 37, SEEK_SET) == EOF ||
        fioPutc (fs, APURegisters.PC & 0xff) == EOF ||
        fioPutc (fs, APURegisters.PC >> 8) == EOF ||
        fioPutc (fs, APURegisters.YA.B.A) == EOF ||
        fioPutc (fs, APURegisters.X) == EOF ||
        fioPutc (fs, APURegisters.YA.B.Y) == EOF ||
        fioPutc (fs, APURegisters.P) == EOF ||
        fioPutc (fs, APURegisters.S) == EOF ||
        fioLseek (fs, 256, SEEK_SET) == EOF ||
        fioWrite (fs, IAPU.RAM, 0x10000) != 1 ||
        fioWrite (fs, spc_dump_dsp, 256) != 1 ||
        fioLseek (fs, -128, SEEK_CUR) == EOF ||
        fioWrite (fs, APU.ExtraRAM, 64) != 1 ||
        fioClose (fs) < 0)
    {
        S9xSetSoundMute (FALSE);
        return (FALSE);
    }
    S9xSetSoundMute (FALSE);
    return (TRUE);
}
"""
    v47.atomic_write_text(source, text[:left] + replacement + text[right:])


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
    patch_cheats2(layout / "cheats2.cpp")
    patch_snapshot(layout / "SNAPSHOT.CPP")
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
        "#ifndef HUNT1041_V73_MEMORY_H\n#define HUNT1041_V73_MEMORY_H\n"
        "#include <string.h>\n#endif\n",
    )
    flags = [
        *v47.COMMON_FLAGS,
        "-Os",
        *v47.SNES_DEFINES,
        *include_args((compat, newlib, layout, layout / "unzip", source_root / "zlib")),
        "-x",
        "c++",
    ]
    shared = (layout / "snes9x.h", layout / "MEMMAP.H", memory)
    builds = (
        ("cheats2", "cheats2.cpp", "snes9x-1.41-1-os-short-ps2-fio-cheats"),
        ("snapshot", "SNAPSHOT.CPP", "snes9x-1.41-1-os-short-ps2-fio-spcdump"),
    )
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
) -> list[dict[str, str]]:
    _rows, manifest, next_address = read_manifest()
    elf_cache: dict[Path, ELFFile] = {}
    evidence: list[dict[str, str]] = []
    for candidate in CANDIDATES:
        address = f"0x{candidate.address:08x}"
        target_row = manifest.get(candidate.address)
        manifest_next = next_address.get(candidate.address)
        if target_row is None or manifest_next is None:
            raise SystemExit(f"{address}: missing audited target boundary")
        if target_row["name"] != candidate.expected_name or target_row["area"] != candidate.area:
            raise SystemExit(f"{address}: manifest identity changed")
        if target_row["status"] not in {"RECONSTRUCTED", "MATCHING"}:
            raise SystemExit(f"{address}: unexpected manifest status")
        if candidate.boundary == "exact-next-boundary":
            if manifest_next != candidate.end_address:
                raise SystemExit(f"{address}: exact boundary changed")
        elif not (candidate.end_address < manifest_next):
            raise SystemExit(f"{address}: terminal boundary no longer precedes manifest boundary")

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
        if candidate.boundary == "terminal-control-flow-boundary":
            jr_ra = int.from_bytes(span[-8:-4], "little")
            next_word = int.from_bytes(
                target[
                    candidate.end_address - TARGET_BASE :
                    candidate.end_address - TARGET_BASE + 4
                ],
                "little",
            )
            if jr_ra != 0x03E00008:
                raise SystemExit(f"{address}: terminal jr-ra boundary changed")
            if next_word != candidate.next_instruction:
                raise SystemExit(f"{address}: following function prologue changed")
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
                "target_gate": f"formal-unpacked-elf:{TARGET_SHA256}",
                "target_span_sha256": sha256_bytes(span),
            }
        )
    if len(evidence) != 2 or len({row["address"] for row in evidence}) != 2:
        raise SystemExit("V73 evidence cardinality gate failed")
    return evidence


def promote(evidence_rows: list[dict[str, str]]) -> tuple[int, int]:
    target_fields, target_rows = v51.read_csv(TARGETS)
    symbol_fields, symbol_rows = v51.read_csv(SYMBOLS)
    targets = {row["address"].lower(): row for row in target_rows}
    symbols = {row["address"].lower(): row for row in symbol_rows}
    if set(targets) != set(symbols):
        raise SystemExit("target and symbol manifests have different address sets")
    changed = 0
    marker = "HUNT1041 V73 strict MATCH;"
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
    rows = make_evidence(target, objects)
    v51.write_csv_atomic(EVIDENCE, EVIDENCE_FIELDS, rows, delimiter="\t")
    print("V73 strict historical PS2-I/O matches: 2/2")
    print(f"target gate: formal-unpacked-elf:{TARGET_SHA256}")
    print("differing_bytes=0; unknown_relocations=none")
    print(f"evidence: {rel(EVIDENCE)}")
    if args.apply:
        changed, formal = promote(rows)
        print(f"promoted rows: {changed}; formal MATCHING now {formal}/1041")
    else:
        print("dry promotion; pass --apply to update the authoritative manifests")


if __name__ == "__main__":
    main()
