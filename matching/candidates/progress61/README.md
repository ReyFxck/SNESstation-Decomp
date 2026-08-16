# Progress 61 exact-source candidate

`libpad-newpadman-8x2-loop-lenint-infomode-reversed.c.txt` is the exact
historical-source variant that produced the strict `padInfoMode` match at
`0x001a8b24`.

The `.c.txt` suffix is intentional: the root Makefile compiles every `*.c`
under `matching/candidates/` with the host compiler. This snapshot is PS2SDK
historical-source evidence and belongs to the EE strict matching gate instead.

Compiler profile: `p61-os` (`-Os`) using the pinned EE GCC 3.2.2 stage-one
compiler.

Proof:
- `result=MATCH`
- `normalized_equal=True`
- `boundary=exact-next-boundary`
- no unknown relocation types

See `analysis/matching/progress61-validated-1.tsv`.
