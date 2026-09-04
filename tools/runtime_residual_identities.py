#!/usr/bin/env python3
"""Freeze V101 runtime identities for three former Stage-3F blockers.

Closures:
  DAT_001babc8 -> __clz_tab
      GCC 3.2.2 libgcc.a::_clz.o .rodata+0, extent 0x100
      status RUNTIME_MEMBER_DATA_OBJECT

  UNK_001ba7e0 -> __thenan_df
      GCC 3.2.2 libgcc.a::_thenan_df.o .rodata+0, extent 0x18
      status RUNTIME_MEMBER_DATA_OBJECT

  UNK_001a6320 -> fde_unencoded_compare
      GCC 3.2.2 libgcc.a::unwind-dw2-fde.o .text+0x660,
      local symbol value 0x660 size 0x28
      status RUNTIME_INTERNAL_CODE_LABEL

The gate verifies the private target, all four local GCC 3.2.2 archive copies,
the exact FP/runtime target witnesses, and the frozen Part4D HOTFIX2 report.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import struct
import subprocess
from collections import Counter
from pathlib import Path
from typing import Sequence

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "runtime_residual_identities.tsv"
PART4D_REPORT = ROOT / "build" / "v101-part4d-hotfix2-runtime-triple" / "report.json"

TARGET_BASE = 0x00100000
EXPECTED_REFERENCE_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

DATA_STATUS = "RUNTIME_MEMBER_DATA_OBJECT"
CODE_STATUS = "RUNTIME_INTERNAL_CODE_LABEL"

SPEC = {
    "DAT_001babc8": {
        "target_address": 0x001BABC8,
        "status": DATA_STATUS,
        "identity": "__clz_tab",
        "member": "_clz.o",
        "section": ".rodata",
        "member_offset": 0x0,
        "extent": 0x100,
        "target_sha256": "14a5d850c255623f9472e3c650abce0c78d32f0276b315b3a276a0462d97a1ac",
        "claim": (
            "exact GCC 3.2.2 libgcc runtime data object identity: "
            "__clz_tab in _clz.o .rodata+0 size 0x100; full target payload "
            "matches all four local archive copies"
        ),
    },
    "UNK_001ba7e0": {
        "target_address": 0x001BA7E0,
        "status": DATA_STATUS,
        "identity": "__thenan_df",
        "member": "_thenan_df.o",
        "section": ".rodata",
        "member_offset": 0x0,
        "extent": 0x18,
        "target_sha256": "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0",
        "claim": (
            "exact GCC 3.2.2 libgcc runtime data object identity: "
            "__thenan_df in _thenan_df.o .rodata+0 size 0x18; exact target "
            "payload plus direct _fpadd_parts/_fpmul_parts/_fpdiv_parts witnesses"
        ),
    },
    "UNK_001a6320": {
        "target_address": 0x001A6320,
        "status": CODE_STATUS,
        "identity": "fde_unencoded_compare",
        "member": "unwind-dw2-fde.o",
        "section": ".text",
        "member_offset": 0x660,
        "extent": 0x28,
        "target_sha256": "",
        "claim": (
            "exact GCC 3.2.2 libgcc internal-code identity: local "
            "fde_unencoded_compare in unwind-dw2-fde.o .text+0x660 size 0x28; "
            "target bytes match all four archive copies and init_object "
            "materializes the target address"
        ),
    },
}

FIELDS = (
    "symbol",
    "target_address",
    "status",
    "identity",
    "member",
    "section",
    "member_offset_hex",
    "extent_hex",
    "target_sha256",
    "archive_copy_count",
    "witness_summary",
    "claim",
)

FP_WITNESSES = (
    (0x001A33C8, 0x001A33CC, 0x001BA7E0, "_fpadd_parts"),
    (0x001A371C, 0x001A3744, 0x001BA7E0, "_fpmul_parts"),
    (0x001A39FC, 0x001A3A14, 0x001BA7E0, "_fpdiv_parts"),
)

CODE_WITNESSES = (
    (0x001A7394, 0x001A73A0, 0x001A6320, "init_object"),
    (0x001A7398, 0x001A73A0, 0x001A6320, "init_object"),
)

SHT_SYMTAB = 2
SHT_NOBITS = 8


class RuntimeResidualError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise RuntimeResidualError(message)


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def s16(value: int) -> int:
    return value - 0x10000 if value & 0x8000 else value


def load_reference(path: Path) -> bytes:
    data = path.read_bytes()
    got = sha(data)
    if got != EXPECTED_REFERENCE_SHA256:
        fail(f"unpacked reference SHA drift: {got}")
    return data


def word(raw: bytes, address: int) -> int:
    off = address - TARGET_BASE
    if off < 0 or off + 4 > len(raw):
        fail(f"word outside private target: 0x{address:08x}")
    return struct.unpack_from("<I", raw, off)[0]


def target_slice(raw: bytes, address: int, size: int) -> bytes:
    off = address - TARGET_BASE
    data = raw[off:off+size]
    if len(data) != size:
        fail(f"target slice truncated at 0x{address:08x}")
    return data


def archives() -> list[Path]:
    found = sorted({
        p for p in (ROOT / "build" / "toolchains").rglob("libgcc.a")
        if "3.2.2" in p.as_posix()
    })
    if len(found) != 4:
        fail(f"expected four GCC 3.2.2 libgcc copies, got {len(found)}")
    return found


def member_bytes(archive: Path, member: str) -> bytes:
    cp = subprocess.run(
        ["ar", "p", str(archive), member],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
        timeout=15,
    )
    if cp.returncode != 0 or not cp.stdout:
        fail(
            f"cannot extract {member} from {archive}: "
            + cp.stderr.decode("utf-8", "replace")[:160]
        )
    return cp.stdout


def parse_elf32(data: bytes):
    if data[:4] != b"\x7fELF" or data[4] != 1:
        fail("archive member is not ELF32")
    endian = "<" if data[5] == 1 else ">"
    hdr = struct.unpack_from(endian + "16sHHIIIIIHHHHHH", data, 0)
    e_shoff = hdr[6]
    e_shentsize = hdr[11]
    e_shnum = hdr[12]
    e_shstrndx = hdr[13]
    rawsh = [
        struct.unpack_from(
            endian + "IIIIIIIIII",
            data,
            e_shoff + i * e_shentsize,
        )
        for i in range(e_shnum)
    ]
    shstr = rawsh[e_shstrndx]
    shnames = data[shstr[4]:shstr[4] + shstr[5]]

    def cstr(blob: bytes, off: int) -> str:
        if off >= len(blob):
            return ""
        end = blob.find(b"\0", off)
        if end < 0:
            end = len(blob)
        return blob[off:end].decode("utf-8", "replace")

    sections = []
    for i, sh in enumerate(rawsh):
        sections.append({
            "index": i,
            "name": cstr(shnames, sh[0]),
            "type": sh[1],
            "flags": sh[2],
            "addr": sh[3],
            "offset": sh[4],
            "size": sh[5],
            "link": sh[6],
            "info": sh[7],
            "align": sh[8],
            "entsize": sh[9],
            "bytes": (
                b""
                if sh[1] == SHT_NOBITS
                else data[sh[4]:sh[4] + sh[5]]
            ),
        })

    symtabs = {}
    for sec in sections:
        if sec["type"] != SHT_SYMTAB or not sec["entsize"]:
            continue
        strsec = sections[sec["link"]]
        symbols = []
        for off in range(0, sec["size"], sec["entsize"]):
            if off + 16 > len(sec["bytes"]):
                break
            st_name, st_value, st_size, st_info, st_other, st_shndx = \
                struct.unpack_from(
                    endian + "IIIBBH",
                    sec["bytes"],
                    off,
                )
            symbols.append({
                "index": len(symbols),
                "name": cstr(strsec["bytes"], st_name),
                "value": st_value,
                "size": st_size,
                "bind": st_info >> 4,
                "type": st_info & 0xF,
                "shndx": st_shndx,
            })
        symtabs[sec["index"]] = symbols

    return sections, [s for group in symtabs.values() for s in group]


def get_section(sections, name: str):
    hits = [s for s in sections if s["name"] == name]
    if len(hits) != 1:
        fail(f"expected one {name} section, got {len(hits)}")
    return hits[0]


def pair_value(raw: bytes, hi_pc: int, low_pc: int) -> list[int]:
    hi = word(raw, hi_pc)
    lo = word(raw, low_pc)
    if ((hi >> 26) & 0x3F) != 15:
        fail(f"expected LUI at 0x{hi_pc:08x}")
    upper = (hi & 0xFFFF) << 16
    op = (lo >> 26) & 0x3F
    imm = lo & 0xFFFF
    values = []
    if op == 9:
        values.append((upper + s16(imm)) & 0xFFFFFFFF)
    elif op == 13:
        values.append((upper | imm) & 0xFFFFFFFF)
    else:
        fail(
            f"unexpected low opcode at 0x{low_pc:08x}: "
            f"op={op} word=0x{lo:08x}"
        )
    return values


def verify_part4d_report() -> dict:
    if not PART4D_REPORT.is_file():
        fail(f"missing Part4D HOTFIX2 report: {PART4D_REPORT}")
    report = json.loads(PART4D_REPORT.read_text(encoding="utf-8"))
    if report.get("triple_closure_ready") is not True:
        fail("Part4D HOTFIX2 report is not triple_closure_ready")
    expected = {
        "dat_001babc8": "__clz_tab",
        "unk_001ba7e0": "__thenan_df",
        "unk_001a6320": "fde_unencoded_compare",
    }
    for key, identity in expected.items():
        row = report.get(key, {})
        if row.get("identity") != identity or row.get("closure_ready") is not True:
            fail(f"Part4D identity drift for {key}: {row}")
    return report


def verify_private(reference: Path) -> dict:
    raw = load_reference(reference)
    report = verify_part4d_report()
    ars = archives()

    # __clz_tab exact full object.
    clz_target = target_slice(raw, SPEC["DAT_001babc8"]["target_address"], 0x100)
    if sha(clz_target) != SPEC["DAT_001babc8"]["target_sha256"]:
        fail("target __clz_tab SHA drift")
    for archive in ars:
        sections, symbols = parse_elf32(member_bytes(archive, "_clz.o"))
        ro = get_section(sections, ".rodata")
        hits = [
            s for s in symbols
            if s["name"] == "__clz_tab"
            and s["type"] == 1
            and s["shndx"] == ro["index"]
        ]
        if len(hits) != 1:
            fail(f"__clz_tab symbol roster drift in {archive}")
        sym = hits[0]
        if sym["value"] != 0 or sym["size"] != 0x100:
            fail(f"__clz_tab geometry drift in {archive}")
        if ro["bytes"][:0x100] != clz_target:
            fail(f"__clz_tab target mismatch in {archive}")

    # __thenan_df exact object and FP target witnesses.
    nan_target = target_slice(raw, SPEC["UNK_001ba7e0"]["target_address"], 0x18)
    if sha(nan_target) != SPEC["UNK_001ba7e0"]["target_sha256"]:
        fail("target __thenan_df SHA drift")
    for archive in ars:
        sections, symbols = parse_elf32(member_bytes(archive, "_thenan_df.o"))
        ro = get_section(sections, ".rodata")
        hits = [
            s for s in symbols
            if s["name"] == "__thenan_df"
            and s["type"] == 1
            and s["shndx"] == ro["index"]
        ]
        if len(hits) != 1:
            fail(f"__thenan_df symbol roster drift in {archive}")
        sym = hits[0]
        if sym["value"] != 0 or sym["size"] != 0x18:
            fail(f"__thenan_df geometry drift in {archive}")
        if ro["bytes"][:0x18] != nan_target:
            fail(f"__thenan_df target mismatch in {archive}")
    for hi_pc, low_pc, expected, _label in FP_WITNESSES:
        if expected not in pair_value(raw, hi_pc, low_pc):
            fail(
                f"FP witness no longer resolves target: "
                f"0x{hi_pc:08x}/0x{low_pc:08x}"
            )

    # fde_unencoded_compare exact local symbol and target prefix.
    code_target = target_slice(raw, SPEC["UNK_001a6320"]["target_address"], 64)
    for archive in ars:
        sections, symbols = parse_elf32(
            member_bytes(archive, "unwind-dw2-fde.o")
        )
        text = get_section(sections, ".text")
        hits = [
            s for s in symbols
            if s["name"] == "fde_unencoded_compare"
            and s["type"] == 2
            and s["bind"] == 0
            and s["shndx"] == text["index"]
        ]
        if len(hits) != 1:
            fail(f"fde_unencoded_compare symbol roster drift in {archive}")
        sym = hits[0]
        if sym["value"] != 0x660 or sym["size"] != 0x28:
            fail(f"fde_unencoded_compare geometry drift in {archive}")
        if text["bytes"][0x660:0x660+64] != code_target:
            fail(f"fde_unencoded_compare target prefix mismatch in {archive}")
    for hi_pc, low_pc, expected, _label in CODE_WITNESSES:
        if expected not in pair_value(raw, hi_pc, low_pc):
            fail(
                f"runtime witness no longer resolves target: "
                f"0x{hi_pc:08x}/0x{low_pc:08x}"
            )

    return report


def expected_rows() -> list[dict[str, str]]:
    witnesses = {
        "DAT_001babc8": "full-0x100-object-match-across-4-libgcc-copies",
        "UNK_001ba7e0": (
            "_fpadd_parts@0x001a33c8/cc;"
            "_fpmul_parts@0x001a371c/44;"
            "_fpdiv_parts@0x001a39fc/0x001a3a14"
        ),
        "UNK_001a6320": (
            "init_object@0x001a7394/0x001a73a0;"
            "init_object@0x001a7398/0x001a73a0"
        ),
    }
    rows = []
    for symbol in sorted(SPEC):
        spec = SPEC[symbol]
        rows.append({
            "symbol": symbol,
            "target_address": f"0x{spec['target_address']:08x}",
            "status": spec["status"],
            "identity": spec["identity"],
            "member": spec["member"],
            "section": spec["section"],
            "member_offset_hex": hex(spec["member_offset"]),
            "extent_hex": hex(spec["extent"]),
            "target_sha256": spec["target_sha256"],
            "archive_copy_count": "4",
            "witness_summary": witnesses[symbol],
            "claim": spec["claim"],
        })
    return rows


def render(rows: Sequence[dict[str, str]]) -> str:
    from io import StringIO
    out = StringIO(newline="")
    writer = csv.DictWriter(
        out, fieldnames=FIELDS, delimiter="\t", lineterminator="\n"
    )
    writer.writeheader()
    writer.writerows(rows)
    return out.getvalue()


def capture(reference: Path, manifest: Path = DEFAULT_MANIFEST):
    verify_private(reference)
    rows = expected_rows()
    manifest.parent.mkdir(parents=True, exist_ok=True)
    manifest.write_text(render(rows), encoding="utf-8")
    return rows


def validate(manifest: Path = DEFAULT_MANIFEST):
    if not manifest.is_file():
        fail(f"missing runtime residual manifest: {manifest}")
    expected = expected_rows()
    with manifest.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != FIELDS:
            fail("runtime residual manifest column drift")
        actual = list(reader)
    if actual != expected:
        fail("runtime residual manifest content drift")
    return actual


def verify_reference(reference: Path, rows=None):
    rows = validate() if rows is None else list(rows)
    verify_private(reference)
    return rows


def parse_args(argv=None):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("command", choices=("capture", "validate", "verify"))
    p.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    p.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
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
            "verified runtime residual identities: "
            "data_objects=2 internal_code_labels=1 archive_copies=4"
        )
        return 0
    except (RuntimeResidualError, OSError, ValueError, KeyError, RuntimeError) as exc:
        print(f"runtime residual identities: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
