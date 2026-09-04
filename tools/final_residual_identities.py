#!/usr/bin/env python3
"""Freeze and verify the final two V101 Stage-3F residual identities.

DAT_0042355a
  -> rtc_f9.control
  -> exact interior field of historical S7RTC object
  -> status HISTORICAL_OBJECT_INTERIOR_FIELD

DAT_00426820
  -> final byte of the four-byte LSDA augmentation field in the linked
     operator_new/_Znwj .eh_frame FDE
  -> status TARGET_NATIVE_EH_FRAME_RELOCATION_BYTE

Neither closure fabricates a Stage-3F storage section.  The first is an exact
historical-object interior-field proof.  The second is self-describing linked
DWARF metadata in the private target, bound to an independently strict
historical function identity.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import struct
from io import StringIO
from pathlib import Path
from typing import Sequence

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
DEFAULT_MANIFEST = (
    ROOT / "analysis" / "link_identity" / "final_residual_identities.tsv"
)

PART5C_REPORT = ROOT / "build" / "v101-part5c-final-two" / "report.json"
PART5D_REPORT = (
    ROOT
    / "build"
    / "v101-part5d-hotfix3-absptr-eh-frame"
    / "report.json"
)

RTC_OBJECT = (
    ROOT
    / "build"
    / "matching"
    / "hunt1041-v74-spc7110-rtc"
    / "objects"
    / "spc7110.o"
)
RTC_HEADER = (
    ROOT
    / "build"
    / "matching"
    / "hunt1041-v74-spc7110-rtc"
    / "historical"
    / "snes9x-target-layout"
    / "spc7110.h"
)
SPC_MATCH = ROOT / "analysis" / "matching" / "hunt1041-v72-validated-v53-6.tsv"
GET_MATCH = ROOT / "analysis" / "matching" / "hunt500plus-v33-validated-204.tsv"
CPP_MATCH = (
    ROOT / "analysis" / "matching" / "hunt1000plus-v45-validated-runtime.tsv"
)

TARGET_BASE = 0x00100000
EXPECTED_REFERENCE_SHA256 = (
    "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"
)

RTC_STATUS = "HISTORICAL_OBJECT_INTERIOR_FIELD"
EH_STATUS = "TARGET_NATIVE_EH_FRAME_RELOCATION_BYTE"

RTC_NAME = "DAT_0042355a"
RTC_ADDRESS = 0x0042355A
RTC_OBJECT_ADDRESS = 0x00423548
RTC_OBJECT_SIZE = 0x18
RTC_FIELD_OFFSET = 0x12
RTC_OBJECT_SHA256 = (
    "9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0"
)

EH_NAME = "DAT_00426820"
EH_ADDRESS = 0x00426820
CIE_ADDRESS = 0x004267F0
FDE_ADDRESS = 0x0042680C
FDE_LENGTH = 0x30
TARGET_FUNCTION = 0x001A9E88
TARGET_FUNCTION_SIZE = 0xDC
TARGET_PERSONALITY = 0x001A9728
TARGET_LSDA = 0x00426BDC
LSDA_FIELD_START = 0x0042681D
LSDA_FIELD_END = 0x00426821
SCAN_START = 0x00426700
SCAN_END = 0x00426C28

SHT_SYMTAB = 2
SHT_NOBITS = 8

SPEC = {
    RTC_NAME: {
        "target_address": RTC_ADDRESS,
        "status": RTC_STATUS,
        "identity": "rtc_f9.control",
        "proof_kind": "historical-object-interior-field",
        "claim": (
            "exact historical S7RTC rtc_f9 interior-field identity: "
            "target object 0x00423548..0x00423560 matches spc7110.o::rtc_f9 "
            "size 0x18; EE ABI32 field offset +0x12 is control; strict "
            "S9xGetSPC7110/S9xSetSPC7110/S9xUpdateRTC identities corroborate "
            "the same state object"
        ),
    },
    EH_NAME: {
        "target_address": EH_ADDRESS,
        "status": EH_STATUS,
        "identity": "operator_new/_Znwj .eh_frame FDE LSDA field byte",
        "proof_kind": "target-native-linked-eh-frame-metadata",
        "claim": (
            "target-native DWARF .eh_frame identity: DAT_00426820 is the final "
            "byte of the four-byte LSDA augmentation field in the unique FDE "
            "for operator_new/_Znwj at 0x001a9e88 size 0xdc; CIE version 1 "
            "augmentation zPL uses DW_EH_PE_absptr on EE/ELF32 and the linked "
            "LSDA value is 0x00426bdc"
        ),
    },
}

FIELDS = (
    "symbol",
    "target_address",
    "status",
    "identity",
    "proof_kind",
    "container",
    "container_start",
    "container_extent_hex",
    "field_offset_hex",
    "field_extent_hex",
    "evidence",
    "claim",
)


class FinalResidualError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise FinalResidualError(message)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def load_reference(path: Path) -> bytes:
    raw = path.read_bytes()
    got = sha256(raw)
    if got != EXPECTED_REFERENCE_SHA256:
        fail(f"unpacked reference SHA drift: {got}")
    return raw


def target_slice(raw: bytes, address: int, size: int) -> bytes:
    off = address - TARGET_BASE
    if off < 0 or off + size > len(raw):
        fail(f"target slice outside image: 0x{address:08x}+0x{size:x}")
    return raw[off:off + size]


def target_u8(raw: bytes, address: int) -> int:
    return target_slice(raw, address, 1)[0]


def target_u32(raw: bytes, address: int) -> int:
    return struct.unpack_from("<I", target_slice(raw, address, 4), 0)[0]


def read_uleb(raw: bytes, address: int) -> tuple[int, int]:
    value = 0
    shift = 0
    while True:
        byte = target_u8(raw, address)
        address += 1
        value |= (byte & 0x7F) << shift
        if not (byte & 0x80):
            return value, address
        shift += 7
        if shift > 63:
            fail("ULEB overflow")


def read_sleb(raw: bytes, address: int) -> tuple[int, int]:
    value = 0
    shift = 0
    while True:
        byte = target_u8(raw, address)
        address += 1
        value |= (byte & 0x7F) << shift
        shift += 7
        if not (byte & 0x80):
            if shift < 64 and byte & 0x40:
                value |= -(1 << shift)
            return value, address
        if shift > 63:
            fail("SLEB overflow")


def parse_elf32(data: bytes):
    if data[:4] != b"\x7fELF" or data[4] != 1:
        fail("historical object is not ELF32")

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

            st_name, st_value, st_size, st_info, st_other, st_shndx = (
                struct.unpack_from(
                    endian + "IIIBBH",
                    sec["bytes"],
                    off,
                )
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

    symbols = [s for group in symtabs.values() for s in group]
    return sections, symbols


def verify_part_reports() -> None:
    if not PART5C_REPORT.is_file():
        fail(f"missing Part5C report: {PART5C_REPORT}")
    if not PART5D_REPORT.is_file():
        fail(f"missing Part5D HOTFIX3 report: {PART5D_REPORT}")

    p5c = json.loads(PART5C_REPORT.read_text(encoding="utf-8"))
    p5d = json.loads(PART5D_REPORT.read_text(encoding="utf-8"))

    a = p5c.get("A", {})
    if a.get("identity") != "rtc_f9.control" or a.get("closure_ready") is not True:
        fail("Part5C rtc_f9 closure report is not ready")

    if p5d.get("symbol") != EH_NAME or p5d.get("closure_ready") is not True:
        fail("Part5D HOTFIX3 .eh_frame closure report is not ready")

    fde = p5d.get("fde", {})
    if (
        fde.get("address") != FDE_ADDRESS
        or fde.get("initial_location") != TARGET_FUNCTION
        or fde.get("address_range") != TARGET_FUNCTION_SIZE
        or fde.get("lsda_field_start") != LSDA_FIELD_START
        or fde.get("lsda_field_end") != LSDA_FIELD_END
        or fde.get("lsda_value") != TARGET_LSDA
    ):
        fail("Part5D HOTFIX3 frozen FDE geometry drift")


def verify_rtc(raw: bytes) -> None:
    target_object = target_slice(raw, RTC_OBJECT_ADDRESS, RTC_OBJECT_SIZE)

    if sha256(target_object) != RTC_OBJECT_SHA256:
        fail("target rtc_f9 block SHA drift")

    if not RTC_OBJECT.is_file():
        fail(f"missing historical SPC7110 object: {RTC_OBJECT}")
    if not RTC_HEADER.is_file():
        fail(f"missing historical SPC7110 header: {RTC_HEADER}")

    sections, symbols = parse_elf32(RTC_OBJECT.read_bytes())

    hits = [
        s for s in symbols
        if s["name"] == "rtc_f9" and s["size"] == RTC_OBJECT_SIZE
    ]
    if len(hits) != 1:
        fail(f"expected one rtc_f9 size 0x18, got {len(hits)}")

    sym = hits[0]
    if sym["shndx"] >= len(sections):
        fail("rtc_f9 section index invalid")

    sec = sections[sym["shndx"]]
    if sec["name"] != ".data":
        fail(f"rtc_f9 expected .data, got {sec['name']}")

    payload = sec["bytes"][sym["value"]:sym["value"] + sym["size"]]
    if len(payload) != RTC_OBJECT_SIZE:
        fail("rtc_f9 payload truncated")
    if payload != target_object:
        fail("rtc_f9 full historical object does not match target block")

    header = RTC_HEADER.read_text(encoding="utf-8", errors="ignore")
    m = re.search(
        r"(?:typedef\s+)?struct(?:\s+\w+)?\s*\{(?P<body>.*?)\}\s*S7RTC\s*;",
        header,
        re.S,
    )
    if not m:
        m = re.search(r"struct\s+S7RTC\s*\{(?P<body>.*?)\}\s*;", header, re.S)
    if not m:
        fail("S7RTC declaration not found")

    body = re.sub(r"/\*.*?\*/|//[^\n]*", " ", m.group("body"), flags=re.S)
    normalized = re.sub(r"\s+", " ", body)

    ordered = (
        r"unsigned\s+char\s+reg\s*\[\s*16\s*\]"
        r".*?\bshort\s+index\b"
        r".*?\buint8\s+control\b"
        r".*?\bbool\s+init\b"
        r".*?\bint\s+last_used\b"
    )
    if not re.search(ordered, normalized, re.S):
        fail("historical S7RTC field layout drift")

    if RTC_ADDRESS != RTC_OBJECT_ADDRESS + RTC_FIELD_OFFSET:
        fail("DAT_0042355a is not rtc_f9.control +0x12")

    # Historical function identity corroboration.
    if not SPC_MATCH.is_file() or not GET_MATCH.is_file():
        fail("missing validated SPC7110 matching manifests")

    spc = SPC_MATCH.read_text(encoding="utf-8", errors="ignore")
    get = GET_MATCH.read_text(encoding="utf-8", errors="ignore")

    for address, identity in (
        ("0x00181bac", "S9xSetSPC7110"),
        ("0x00182638", "S9xUpdateRTC"),
    ):
        if address not in spc or identity not in spc:
            fail(f"missing strict historical RTC identity {address} {identity}")

    if "0x001813f0" not in get or "S9xGetSPC7110" not in get:
        fail("missing strict historical S9xGetSPC7110 identity")


def verify_eh_frame(raw: bytes) -> None:
    if not CPP_MATCH.is_file():
        fail(f"missing operator_new matching manifest: {CPP_MATCH}")

    lines = [
        line
        for line in CPP_MATCH.read_text(
            encoding="utf-8", errors="ignore"
        ).splitlines()
        if "0x001a9e88" in line.lower()
    ]
    if len(lines) != 1:
        fail(f"expected one operator_new manifest row, got {len(lines)}")

    line = lines[0]
    for term in (
        "operator_new",
        "_Znwj",
        "historical-symbol-size=220",
        "new_op.cc",
        "MATCH",
    ):
        if term not in line:
            fail(f"operator_new manifest missing {term!r}")

    # CIE.
    cie_len = target_u32(raw, CIE_ADDRESS)
    cie_id = target_u32(raw, CIE_ADDRESS + 4)
    cie_end = CIE_ADDRESS + 4 + cie_len

    if cie_len != 0x18 or cie_id != 0:
        fail("operator_new CIE geometry drift")

    p = CIE_ADDRESS + 8
    version = target_u8(raw, p)
    p += 1

    aug = bytearray()
    while True:
        b = target_u8(raw, p)
        p += 1
        if b == 0:
            break
        aug.append(b)

    augmentation = bytes(aug).decode("ascii", "replace")
    code_align, p = read_uleb(raw, p)
    data_align, p = read_sleb(raw, p)
    return_reg, p = read_uleb(raw, p)
    aug_len, p = read_uleb(raw, p)
    aug_start = p
    aug_end = p + aug_len

    if (
        version != 1
        or augmentation != "zPL"
        or code_align != 1
        or data_align != 4
        or return_reg != 64
        or aug_len != 6
        or aug_end > cie_end
    ):
        fail("operator_new CIE semantic decode drift")

    personality_encoding = target_u8(raw, p)
    p += 1

    remaining = aug_end - p
    if remaining < 2:
        fail("zPL CIE augmentation too short")

    personality_size = remaining - 1
    personality = int.from_bytes(
        target_slice(raw, p, personality_size), "little"
    )
    p += personality_size
    lsda_encoding = target_u8(raw, p)
    p += 1

    if p != aug_end:
        fail("zPL CIE augmentation parse did not consume exact payload")

    # 0x00 = DW_EH_PE_absptr; on EE/ELF32 this is a four-byte pointer.
    if (
        personality_encoding != 0x00
        or personality_size != 4
        or personality != TARGET_PERSONALITY
        or lsda_encoding != 0x00
    ):
        fail("operator_new CIE P/L encoding drift")

    # FDE.
    fde_len = target_u32(raw, FDE_ADDRESS)
    fde_end = FDE_ADDRESS + 4 + fde_len
    cie_ref_field = FDE_ADDRESS + 4
    cie_back = target_u32(raw, cie_ref_field)
    resolved_cie = cie_ref_field - cie_back
    initial_location = target_u32(raw, FDE_ADDRESS + 8)
    address_range = target_u32(raw, FDE_ADDRESS + 12)

    fde_aug_len, nextp = read_uleb(raw, FDE_ADDRESS + 16)
    lsda_field_start = nextp
    lsda_field_end = nextp + fde_aug_len

    if fde_aug_len != 4:
        fail("operator_new FDE LSDA width drift")

    lsda = int.from_bytes(
        target_slice(raw, lsda_field_start, fde_aug_len),
        "little",
    )

    if (
        fde_len != FDE_LENGTH
        or resolved_cie != CIE_ADDRESS
        or initial_location != TARGET_FUNCTION
        or address_range != TARGET_FUNCTION_SIZE
        or lsda_field_start != LSDA_FIELD_START
        or lsda_field_end != LSDA_FIELD_END
        or lsda != TARGET_LSDA
    ):
        fail("operator_new FDE/LSDA geometry drift")

    if not (lsda_field_start <= EH_ADDRESS < lsda_field_end):
        fail("DAT_00426820 is not inside operator_new LSDA field")
    if EH_ADDRESS != lsda_field_end - 1:
        fail("DAT_00426820 is not final LSDA byte")

    expected_byte = (TARGET_LSDA >> 24) & 0xFF
    if target_u8(raw, EH_ADDRESS) != expected_byte:
        fail("DAT_00426820 byte no longer equals linked LSDA MSB")

    # Unique FDE for this exact function/range in the local metadata corridor.
    candidates = []
    for address in range(SCAN_START, SCAN_END - 0x14 + 1, 4):
        length = target_u32(raw, address)

        if length < 0x10 or length > 0x400:
            continue

        end = address + 4 + length
        if end > SCAN_END:
            continue

        cie_ref = target_u32(raw, address + 4)
        if cie_ref == 0:
            continue

        fn = target_u32(raw, address + 8)
        rng = target_u32(raw, address + 12)
        if fn != TARGET_FUNCTION or rng != TARGET_FUNCTION_SIZE:
            continue

        resolved = (address + 4) - cie_ref
        if not (SCAN_START <= resolved < address):
            continue

        if target_u32(raw, resolved + 4) != 0:
            continue
        if target_u8(raw, resolved + 8) != 1:
            continue

        candidates.append(address)

    if candidates != [FDE_ADDRESS]:
        fail(
            "operator_new FDE uniqueness drift: "
            + ",".join(f"0x{x:08x}" for x in candidates)
        )

    if not (fde_end <= TARGET_LSDA < SCAN_END):
        fail("operator_new linked LSDA left the nearby exception-metadata corridor")


def verify_private(reference: Path) -> None:
    raw = load_reference(reference)
    verify_part_reports()
    verify_rtc(raw)
    verify_eh_frame(raw)


def expected_rows() -> list[dict[str, str]]:
    return [
        {
            "symbol": RTC_NAME,
            "target_address": f"0x{RTC_ADDRESS:08x}",
            "status": RTC_STATUS,
            "identity": "rtc_f9.control",
            "proof_kind": "historical-object-interior-field",
            "container": "spc7110.o::rtc_f9 / S7RTC",
            "container_start": f"0x{RTC_OBJECT_ADDRESS:08x}",
            "container_extent_hex": "0x18",
            "field_offset_hex": "0x12",
            "field_extent_hex": "0x1",
            "evidence": (
                "Part5C closure_ready; exact full rtc_f9 object payload; "
                "S7RTC EE ABI32 field layout; strict SPC7110 RTC function identities"
            ),
            "claim": SPEC[RTC_NAME]["claim"],
        },
        {
            "symbol": EH_NAME,
            "target_address": f"0x{EH_ADDRESS:08x}",
            "status": EH_STATUS,
            "identity": "operator_new/_Znwj .eh_frame FDE LSDA field byte",
            "proof_kind": "target-native-linked-eh-frame-metadata",
            "container": "linked .eh_frame FDE @0x0042680c",
            "container_start": f"0x{FDE_ADDRESS:08x}",
            "container_extent_hex": "0x34",
            "field_offset_hex": "0x14",
            "field_extent_hex": "0x1",
            "evidence": (
                "Part5D HOTFIX3 closure_ready; CIE v1 zPL; "
                "DW_EH_PE_absptr/EE-ABI32; unique FDE for operator_new/_Znwj; "
                "LSDA field 0x0042681d..0x00426821 -> 0x00426bdc"
            ),
            "claim": SPEC[EH_NAME]["claim"],
        },
    ]


def render(rows: Sequence[dict[str, str]]) -> str:
    out = StringIO(newline="")
    writer = csv.DictWriter(
        out,
        fieldnames=FIELDS,
        delimiter="\t",
        lineterminator="\n",
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
        fail(f"missing final residual manifest: {manifest}")

    with manifest.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != FIELDS:
            fail("final residual manifest column drift")
        actual = list(reader)

    expected = expected_rows()
    if actual != expected:
        fail("final residual manifest content drift")

    return actual


def verify_reference(
    reference: Path = DEFAULT_REFERENCE,
    manifest: Path = DEFAULT_MANIFEST,
):
    rows = validate(manifest)
    verify_private(reference)
    return rows


def parse_args(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("capture", "validate", "verify"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    return parser.parse_args(argv)


def main(argv=None):
    args = parse_args(argv)

    try:
        if args.command == "capture":
            rows = capture(args.reference.resolve(), args.manifest)
        else:
            rows = validate(args.manifest)
            if args.command == "verify":
                verify_private(args.reference.resolve())

        print(
            "verified final Stage-3F residual identities: "
            "historical_object_fields=1 target_native_eh_frame_bytes=1"
        )
        return 0

    except (
        FinalResidualError,
        OSError,
        ValueError,
        KeyError,
        RuntimeError,
        struct.error,
    ) as exc:
        print(f"final Stage-3F residual identities: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
