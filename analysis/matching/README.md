# Matching evidence

This directory contains immutable function-comparison manifests and reports.
The formal function table cites these files; exploratory sweeps belong under
`analysis/archive/`.

| File form | Meaning |
|---|---|
| `*-validated-*.tsv` | Successful strict compiler/object comparisons |
| `*.csv` | Inputs for maintained focused comparators |
| `*-listing-report.md` | Human-readable results against committed listing bytes |
| `*-target-spans.tsv` | Frozen target-side recovery facts; not a promotion by itself |
| `*-frontier-map-*.tsv` | Historical work queues retained as evidence, not current status |

A formal match requires complete function boundaries, a reproducible candidate
object, known relocation handling and zero differing target bytes. The current
result is **1,041/1,041**; the remaining project work is whole-image linking,
not another function-count hunt.

Compiler-candidate history is summarized in
[`../../docs/RECOVERY_HISTORY.md`](../../docs/RECOVERY_HISTORY.md).
