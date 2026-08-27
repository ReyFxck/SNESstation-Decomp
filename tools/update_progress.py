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
        or provider_frontier != 224
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
        or len(closure_rows) != 224
        or closure_kind_counts
        != {
            "absolute-target-anchor": 175,
            "semantic-text-alias": 9,
            "compatibility-storage": 39,
            "compatibility-runtime-shim": 1,
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
| Recovered exact results still pending formal promotion | {project_status.recovered_pending:,} | V53 recovery set is fully formal; V81 final-frontier proofs included; working checkpoint **{project_status.working_checkpoint:,}/{VALIDATED_TARGETS:,} ({project_status.working_percent:.2f}%)** |
| Reconstructed / matching | {len(reconstructed):,} | **{pct(len(reconstructed), VALIDATED_TARGETS):.2f}%** |
| Mapped (identified + partial + reconstructed) | {len(mapped):,} | **{pct(len(mapped), VALIDATED_TARGETS):.2f}%** |

The README graphic is generated to [`assets/progress.svg`](../assets/progress.svg). Its 200 cells are a largest-remainder visualization of this same {VALIDATED_TARGETS:,}-entry universe.

## Source-form checkpoint

All {VALIDATED_TARGETS:,} validated entries now have a behavioral/source-model
representation and **{pseudocode_only_count:,}** remain only as structural
pseudocode after typed promotions. The separate Stage-2 gate compiles the
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

| Checkpoint | Count | Meaning |
|---|---:|---|
| Formal `MATCHING` manifest | **{project_status.formal_matching:,}/{project_status.total:,} ({project_status.formal_percent:.2f}%)** | Promoted rows with immutable compiler/object evidence. |
| Recovered exact results still pending promotion | **{project_status.recovered_pending:,}** | All recovered results are formal; V81 closed the final 20 raw-exact function spans. |
| Working checkpoint | **{project_status.working_checkpoint:,}/{project_status.total:,} ({project_status.working_percent:.2f}%)** | Formal rows plus any disjoint recovered-but-unpromoted results. |
| Working frontier | **{project_status.working_remaining:,}** | Audited entries not yet formally matched or covered by recovered evidence. |
| Build-ready EE source ownership | **97/97 TUs** | 96 canonical objects partially link with a frozen ABI/symbol map and no duplicate/common definitions. |
| Source-address alias tranche | **{alias_proved}/{len(alias_rows)} proved** | Zero-byte linker aliases bind proven alternate names to canonical global text symbols; {alias_blocked} remain blocked. |
| Zero-byte link-contract frontier | **{contract_resolved:,}/{len(contract_rows):,} resolved** | {contract_anchors:,} absolute target-address anchors plus {contract_aliases} semantic text aliases leave {contract_blocked:,} provider contracts explicit. |
| Private embedded-asset providers | **{len(provider_symbols)}/{len(expected_private_symbols)} resolved** | Five verified private bundles emit {provider_bytes:,} ignored bytes and reduce the active frontier to {provider_frontier:,}. |
| Source-link provider namespace | **{len(closure_rows)}/{len(active_provider_symbols)} resolved** | {closure_kind_counts['absolute-target-anchor']} target anchors, {closure_kind_counts['semantic-text-alias']} text aliases, {closure_kind_counts['compatibility-storage']} storage definitions and {closure_kind_counts['compatibility-runtime-shim']} EE shims reduce aggregate externals to zero. |
| Original Stage-3C named-data tranche | **54/54 adjudicated; {named_data_fingerprinted} exact target ranges + {named_data_statuses['SOURCE_REFACTOR_CLOSED']} closed source refactors** | All target objects are fingerprinted, {len(exact_named_data)} compatibility stores are replaced, and {len(exact_clusters)} overlap-aware private-reference clusters cover {exact_cluster_bytes:,} unique bytes. |
| Original Stage-3E named-contract tranche | **212/212 adjudicated; {named_contract_fingerprinted} fingerprinted ranges/data aliases + {named_contract_statuses['SOURCE_REFACTOR_CLOSED']} closed source refactors** | {named_contract_statuses['TEXT_ALIAS_PROVED']} text aliases, {named_contract_statuses['TARGET_RANGE_PROVED']} target ranges, two target entries, two external addresses and one canonical data alias remove all remaining compatibility storage. |
| Stage-3D libgcc subtranche | **7/7 closed; {libgcc_statuses['ARCHIVE_TEXT_EXACT']} exact archive members + {libgcc_statuses['SOURCE_REFACTOR_CLOSED']} source refactors** | {libgcc_exact_bytes:,} complete member-text bytes and {libgcc_relocations} relocations are privately verified; only `snprintf` remains a compatibility runtime shim. |
| Unpacked layout oracle | **1 section / 13 blocks / 51 windows** | Byte-free hashes freeze the private target geometry and locate the first rebuilt-image difference. |
| Complete replacement ELF | **No** | Function matching alone does not prove the final linked and packed binary. |

The six V53 recoveries, the two V73 PS2-I/O proofs, the two V74 SPC7110 RTC
proofs and the V75 C4 float/math batch remain formal `MATCHING` rows. V76
proves the complete 580-byte `C4SprDisintegrate` function, V77 proves the
complete 472-byte `C4DrawWireFrame` function, V78 proves the complete
584-byte `C4BitPlaneWave` function, and V79 proves the complete 952-byte
`C4ConvOAM` function. V80 proves 23 complete non-C4 spans totaling 14,244
bytes with raw-exact assembly reconstructions. V81 proves the final 20 spans,
covering another 71,384 bytes, and closes the audited function frontier at
1,041/1,041. The frontier map now contains 0 entries.
V82 then freezes the first whole-program measurement gate without publishing
the private image: one SJCRUNCH2 section, thirteen blocks and fifty-one hash
windows. V83 freezes the first 257 source-address aliases; V84 extends the
cumulative proof to {alias_proved} of {len(alias_rows)} aliases against
{alias_targets} canonical global text symbols and applies them without changing
any allocated section bytes; {alias_blocked} boundary/archive rows remain
explicit blockers.
V85/V89 froze the preceding 1,594-name post-Stage-3C aggregate. V90 removes
twenty target-absent instruction/stack adapters and canonicalizes `errno` to
the existing target word. V91 then removes three compiler-generated lift
artifacts, so the live source now has 1,893 externals and the
alias-resolved contract map has {len(contract_rows):,} rows. It still resolves
{contract_resolved:,} contracts without allocating a byte: {contract_anchors:,}
target-address data anchors and {contract_aliases} semantic aliases. The
remaining {contract_blocked:,} names are the explicit provider frontier. The
V86 private-reference gate then materializes five embedded-asset bundles,
proves all {len(provider_symbols)} source-level data/size symbols, and reduces
that frontier to {provider_frontier:,} without changing any existing allocated
section. The live V91 closure classifies all {len(closure_rows)} remaining
source-link names and proves an aggregate external count of zero. V87 remains
the historical pre-refactor 248-row checkpoint. V88 opened the original
54-row Stage-3C audit. V89 closes it: 50 real target objects carry exact
private-reference fingerprints in {len(exact_clusters)} overlap-aware clusters
covering {exact_cluster_bytes:,} unique bytes, while four invented source
adapters are proved absent and removed. V90 then closes the historical
212-row Stage-3E ledger: {named_contract_statuses['TARGET_RANGE_PROVED']} exact
target ranges plus one canonical data alias are fingerprinted, all seven zlib
peers bind to recovered target text, and {named_contract_statuses['SOURCE_REFACTOR_CLOSED']}
source-only contracts are eliminated. Across Stages 3C and 3E, 196 provider
names occupy {len(combined_clusters)} overlap-aware clusters covering
{combined_cluster_bytes:,} unique bytes; compatibility storage falls from 39
to zero. V91 closes all seven libgcc contracts: four complete archive-member
text sections totaling {libgcc_exact_bytes:,} bytes are exact after masking
{libgcc_relocations} relocation-controlled words, while three source-only
compiler libcalls are removed. `snprintf` is the sole remaining runtime shim.
The current batch is documented in
[`V91_STAGE3D_LIBGCC_CLOSED.md`](V91_STAGE3D_LIBGCC_CLOSED.md); V90 remains documented in
[`V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md`](V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md);
V89 remains documented in [`V89_STAGE3C_CLOSED.md`](V89_STAGE3C_CLOSED.md); V88 remains documented
in [`V88_STAGE3C_NAMED_DATA.md`](V88_STAGE3C_NAMED_DATA.md); V87 remains documented
in [`V87_PROVIDER_FRONTIER_CLOSED.md`](V87_PROVIDER_FRONTIER_CLOSED.md); V86 remains
documented in [`V86_PRIVATE_ASSET_PROVIDERS.md`](V86_PRIVATE_ASSET_PROVIDERS.md); V85 remains
documented in [`V85_ZERO_BYTE_LINK_FRONTIER.md`](V85_ZERO_BYTE_LINK_FRONTIER.md); V84 remains
documented in [`V84_REVIEWED_SOURCE_ALIASES.md`](V84_REVIEWED_SOURCE_ALIASES.md); the layout
oracle remains frozen in
[`V82_UNPACKED_LAYOUT_ORACLE.md`](V82_UNPACKED_LAYOUT_ORACLE.md), and the function
closure remains frozen in
[`V81_FUNCTION_FRONTIER_CLOSED.md`](V81_FUNCTION_FRONTIER_CLOSED.md).

## Final proof gates

1. **Function gate closed:** all 1,041 audited rows have exact, reproducible evidence.
2. **Source/object gate closed:** 97/97 TUs compile; 96 canonical objects have frozen ownership.
3. **Unpacked oracle closed:** section/block geometry and 64 KiB hashes are frozen.
4. **Address-alias tranche frozen:** {alias_proved}/{len(alias_rows)} are proved and zero-byte bound; the remaining {alias_blocked} are carried into the V85 provider frontier.
5. **Zero-byte link-contract tranche frozen:** {contract_resolved:,}/{len(contract_rows):,} are resolved and the exact {contract_blocked:,}-name input provider frontier is classified.
6. **Private-asset tranche closed:** {len(provider_symbols)}/{len(expected_private_symbols)} provider symbols and {provider_bytes:,} bytes are privately verified.
7. **Source-link provider namespace closed:** {len(closure_rows)}/{len(active_provider_symbols)} remaining contracts resolve and the aggregate has zero undefined globals.
8. **Original Stage-3C tranche closed:** all 54 historical rows are adjudicated as {named_data_fingerprinted} exact target ranges plus {named_data_statuses['SOURCE_REFACTOR_CLOSED']} completed source refactors.
9. **Original Stage-3E tranche closed:** all 212 historical rows are adjudicated; {named_contract_fingerprinted} target-backed ranges/data aliases are fingerprinted, seven zlib peers are exact text aliases and compatibility storage falls from 39 to zero.
10. **Stage-3D libgcc subtranche closed:** all seven contracts are adjudicated as four exact archive members plus three completed source refactors; runtime shims fall from four to one.
11. Close the remaining Stage 3D libc/Newlib and PS2 runtime/archive identities, including `snprintf`.
12. Close Stage 3F by recovering ranges, bytes, BSS boundaries and overlaps for the 1,265 unnamed address contracts.
13. Reproduce data/rodata/bss layout, relocations, section alignment, linker script, object order and library order.
14. Reproduce SJCRUNCH2 packing and compare both unpacked and packed hashes.

The stable one-command interface is [`make reproduce`](../REPRODUCTION.md).
It already runs every implemented gate and intentionally stops at the first
unproven final-ELF stage.
"""

    readme = ROOT / "README.md"
    readme_text = readme.read_text(encoding="utf-8")
    start = "<!-- DECOMP_PROGRESS_START -->"
    end = "<!-- DECOMP_PROGRESS_END -->"
    block = f"""{start}
## Decompilation progress

<p align="center">
  <img src="assets/progress.svg" width="720" alt="SNES Station v0.23 decompilation progress" />
</p>

> **100% means structural coverage of the audited {VALIDATED_TARGETS:,}-entry target universe.** It is not a claim of byte matching or an exact compiler function count. See [`docs/PROGRESS.generated.md`](docs/PROGRESS.generated.md) for the full accounting.

- **Matching:** {pct(len(matching), VALIDATED_TARGETS):.2f}%
- **Formal checkpoint:** **{project_status.formal_matching:,}/{VALIDATED_TARGETS:,}** strict promoted matches
- **Recovered pending:** **{project_status.recovered_pending:,}** (the V53 recovery set is fully formal)
- **Working checkpoint:** **{project_status.working_checkpoint:,}/{VALIDATED_TARGETS:,}** with **{project_status.working_remaining:,}** entries remaining
- **Reconstructed:** **{pct(len(reconstructed), VALIDATED_TARGETS):.2f}%** ({len(reconstructed):,}/{VALIDATED_TARGETS:,} validated targets)
- **Mapped / identified:** **{pct(len(mapped), VALIDATED_TARGETS):.2f}%** ({len(mapped):,}/{VALIDATED_TARGETS:,} validated targets)
- **Source-form checkpoint:** **{source_model_count:,} behavioral/source-model + {pseudocode_only_count:,} structural-pseudocode-only**
- **Build-ready EE source ownership:** **97/97 TUs** (96 canonical + 1 alternate)
- **Source-address aliases:** **{alias_proved}/{len(alias_rows)} proved**, **{alias_blocked} blocked**
- **Zero-byte link contracts:** **{contract_resolved:,}/{len(contract_rows):,} resolved**, **{contract_blocked:,} blocked** ({contract_anchors:,} address anchors + {contract_aliases} semantic aliases)
- **Private embedded assets:** **{len(provider_symbols)}/{len(expected_private_symbols)} providers**, **{provider_bytes:,} verified private bytes**, **{provider_frontier:,} remaining externals**
- **Source-link provider namespace:** **{len(closure_rows)}/{len(active_provider_symbols)} resolved**, **0 aggregate externals** ({closure_kind_counts['absolute-target-anchor']} anchors + {closure_kind_counts['semantic-text-alias']} aliases + {closure_kind_counts['compatibility-storage']} storage + {closure_kind_counts['compatibility-runtime-shim']} shims)
- **Original Stage-3C named data:** **54/54 closed** (**{named_data_fingerprinted} exact target ranges + {named_data_statuses['SOURCE_REFACTOR_CLOSED']} completed source refactors**; {named_data_statuses['ADDRESS_PROVED']} address-only remain)
- **Original Stage-3E named contracts:** **212/212 closed** (**{named_contract_fingerprinted} fingerprinted ranges/data aliases + {named_contract_statuses['TEXT_ALIAS_PROVED']} text aliases + {named_contract_statuses['SOURCE_REFACTOR_CLOSED']} completed source refactors**; compatibility storage **39 → 0**)
- **Stage-3D libgcc contracts:** **7/7 closed** (**{libgcc_statuses['ARCHIVE_TEXT_EXACT']} exact archive members + {libgcc_statuses['SOURCE_REFACTOR_CLOSED']} completed source refactors**; runtime shims **4 → 1**)
- **Unpacked layout oracle:** **1 section / 13 blocks / 51 hash windows**
- **Complete replacement ELF:** **not yet**
- **Renderer draw family:** **{pct(len(draw_recon), len(draw)):.1f}% reconstructed / {pct(len(draw_mapped), len(draw)):.1f}% mapped**

The renderer-specific grid lives in [`docs/PROGRESS.generated.md`](docs/PROGRESS.generated.md); the build/matching audit lives in [`docs/SOURCE_COMPLETENESS.generated.md`](docs/SOURCE_COMPLETENESS.generated.md).
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
