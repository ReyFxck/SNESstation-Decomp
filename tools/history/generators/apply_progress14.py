#!/usr/bin/env python3
"""Apply Progress 14 promotions on top of the Progress 13 checkpoint.

This pass does not add scanner hits. It promotes only targets whose complete
behavioral flow is already represented by recovered source and existing target
assembly, then regenerates project-wide progress artifacts.
"""
from __future__ import annotations

import csv
import shutil
import subprocess
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"

PROMOTIONS = {
    "0x00105750": {
        "name": "build_sram_path_00105750",
        "status": "RECONSTRUCTED",
        "confidence": "very-high",
        "notes": "Progress 14: complete split-path, 0x1b basename truncation, mc0:SNES_EMU/ prefix and .SRM append flow recovered from 0x00105750..0x001057f8",
    },
    "0x0010a840": {
        "name": "apu_buffer_allocator",
        "status": "RECONSTRUCTED",
        "confidence": "very-high",
        "notes": "Progress 14: exact three-allocation order, failure cleanup through 0x0010a8bc and boolean return flow represented by recovered source",
    },
    "0x00151330": {
        "name": "per_rom_cleanup",
        "status": "RECONSTRUCTED",
        "confidence": "high",
        "notes": "Progress 14: complete wrapper flow recovered: free/clear per-ROM buffers via 0x00151360 then call 0x00150f54(memory, 0); historical C++ method name remains unclaimed",
    },
}


def patch_csv(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        fields = reader.fieldnames
        if not fields:
            raise SystemExit(f"missing CSV header: {path}")
        rows = list(reader)

    seen = set()
    for row in rows:
        address = row["address"].lower()
        if address in PROMOTIONS:
            row.update({k: v for k, v in PROMOTIONS[address].items() if k in row})
            seen.add(address)

    missing = set(PROMOTIONS) - seen
    if missing:
        raise SystemExit(f"{path}: missing Progress 14 targets: {sorted(missing)}")

    tmp = path.with_suffix(path.suffix + ".p14tmp")
    with tmp.open("w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    tmp.replace(path)
    return rows


def validate_manifest_pair(a: list[dict[str, str]], b: list[dict[str, str]]) -> str:
    aa = {r["address"].lower(): r["status"] for r in a}
    bb = {r["address"].lower(): r["status"] for r in b}
    if len(aa) != len(a) or len(bb) != len(b):
        raise SystemExit("duplicate address found in progress manifests")
    if aa != bb:
        raise SystemExit("progress_targets.csv and symbols.csv address/status maps diverged")

    counts = Counter(aa.values())
    reconstructed = counts["RECONSTRUCTED"] + counts["MATCHING"]
    mapped = sum(counts.values()) - counts["UNKNOWN"]
    if reconstructed != 799 or mapped != 827:
        raise SystemExit(
            f"unexpected Progress 14 totals: reconstructed={reconstructed} mapped={mapped}"
        )
    if counts["PARTIAL"] != 2 or counts["IDENTIFIED"] != 26:
        raise SystemExit(
            f"unexpected residual statuses: PARTIAL={counts['PARTIAL']} IDENTIFIED={counts['IDENTIFIED']}"
        )
    return (
        f"matching={counts['MATCHING']} reconstructed={reconstructed} mapped={mapped} "
        f"partial={counts['PARTIAL']} identified={counts['IDENTIFIED']}"
    )


def syntax_check() -> int:
    cc = shutil.which("cc")
    if cc is None:
        return -1
    files = sorted((ROOT / "src").rglob("*.c"))
    for src in files:
        subprocess.run(
            [cc, "-std=c11", "-Wall", "-Wextra", "-Werror", "-fsyntax-only",
             "-I", str(ROOT / "include"), str(src)],
            cwd=ROOT,
            check=True,
        )
    return len(files)


def main() -> None:
    targets = patch_csv(TARGETS)
    symbols = patch_csv(SYMBOLS)
    totals = validate_manifest_pair(targets, symbols)

    subprocess.run(["python3", str(ROOT / "tools" / "update_progress.py")], cwd=ROOT, check=True)

    checked = syntax_check()
    validation = ROOT / "analysis" / "progress14_validation.txt"
    syntax_line = (
        f"host syntax check: PASS ({checked} recovered C translation units)"
        if checked >= 0 else
        "host syntax check: SKIPPED (cc not found)"
    )
    validation.write_text(
        "Progress 14 validation\n"
        "======================\n"
        f"{totals}\n"
        "expected proxy: reconstructed=799/1137=70.27%, mapped=827/1137=72.74%\n"
        "promoted: 0x00105750, 0x0010a840, 0x00151330\n"
        "remaining PARTIAL: main@0x00104f18, padInit@0x001a8484\n"
        f"{syntax_line}\n",
        encoding="utf-8",
    )
    print(totals)
    print(syntax_line)
    print("Progress 14 applied successfully")


if __name__ == "__main__":
    main()
