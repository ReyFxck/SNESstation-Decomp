# HUNT500+ V19

- Base checkpoint: **461/1041**
- Final checkpoint: **464/1041 (44.57%)**
- New strict matches: **3**
- Strategy: gzerror two-byte finisher using semantic C source variants and every historical EE GCC already present locally; strict relocation-aware equality only.
- Promotion policy: historical/source-equivalent strict equality only; no target-byte injection; exact next-boundary and relocation-aware normalized equality required.
- Ambiguous duplicate/tiny fingerprints are evidence-only.
- Strict evidence: `analysis/matching/hunt500plus-v19-validated-3.tsv`

## Gate

All promotions require exact next-boundary size, relocation-aware normalized equality,
no unknown relocations, and repository validation gates.
