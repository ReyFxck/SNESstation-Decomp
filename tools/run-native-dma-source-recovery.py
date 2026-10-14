#!/usr/bin/env python3
"""Prove native general DMA and the DMA-local OAM/CGRAM helpers.

All 7668 bytes match fully linked original historical instructions. Both audited
DMA partitions and CGRAM additionally match 6920 complete frozen target bytes.
Preserves native shared state, allocation ABI, 64-bit cycles and all 24 jump-table
bytes. Unselected historical context is never executed or promoted. This public
proof does not rerun a private ELF or establish a new whole-image result.
"""
from __future__ import annotations
import csv
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from compare_elf_functions import ELFFile
from build_source_tree import SOURCE_FIXED_FLAGS

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-dma-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
SOURCE = "src/snes9x/native_dma.cpp"
EVIDENCE = ROOT / "analysis/functions/native_dma_exact_7668.tsv"
FLAGS = SOURCE_FIXED_FLAGS[SOURCE]
PROVIDERS = {'DAT_00345498': 3429528,
 'DAT_003454e0': 3429600,
 'DAT_0034e2b0': 3465904,
 'DAT_0035b788': 3520392,
 'DAT_0035c268': 3523176,
 'DAT_0035d360': 3527520,
 'DAT_003f2eb0': 4140720,
 'S9xDoHBlankProcessing': 1139112,
 'S9xGetPPU': 1418736,
 'S9xSetPPU': 1413736,
 'S9xUpdateScreen': 1371220,
 '_Z10S9xSetBytehj': 1751296,
 '_Z14GetBasePointerj': 1754932,
 '_ZdaPv': 1741080,
 '_Znaj': 1744744,
 '__gxx_personality_v0': 0,
 'g_APU_003453b8': 3429304,
 'g_CPU_blob': 3429184,
 'g_S9xAPUCycles_003f44a8': 4146344,
 'g_S9xApuOpcodes_00411010': 4263952,
 'g_SA1_blob': 3431160,
 'g_s7r_blob': 4273416,
 'memcpy': 1688420}
LEGACY_NAMES = {'DAT_00345498': 'IAPU',
 'DAT_003454e0': 'Settings',
 'DAT_0034e2b0': 'Memory',
 'DAT_0035b788': 'PPU',
 'DAT_0035c268': 'IPPU',
 'DAT_0035d360': 'DMA',
 'DAT_003f2eb0': 'SignExtend',
 'g_APU_003453b8': 'APU',
 'g_CPU_blob': 'CPU',
 'g_S9xAPUCycles_003f44a8': 'S9xAPUCycles',
 'g_S9xApuOpcodes_00411010': 'S9xApuOpcodes',
 'g_SA1_blob': 'SA1',
 'g_s7r_blob': 's7r'}
SPECS = [{'address': '0x00129af4',
  'symbol': 'S9xDoDMA',
  'source_file': 'src/snes9x/native_dma.cpp',
  'size': '6388',
  'relocations': '211',
  'historical_raw_sha256': '79e3bd91bbd9c7d2a934df72091ab22c843165b2c39167e77efc026491f94c4d',
  'isolated_raw_sha256': 'd784901fd3f1b09e124790e58027f3117bef93cf58d62fecbd28047a4708c5e5',
  'normalized_sha256': '5e662ffbca2e0bb674ae6f7b221f0c8fc03c4fc2f314b27fee070433025bf526',
  'linked_sha256': '0fe1ffc35b9e8cba7568c865c5e96277fc67c28813bceb6c71dd8b585266bb22',
  'target_partitions': '0x00129af4:2316:a9ae7335a957dc56292cd234e7c49a9cc432b6f6e4c4f300225f30b4476b27a9;0x0012a400:4072:dd2102d989e6c94134d3272194d288cf3aa85ba2af8e5e336119fafec36f768e',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/matching/hunt1041-v72-validated-v53-6.tsv'},
 {'address': '0x0012ba5c',
  'symbol': '_Z13REGISTER_2104h',
  'source_file': 'src/snes9x/native_dma.cpp',
  'size': '748',
  'relocations': '18',
  'historical_raw_sha256': 'ad730aa8fce958e7d92903c9681dac0156c7c89352a67000ed6475faa9fe1153',
  'isolated_raw_sha256': 'ad730aa8fce958e7d92903c9681dac0156c7c89352a67000ed6475faa9fe1153',
  'normalized_sha256': '3a516e2967137fdf5edb46e16cf400610733d759b440c5cd2e4f9ea2dc2dc496',
  'linked_sha256': '9d24ac102fca82c1e51176fcb0c7c8bcb079aaff1885cef714a28d94cfa10661',
  'target_partitions': '',
  'proof_level': 'linked-historical-reference',
  'evidence': 'analysis/matching/hunt500plus-v33-validated-204.tsv'},
 {'address': '0x0012bd48',
  'symbol': '_Z13REGISTER_2122h',
  'source_file': 'src/snes9x/native_dma.cpp',
  'size': '532',
  'relocations': '34',
  'historical_raw_sha256': 'f7f88072110dfba0def6df371c932266ab2d9a123156db5dbf5d0b83d1ca663b',
  'isolated_raw_sha256': 'f7f88072110dfba0def6df371c932266ab2d9a123156db5dbf5d0b83d1ca663b',
  'normalized_sha256': '0e7cbfeadb3a2392b6023e27d6050f2924cf7084b0e8155935e17f0924be0a9a',
  'linked_sha256': 'b6f6d33c2c930eed15a938f4cf28a230404ace5cafffeaee081c5f054114a67c',
  'target_partitions': '0x0012bd48:532:b6f6d33c2c930eed15a938f4cf28a230404ace5cafffeaee081c5f054114a67c',
  'proof_level': 'provider-linked-target',
  'evidence': 'analysis/matching/hunt1041-v49-validated-20.tsv'}]

def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command))
                         + "\n" + result.stdout[-12000:])
    return result.stdout


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


def normalized(elf, symbol):
    raw = bytearray(elf.symbol_bytes(symbol, symbol.size))
    for relocation in elf.relocation_masks(symbol, symbol.size):
        if not relocation.known:
            raise SystemExit(f"unknown relocation: {symbol.name}")
        for index, mask in enumerate(relocation.mask_bytes):
            raw[relocation.start + index] &= 255 ^ mask
    return bytes(raw)


def section_bytes(elf, name):
    section = next(s for s in elf.sections if s.name == name)
    return elf.data[section.offset:section.offset + section.size]


def main():
    if (run([CXX, "-dumpmachine"]).strip() != "ee"
            or run([CXX, "-dumpversion"]).strip() != "3.2.2"):
        raise SystemExit("wrong historical compiler")
    if FLAGS[-1] != "-ffunction-sections":
        raise SystemExit("native DMA section profile drift")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v52_closure as recipe
    recipe.v47.ensure_git_commit(recipe.v47.PS2DEV, recipe.v47.PS2DEV_REPO,
                                 recipe.v47.PS2DEV_COMMIT)
    BUILD.mkdir(parents=True, exist_ok=True)
    old_build = recipe.BUILD
    try:
        recipe.BUILD = BUILD / "profile"
        root, upstream, layout = recipe.prepare_snes_layout()
        recipe.patch_sources(layout)
        recipe.write_compat_headers(BUILD / "compat")
    finally:
        recipe.BUILD = old_build
    newlib = recipe.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    includes = ["-I" + str(p) for p in
                (BUILD / "compat", newlib, layout, layout / "unzip", root / "zlib")]
    original_path, state_path = BUILD / "DMA.historical.o", BUILD / "GLOBALS.layout.o"
    for source, output in ((layout / "DMA.CPP", original_path),
                           (layout / "GLOBALS.CPP", state_path)):
        run([CXX, *FLAGS[:-1], "-DZLIB", *includes, "-x", "c++", "-c", source,
             "-o", output])
    obj = BUILD / "native_dma.o"
    run([CXX, *FLAGS, "-c", ROOT / SOURCE, "-o", obj])
    original, isolated, state = (ELFFile(p) for p in (original_path, obj, state_path))
    window_sha = "a1e981ed42e67cf6f29ec9aa638c21f45c531c536a629a4a06e0a4b053e04ed3"
    window = [r for r in json.loads((ROOT / "analysis/link_identity/code_windows.json")
              .read_text())["result"]["selected_sources"] if r["name"] == "dma"]
    if (len(section_bytes(original, ".text")) != 9320
            or digest(section_bytes(original, ".text")) != window_sha
            or len(window) != 1 or any(window[0][k] != v for k, v in
                {"address": 0x129af4, "size": 9320, "raw_sha256": window_sha,
                 "relocations": 351}.items())):
        raise SystemExit("frozen complete historical DMA code window drift")
    state_sha = "8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    owners = json.loads((ROOT / "analysis/link_identity/historical_data.json")
                        .read_text())["owners"]
    owner = [r for r in owners if r["unit"] == "globals" and r["symbol"] == "@section:.data"]
    if (len(section_bytes(state, ".data")) != 0xaf848
            or digest(section_bytes(state, ".data")) != state_sha
            or len(owner) != 1 or any(owner[0][k] != v for k, v in
                {"address": 0x345060, "size": 0xaf848, "source_offset": 0,
                 "sha256": state_sha}.items())):
        raise SystemExit("frozen complete DMA shared-state geometry drift")
    expected_sizes = {"CPU": 104, "Settings": 328, "PPU": 2780, "IPPU": 4340,
                      "Memory": 54404, "APU": 224, "IAPU": 60, "DMA": 176,
                      "SA1": 32856, "S9xAPUCycles": 1024, "SignExtend": 4}
    for name, historical_name in LEGACY_NAMES.items():
        if historical_name in ("s7r", "S9xApuOpcodes"):
            continue
        symbol = state.find_symbol(historical_name)
        if (symbol.value + 0x345060 != PROVIDERS[name]
                or symbol.size != expected_sizes[historical_name]):
            raise SystemExit("native DMA shared-provider geometry drift: " + name)
    functions = [s for s in isolated.symbols if s.info & 15 == 2 and s.size]
    writable = [s for s in isolated.symbols if s.info & 15 == 1 and s.size
                and 0 < s.section_index < len(isolated.sections)
                and isolated.sections[s.section_index].name.startswith((".data", ".bss"))]
    if ({s.name for s in functions} != {r["symbol"] for r in SPECS}
            or sum(s.size for s in functions) != 7668 or writable
            or len(section_bytes(isolated, ".rodata")) != 24):
        raise SystemExit("native DMA functions/jump-table/shared-storage inventory drift")
    imports = {s.name for s in isolated.symbols if s.section_index == 0 and s.name}
    if imports != set(PROVIDERS):
        raise SystemExit("native DMA provider ABI/import roster drift")
    script = BUILD / "native_dma.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in PROVIDERS.items())
        + "SECTIONS {\n" + "".join(
            f".text.{i} {int(r['address'], 0):#x} : {{ *(.text.{r['symbol']}) }}\n"
            for i, r in enumerate(SPECS))
        + ".rodata 0x1b1f20 : { *(.rodata*) } "
          "/DISCARD/ : { *(.data*) *(.bss*) *(COMMON) *(.reginfo) *(.pdr) "
          "*(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    linked_path = BUILD / "native_dma.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, obj, "-o", linked_path])
    linked = ELFFile(linked_path)
    bindings = {LEGACY_NAMES.get(name, name): address for name, address in PROVIDERS.items()}
    # Unselected historical context is comparison-only and never executed.
    for symbol in original.symbols:
        if symbol.section_index == 0 and symbol.name and symbol.name not in bindings:
            bindings[symbol.name] = 0
    script = BUILD / "DMA.historical.ld"
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in bindings.items())
        + "SECTIONS { .text 0x129af4 : { *(.text) } "
          ".rodata 0x1b1f20 : { *(.rodata*) } .data 0x33ccc8 : { *(.data*) } "
          ".bss 0x7fc00000 : { *(.bss*) *(COMMON) } /DISCARD/ : { *(.reginfo) "
          "*(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    reference_path = BUILD / "DMA.historical.elf"
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, original_path, "-o", reference_path])
    reference = ELFFile(reference_path)
    if section_bytes(linked, ".rodata") != section_bytes(reference, ".rodata")[:24]:
        raise SystemExit("complete native DMA jump-table bytes/placement drift")
    proved_target_bytes = 0
    for spec in SPECS:
        name, size, address = spec["symbol"], int(spec["size"]), int(spec["address"], 0)
        old, local, placed, ref = (elf.find_symbol(name)
                                  for elf in (original, isolated, linked, reference))
        raw = linked.symbol_bytes(placed, size)
        if (any(symbol.size != size for symbol in (old, local, placed, ref))
                or placed.value != address or ref.value != address
                or digest(original.symbol_bytes(old, size)) != spec["historical_raw_sha256"]
                or digest(isolated.symbol_bytes(local, size)) != spec["isolated_raw_sha256"]
                or normalized(original, old) != normalized(isolated, local)
                or digest(normalized(isolated, local)) != spec["normalized_sha256"]
                or len(isolated.relocation_ranges(local, size)) != int(spec["relocations"])
                or raw != reference.symbol_bytes(ref, size)
                or digest(raw) != spec["linked_sha256"]):
            raise SystemExit("native DMA full instructions/provider-linked reference drift: " + name)
        with (ROOT / spec["evidence"]).open(newline="") as stream:
            witnesses = {r["address"]: r for r in csv.DictReader(stream, delimiter="\t")}
        partitions = spec["target_partitions"].split(";") if spec["target_partitions"] else []
        checked = 0
        for partition in partitions:
            start_text, length_text, sha = partition.split(":")
            start, length = int(start_text, 0), int(length_text)
            witness = witnesses.get(start_text)
            offset = start - address
            if (offset != checked or length <= 0 or offset + length > size
                    or witness is None or witness["target_span_sha256"] != sha
                    or witness["object_size"] != length_text
                    or int(witness["object_offset"], 0) != offset
                    or digest(raw[offset:offset + length]) != sha):
                raise SystemExit("complete native DMA target partition drift: " + start_text)
            checked += length
        if ((partitions and (checked != size or spec["proof_level"] != "provider-linked-target"))
                or (not partitions and spec["proof_level"] != "linked-historical-reference")):
            raise SystemExit("unsupported native DMA proof claim: " + name)
        for start_text in ([p.split(":")[0] for p in partitions] or [spec["address"]]):
            witness = witnesses.get(start_text)
            if (witness is None or witness["result"] != "MATCH"
                    or witness["differing_bytes"] != "0" or witness["unknown_relocations"]
                    or witness["normalized_equal"] != "True"
                    or witness["object_symbol"] != name or witness.get("object_symbol_size", witness["object_size"]) != str(size)):
                raise SystemExit("frozen normalized native DMA witness drift: " + start_text)
        proved_target_bytes += checked
        print(f"native DMA: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native DMA source evidence ledger drift")
    if proved_target_bytes != 6920:
        raise SystemExit("native DMA complete target-proof coverage drift")
    print("native DMA source proof: 3/3 routines; 7668 linked historical bytes; "
          "6920 complete provider-linked target bytes; no duplicate shared state")


if __name__ == "__main__":
    main()
