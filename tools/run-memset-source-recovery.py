#!/usr/bin/env python3
"""Byte-exact historical-source gate for SNES Station's PS2LIB memset.S."""
from __future__ import annotations

import csv
import hashlib
import subprocess
from pathlib import Path

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "memset-source-recovery"
SOURCE = ROOT / "src" / "ps2" / "memset.S"
TARGETS = ROOT / "analysis" / "functions" / "memset_exact_56.tsv"
SOURCE_SHA256 = "bdd5d98086e83c0464ab16a085ea5f806c1ccfb88dfd981f3de15ccd0a9f7fd3"
EXPECTED_TARGET_SHA256 = "cecbe61c3a5bab20064a2941574a50bd6e9c1727fc5c161be546310cd045a731"


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
            f"command failed: {' '.join(map(str, command))}\n{cp.stdout[-8000:]}"
        )


def load_target() -> bytes:
    with TARGETS.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    if len(rows) != 1 or rows[0]["symbol"] != "memset":
        raise SystemExit("expected one memset target row")
    row = rows[0]
    if int(row["address"], 0) != 0x0019C39C or int(row["size_hex"], 0) != 0x38:
        raise SystemExit("memset target geometry drift")
    raw = bytes.fromhex(row["raw_hex"])
    if len(raw) != 56 or hashlib.sha256(raw).hexdigest() != row["sha256"]:
        raise SystemExit("memset target byte/hash drift")
    if row["sha256"] != EXPECTED_TARGET_SHA256:
        raise SystemExit("memset frozen target hash drift")
    return raw


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest() != SOURCE_SHA256:
        raise SystemExit("historical memset.S source hash drift")

    compiler = rm.libgcc.resolve_tool(
        str(
            ROOT
            / "build"
            / "toolchains"
            / "ee-gcc-3.2.2-stage1"
            / "prefix"
            / "bin"
            / "ee-gcc"
        )
    )
    version = subprocess.check_output([str(compiler), "-dumpversion"], text=True).strip()
    machine = subprocess.check_output([str(compiler), "-dumpmachine"], text=True).strip()
    if (version, machine) != ("3.2.2", "ee"):
        raise SystemExit(f"unexpected compiler: {machine} gcc {version}")

    expected = load_target()
    obj = BUILD / "memset.o"
    run([compiler, *rm.FLAGS, "-c", SOURCE, "-o", obj])
    image, masks, normalized = rm.libgcc.text_image(obj)
    differing = rm.libgcc.differing_unmasked(expected, image, masks)
    ok = (
        len(image) == len(expected)
        and not masks
        and differing == 0
        and image == expected
        and normalized == expected
        and hashlib.sha256(image).hexdigest() == EXPECTED_TARGET_SHA256
    )

    print("source: src/ps2/memset.S")
    print(f"source provenance: ps2sdk@{rm.APR15} ee/libc/src/memset.S")
    print(f"compiler: {machine} gcc {version}")
    print(
        f"{'MATCH' if ok else 'DIFF':5} memset.o target=56 text={len(image)} "
        f"relocs={len(masks)} diff={differing}"
    )
    if not ok:
        raise SystemExit("MEMSET historical-source object gate: FAIL")
    print("MEMSET historical-source object gate: 1/1 MATCH")
    print("MEMSET raw target gate: MATCH 56/56 bytes")
    print(f"raw sha256: {hashlib.sha256(image).hexdigest()}")


if __name__ == "__main__":
    main()
