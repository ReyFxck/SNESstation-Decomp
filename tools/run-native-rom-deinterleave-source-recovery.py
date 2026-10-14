#!/usr/bin/env python3
"""Prove native mode-2 ROM conversion and the real three-argument PS2 callback.

The only isolated/historical raw difference is the verified diagnostic string
LO16 addend. Both placements reproduce all 552 frozen target instruction bytes.
The 56-byte literal comes from the frozen MEMMAP rodata prefix. Unselected
historical methods are comparison-only; no private-image recapture is claimed.
"""
from __future__ import annotations
import csv
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from build_source_tree import SOURCE_FIXED_FLAGS
from compare_elf_functions import ELFFile

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build/matching/native-rom-deinterleave-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
CC = ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
SOURCE = "src/snes9x/native_rom_deinterleave.cpp"
EVIDENCE = ROOT / "analysis/functions/native_rom_deinterleave_exact_552.tsv"
PROVIDERS = {
    "DAT_003454e0": 0x3454e0, "DAT_0034e2b0": 0x34e2b0,
    "S9xReset": 0x115b58, "_ZN7CMemory7InitROMEh": 0x1522d8,
    "__gxx_personality_v0": 0x1a9728,
    "malloc": 0x19e4b4, "free": 0x19e784, "memmove": 0x19c4a0,
}
SPECS = [
    {"address": "0x001056b0", "symbol": "S9xMessage", "source_file": SOURCE,
     "size": "8", "relocations": "0",
     "historical_raw_sha256": "6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1",
     "isolated_raw_sha256": "6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1",
     "normalized_sha256": "6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1",
     "linked_sha256": "6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1",
     "target_sha256": "6d64edf91449c1b17746c1ef18afa2eb25c70bdf1322ab3df5a2630993b7e2f1",
     "proof_level": "provider-linked-target", "reference_symbol": "snes_leaf_001056b0",
     "evidence": "analysis/matching/hunt1000plus-v41-validated-28.tsv"},
    {"address": "0x001520b8", "symbol": "S9xDeinterleaveMode2", "source_file": SOURCE,
     "size": "544", "relocations": "18",
     "historical_raw_sha256": "d6582de42a88bc24cc5016252e5cbf7d98861cde022fdbbad562bdf954315304",
     "isolated_raw_sha256": "f85ff7069c988960e45cd130a44d349cbf030e2c116e9fe48782f4142231cc41",
     "normalized_sha256": "f85ff7069c988960e45cd130a44d349cbf030e2c116e9fe48782f4142231cc41",
     "linked_sha256": "2cef082042d2e4140084afade3d05d1190c4a2e2829e0ff786b9c0e4cf5a3457",
     "target_sha256": "2cef082042d2e4140084afade3d05d1190c4a2e2829e0ff786b9c0e4cf5a3457",
     "proof_level": "provider-linked-target", "reference_symbol": "S9xDeinterleaveMode2",
     "evidence": "analysis/matching/hunt1041-v51-validated-16.tsv"},
]


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


def witness(relative, address, symbol, size):
    with (ROOT / relative).open(newline="") as stream:
        rows = [r for r in csv.DictReader(stream, delimiter="\t") if r["address"] == address]
    if (len(rows) != 1 or any(rows[0][k] != v for k, v in
            {"result": "MATCH", "differing_bytes": "0", "normalized_equal": "True",
             "unknown_relocations": "", "object_symbol": symbol, "object_size": str(size)}.items())):
        raise SystemExit("frozen native ROM witness drift: " + symbol)
    return rows[0]


def place(name, providers, sections):
    script = BUILD / (name + ".ld")
    script.write_text("".join(f"PROVIDE({n} = {v:#x});\n" for n, v in providers.items())
        + "SECTIONS {\n" + sections
        + "\n/DISCARD/ : { *(.reginfo) *(.pdr) *(.mdebug*) *(.comment) *(.note*) *(.eh_frame*) } }\n")
    path = BUILD / (name + ".elf")
    run([CXX.with_name("ee-ld"), "-EL", "-T", script, BUILD / (name + ".o"), "-o", path])
    return ELFFile(path)


def main():
    for compiler in (CC, CXX):
        if run([compiler, "-dumpmachine"]).strip() != "ee" or run([compiler, "-dumpversion"]).strip() != "3.2.2":
            raise SystemExit("wrong historical compiler")
    flags = SOURCE_FIXED_FLAGS[SOURCE]
    if flags != SOURCE_FIXED_FLAGS["src/snes9x/native_controllers.cpp"] or flags[-1] != "-ffunction-sections":
        raise SystemExit("native ROM compiler profile drift")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v52_closure as recipe
    recipe.v47.ensure_git_commit(recipe.v47.PS2DEV, recipe.v47.PS2DEV_REPO, recipe.v47.PS2DEV_COMMIT)
    BUILD.mkdir(parents=True, exist_ok=True)
    saved = recipe.BUILD
    try:
        recipe.BUILD = BUILD / "profile"
        root, _upstream, layout = recipe.prepare_snes_layout()
        recipe.patch_sources(layout)
        recipe.write_compat_headers(BUILD / "compat")
    finally:
        recipe.BUILD = saved
    newlib = recipe.v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    includes = ["-I" + str(p) for p in (BUILD / "compat", newlib, layout, layout / "unzip", root / "zlib")]
    historical = {}
    for source in ("MEMMAP.CPP", "GLOBALS.CPP"):
        path = BUILD / (source + ".historical.o")
        run([CXX, *flags[:-1], "-DZLIB", *includes, "-x", "c++", "-c", layout / source, "-o", path])
        historical[source] = ELFFile(path)
    body = "void snes_leaf_001056b0(void) {}"
    if body not in (ROOT / "src/ps2/address_leaf_recovered.c").read_text():
        raise SystemExit("independent PS2 message leaf source drift")
    leaf = BUILD / "message-leaf.c"
    leaf.write_text(body + "\n")
    run([CC, *flags[:-1], "-O2", "-c", leaf, "-o", BUILD / "message-leaf.o"])
    independent_leaf = ELFFile(BUILD / "message-leaf.o")
    path = BUILD / "native_rom_deinterleave.o"
    run([CXX, *flags, "-c", ROOT / SOURCE, "-o", path])
    local = ELFFile(path)
    functions = [s for s in local.symbols if s.info & 15 == 2 and s.size]
    writable = [s for s in local.symbols if s.info & 15 == 1 and s.size and 0 < s.section_index < len(local.sections)
                and local.sections[s.section_index].name.startswith((".data", ".bss"))]
    if ({s.name for s in functions} != {r["symbol"] for r in SPECS}
            or sum(s.size for s in functions) != 552 or any(s.info >> 4 != 1 for s in functions) or writable
            or {s.name for s in local.symbols if s.section_index == 0 and s.name} != set(PROVIDERS)):
        raise SystemExit("native ROM function/shared-storage/provider inventory drift")
    state = historical["GLOBALS.CPP"]
    state_sha = "8cfe2f17d5cd6b3198a74b79ed5edbf4508b3b578b56aaea228e30d491c90490"
    owners = json.loads((ROOT / "analysis/link_identity/historical_data.json").read_text())["owners"]
    owner = [r for r in owners if r["unit"] == "globals" and r["symbol"] == "@section:.data"]
    if (len(section_bytes(state, ".data")) != 0xaf848 or digest(section_bytes(state, ".data")) != state_sha
            or len(owner) != 1 or any(owner[0][k] != v for k, v in
                {"address": 0x345060, "size": 0xaf848, "source_offset": 0, "sha256": state_sha}.items())):
        raise SystemExit("complete frozen GLOBALS state digest drift")
    for name, address, size in (("Settings", 0x3454e0, 328), ("Memory", 0x34e2b0, 54404)):
        symbol = state.find_symbol(name)
        if symbol.value + 0x345060 != address or symbol.size != size:
            raise SystemExit("native ROM state geometry drift: " + name)
    original = historical["MEMMAP.CPP"]
    windows = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    selected = [r for r in windows if r["address"] == 0x1520b8 and r["symbol"] == "S9xDeinterleaveMode2"]
    if len(selected) != 1 or any(selected[0][k] != v for k, v in
            {"size": 544, "new_bytes": 544, "relocations": 18,
             "raw_sha256": SPECS[1]["historical_raw_sha256"]}.items()):
        raise SystemExit("complete frozen mode-2 source slice drift")
    rodata = section_bytes(original, ".rodata")
    rodata_sha = "9137276f30f4eff45412264c0a20379b487f2114b4f037a3aa9ed31ba4816c63"
    prefix_sha = "d77ceaa56edfbe58df4448df1c1cf77ef7295707acafda54f47e012f6eedc7be"
    rows = json.loads((ROOT / "analysis/link_identity/window11_rodata.json").read_text())["source_sections"]
    prefix = [r for r in rows if r["name"] == "memmap_prefix"]
    if (len(rodata) != 4352 or digest(rodata) != rodata_sha or digest(rodata[:2848]) != prefix_sha
            or len(prefix) != 1 or any(prefix[0][k] != v for k, v in
                {"address": 0x1b63d8, "source_offset": 0, "full_size": 4352, "size": 2848,
                 "relocations": 0, "full_sha256": rodata_sha,
                 "raw_sha256": prefix_sha, "linked_sha256": prefix_sha}.items())):
        raise SystemExit("independent frozen MEMMAP diagnostic data window drift")
    literal = section_bytes(local, ".rodata")
    if (len(literal) != 56 or literal != rodata[0xe8:0xe8 + 56]
            or digest(literal) != "e97035245bf08a9e1f7872c7138ab04e2264f80877b43769d586a37b4ace4747"):
        raise SystemExit("complete 56-byte diagnostic literal drift")
    linked = place("native_rom_deinterleave", PROVIDERS,
        ".text.mode 0x1520b8 : { *(.text.S9xDeinterleaveMode2) }\n"
        ".text.message 0x1056b0 : { *(.text.S9xMessage) }\n"
        ".rodata 0x1b64c0 : { *(.rodata*) }\n"
        "/DISCARD/ : { *(.data*) *(.bss*) *(COMMON) }")
    bindings = {n.replace("DAT_003454e0", "Settings").replace("DAT_0034e2b0", "Memory"): v
                for n, v in PROVIDERS.items() if n != "_ZN7CMemory7InitROMEh"}
    bindings["S9xMessage"] = 0x1056b0
    # Only selected mode-2 code is proved/executed by this recovery. Every
    # selected callee has its target address; other methods remain context.
    for symbol in original.symbols:
        if symbol.section_index == 0 and symbol.name and symbol.name not in bindings:
            bindings[symbol.name] = 0
    reference = place("MEMMAP.CPP.historical", bindings,
        ".text 0x150a74 : { *(.text) }\n.rodata 0x1b63d8 : { *(.rodata*) }\n"
        ".data 0x40000000 : { *(.data*) }\n.bss 0x50000000 : { *(.bss*) *(COMMON) }")
    init = original.find_symbol("_ZN7CMemory7InitROMEh")
    init_witness = witness("analysis/matching/hunt1041-v52-validated-17.tsv", "0x001522d8", init.name, 4220)
    if (init.size != 4220 or init.value + 0x150a74 != PROVIDERS[init.name]
            or init_witness["target_span_sha256"] != "5c6072d8d1892b2346877a2820d67d29a2040a333951ad704b9301380cb47a8c"):
        raise SystemExit("native InitROM callee ABI/address witness drift")
    if section_bytes(linked, ".rodata") != literal:
        raise SystemExit("linked diagnostic data drift")
    for spec in SPECS:
        name, size = spec["symbol"], int(spec["size"])
        old_elf = independent_leaf if name == "S9xMessage" else original
        old, isolated, final = old_elf.find_symbol(spec["reference_symbol"]), local.find_symbol(name), linked.find_symbol(name)
        old_raw, raw, final_raw = old_elf.symbol_bytes(old, size), local.symbol_bytes(isolated, size), linked.symbol_bytes(final, size)
        if (any(s.size != size for s in (old, isolated, final)) or final.value != int(spec["address"], 0)
                or digest(old_raw) != spec["historical_raw_sha256"] or digest(raw) != spec["isolated_raw_sha256"]
                or normalized(local, isolated) != normalized(old_elf, old)
                or digest(normalized(local, isolated)) != spec["normalized_sha256"]
                or local.relocation_masks(isolated, size) != old_elf.relocation_masks(old, size)
                or len(local.relocation_ranges(isolated, size)) != int(spec["relocations"])
                or digest(final_raw) != spec["linked_sha256"] or digest(final_raw) != spec["target_sha256"]):
            raise SystemExit("native ROM code/full target digest drift: " + name)
        frozen = witness(spec["evidence"], spec["address"], spec["reference_symbol"], size)
        if name == "S9xMessage":
            if raw != old_raw or frozen["raw_equal"] != "True":
                raise SystemExit("complete message leaf target witness drift")
        else:
            differences = [i for i in range(size) if old_raw[i] != raw[i]]
            if (differences != [0x50] or old_raw[0x50:0x54] != bytes.fromhex("e800c624")
                    or raw[0x50:0x54] != bytes.fromhex("0000c624")
                    or not any(r.start == 0x50 and r.relocation_type == 6 for r in local.relocation_masks(isolated, size))
                    or frozen["target_span_sha256"] != spec["target_sha256"]):
                raise SystemExit("unbounded isolated source/string addend drift")
            ref = reference.find_symbol(name)
            if ref.value != final.value or reference.symbol_bytes(ref, size) != final_raw:
                raise SystemExit("complete selected historical placement drift")
        print(f"native ROM: MATCH {name} bytes={size}/{size} proof=provider-linked-target")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native ROM evidence ledger drift")
    print("native ROM proof: 2/2 routines; 552 complete target instruction bytes; "
          "56 complete target data bytes; verified string addend; shared native state retained")


if __name__ == "__main__":
    main()
