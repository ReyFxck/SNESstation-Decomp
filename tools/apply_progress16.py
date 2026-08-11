#!/usr/bin/env python3
"""Apply and validate the 85.05% Progress 16 structural-source checkpoint."""
from __future__ import annotations

import csv
import hashlib
import re
import shutil
import subprocess
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"
RECOVERED = ROOT / "analysis" / "progress16_recovered_targets.csv"
PSEUDOCODE = ROOT / "analysis" / "functions" / "progress16_r5900_pseudocode.c.txt"
JAL = ROOT / "analysis" / "jal_candidates.csv"
EXPECTED_ADDITIONS = 165
EXPECTED_WARNING_ADDRESSES = {
    "0x001029c4", "0x0010c094", "0x0010c1f8", "0x0010d734",
    "0x0012c444", "0x0012c4a8", "0x0012e04c", "0x001a5228",
    "0x001a54c8",
}


def read_csv(path: Path) -> tuple[list[str], list[dict[str, str]]]:
    with path.open(newline="", encoding="utf-8") as handle:
        reader = csv.DictReader(handle)
        if not reader.fieldnames:
            raise SystemExit(f"missing CSV header: {path}")
        return list(reader.fieldnames), list(reader)


def counts_for(rows: list[dict[str, str]]) -> Counter[str]:
    return Counter(row["status"] for row in rows)


def parse_pseudocode() -> dict[str, str]:
    text = PSEUDOCODE.read_text(encoding="utf-8")
    marker = re.compile(r"/\* ===== (0x[0-9a-f]+) ===== \*/\n")
    matches = list(marker.finditer(text))
    sections: dict[str, str] = {}
    for index, match in enumerate(matches):
        end = matches[index + 1].start() if index + 1 < len(matches) else len(text)
        body = text[match.end():end].rstrip() + "\n"
        sections[match.group(1)] = body
    return sections


def validate_recovery_evidence() -> list[dict[str, str]]:
    _, recovered = read_csv(RECOVERED)
    if len(recovered) != EXPECTED_ADDITIONS:
        raise SystemExit(
            f"expected {EXPECTED_ADDITIONS} recovered rows, found {len(recovered)}"
        )
    addresses = [row["address"].lower() for row in recovered]
    if len(set(addresses)) != len(addresses):
        raise SystemExit("duplicate Progress 16 recovery address")

    _, jal_rows = read_csv(JAL)
    candidates = {row["target"].lower(): row for row in jal_rows}
    sections = parse_pseudocode()
    if set(sections) != set(addresses):
        raise SystemExit("Progress 16 pseudocode/manifest address sets diverged")

    warning_addresses = {
        row["address"].lower() for row in recovered if int(row["warning_count"])
    }
    if warning_addresses != EXPECTED_WARNING_ADDRESSES:
        raise SystemExit("Progress 16 reviewed-warning address set changed")

    for row in recovered:
        address = row["address"].lower()
        candidate = candidates.get(address)
        if candidate is None:
            raise SystemExit(f"{address}: not present in JAL candidates")
        if candidate["first_call_site"].lower() != row["first_call_site"].lower():
            raise SystemExit(f"{address}: first caller changed")
        body = sections[address]
        digest = hashlib.sha256(body.encode("utf-8")).hexdigest()
        if digest != row["pseudocode_sha256"]:
            raise SystemExit(f"{address}: pseudocode hash mismatch")
        if body.count("\n") != int(row["pseudocode_lines"]):
            raise SystemExit(f"{address}: pseudocode line count mismatch")
        warning_count = body.count("WARNING:")
        if warning_count != int(row["warning_count"]):
            raise SystemExit(f"{address}: warning count mismatch")
        if warning_count > 2:
            raise SystemExit(f"{address}: ambiguous warning set was not approved")
    return recovered


def check_input_baseline(rows: list[dict[str, str]]) -> bool:
    counts = counts_for(rows)
    reconstructed = counts["RECONSTRUCTED"] + counts["MATCHING"]
    mapped = sum(counts.values()) - counts["UNKNOWN"]
    if (reconstructed, mapped) == (802, 827):
        return False
    if (reconstructed, mapped) == (967, 992):
        return True
    raise SystemExit(
        "Progress 16 expects the P15 or already-applied P16 baseline; got "
        f"reconstructed={reconstructed} mapped={mapped}"
    )


def add_rows(path: Path, recovered: list[dict[str, str]]) -> list[dict[str, str]]:
    fields, rows = read_csv(path)
    existing = {row["address"].lower(): row for row in rows}
    for item in recovered:
        address = item["address"].lower()
        if item["name"].startswith("snes_p16_"):
            name_evidence = (
                "address label retained because the historical symbol is unproven"
            )
        else:
            name_evidence = (
                "name validated against Snes9x 1.41 source order and target behavior"
            )
        note = (
            "Progress 16: " + item["evidence"] + "; "
            f"{item['pseudocode_lines']} pseudocode lines, "
            f"sha256 {item['pseudocode_sha256'][:16]}; "
            + name_evidence
        )
        if address in existing:
            row = existing[address]
            if row["status"] != "RECONSTRUCTED":
                raise SystemExit(f"{path}: {address} exists with status {row['status']}")
            if not row["notes"].startswith("Progress 16:"):
                raise SystemExit(f"{path}: refusing to replace pre-Progress-16 row {address}")
            row.update({
                "name": item["name"],
                "confidence": item["confidence"],
                "notes": note,
            })
            if "area" in row:
                row["area"] = item["area"]
            continue
        row = {field: "" for field in fields}
        row.update({
            "address": address,
            "name": item["name"],
            "status": "RECONSTRUCTED",
            "confidence": item["confidence"],
            "notes": note,
        })
        if "area" in row:
            row["area"] = item["area"]
        rows.append(row)
        existing[address] = row

    rows.sort(key=lambda row: int(row["address"], 16))
    tmp = path.with_suffix(path.suffix + ".p16tmp")
    with tmp.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    tmp.replace(path)
    return rows


def validate_manifest_pair(targets: list[dict[str, str]],
                           symbols: list[dict[str, str]]) -> str:
    target_map = {row["address"].lower(): row["status"] for row in targets}
    symbol_map = {row["address"].lower(): row["status"] for row in symbols}
    if len(target_map) != len(targets) or len(symbol_map) != len(symbols):
        raise SystemExit("duplicate address in progress manifests")
    if target_map != symbol_map:
        raise SystemExit("progress_targets.csv and symbols.csv diverged")

    counts = Counter(target_map.values())
    reconstructed = counts["RECONSTRUCTED"] + counts["MATCHING"]
    mapped = sum(counts.values()) - counts["UNKNOWN"]
    if reconstructed != 967 or mapped != 992:
        raise SystemExit(
            f"unexpected P16 totals: reconstructed={reconstructed} mapped={mapped}"
        )
    return (
        f"matching={counts['MATCHING']} reconstructed={reconstructed} mapped={mapped} "
        f"partial={counts['PARTIAL']} identified={counts['IDENTIFIED']}"
    )


def syntax_check() -> str:
    cc = shutil.which("cc")
    if cc is None:
        return "host syntax check: SKIPPED (cc not found)"
    files = sorted((ROOT / "src").rglob("*.c"))
    for source in files:
        subprocess.run(
            [cc, "-std=c11", "-Wall", "-Wextra", "-Werror", "-fsyntax-only",
             "-I", str(ROOT / "include"), str(source)],
            cwd=ROOT,
            check=True,
        )
    return f"host syntax check: PASS ({len(files)} recovered C translation units)"


def main() -> None:
    recovered = validate_recovery_evidence()
    _, before = read_csv(TARGETS)
    already_applied = check_input_baseline(before)

    targets = add_rows(TARGETS, recovered)
    symbols = add_rows(SYMBOLS, recovered)
    totals = validate_manifest_pair(targets, symbols)

    subprocess.run(
        ["python3", str(ROOT / "tools" / "update_progress.py")],
        cwd=ROOT,
        check=True,
    )
    syntax_line = syntax_check()

    area_counts = Counter(row["area"] for row in recovered)
    warning_rows = sum(int(row["warning_count"]) != 0 for row in recovered)
    validation = ROOT / "analysis" / "progress16_validation.txt"
    validation.write_text(
        "Progress 16 validation\n"
        "======================\n"
        f"{totals}\n"
        "expected proxy: reconstructed=967/1137=85.05%, mapped=992/1137=87.25%\n"
        f"new structural recoveries: {len(recovered)}\n"
        f"warning-free decompiles: {len(recovered) - warning_rows}\n"
        f"short reviewed-warning decompiles: {warning_rows}\n"
        "reviewed-warning targets: "
        + ", ".join(sorted(EXPECTED_WARNING_ADDRESSES)) + "\n"
        "areas: " + ", ".join(
            f"{area}={count}" for area, count in sorted(area_counts.items())
        ) + "\n"
        "processor: Ghidra 10.4 + ghidra-emotionengine-reloaded 2.1.10, "
        "r5900:LE:32:default\n"
        "matching remains zero; structural decompilation is not a byte match\n"
        f"{syntax_line}\n",
        encoding="utf-8",
    )

    print(totals)
    print(syntax_line)
    print("already applied" if already_applied else "Progress 16 applied successfully")


if __name__ == "__main__":
    main()
