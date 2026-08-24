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
<text x="42" y="450" class="source">Source-form checkpoint (not build readiness)</text>
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
| Recovered exact results still pending formal promotion | {project_status.recovered_pending:,} | V53 promotion completed in V72; working checkpoint **{project_status.working_checkpoint:,}/{VALIDATED_TARGETS:,} ({project_status.working_percent:.2f}%)** |
| Reconstructed / matching | {len(reconstructed):,} | **{pct(len(reconstructed), VALIDATED_TARGETS):.2f}%** |
| Mapped (identified + partial + reconstructed) | {len(mapped):,} | **{pct(len(mapped), VALIDATED_TARGETS):.2f}%** |

The README graphic is generated to [`assets/progress.svg`](../assets/progress.svg). Its 200 cells are a largest-remainder visualization of this same {VALIDATED_TARGETS:,}-entry universe.

## Source-form checkpoint

Structural coverage is not the same as build readiness. Of the {VALIDATED_TARGETS:,}
validated entries, **{source_model_count:,}** currently have a behavioral/source-model representation
and **{pseudocode_only_count:,}** are currently represented only as structural
pseudocode after applying typed source promotions. Neither
category is a byte-matching claim. See
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
| Recovered exact results still pending promotion | **{project_status.recovered_pending:,}** | All six recovered V53 results were regenerated and promoted by V72. |
| Working checkpoint | **{project_status.working_checkpoint:,}/{project_status.total:,} ({project_status.working_percent:.2f}%)** | Formal rows plus any disjoint recovered-but-unpromoted results. |
| Working frontier | **{project_status.working_remaining:,}** | Audited entries not yet formally matched or covered by recovered evidence. |
| Complete replacement ELF | **No** | Function matching alone does not prove the final linked and packed binary. |

The six V53 recoveries are now formal `MATCHING` rows. Their regenerated
compiler objects, exact partitions, relocation checks and immutable hashes are
documented in [`V72_V53_PROMOTED.md`](V72_V53_PROMOTED.md).

## Final proof gates

1. Promote every remaining function using exact, reproducible evidence.
2. Freeze translation-unit ownership, global types and source boundaries.
3. Reproduce data/rodata/bss layout, relocations and section alignment.
4. Recover the historical archives, linker script, object order and library order.
5. Reproduce SJCRUNCH2 packing and compare both unpacked and packed hashes.

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
- **Recovered pending:** **{project_status.recovered_pending:,}** (all six V53 recoveries were promoted in V72)
- **Working checkpoint:** **{project_status.working_checkpoint:,}/{VALIDATED_TARGETS:,}** with **{project_status.working_remaining:,}** entries remaining
- **Reconstructed:** **{pct(len(reconstructed), VALIDATED_TARGETS):.2f}%** ({len(reconstructed):,}/{VALIDATED_TARGETS:,} validated targets)
- **Mapped / identified:** **{pct(len(mapped), VALIDATED_TARGETS):.2f}%** ({len(mapped):,}/{VALIDATED_TARGETS:,} validated targets)
- **Source-form checkpoint:** **{source_model_count:,} behavioral/source-model + {pseudocode_only_count:,} structural-pseudocode-only**
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
