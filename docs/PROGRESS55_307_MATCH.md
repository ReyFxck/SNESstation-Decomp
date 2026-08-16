# Progress 55 — 307 strict committed-listing matches

Progress 55 adds **107 unique strict matches** on top of the committed
Progress 54 checkpoint of **200/1041**, reaching **307/1041 (29.49%)**.

The 107-function union is deduplicated from nine independently generated
strict evidence files. Sweep9 itself adds no function; it confirmed that
the already-produced Progress55 evidence union had crossed 300.

## Evidence counts before deduplication

- `build/matching/progress55-300plus-sweep1/matches.tsv`: **24** rows
- `build/matching/progress55-300plus-sweep2/matches.tsv`: **12** rows
- `build/matching/progress55-300plus-sweep3/matches.tsv`: **18** rows
- `build/matching/progress55-300plus-sweep4/matches.tsv`: **11** rows
- `build/matching/progress55-300plus-sweep8-archive-miner/matches.tsv`: **1** rows
- `build/matching/progress55-screen300/matches.tsv`: **27** rows
- `build/matching/progress55-screen300c/matches.tsv`: **37** rows
- `build/matching/progress55-screen300d/matches.tsv`: **11** rows
- `build/matching/progress55-screen300b/new_matches.tsv`: **20** rows

After address deduplication: **107 unique functions**.

Frozen canonical union:
`analysis/matching/progress55-validated-107.tsv`

## Unique matches by area

- `gslib`: 37
- `libgcc`: 17
- `zlib`: 14
- `libpad`: 12
- `unzip`: 10
- `libmc`: 10
- `audio-rpc`: 2
- `libkernel`: 2
- `unwind`: 1
- `unwind-fde`: 1
- `ps2`: 1

These are function-level committed-listing strict MATCH claims using the
repository's bit-precise MIPS relocation normalization. The verified local
original unpacked ELF remains the stronger formal target gate when present.
