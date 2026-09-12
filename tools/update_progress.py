#!/usr/bin/env python3
"""Generate progress docs and a GitHub-friendly SVG overview.

Progress 17 replaces the raw JAL-pattern proxy with an audited structural
target universe.  The raw scanner finds 1,137 distinct targets; 292 are proven
post-code data patterns, while 196 independently mapped real entries have no
direct JAL hit.  The resulting denominator is 1,041 validated entries.  It is
still a structural-analysis universe, not a claim about compiler matching or
the mathematically exact number of functions in the ELF.
"""
from __future__ import annotations

import argparse
import csv
import math
from html import escape
from pathlib import Path

from project_status import load_status
import runtime_refactors
import runtime_members
import runtime_overrides
import unnamed_data
import data_backing
import historical_data
import link_layout_probe
import startup_integration
import frontend_eh_frames
import historical_tail_data
import runtime_tail_data
import tail_metadata
import window36_data
import media_assets
import window35_data
import window11_rodata
import code_windows

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "analysis" / "progress_targets.csv"
SOURCE_READINESS = ROOT / "analysis" / "source_readiness.csv"
SOURCE_ALIASES = ROOT / "analysis" / "link_identity" / "source_address_aliases.tsv"
LINK_CONTRACTS = ROOT / "analysis" / "link_identity" / "link_contracts.tsv"
PRIVATE_ASSET_PROVIDERS = ROOT / "analysis" / "link_identity" / "private_asset_providers.tsv"
PROVIDER_FRONTIER_CLOSURE = ROOT / "analysis" / "link_identity" / "provider_frontier_closure.tsv"
NAMED_DATA = ROOT / "analysis" / "link_identity" / "named_data.tsv"
NAMED_CONTRACTS = ROOT / "analysis" / "link_identity" / "named_contracts.tsv"
LIBGCC_CONTRACTS = ROOT / "analysis" / "link_identity" / "libgcc_contracts.tsv"
OUT = ROOT / "docs" / "PROGRESS.generated.md"
SVG_OUT = ROOT / "assets" / "progress.svg"
STATUS_OUT = ROOT / "docs" / "status" / "PROJECT_STATUS.generated.md"
RAW_JAL_TARGETS = 1137
REJECTED_JAL_PATTERNS = 292
NON_JAL_ENTRIES = 196
VALIDATED_TARGETS = RAW_JAL_TARGETS - REJECTED_JAL_PATTERNS + NON_JAL_ENTRIES

STATUS_ICON = {
    "MATCHING": "🟦",
    "RECONSTRUCTED": "🟩",
    "PARTIAL": "🟧",
    "IDENTIFIED": "🟨",
    "UNKNOWN": "⬜",
}

# SVG palette intentionally mirrors the status legend while remaining readable
# on both GitHub light and dark themes.
SVG_COLOR = {
    "MATCHING": "#58a6ff",
    "RECONSTRUCTED": "#3fb950",
    "PARTIAL": "#d29922",
    "IDENTIFIED": "#e3b341",
    "UNKNOWN": "#30363d",
}

DRAW_FAMILY_START = 0x0018428C
DRAW_FAMILY_END = 0x0018BAC0
SVG_COLS = 20
SVG_ROWS = 10
SVG_CELLS = SVG_COLS * SVG_ROWS


def pct(n: int, d: int) -> float:
    return 0.0 if not d else n * 100.0 / d


def _quantized_progress_cells(counts: dict[str, int]) -> list[str]:
    """Map status counts onto a fixed 200-cell validated-universe grid.

    Largest-remainder apportionment keeps the grid total exact while making
    the visible proportions track the same 1,041-entry denominator used by
    the percentages. UNKNOWN is the untracked remainder.
    """
    mapped_total = sum(counts.values())
    if mapped_total > VALIDATED_TARGETS:
        raise SystemExit(
            f"manifest has {mapped_total} rows, above the "
            f"{VALIDATED_TARGETS}-entry validated universe"
        )
    unknown_count = VALIDATED_TARGETS - mapped_total
    all_counts = {
        "MATCHING": counts.get("MATCHING", 0),
        "RECONSTRUCTED": counts.get("RECONSTRUCTED", 0),
        "PARTIAL": counts.get("PARTIAL", 0),
        "IDENTIFIED": counts.get("IDENTIFIED", 0),
        "UNKNOWN": unknown_count,
    }
    exact = {k: v * SVG_CELLS / VALIDATED_TARGETS for k, v in all_counts.items()}
    base = {k: math.floor(v) for k, v in exact.items()}
    left = SVG_CELLS - sum(base.values())
    for k in sorted(exact, key=lambda x: (exact[x] - base[x]), reverse=True)[:left]:
        base[k] += 1

    cells: list[str] = []
    # Strongest evidence first, then progressively weaker coverage, then blank.
    for status in ("MATCHING", "RECONSTRUCTED", "PARTIAL", "IDENTIFIED", "UNKNOWN"):
        cells.extend([status] * base[status])
    return cells[:SVG_CELLS]


def _render_svg(
    status_counts: dict[str, int],
    source_model_count: int,
    pseudocode_only_count: int,
) -> str:
    cells = _quantized_progress_cells(status_counts)
    matching = status_counts.get("MATCHING", 0)
    reconstructed = matching + status_counts.get("RECONSTRUCTED", 0)
    mapped = sum(status_counts.values())

    width, height = 720, 525
    left, top = 42, 105
    cell, gap = 28, 4
    grid_w = SVG_COLS * cell + (SVG_COLS - 1) * gap
    source_model_width = round(grid_w * source_model_count / VALIDATED_TARGETS)
    pseudocode_width = grid_w - source_model_width

    rects = []
    for i, status in enumerate(cells):
        row, col = divmod(i, SVG_COLS)
        x = left + col * (cell + gap)
        y = top + row * (cell + gap)
        rects.append(
            f'<rect x="{x}" y="{y}" width="{cell}" height="{cell}" rx="4" '
            f'fill="{SVG_COLOR[status]}" data-status="{status}"/>'
        )

    legends = []
    legend_items = [
        ("MATCHING", "matching"),
        ("RECONSTRUCTED", "reconstructed"),
        ("PARTIAL", "partial"),
        ("IDENTIFIED", "identified"),
        ("UNKNOWN", "unmapped"),
    ]
    lx, ly = 30, 78
    for status, label in legend_items:
        legends.append(
            f'<rect x="{lx}" y="{ly - 12}" width="12" height="12" rx="2" fill="{SVG_COLOR[status]}"/>'
            f'<text x="{lx + 18}" y="{ly - 2}" class="legend">{escape(label)}</text>'
        )
        lx += 125 if status != "UNKNOWN" else 0

    svg = f'''<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" role="img" aria-labelledby="title desc">
<title id="title">SNES Station v0.23 decompilation progress</title>
<desc id="desc">{pct(reconstructed, VALIDATED_TARGETS):.2f}% structurally reconstructed, {source_model_count} entries at a behavioral source-model checkpoint, {pseudocode_only_count} entries represented only as structural pseudocode, and {pct(matching, VALIDATED_TARGETS):.2f}% machine-code matching.</desc>
<style>
  .title {{ font: 700 18px -apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif; fill: #c9d1d9; }}
  .sub {{ font: 600 13px -apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif; fill: #8b949e; }}
  .legend {{ font: 11px -apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif; fill: #8b949e; }}
  .source {{ font: 600 12px -apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif; fill: #c9d1d9; }}
</style>
<rect width="100%" height="100%" rx="12" fill="#0d1117"/>
<text x="30" y="28" class="title">SNES Station v0.23 — decompilation progress</text>
<text x="30" y="48" class="sub">{pct(reconstructed, VALIDATED_TARGETS):.2f}% reconstructed · {pct(mapped, VALIDATED_TARGETS):.2f}% mapped · {pct(matching, VALIDATED_TARGETS):.2f}% matching</text>
{''.join(legends)}
{''.join(rects)}
<text x="42" y="450" class="source">Source-form coverage (separate from object ownership)</text>
<rect x="42" y="462" width="{source_model_width}" height="16" rx="4" fill="#238636"/>
<rect x="{42 + source_model_width}" y="462" width="{pseudocode_width}" height="16" rx="4" fill="#d29922"/>
<text x="42" y="498" class="legend">{source_model_count} behavioral/source-model · {pseudocode_only_count} structural pseudocode only · complete ELF: no</text>
</svg>'''
    return svg


def _write_or_check(path: Path, content: str, check: bool) -> None:
    if check:
        if not path.is_file():
            raise SystemExit(f"missing generated file {path.relative_to(ROOT)}")
        if path.read_text(encoding="utf-8") != content:
            raise SystemExit(
                f"stale generated file {path.relative_to(ROOT)}; run make docs"
            )
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--check", action="store_true", help="fail when generated project status is stale"
    )
    args = parser.parse_args()

    rows = list(csv.DictReader(MANIFEST.open(encoding="utf-8")))
    seen: set[tuple[str, str]] = set()
    for row in rows:
        key = (row["address"].lower(), row["name"])
        if key in seen:
            raise SystemExit(f"duplicate progress target: {row['address']} {row['name']}")
        seen.add(key)
        row["addr_int"] = int(row["address"], 16)

    status_counts = {status: 0 for status in STATUS_ICON}
    for row in rows:
        status_counts[row["status"]] = status_counts.get(row["status"], 0) + 1

    reconstructed = [r for r in rows if r["status"] in {"RECONSTRUCTED", "MATCHING"}]
    mapped = [r for r in rows if r["status"] != "UNKNOWN"]
    matching = [r for r in rows if r["status"] == "MATCHING"]
    project_status = load_status(ROOT)
    if project_status.formal_matching != len(matching):
        raise SystemExit("project-status and progress-manifest matching counts differ")
    readiness = list(csv.DictReader(SOURCE_READINESS.open(encoding="utf-8")))
    readiness_by_address = {row["address"].lower(): row for row in readiness}
    manifest_addresses = {row["address"].lower() for row in rows}
    if set(readiness_by_address) != manifest_addresses:
        raise SystemExit("analysis/source_readiness.csv is stale; run tools/audit_source_completeness.py first")
    source_model_count = sum(
        row["source_form"] == "BEHAVIORAL_SOURCE_MODEL" for row in readiness
    )
    pseudocode_only_count = len(readiness) - source_model_count
    alias_rows = list(
        csv.DictReader(SOURCE_ALIASES.open(encoding="utf-8"), delimiter="\t")
    )
    alias_proved = sum(row["status"] == "PROVED" for row in alias_rows)
    alias_blocked = sum(row["status"] == "BLOCKED" for row in alias_rows)
    if not alias_rows or alias_proved + alias_blocked != len(alias_rows):
        raise SystemExit("invalid source-address alias status manifest")
    alias_targets = len(
        set(row["canonical_symbol"] for row in alias_rows if row["status"] == "PROVED")
    )
    contract_rows = list(
        csv.DictReader(LINK_CONTRACTS.open(encoding="utf-8"), delimiter="\t")
    )
    contract_resolved = sum(row["status"] == "RESOLVED" for row in contract_rows)
    contract_blocked = sum(row["status"] == "BLOCKED" for row in contract_rows)
    contract_anchors = sum(
        row["resolution_kind"] == "absolute-address-anchor" for row in contract_rows
    )
    contract_aliases = sum(
        row["resolution_kind"] == "semantic-text-alias" for row in contract_rows
    )
    if (
        not contract_rows
        or contract_resolved + contract_blocked != len(contract_rows)
        or contract_anchors + contract_aliases != contract_resolved
    ):
        raise SystemExit("invalid zero-byte link-contract manifest")
    provider_rows = list(
        csv.DictReader(PRIVATE_ASSET_PROVIDERS.open(encoding="utf-8"), delimiter="\t")
    )
    provider_symbols = {
        symbol
        for row in provider_rows
        for symbol in (row["data_symbol"], row["size_symbol"])
    }
    expected_private_symbols = {
        row["symbol"]
        for row in contract_rows
        if row["status"] == "BLOCKED" and row["provider_kind"] == "private-asset"
    }
    provider_bytes = sum(
        int(row["size_hex"], 0) + int(row["padding_hex"], 0) + 4
        for row in provider_rows
    )
    provider_frontier = contract_blocked - len(provider_symbols)
    if (
        not provider_rows
        or len(provider_rows) != 5
        or provider_symbols != expected_private_symbols
        or len(provider_symbols) != 10
        or provider_bytes != 62_736
        or provider_frontier != 223
    ):
        raise SystemExit("invalid private-asset provider manifest")
    closure_rows = list(
        csv.DictReader(PROVIDER_FRONTIER_CLOSURE.open(encoding="utf-8"), delimiter="\t")
    )
    active_provider_symbols = {
        row["symbol"]
        for row in contract_rows
        if row["status"] == "BLOCKED" and row["symbol"] not in provider_symbols
    }
    closure_symbols = {row["symbol"] for row in closure_rows}
    closure_kind_counts = {
        kind: sum(row["resolution_kind"] == kind for row in closure_rows)
        for kind in (
            "absolute-target-anchor",
            "semantic-text-alias",
            "compatibility-storage",
            "compatibility-runtime-shim",
        )
    }
    closure_storage_bytes = sum(
        int(row["storage_size_hex"], 0)
        for row in closure_rows
        if row["resolution_kind"] == "compatibility-storage"
    )
    if (
        closure_symbols != active_provider_symbols
        or len(closure_rows) != 223
        or closure_kind_counts
        != {
            "absolute-target-anchor": 175,
            "semantic-text-alias": 9,
            "compatibility-storage": 39,
            "compatibility-runtime-shim": 0,
        }
        or closure_storage_bytes != 144_630
    ):
        raise SystemExit("invalid provider-frontier closure manifest")

    named_data_rows = list(
        csv.DictReader(NAMED_DATA.open(encoding="utf-8"), delimiter="\t")
    )
    named_data_statuses = {
        status: sum(row["status"] == status for row in named_data_rows)
        for status in (
            "PRIVATE_BYTES_PROVED",
            "RANGE_PROVED",
            "ADDRESS_PROVED",
            "SOURCE_REFACTOR_CLOSED",
        )
    }
    named_data_addressed = sum(bool(row["target_address"]) for row in named_data_rows)
    named_data_fingerprinted = (
        named_data_statuses["PRIVATE_BYTES_PROVED"]
        + named_data_statuses["RANGE_PROVED"]
    )
    compatibility_storage = {
        row["symbol"]
        for row in closure_rows
        if row["resolution_kind"] == "compatibility-storage"
    }
    exact_named_data = [
        row
        for row in named_data_rows
        if row["symbol"] in compatibility_storage and row["status"] == "RANGE_PROVED"
    ]
    exact_range_rows = [
        row for row in named_data_rows if row["status"] == "RANGE_PROVED"
    ]
    exact_intervals = sorted(
        (
            int(row["target_address"], 0),
            int(row["target_address"], 0) + int(row["extent_hex"], 0),
        )
        for row in exact_range_rows
    )
    exact_clusters: list[list[int]] = []
    for start_address, end_address in exact_intervals:
        if not exact_clusters or start_address > exact_clusters[-1][1]:
            exact_clusters.append([start_address, end_address])
        else:
            exact_clusters[-1][1] = max(exact_clusters[-1][1], end_address)
    exact_cluster_bytes = sum(end - start for start, end in exact_clusters)
    if (
        len(named_data_rows) != 54
        or len({row["symbol"] for row in named_data_rows}) != 54
        or named_data_statuses
        != {
            "PRIVATE_BYTES_PROVED": 10,
            "RANGE_PROVED": 40,
            "ADDRESS_PROVED": 0,
            "SOURCE_REFACTOR_CLOSED": 4,
        }
        or named_data_addressed != 50
        or named_data_fingerprinted != 50
        or len(exact_named_data) != 32
        or len(exact_range_rows) != 40
        or len(exact_clusters) != 15
        or exact_cluster_bytes != 141_159
    ):
        raise SystemExit("invalid Stage-3C named-data manifest")

    named_contract_rows = list(
        csv.DictReader(NAMED_CONTRACTS.open(encoding="utf-8"), delimiter="\t")
    )
    named_contract_statuses = {
        status: sum(row["status"] == status for row in named_contract_rows)
        for status in (
            "TEXT_ALIAS_PROVED",
            "TARGET_RANGE_PROVED",
            "TARGET_ENTRY_PROVED",
            "EXTERNAL_ADDRESS_PROVED",
            "DATA_ALIAS_PROVED",
            "SOURCE_REFACTOR_CLOSED",
        )
    }
    named_contract_fingerprinted = sum(
        row["status"] in {"TARGET_RANGE_PROVED", "DATA_ALIAS_PROVED"}
        for row in named_contract_rows
    )
    named_contract_addressed = sum(
        bool(row["target_address"]) for row in named_contract_rows
    )
    named_contract_zlib = sum(
        row["category"] == "zlib-peer" for row in named_contract_rows
    )
    stage3e_range_rows = [
        row for row in named_contract_rows
        if row["status"] == "TARGET_RANGE_PROVED"
    ]
    stage3e_exact_storage = {
        row["symbol"]
        for row in stage3e_range_rows
        if row["symbol"] in compatibility_storage
    }
    stage3e_intervals = sorted(
        (
            int(row["target_address"], 0),
            int(row["target_address"], 0) + int(row["extent_hex"], 0),
        )
        for row in stage3e_range_rows
    )
    stage3e_clusters: list[list[int]] = []
    for start_address, end_address in stage3e_intervals:
        if not stage3e_clusters or start_address > stage3e_clusters[-1][1]:
            stage3e_clusters.append([start_address, end_address])
        else:
            stage3e_clusters[-1][1] = max(stage3e_clusters[-1][1], end_address)
    stage3e_cluster_bytes = sum(end - start for start, end in stage3e_clusters)
    combined_intervals = sorted([*exact_intervals, *stage3e_intervals])
    combined_clusters: list[list[int]] = []
    for start_address, end_address in combined_intervals:
        if not combined_clusters or start_address > combined_clusters[-1][1]:
            combined_clusters.append([start_address, end_address])
        else:
            combined_clusters[-1][1] = max(combined_clusters[-1][1], end_address)
    combined_cluster_bytes = sum(end - start for start, end in combined_clusters)
    if (
        len(named_contract_rows) != 212
        or len({row["symbol"] for row in named_contract_rows}) != 212
        or named_contract_statuses
        != {
            "TEXT_ALIAS_PROVED": 23,
            "TARGET_RANGE_PROVED": 164,
            "TARGET_ENTRY_PROVED": 2,
            "EXTERNAL_ADDRESS_PROVED": 2,
            "DATA_ALIAS_PROVED": 1,
            "SOURCE_REFACTOR_CLOSED": 20,
        }
        or named_contract_fingerprinted != 165
        or named_contract_addressed != 192
        or named_contract_zlib != 7
        or len(stage3e_exact_storage) != 7
        or len(stage3e_clusters) != 49
        or stage3e_cluster_bytes != 26_633
        or len(combined_clusters) != 61
        or combined_cluster_bytes != 167_782
    ):
        raise SystemExit("invalid Stage-3E named-contract manifest")

    libgcc_rows = list(
        csv.DictReader(LIBGCC_CONTRACTS.open(encoding="utf-8"), delimiter="\t")
    )
    libgcc_statuses = {
        status: sum(row["status"] == status for row in libgcc_rows)
        for status in ("ARCHIVE_TEXT_EXACT", "SOURCE_REFACTOR_CLOSED")
    }
    libgcc_exact_bytes = sum(
        int(row["extent_hex"], 0)
        for row in libgcc_rows
        if row["status"] == "ARCHIVE_TEXT_EXACT"
    )
    libgcc_relocations = sum(
        int(row["relocation_count"])
        for row in libgcc_rows
        if row["status"] == "ARCHIVE_TEXT_EXACT"
    )
    if (
        len(libgcc_rows) != 7
        or len({row["symbol"] for row in libgcc_rows}) != 7
        or libgcc_statuses
        != {"ARCHIVE_TEXT_EXACT": 4, "SOURCE_REFACTOR_CLOSED": 3}
        or libgcc_exact_bytes != 3_848
        or libgcc_relocations != 21
    ):
        raise SystemExit("invalid Stage-3D libgcc manifest")

    runtime_rows = runtime_refactors.validate_manifest(runtime_refactors.parse_args(["validate"]))
    runtime_closed = len({row["former_contract"] for row in runtime_rows})
    member_rows, member_objects = runtime_members.validate_manifest(runtime_members.parse_args(["validate"]))
    member_report = runtime_members.statistics(member_rows, member_objects)
    override_rows, override_witnesses = runtime_overrides.validate_manifest(runtime_overrides.parse_args(["validate"]))
    override_report = runtime_overrides.statistics(override_rows, override_witnesses)
    unnamed_rows = unnamed_data.validate_manifest(unnamed_data.parse_args(["validate"]))
    unnamed_report = unnamed_data.statistics(unnamed_rows)
    backing_rows, backing_sections = data_backing.validate(data_backing.parse_args(["validate"]))
    backing_report = data_backing.statistics(backing_rows, backing_sections)
    historical_report = historical_data.validate()
    historical_bytes = sum(row["size"] for row in historical_report["owners"])
    layout_probe = link_layout_probe.validate(link_layout_probe.parse_args(["validate"]))
    layout_probe_result = layout_probe["result"]
    startup_gate = startup_integration.validate(startup_integration.parse_args(["validate"]))
    startup_result = startup_gate["result"]
    frontend_eh_gate = frontend_eh_frames.validate(frontend_eh_frames.parse_args(["validate"]))
    frontend_eh_result = frontend_eh_gate["result"]
    historical_tail_gate = historical_tail_data.validate(historical_tail_data.parse_args(["validate"]))
    historical_tail_result = historical_tail_gate["result"]
    runtime_tail_gate = runtime_tail_data.validate(runtime_tail_data.parse_args(["validate"]))
    runtime_tail_result = runtime_tail_gate["result"]
    tail_metadata_gate = tail_metadata.validate(tail_metadata.parse_args(["validate"]))
    tail_metadata_result = tail_metadata_gate["result"]
    window36_gate = window36_data.validate(window36_data.parse_args(["validate"]))
    window36_result = window36_gate["result"]
    media_gate = media_assets.validate(media_assets.parse_args(["validate"]))
    media_result = media_gate["result"]
    window35_gate = window35_data.validate(window35_data.parse_args(["validate"]))
    window35_result = window35_gate["result"]
    window11_gate = window11_rodata.validate(window11_rodata.parse_args(["validate"]))
    window11_result = window11_gate["result"]
    code_window_gate = code_windows.validate(code_windows.parse_args(["validate"]))
    code_window_result = code_window_gate["result"]
    stage3d_closed = len(libgcc_rows) + runtime_closed + member_report["contracts_closed"] + len(override_rows)
    stage3d_remaining = 53 - stage3d_closed

    draw = [
        r for r in rows
        if r["area"] == "renderer"
        and DRAW_FAMILY_START <= r["addr_int"] <= DRAW_FAMILY_END
        and r["addr_int"] < 0x001AC000
    ]
    draw.sort(key=lambda r: r["addr_int"])
    draw_recon = [r for r in draw if r["status"] in {"RECONSTRUCTED", "MATCHING"}]
    draw_mapped = [r for r in draw if r["status"] != "UNKNOWN"]

    blocks = [STATUS_ICON[r["status"]] for r in draw]
    grid_lines = ["".join(blocks[i:i+15]) for i in range(0, len(blocks), 15)]

    svg_text = _render_svg(status_counts, source_model_count, pseudocode_only_count)

    text = f"""# Generated progress snapshot

> Generated by `tools/update_progress.py`. Do not hand-edit this file.

## Validated structural target universe

Progress 17 audits every raw call-scanner target instead of treating every decoded JAL-shaped word as code. The denominator is:

`{RAW_JAL_TARGETS:,} raw JAL targets - {REJECTED_JAL_PATTERNS} post-code data patterns + {NON_JAL_ENTRIES} independently mapped non-JAL entries = {VALIDATED_TARGETS:,} validated entries`

The rejected patterns and their reasons are recorded in [`analysis/progress17_rejected_jal_candidates.csv`](../analysis/progress17_rejected_jal_candidates.csv). This is a closed, evidence-backed **structural-analysis universe**; it is not an assertion that the ELF has exactly {VALIDATED_TARGETS:,} compiler-created functions.

| Metric | Count | Validated universe |
|---|---:|---:|
| Matching | {len(matching):,} | **{pct(len(matching), VALIDATED_TARGETS):.2f}%** |
| Recovered exact results still pending formal promotion | {project_status.recovered_pending:,} | All recovered exact results are formal; working result **{project_status.working_checkpoint:,}/{VALIDATED_TARGETS:,} ({project_status.working_percent:.2f}%)** |
| Reconstructed / matching | {len(reconstructed):,} | **{pct(len(reconstructed), VALIDATED_TARGETS):.2f}%** |
| Mapped (identified + partial + reconstructed) | {len(mapped):,} | **{pct(len(mapped), VALIDATED_TARGETS):.2f}%** |

The README graphic is generated to [`assets/progress.svg`](../assets/progress.svg). Its 200 cells are a largest-remainder visualization of this same {VALIDATED_TARGETS:,}-entry universe.

## Source-form checkpoint

All {VALIDATED_TARGETS:,} validated entries now have a behavioral/source-model
representation and **{pseudocode_only_count:,}** remain only as structural
pseudocode after typed promotions. The separate EE source gate compiles the
frozen 97-unit tree into 96 canonical EE objects plus one explicit alternate.
Source form, object ownership and original-source provenance remain distinct
claims. See
[`docs/SOURCE_COMPLETENESS.generated.md`](SOURCE_COMPLETENESS.generated.md) for
the generated invariant audit and remaining ELF gates.

## Renderer draw-family map

This grid is exact for the **30 macro-expanded draw-family entry points from `0x0018428c` through `0x0018bac0`** currently tracked in `docs/RENDERER_MAP.md`.

- **Reconstructed:** {len(draw_recon)}/{len(draw)} = **{pct(len(draw_recon), len(draw)):.1f}%**
- **Mapped:** {len(draw_mapped)}/{len(draw)} = **{pct(len(draw_mapped), len(draw)):.1f}%**

```text
{chr(10).join(grid_lines)}
```

Legend: 🟩 reconstructed · 🟨 identified · 🟧 partial · ⬜ unknown · 🟦 matching

Each square corresponds to one function boundary, in ascending address order. The address/status table is sourced from [`analysis/progress_targets.csv`](../analysis/progress_targets.csv).

## Why there are multiple percentages

A decomp project can measure different things. **Matching** means rebuilt code is proven to reproduce the target machine code; **reconstructed** means source behavior/structure has been recovered but is not yet compiler-matched; **mapped** includes high-confidence identifications that still need source reconstruction.

Until the exact original compiler/toolchain is reproduced, reconstructed and mapped coverage are more useful than the matching percentage.
"""

    status_text = f"""# Current project status

> Generated by `tools/update_progress.py`. Do not hand-edit this file.

## Headline result

| Measure | Result | Status |
|---|---:|---|
| Audited function entries | **{project_status.formal_matching:,}/{project_status.total:,} ({project_status.formal_percent:.0f}%)** | Complete |
| EE source ownership | **97/97 translation units** | Complete |
| Runtime contracts | **{stage3d_closed}/53** | Complete |
| Program-data address identities | **{backing_report['resolved_contracts']:,}/{backing_report['contracts_total']:,}** | Complete |
| Exact 64 KiB image windows | **{code_window_result['exact_chunks']}/{code_window_result['chunk_count']}** | In progress |
| Remaining image differences | **{code_window_result['differing_bytes']:,} bytes** | In progress |
| Complete replacement ELF | **Not yet** | In progress |

The function count and the whole-image count answer different questions.
**1,041/1,041** means the frozen function audit is closed. **{code_window_result['exact_chunks']}/{code_window_result['chunk_count']}** means {code_window_result['exact_chunks']}
complete 64 KiB regions of the rebuilt unpacked image match the target. The
latter is the relevant number for final linking.

## Completed proof areas

| Area | Direct result | Meaning |
|---|---:|---|
| Function matching | **{project_status.formal_matching:,}/{project_status.total:,}** | Complete-boundary compiler/object evidence for every audited entry |
| Readable source models | **{source_model_count:,}/{VALIDATED_TARGETS:,}** | Every audited entry has a behavioral/source representation |
| Source ownership | **97/97 units** | 96 canonical EE objects plus one explicit alternate compile with the frozen ABI |
| Source-address aliases | **{alias_proved}/{len(alias_rows)} proved** | {alias_blocked} intentionally blocked names remain outside the alias claim |
| Zero-byte link contracts | **{contract_resolved:,}/{len(contract_rows):,}** | {contract_anchors:,} address anchors and {contract_aliases} semantic aliases |
| Source-link providers | **{len(closure_rows)}/{len(active_provider_symbols)}** | The recovered relocatable aggregate has zero undefined globals |
| Named data | **54/54** | {named_data_fingerprinted} exact target ranges and {named_data_statuses['SOURCE_REFACTOR_CLOSED']} completed source refactors |
| Named link contracts | **212/212** | {named_contract_fingerprinted} fingerprinted ranges/data aliases and {named_contract_statuses['SOURCE_REFACTOR_CLOSED']} completed source refactors |
| Compiler-runtime contracts | **7/7** | Four exact archive members plus three proved source refactors |
| Runtime contracts | **{stage3d_closed}/53** | PS2LIB member text and target-selected `puts`/`abort` behavior accounted for |
| Address identity | **{backing_report['resolved_contracts']:,}/{backing_report['contracts_total']:,}** | Every tracked address has a provider/refactor identity; full object bounds remain separate |
| Historical data | **{len(historical_report['owners'])} intervals / {historical_bytes:,} bytes** | {backing_report['historical_source_bytes']:,} bytes are freshly rebuilt from pinned public source |
| Exact startup | **{startup_result['startup_exact_bytes']} bytes / {startup_result['startup_functions_exact']} functions** | Target entry and {startup_result['startup_relocations_applied']} relocations reproduce exactly |
| Embedded media | **{media_result['media_sections']} containers / {media_result['media_asset_bytes']:,} bytes** | Privately verified and integrated without committing payload data |

## Whole-image comparison

| Measure | Result |
|---|---:|
| Unpacked target size | **{code_window_result['target_initialized_size']:,} bytes** |
| Exact windows | **1–6 and 11–50** |
| Remaining windows | **0 and 7–10** |
| Exact-window coverage | **{code_window_result['exact_chunks']}/{code_window_result['chunk_count']} ({pct(code_window_result['exact_chunks'], code_window_result['chunk_count']):.2f}%)** |
| Remaining different bytes | **{code_window_result['differing_bytes']:,}** |
| Proved source integrated across windows 1–6 | **{code_window_result['source_bytes']:,} bytes** |
| Newly labelled exact scheduling residual | **{code_window_result['residual_bytes']:,} bytes** |

## Still open

1. Close image windows 0 and 7–10.
2. Prove complete data/object bounds needed by the final link.
3. Reproduce the exact linker script, section placement, object/archive order
   and remaining relocation results.
4. Reproduce the SJCRUNCH2/LZO stub and packed container.
5. Match both frozen target hashes.

The original ELF and generated private payloads remain ignored. Public status
is derived only from committed manifests; private checks compare a
user-supplied reference without publishing its bytes.
"""

    readme = ROOT / "README.md"
    readme_text = readme.read_text(encoding="utf-8")
    start = "<!-- DECOMP_PROGRESS_START -->"
    end = "<!-- DECOMP_PROGRESS_END -->"
    block = f"""{start}
## Current status

| Measure | Result | What it means | Status |
|---|---:|---|---|
| Audited function entries | **{project_status.formal_matching:,}/{VALIDATED_TARGETS:,} ({project_status.formal_percent:.0f}%)** | Every entry in the frozen audit has complete-boundary matching evidence and a readable source model. | Complete |
| EE source ownership | **97/97 translation units** | All recovered units compile with the historical EE ABI; 96 canonical objects form the duplicate-free source aggregate. | Complete |
| Runtime contracts | **{stage3d_closed}/53** | Every tracked PS2LIB, libc, libgcc and target-selected runtime dependency has an evidence-backed provider or refactor. | Complete |
| Address identities | **{backing_report['resolved_contracts']:,}/{backing_report['contracts_total']:,}** | Every tracked program-data address has a proved identity; exact full object bounds are a separate question. | Complete |
| Whole-image windows | **{code_window_result['exact_chunks']}/{code_window_result['chunk_count']} ({pct(code_window_result['exact_chunks'], code_window_result['chunk_count']):.2f}%)** | 64 KiB windows **1–6 and 11–50** match the unpacked reference exactly. Windows **0 and 7–10** remain. | In progress |
| Remaining byte differences | **{code_window_result['differing_bytes']:,}** | Byte positions still different in the {code_window_result['target_initialized_size']:,}-byte unpacked image. | In progress |
| Replacement ELF | **Not yet** | Final object order, linker layout, remaining relocations and SJCRUNCH2 packing are not fully reproduced. | In progress |

The **{project_status.formal_matching:,}/{VALIDATED_TARGETS:,}** result measures the audited function frontier. It does not
mean the complete ELF is already identical. The whole-image result is the
direct measure for final linking progress.

Detailed machine-generated counts are in
[`docs/status/PROJECT_STATUS.generated.md`](docs/status/PROJECT_STATUS.generated.md).
{end}"""
    if start in readme_text and end in readme_text:
        before = readme_text.split(start, 1)[0]
        after = readme_text.split(end, 1)[1]
        readme_text = before + block + after
    else:
        anchor = "> **Status:** active reverse engineering. The project is not yet buildable as a complete replacement ELF.\n"
        readme_text = readme_text.replace(anchor, anchor + "\n" + block + "\n")
    _write_or_check(OUT, text, args.check)
    _write_or_check(SVG_OUT, svg_text, args.check)
    _write_or_check(STATUS_OUT, status_text, args.check)
    _write_or_check(readme, readme_text, args.check)

    action = "verified" if args.check else "wrote"
    print(
        f"{action} {OUT.relative_to(ROOT)}, {SVG_OUT.relative_to(ROOT)}, "
        f"{STATUS_OUT.relative_to(ROOT)} and README progress block"
    )
    print(f"matching={len(matching)} reconstructed={len(reconstructed)} mapped={len(mapped)}")
    print(f"draw_reconstructed={len(draw_recon)}/{len(draw)} draw_mapped={len(draw_mapped)}/{len(draw)}")


if __name__ == "__main__":
    main()
