#!/usr/bin/env python3
"""Prove all 584 native C4 wave instructions and the 80-byte bitmap table.

The isolated compiler profile and packed PS2 load forms are independently pinned
by V78. Four verified table LO16 addends change when isolating the native table;
original placement reproduces the complete frozen code and data bytes. No
private-image recapture or proof of unselected historical context is inferred.
"""
from __future__ import annotations
import csv
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from build_source_tree import SOURCE_COMPILER_PROFILES, SOURCE_FIXED_FLAGS
from build_ee_gcc_regalloc_profile import PROFILE_NAME, build_profile
from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-c4-wave-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
SOURCE = "src/snes9x/native_c4_wave.cpp"
EVIDENCE = ROOT / "analysis/functions/native_c4_wave_exact_584.tsv"
SYMBOL = "_Z14C4BitPlaneWavev"
TABLE = "_ZZ14C4BitPlaneWavevE7bmpdata"
TABLE_SHA = "7ae57201960fa77d80a6142302bc4671f721ac72b56efbd9f6e71d27cdb3cd6f"
SPECS = [{
    "address": "0x0010d2a8", "symbol": SYMBOL, "source_file": SOURCE,
    "size": "584", "relocations": "18",
    "historical_raw_sha256": "a69e3169d452a9c7e52e3e8b14f2b3720dbfbd3b5aadc10c9a974989b5994d7f",
    "isolated_raw_sha256": "bbd59ac7df5f2b5aed1aa08a744c24aaf56a034c6f6fa90395f8aa25004a3e10",
    "normalized_sha256": "3953c74373969b824418bf472d5e3c84cf2a1289cd41d9a91d0b8b06ef5f1cb8",
    "linked_sha256": "238f6f0603598494941793416566184c83843985afe3bd3913b0df34b83e4c00",
    "target_sha256": "238f6f0603598494941793416566184c83843985afe3bd3913b0df34b83e4c00",
    "proof_level": "provider-linked-target", "compiler_profile": "mips-local-t5-before-t4",
    "evidence": "analysis/matching/hunt1041-v78-validated-c4bit-1.tsv",
}]


def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command)) + "\n" + result.stdout[-10000:])
    return result.stdout


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def section_bytes(elf, name):
    section = next(s for s in elf.sections if s.name == name)
    if section.type == 8:
        raise SystemExit("cannot read zero-fill section as stored bytes")
    return elf.data[section.offset:section.offset + section.size]


def normalized(elf, symbol):
    raw = bytearray(elf.symbol_bytes(symbol, symbol.size))
    for relocation in elf.relocation_masks(symbol, symbol.size):
        if not relocation.known:
            raise SystemExit("unknown relocation: " + symbol.name)
        for index, mask in enumerate(relocation.mask_bytes):
            raw[relocation.start + index] &= 255 ^ mask
    return bytes(raw)


def place(name, path, providers, sections):
    script = BUILD / (name + ".ld")
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in providers.items())
        + "SECTIONS {\n" + sections
        + "\n/DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    target = BUILD / (name + ".elf")
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, path, "-o", target])
    return ELFFile(target)


def main():
    if run([CXX, "-dumpmachine"]).strip() != "ee" or run([CXX, "-dumpversion"]).strip() != "3.2.2":
        raise SystemExit("wrong historical compiler")
    flags = SOURCE_FIXED_FLAGS[SOURCE]
    if (flags != SOURCE_FIXED_FLAGS["src/snes9x/native_c4_raster.cpp"]
            or SOURCE_COMPILER_PROFILES != {SOURCE: PROFILE_NAME}
            or PROFILE_NAME != SPECS[0]["compiler_profile"]):
        raise SystemExit("native C4 wave compiler profile/opt-in scope drift")
    cc1plus = CXX.parents[1] / "lib/gcc-lib/ee/3.2.2/cc1plus"
    original_compiler_sha = digest(cc1plus.read_bytes())
    profile = build_profile(CXX)
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v78_c4bit as recipe
    # V78 checks the official archive, packed-access patches and private
    # profile metadata without reading or changing the private target image.
    built = recipe.build_object(CXX)
    if digest(cc1plus.read_bytes()) != original_compiler_sha:
        raise SystemExit("canonical C++ compiler changed during isolated profile construction")
    historical = ELFFile(built.path)
    BUILD.mkdir(parents=True, exist_ok=True)
    path = BUILD / "native_c4_wave.o"
    run([CXX, f"-B{profile.as_posix()}/", *flags, "-c", ROOT / SOURCE, "-o", path])
    local = ELFFile(path)
    functions = [s for s in local.symbols if s.info & 15 == 2 and s.size]
    writable = [s for s in local.symbols if s.info & 15 == 1 and s.size and 0 < s.section_index < len(local.sections)
                and local.sections[s.section_index].name.startswith((".data", ".bss"))]
    if (len(functions) != 1 or functions[0].name != SYMBOL or functions[0].size != 584
            or functions[0].info >> 4 != 1 or len(writable) != 1 or writable[0].name != TABLE
            or writable[0].size != 80 or writable[0].value != 0 or writable[0].info >> 4 != 0
            or {s.name for s in local.symbols if s.section_index == 0 and s.name} != {"DAT_0034e2b0"}):
        raise SystemExit("native C4 wave code/table/shared-storage inventory drift")
    original_table = historical.find_symbol(TABLE)
    old_data = section_bytes(historical, ".data")
    prefix_sha = "0d02e06b760365d1028fec081e17082fbb54da13f85599069bfa10a20ea96764"
    rows = json.loads((ROOT / "analysis/link_identity/window35_data.json").read_text())["source_sections"]
    table_window = [r for r in rows if r["name"] == "c4emu_tables"]
    if (original_table.value != 0x30 or original_table.size != 80
            or digest(old_data[:2176]) != prefix_sha or len(table_window) != 1
            or any(table_window[0][k] != v for k, v in
                {"address": 0x3359d0, "source_offset": 0, "size": 2176, "relocations": 0,
                 "raw_sha256": prefix_sha, "linked_sha256": prefix_sha}.items())):
        raise SystemExit("independent frozen C4 data-table window drift")
    if (section_bytes(local, ".data") != old_data[0x30:0x80]
            or digest(section_bytes(local, ".data")) != TABLE_SHA):
        raise SystemExit("complete 80-byte bitmap offset table drift")
    anchors = json.loads((ROOT / "analysis/link_identity/historical_data.json").read_text())["global_anchors"]
    memory = [r for r in anchors if r["symbol"] == "Memory"]
    if len(memory) != 1 or memory[0]["address"] != 0x34e2b0 or memory[0]["source_offset"] != 37456:
        raise SystemExit("shared original C4 RAM owner drift")
    linked = place("native", path, {"DAT_0034e2b0": 0x34e2b0},
        ".text 0x10d2a8 : { *(.text._Z14C4BitPlaneWavev) }\n"
        ".data 0x335a00 : { *(.data*) }\n/DISCARD/ : { *(.bss*) *(COMMON) }")
    old = historical.find_symbol(SYMBOL)
    # The full historical module supplies the independent selected code and
    # table placement only. Its other functions are comparison-only context.
    bindings = {"Memory": 0x34e2b0}
    for symbol in historical.symbols:
        if symbol.section_index == 0 and symbol.name and symbol.name not in bindings:
            bindings[symbol.name] = 0
    reference = place("reference", built.path, bindings,
        f".text {0x10d2a8 - old.value:#x} : {{ *(.text) }}\n"
        ".data 0x3359d0 : { *(.data*) }\n.rodata 0x40000000 : { *(.rodata*) }\n"
        ".bss 0x50000000 : { *(.bss*) *(COMMON) }")
    isolated, final, ref = local.find_symbol(SYMBOL), linked.find_symbol(SYMBOL), reference.find_symbol(SYMBOL)
    old_raw, raw, final_raw = historical.symbol_bytes(old, 584), local.symbol_bytes(isolated, 584), linked.symbol_bytes(final, 584)
    spec = SPECS[0]
    windows = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    selected = [r for r in windows if r["address"] == 0x10d2a8 and r["symbol"] == SYMBOL]
    if (len(selected) != 1 or any(selected[0][k] != v for k, v in
            {"size": 584, "new_bytes": 584, "relocations": 18,
             "raw_sha256": spec["historical_raw_sha256"]}.items())
            or any(s.size != 584 for s in (old, isolated, final, ref)) or final.value != 0x10d2a8 or ref.value != final.value
            or digest(old_raw) != spec["historical_raw_sha256"] or digest(raw) != spec["isolated_raw_sha256"]
            or normalized(local, isolated) != normalized(historical, old)
            or digest(normalized(local, isolated)) != spec["normalized_sha256"]
            or local.relocation_masks(isolated, 584) != historical.relocation_masks(old, 584)
            or len(local.relocation_ranges(isolated, 584)) != 18
            or digest(final_raw) != spec["linked_sha256"] or digest(final_raw) != spec["target_sha256"]
            or reference.symbol_bytes(ref, 584) != final_raw):
        raise SystemExit("native C4 wave instructions/full target placement drift")
    offsets = [0x54, 0xbc, 0x15c, 0x1c4]
    if ([i for i in range(584) if old_raw[i] != raw[i]] != offsets
            or any(old_raw[i:i + 2] != b"\x30\0" or raw[i:i + 2] != b"\0\0" for i in offsets)
            or any(not any(r.start == i and r.relocation_type == 6 for r in local.relocation_masks(isolated, 584)) for i in offsets)):
        raise SystemExit("unbounded native wave table relocation addend drift")
    final_table = linked.find_symbol(TABLE)
    if (final_table.value != 0x335a00 or final_table.size != 80
            or digest(linked.symbol_bytes(final_table, 80)) != TABLE_SHA):
        raise SystemExit("complete linked bitmap table target data drift")
    with (ROOT / spec["evidence"]).open(newline="") as stream:
        frozen = list(csv.DictReader(stream, delimiter="\t"))
    if len(frozen) != 1 or any(frozen[0][k] != v for k, v in
            {"address": spec["address"], "result": "MATCH", "differing_bytes": "0",
             "normalized_equal": "True", "unknown_relocations": "", "object_symbol": SYMBOL,
             "object_size": "584", "relocation_count": "18", "target_span_sha256": spec["target_sha256"]}.items()):
        raise SystemExit("complete frozen C4 wave target witness drift")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native C4 wave evidence ledger drift")
    print("native C4 wave: MATCH bytes=584/584 proof=provider-linked-target; "
          "80 complete target data bytes; four verified table addends; isolated compiler profile")


if __name__ == "__main__":
    main()
