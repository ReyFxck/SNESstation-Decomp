#!/usr/bin/env python3
"""Verify complete checked-in source coverage for the final native census gaps.

This gate closes source-body availability, not target machine-code identity.
Thirty-two bodies lack complete frozen target witnesses, four retain failed
full-link comparisons, and two clip comparators remain source-only helpers.
No private ELF is read and no new exact-function promotion is made.
"""
from __future__ import annotations

import csv
import hashlib
import importlib.util
import json
import subprocess
import sys
from collections import Counter
from pathlib import Path

from build_source_tree import SOURCE_FIXED_FLAGS
from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-pending-source-archive-recovery"
CENSUS = ROOT / "analysis/functions/native_bulk_census.tsv"
MANIFEST = ROOT / "analysis/functions/native_pending_source_archive.tsv"
ARCHIVE_ROOT = ROOT / "build/upstream/hunt1000plus-v47-snes/source-1.41-1/snes9x-1.41-1-src/snes9x"
ARCHIVE_SHA256 = "5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b"

MODULES = {
    "dsp1": ("DSP1.CPP", ()),
    "memmap": ("MEMMAP.CPP", ("-DFAST_LSB_WORD_ACCESS",)),
    "soundux": ("SOUNDUX.CPP", ()),
    "snapshot": ("SNAPSHOT.CPP", ()),
    "snaporig": ("snaporig.cpp", ()),
    "spc7110": ("spc7110.cpp", ()),
}
SOURCE_STATUS = {
    "retained-pinned-historical-source-no-complete-target-witness",
    "retained-pinned-historical-source-blocked-link-comparison",
}
CLIP_STATUS = "retained-checked-in-upstream-helper-no-complete-target-body-witness"


def digest(raw: bytes) -> str:
    return hashlib.sha256(raw).hexdigest()


def run(command: list[object]) -> str:
    result = subprocess.run(
        [str(part) for part in command], cwd=ROOT, text=True,
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command)) + "\n" + result.stdout[-16000:])
    return result.stdout


def load_module(path: Path):
    spec = importlib.util.spec_from_file_location("native_pending_cpu_profile", path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main() -> None:
    cpu = load_module(ROOT / "tools/run-native-cpu-interrupts-source-recovery.py")
    if run([cpu.CXX, "-dumpmachine"]).strip() != "ee" or run([cpu.CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")

    manifest = list(csv.DictReader(MANIFEST.open(newline=""), delimiter="\t"))
    if len(manifest) != 6 or {Path(row["file"]).name for row in manifest} != {value[0] for value in MODULES.values()}:
        raise SystemExit("pending source archive manifest drift")
    manifest_by_name = {Path(row["file"]).name: row for row in manifest}
    for name, _ in MODULES.values():
        row = manifest_by_name[name]
        archive = (ARCHIVE_ROOT / name).read_bytes()
        checked_path = ROOT / row["file"]
        checked = checked_path.read_bytes()
        # The pinned tarball is Latin-1. UTF-8 repository text must round-trip
        # to the exact archive bytes, including MEMMAP's degree characters.
        roundtrip = checked_path.read_text(encoding="utf-8").encode("latin1")
        if (len(archive) != int(row["archive_size"]) or digest(archive) != row["archive_sha256"]
                or len(checked) != int(row["checked_size"]) or digest(checked) != row["checked_sha256"]
                or roundtrip != archive):
            raise SystemExit("pinned historical source byte/encoding drift: " + name)
        if "Snes9x - Portable Super Nintendo Entertainment System" not in checked_path.read_text(encoding="utf-8"):
            raise SystemExit("historical license header drift: " + name)

    rows = list(csv.DictReader(CENSUS.open(newline=""), delimiter="\t"))
    if len(rows) != 250 or len({(row["module"], row["symbol"]) for row in rows}) != 250:
        raise SystemExit("native census roster drift")
    archived = [row for row in rows if row["status"] in SOURCE_STATUS]
    clip = [row for row in rows if row["status"] == CLIP_STATUS]
    if (len(archived) != 36 or len(clip) != 2
            or Counter(row["status"] for row in archived) != {
                "retained-pinned-historical-source-no-complete-target-witness": 32,
                "retained-pinned-historical-source-blocked-link-comparison": 4,
            }
            or Counter(row["module"] for row in archived) != {
                "dsp1": 5, "memmap": 8, "soundux": 6,
                "snapshot": 4, "snaporig": 1, "spc7110": 12,
            }):
        raise SystemExit("zero-gap historical source classification drift")
    for row in archived:
        expected = "src/snes9x/historical_reference/" + MODULES[row["module"]][0]
        if row["source_file"] != expected or row["evidence"] != "tools/run-native-pending-archive-source-recovery.py":
            raise SystemExit("historical source census provenance drift: " + row["symbol"])
    if {(row["symbol"], row["source_file"], row["size"]) for row in clip} != {
        ("_Z10IntComparePKvS0_", "src/snes9x/native_clip.cpp", "40"),
        ("_Z11BandComparePKvS0_", "src/snes9x/native_clip.cpp", "40"),
    }:
        raise SystemExit("checked-in clip helper source closure drift")
    forbidden = {"no-complete-frozen-function-witness", "blocked-after-full-link-check",
                 "retained-upstream-helper-no-complete-target-body-witness"}
    if forbidden & {row["status"] for row in rows}:
        raise SystemExit("legacy source-body gap remains in native census")

    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v52_closure as recipe
    if recipe.v47.SNES_141_1_ARCHIVE.sha256 != ARCHIVE_SHA256:
        raise SystemExit("pinned Snes9x archive digest declaration drift")
    BUILD.mkdir(parents=True, exist_ok=True)
    old_build = recipe.BUILD
    try:
        recipe.BUILD = BUILD / "profile"
        root, _upstream, layout = recipe.prepare_snes_layout()
        recipe.patch_sources(layout)
        recipe.write_compat_headers(BUILD / "compat")
    finally:
        recipe.BUILD = old_build
    newlib = recipe.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    includes = ["-I" + str(path) for path in (BUILD / "compat", newlib, layout, layout / "unzip", root / "zlib")]
    compiled: dict[str, ELFFile] = {}
    for module, (name, extra) in MODULES.items():
        output = BUILD / (module + ".o")
        run([cpu.CXX, *cpu.FLAGS[:-1], *extra, "-DZLIB", *includes,
             "-x", "c++", "-c", layout / name, "-o", output])
        compiled[module] = ELFFile(output)
    for row in archived:
        symbol = compiled[row["module"]].find_symbol(row["symbol"])
        if symbol.size != int(row["size"]):
            raise SystemExit("historical compiler symbol/size drift: " + row["symbol"])

    clip_output = BUILD / "native_clip.o"
    run([cpu.CXX, *SOURCE_FIXED_FLAGS["src/snes9x/native_clip.cpp"],
         "-c", ROOT / "src/snes9x/native_clip.cpp", "-o", clip_output])
    clip_elf = ELFFile(clip_output)
    for row in clip:
        if clip_elf.find_symbol(row["symbol"]).size != int(row["size"]):
            raise SystemExit("checked-in clip helper compiler geometry drift: " + row["symbol"])

    canonical = (ROOT / "analysis/source_tree/translation_units.tsv").read_text()
    if "src/snes9x/historical_reference/" in canonical:
        raise SystemExit("unproved historical reference entered canonical link tree")
    print("native pending source archive: 38/38 bodies retained; 0 source gaps; 32 target-witness gaps + 4 blocked links + 2 source-only helpers explicitly preserved")


if __name__ == "__main__":
    main()
