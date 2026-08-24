# Matching evidence

Files in this directory are committed proof inputs or outputs. Validated batch
TSVs are retained because `analysis/progress_targets.csv` records their paths in
the per-function notes.

Naming conventions:

- `*-validated-*.tsv` — immutable successful batch evidence.
- `*.csv` — focused comparator manifests for maintained strict gates.
- `*-listing-report.md` — human-readable results against committed target bytes.
- `*-target-spans.tsv` — frozen target-side recovery data, not automatically a
  formal promotion.

The V53 target spans are paired with the completed compiler-side proof in
`hunt1041-v72-validated-v53-6.tsv`; the frozen span file is retained as the
recovery record rather than rewritten after promotion.

V73's two strict PS2-I/O source variants are frozen in
`hunt1041-v73-validated-2.tsv`. The parallel 40-entry ownership queue is
`hunt1041-v73-frontend-map-40.tsv`; it is a frozen planning checkpoint and does
not itself promote any row.

V74's two SPC7110 RTC proofs are frozen in
`hunt1041-v74-validated-spc7110-rtc-2.tsv`. The generated
`hunt1041-v74-frontier-map-52.tsv` supersedes V73's partial queue, covers every
remaining non-matching row, and is verified by `tools/update_frontier_map.py`.

Unreferenced parameter sweeps and screening results belong under
`analysis/archive/experiments/`, not beside authoritative evidence.
