# HUNT500+ V20

- Base checkpoint: **464/1041**
- Final checkpoint: **467/1041 (44.86%)**
- New strict matches: **3**
- Strategy: zlib 1.1.3 wide sweep using the V19-proven 32-bit __SIZE_TYPE__ environment across official fossil and recovered zlib TUs, all locally available historical EE GCCs, followed by fixed-point strict closure.
- Promotion policy: strict relocation-aware equality only; recovered address anchors require encoded source address plus exact bytes; object-layout closure requires coherent independent anchors; no target-byte injection.
- Ambiguous duplicate/tiny fingerprints are evidence-only.
- Strict evidence: `analysis/matching/hunt500plus-v20-validated-3.tsv`

## Gate

All promotions require exact next-boundary size, relocation-aware normalized equality,
no unknown relocations, and repository validation gates.
