#!/usr/bin/env python3
'''Exact target-guided source-literal reconstructions.

These are deliberately NOT classified as historical whole-object data.
Each owner is a minimal source payload whose bytes are explicitly written
here, whose target interval is privately verified, and whose provenance is
limited to public SPC7110-era literals plus target code witnesses.

No neighboring bytes are authorized by these records.
'''
from __future__ import annotations

import argparse
import hashlib
import struct
from functools import lru_cache
from pathlib import Path
from typing import Sequence

import libgcc_contracts as libgcc
import historical_fragments

ORIGIN = "recovered-source-literal"
STATUS = "RECOVERED_SOURCE_LITERAL_EXACT"
CLAIM = "exact recovered source-literal interval; not historical whole-section identity"

_LITERAL_ROWS = (
    {
        "name": "spc7110_pack_smht",
        "address": 0x001B8530,
        "payload": b"SMHT-SP7\x00",
        "public_source": "Snes9x 1.41-1 unix/unix.cpp: SPC7110 pack-directory literal",
        "witness_pcs": (0x0018085C, 0x00180864),
        "covers": ("DAT_001b8530", "DAT_001b8538"),
    },
    {
        "name": "spc7110_pack_feoez",
        "address": 0x001B8558,
        "payload": b"FEOEZSP7\x00",
        "public_source": "Snes9x 1.41-1 unix/unix.cpp: SPC7110 pack-directory literal",
        "witness_pcs": (0x001808AC, 0x001808B8),
        "covers": ("DAT_001b8558",),
    },
    {
        "name": "spc7110_pack_misc",
        "address": 0x001B8580,
        "payload": b"MISC-SP7\x00",
        "public_source": "Snes9x 1.41-1 unix/unix.cpp: SPC7110 default pack-directory literal",
        "witness_pcs": (),
        "covers": ("DAT_001b8580", "DAT_001b8588"),
    },
    {
        "name": "spc7110_pack_sjump",
        "address": 0x001B8590,
        "payload": b"SJUMPSP7\x00",
        "public_source": "Snes9x 1.41-1 unix/unix.cpp: SPC7110 pack-directory literal",
        "witness_pcs": (0x00180900, 0x00180904, 0x00180910),
        "covers": ("DAT_001b8590",),
    },
    {
        "name": "spc7110_path_separator",
        "address": 0x001B85C8,
        "payload": b"/\x00",
        "public_source": "SPC7110 pack path construction; target-guided recovered separator literal",
        "witness_pcs": (
            0x00180E34, 0x00180E3C, 0x00180E5C,
            0x00180E70, 0x00180E7C,
        ),
        "covers": ("UNK_001b85c8",),
    },
    {
        "name": "unzip_mask_bits",
        "unit": "recovered-unzip-source-tables",
        "address": 0x004243D8,
        "payload": struct.pack("<17H",
            0x0000,0x0001,0x0003,0x0007,0x000f,0x001f,0x003f,0x007f,
            0x00ff,0x01ff,0x03ff,0x07ff,0x0fff,0x1fff,0x3fff,0x7fff,0xffff),
        "public_source": "iaddis/SNESticle Gep/Source/common/unzip/explode.c: UWORD mask_bits[17]",
        "witness_pcs": (0x0018C24C, 0x0018C250, 0x0018C888, 0x0018C88C),
        "covers": ("DAT_004243d8",),
    },
    {
        "name": "zlib_inflate_mask",
        "unit": "recovered-zlib-source-tables",
        "address": 0x00425970,
        "payload": struct.pack("<17I",
            0x0000,0x0001,0x0003,0x0007,0x000f,0x001f,0x003f,0x007f,
            0x00ff,0x01ff,0x03ff,0x07ff,0x0fff,0x1fff,0x3fff,0x7fff,0xffff),
        "public_source": "zlib infutil.c: inflate_mask[17]",
        "witness_pcs": (0x00194E30, 0x00194E38, 0x00195B7C, 0x00195B84),
        "covers": ("DAT_00425970",),
    },
)

class RecoveredLiteralError(RuntimeError):
    pass

def require(condition: bool, message: str) -> None:
    if not condition:
        raise RecoveredLiteralError(message)

def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()

def validate() -> dict:
    owners = []
    last_end = -1
    names = set()
    labels = set()
    for source in sorted(_LITERAL_ROWS, key=lambda r: r["address"]):
        name = source["name"]
        address = int(source["address"])
        payload = bytes(source["payload"])
        covers = tuple(source["covers"])
        require(name not in names, "duplicate recovered literal name")
        require(payload, "empty recovered literal")
        require(address >= libgcc.TARGET_BASE, "recovered literal below target base")
        require(address >= last_end, "overlapping recovered literals")
        require(all(isinstance(x, str) and x for x in covers), "bad recovered label roster")
        require(not labels.intersection(covers), "duplicate recovered label owner")
        names.add(name)
        labels.update(covers)
        last_end = address + len(payload)
        owners.append({
            "unit": source.get("unit", "recovered-spc7110-literals"),
            "symbol": name,
            "address": address,
            "size": len(payload),
            "sha256": digest(payload),
            "status": STATUS,
            "claim": CLAIM,
            "public_source": source["public_source"],
            "witness_pcs": list(source["witness_pcs"]),
            "covers": list(covers),
        })
    historical = historical_fragments.validate()
    for owner in historical["owners"]:
        require(owner["symbol"] not in names, "duplicate historical fragment owner")
        require(not labels.intersection(owner["covers"]), "duplicate historical fragment label")
        names.add(owner["symbol"])
        labels.update(owner["covers"])
        owners.append(owner)
    expected = {
        "DAT_001b8530","DAT_001b8538","DAT_001b8558",
        "DAT_001b8580","DAT_001b8588","DAT_001b8590","UNK_001b85c8",
        "DAT_004243d8","DAT_00425970",
        "DAT_003359d0","DAT_00335a50","DAT_00335e50"
    }
    require(labels == expected, f"recovered label roster drift: {sorted(labels)}")
    return {"owners": owners, "owner_count": len(owners), "covered_labels": sorted(labels)}

@lru_cache(maxsize=1)
def _historical_fragment_payloads() -> dict[str, bytes]:
    return historical_fragments.fresh_payloads()

def payload(name: str) -> bytes:
    rows = [r for r in _LITERAL_ROWS if r["name"] == name]
    if len(rows) == 1:
        return bytes(rows[0]["payload"])
    historical = _historical_fragment_payloads()
    require(name in historical, f"unknown recovered/source fragment: {name}")
    return historical[name]

def verify_reference(reference: Path) -> dict:
    proof = validate()
    raw = libgcc.load_reference(reference)
    for owner in proof["owners"]:
        start = owner["address"] - libgcc.TARGET_BASE
        expected = payload(owner["symbol"])
        found = raw[start:start+len(expected)]
        require(found == expected, f"target bytes differ: {owner['symbol']}")
    return proof

def source_backing_payloads(sections: Sequence[dict[str, str]]) -> dict[str, bytes]:
    proof = validate()
    result = {}
    for row in sections:
        if row["origin"] != ORIGIN:
            continue
        start = int(row["target_address"], 0)
        end = start + int(row["extent_hex"], 0)
        owners = [
            owner for owner in proof["owners"]
            if owner["address"] <= start and end <= owner["address"] + owner["size"]
        ]
        require(len(owners) == 1, "recovered backing lacks one exact owner")
        owner = owners[0]
        material = payload(owner["symbol"])
        offset = start - owner["address"]
        piece = material[offset:offset+(end-start)]
        require(len(piece) == end-start, "truncated recovered payload")
        require(digest(piece) == row["sha256"], "recovered payload/section SHA mismatch")
        result[row["section"]] = piece
    return result

def parse_args(argv=None):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("command", choices=("validate", "verify"))
    p.add_argument("--reference", type=Path, default=libgcc.DEFAULT_REFERENCE)
    return p.parse_args(argv)

def main(argv=None):
    args = parse_args(argv)
    try:
        proof = verify_reference(args.reference.resolve()) if args.command == "verify" else validate()
        print(
            f"verified recovered source literals: {proof['owner_count']} owners / "
            f"{len(proof['covered_labels'])} labels"
        )
        return 0
    except (RecoveredLiteralError, OSError, ValueError, libgcc.LibgccContractError) as exc:
        print(f"recovered literals: FAIL -- {exc}")
        return 1

if __name__ == "__main__":
    raise SystemExit(main())
