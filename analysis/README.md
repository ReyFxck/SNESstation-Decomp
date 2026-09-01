# Analysis data and evidence

The root of this directory contains current machine-readable project state.
The most important files are:

| File | Role |
|---|---|
| `progress_targets.csv` | Authoritative 1,041-entry formal status manifest |
| `symbols.csv` | Mirrored symbol/status manifest checked row-for-row |
| `source_promotions.csv` | Typed-source overrides for historical pseudocode |
| `source_readiness.csv` | Generated source-form and matching audit |
| `source_tree/` | Stage-2 EE ABI, TU and defined/external ownership manifests |
| `link_identity/` | Stage-3 unpacked-layout oracle, aliases, link-contract/provider closure and hash-only named-data/private-provider evidence |
| `progress16_recovered_targets.csv` | Frozen Progress-16 structural universe |
| `progress17_recovered_targets.csv` | Frozen Progress-17 structural universe |
| `progress17_rejected_jal_candidates.csv` | Rejected data words from the raw JAL scan |

Subdirectories:

- `functions/` — committed assembly and structural snapshots anchored by address.
- `matching/` — immutable strict comparison evidence and small gate manifests.
- `link_identity/` — hash-only unpacked layout, zero-byte alias/anchor
  decisions, the closed 54-row Stage-3C and 212-row Stage-3E ledgers, the
  closed seven-row Stage-3D libgcc ledger, four formatter-call refactor proofs,
  43 runtime contracts selecting 42 complete PS2LIB member texts (two blocked), private-provider
  ranges and the complete source-link provider closure.
- `archive/` — historical validation logs and exploratory sweeps no longer used
  as live inputs.

Do not promote a row by editing only one CSV. Use the evidence promotion tools
and run `make check`; the audit requires `progress_targets.csv` and
`symbols.csv` to agree exactly.
