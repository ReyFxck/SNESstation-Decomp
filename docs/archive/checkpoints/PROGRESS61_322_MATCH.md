# Progress 61 — 322 strict function-level matches

Progress 61 promotes one strict source-lineage match on top of the 321/1041
checkpoint:

- `0x001a8b24` `padInfoMode`

The target's source lineage preserves the reversed signed comparison
`pdata->nrOfModes < index`. The exact PS2DEV-derived candidate compiled with
the pinned EE GCC 3.2.2 stage-one compiler using `p61-os` (`-Os`) and passed:

- `result=MATCH`
- `normalized_equal=True`
- `boundary=exact-next-boundary`
- no unknown relocation types

Machine-readable evidence:

`analysis/matching/progress61-validated-1.tsv`

Exact source snapshot:

`matching/candidates/progress61/libpad-newpadman-8x2-loop-lenint-infomode-reversed.c.txt`

Candidate hash:

`analysis/matching/progress61-candidate-sha256.txt`

Result: **322/1041 = 30.93%** strict function-level matching.

This is a function-level matching checkpoint, not a full replacement-ELF
identity claim.
