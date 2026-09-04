#!/usr/bin/env python3
"""Prove runtime assignments of recovered code addresses to function-pointer slots.

This closes address-identity contracts only. It does not claim data storage,
object extent, a canonical source symbol for the LAB entry, or replacement ELF.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import re
import struct
from pathlib import Path
from typing import Sequence

import libgcc_contracts as libgcc

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "runtime_code_pointers.tsv"
DEFAULT_EVIDENCE = ROOT / "analysis" / "functions" / "progress16_r5900_pseudocode.c.txt"

STATUS = "RUNTIME_CODE_POINTER_REFACTOR"
CLAIM = (
    "runtime code-pointer identity proved by exact matched writer instructions; "
    "no data storage or replacement-ELF claim"
)
FIELDS = (
    "symbol","target_address","status",
    "writer_function","writer_address","writer_extent_hex","matching_evidence",
    "pointer_symbol","pointer_address","initial_pointer_value",
    "target_hi_pc","target_hi_word","target_lo_pc","target_lo_word",
    "pointer_base_pc","pointer_base_word","store_pc","store_word",
    "control_pc","control_word","store_control_pc","store_control_word",
    "writer_sha256","instruction_window_sha256","evidence_token","claim",
)

SPEC = {
    "LAB_0012f8a8": {
        "target": 0x0012F8A8, "writer_function": "CMemory_ApplyROMFixes",
        "writer_address": 0x00156C60, "pointer_symbol": "PTR_FUN_00341658",
        "pointer": 0x00341658, "initial": 0x0012E750,
        "target_hi": (0x00156E44, 0x3C020013),
        "target_lo": (0x00156E4C, 0x2442F8A8),
        "pointer_base": (0x00156E48, 0x3C030034),
        "store": (0x00156E50, 0xAC621658),
        "control": None, "store_control": None,
        "evidence_token": "PTR_FUN_00341658 = &LAB_0012f8a8;",
    },
    "LAB_0012fb78": {
        "target": 0x0012FB78, "writer_function": "CMemory_ApplyROMFixes",
        "writer_address": 0x00156C60, "pointer_symbol": "PTR_FUN_0034165c",
        "pointer": 0x0034165C, "initial": 0x0012F744,
        "target_hi": (0x00156E58, 0x3C020013),
        "target_lo": (0x00156E5C, 0x2442FB78),
        "pointer_base": (0x00156E54, 0x3C030034),
        "store": (0x00156E60, 0xAC62165C),
        "control": None, "store_control": None,
        "evidence_token": "PTR_FUN_0034165c = &LAB_0012fb78;",
    },
    "LAB_00170194": {
        "target": 0x00170194, "writer_function": "CMemory_InitROM",
        "writer_address": 0x001522D8, "pointer_symbol": "PTR_FUN_003fa628",
        "pointer": 0x003FA628, "initial": 0x0016FE00,
        "target_hi": (0x00152F04, 0x3C020017),
        "target_lo": (0x00152F08, 0x24420194),
        "pointer_base": (0x00152EF8, 0x3C030040),
        "store": (0x00152F10, 0xAC62A628),
        "control": None, "store_control": None,
        "evidence_token": "PTR_FUN_003fa628 = &LAB_00170194;",
    },
    "LAB_00170138": {
        "target": 0x00170138, "writer_function": "CMemory_InitROM",
        "writer_address": 0x001522D8, "pointer_symbol": "PTR_FUN_003fa62c",
        "pointer": 0x003FA62C, "initial": 0x0016FC90,
        "target_hi": (0x00152F14, 0x3C020017),
        "target_lo": (0x00152F1C, 0x24420138),
        "pointer_base": (0x00152EF8, 0x3C030040),
        "store": (0x00152F00, 0xAC62A62C),
        "control": (0x00152F18, 0x1000FFF7),
        "store_control": (0x00152EFC, 0x1000FDC3),
        "evidence_token": "PTR_FUN_003fa62c = &LAB_00170138;",
    },
}

class RuntimeCodePointerError(RuntimeError):
    pass

def fail(message: str) -> None:
    raise RuntimeCodePointerError(message)

def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()

def signed16(value: int) -> int:
    return value if value < 0x8000 else value - 0x10000

def read_word(raw: bytes, address: int) -> int:
    offset = address - libgcc.TARGET_BASE
    if offset < 0 or offset + 4 > len(raw):
        fail(f"word outside target image: 0x{address:08x}")
    return struct.unpack_from("<I", raw, offset)[0]

def branch_target(pc: int, word: int) -> int:
    op = word >> 26
    if op not in (1,4,5,6,7):
        fail(f"expected direct branch at 0x{pc:08x}")
    return (pc + 4 + (signed16(word & 0xFFFF) << 2)) & 0xFFFFFFFF

def decode_target(hi_word: int, lo_word: int) -> tuple[int,int]:
    if hi_word >> 26 != 0x0F:
        fail("target high word is not LUI")
    reg = hi_word >> 16 & 31
    hi = hi_word & 0xFFFF
    op, rs, rt = lo_word >> 26, lo_word >> 21 & 31, lo_word >> 16 & 31
    if rs != reg or rt != reg or op not in (9,13):
        fail("target low word is not same-register ADDIU/ORI")
    imm = lo_word & 0xFFFF
    value = ((hi << 16) + signed16(imm)) & 0xFFFFFFFF if op == 9 else ((hi << 16) | imm)
    return reg, value

def decode_store(base_word: int, store_word: int) -> tuple[int,int]:
    if base_word >> 26 != 0x0F:
        fail("pointer base is not LUI")
    base_reg = base_word >> 16 & 31
    base = (base_word & 0xFFFF) << 16
    op, rs, rt = store_word >> 26, store_word >> 21 & 31, store_word >> 16 & 31
    if op != 0x2B or rs != base_reg:
        fail("pointer write is not SW through proved base register")
    return rt, (base + signed16(store_word & 0xFFFF)) & 0xFFFFFFFF

def progress_writer(address: int, name: str) -> None:
    path = ROOT / "analysis" / "progress_targets.csv"
    with path.open(encoding="utf-8", newline="") as stream:
        rows = [r for r in csv.DictReader(stream) if int(r["address"], 0) == address]
    if len(rows) != 1 or rows[0]["status"] != "MATCHING" or rows[0]["name"] != name:
        fail(f"writer progress identity drift: 0x{address:08x} {name}")

def matching_span(address: int) -> tuple[int,str]:
    candidates = []
    for path in sorted((ROOT / "analysis" / "matching").glob("*.tsv")):
        try:
            with path.open(encoding="utf-8", newline="") as stream:
                reader = csv.DictReader(stream, delimiter="\t")
                fields = set(reader.fieldnames or ())
                if not {"address","object_size","result","differing_bytes"} <= fields:
                    continue
                for row in reader:
                    try:
                        row_address = int(row["address"], 0)
                        size = int(row["object_size"])
                    except (TypeError, ValueError):
                        continue
                    if (
                        row_address == address and row["result"] == "MATCH"
                        and row["differing_bytes"] == "0"
                        and not row.get("unknown_relocations") and size > 0
                    ):
                        candidates.append((size, path.relative_to(ROOT).as_posix()))
        except (OSError, UnicodeError):
            continue
    if not candidates:
        fail(f"writer lacks strict MATCH evidence: 0x{address:08x}")
    size = max(x[0] for x in candidates)
    evidence = min(x[1] for x in candidates if x[0] == size)
    return size, evidence

def require_evidence(token: str) -> None:
    if not DEFAULT_EVIDENCE.is_file():
        fail("missing pseudocode evidence")
    if token not in DEFAULT_EVIDENCE.read_text(encoding="utf-8", errors="replace"):
        fail(f"runtime assignment evidence missing: {token}")

def public_rows() -> list[dict[str,str]]:
    rows = []
    for symbol in sorted(SPEC):
        spec = SPEC[symbol]
        progress_writer(spec["writer_address"], spec["writer_function"])
        extent, evidence = matching_span(spec["writer_address"])
        require_evidence(spec["evidence_token"])

        def pair(field):
            value = spec[field]
            return ("","") if value is None else (f"0x{value[0]:08x}", f"0x{value[1]:08x}")

        hi_pc, hi_word = pair("target_hi")
        lo_pc, lo_word = pair("target_lo")
        base_pc, base_word = pair("pointer_base")
        store_pc, store_word = pair("store")
        ctl_pc, ctl_word = pair("control")
        sctl_pc, sctl_word = pair("store_control")
        rows.append({
            "symbol": symbol,
            "target_address": f"0x{spec['target']:08x}",
            "status": STATUS,
            "writer_function": spec["writer_function"],
            "writer_address": f"0x{spec['writer_address']:08x}",
            "writer_extent_hex": hex(extent),
            "matching_evidence": evidence,
            "pointer_symbol": spec["pointer_symbol"],
            "pointer_address": f"0x{spec['pointer']:08x}",
            "initial_pointer_value": f"0x{spec['initial']:08x}",
            "target_hi_pc": hi_pc, "target_hi_word": hi_word,
            "target_lo_pc": lo_pc, "target_lo_word": lo_word,
            "pointer_base_pc": base_pc, "pointer_base_word": base_word,
            "store_pc": store_pc, "store_word": store_word,
            "control_pc": ctl_pc, "control_word": ctl_word,
            "store_control_pc": sctl_pc, "store_control_word": sctl_word,
            "writer_sha256": "", "instruction_window_sha256": "",
            "evidence_token": spec["evidence_token"], "claim": CLAIM,
        })
    return rows

def render(rows: Sequence[dict[str,str]]) -> str:
    from io import StringIO
    output = StringIO(newline="")
    writer = csv.DictWriter(output, fieldnames=FIELDS, delimiter="\t", lineterminator="\n")
    writer.writeheader()
    writer.writerows(rows)
    return output.getvalue()

def private_proof(raw: bytes, row: dict[str,str]) -> tuple[str,str]:
    spec = SPEC[row["symbol"]]
    for field in ("target_hi","target_lo","pointer_base","store","control","store_control"):
        pair = spec[field]
        if pair is not None and read_word(raw, pair[0]) != pair[1]:
            fail(f"{row['symbol']} instruction drift at 0x{pair[0]:08x}")

    target_reg, target = decode_target(spec["target_hi"][1], spec["target_lo"][1])
    src_reg, pointer = decode_store(spec["pointer_base"][1], spec["store"][1])
    if target != spec["target"] or src_reg != target_reg or pointer != spec["pointer"]:
        fail(f"{row['symbol']} decoded writer identity drift")

    if spec["control"] is not None:
        ctl_pc, ctl_word = spec["control"]
        if spec["target_lo"][0] != ctl_pc + 4:
            fail(f"{row['symbol']} low-half is not the proved delay slot")
        if branch_target(ctl_pc, ctl_word) != spec["pointer_base"][0]:
            fail(f"{row['symbol']} delay-slot branch target drift")

    if spec["store_control"] is not None:
        ctl_pc, _ = spec["store_control"]
        if spec["store"][0] != ctl_pc + 4:
            fail(f"{row['symbol']} store is not the proved delay slot")

    if read_word(raw, spec["pointer"]) != spec["initial"]:
        fail(f"{row['symbol']} initial pointer value drift")

    start = spec["writer_address"]
    extent = int(row["writer_extent_hex"], 0)
    writer = raw[start-libgcc.TARGET_BASE:start-libgcc.TARGET_BASE+extent]
    if len(writer) != extent:
        fail(f"{row['symbol']} writer body truncated")

    pcs = [
        pair[0] for field in ("target_hi","target_lo","pointer_base","store","control","store_control")
        if (pair := spec[field]) is not None
    ]
    lo, hi = min(pcs), max(pcs) + 4
    window = raw[lo-libgcc.TARGET_BASE:hi-libgcc.TARGET_BASE]
    return digest(writer), digest(window)

def capture(reference: Path, manifest: Path = DEFAULT_MANIFEST) -> list[dict[str,str]]:
    raw = libgcc.load_reference(reference)
    rows = public_rows()
    for row in rows:
        row["writer_sha256"], row["instruction_window_sha256"] = private_proof(raw, row)
    manifest.parent.mkdir(parents=True, exist_ok=True)
    manifest.write_text(render(rows), encoding="utf-8")
    return rows

def validate(manifest: Path = DEFAULT_MANIFEST) -> list[dict[str,str]]:
    if not manifest.is_file():
        fail(f"missing runtime code-pointer manifest: {manifest}")
    expected = public_rows()
    with manifest.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != FIELDS:
            fail("runtime code-pointer manifest columns drift")
        actual = list(reader)
    if len(actual) != len(expected):
        fail("runtime code-pointer roster drift")
    for want, got in zip(expected, actual):
        for key in FIELDS:
            if key in ("writer_sha256","instruction_window_sha256"):
                if not re.fullmatch(r"[0-9a-f]{64}", got[key]):
                    fail(f"private fingerprint missing: {got['symbol']}")
            elif want[key] != got[key]:
                fail(f"public proof drift: {got.get('symbol','?')} field={key}")
    return actual

def verify_reference(reference: Path, rows: Sequence[dict[str,str]] | None = None) -> list[dict[str,str]]:
    rows = validate() if rows is None else list(rows)
    raw = libgcc.load_reference(reference)
    for row in rows:
        writer_sha, window_sha = private_proof(raw, row)
        if writer_sha != row["writer_sha256"] or window_sha != row["instruction_window_sha256"]:
            fail(f"private proof drift: {row['symbol']}")
    return rows

def parse_args(argv=None):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("command", choices=("capture","validate","verify"))
    p.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    p.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    return p.parse_args(argv)

def main(argv=None):
    args = parse_args(argv)
    try:
        if args.command == "capture":
            rows = capture(args.reference.resolve(), args.manifest)
        else:
            rows = validate(args.manifest)
            if args.command == "verify":
                verify_reference(args.reference.resolve(), rows)
        print(
            f"verified runtime code-pointer refactors: closed={len(rows)}/{len(SPEC)} "
            f"writers={len({r['writer_address'] for r in rows})}"
        )
        return 0
    except (RuntimeCodePointerError, OSError, ValueError, KeyError, libgcc.LibgccContractError) as exc:
        print(f"runtime code pointers: FAIL -- {exc}")
        return 1

if __name__ == "__main__":
    raise SystemExit(main())
