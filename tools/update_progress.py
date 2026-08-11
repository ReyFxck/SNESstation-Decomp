#!/usr/bin/env python3
"""Generate progress docs and a GitHub-friendly SVG overview.

The project-wide percentage is intentionally a *coverage proxy* against the
1,137 heuristic JAL targets currently found by scan_calls.py. It is not a claim
that the ELF contains exactly 1,137 functions.
"""
from __future__ import annotations

import csv
import math
from html import escape
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "analysis" / "progress_targets.csv"
OUT = ROOT / "docs" / "PROGRESS.generated.md"
SVG_OUT = ROOT / "assets" / "progress.svg"
JAL_CANDIDATE_PROXY = 1137

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


def _quantized_proxy_cells(counts: dict[str, int]) -> list[str]:
    """Map status counts onto a fixed 200-cell proxy grid.

    Largest-remainder apportionment keeps the grid total exact while making
    the visible proportions track the same 1,137-target denominator used by
    the percentages. UNKNOWN is the untracked remainder.
    """
    mapped_total = sum(counts.values())
    unknown_count = max(0, JAL_CANDIDATE_PROXY - mapped_total)
    all_counts = {
        "MATCHING": counts.get("MATCHING", 0),
        "RECONSTRUCTED": counts.get("RECONSTRUCTED", 0),
        "PARTIAL": counts.get("PARTIAL", 0),
        "IDENTIFIED": counts.get("IDENTIFIED", 0),
        "UNKNOWN": unknown_count,
    }
    exact = {k: v * SVG_CELLS / JAL_CANDIDATE_PROXY for k, v in all_counts.items()}
    base = {k: math.floor(v) for k, v in exact.items()}
    left = SVG_CELLS - sum(base.values())
    for k in sorted(exact, key=lambda x: (exact[x] - base[x]), reverse=True)[:left]:
        base[k] += 1

    cells: list[str] = []
    # Strongest evidence first, then progressively weaker coverage, then blank.
    for status in ("MATCHING", "RECONSTRUCTED", "PARTIAL", "IDENTIFIED", "UNKNOWN"):
        cells.extend([status] * base[status])
    return cells[:SVG_CELLS]


def _write_svg(status_counts: dict[str, int]) -> None:
    cells = _quantized_proxy_cells(status_counts)
    matching = status_counts.get("MATCHING", 0)
    reconstructed = matching + status_counts.get("RECONSTRUCTED", 0)
    mapped = sum(status_counts.values())

    width, height = 720, 450
    left, top = 42, 105
    cell, gap = 28, 4
    grid_w = SVG_COLS * cell + (SVG_COLS - 1) * gap

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
        ("UNKNOWN", "unmapped proxy"),
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
<desc id="desc">{pct(reconstructed, JAL_CANDIDATE_PROXY):.2f}% reconstructed and {pct(mapped, JAL_CANDIDATE_PROXY):.2f}% mapped using a 1,137-target heuristic coverage proxy.</desc>
<style>
  .title {{ font: 700 18px -apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif; fill: #c9d1d9; }}
  .sub {{ font: 600 13px -apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif; fill: #8b949e; }}
  .legend {{ font: 11px -apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif; fill: #8b949e; }}
</style>
<rect width="100%" height="100%" rx="12" fill="#0d1117"/>
<text x="30" y="28" class="title">SNES Station v0.23 — decompilation progress</text>
<text x="30" y="48" class="sub">{pct(reconstructed, JAL_CANDIDATE_PROXY):.2f}% reconstructed · {pct(mapped, JAL_CANDIDATE_PROXY):.2f}% mapped · {pct(matching, JAL_CANDIDATE_PROXY):.2f}% matching</text>
{''.join(legends)}
{''.join(rects)}
</svg>'''
    SVG_OUT.write_text(svg, encoding="utf-8")


def main() -> None:
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

    _write_svg(status_counts)

    text = f"""# Generated progress snapshot

> Generated by `tools/update_progress.py`. Do not hand-edit this file.

## Project-wide coverage proxy

The current call-target scanner finds **{JAL_CANDIDATE_PROXY:,} heuristic JAL targets** in the analysis image. This is a deliberately conservative denominator and is **not** asserted to be the exact function count.

| Metric | Count | Proxy |
|---|---:|---:|
| Matching | {len(matching)} | **{pct(len(matching), JAL_CANDIDATE_PROXY):.2f}%** |
| Reconstructed / matching | {len(reconstructed)} | **{pct(len(reconstructed), JAL_CANDIDATE_PROXY):.2f}%** |
| Mapped (identified + partial + reconstructed) | {len(mapped)} | **{pct(len(mapped), JAL_CANDIDATE_PROXY):.2f}%** |

The README graphic is generated to [`assets/progress.svg`](../assets/progress.svg). Its 200 cells are a quantized visualization of this same 1,137-target proxy, inspired by the compact visual progress summaries used by established decompilation projects.

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
    OUT.write_text(text, encoding="utf-8")

    readme = ROOT / "README.md"
    readme_text = readme.read_text(encoding="utf-8")
    start = "<!-- DECOMP_PROGRESS_START -->"
    end = "<!-- DECOMP_PROGRESS_END -->"
    block = f"""{start}
## Decompilation progress

<p align="center">
  <img src="assets/progress.svg" width="720" alt="SNES Station v0.23 decompilation progress" />
</p>

> Percentages are evidence-based proxies, not a claim that the complete ELF has exactly {JAL_CANDIDATE_PROXY:,} functions. See [`docs/PROGRESS.generated.md`](docs/PROGRESS.generated.md) for the measurement rules.

- **Matching:** {pct(len(matching), JAL_CANDIDATE_PROXY):.2f}%
- **Reconstructed:** **{pct(len(reconstructed), JAL_CANDIDATE_PROXY):.2f}%** ({len(reconstructed)} tracked targets)
- **Mapped / identified:** **{pct(len(mapped), JAL_CANDIDATE_PROXY):.2f}%** ({len(mapped)} tracked targets)
- **Renderer draw family:** **{pct(len(draw_recon), len(draw)):.1f}% reconstructed / {pct(len(draw_mapped), len(draw)):.1f}% mapped**

The renderer-specific 30-function grid and status legend live in [`docs/PROGRESS.generated.md`](docs/PROGRESS.generated.md).
{end}"""
    if start in readme_text and end in readme_text:
        before = readme_text.split(start, 1)[0]
        after = readme_text.split(end, 1)[1]
        readme_text = before + block + after
    else:
        anchor = "> **Status:** active reverse engineering. The project is not yet buildable as a complete replacement ELF.\n"
        readme_text = readme_text.replace(anchor, anchor + "\n" + block + "\n")
    readme.write_text(readme_text, encoding="utf-8")

    print(f"wrote {OUT.relative_to(ROOT)}, {SVG_OUT.relative_to(ROOT)} and README progress block")
    print(f"matching={len(matching)} reconstructed={len(reconstructed)} mapped={len(mapped)}")
    print(f"draw_reconstructed={len(draw_recon)}/{len(draw)} draw_mapped={len(draw_mapped)}/{len(draw)}")


if __name__ == "__main__":
    main()
