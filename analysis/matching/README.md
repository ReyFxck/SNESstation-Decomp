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
`hunt1041-v74-validated-spc7110-rtc-2.tsv`; its 52-entry frontier map is now a
frozen planning checkpoint.

V75's five formal C4 proofs are frozen in
`hunt1041-v75-validated-c4-5.tsv`. The exact unlisted `C4Op15` span is kept
separately in `hunt1041-v75-c4-companion-1.tsv` so it cannot accidentally
inflate the 1,041-row count. The `hunt1041-v75-frontier-map-47.tsv` file covers
every row that remained at that checkpoint and is now frozen.

V76's strict `C4SprDisintegrate` proof is frozen in
`hunt1041-v76-validated-c4spr-1.tsv`; its 46-entry frontier is now frozen.

V77's strict `C4DrawWireFrame` proof is frozen in
`hunt1041-v77-validated-c4draw-1.tsv`; its 45-entry frontier is now frozen.

V78's strict `C4BitPlaneWave` proof is frozen in
`hunt1041-v78-validated-c4bit-1.tsv`. The generated
`hunt1041-v78-frontier-map-44.tsv` is the current exhaustive queue; all earlier
frontier maps remain immutable checkpoint evidence.

Unreferenced parameter sweeps and screening results belong under
`analysis/archive/experiments/`, not beside authoritative evidence.
