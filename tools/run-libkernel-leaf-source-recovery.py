#!/usr/bin/env python3
"""Byte-exact historical assembly gate for the SNES Station libkernel leaves.

The source is the exact PS2SDK kernel.S at ps2sdk@a80df908.  The target byte
table is a focused extract from the SHA-256-frozen unpacked SNES_EMU image
739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b.
No private ELF is stored in the repository.
"""
from __future__ import annotations

import csv
import hashlib
import subprocess
from pathlib import Path

import runtime_members as rm

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "matching" / "libkernel-leaf-source-recovery"
SOURCE = ROOT / "src" / "ps2" / "kernel.S"
TARGETS = ROOT / "analysis" / "functions" / "libkernel_leaf_exact_508.tsv"
SOURCE_SHA256 = "73787b575c80535865f68476a5dba62169f3cefe0e9c666245c77de037d99185"
EXPECTED_UNPACKED_SHA256 = "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b"

CORRIDORS = (
    ("main", 0x0019CE60, 0x0019CFBC, "33257bf897b580b9d3311cc6cb595f9c28dbfeaf5f22659a092bc80fa84aa2e2"),
    ("dmac-handlers", 0x0019F5A0, 0x0019F5C0, "4d945ee7dfaa06c2b03b28dda9b4cd21ed05108faa7d7026f09d88412a25773f"),
    ("sif-tail", 0x0019F5D0, 0x0019F600, "70b6c36f51c7d0e87517b62289980cad4ac14c11ef15d9a9f8bdbe2e2da2115d"),
    ("late-syscalls", 0x0019FCD0, 0x0019FD20, "14574a6b5bf28e1b0340d663ee761d4de1df2743465685c679ff7c0ac1fe4d9e"),
)
COMBINED_SHA256 = "9d01b69ef152e91b3cca121850f14ec6409bce4a0469c0653c3dd2699d95cfed"


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


def load_targets() -> list[dict[str, object]]:
    with TARGETS.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    if len(rows) != 22:
        raise SystemExit(f"expected 22 libkernel target rows, found {len(rows)}")

    seen: set[str] = set()
    result: list[dict[str, object]] = []
    for row in rows:
        symbol = row["symbol"]
        address = int(row["address"], 0)
        size = int(row["size_hex"], 0)
        raw = bytes.fromhex(row["raw_hex"])
        if symbol in seen or len(raw) != size:
            raise SystemExit(f"malformed target row: {symbol}")
        if hashlib.sha256(raw).hexdigest() != row["sha256"]:
            raise SystemExit(f"target byte hash drift: {symbol}")
        seen.add(symbol)
        result.append({"symbol": symbol, "address": address, "size": size, "raw": raw})
    if sum(int(row["size"]) for row in result) != 508:
        raise SystemExit("selected libkernel target size drift")
    return result


def main() -> None:
    BUILD.mkdir(parents=True, exist_ok=True)
    if hashlib.sha256(SOURCE.read_bytes()).hexdigest() != SOURCE_SHA256:
        raise SystemExit("historical kernel.S source hash drift")

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

    targets = load_targets()
    actual: dict[str, bytes] = {}
    failed = False

    print("source: src/ps2/kernel.S")
    print(f"source provenance: ps2sdk@{rm.APR18}")
    print(f"compiler: {machine} gcc {version}")
    print(f"private target identity: unpacked sha256={EXPECTED_UNPACKED_SHA256}")
    print()

    for row in targets:
        symbol = str(row["symbol"])
        size = int(row["size"])
        expected = bytes(row["raw"])
        define = "F_" + symbol
        obj = BUILD / f"{symbol}.o"
        run([compiler, *rm.FLAGS, "-D" + define, "-c", SOURCE, "-o", obj])
        image, masks, _normalized = rm.libgcc.text_image(obj)
        padded_size = (size + 7) & ~7
        body = image[:size]
        tail = image[size:]
        differing = sum(a != b for a, b in zip(body, expected))
        ok = (
            len(image) == padded_size
            and not masks
            and len(body) == size
            and not any(tail)
            and body == expected
        )
        actual[symbol] = body
        print(
            f"{'MATCH' if ok else 'DIFF':5} {symbol:24} "
            f"target={size:3} text={len(image):3} pad={len(tail):2} "
            f"relocs={len(masks)} diff={differing}"
        )
        failed |= not ok

    if failed:
        raise SystemExit("LIBKERNEL historical-source object gate: FAIL")

    print()
    print("LIBKERNEL historical-source object gate: 22/22 MATCH")

    combined_actual = bytearray()
    combined_expected = bytearray()
    for name, start, end, expected_sha in CORRIDORS:
        selected = [
            row for row in targets
            if start <= int(row["address"]) < end
        ]
        selected.sort(key=lambda row: int(row["address"]))
        cursor = start
        got = bytearray()
        want = bytearray()
        for row in selected:
            address = int(row["address"])
            size = int(row["size"])
            if address != cursor:
                raise SystemExit(
                    f"{name} target coverage gap at 0x{cursor:08x}; next=0x{address:08x}"
                )
            symbol = str(row["symbol"])
            got.extend(actual[symbol])
            want.extend(bytes(row["raw"]))
            cursor += size
        if cursor != end:
            raise SystemExit(f"{name} target coverage ends at 0x{cursor:08x}, expected 0x{end:08x}")
        got_sha = hashlib.sha256(got).hexdigest()
        if got != want or got_sha != expected_sha:
            raise SystemExit(f"LIBKERNEL raw corridor {name}: FAIL")
        print(
            f"LIBKERNEL raw corridor {name}: MATCH {len(got)}/{end-start} bytes "
            f"sha256={got_sha}"
        )
        combined_actual.extend(got)
        combined_expected.extend(want)

    if combined_actual != combined_expected or len(combined_actual) != 508:
        raise SystemExit("LIBKERNEL selected raw total: FAIL")
    combined_sha = hashlib.sha256(combined_actual).hexdigest()
    if combined_sha != COMBINED_SHA256:
        raise SystemExit("LIBKERNEL selected raw combined hash drift")
    print(f"LIBKERNEL selected raw total: MATCH 508/508 bytes")
    print(f"combined raw sha256: {combined_sha}")


if __name__ == "__main__":
    main()
