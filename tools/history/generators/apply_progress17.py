#!/usr/bin/env python3
"""Apply and validate the Progress 17 structural 100% checkpoint."""
from __future__ import annotations

import csv
import hashlib
import re
import shutil
import subprocess
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
TARGETS = ROOT / "analysis" / "progress_targets.csv"
SYMBOLS = ROOT / "analysis" / "symbols.csv"
JAL = ROOT / "analysis" / "jal_candidates.csv"
P17_TARGETS = ROOT / "analysis" / "progress17_targets.csv"
REJECTED = ROOT / "analysis" / "progress17_rejected_jal_candidates.csv"
RECOVERED = ROOT / "analysis" / "progress17_recovered_targets.csv"
PSEUDOCODE = (
    ROOT / "analysis" / "functions" / "progress17_r5900_pseudocode.c.txt"
)

CODE_START = 0x00100000
CODE_END = 0x001B0800
EXPECTED_RAW_JAL = 1137
EXPECTED_REJECTED = 292
EXPECTED_VALIDATED = 1041
EXPECTED_NON_JAL = 196
EXPECTED_TARGETS = 74
EXPECTED_NEW = 49
EXPECTED_PROMOTIONS = 25
EXPECTED_WARNING_ROWS = 57
EXPECTED_WARNINGS = 65

EXPECTED_FILE_SHA256 = {
    P17_TARGETS: "0f30289c18bf767a3863925ff7f58fa59897199f129366ed8e9b63d30488e259",
    REJECTED: "5263dec508b39829b7477815a44fcca35c4b423a9102fb08fc58c5fe8d7c16e6",
    RECOVERED: "2918a2194be053bf00407e097a145d9de41634fe3645dc8be1d6b507753a3655",
    PSEUDOCODE: "6bb50e4c28635b490f71b2a124962ce408e58c4bbe7108008a3c731991696876",
}

EXPECTED_WARNING_OCCURRENCES = Counter({
    "global-label-overlap": 40,
    "unreachable-block": 14,
    "nonreturn-annotation": 8,
    "stack-pointer": 1,
    "intentional-infinite-loop": 1,
    "type-propagation": 1,
})


def read_csv(path: Path) -> tuple[list[str], list[dict[str, str]]]:
    with path.open(newline="", encoding="utf-8") as handle:
        reader = csv.DictReader(handle)
        if not reader.fieldnames:
            raise SystemExit(f"missing CSV header: {path}")
        return list(reader.fieldnames), list(reader)


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def validate_pinned_files() -> None:
    for path, expected in EXPECTED_FILE_SHA256.items():
        actual = sha256(path)
        if actual != expected:
            raise SystemExit(
                f"{path.relative_to(ROOT)} hash changed: {actual} (expected {expected})"
            )


def parse_pseudocode() -> dict[str, str]:
    text = PSEUDOCODE.read_text(encoding="utf-8")
    marker = re.compile(r"/\* ===== (0x[0-9a-f]+) ===== \*/\n")
    matches = list(marker.finditer(text))
    sections: dict[str, str] = {}
    for index, match in enumerate(matches):
        end = matches[index + 1].start() if index + 1 < len(matches) else len(text)
        body = text[match.end():end].rstrip() + "\n"
        address = match.group(1)
        if address in sections:
            raise SystemExit(f"duplicate pseudocode section: {address}")
        sections[address] = body
    return sections


def warning_category(warning: str) -> str:
    if warning == "Globals starting with '_' overlap smaller symbols at the same address":
        return "global-label-overlap"
    if re.fullmatch(r"Removing unreachable block \(ram,0x[0-9a-f]+\)", warning):
        return "unreachable-block"
    exact = {
        "Subroutine does not return": "nonreturn-annotation",
        "This function may have set the stack pointer": "stack-pointer",
        "Do nothing block with infinite loop": "intentional-infinite-loop",
        "Type propagation algorithm not settling": "type-propagation",
    }
    try:
        return exact[warning]
    except KeyError as exc:
        raise SystemExit(f"unclassified committed warning: {warning!r}") from exc


def validate_recovery_evidence() -> tuple[
    list[dict[str, str]], dict[str, dict[str, str]]
]:
    validate_pinned_files()
    _, target_rows = read_csv(P17_TARGETS)
    _, recovered = read_csv(RECOVERED)
    if len(target_rows) != EXPECTED_TARGETS or len(recovered) != EXPECTED_TARGETS:
        raise SystemExit("Progress 17 requires exactly 74 targets and recoveries")

    target_map = {row["address"].lower(): row for row in target_rows}
    recovered_map = {row["address"].lower(): row for row in recovered}
    if len(target_map) != len(target_rows) or len(recovered_map) != len(recovered):
        raise SystemExit("duplicate Progress 17 recovery address")
    if set(target_map) != set(recovered_map):
        raise SystemExit("Progress 17 target/recovery address sets diverged")

    source_counts = Counter(row["source"] for row in target_rows)
    if source_counts != Counter({
        "untracked-code-jal": EXPECTED_NEW,
        "mapped-promotion": EXPECTED_PROMOTIONS,
    }):
        raise SystemExit(f"unexpected Progress 17 source counts: {source_counts}")

    sections = parse_pseudocode()
    if set(sections) != set(recovered_map):
        raise SystemExit("Progress 17 pseudocode/manifest address sets diverged")

    warning_occurrences: Counter[str] = Counter()
    warning_rows = 0
    warning_total = 0
    for address, row in recovered_map.items():
        target = target_map[address]
        for field in (
            "source", "prior_status", "area", "first_call_site", "call_count"
        ):
            if row[field] != target[field]:
                raise SystemExit(f"{address}: recovery {field} changed")

        body = sections[address]
        if hashlib.sha256(body.encode("utf-8")).hexdigest() != row["pseudocode_sha256"]:
            raise SystemExit(f"{address}: pseudocode hash mismatch")
        if body.count("\n") != int(row["pseudocode_lines"]):
            raise SystemExit(f"{address}: pseudocode line count mismatch")

        warnings = re.findall(r"/\* WARNING: (.*?) \*/", body)
        manifest_warnings = row["warning_text"].split(" | ") if row["warning_text"] else []
        if warnings != manifest_warnings or len(warnings) != int(row["warning_count"]):
            raise SystemExit(f"{address}: warning evidence changed")
        manifest_categories = (
            row["warning_categories"].split(" | ")
            if row["warning_categories"] else []
        )
        actual_categories: list[str] = []
        for warning in warnings:
            category = warning_category(warning)
            warning_occurrences[category] += 1
            if category not in actual_categories:
                actual_categories.append(category)
        if actual_categories != manifest_categories:
            raise SystemExit(f"{address}: warning categories changed")
        warning_total += len(warnings)
        warning_rows += bool(warnings)

    if warning_rows != EXPECTED_WARNING_ROWS or warning_total != EXPECTED_WARNINGS:
        raise SystemExit(
            f"unexpected warning totals: rows={warning_rows}, warnings={warning_total}"
        )
    if warning_occurrences != EXPECTED_WARNING_OCCURRENCES:
        raise SystemExit(f"warning taxonomy changed: {warning_occurrences}")
    return recovered, target_map


def validate_universe() -> str:
    _, jal_rows = read_csv(JAL)
    _, rejected_rows = read_csv(REJECTED)
    if len(jal_rows) != EXPECTED_RAW_JAL or len(rejected_rows) != EXPECTED_REJECTED:
        raise SystemExit("raw-JAL or rejected-pattern total changed")
    jal = {row["target"].lower(): row for row in jal_rows}
    rejected = {row["target"].lower(): row for row in rejected_rows}
    if len(jal) != len(jal_rows) or len(rejected) != len(rejected_rows):
        raise SystemExit("duplicate raw-JAL or rejected-pattern address")
    if not set(rejected).issubset(jal):
        raise SystemExit("rejected target is absent from raw JAL scan")

    reasons = Counter(row["reason"] for row in rejected_rows)
    expected_reasons = Counter({
        "target-outside-confirmed-code": 258,
        "data-only-call-site": 34,
    })
    if reasons != expected_reasons:
        raise SystemExit(f"rejection reasons changed: {reasons}")
    for address, row in rejected.items():
        candidate = jal[address]
        if (
            row["first_call_site"].lower() != candidate["first_call_site"].lower()
            or row["call_count"] != candidate["call_count"]
        ):
            raise SystemExit(f"{address}: rejected scanner evidence changed")
        target = int(address, 16)
        caller = int(row["first_call_site"], 16)
        if row["reason"] == "target-outside-confirmed-code":
            valid = not (CODE_START <= target < CODE_END)
        else:
            valid = CODE_START <= target < CODE_END and not (
                CODE_START <= caller < CODE_END
            )
        if not valid:
            raise SystemExit(f"{address}: rejection no longer satisfies its rule")

    accepted = set(jal).difference(rejected)
    if len(accepted) != EXPECTED_RAW_JAL - EXPECTED_REJECTED:
        raise SystemExit("accepted JAL arithmetic changed")
    return (
        f"{len(jal)} raw JAL - {len(rejected)} rejected + "
        f"{EXPECTED_NON_JAL} non-JAL = {EXPECTED_VALIDATED} validated"
    )


def counts_for(rows: list[dict[str, str]]) -> Counter[str]:
    return Counter(row["status"] for row in rows)


def check_input_baseline(rows: list[dict[str, str]]) -> bool:
    counts = counts_for(rows)
    reconstructed = counts["RECONSTRUCTED"] + counts["MATCHING"]
    mapped = len(rows) - counts["UNKNOWN"]
    if len(rows) == 992 and (reconstructed, mapped) == (967, 992):
        return False
    if len(rows) == EXPECTED_VALIDATED and (
        reconstructed, mapped
    ) == (EXPECTED_VALIDATED, EXPECTED_VALIDATED):
        return True
    raise SystemExit(
        "Progress 17 expects the P16 or already-applied P17 baseline; got "
        f"rows={len(rows)} reconstructed={reconstructed} mapped={mapped}"
    )


def apply_rows(
    path: Path,
    recovered: list[dict[str, str]],
    target_map: dict[str, dict[str, str]],
) -> list[dict[str, str]]:
    fields, rows = read_csv(path)
    existing = {row["address"].lower(): row for row in rows}
    if len(existing) != len(rows):
        raise SystemExit(f"{path}: duplicate address")

    for item in recovered:
        address = item["address"].lower()
        current = existing.get(address)
        source = item["source"]
        if source == "mapped-promotion":
            if current is None:
                raise SystemExit(f"{path}: missing mapped promotion {address}")
            if current["status"] not in {
                item["prior_status"], "RECONSTRUCTED"
            }:
                raise SystemExit(
                    f"{path}: {address} has unexpected status {current['status']}"
                )
        elif current is not None and not current["notes"].startswith("Progress 17:"):
            raise SystemExit(f"{path}: refusing to replace pre-P17 row {address}")

        warning_note = (
            "no Ghidra warnings"
            if item["warning_count"] == "0"
            else (
                f"{item['warning_count']} pinned Ghidra warning(s): "
                f"{item['warning_categories']}"
            )
        )
        prior_note = ""
        if source == "mapped-promotion":
            prior_note = "; prior mapping evidence: " + target_map[address]["evidence"]
        note = (
            "Progress 17: " + item["evidence"] + "; "
            f"{item['pseudocode_lines']} pseudocode lines, "
            f"sha256 {item['pseudocode_sha256'][:16]}; {warning_note}; "
            f"source={source}" + prior_note
        )
        if current is None:
            current = {field: "" for field in fields}
            current["address"] = address
            rows.append(current)
            existing[address] = current
        current.update({
            "name": item["name"],
            "status": "RECONSTRUCTED",
            "confidence": item["confidence"],
            "notes": note,
        })
        if "area" in current:
            current["area"] = item["area"]

    rows.sort(key=lambda row: int(row["address"], 16))
    temp = path.with_suffix(path.suffix + ".p17tmp")
    with temp.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    temp.replace(path)
    return rows


def validate_final_pair(
    targets: list[dict[str, str]], symbols: list[dict[str, str]]
) -> str:
    target_map = {row["address"].lower(): row for row in targets}
    symbol_map = {row["address"].lower(): row for row in symbols}
    if len(target_map) != len(targets) or len(symbol_map) != len(symbols):
        raise SystemExit("duplicate address in final progress manifests")
    if set(target_map) != set(symbol_map):
        raise SystemExit("progress_targets.csv and symbols.csv address sets diverged")
    for address in target_map:
        for field in ("name", "status", "confidence", "notes"):
            if target_map[address][field] != symbol_map[address][field]:
                raise SystemExit(f"{address}: manifest field {field} diverged")

    counts = Counter(row["status"] for row in targets)
    if len(targets) != EXPECTED_VALIDATED or counts != Counter({
        "RECONSTRUCTED": EXPECTED_VALIDATED
    }):
        raise SystemExit(f"unexpected final status counts: {counts}")

    _, jal_rows = read_csv(JAL)
    _, rejected_rows = read_csv(REJECTED)
    jal = {row["target"].lower() for row in jal_rows}
    rejected = {row["target"].lower() for row in rejected_rows}
    accepted = jal - rejected
    if rejected & set(target_map):
        raise SystemExit("a rejected raw-JAL pattern is tracked as a function")
    if not accepted.issubset(target_map):
        raise SystemExit("a validated raw-JAL target is missing from the final manifest")
    non_jal = set(target_map) - jal
    if len(non_jal) != EXPECTED_NON_JAL:
        raise SystemExit(f"expected 196 non-JAL entries, found {len(non_jal)}")
    return (
        f"matching=0 reconstructed={len(targets)}/{EXPECTED_VALIDATED} "
        f"mapped={len(targets)}/{EXPECTED_VALIDATED}"
    )


def syntax_check() -> str:
    cc = shutil.which("cc")
    if cc is None:
        return "host syntax check: SKIPPED (cc not found)"
    files = sorted((ROOT / "src").rglob("*.c"))
    for source in files:
        subprocess.run(
            [
                cc, "-std=c11", "-Wall", "-Wextra", "-Werror",
                "-fsyntax-only", "-I", str(ROOT / "include"), str(source),
            ],
            cwd=ROOT,
            check=True,
        )
    return f"host syntax check: PASS ({len(files)} recovered C translation units)"


def main() -> None:
    recovered, target_map = validate_recovery_evidence()
    universe_line = validate_universe()
    _, before = read_csv(TARGETS)
    already_applied = check_input_baseline(before)

    targets = apply_rows(TARGETS, recovered, target_map)
    symbols = apply_rows(SYMBOLS, recovered, target_map)
    totals = validate_final_pair(targets, symbols)

    subprocess.run(
        ["python3", str(ROOT / "tools" / "update_progress.py")],
        cwd=ROOT,
        check=True,
    )
    syntax_line = syntax_check()

    area_counts = Counter(row["area"] for row in recovered)
    source_counts = Counter(row["source"] for row in recovered)
    validation = ROOT / "analysis" / "progress17_validation.txt"
    validation.write_text(
        "Progress 17 validation\n"
        "======================\n"
        f"{totals}\n"
        f"validated-universe arithmetic: {universe_line}\n"
        f"structural closure targets: {len(recovered)}\n"
        f"new code JAL targets: {source_counts['untracked-code-jal']}\n"
        f"mapped targets promoted: {source_counts['mapped-promotion']}\n"
        f"warning-free decompiles: {len(recovered) - EXPECTED_WARNING_ROWS}\n"
        f"warning-bearing decompiles: {EXPECTED_WARNING_ROWS}\n"
        f"pinned warning occurrences: {EXPECTED_WARNINGS}\n"
        "warning taxonomy: " + ", ".join(
            f"{name}={count}"
            for name, count in sorted(EXPECTED_WARNING_OCCURRENCES.items())
        ) + "\n"
        "areas: " + ", ".join(
            f"{area}={count}" for area, count in sorted(area_counts.items())
        ) + "\n"
        "processor: Ghidra 10.4 + ghidra-emotionengine-reloaded 2.1.10, "
        "r5900:LE:32:default\n"
        "matching remains zero; 100% is audited structural coverage, not a byte match\n"
        f"{syntax_line}\n",
        encoding="utf-8",
    )

    print(universe_line)
    print(totals)
    print(syntax_line)
    print("already applied" if already_applied else "Progress 17 applied successfully")


if __name__ == "__main__":
    main()
