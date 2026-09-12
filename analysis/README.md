# Analysis data and evidence

This directory contains the machine-readable state behind every public project
number. Generated documentation is a view of these files, not an independent
source of truth.

| Path | Purpose |
|---|---|
| `progress_targets.csv` | Authoritative 1,041-entry function status manifest |
| `symbols.csv` | Mirrored symbol/status manifest checked row-for-row |
| `source_promotions.csv` | Typed-source replacements for early structural models |
| `source_readiness.csv` | Generated source-form and matching audit |
| `source_tree/` | EE ABI, translation-unit and symbol-ownership manifests |
| `link_identity/` | Layout hashes, aliases, data/runtime ownership and whole-image contracts |
| `matching/` | Immutable strict comparison evidence |
| `functions/` | Address-anchored assembly and structural snapshots |
| `decompdev/` | Public-only Objdiff report contract |
| `archive/` | Superseded sweeps and validation logs |

Do not promote a function by editing one CSV in isolation. Use the evidence
promotion tools and run `make check`; the audit requires the target and symbol
manifests to agree exactly.

The current human-readable summary is
[`../docs/status/PROJECT_STATUS.generated.md`](../docs/status/PROJECT_STATUS.generated.md).
