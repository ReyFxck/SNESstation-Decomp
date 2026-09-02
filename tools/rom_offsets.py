#!/usr/bin/env python3
"""Audit removal of ROM-relative pseudo-globals, never claim image storage."""
from __future__ import annotations
import argparse
import csv
import hashlib
import re
import struct
from pathlib import Path
import libgcc_contracts as libgcc

ROOT = Path(__file__).resolve().parents[1]
SOURCE = "src/ps2/progress28_structural_lift_recovered.c"
HELPER = "include/rom_offsets_recovered.h"
DEFAULT_MANIFEST = ROOT / "analysis/link_identity/rom_offset_refactors.tsv"
STATUS = "ROM_OFFSET_SOURCE_REFACTOR_CLOSED"
CLAIM = "ROM-relative constant, not storage at the numerically equal EE address"
V52 = "analysis/matching/hunt1041-v52-validated-17.tsv"
V33 = "analysis/matching/hunt500plus-v33-validated-204.tsv"
FUNCTIONS = {
    0x1513bc: (3324, V52, "6239824cf4772b39bebc112b1e8f4ec711b8e0fbf7366ee2bee95c8cd57b4239"),
    0x1522d8: (4220, V52, "5c6072d8d1892b2346877a2820d67d29a2040a333951ad704b9301380cb47a8c"),
    0x154c2c: (880, V33, "fe4ccccb5cc1a55044f02123e8162b13803e9d3860aa8bd28abe8576671518e2"),
    0x156c60: (6256, V52, "ccd5f51a7be5548bf5a7ca28ee0ae35b4c11c0417b936de3ef1182953da43471"),
}
SPEC = {"UNK_00108000": (0x1513bc, "rom-copy-offset"),
        "UNK_00208000": (0x154c2c, "rom-copy-offset"),
        "UNK_00400001": (0x1522d8, "rom-size-limit"),
        "DAT_001385ec": (0x156c60, "rom-byte-patch"),
        "DAT_001385ed": (0x156c60, "rom-byte-patch")}
for prefix in ("00407f", "0040ff"):
    for suffix in ("d5", "d6", "d7", "d8", "d9", "dc", "dd", "de", "df"):
        SPEC["DAT_" + prefix + suffix] = (0x1522d8, "rom-header-byte")
    for suffix in ("b0", "b2", "c0"):
        SPEC["UNK_" + prefix + suffix] = (0x1522d8, "rom-header-pointer")
FIELDS = ("symbol", "offset_hex", "kind", "status", "source", "source_function",
          "function_address", "function_extent_hex", "function_sha256", "matching_evidence",
          "matching_evidence_sha256", "source_function_sha256", "helper_sha256", "claim")


class ROMOffsetError(RuntimeError):
    pass


def digest(data):
    return hashlib.sha256(data).hexdigest()


def body_at(source, address):
    marker = f"/* ===== 0x{address:08x} "
    if source.count(marker) != 1:
        raise ROMOffsetError("missing unique source function marker")
    return source.split(marker, 1)[1].split("/* =====", 1)[0]


def historical_externals():
    return [{"symbol": name, "category": "target-address-data", "provider_kind": "program-data",
             "owner": "reserved:target-data.o", "resolution_gate": "program-data",
             "requesters": "ps2/progress28_structural_lift_recovered.o"} for name in sorted(SPEC)]


def expected_rows(root=ROOT):
    source = (root / SOURCE).read_text()
    helper = (root / HELPER).read_bytes()
    if re.search(r"\b(?:" + "|".join(SPEC) + r")\b", source):
        raise ROMOffsetError("ROM offset still imported as an EE-address symbol")
    if not all(part in helper.decode() for part in ("uint8_t *", "(uint32_t)(base)", "(uint32_t)(offset)")):
        raise ROMOffsetError("ROM byte stride / EE pointer width drift")
    result, functions = [], {}
    for address, (size, evidence, target_hash) in FUNCTIONS.items():
        body = body_at(source, address)
        with (root/evidence).open(newline="") as stream:
            matching = [row for row in csv.DictReader(stream, delimiter="\t")
                        if int(row["address"], 0) == address and int(row["object_size"]) == size
                        and row["result"] == "MATCH" and row["differing_bytes"] == "0"
                        and not row.get("unknown_relocations")]
        if not matching:
            raise ROMOffsetError("historical source function is not strictly matched")
        functions[address] = (body, digest(body.encode()), digest((root/evidence).read_bytes()))
    for name, (address, kind) in sorted(SPEC.items()):
        body, body_hash, evidence_hash = functions[address]
        offset = int(name.rsplit("_", 1)[1], 16)
        if kind == "rom-size-limit":
            if "*(uint *)(iVar19 + 0xb054) < 0x00400001u" not in body:
                raise ROMOffsetError("ROM size limit must be an integer comparison")
        elif not re.search(r"P28_ROM_AT\([^;\n]*, 0x" + f"{offset:08x}" + r"u\)", body):
            raise ROMOffsetError("ROM offset not used by the intended source function")
        if kind == "rom-byte-patch":
            expected = "d0" if offset == 0x1385ec else "b2"
            for tail in (f"== 0x{expected}u", "= 0xeau"):
                if f"*P28_ROM_AT(param_1[1], 0x{offset:08x}u) {tail}" not in body:
                    raise ROMOffsetError("ROM patch must compare unsigned bytes and store one byte")
        size, evidence, target_hash = FUNCTIONS[address]
        result.append(dict(zip(FIELDS, (name, f"0x{offset:08x}", kind, STATUS, SOURCE,
            f"snes_p28_{address:08x}", f"0x{address:08x}", hex(size), target_hash, evidence,
            evidence_hash, body_hash, digest(helper), CLAIM))))
    return result


def validate(manifest=DEFAULT_MANIFEST, external_map=libgcc.DEFAULT_EXTERNAL, root=ROOT):
    rows = libgcc.read_table(manifest, FIELDS)
    if rows != expected_rows(root):
        raise ROMOffsetError("ROM refactor ledger / source proof drift")
    live = {r["symbol"] for r in libgcc.read_table(external_map, libgcc.EXTERNAL_FIELDS)}
    if live & SPEC.keys():
        raise ROMOffsetError("closed ROM offsets remain live externals")
    return rows


def verify_reference(reference):
    raw = libgcc.load_reference(reference)
    for address, (size, _evidence, fingerprint) in FUNCTIONS.items():
        if digest(raw[address-libgcc.TARGET_BASE:address-libgcc.TARGET_BASE+size]) != fingerprint:
            raise ROMOffsetError("original ROM-function fingerprint mismatch")
    for pc, offset in ((0x157c6c, 0x5ec), (0x157c84, 0x5ed)):
        word = struct.unpack_from("<I", raw, pc-libgcc.TARGET_BASE)[0]
        if word >> 26 != 36 or word >> 21 & 31 != 4 or word & 65535 != offset:
            raise ROMOffsetError("original ROM patch byte-load witness mismatch")
    return raw


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("capture", "validate", "verify"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    args = parser.parse_args()
    try:
        if args.command != "validate":
            verify_reference(args.reference)
        if args.command == "capture":
            from runtime_members import render
            args.manifest.write_text(render(FIELDS, expected_rows()))
        rows = validate(args.manifest)
        print(f"verified ROM offset source refactors: closed={len(rows)}/29 (no image storage claimed)")
        return 0
    except (ROMOffsetError, libgcc.LibgccContractError, OSError, ValueError, KeyError) as error:
        print(f"ROM offsets: FAIL -- {error}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
