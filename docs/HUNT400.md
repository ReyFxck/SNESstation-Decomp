# HUNT400 batch

Base checkpoint: **352/1041** at `53ec00fadbdf8a526ad5f35943f1e7170724b7cf`.
Strict matches added: **4**.
Resulting checkpoint: **356/1041 = 34.20%**.
Goal 400 reached: **no**.

The batch accepts only relocation-aware strict comparator matches or anchored raw `.text` spans with no relocations and exact target bytes. Target-derived mass `.word` reconstruction is excluded.

## Evidence

- `analysis/matching/hunt400-validated-4.tsv`
- `analysis/matching/hunt400-object-provenance.tsv`
- `analysis/matching/hunt400-inferred-zero-ranges.tsv`

## New strict functions

- `0x0018d918` `explode` — snes9x-1.41-pinned-source / name-strict / exact-next-boundary
- `0x0018e92c` `LoadFollowers` — snes9x-1.41-pinned-source / name-strict / exact-next-boundary
- `0x0018eeb4` `partial_clear` — snes9x-1.41-pinned-source / name-strict / exact-next-boundary
- `0x0018ff6c` `unzOpenCurrentFile` — snes9x-1.41-pinned-source / name-strict / exact-next-boundary
