#!/usr/bin/env python3
"""Verify complete source and frozen-target relations for the final census set.

All 38 formerly pending bodies are rebuilt.  Nine historical bodies have
complete relocation-normalized target proofs, two clip helpers have complete
provider-linked target bytes, and every remaining historical body is mapped to
its frozen exact target variant, aggregate, inline site, or no-op replacement.
No private ELF is read and no replacement-image claim is made.
"""
from __future__ import annotations

import csv
import hashlib
import importlib.util
import json
import re
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
RELATIONS = ROOT / "analysis/functions/native_pending_target_relations.tsv"
CODE_WINDOWS = ROOT / "analysis/link_identity/code_windows.json"
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
STATUS_BY_RELATION = {
    "exact-normalized-target": "proved-frozen-normalized-target",
    "exact-linked-target": "proved-frozen-linked-target",
    "target-variant-exact-window": "retained-source-with-frozen-target-variant",
    "target-aggregate-variant": "retained-source-with-frozen-target-aggregate",
    "inlined-into-target-variant": "retained-source-inlined-into-frozen-target-variant",
    "target-noop-variant": "retained-source-with-frozen-target-noop-variant",
}


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


def non_relocation_differences(target: bytes, elf: ELFFile, symbol_name: str) -> list[int]:
    """Return differing byte offsets after masking only candidate relocations."""
    symbol = elf.find_symbol(symbol_name)
    candidate = elf.symbol_bytes(symbol, symbol.size)
    if len(target) != len(candidate):
        return list(range(min(len(target), len(candidate)), max(len(target), len(candidate)))) or [-1]
    masks = elf.relocation_masks(symbol, 4)
    differences = []
    for index, (expected, actual) in enumerate(zip(target, candidate)):
        ignored = 0
        for relocation in masks:
            if relocation.start <= index < relocation.end:
                ignored |= relocation.mask_bytes[index - relocation.start]
        if (expected ^ actual) & (~ignored & 0xff):
            differences.append(index)
    return differences


def normalized_pair_equal(left: ELFFile, left_name: str, right: ELFFile, right_name: str) -> bool:
    """Compare two relocatables while masking the union of precise MIPS fields."""
    left_symbol, right_symbol = left.find_symbol(left_name), right.find_symbol(right_name)
    left_raw = left.symbol_bytes(left_symbol, left_symbol.size)
    right_raw = right.symbol_bytes(right_symbol, right_symbol.size)
    if len(left_raw) != len(right_raw):
        return False
    ignored = bytearray(len(left_raw))
    for elf, symbol in ((left, left_symbol), (right, right_symbol)):
        for relocation in elf.relocation_masks(symbol, 4):
            for index, mask in enumerate(relocation.mask_bytes):
                ignored[relocation.start + index] |= mask
    return all(((a ^ b) & (~ignored[index] & 0xff)) == 0
               for index, (a, b) in enumerate(zip(left_raw, right_raw)))


def listing_bytes(path: Path, address: int, size: int) -> bytes:
    match = re.search(
        rf"^===== 0x{address:08x}.*?\n(.*?)(?=^=====|\Z)",
        path.read_text(), re.M | re.S,
    )
    if not match:
        raise SystemExit(f"target listing entry missing: {address:#x}")
    raw = b"".join(bytes.fromhex(" ".join(row.group(1).split())) for row in re.finditer(
        r"^\s*[0-9a-f]+:\s+((?:[0-9a-f]{2}\s+){4})", match.group(1), re.M,
    ))
    if len(raw) != size:
        raise SystemExit(f"target listing geometry drift: {address:#x}")
    return raw


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
    relations = list(csv.DictReader(RELATIONS.open(newline=""), delimiter="\t"))
    if len(relations) != 38 or len({(row["module"], row["symbol"]) for row in relations}) != 38:
        raise SystemExit("pending target relation roster drift")
    relation_by_key = {(row["module"], row["symbol"]): row for row in relations}
    selected = [row for row in rows if (row["module"], row["symbol"]) in relation_by_key]
    archived = [row for row in selected if row["module"] in MODULES]
    clip = [row for row in selected if row["module"] == "clip"]
    if (len(selected) != 38 or len(archived) != 36 or len(clip) != 2
            or Counter(row["status"] for row in selected) != {
                "proved-frozen-normalized-target": 9,
                "proved-frozen-linked-target": 2,
                "retained-source-with-frozen-target-variant": 13,
                "retained-source-with-frozen-target-aggregate": 12,
                "retained-source-inlined-into-frozen-target-variant": 1,
                "retained-source-with-frozen-target-noop-variant": 1,
            }
            or Counter(row["module"] for row in archived) != {
                "dsp1": 5, "memmap": 8, "soundux": 6,
                "snapshot": 4, "snaporig": 1, "spc7110": 12,
            }):
        raise SystemExit("zero-gap target relation classification drift")
    for row in selected:
        relation = relation_by_key[row["module"], row["symbol"]]
        if (int(relation["historical_size"]) != int(row["size"])
                or relation["relation"] not in STATUS_BY_RELATION
                or row["status"] != STATUS_BY_RELATION[relation["relation"]]):
            raise SystemExit("target relation/census drift: " + row["symbol"])
    for row in archived:
        expected = "src/snes9x/historical_reference/" + MODULES[row["module"]][0]
        if row["source_file"] != expected or row["evidence"] != "tools/run-native-pending-archive-source-recovery.py":
            raise SystemExit("historical source census provenance drift: " + row["symbol"])
    if {(row["symbol"], row["source_file"], row["size"]) for row in clip} != {
        ("_Z10IntComparePKvS0_", "src/snes9x/native_clip.cpp", "40"),
        ("_Z11BandComparePKvS0_", "src/snes9x/native_clip.cpp", "40"),
    }:
        raise SystemExit("checked-in clip helper source closure drift")
    forbidden_tokens = ("no-complete-target", "blocked-link", "source-only-helper",
                        "no-complete-frozen", "blocked-after-full-link")
    if any(any(token in row["status"] for token in forbidden_tokens) for row in rows):
        raise SystemExit("legacy target-witness gap remains in native census")

    windows = json.loads(CODE_WINDOWS.read_text())["result"]["selected_sources"]
    window_by_name = {window["name"]: window for window in windows}
    for relation in relations:
        window = window_by_name.get(relation["witness"])
        if window is None:
            raise SystemExit("missing frozen target window: " + relation["witness"])
        window_address, window_size = int(relation["window_address"], 0), int(relation["window_size"])
        target_address, target_size = int(relation["target_address"], 0), int(relation["target_size"])
        if (window["address"], window["size"]) != (window_address, window_size):
            raise SystemExit("frozen target window geometry drift: " + relation["symbol"])
        if not (window_address <= target_address and target_address + target_size <= window_address + window_size):
            raise SystemExit("target relation lies outside witness: " + relation["symbol"])
        evidence = ROOT / relation["evidence"]
        if not evidence.is_file():
            raise SystemExit("target relation evidence missing: " + relation["evidence"])
        if relation["evidence"].startswith("analysis/matching/"):
            evidence_rows = list(csv.DictReader(evidence.open(newline=""), delimiter="\t"))
            matches = [entry for entry in evidence_rows
                       if int(entry["address"], 0) == window_address and entry.get("result") == "MATCH"]
            if len(matches) != 1 or matches[0].get("differing_bytes") != "0" or matches[0].get("normalized_equal") != "True":
                raise SystemExit("frozen matching evidence drift: " + relation["symbol"])

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

    clip_ledger = list(csv.DictReader(
        (ROOT / "analysis/functions/native_clip_exact_4652.tsv").open(newline=""), delimiter="\t",
    ))
    clip_rows = {row["symbol"]: row for row in clip_ledger}
    for row in clip:
        proof = clip_rows.get(row["symbol"])
        if (proof is None or proof["size"] != row["size"]
                or proof["proof_level"] != "complete-frozen-linked-target"
                or proof["linked_sha256"] != "9792f62b10067940165eb737cce03fc766349712464ae64b8aa7f2dca8b4fc52"):
            raise SystemExit("complete clip helper target proof drift: " + row["symbol"])

    # Five checked-in 1.41-1 bodies reproduce the exact relocatable functions
    # used by their frozen normalized-target proofs.
    raw_exact = {
        ("dsp1", "_Z7DSPOp28v"): "tsv_0012e04c__Z7DSPOp28v",
        ("memmap", "_ZN7CMemory7LoadROMEPKc"): "tsv_001513bc__ZN7CMemory7LoadROMEPKc",
        ("memmap", "_ZN7CMemory7InitROMEh"): "tsv_001522d8__ZN7CMemory7InitROMEh",
        ("memmap", "_ZN7CMemory13ApplyROMFixesEv"): "tsv_00156c60__ZN7CMemory13ApplyROMFixesEv",
        ("soundux", "_Z11DecodeBlockP7Channel"): "tsv_00174fe4__Z11DecodeBlockP7Channel",
    }
    for (module, symbol_name), witness_name in raw_exact.items():
        symbol = compiled[module].find_symbol(symbol_name)
        window = window_by_name[witness_name]
        if digest(compiled[module].symbol_bytes(symbol, symbol.size)) != window["raw_sha256"]:
            raise SystemExit("normalized-target historical body drift: " + symbol_name)

    # Rebuild the public exact assembly used by two small historical wrappers
    # and by the two same-geometry variants whose mismatch is intentional.
    stage_path = BUILD / "stage3p_exact.o"
    run([cpu.CXX.with_name("ee-gcc"), "-G0", "-EL", "-c",
         ROOT / "matching/candidates/stage3p_code_residual_exact.S", "-o", stage_path])
    stage = ELFFile(stage_path)
    stage_symbols = {
        "stage3p_tail_gap_00177e5c": ("soundux", "_Z18S9xSetSoundControli", True),
        "stage3p_tail_gap_00171230": ("snapshot", "S9xLoadSnapshot", True),
        "stage3p_dsp2_set_byte": ("dsp1", "_Z11DSP2SetByteht", False),
        "stage3p_memory_safe": ("memmap", "_ZN7CMemory4SafeEPKc", False),
    }
    for target_name, (module, source_name, should_match) in stage_symbols.items():
        target_symbol = stage.find_symbol(target_name)
        target = stage.symbol_bytes(target_symbol, target_symbol.size)
        window = window_by_name[target_name]
        if digest(target) != window["raw_sha256"] or len(target) != window["size"]:
            raise SystemExit("public exact target assembly drift: " + target_name)
        matches = not non_relocation_differences(target, compiled[module], source_name)
        if matches != should_match:
            raise SystemExit("historical/target relation drift: " + source_name)

    # FreezeBlock is instruction-equivalent to the target-proved recovered C
    # body.  Both relocation sets are masked precisely; no broad byte masking.
    freeze_path = BUILD / "freeze_block_target_proved.o"
    run([cpu.CXX.with_name("ee-gcc"), "-EL", "-mno-abicalls", "-march=r5900", "-mtune=r5900",
         "-DPS2_EE", "-D_EE", "-DLSB_FIRST", "-DALIGN_DWORD", "-DCODE_PLATFORM=3",
         "-Iinclude", "-Iinclude/ee_stage1_compat", "-Imatching/ee_abi_compat", "-w",
         "-O2", "-fstrict-aliasing", "-fno-common", "-c",
         ROOT / "matching/candidates/hunt1041_v48.c", "-o", freeze_path])
    freeze = ELFFile(freeze_path)
    if not normalized_pair_equal(compiled["snapshot"], "_Z11FreezeBlockPvPcPhi",
                                 freeze, "v48_00172174"):
        raise SystemExit("historical FreezeBlock/target-proved recovered body drift")

    # LoadOrigSnapshot has a complete committed 96-byte target listing.
    orig_target = listing_bytes(
        ROOT / "analysis/functions/progress11_short_targets.asm", 0x0017022c, 96,
    )
    if non_relocation_differences(orig_target, compiled["snaporig"], "S9xLoadOrigSnapshot"):
        raise SystemExit("historical S9xLoadOrigSnapshot target mismatch")

    tile_listing = (ROOT / "analysis/functions/tile_init_and_helpers_00183000.asm").read_text()
    if not re.search(r"^\s*18339c: 08 00 e0 03.*\n\s*1833a0: 00 00 00 00", tile_listing, re.M):
        raise SystemExit("frozen SPC7110 logging no-op target drift")
    memmap_source = (ROOT / "src/snes9x/historical_reference/MEMMAP.CPP").read_text()
    if memmap_source.count("ReadInt (patch_file") != 4 or "static long ReadInt" not in memmap_source:
        raise SystemExit("historical ReadInt inline-source relation drift")

    canonical = (ROOT / "analysis/source_tree/translation_units.tsv").read_text()
    if "src/snes9x/historical_reference/" in canonical:
        raise SystemExit("unproved historical reference entered canonical link tree")
    print("native pending target relations: 38/38 mapped; 9 normalized exact + 2 fully linked exact; 27 frozen variants/aggregates/inlines; 0 source or target-relation gaps")


if __name__ == "__main__":
    main()
