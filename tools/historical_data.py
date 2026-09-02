#!/usr/bin/env python3
"""Rebuild typed Snes9x data and prove placement using exact relocated code.

No bytes are copied into this manifest. Public validation checks the frozen
recipe/evidence; private rebuild compiles pinned public source in a fresh
directory, resolves every relocation in the selected functions, and compares
the complete provider intervals against the original. This is not final link
identity and does not authorize arbitrary surrounding bytes.
"""
from __future__ import annotations
import argparse
import csv
import hashlib
import json
import shlex
import struct
import subprocess
import sys
import tempfile
from pathlib import Path
from collections import defaultdict
import libgcc_contracts as libgcc
import compare_elf_functions as comparison

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools/history/research"))
import hunt1041_v52_closure as v52
import hunt1041_v75_c4 as v75
v47, v51 = v52.v47, v52.v51
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/historical_data.json"
DEFAULT_BUILD = ROOT / "build/historical-data"
DEFAULT_COMPILER = ROOT / "build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
STATUS = "HISTORICAL_SOURCE_BYTES_EXACT"
CLAIM = "exact typed source-provider interval; not final-link or full-image identity"
UNITS = {"globals": "GLOBALS.CPP", "snaporig": "snaporig.cpp", "apu": "APU.CPP",
         "dsp1": "DSP1.CPP", "fxemu": "fxemu.cpp", "ppu": "ppu.cpp",
         "memmap": "MEMMAP.CPP", "c4": "c4.cpp"}
V52 = "analysis/matching/hunt1041-v52-validated-17.tsv"
V33 = "analysis/matching/hunt500plus-v33-validated-204.tsv"
V46 = "analysis/matching/hunt1000plus-v46-validated-42.tsv"
V75 = "analysis/matching/hunt1041-v75-validated-c4-5.tsv"
# Most anchors reapply every selected function relocation. The two focused
# exceptions prove only their declared data references, never all of .rodata:
# MEMMAP's string pool and crc32Table have different historical placements.
FILTERS = {"ppu": {"symbols": ("@section:.data",)},
           "memmap": {"pcs": (0x1528b8, 0x1528c4)}}
FUNCTIONS = (
    ("snaporig", "_Z9ReadBlockPKcPviS1_", 0x17028c, 268, V52),
    ("snaporig", "_Z16ReadOrigSnapshotPv", 0x170398, 3528, V52),
    ("apu", "S9xResetAPU", 0x10a934, 1044, V52),
    ("apu", "_Z14S9xFixEnvelopeihhh", 0x10b370, 500, V33),
    ("dsp1", "_Z7DSPOp00v", 0x12c13c, 36, V33),
    ("dsp1", "_Z12DSP1_InversessPsS_", 0x12c188, 284, V33),
    ("dsp1", "_Z7DSPOp2Fv", 0x12e2d0, 16, V33),
    ("dsp1", "_Z11S9xInitDSP1v", 0x12e688, 60, V33),
    ("fxemu", "_Z20fx_readRegisterSpacev", 0x12fef0, 580, V33),
    ("ppu", "S9xSetPPU", 0x159268, 5000, V46),
    ("memmap", "_ZN7CMemory7InitROMEh", 0x1522d8, 4220, V52),
    *(("c4", row.symbol, row.address, row.size, V75)
      for row in v75.CANDIDATES if row.formal_manifest),
)
# unit, symbol (or explicitly bounded section), target, extent, source offset.
OWNERS = (
    ("globals", "@section:.data", 0x345060, 0xaf848, 0),
    ("snaporig", "OrigPPU", 0x3fa6c0, 0xc34, 0),
    ("snaporig", "OrigDMA", 0x3fb2f8, 0xc0, 0xc38),
    ("snaporig", "OrigRegisters", 0x3fb3b8, 0x10, 0xcf8),
    ("snaporig", "OrigCPU", 0x3fb3c8, 0x60, 0xd08),
    ("snaporig", "OrigAPU", 0x3fb428, 0xe0, 0xd68),
    ("snaporig", "OrigSoundData", 0x3fb508, 0x10670, 0xe48),
    ("snaporig", "OrigAPURegisters", 0x40bb78, 8, 0x114b8),
    ("apu", "spc_is_dumping", 0x335370, 4, 0),
    ("apu", "spc_is_dumping_temp", 0x335374, 4, 4),
    ("apu", "spc_dump_dsp", 0x335378, 0x100, 8),
    ("apu", "_ZZ14S9xFixEnvelopeihhhE10AttackRate", 0x335478, 0x80, 0x108),
    ("apu", "_ZZ14S9xFixEnvelopeihhhE9DecayRate", 0x3354f8, 0x40, 0x188),
    ("apu", "_ZZ14S9xFixEnvelopeihhhE11SustainRate", 0x335538, 0x100, 0x1c8),
    ("apu", "_ZZ14S9xFixEnvelopeihhhE12IncreaseRate", 0x335638, 0x100, 0x2c8),
    ("apu", "_ZZ14S9xFixEnvelopeihhhE15DecreaseRateExp", 0x335738, 0x100, 0x3c8),
    ("dsp1", "CosTable2", 0x33ce78, 0x2000, 0),
    ("dsp1", "SinTable2", 0x33ee78, 0x2000, 0x2000),
    ("dsp1", "Op04Radius", 0x340f92, 2, 0x411a),
    ("dsp1", "Op0CX1", 0x34139a, 2, 0x4522),
    ("dsp1", "Op02VVA", 0x3413b2, 2, 0x453a),
    ("dsp1", "ViewerZc", 0x34144c, 4, 0x45d4),
    ("dsp1", "CXdistance", 0x34145c, 4, 0x45e4),
    ("dsp1", "Op14Yr", 0x3415b4, 2, 0x473c),
    ("dsp1", "Op14U", 0x3415b6, 2, 0x473e),
    ("dsp1", "Op14F", 0x3415b8, 2, 0x4740),
    ("dsp1", "Op1CY", 0x341616, 2, 0x479e),
    ("dsp1", "Op1CYBR", 0x34161c, 2, 0x47a4),
    ("dsp1", "Op1CX1", 0x341626, 2, 0x47ae),
    ("dsp1", "Op1CZ1", 0x34162a, 2, 0x47b2),
    ("dsp1", "Op1CY2", 0x34162e, 2, 0x47b6),
    ("fxemu", "GSU", 0x342a38, 0x7fc, 0),
    ("fxemu", "_ZZ20fx_readRegisterSpacevE8avHeight", 0x343240, 0x10, 0x808),
    ("fxemu", "_ZZ20fx_readRegisterSpacevE6avMult", 0x343250, 0x10, 0x818),
    ("ppu", "_ZZ9S9xSetPPUE8IncCount", 0x3f4bf8, 8, 8),
    ("ppu", "_ZZ9S9xSetPPUE5Shift", 0x3f4c00, 8, 0x10),
    ("memmap", "crc32Table", 0x1b6ef8, 0x400, 0xd00),
    *(("c4", name, 0x335938 + 2*i, 2, 2*i) for i, name in enumerate((
        "C4WFXVal", "C4WFYVal", "C4WFZVal", "C4WFX2Val", "C4WFY2Val", "C4WFDist",
        "C4WFScale", "C41FXVal", "C41FYVal", "C41FAngleRes", "C41FDist", "C41FDistVal"))),
)
SECTION_BASES = {"dsp1": (".data", 0x33ce78), "fxemu": (".data", 0x342a38),
                 "ppu": (".data", 0x3f4bf0), "memmap": (".rodata", 0x1b61f8),
                 "c4": (".data", 0x335938)}


class HistoricalDataError(RuntimeError):
    pass


def require(condition, message):
    if not condition:
        raise HistoricalDataError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def recipe_hash():
    return digest(b"\0".join(Path(m.__file__).read_bytes() for m in
                            (sys.modules[__name__], comparison, v47, v51, v52, v75)))


def run(command):
    result = subprocess.run(list(map(str, command)), text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    require(result.returncode == 0, f"command failed: {command[0]}\n{result.stdout[-6000:]}")
    return result.stdout.strip()


def symbol_key(elf, symbol):
    if symbol.name:
        return symbol.name
    require(symbol.info & 15 == 3 and 0 < symbol.section_index < len(elf.sections), "unnamed non-section relocation")
    return "@section:" + elf.sections[symbol.section_index].name


def relocations(elf, index):
    require((elf.elf_class, elf.endian, elf.machine, elf.file_type) == (1, "<", 8, 1), "expected MIPS ELF32 REL")
    tables = [s for s in elf.sections if s.type == 2]
    require(len(tables) == 1 and tables[0].entry_size == 16, "ambiguous symbol table")
    result = []
    for section in elf.sections:
        if section.info != index or section.type not in (4, 9):
            continue
        require(section.type == 9 and section.entry_size == 8 and section.size % 8 == 0
                and section.link == tables[0].index, "unsupported relocation table")
        for offset in range(section.offset, section.offset+section.size, 8):
            place, info = struct.unpack_from("<II", elf.data, offset)
            require(info >> 8 < len(elf.symbols), "bad relocation symbol")
            result.append((place, info & 255, symbol_key(elf, elf.symbols[info >> 8])))
    return result


def solve_relocations(records):
    """Solve common S values from complete HI16/LO16 and R_MIPS_26 pairs.

    records are in ELF relocation TABLE order, not instruction order. A HI16
    instruction can physically follow its associated LO16 instruction.
    Every patched word is compared after solving; masking is not a proof.
    """
    pending, bindings = defaultdict(list), {}

    def bind(name, value, witnesses):
        value &= 0xffffffff
        require(libgcc.TARGET_BASE <= value < 0x80000000, "relocation provider outside low target memory")
        if name in bindings:
            require(bindings[name]["address"] == value, f"inconsistent relocated provider: {name}")
            bindings[name]["witnesses"].update(witnesses)
        else:
            bindings[name] = {"address": value, "witnesses": set(witnesses)}

    signed = lambda value: value - 0x10000 if value & 0x8000 else value
    for row in records:
        kind, key, old, new, pc = (row[k] for k in ("kind", "symbol", "source", "target", "pc"))
        if kind == 5:
            pending[key].append(row)
        elif kind == 6:
            for hi in pending.pop(key, []):
                addend = ((hi["source"] & 65535) << 16) + signed(old & 65535)
                value = ((hi["target"] & 65535) << 16) + signed(new & 65535)
                bind(key, value-addend, (hi["pc"], pc))
        elif kind == 4:
            value = ((pc+4) & 0xf0000000) | ((new & 0x3ffffff) << 2)
            bind(key, value-((old & 0x3ffffff) << 2), (pc,))
        else:
            raise HistoricalDataError(f"unsupported data-proof text relocation {kind}")
    require(not any(pending.values()), "unpaired HI16 relocation")
    pending = defaultdict(list)
    for row in records:
        kind, key, old, new, pc = (row[k] for k in ("kind", "symbol", "source", "target", "pc"))
        require(key in bindings, f"no full provider address for {key}")
        value = bindings[key]["address"]
        if kind == 5:
            pending[key].append(row)
            continue
        if kind == 6:
            lo = (value + signed(old & 65535)) & 65535
            require(((old & 0xffff0000) | lo) == new, "LO16 opcode/value mismatch")
            for hi in pending.pop(key, []):
                addend = ((hi["source"] & 65535) << 16) + signed(old & 65535)
                encoded = ((value + addend + 0x8000) >> 16) & 65535
                require(((hi["source"] & 0xffff0000) | encoded) == hi["target"], "HI16 opcode/value mismatch")
        elif kind == 4:
            target = value + ((old & 0x3ffffff) << 2)
            require(target % 4 == 0 and (target & 0xf0000000) == ((pc+4) & 0xf0000000), "jump region mismatch")
            require(((old & 0xfc000000) | ((target >> 2) & 0x3ffffff)) == new, "jump relocation mismatch")
    return {name: {"address": value["address"], "witnesses": sorted(value["witnesses"])}
            for name, value in sorted(bindings.items())}


def infer_bindings(elf, functions, raw, selection=None):
    records, proof, covered = [], [], {}
    indices = set()
    for unit, name, address, size, evidence in functions:
        symbol = elf.find_symbol(name)
        require(symbol.size == size, f"historical function extent drift: {name}")
        check = comparison.compare_function(raw, address-libgcc.TARGET_BASE, size, elf, name)
        require(check.matching and not check.unknown_relocation_types, f"historical normalized function mismatch: {name}")
        indices.add(symbol.section_index)
        for i in range(size // 4):
            key = symbol.value+i*4
            require(key not in covered, "overlapping source function proof")
            covered[key] = address+i*4
        proof.append({"unit": unit, "symbol": name, "address": address, "size": size,
                      "sha256": digest(raw[address-libgcc.TARGET_BASE:address-libgcc.TARGET_BASE+size]),
                      "evidence": evidence, "evidence_sha256": digest((ROOT/evidence).read_bytes()),
                      "relocation_scope": "selected-data-references" if selection else "complete-function-relocations"})
    require(len(indices) == 1, "selected functions must share a text section")
    section = elf.sections[indices.pop()]
    for offset, kind, key in relocations(elf, section.index):
        if offset not in covered:
            continue
        pc = covered[offset]
        if selection and ("symbols" in selection and key not in selection["symbols"]
                          or "pcs" in selection and pc not in selection["pcs"]):
            continue
        records.append({"kind": kind, "symbol": key, "pc": pc,
                        "source": struct.unpack_from("<I", elf.data, section.offset+offset)[0],
                        "target": struct.unpack_from("<I", raw, pc-libgcc.TARGET_BASE)[0]})
    if selection:
        require(records, "empty focused relocation proof")
        if "pcs" in selection:
            require({r["pc"] for r in records} == set(selection["pcs"]), "focused relocation PC roster drift")
        if "symbols" in selection:
            require({r["symbol"] for r in records} == set(selection["symbols"]), "focused relocation symbol roster drift")
    return solve_relocations(records), proof


def dependency_hashes(depfile, roots):
    text = depfile.read_text().replace("\\\n", " ")
    require(":" in text, "compiler dependency file missing target")
    paths = shlex.split(text.split(":", 1)[1])
    result = {}
    for item in paths:
        path = Path(item).resolve()
        found = None
        for prefix, directory in roots:
            if path.is_relative_to(directory.resolve()):
                found = prefix + "/" + path.relative_to(directory.resolve()).as_posix()
                break
        require(found is not None and path.is_file(), f"unpinned compiler dependency: {path}")
        result[found] = digest(path.read_bytes())
    require(result, "empty compiler input closure")
    return dict(sorted(result.items()))


def owner_bytes(elf, name, size, offset):
    if name.startswith("@section:"):
        candidates = [s for s in elf.sections if s.name == name[9:]]
        require(len(candidates) == 1, "provider section missing")
        section = candidates[0]
        require(offset == 0 and section.size == size,
                f"whole provider section size drift: {name}: expected {size}, got {section.size}")
    else:
        symbol = elf.find_symbol(name)
        require(symbol.info & 15 == 1 and symbol.size == size and symbol.value == offset,
                f"typed provider extent/value drift: {name}")
        section = elf.sections[symbol.section_index]
    require(section.type == 1 and section.name in (".data", ".rodata") and offset+size <= section.size,
            "provider must be initialized data within its section")
    require(not any(offset <= place < offset+size for place, _kind, _name in relocations(elf, section.index)),
            "provider has unresolved data relocations")
    return elf.data[section.offset+offset:section.offset+offset+size]


def build_manifest(args, payloads=None):
    raw = libgcc.load_reference(args.reference)
    compiler = args.compiler.resolve()
    require(run([compiler, "-dumpversion"]) == "3.2.2" and run([compiler, "-dumpmachine"]) == "ee",
            "historical EE GCC 3.2.2 required")
    v47.ensure_git_commit(v47.PS2DEV, v47.PS2DEV_REPO, v47.PS2DEV_COMMIT)
    newlib = v47.PS2DEV / "ps2toolchain/soft/newlib-1.10.0/newlib/libc/include"
    gcc_include = Path(run([compiler, "-print-file-name=include"])).resolve()
    args.build_dir.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="recipe-", dir=args.build_dir) as tmp:
        work = Path(tmp)
        old_build = v52.BUILD
        try:
            v52.BUILD = work
            source_root, _original, layout = v52.prepare_snes_layout()
            v52.patch_sources(layout)
            v75.patch_c4(layout / "c4.cpp")
            compat = work / "compat"
            v52.write_compat_headers(compat)
            inputs, objects = {}, {}
            roots = [("layout", layout), ("compat", compat), ("snes9x-1.41-1", source_root),
                     ("newlib-1.10.0", newlib), ("gcc-3.2.2", gcc_include)]
            for unit, filename in UNITS.items():
                target, depfile = work/f"{unit}.o", work/f"{unit}.d"
                flags = [*v47.COMMON_FLAGS, "-Os", *v47.SNES_DEFINES, "-DZLIB", "-nostdinc"]
                if unit == "c4":
                    flags = [flag for flag in flags if flag not in ("-fshort-double", "-DZLIB")]
                    flags.append("-fno-builtin")
                flags += v47.include_args([compat, newlib, layout, layout/"unzip", source_root/"zlib", gcc_include])
                run([compiler, *flags, "-x", "c++", "-MD", "-MF", depfile, "-c", layout/filename, "-o", target])
                inputs[unit] = dependency_hashes(depfile, roots)
                objects[unit] = comparison.ELFFile(target)
            bindings, functions = {}, []
            for unit in UNITS:
                selected = [row for row in FUNCTIONS if row[0] == unit]
                if selected:
                    try:
                        bindings[unit], proof = infer_bindings(objects[unit], selected, raw, FILTERS.get(unit))
                    except HistoricalDataError as error:
                        raise HistoricalDataError(f"{unit}: {error}") from error
                    functions.extend(proof)
            anchors = []
            for unit in ("snaporig", "apu"):
                for name, binding in bindings[unit].items():
                    symbols = [s for s in objects["globals"].symbols if s.name == name and s.section_index]
                    if not symbols:
                        continue
                    require(len(symbols) == 1, "ambiguous GLOBALS provider")
                    symbol = symbols[0]
                    if objects["globals"].sections[symbol.section_index].name != ".data":
                        continue
                    require(binding["address"] - symbol.value == 0x345060, f"GLOBALS anchor mismatch: {name}")
                    anchors.append({"unit": unit, "symbol": name, "source_offset": symbol.value, **binding})
            require(len({r["symbol"] for r in anchors}) >= 14, "insufficient independent GLOBALS anchors")
            section_anchors = []
            for unit, (section_name, base) in SECTION_BASES.items():
                bound = bindings[unit]
                if "@section:" + section_name in bound:
                    require(bound["@section:"+section_name]["address"] == base, "focused section base drift")
                named = []
                for name, binding in bound.items():
                    symbols = [s for s in objects[unit].symbols if s.name == name
                               and 0 < s.section_index < len(objects[unit].sections)
                               and objects[unit].sections[s.section_index].name == section_name]
                    if not symbols:
                        continue
                    require(len(symbols) == 1 and binding["address"]-symbols[0].value == base,
                            f"inconsistent named section anchor: {unit}:{name}")
                    named.append({"unit": unit, "symbol": name, "source_offset": symbols[0].value, **binding})
                require("@section:"+section_name in bound or len(named) >= 3,
                        f"insufficient data placement evidence: {unit}")
                if unit == "dsp1":
                    require(len(named) >= 5, "insufficient independent DSP1 placement anchors")
                section_anchors.extend(named)
            owners = []
            for unit, name, address, size, offset in OWNERS:
                payload = owner_bytes(objects[unit], name, size, offset)
                if unit == "snaporig":
                    require(bindings[unit].get(name, {}).get("address") == address, "snapshot provider binding mismatch")
                elif unit == "apu":
                    direct = bindings[unit].get(name, {}).get("address")
                    section_base = bindings[unit].get("@section:.data", {}).get("address")
                    require(direct == address or section_base is not None and section_base+offset == address,
                            "APU provider binding mismatch")
                elif unit in SECTION_BASES:
                    section_name, base = SECTION_BASES[unit]
                    require(base+offset == address, "source-provider address/offset mismatch")
                    if not name.startswith("@section:"):
                        require(objects[unit].sections[objects[unit].find_symbol(name).section_index].name == section_name,
                                "source-provider section identity drift")
                require(payload == raw[address-libgcc.TARGET_BASE:address-libgcc.TARGET_BASE+size],
                        f"historical data bytes differ: {name}")
                owners.append({"unit": unit, "symbol": name, "address": address, "size": size,
                               "source_offset": offset, "sha256": digest(payload), "status": STATUS, "claim": CLAIM})
                if payloads is not None:
                    payloads[unit, name] = payload
            return {"version": 1, "target_sha256": libgcc.TARGET_SHA256, "recipe_sha256": recipe_hash(),
                    "source_archive_sha256": v47.SNES_141_1_ARCHIVE.sha256,
                    "newlib_commit": v47.PS2DEV_COMMIT, "inputs": inputs, "functions": functions,
                    "bindings": bindings, "global_anchors": sorted(anchors, key=lambda r: (r["unit"], r["symbol"])),
                    "section_anchors": sorted(section_anchors, key=lambda r: (r["unit"], r["symbol"])),
                    "relocation_filters": {k: {a: list(b) for a, b in v.items()} for k, v in FILTERS.items()},
                    "owners": owners, "replacement_elf": False, "claim": CLAIM}
        finally:
            v52.BUILD = old_build


def validate(manifest=DEFAULT_MANIFEST):
    data = json.loads(manifest.read_text())
    require(data["version"] == 1 and data["target_sha256"] == libgcc.TARGET_SHA256
            and data["recipe_sha256"] == recipe_hash() and data["claim"] == CLAIM
            and data["replacement_elf"] is False, "historical data recipe / scope drift")
    require(data["source_archive_sha256"] == v47.SNES_141_1_ARCHIVE.sha256
            and data["newlib_commit"] == v47.PS2DEV_COMMIT and set(data["inputs"]) == set(UNITS),
            "historical source identity drift")
    require(data["relocation_filters"] == {k: {a: list(b) for a,b in v.items()} for k,v in FILTERS.items()},
            "focused data-reference selection drift")
    for inputs in data["inputs"].values():
        require(inputs and all(libgcc.SHA_RE.fullmatch(value) and not Path(key).is_absolute()
                              and ".." not in Path(key).parts for key, value in inputs.items()), "invalid input closure")
    require(len(data["owners"]) == len(OWNERS), "provider roster drift")
    for row, (unit, name, address, size, offset) in zip(data["owners"], OWNERS):
        require((row["unit"], row["symbol"], row["address"], row["size"], row["source_offset"])
                == (unit, name, address, size, offset), "typed provider interval drift")
        require(row["claim"] == CLAIM and row["status"] == STATUS and libgcc.SHA_RE.fullmatch(row["sha256"]),
                "typed provider claim / fingerprint drift")
    expected_functions = [row for unit in UNITS for row in FUNCTIONS if row[0] == unit]
    require(len(data["functions"]) == len(expected_functions), "function proof roster drift")
    for row, (unit, name, address, size, evidence) in zip(data["functions"], expected_functions):
        require((row["unit"], row["symbol"], row["address"], row["size"], row["evidence"])
                == (unit, name, address, size, evidence), "function proof identity drift")
        require(row["evidence_sha256"] == digest((ROOT/evidence).read_bytes())
                and libgcc.SHA_RE.fullmatch(row["sha256"]), "function proof hashes drift")
        require(row["relocation_scope"] == ("selected-data-references" if unit in FILTERS else "complete-function-relocations"),
                "relocation proof scope drift")
        with (ROOT/evidence).open(newline="") as stream:
            found = [r for r in csv.DictReader(stream, delimiter="\t") if int(r["address"], 0) == address
                     and int(r["object_size"]) == size and r["result"] == "MATCH"
                     and r["differing_bytes"] == "0" and not r.get("unknown_relocations")]
        require(found, "provider anchor function is not strictly matched")
    require(len({r["symbol"] for r in data["global_anchors"]}) >= 14, "GLOBALS anchors missing")
    for row in data["global_anchors"]:
        require(row["unit"] in ("snaporig", "apu") and row["source_offset"] >= 0
                and row["address"]-row["source_offset"] == 0x345060
                and data["bindings"][row["unit"]][row["symbol"]] == {k:row[k] for k in ("address", "witnesses")},
                "GLOBALS anchor geometry drift")
    for unit, bindings in data["bindings"].items():
        ranges = [(row[2], row[2]+row[3]) for row in FUNCTIONS if row[0] == unit]
        require(ranges and bindings, "unknown/empty binding unit")
        for row in bindings.values():
            require(libgcc.TARGET_BASE <= row["address"] < 0x80000000 and row["witnesses"]
                    and row["witnesses"] == sorted(set(row["witnesses"])), "invalid binding address/trace")
            require(all(pc % 4 == 0 and any(a <= pc < b for a,b in ranges) for pc in row["witnesses"]),
                    "relocation witness outside matched functions")
    require(set(data["bindings"]) == set(UNITS)-{"globals"}, "binding unit roster drift")
    require(all(r["unit"] in SECTION_BASES for r in data["section_anchors"]), "unexpected named section anchor unit")
    for unit, (section_name, base) in SECTION_BASES.items():
        rows = [r for r in data["section_anchors"] if r["unit"] == unit]
        named = {r["symbol"] for r in rows}
        require(len(named) == len(rows), "duplicate named section anchors")
        for row in rows:
            require(row["source_offset"] >= 0 and row["address"]-row["source_offset"] == base
                    and data["bindings"][unit][row["symbol"]] == {k:row[k] for k in ("address", "witnesses")},
                    "named section anchor geometry drift")
        binding = data["bindings"][unit].get("@section:"+section_name)
        require(binding and binding["address"] == base or len(rows) >= 3, "missing section placement proof")
        if unit == "dsp1":
            require(len(rows) >= 5, "DSP1 section anchors missing")
        focus = FILTERS.get(unit)
        if focus:
            if "symbols" in focus:
                require(set(data["bindings"][unit]) == set(focus["symbols"]), "unexpected focused provider")
            if "pcs" in focus:
                require({pc for row in data["bindings"][unit].values() for pc in row["witnesses"]} == set(focus["pcs"]),
                        "focused proof witness roster drift")
    return data


def verify_reference(reference, data):
    raw = libgcc.load_reference(reference)
    for row in data["owners"] + data["functions"]:
        start = row["address"]-libgcc.TARGET_BASE
        require(digest(raw[start:start+row["size"]]) == row["sha256"], "private historical interval fingerprint drift")
    return raw


def parse_args(argv=None):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("command", choices=("capture", "validate", "verify", "build"))
    p.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    p.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    p.add_argument("--compiler", type=Path, default=DEFAULT_COMPILER)
    p.add_argument("--build-dir", type=Path, default=DEFAULT_BUILD)
    return p.parse_args(argv)


def main(argv=None):
    args = parse_args(argv)
    try:
        if args.command == "capture":
            data = build_manifest(args)
            args.manifest.write_text(json.dumps(data, indent=2, sort_keys=True)+"\n")
        data = validate(args.manifest)
        if args.command in ("verify", "build"):
            verify_reference(args.reference, data)
        if args.command == "build":
            require(build_manifest(args) == data, "fresh historical rebuild differs from frozen proof")
        print(f"verified historical data providers: intervals={len(data['owners'])} "
              f"bytes={sum(r['size'] for r in data['owners'])} anchors={len(data['global_anchors'])} "
              "(typed source bytes; final link unproved)")
        return 0
    except (HistoricalDataError, libgcc.LibgccContractError, OSError, ValueError, KeyError) as error:
        print(f"historical data: FAIL -- {error}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
