# Progress 54 — 200 strict committed-listing matches

Progress 54 closes a 98-function batch on top of the 102-function Progress 53
checkpoint, reaching **200/1041 MATCHING (19.21%)**.

- Broad recovered-source/compiler screen: **82/82**.
- Historical PS2DEV per-object `-DF_<object>` source-shape recovery: **16/16**.
- Historical PS2DEV source is pinned at
  `duduclx/PS2DEV@bac0006c6302edcf1bdae253799484497b4e5032`.

No row is promoted from size, compiler acceptance, or visual similarity alone.
`tools/validate-progress54-evidence.py` reopens the generated EE objects and
compares all 98 bodies using the repository's bit-precise MIPS relocation
normalization.

The original unpacked image remains the stronger formal target gate when a
legally obtained local copy is present. Progress 54 makes function-level
committed-listing MATCH claims.

Frozen evidence:
- `analysis/matching/progress54-screen2-matches.tsv`
- `analysis/matching/progress54-historical-per-object-matches.tsv`
- `analysis/matching/progress54-validated-98.tsv`

Research reproduction:
- `tools/research/progress54_screen2.py`
- `tools/research/progress54_historical_per_object.py`
