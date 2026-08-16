# Progress 68 — ps2lib-era NEW_PADMAN `padInit`: 334 strict matches

Progress 68 recovers `padInit` from the ps2lib-era pad library migrated into
PS2SDK in commit `01d625018c3fde3044292446c910b8ea508adfdc`.

The exact historical source identifies itself as PS2LIB pad rev 1.3
(2003-04-16). Selecting its `NEW_PADMAN` path and compiling with the pinned
EE GCC 3.2.2 `-Os` profile produces a strict match for:

- `0x001a8484` `padInit`
- size: 400 bytes
- `differing_bytes=0`
- `normalized_equal=True`
- boundary: `exact-next-boundary`
- no unknown relocation types

The committed objdump listing elides four runs of zero words with literal
`...` markers. Progress 68 reconstructs only those four bracketed gaps, for
64 bytes total, and the promotion gate independently verifies that every
corresponding candidate byte is zero.

Evidence:

- `analysis/matching/progress68-validated-1.tsv`
- `analysis/matching/progress68-inferred-zero-ranges.tsv`
- `analysis/matching/progress68-source-provenance.txt`
- `analysis/matching/progress68-candidate-sha256.txt`

Frozen source:

- `matching/candidates/progress68/libpad-ps2lib-2004-newpadman.c.txt`

Checkpoint: **334/1041 = 32.08%** strict function-level matching.

Remaining to 350: **16**.

This remains a function-level matching checkpoint, not a claim of a complete
byte-identical replacement ELF.
