#!/usr/bin/env python3
"""Scan C translation units with the historical EE compiler front end.

This is deliberately a diagnostic baseline rather than a fake "full build":
the stage-one compiler has no PS2SDK/Newlib target headers or libraries.  The
report distinguishes missing-header failures from real parser/type failures so
the next migration work can be prioritized without drowning in compiler output.
"""
from __future__ import annotations

import argparse
import csv
import os
import shlex
import subprocess
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


@dataclass(frozen=True)
class Result:
    source: str
    status: str
    returncode: int
    diagnostic: str


def discover_sources(paths: list[Path]) -> list[Path]:
    found: set[Path] = set()
    for raw in paths:
        path = raw if raw.is_absolute() else ROOT / raw
        if path.is_file() and path.suffix == ".c":
            found.add(path.resolve())
        elif path.is_dir():
            found.update(item.resolve() for item in path.rglob("*.c"))
        else:
            raise FileNotFoundError(f"source path does not exist: {raw}")
    return sorted(found, key=lambda item: str(item))


def first_diagnostic(text: str) -> str:
    lines = [line.strip() for line in text.splitlines() if line.strip()]
    priorities = ("fatal error:", "error:", "warning:")
    for needle in priorities:
        for line in lines:
            if needle in line:
                return line[:1000]
    return lines[0][:1000] if lines else ""


def classify(returncode: int, output: str) -> str:
    if returncode == 0:
        return "PASS"
    lower = output.lower()
    if "internal compiler error" in lower or "segmentation fault" in lower:
        return "COMPILER_CRASH"
    if "no such file or directory" in lower and (
        "fatal error:" in lower or "cannot find" in lower
    ):
        return "MISSING_HEADER"
    return "EE_C_ERROR"


def scan_one(compiler: str, flags: list[str], source: Path) -> Result:
    command = [compiler, *flags, "-fsyntax-only", str(source)]
    process = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        env={**os.environ, "LC_ALL": "C"},
    )
    output = process.stdout
    return Result(
        source=str(source.relative_to(ROOT)),
        status=classify(process.returncode, output),
        returncode=process.returncode,
        diagnostic=first_diagnostic(output),
    )


def write_reports(output_dir: Path, results: list[Result], compiler: str, flags: list[str]) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    csv_path = output_dir / "report.csv"
    with csv_path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream)
        writer.writerow(("source", "status", "returncode", "diagnostic"))
        for row in results:
            writer.writerow((row.source, row.status, row.returncode, row.diagnostic))

    counts: dict[str, int] = {}
    for row in results:
        counts[row.status] = counts.get(row.status, 0) + 1

    md = [
        "# Historical EE translation-unit scan",
        "",
        "This is a front-end compatibility baseline, not a link or matching claim.",
        "",
        f"- Compiler: `{compiler}`",
        f"- Flags: `{' '.join(flags)}`",
        f"- Translation units: **{len(results)}**",
        f"- PASS: **{counts.get('PASS', 0)}**",
        f"- MISSING_HEADER: **{counts.get('MISSING_HEADER', 0)}**",
        f"- EE_C_ERROR: **{counts.get('EE_C_ERROR', 0)}**",
        f"- COMPILER_CRASH: **{counts.get('COMPILER_CRASH', 0)}**",
        "",
        "| Source | Status | First diagnostic |",
        "|---|---|---|",
    ]
    for row in results:
        diagnostic = row.diagnostic.replace("|", "\\|")
        md.append(f"| `{row.source}` | **{row.status}** | {diagnostic} |")
    md.append("")
    (output_dir / "report.md").write_text("\n".join(md), encoding="utf-8")


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True)
    parser.add_argument("--flags", default="")
    parser.add_argument("--output-dir", type=Path, required=True)
    parser.add_argument("--jobs", type=int, default=2)
    parser.add_argument("--strict", action="store_true")
    parser.add_argument("paths", nargs="+", type=Path)
    return parser


def main() -> int:
    args = build_parser().parse_args()
    if args.jobs < 1 or args.jobs > 32:
        raise SystemExit("--jobs must be between 1 and 32")
    flags = shlex.split(args.flags)
    sources = discover_sources(args.paths)
    if not sources:
        raise SystemExit("no C translation units found")

    print(f"EE source scan: {len(sources)} translation units, jobs={args.jobs}")
    results: list[Result] = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = {
            pool.submit(scan_one, args.compiler, flags, source): source
            for source in sources
        }
        for index, future in enumerate(as_completed(futures), start=1):
            result = future.result()
            results.append(result)
            print(f"[{index:03d}/{len(sources):03d}] {result.status:14s} {result.source}")

    results.sort(key=lambda item: item.source)
    write_reports(args.output_dir, results, args.compiler, flags)

    passed = sum(row.status == "PASS" for row in results)
    missing = sum(row.status == "MISSING_HEADER" for row in results)
    errors = sum(row.status == "EE_C_ERROR" for row in results)
    crashes = sum(row.status == "COMPILER_CRASH" for row in results)
    print(
        "EE source scan summary: "
        f"PASS={passed} MISSING_HEADER={missing} EE_C_ERROR={errors} "
        f"COMPILER_CRASH={crashes}"
    )
    print(f"report: {args.output_dir / 'report.md'}")
    if crashes:
        return 2
    if args.strict and passed != len(results):
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
