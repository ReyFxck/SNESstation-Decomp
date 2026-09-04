#!/usr/bin/env python3
'''Freshly rebuilt historical source fragments for V101.'''
from __future__ import annotations

import argparse
import hashlib
import json
import struct
import subprocess
import sys
from pathlib import Path
from typing import Sequence

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools" / "history" / "research"))

import hunt1041_v75_c4 as v75
import hunt1000plus_v47_closure as v47
from compare_elf_functions import ELFFile
import libgcc_contracts as libgcc

STATUS = "HISTORICAL_SOURCE_FRAGMENT_EXACT"
CLAIM = "fresh hash-pinned historical source fragment; no whole-TU or replacement-ELF claim"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "historical_fragments.json"
DEFAULT_COMPILER = (
    ROOT / "build" / "toolchains" / "ee-gcc-3.2.2-cxx-stage1"
    / "prefix" / "bin" / "ee-g++"
)
DEFAULT_BUILD = ROOT / "build" / "historical-fragments"

FRAGMENTS = (
    ("c4emu_prefix", 0x003359D0, 0x080, 0x000, ("DAT_003359d0",)),
    ("C4SinTable",   0x00335A50, 0x400, 0x080, ("DAT_00335a50",)),
    ("C4CosTable",   0x00335E50, 0x400, 0x480, ("DAT_00335e50",)),
)

class HistoricalFragmentError(RuntimeError):
    pass

def require(ok: bool, message: str) -> None:
    if not ok:
        raise HistoricalFragmentError(message)

def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()

def run(cmd: Sequence[object]) -> str:
    p = subprocess.run([str(x) for x in cmd], cwd=ROOT, text=True, capture_output=True)
    if p.returncode:
        raise HistoricalFragmentError(
            "command failed: " + " ".join(str(x) for x in cmd) + "\n" + p.stdout + "\n" + p.stderr
        )
    return p.stdout.strip()

def include_args(paths):
    result = []
    for path in paths:
        result += ["-I", path.resolve().relative_to(ROOT.resolve()).as_posix()]
    return result

def relocation_offsets(elf: ELFFile, target_section: int):
    result = []
    for sec in elf.sections:
        if sec.type not in (9,4) or sec.info != target_section or not sec.entry_size:
            continue
        require(elf.elf_class == 1, "expected ELF32")
        for index in range(sec.size // sec.entry_size):
            off = sec.offset + index * sec.entry_size
            result.append(struct.unpack_from(elf.endian + "I", elf.data, off)[0])
    return sorted(result)

def build(compiler: Path = DEFAULT_COMPILER):
    compiler = compiler.resolve()
    require(compiler.is_file(), f"missing EE compiler: {compiler}")
    require(run([compiler, "-dumpversion"]) == "3.2.2", "EE GCC 3.2.2 required")
    require(run([compiler, "-dumpmachine"]) == "ee", "EE target compiler required")

    source_root, _original, layout, newlib = v75.prepare_layout()
    source = layout / "c4emu.cpp"
    require(source.is_file(), "Snes9x 1.41-1 c4emu.cpp missing")

    DEFAULT_BUILD.mkdir(parents=True, exist_ok=True)
    compat = DEFAULT_BUILD / "compat"
    compat.mkdir(parents=True, exist_ok=True)
    (compat / "memory.h").write_text(
        "#ifndef V101_HISTFRAG_MEMORY_H\n#define V101_HISTFRAG_MEMORY_H\n"
        "#include <string.h>\n#endif\n",
        encoding="utf-8",
    )

    flags = [
        *v47.COMMON_FLAGS,
        *v47.SNES_DEFINES,
        *include_args((compat, newlib, layout, layout / "unzip", source_root / "zlib")),
        "-x", "c++", "-Os",
    ]
    obj = DEFAULT_BUILD / "c4emu.o"
    run([compiler, *flags, "-c", source.relative_to(ROOT), "-o", obj.relative_to(ROOT)])
    elf = ELFFile(obj)

    data = [s for s in elf.sections if s.name == ".data" and s.type == 1]
    require(len(data) == 1, "expected one c4emu .data")
    data = data[0]
    require(data.size >= 0x880, f"c4emu .data too small: 0x{data.size:x}")

    expected_symbols = (
        ("C4TestPattern", 0x000, 0x030),
        ("_ZZ14C4BitPlaneWavevE7bmpdata", 0x030, 0x050),
        ("C4SinTable", 0x080, 0x400),
        ("C4CosTable", 0x480, 0x400),
    )
    for name, value, size in expected_symbols:
        matches = [
            s for s in elf.symbols
            if s.name == name and s.section_index == data.index
            and s.value == value and s.size == size
        ]
        require(len(matches) == 1, f"symbol geometry drift: {name}")

    rels = relocation_offsets(elf, data.index)
    require(not [x for x in rels if 0 <= x < 0x880],
            f"data relocations inside claimed fragments: {rels}")

    blob = elf.data[data.offset:data.offset+data.size]
    payloads = {}
    owners = []
    for symbol,address,size,source_offset,covers in FRAGMENTS:
        payload = blob[source_offset:source_offset+size]
        require(len(payload) == size, f"truncated fragment: {symbol}")
        payloads[symbol] = payload
        owners.append({
            "unit": "snes9x-1.41-1-c4emu",
            "symbol": symbol,
            "address": address,
            "size": size,
            "source_offset": source_offset,
            "sha256": digest(payload),
            "status": STATUS,
            "claim": CLAIM,
            "covers": list(covers),
        })

    metadata = {
        "version": 1,
        "target_sha256": libgcc.TARGET_SHA256,
        "status": STATUS,
        "claim": CLAIM,
        "source_archive_sha256": v47.SNES_141_1_ARCHIVE.sha256,
        "source_sha256": digest(source.read_bytes()),
        "flags": flags,
        "data_size": data.size,
        "data_relocations": rels,
        "owners": owners,
        "replacement_elf": False,
    }
    return metadata, payloads

def capture(reference: Path, compiler: Path, manifest: Path):
    data, payloads = build(compiler)
    raw = libgcc.load_reference(reference)
    for owner in data["owners"]:
        start = owner["address"] - libgcc.TARGET_BASE
        require(
            raw[start:start+owner["size"]] == payloads[owner["symbol"]],
            f"historical source differs from target: {owner['symbol']}",
        )
    manifest.parent.mkdir(parents=True, exist_ok=True)
    manifest.write_text(json.dumps(data, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return data

def validate(manifest: Path = DEFAULT_MANIFEST):
    require(manifest.is_file(), f"missing manifest: {manifest}")
    data = json.loads(manifest.read_text(encoding="utf-8"))
    require(data.get("version") == 1, "manifest version drift")
    require(data.get("target_sha256") == libgcc.TARGET_SHA256, "target identity drift")
    require(data.get("status") == STATUS and data.get("claim") == CLAIM, "claim drift")
    require(data.get("replacement_elf") is False, "replacement ELF must remain unclaimed")
    require(data.get("source_archive_sha256") == v47.SNES_141_1_ARCHIVE.sha256, "archive drift")
    require(len(data.get("owners", [])) == 3, "owner roster drift")
    for row, spec in zip(data["owners"], FRAGMENTS):
        symbol,address,size,source_offset,covers = spec
        require(
            (row["symbol"],row["address"],row["size"],row["source_offset"],tuple(row["covers"]))
            == (symbol,address,size,source_offset,covers),
            f"owner geometry drift: {symbol}",
        )
        require(len(row["sha256"]) == 64, f"owner SHA missing: {symbol}")
    require(not [x for x in data.get("data_relocations", []) if 0 <= x < 0x880],
            "frozen relocation scope drift")
    return data

def fresh_payloads(compiler: Path = DEFAULT_COMPILER):
    frozen = validate()
    fresh, payloads = build(compiler)
    for key in ("source_archive_sha256","source_sha256","flags","data_size"):
        require(fresh[key] == frozen[key], f"fresh recipe drift: {key}")
    by_name = {r["symbol"]: r for r in fresh["owners"]}
    for row in frozen["owners"]:
        other = by_name.get(row["symbol"])
        require(other is not None, f"fresh owner missing: {row['symbol']}")
        require(
            (other["address"],other["size"],other["source_offset"],other["sha256"])
            == (row["address"],row["size"],row["source_offset"],row["sha256"]),
            f"fresh payload drift: {row['symbol']}",
        )
    return payloads

def verify_reference(reference: Path, data=None):
    data = validate() if data is None else data
    raw = libgcc.load_reference(reference)
    for owner in data["owners"]:
        start = owner["address"] - libgcc.TARGET_BASE
        require(
            digest(raw[start:start+owner["size"]]) == owner["sha256"],
            f"private target fingerprint drift: {owner['symbol']}",
        )
    return data

def parse_args(argv=None):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("command", choices=("capture","validate","verify","build"))
    p.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    p.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    p.add_argument("--compiler", type=Path, default=DEFAULT_COMPILER)
    return p.parse_args(argv)

def main(argv=None):
    args = parse_args(argv)
    try:
        if args.command == "capture":
            data = capture(args.reference.resolve(), args.compiler.resolve(), args.manifest)
        else:
            data = validate(args.manifest)
        if args.command == "verify":
            verify_reference(args.reference.resolve(), data)
        if args.command == "build":
            verify_reference(args.reference.resolve(), data)
            fresh_payloads(args.compiler.resolve())
        print(
            f"verified historical fragments: owners={len(data['owners'])} "
            f"bytes={sum(r['size'] for r in data['owners'])}"
        )
        return 0
    except (HistoricalFragmentError, OSError, ValueError, KeyError, libgcc.LibgccContractError) as exc:
        print(f"historical fragments: FAIL -- {exc}")
        return 1

if __name__ == "__main__":
    raise SystemExit(main())
