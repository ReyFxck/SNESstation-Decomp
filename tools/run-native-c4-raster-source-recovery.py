#!/usr/bin/env python3
"""Prove native C4 line drawing and sprite disintegration with shared state.

All 1120 instruction bytes match the independently pinned historical module.
The 580-byte sprite routine additionally reproduces the complete frozen target
digest. Line drawing retains its frozen normalized target witness and complete
linked historical-reference comparison; no new private-image proof is inferred.
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
BUILD = ROOT / "build/matching/native-c4-raster-source-recovery"
CXX = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
SOURCE = "src/snes9x/native_c4_raster.cpp"
EVIDENCE = ROOT / "analysis/functions/native_c4_raster_exact_1120.tsv"
STATE = {
    "C4WFXVal": "_DAT_00335938", "C4WFYVal": "_DAT_0033593a",
    "C4WFZVal": "DAT_0033593c", "C4WFX2Val": "DAT_0033593e",
    "C4WFY2Val": "DAT_00335940", "C4WFDist": "DAT_00335942",
    "C4WFScale": "DAT_00335944",
}
PROVIDERS = {
    "DAT_0034e2b0": 0x34e2b0, "_DAT_00335938": 0x335938,
    "_DAT_0033593a": 0x33593a, "DAT_0033593c": 0x33593c,
    "DAT_0033593e": 0x33593e, "DAT_00335940": 0x335940,
    "DAT_00335942": 0x335942, "DAT_00335944": 0x335944,
    "C4TransfWireFrame2": 0x10bbcc, "C4CalcWireFrame": 0x10becc,
    "memset": 0x19c39c, "__gxx_personality_v0": 0x1a9728,
}
SPECS = [
    {"address": "0x0010cbb0", "symbol": "_Z10C4DrawLineiisiish", "source_file": SOURCE,
     "size": "540", "relocations": "36",
     "historical_raw_sha256": "6c7d8e1c6ab5139b7ce0d86483541476800ac5c5c46555514c6c4622e87bbe58",
     "isolated_raw_sha256": "6c7d8e1c6ab5139b7ce0d86483541476800ac5c5c46555514c6c4622e87bbe58",
     "normalized_sha256": "be4ab95329d977e5c23fa90e1ba582ca9c58f565d02d7400ea3bc2696e4a4839",
     "linked_sha256": "df7ee566610996f1856b6ef3c3f0b78ed07d34768582d521d63a1d06005bd163",
     "target_sha256": "", "proof_level": "linked-historical-reference",
     "evidence": "analysis/matching/hunt500plus-v33-validated-204.tsv"},
    {"address": "0x0010d4f0", "symbol": "_Z17C4SprDisintegratev", "source_file": SOURCE,
     "size": "580", "relocations": "11",
     "historical_raw_sha256": "ff52cc683de105f79f23d4f70894354e694a940f3301f336d36df646825b9940",
     "isolated_raw_sha256": "ff52cc683de105f79f23d4f70894354e694a940f3301f336d36df646825b9940",
     "normalized_sha256": "c9feec5d6490a3f14ec2634d986686dd57966fa80e222f9887b6204cc8017b1c",
     "linked_sha256": "c7390c51c88e66701aff3338d7a421055d8cb6dd950dfaffcb0371574bc263b1",
     "target_sha256": "c7390c51c88e66701aff3338d7a421055d8cb6dd950dfaffcb0371574bc263b1",
     "proof_level": "provider-linked-target",
     "evidence": "analysis/matching/hunt1041-v76-validated-c4spr-1.tsv"},
]


def run(command):
    result = subprocess.run([str(x) for x in command], cwd=ROOT, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if result.returncode:
        raise SystemExit("command failed: " + " ".join(map(str, command)) + "\n" + result.stdout[-10000:])
    return result.stdout


def digest(raw):
    return hashlib.sha256(raw).hexdigest()


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
    if flags != (*SOURCE_FIXED_FLAGS["src/snes9x/c4_math.cpp"], "-ffunction-sections"):
        raise SystemExit("native C4 raster compiler profile drift")
    sys.path.insert(0, str(ROOT / "tools/history/research"))
    import hunt1041_v77_c4draw as recipe
    # Recipe verifies the official archive and every PS2 source/header patch.
    built = recipe.build_object(CXX)
    historical = ELFFile(built.path)
    BUILD.mkdir(parents=True, exist_ok=True)
    path = BUILD / "native_c4_raster.o"
    run([CXX, *flags, "-c", ROOT / SOURCE, "-o", path])
    local = ELFFile(path)
    functions = [s for s in local.symbols if s.info & 15 == 2 and s.size]
    writable = [s for s in local.symbols if s.info & 15 == 1 and s.size and 0 < s.section_index < len(local.sections)
                and local.sections[s.section_index].name.startswith((".data", ".bss"))]
    if ({s.name for s in functions} != {r["symbol"] for r in SPECS}
            or sum(s.size for s in functions) != 1120 or any(s.info >> 4 != 1 for s in functions) or writable
            or {s.name for s in local.symbols if s.section_index == 0 and s.name} != set(PROVIDERS)):
        raise SystemExit("native C4 raster function/shared-storage/provider inventory drift")
    data = json.loads((ROOT / "analysis/link_identity/historical_data.json").read_text())
    for name, alias in STATE.items():
        owner = [r for r in data["owners"] if r["unit"] == "c4" and r["symbol"] == name]
        if (len(owner) != 1 or owner[0]["address"] != PROVIDERS[alias] or owner[0]["size"] != 2
                or owner[0]["source_offset"] != PROVIDERS[alias] - 0x335938
                or owner[0]["sha256"] != "96a296d224f285c67bee93c30f8a309157f0daa35dc5b87e410b78630a09cfc7"
                or data["bindings"]["c4"][name]["address"] != PROVIDERS[alias]):
            raise SystemExit("shared signed-short C4 math state geometry drift: " + name)
    memory = [r for r in data["global_anchors"] if r["symbol"] == "Memory"]
    if (len(memory) != 1 or memory[0]["address"] != PROVIDERS["DAT_0034e2b0"]
            or memory[0]["source_offset"] != 37456):
        raise SystemExit("shared original C4 Memory provider drift")
    definitions = list(csv.DictReader((ROOT / "analysis/source_tree/defined_symbol_ownership.tsv").open(), delimiter="\t"))
    for name, source, size in (("C4CalcWireFrame", "src/snes9x/c4_math.cpp", 456),
                               ("C4TransfWireFrame2", "src/snes9x/c4_math.cpp", 768),
                               ("memset", "src/ps2/memset.S", 56)):
        rows = [r for r in definitions if r["symbol"] == name]
        if len(rows) != 1 or rows[0]["source"] != source or rows[0]["size_hex"] != hex(size):
            raise SystemExit("native C4 canonical callee ownership drift: " + name)
    linked = place("native", path, PROVIDERS,
        "".join(f".text.{i} {int(r['address'], 0):#x} : {{ *(.text.{r['symbol']}) }}\n" for i, r in enumerate(SPECS))
        + "/DISCARD/ : { *(.rodata*) *(.data*) *(.bss*) *(COMMON) }")
    bindings = {"Memory": 0x34e2b0, **{name: PROVIDERS[alias] for name, alias in STATE.items()},
                **{name: PROVIDERS[name] for name in ("C4TransfWireFrame2", "C4CalcWireFrame", "memset", "__gxx_personality_v0")}}
    # Each full historical placement is comparison-only outside the selected
    # function. Every selected data/callee relocation has its original address.
    for symbol in historical.symbols:
        if symbol.section_index == 0 and symbol.name and symbol.name not in bindings:
            bindings[symbol.name] = 0
    windows = json.loads((ROOT / "analysis/link_identity/code_windows.json").read_text())["result"]["selected_sources"]
    for index, spec in enumerate(SPECS):
        name, size, address = spec["symbol"], int(spec["size"]), int(spec["address"], 0)
        old, isolated, final = historical.find_symbol(name), local.find_symbol(name), linked.find_symbol(name)
        old_raw, raw, final_raw = historical.symbol_bytes(old, size), local.symbol_bytes(isolated, size), linked.symbol_bytes(final, size)
        selected = [r for r in windows if r["address"] == address and r["symbol"] == name]
        if (len(selected) != 1 or any(selected[0][k] != v for k, v in
                {"size": size, "new_bytes": size, "relocations": int(spec["relocations"]),
                 "raw_sha256": spec["historical_raw_sha256"]}.items())
                or any(s.size != size for s in (old, isolated, final)) or final.value != address
                or raw != old_raw or digest(raw) != spec["historical_raw_sha256"]
                or digest(raw) != spec["isolated_raw_sha256"]
                or normalized(local, isolated) != normalized(historical, old)
                or digest(normalized(local, isolated)) != spec["normalized_sha256"]
                or local.relocation_masks(isolated, size) != historical.relocation_masks(old, size)
                or len(local.relocation_ranges(isolated, size)) != int(spec["relocations"])
                or digest(final_raw) != spec["linked_sha256"]):
            raise SystemExit("native C4 historical instructions/addends/placement drift: " + name)
        reference = place("reference" + str(index), built.path, bindings,
            f".text {address - old.value:#x} : {{ *(.text) }}\n"
            ".rodata 0x40000000 : { *(.rodata*) }\n.data 0x50000000 : { *(.data*) }\n"
            ".bss 0x60000000 : { *(.bss*) *(COMMON) }")
        ref = reference.find_symbol(name)
        if ref.value != address or reference.symbol_bytes(ref, size) != final_raw:
            raise SystemExit("complete selected linked historical reference drift: " + name)
        with (ROOT / spec["evidence"]).open(newline="") as stream:
            rows = [r for r in csv.DictReader(stream, delimiter="\t") if r["address"] == spec["address"]]
        if (len(rows) != 1 or any(rows[0][k] != v for k, v in
                {"result": "MATCH", "differing_bytes": "0", "normalized_equal": "True",
                 "unknown_relocations": "", "object_symbol": name, "object_size": str(size)}.items())):
            raise SystemExit("frozen native C4 normalized target witness drift: " + name)
        if spec["target_sha256"]:
            if (rows[0]["target_span_sha256"] != spec["target_sha256"]
                    or digest(final_raw) != spec["target_sha256"] or spec["proof_level"] != "provider-linked-target"):
                raise SystemExit("complete native C4 target digest drift")
        elif spec["proof_level"] != "linked-historical-reference":
            raise SystemExit("unsupported native C4 proof scope")
        print(f"native C4 raster: MATCH {name} bytes={size}/{size} proof={spec['proof_level']}")
    with EVIDENCE.open(newline="") as stream:
        if list(csv.DictReader(stream, delimiter="\t")) != SPECS:
            raise SystemExit("native C4 raster evidence ledger drift")
    print("native C4 raster proof: 2/2 routines; 1120 historical instruction bytes; "
          "580 complete target instruction bytes; shared native C4 state retained")


if __name__ == "__main__":
    main()
