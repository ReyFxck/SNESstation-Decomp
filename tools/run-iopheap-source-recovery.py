#!/usr/bin/env python3
"""Exact historical-source gate for SNES Station's IOP heap members."""
from __future__ import annotations

import csv
import hashlib
import re
import subprocess
from pathlib import Path

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "iopheap-source-recovery"
SOURCE = ROOT / "matching" / "candidates" / "iopheap.c"
OBJECT_LEDGER = ROOT / "analysis" / "link_identity" / "runtime_member_objects.tsv"

SPECS = (
    ("kernel/SifAllocIopHeap.o", "SifAllocIopHeap.o", "F_SifAllocIopHeap",
     ROOT / "analysis/functions/loadfile_iop_0019d600.asm", 0x0019D63C, 0x7C),
    ("kernel/SifFreeIopHeap.o", "SifFreeIopHeap.o", "F_SifFreeIopHeap",
     ROOT / "analysis/functions/loadfile_iop_0019d600.asm", 0x0019D6B8, 0x88),
    ("kernel/SifInitIopHeap.o", "SifInitIopHeap.o", "F_SifInitIopHeap",
     ROOT / "analysis/functions/libkernel_client_init_0019f5d0.asm", 0x0019F9E8, 0xC0),
)


def listing_bytes(path: Path, start: int, size: int) -> bytes:
    byte_map: dict[int, int] = {}
    for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
        stripped = line.strip()
        if ":" not in stripped:
            continue
        addr_text, rest = stripped.split(":", 1)
        if re.fullmatch(r"[0-9A-Fa-f]+", addr_text) is None:
            continue
        fields = rest.split()
        if len(fields) < 4 or any(
            re.fullmatch(r"[0-9A-Fa-f]{2}", field) is None
            for field in fields[:4]
        ):
            continue
        address = int(addr_text, 16)
        raw = bytes(int(field, 16) for field in fields[:4])
        for offset, value in enumerate(raw):
            byte_map[address + offset] = value

    missing = [a for a in range(start, start + size) if a not in byte_map]
    if missing:
        raise SystemExit(
            f"{path.name}: missing {len(missing)} target bytes; "
            f"first=0x{missing[0]:08x}"
        )
    return bytes(byte_map[a] for a in range(start, start + size))


def ledger_rows() -> dict[str, dict[str, str]]:
    with OBJECT_LEDGER.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    return {row["member"]: row for row in rows}


def run(command: list[str | Path]) -> None:
    cp = subprocess.run(
        [str(x) for x in command],
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    if cp.returncode:
        raise SystemExit(
            f"command failed: {' '.join(map(str, command))}\n{cp.stdout[-6000:]}"
        )


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    compiler = rm.libgcc.resolve_tool(
        str(ROOT / "build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc")
    )
    version = subprocess.check_output([str(compiler), "-dumpversion"], text=True).strip()
    machine = subprocess.check_output([str(compiler), "-dumpmachine"], text=True).strip()
    if (version, machine) != ("3.2.2", "ee"):
        raise SystemExit(f"unexpected compiler: {machine} gcc {version}")

    frozen = ledger_rows()
    failed = False

    print("source: matching/candidates/iopheap.c")
    print("lineage: ps2dev/ps2sdk@a80df908 ee/kernel/src/iopheap.c")
    print(f"compiler: {machine} gcc {version}")
    print()

    for member, obj_name, define, listing, address, expected_size in SPECS:
        obj = BUILD / obj_name
        run([
            compiler,
            *rm.FLAGS,
            "-Imatching/ee_abi_compat",
            "-D" + define,
            "-c", SOURCE,
            "-o", obj,
        ])

        image, masks, normalized = rm.libgcc.text_image(obj)
        target = listing_bytes(listing, address, expected_size)
        differing = rm.libgcc.differing_unmasked(target, image, masks)
        normalized_target = rm.libgcc.normalize_target(target, masks)
        row = frozen[member]

        ok = (
            row["status"] == "MEMBER_TEXT_EXACT"
            and row["source_revision"] == rm.APR18
            and row["source"] == "ee/kernel/src/iopheap.c"
            and row["define"] == define
            and len(image) == expected_size
            and differing == 0
            and normalized == normalized_target
            and hashlib.sha256(normalized).hexdigest() == row["normalized_sha256"]
            and len(masks) == int(row["relocation_count"])
        )

        print(
            f"{'MATCH' if ok else 'DIFF':5} {define:20} "
            f"target=0x{address:08x} bytes={len(image)}/{expected_size} "
            f"relocs={len(masks)} diff={differing}"
        )
        failed |= not ok

    if failed:
        raise SystemExit("IOPHEAP historical-source gate: FAIL")

    print()
    print("IOPHEAP historical-source gate: 3/3 MATCH")


if __name__ == "__main__":
    main()
