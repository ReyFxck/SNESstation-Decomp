#!/usr/bin/env python3
from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import struct
from collections import deque
from pathlib import Path
from typing import Sequence

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_REFERENCE = ROOT / "build" / "SNES_EMU.unpacked.bin"
DEFAULT_MANIFEST = ROOT / "analysis" / "link_identity" / "pcm_buffer_consumed_extents.tsv"

TARGET_BASE = 0x00100000
EXPECTED_REFERENCE_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

STATUS = "PCM_BUFFER_MINIMUM_EXTENT"
CLAIM = (
    "minimum target-consumed PS2 PCM channel extent proved from exact SIZE CFG, "
    "reachable mixer-count transformations, target access formulas, and private "
    "range fingerprint; complete original C array extent remains unclaimed"
)

SIZE_STORE = 0x00105414
MAX_MIXER_COUNT = 1920
WORST_HALFWORD_OFFSET = 0x3BFE
MINIMUM_EXTENT = 0x3C00

OWNER_SPEC = {
    "pcm_left": {
        "base": 0x001BBD80,
        "aliases": (
            "DAT_001bbd80", "DAT_001bbd82",
            "DAT_001bbd84", "DAT_001bbd86",
        ),
    },
    "pcm_right": {
        "base": 0x001D3480,
        "aliases": (
            "DAT_001d3480", "DAT_001d3482",
            "DAT_001d3484", "DAT_001d3486",
        ),
    },
}

SYMBOL_TO_OWNER = {
    symbol: owner
    for owner, spec in OWNER_SPEC.items()
    for symbol in spec["aliases"]
}

FIELDS = (
    "owner",
    "base_address",
    "end_address",
    "minimum_extent_hex",
    "aliases",
    "status",
    "size_values",
    "max_mixer_count",
    "worst_halfword_offset_hex",
    "enqueue_symbol",
    "enqueue_match_evidence",
    "size_store_pc",
    "size_store_owner",
    "private_sha256",
    "zero_bytes",
    "nonzero_bytes",
    "claim",
)

V1 = 3


class PcmExtentError(RuntimeError):
    pass


def fail(message: str) -> None:
    raise PcmExtentError(message)


def load_reference(path: Path) -> bytes:
    payload = path.read_bytes()
    got = hashlib.sha256(payload).hexdigest()
    if got != EXPECTED_REFERENCE_SHA256:
        fail(f"unpacked reference SHA drift: {got}")
    return payload


def read_word(raw: bytes, address: int) -> int:
    off = address - TARGET_BASE
    if off < 0 or off + 4 > len(raw):
        fail(f"word outside target image: 0x{address:08x}")
    return struct.unpack_from("<I", raw, off)[0]


def s16(value: int) -> int:
    return value - 0x10000 if value & 0x8000 else value


def digest(payload: bytes) -> str:
    return hashlib.sha256(payload).hexdigest()


def progress_rows() -> list[dict[str, str]]:
    with (ROOT / "analysis" / "progress_targets.csv").open(
        encoding="utf-8", newline=""
    ) as stream:
        rows = list(csv.DictReader(stream))
    rows.sort(key=lambda row: int(row["address"], 0))
    return rows


def owner_range(address: int) -> tuple[int, int, dict[str, str]]:
    rows = progress_rows()
    for i, row in enumerate(rows):
        start = int(row["address"], 0)
        end = (
            int(rows[i + 1]["address"], 0)
            if i + 1 < len(rows)
            else 0xFFFFFFFF
        )
        if start <= address < end:
            return start, end, row
    fail(f"no progress owner for 0x{address:08x}")


def strict_enqueue_evidence() -> str:
    path = ROOT / "analysis" / "matching" / "hunt1000plus-v46-validated-42.tsv"
    with path.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    hits = [
        row for row in rows
        if row.get("address") == "0x001078f8"
        and row.get("result") == "MATCH"
        and row.get("object_symbol") == "SjPCM_Enqueue"
        and row.get("differing_bytes") == "0"
        and row.get("normalized_equal") == "True"
    ]
    if len(hits) != 1:
        fail(f"SjPCM_Enqueue strict-MATCH proof drift: {len(hits)}")
    return path.relative_to(ROOT).as_posix()


def is_jr(word: int) -> bool:
    return ((word >> 26) & 0x3F) == 0 and (word & 0x3F) == 8


def direct_target(pc: int, word: int) -> int | None:
    op = (word >> 26) & 0x3F
    if op in (2, 3):
        return ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
    if op in (1,4,5,6,7,0x14,0x15,0x16,0x17):
        return (pc + 4 + (s16(word & 0xFFFF) << 2)) & 0xFFFFFFFF
    return None


def is_control(word: int) -> bool:
    op = (word >> 26) & 0x3F
    return op in (1,2,3,4,5,6,7,0x14,0x15,0x16,0x17) or is_jr(word)


def is_branch_likely(word: int) -> bool:
    return ((word >> 26) & 0x3F) in (0x14,0x15,0x16,0x17)


def is_conditional_branch(word: int) -> bool:
    return ((word >> 26) & 0x3F) in (1,4,5,6,7,0x14,0x15,0x16,0x17)


def is_unconditional_direct(word: int) -> bool:
    op = (word >> 26) & 0x3F
    if op == 2:
        return True
    return (
        op == 4
        and ((word >> 21) & 31) == 0
        and ((word >> 16) & 31) == 0
    )


def writes_v1(word: int) -> bool:
    op = (word >> 26) & 0x3F
    rt = (word >> 16) & 31
    rd = (word >> 11) & 31
    fn = word & 0x3F
    if op == 0:
        return rd == V1 and fn != 8
    if op == 3:
        return False
    return (
        op in (
            8,9,10,11,12,13,14,15,24,25,26,27,
            32,33,34,35,36,37,38,39,48,49,50,51,52,53,54,55,
        )
        and rt == V1
    )


def classify_v1_def(pc: int, word: int) -> tuple[str, int | None, int, int]:
    op = (word >> 26) & 0x3F
    rs = (word >> 21) & 31
    rt = (word >> 16) & 31
    imm = word & 0xFFFF
    if rt == V1 and op == 9 and rs == 0:
        return ("CONST", s16(imm) & 0xFFFFFFFF, pc, word)
    if rt == V1 and op == 13 and rs == 0:
        return ("CONST", imm, pc, word)
    if op == 0:
        rd = (word >> 11) & 31
        rt2 = (word >> 16) & 31
        fn = word & 0x3F
        if rd == V1 and fn in (0x21, 0x2D) and rs == 0 and rt2 == 0:
            return ("CONST", 0, pc, word)
    return ("DYNAMIC", None, pc, word)


def cfg_size_values(raw: bytes) -> tuple[list[int], str]:
    start, end, row = owner_range(SIZE_STORE)
    if row["status"] != "MATCHING":
        fail("SIZE-store owner is no longer MATCHING")
    if read_word(raw, SIZE_STORE) != 0xAC43BAE0:
        fail("DAT_001ebae0 common store instruction drift")

    pcs = list(range(start, end, 4))
    pcset = set(pcs)

    def successors(pc: int) -> list[int]:
        word = read_word(raw, pc)
        prev = pc - 4

        if prev in pcset:
            pword = read_word(raw, prev)
            if is_control(pword):
                pop = (pword >> 26) & 0x3F
                target = direct_target(prev, pword)
                if is_branch_likely(pword):
                    return [target] if target in pcset else []
                if pop == 3:
                    return [pc + 4] if pc + 4 in pcset else []
                if is_jr(pword):
                    return []
                if is_unconditional_direct(pword):
                    return [target] if target in pcset else []
                if is_conditional_branch(pword):
                    out = []
                    if target in pcset:
                        out.append(target)
                    if pc + 4 in pcset:
                        out.append(pc + 4)
                    return out
                if pop == 2:
                    return [target] if target in pcset else []

        if is_control(word):
            out = [pc + 4] if pc + 4 in pcset else []
            if is_branch_likely(word) and pc + 8 in pcset:
                out.append(pc + 8)
            return out

        return [pc + 4] if pc + 4 in pcset else []

    entry = ("ENTRY_UNKNOWN", None, start, 0)
    call_clobber = ("CALL_CLOBBER", None, 0, 0)
    in_defs = {pc: set() for pc in pcs}
    out_defs = {pc: set() for pc in pcs}
    in_defs[start] = {entry}
    work = deque([start])
    queued = {start}
    iterations = 0

    while work:
        pc = work.popleft()
        queued.discard(pc)
        iterations += 1
        if iterations > 200000:
            fail("CFG reaching-definition analysis did not converge")

        incoming = set(in_defs[pc])
        word = read_word(raw, pc)
        outgoing = {classify_v1_def(pc, word)} if writes_v1(word) else incoming

        prev = pc - 4
        if prev in pcset:
            pword = read_word(raw, prev)
            if ((pword >> 26) & 0x3F) == 3:
                outgoing = {call_clobber}

        out_defs[pc] = outgoing
        for nxt in successors(pc):
            merged = in_defs[nxt] | outgoing
            if merged != in_defs[nxt]:
                in_defs[nxt] = merged
                if nxt not in queued:
                    queued.add(nxt)
                    work.append(nxt)

    reaching = in_defs[SIZE_STORE]
    bad = [tag for tag in reaching if tag[0] != "CONST"]
    if bad:
        fail(
            "non-constant definition reaches DAT_001ebae0 store: "
            + ", ".join(f"{tag[0]}@0x{tag[2]:08x}" for tag in bad)
        )

    values = sorted({int(tag[1]) for tag in reaching if tag[1] is not None})
    if values != [800, 960]:
        fail(f"SIZE reaching-definition values drift: {values}")
    return values, row["name"]


def verify_mix_model(raw: bytes) -> list[int]:
    p17 = (
        ROOT / "analysis" / "functions" / "progress17_r5900_pseudocode.c.txt"
    ).read_text(encoding="utf-8", errors="ignore")
    required = (
        "DAT_001ebae0 = 0x3c0;",
        "DAT_001ebae0 = 800;",
        "DAT_001ebae4 = DAT_001ebae0;",
        "DAT_001ebae4 = DAT_001ebae0 << 1;",
        "DAT_001ebae4 = (int)DAT_001ebae4 >> 2;",
        "DAT_001ebae4 = DAT_001ebae4 / 2;",
    )
    for token in required:
        if token not in p17:
            fail(f"recovered SIZE/MIX token missing: {token}")

    witnesses = {
        0x0010542C: 0x8C43BAE0,
        0x00105434: 0xAC43BAE4,
        0x001055E0: 0x8CA2BAE4,
        0x001055E4: 0x24440003,
        0x001055E8: 0x28430000,
        0x001055EC: 0x0083100B,
        0x001055F0: 0x00021083,
        0x001055F8: 0xACA2BAE4,
        0x00105600: 0x8C42BAE0,
        0x00105604: 0x00021040,
        0x0010560C: 0xAC62BAE4,
        0x00105940: 0x8C45BAE4,
        0x00105944: 0x0C05D95E,
        0x00105958: 0x8C46BAE0,
        0x00105964: 0x0C041E3E,
    }
    for pc, expected in witnesses.items():
        if read_word(raw, pc) != expected:
            fail(f"SIZE/MIX/call witness drift at 0x{pc:08x}")

    reachable = set()
    for size in (800, 960):
        for value in (size, size * 2):
            reachable.add(value)
            reachable.add(value // 2)
            reachable.add(value // 4)
    reachable = sorted(value for value in reachable if value > 0)
    if reachable != [200,240,400,480,800,960,1600,1920]:
        fail(f"reachable mixer-count set drift: {reachable}")
    return reachable


def verify_low_access_model() -> dict[str, int]:
    p16 = (
        ROOT / "analysis" / "functions" / "progress16_r5900_pseudocode.c.txt"
    ).read_text(encoding="utf-8", errors="ignore")
    for symbol in SYMBOL_TO_OWNER:
        if symbol not in p16:
            fail(f"low PCM alias missing from mixer model: {symbol}")
    for token in ("0x1fffe", "0x1fffc"):
        if token not in p16:
            fail(f"low PCM mask missing from mixer model: {token}")

    modes = {
        "index<<1 +2": ((MAX_MIXER_COUNT - 1) << 1) + 2,
        "(index&0x1fffc)<<1 +6":
            (((MAX_MIXER_COUNT - 1) & 0x1FFFC) << 1) + 6,
        "(index&0x1fffe)<<2 +6":
            (((MAX_MIXER_COUNT - 1) & 0x1FFFE) << 2) + 6,
        "index<<3 +6": ((MAX_MIXER_COUNT - 1) << 3) + 6,
    }
    if max(modes.values()) != WORST_HALFWORD_OFFSET:
        fail(f"worst PCM offset drift: {modes}")
    if WORST_HALFWORD_OFFSET + 2 != MINIMUM_EXTENT:
        fail("minimum extent no longer exactly covers worst halfword")
    return modes


def private_proof(raw: bytes) -> tuple[list[int], str, list[int], dict[str,int]]:
    strict_enqueue_evidence()
    values, store_owner = cfg_size_values(raw)
    reachable = verify_mix_model(raw)
    modes = verify_low_access_model()
    if max(reachable) != MAX_MIXER_COUNT:
        fail("hard mixer-count maximum drift")
    return values, store_owner, reachable, modes


def expected_public_rows() -> list[dict[str, str]]:
    evidence = strict_enqueue_evidence()
    rows = []
    for owner in sorted(OWNER_SPEC):
        spec = OWNER_SPEC[owner]
        base = spec["base"]
        rows.append({
            "owner": owner,
            "base_address": f"0x{base:08x}",
            "end_address": f"0x{base + MINIMUM_EXTENT:08x}",
            "minimum_extent_hex": hex(MINIMUM_EXTENT),
            "aliases": ",".join(spec["aliases"]),
            "status": STATUS,
            "size_values": "800,960",
            "max_mixer_count": str(MAX_MIXER_COUNT),
            "worst_halfword_offset_hex": hex(WORST_HALFWORD_OFFSET),
            "enqueue_symbol": "SjPCM_Enqueue",
            "enqueue_match_evidence": evidence,
            "size_store_pc": f"0x{SIZE_STORE:08x}",
            "size_store_owner": "main",
            "private_sha256": "",
            "zero_bytes": "",
            "nonzero_bytes": "",
            "claim": CLAIM,
        })
    return rows


def render(rows: Sequence[dict[str, str]]) -> str:
    from io import StringIO
    output = StringIO(newline="")
    writer = csv.DictWriter(
        output, fieldnames=FIELDS, delimiter="\t", lineterminator="\n"
    )
    writer.writeheader()
    writer.writerows(rows)
    return output.getvalue()


def capture(reference: Path, manifest: Path = DEFAULT_MANIFEST) -> list[dict[str,str]]:
    raw = load_reference(reference)
    size_values, store_owner, reachable, _modes = private_proof(raw)
    if size_values != [800,960]:
        fail("SIZE proof summary drift")
    if store_owner != "main":
        fail(f"SIZE-store owner name drift: {store_owner}")
    if max(reachable) != MAX_MIXER_COUNT:
        fail("mixer-count proof summary drift")

    rows = expected_public_rows()
    for row in rows:
        start = int(row["base_address"], 0)
        off = start - TARGET_BASE
        payload = raw[off:off + MINIMUM_EXTENT]
        if len(payload) != MINIMUM_EXTENT:
            fail(f"truncated private PCM range: {row['owner']}")
        row["private_sha256"] = digest(payload)
        row["zero_bytes"] = str(payload.count(0))
        row["nonzero_bytes"] = str(MINIMUM_EXTENT - payload.count(0))

    if int(rows[0]["end_address"], 0) > int(rows[1]["base_address"], 0):
        fail("left proved range overlaps right base")

    manifest.parent.mkdir(parents=True, exist_ok=True)
    manifest.write_text(render(rows), encoding="utf-8")
    return rows


def validate(manifest: Path = DEFAULT_MANIFEST) -> list[dict[str,str]]:
    if not manifest.is_file():
        fail(f"missing PCM extent manifest: {manifest}")
    expected = expected_public_rows()
    with manifest.open(encoding="utf-8", newline="") as stream:
        reader = csv.DictReader(stream, delimiter="\t")
        if tuple(reader.fieldnames or ()) != FIELDS:
            fail("PCM extent manifest column drift")
        actual = list(reader)

    if len(actual) != 2:
        fail(f"PCM owner roster drift: {len(actual)}")

    for want, got in zip(expected, actual):
        for field in FIELDS:
            if field == "private_sha256":
                if not re.fullmatch(r"[0-9a-f]{64}", got[field]):
                    fail(f"private PCM fingerprint missing: {got['owner']}")
            elif field in ("zero_bytes", "nonzero_bytes"):
                if not got[field].isdigit():
                    fail(f"PCM byte-count field missing: {got['owner']} {field}")
            elif want[field] != got[field]:
                fail(f"PCM public proof drift: {got.get('owner','?')} field={field}")
        if int(got["zero_bytes"]) + int(got["nonzero_bytes"]) != MINIMUM_EXTENT:
            fail(f"PCM byte-count total drift: {got['owner']}")
    return actual


def verify_reference(
    reference: Path, rows: Sequence[dict[str,str]] | None = None
) -> list[dict[str,str]]:
    rows = validate() if rows is None else list(rows)
    raw = load_reference(reference)
    private_proof(raw)
    for row in rows:
        start = int(row["base_address"], 0)
        off = start - TARGET_BASE
        payload = raw[off:off + MINIMUM_EXTENT]
        if digest(payload) != row["private_sha256"]:
            fail(f"private PCM SHA drift: {row['owner']}")
        if payload.count(0) != int(row["zero_bytes"]):
            fail(f"private PCM zero-byte count drift: {row['owner']}")
        if MINIMUM_EXTENT - payload.count(0) != int(row["nonzero_bytes"]):
            fail(f"private PCM nonzero-byte count drift: {row['owner']}")
    return rows


def parse_args(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("capture","validate","verify"))
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    return parser.parse_args(argv)


def main(argv=None) -> int:
    args = parse_args(argv)
    try:
        if args.command == "capture":
            rows = capture(args.reference.resolve(), args.manifest)
        else:
            rows = validate(args.manifest)
            if args.command == "verify":
                verify_reference(args.reference.resolve(), rows)
        print(
            "verified PCM minimum extents: "
            f"owners={len(rows)} aliases={len(SYMBOL_TO_OWNER)} "
            f"extent_each=0x{MINIMUM_EXTENT:x} max_mixer_count={MAX_MIXER_COUNT}"
        )
        return 0
    except (PcmExtentError, OSError, ValueError, KeyError) as exc:
        print(f"pcm buffer consumed extent: FAIL -- {exc}")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
