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

Unreferenced parameter sweeps and screening results belong under
`analysis/archive/experiments/`, not beside authoritative evidence.
