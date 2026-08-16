# Progress 60 exact-source candidates

These are the exact generated source variants that produced the ten strict function-level matches promoted at the 321/1041 checkpoint.

All ten promoted rows use the `p60-os` (`-Os`) profile with the pinned EE GCC 3.2.2 stage-one compiler. Every promoted row has `normalized_equal=True`, `boundary=exact-next-boundary`, and no unknown relocation types.

See `analysis/matching/progress60-validated-10.tsv` and `analysis/matching/progress60-candidate-sha256.txt`.

## Host-syntax exclusion

The historical C snapshots use the `.c.txt` suffix intentionally. Their bytes are
kept as source evidence, but the root Makefile discovers every `*.c` below
`matching/candidates/` and feeds it to the host compiler. Some exact historical
PS2SDK sources require headers such as `tamtypes.h`, so compiling these evidence
snapshots as host translation units would be the wrong validation gate.
