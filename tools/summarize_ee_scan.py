#!/usr/bin/env python3
"""Summarize the historical EE translation-unit scan by first diagnostic."""
from __future__ import annotations

import argparse
import csv
import re
from collections import Counter
from pathlib import Path


LOCATION_RE = re.compile(r"^(?:[^:]+):\d+(?::\d+)?:\s*")


def normalize(text: str) -> str:
    text = LOCATION_RE.sub("", text.strip())
    return " ".join(text.split())


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report", type=Path)
    parser.add_argument("--top", type=int, default=20)
    args = parser.parse_args()

    with args.report.open(encoding="utf-8", newline="") as stream:
        rows = list(csv.DictReader(stream))

    statuses = Counter(row["status"] for row in rows)
    print(
        "EE scan: "
        + " ".join(
            f"{name}={statuses.get(name, 0)}"
            for name in ("PASS", "MISSING_HEADER", "EE_C_ERROR", "COMPILER_CRASH")
        )
    )

    passed = [row["source"] for row in rows if row["status"] == "PASS"]
    print("\nPASS translation units:")
    for source in passed:
        print(f"  {source}")

    failures = [row for row in rows if row["status"] != "PASS"]
    grouped = Counter(normalize(row["diagnostic"]) for row in failures)
    print(f"\nTop {min(args.top, len(grouped))} first diagnostics:")
    for diagnostic, count in grouped.most_common(args.top):
        print(f"  {count:3d}x  {diagnostic}")

    print("\nFirst failing translation units:")
    for row in failures[: min(25, len(failures))]:
        print(f"  {row['source']}: {normalize(row['diagnostic'])}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
