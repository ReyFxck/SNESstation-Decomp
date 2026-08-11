#!/usr/bin/env python3
"""Extract the audited Progress 17 structural-source closure.

The input is the exact 74-function output produced by
``DecompileProgress17.java`` with the Emotion Engine processor.  Every target
is retained, but every decompiler warning must match one of the explicitly
classified warning families below.  The raw-output hash, per-function hashes,
warning text and target manifest make the checkpoint reproducible and keep the
remaining type uncertainty visible.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TARGETS = ROOT / "analysis" / "progress17_targets.csv"
PROGRESS = ROOT / "analysis" / "progress_targets.csv"
DEFAULT_OUTPUT = (
    ROOT / "analysis" / "functions" / "progress17_r5900_pseudocode.c.txt"
)
DEFAULT_MANIFEST = ROOT / "analysis" / "progress17_recovered_targets.csv"

EXPECTED_TARGETS = 74
EXPECTED_RAW_SHA256 = (
    "e8ee13a63a1eef1dd9dc6a825e161beb4a0625e6e67a98cb59f294ba4e365cff"
)

# Names are used only where the target behavior and historical source ordering
# independently agree.  All other newly discovered functions keep an address
# label rather than receiving a plausible but unproven symbol.
KNOWN_NAMES = {
    0x00143390: "RenderLine",
    0x001434AC: "S9xEndScreenRefresh",
    0x00146720: "DrawBGMode7Background16Add",
    0x00146E6C: "DrawBGMode7Background16Add1_2",
    0x00147578: "DrawBGMode7Background16Sub",
    0x00147C68: "DrawBGMode7Background16Sub1_2",
    0x001493A8: "DrawBGMode7Background16Add_i",
    0x0014A784: "DrawBGMode7Background16Add1_2_i",
    0x0014BABC: "DrawBGMode7Background16Sub_i",
    0x0014CE24: "DrawBGMode7Background16Sub1_2_i",
    0x0014EC54: "S9xUpdateScreen",
    0x00170398: "snapshot_Freeze",
    0x001728D4: "snapshot_Unfreeze",
}

WARNING_RULES = (
    (
        re.compile(
            r"Globals starting with '_' overlap smaller symbols at the same address"
        ),
        "global-label-overlap",
    ),
    (
        re.compile(r"Removing unreachable block \(ram,0x[0-9a-f]+\)"),
        "unreachable-block",
    ),
    (re.compile(r"Subroutine does not return"), "nonreturn-annotation"),
    (
        re.compile(r"This function may have set the stack pointer"),
        "stack-pointer",
    ),
    (
        re.compile(r"Do nothing block with infinite loop"),
        "intentional-infinite-loop",
    ),
    (
        re.compile(r"Type propagation algorithm not settling"),
        "type-propagation",
    ),
)


def read_csv(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as handle:
        return list(csv.DictReader(handle))


def parse_sections(text: str) -> dict[int, str]:
    marker = re.compile(r"/\* ===== (0x[0-9a-fA-F]+) ===== \*/\n")
    matches = list(marker.finditer(text))
    sections: dict[int, str] = {}
    for index, match in enumerate(matches):
        end = matches[index + 1].start() if index + 1 < len(matches) else len(text)
        body = text[match.end():end]
        body = body.split("/* attempted=", 1)[0].rstrip() + "\n"
        address = int(match.group(1), 16)
        if address in sections:
            raise SystemExit(f"duplicate Ghidra section: 0x{address:08x}")
        sections[address] = body
    return sections


def classify_warnings(body: str, address: int) -> tuple[list[str], list[str]]:
    warnings = re.findall(r"/\* WARNING: (.*?) \*/", body)
    categories: list[str] = []
    for warning in warnings:
        category = next(
            (
                name
                for pattern, name in WARNING_RULES
                if pattern.fullmatch(warning)
            ),
            None,
        )
        if category is None:
            raise SystemExit(
                f"0x{address:08x}: unclassified Ghidra warning: {warning!r}"
            )
        if category not in categories:
            categories.append(category)
    return warnings, categories


def confidence_for(categories: list[str]) -> str:
    if "type-propagation" in categories:
        return "medium-low"
    if categories:
        return "medium"
    return "medium-high"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("input", type=Path, help="combined Ghidra decompiler output")
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    args = parser.parse_args()

    raw = args.input.read_bytes()
    raw_digest = hashlib.sha256(raw).hexdigest()
    if raw_digest != EXPECTED_RAW_SHA256:
        raise SystemExit(
            "unexpected raw Ghidra output hash: "
            f"{raw_digest} (expected {EXPECTED_RAW_SHA256})"
        )
    text = raw.decode("utf-8")
    if "/* attempted=74 completed=74 */" not in text:
        raise SystemExit("Ghidra completion marker is missing or changed")

    target_rows = read_csv(TARGETS)
    progress_rows = {
        row["address"].lower(): row for row in read_csv(PROGRESS)
    }
    if len(target_rows) != EXPECTED_TARGETS:
        raise SystemExit(f"expected 74 target rows, found {len(target_rows)}")

    target_addresses = {int(row["address"], 16) for row in target_rows}
    sections = parse_sections(text)
    if len(sections) != EXPECTED_TARGETS or set(sections) != target_addresses:
        raise SystemExit("Progress 17 target and Ghidra address sets diverged")
    for address, body in sections.items():
        if (
            "/* decompiler failed:" in body
            or "/* no function could be created */" in body
        ):
            raise SystemExit(f"0x{address:08x}: incomplete Ghidra decompile")

    args.output.parent.mkdir(parents=True, exist_ok=True)
    header = f"""/*
 * Progress 17 structural-source closure for SNES Station v0.23 WIP.
 *
 * Generated from the exact unpacked target with Ghidra 10.4 and
 * ghidra-emotionengine-reloaded 2.1.10 (r5900:LE:32:default).
 * Raw 74-function output SHA-256: {EXPECTED_RAW_SHA256}
 *
 * This is address-labelled analysis pseudocode, not build-ready C and not a
 * claim of compiler matching.  Ghidra warnings are preserved verbatim and
 * classified in analysis/progress17_recovered_targets.csv.  Absolute DAT_*
 * names and placeholder scalar types retain unresolved target evidence.
 */

"""
    chunks = [header]
    recovered: list[dict[str, str]] = []
    for target in target_rows:
        address = int(target["address"], 16)
        address_text = f"0x{address:08x}"
        body = sections[address]
        warnings, categories = classify_warnings(body, address)
        section = f"/* ===== {address_text} ===== */\n{body}\n"
        chunks.append(section)

        prior = progress_rows.get(address_text)
        if target["source"] == "mapped-promotion":
            if prior is None:
                raise SystemExit(f"{address_text}: mapped promotion is not tracked")
            name = prior["name"]
            name_evidence = "existing binary-derived mapped name retained"
        else:
            if prior is not None and not prior["notes"].startswith("Progress 17:"):
                raise SystemExit(
                    f"{address_text}: new target already exists outside Progress 17"
                )
            name = KNOWN_NAMES.get(address, f"snes_p17_{address:08x}")
            name_evidence = (
                "name validated against Snes9x 1.41 source order and target behavior"
                if address in KNOWN_NAMES
                else "address label retained because the historical symbol is unproven"
            )

        evidence = "complete R5900 structural decompile"
        if categories:
            evidence += "; pinned warnings: " + ", ".join(categories)
        evidence += "; " + name_evidence
        recovered.append({
            "address": address_text,
            "name": name,
            "source": target["source"],
            "prior_status": target["prior_status"],
            "area": target["area"],
            "confidence": confidence_for(categories),
            "first_call_site": target["first_call_site"],
            "call_count": target["call_count"],
            "pseudocode_lines": str(body.count("\n")),
            "warning_count": str(len(warnings)),
            "warning_categories": " | ".join(categories),
            "warning_text": " | ".join(warnings),
            "pseudocode_sha256": hashlib.sha256(
                body.encode("utf-8")
            ).hexdigest(),
            "evidence": evidence,
        })

    args.output.write_text("".join(chunks), encoding="utf-8")
    with args.manifest.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(
            handle, fieldnames=list(recovered[0]), lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(recovered)

    warning_rows = sum(int(row["warning_count"]) != 0 for row in recovered)
    warning_count = sum(int(row["warning_count"]) for row in recovered)
    print(f"wrote {args.output.relative_to(ROOT)} ({len(recovered)} functions)")
    print(f"wrote {args.manifest.relative_to(ROOT)}")
    print(
        f"warning-free={len(recovered) - warning_rows} "
        f"warning-bearing={warning_rows} warnings={warning_count}"
    )


if __name__ == "__main__":
    main()
