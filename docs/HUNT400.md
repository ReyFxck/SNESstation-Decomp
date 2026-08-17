# HUNT400 batch

Base checkpoint: **356/1041** at `cc5045cdbc2f0ae48b7850b8e9b7223b31295e76`.
Strict matches added: **1**.
Resulting checkpoint: **357/1041 = 34.29%**.
Goal 400 reached: **no**.

The batch accepts only relocation-aware strict comparator matches or anchored raw `.text` spans with no relocations and exact target bytes. Target-derived mass `.word` reconstruction is excluded.

## Evidence

- `analysis/matching/hunt400-validated-1.tsv`
- `analysis/matching/hunt400-object-provenance.tsv`
- `analysis/matching/hunt400-inferred-zero-ranges.tsv`

## New strict functions

- `0x001308f8` `snes_p13_gate_predicate_001308f8` — snes9x-1.41-pinned-source / anonymous-strict-fingerprint / exact-next-boundary
