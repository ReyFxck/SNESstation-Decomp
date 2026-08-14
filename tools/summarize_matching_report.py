#!/usr/bin/env python3
"""Print a compact summary of compare_elf_functions.py Markdown output."""
from __future__ import annotations

import argparse
import re
from pathlib import Path


RESULT_RE = re.compile(r"- Result: \*\*(\d+)/(\d+) relocation-normalized matches\*\*")
ROW_RE = re.compile(
    r"^\| `(?P<address>0x[0-9a-fA-F]+)` \| `(?P<target>[^`]+)` \| "
    r"`(?P<object>[^`]+)` \| (?P<sizes>[^|]+) \| (?P<relocs>[^|]+) \| "
    r"\*\*(?P<status>[^*]+)\*\* \| (?P<difference>[^|]+) \|$"
)


def parse_report(text: str) -> tuple[int, int, list[dict[str, str]]]:
    match = RESULT_RE.search(text)
    if match is None:
        raise ValueError("matching result line not found")
    rows: list[dict[str, str]] = []
    for line in text.splitlines():
        row = ROW_RE.match(line.strip())
        if row is not None:
            rows.append(row.groupdict())
    return int(match.group(1)), int(match.group(2)), rows


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("report", type=Path)
    args = parser.parse_args()
    matched, total, rows = parse_report(args.report.read_text(encoding="utf-8"))
    print(f"matching summary: {matched}/{total}")
    for row in rows:
        if row["status"] == "MATCHING":
            print(f"  MATCH {row['address']} {row['target']}")
    for row in rows:
        if row["status"] != "MATCHING":
            print(
                f"  MISS  {row['address']} {row['target']}: "
                f"{row['sizes'].strip()}; {row['difference'].strip()}"
            )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
