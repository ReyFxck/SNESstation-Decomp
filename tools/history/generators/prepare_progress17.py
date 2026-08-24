#!/usr/bin/env python3
"""Build the audited Progress 17 target and scanner-rejection manifests."""
from __future__ import annotations

import csv
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
JAL = ROOT / "analysis" / "jal_candidates.csv"
PROGRESS = ROOT / "analysis" / "progress_targets.csv"
TARGETS_OUT = ROOT / "analysis" / "progress17_targets.csv"
REJECTED_OUT = ROOT / "analysis" / "progress17_rejected_jal_candidates.csv"

CODE_START = 0x00100000
CODE_END = 0x001B0800
EXPECTED_RAW_JAL_TARGETS = 1137
EXPECTED_TRACKED_P16 = 992
EXPECTED_REJECTED = 292
EXPECTED_NEW_CODE_TARGETS = 49
EXPECTED_PROMOTIONS = 25
EXPECTED_VALIDATED_UNIVERSE = 1041


def read_csv(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as handle:
        return list(csv.DictReader(handle))


def write_csv(path: Path, rows: list[dict[str, str]]) -> None:
    with path.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(
            handle, fieldnames=list(rows[0]), lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(rows)


def in_code(address: int) -> bool:
    return CODE_START <= address < CODE_END


def area_for(address: int) -> str:
    if address < 0x00114000:
        return "frontend-core"
    if address < 0x00131000:
        return "snes-core-dsp"
    if address < 0x00150B1C:
        return "renderer"
    if address < 0x00160000:
        return "memory-ppu"
    if address < 0x00184000:
        return "cpu-audio-runtime"
    if address < 0x001A0000:
        return "legacy-zip-zlib"
    return "gcc-runtime"


def main() -> None:
    jal_rows = read_csv(JAL)
    progress_rows = read_csv(PROGRESS)
    if len(jal_rows) != EXPECTED_RAW_JAL_TARGETS:
        raise SystemExit(f"expected 1,137 raw JAL targets, found {len(jal_rows)}")
    if len(progress_rows) != EXPECTED_TRACKED_P16:
        raise SystemExit(f"expected 992 P16 rows, found {len(progress_rows)}")

    jal = {row["target"].lower(): row for row in jal_rows}
    tracked = {row["address"].lower(): row for row in progress_rows}
    if len(jal) != len(jal_rows) or len(tracked) != len(progress_rows):
        raise SystemExit("duplicate address in an input manifest")

    rejected: list[dict[str, str]] = []
    new_targets: list[dict[str, str]] = []
    for address, row in sorted(jal.items(), key=lambda item: int(item[0], 16)):
        if address in tracked:
            continue
        target = int(address, 16)
        caller = int(row["first_call_site"], 16)
        if not in_code(target):
            reason = "target-outside-confirmed-code"
            evidence = (
                "decoded JAL word occurs in post-code data and points outside "
                "0x00100000..0x001b07ff"
            )
        elif not in_code(caller):
            reason = "data-only-call-site"
            evidence = (
                "all observed JAL-pattern words are in post-code data; no "
                "executable caller"
            )
        else:
            new_targets.append({
                "address": address,
                "source": "untracked-code-jal",
                "prior_status": "UNTRACKED",
                "area": area_for(target),
                "first_call_site": row["first_call_site"].lower(),
                "call_count": row["call_count"],
                "evidence": "target and first caller both inside confirmed code",
            })
            continue
        rejected.append({
            "target": address,
            "first_call_site": row["first_call_site"].lower(),
            "call_count": row["call_count"],
            "reason": reason,
            "evidence": evidence,
        })

    promotions: list[dict[str, str]] = []
    for row in progress_rows:
        if row["status"] in {"RECONSTRUCTED", "MATCHING"}:
            continue
        address = row["address"].lower()
        candidate = jal.get(address)
        promotions.append({
            "address": address,
            "source": "mapped-promotion",
            "prior_status": row["status"],
            "area": row["area"],
            "first_call_site": (
                candidate["first_call_site"].lower() if candidate else ""
            ),
            "call_count": candidate["call_count"] if candidate else "0",
            "evidence": row["notes"],
        })

    targets = sorted(
        new_targets + promotions, key=lambda row: int(row["address"], 16)
    )
    if len(rejected) != EXPECTED_REJECTED:
        raise SystemExit(f"expected 292 rejected scanner hits, found {len(rejected)}")
    if len(new_targets) != EXPECTED_NEW_CODE_TARGETS:
        raise SystemExit(f"expected 49 new code targets, found {len(new_targets)}")
    if len(promotions) != EXPECTED_PROMOTIONS:
        raise SystemExit(f"expected 25 promotions, found {len(promotions)}")
    if len(tracked) + len(new_targets) != EXPECTED_VALIDATED_UNIVERSE:
        raise SystemExit("validated universe arithmetic changed")
    if len({row["address"] for row in targets}) != len(targets):
        raise SystemExit("duplicate Progress 17 target")

    write_csv(TARGETS_OUT, targets)
    write_csv(REJECTED_OUT, rejected)
    reasons = Counter(row["reason"] for row in rejected)
    print(f"wrote {TARGETS_OUT.relative_to(ROOT)} ({len(targets)} targets)")
    print(f"wrote {REJECTED_OUT.relative_to(ROOT)} ({len(rejected)} rows)")
    print(
        "validated universe: 1137 raw - 292 rejected + 196 non-JAL entries "
        "= 1041"
    )
    print("rejections: " + ", ".join(
        f"{reason}={count}" for reason, count in sorted(reasons.items())
    ))


if __name__ == "__main__":
    main()
