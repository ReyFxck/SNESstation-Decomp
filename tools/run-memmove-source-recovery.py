#!/usr/bin/env python3
"""Byte-exact historical-source gate for SNES Station's PS2LIB memmove.S."""
from __future__ import annotations

import csv
import hashlib
import subprocess
from pathlib import Path

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "memmove-source-recovery"
SOURCE = ROOT / "src" / "ps2" / "memmove.S"
TARGETS = ROOT / "analysis" / "functions" / "memmove_exact_136.tsv"
SOURCE_SHA256 = "9eb1f7106592d6cd1badec162ebb5bfe08f32fb8a0901de3461744d93a162541"
EXPECTED_TARGET_SHA256 = "2c186336c5b6a90179b8d87225a562a458f9eb58ca86322cfb1630950f3336ed"


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
    if len(rows) != 1 or rows[0]["symbol"] != "memmove":
        raise SystemExit("expected one memmove target row")
    row = rows[0]
    if int(row["address"], 0) != 0x0019C4A0 or int(row["size_hex"], 0) != 0x88:
        raise SystemExit("memmove target geometry drift")
    raw = bytes.fromhex(row["raw_hex"])
    if len(raw) != 136 or hashlib.sha256(raw).hexdigest() != row["sha256"]:
        raise SystemExit("memmove target byte/hash drift")
    if row["sha256"] != EXPECTED_TARGET_SHA256:
        raise SystemExit("memmove frozen target hash drift")
    return raw


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest() != SOURCE_SHA256:
        raise SystemExit("historical memmove.S source hash drift")

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
    obj = BUILD / "memmove.o"
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

    print("source: src/ps2/memmove.S")
    print(f"source provenance: ps2sdk@{rm.APR15} ee/libc/src/memmove.S")
    print(f"compiler: {machine} gcc {version}")
    print(
        f"{'MATCH' if ok else 'DIFF':5} memmove.o target=136 text={len(image)} "
        f"relocs={len(masks)} diff={differing}"
    )
    if not ok:
        raise SystemExit("MEMMOVE historical-source object gate: FAIL")
    print("MEMMOVE historical-source object gate: 1/1 MATCH")
    print("MEMMOVE raw target gate: MATCH 136/136 bytes")
    print(f"raw sha256: {hashlib.sha256(image).hexdigest()}")


if __name__ == "__main__":
    main()
