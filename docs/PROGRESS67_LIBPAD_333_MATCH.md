# Progress 67 — historical libpad object layout: 333 strict matches

Progress 67 uses the historical `libpad-newpadman` object layout compiled with
the pinned EE GCC 3.2.2 `-Os` profile.

Eighteen already-MATCHING libpad functions establish a dominant target/object
load base of `0x001a83e0`.

The layout sweep proves five additional strict function-level matches:

- `0x001a8938` `padSetReqState`
- `0x001a8964` `padStateInt2String`
- `0x001a89a4` `padReqStateInt2String`
- `0x001a8f08` `padSetActAlign`
- `0x001a8fe0` `padSetActDirect`

Three prior manifest starts were boundary errors:

- `padStateInt2String`: `0x001a896c` -> `0x001a8964`
- `padReqStateInt2String`: `0x001a89ac` -> `0x001a89a4`
- `padSetActDirect`: `0x001a8fe4` -> `0x001a8fe0`

For all five, the full historical object symbol matches the target with:

- strict relocation-aware `MATCH`
- `differing_bytes=0`
- `normalized_equal=True`
- no unknown relocation types
- `object-layout-exact-next` boundary proof

Machine-readable evidence:

- `analysis/matching/progress67-validated-5.tsv`
- `analysis/matching/progress67-boundary-corrections.tsv`

Checkpoint: **333/1041 = 31.99%** strict function-level matching.

`padInit` was not corrected or promoted: its object-layout prediction could not
be tested because the committed target listing does not contain the complete
required byte range.

Already-MATCHING functions that mismatch the generic `libpad-newpadman` P63
object are not demoted. Their earlier promotion gates use more specific source
variants and remain authoritative.

This remains a function-level matching checkpoint; it is not a claim of a
complete replacement ELF.
