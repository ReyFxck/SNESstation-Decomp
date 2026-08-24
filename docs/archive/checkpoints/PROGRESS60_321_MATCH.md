# Progress 60 — 321 strict function-level matches

Checkpoint date: 2026-08-16.

Progress 60 promotes ten strict source-lineage matches on top of the Progress 58 checkpoint of 311/1041.

## Promoted functions

| Address | Function | Exact candidate |
|---|---|---|
| `0x0018e440` | `FillBitBuffer` | `FillBitBuffer-bits-left-signed` |
| `0x0019d410` | `fioMkdir` | `fioMkdir-path256` |
| `0x0019e8e4` | `strncasecmp` | `strncasecmp-n-unsigned-int` |
| `0x001a17a4` | `mcRename` | `libmc-rename-copy32-getent-rpc` |
| `0x001a8420` | `padGetDmaStr` | `libpad-newpadman-8x2` |
| `0x001a8690` | `padPortOpen` | `libpad-newpadman-8x2` |
| `0x001a87b0` | `padPortClose` | `libpad-newpadman-8x2` |
| `0x001a8864` | `padRead` | `libpad-newpadman-8x2-loop-lenint` |
| `0x001a88a8` | `padGetState` | `libpad-newpadman` |
| `0x001a9080` | `padGetConnection` | `libpad-newpadman` |

Every promoted row was compiled with the pinned EE GCC 3.2.2 stage-one compiler using `p60-os` (`-Os`) and has `normalized_equal=True`, `boundary=exact-next-boundary`, and no unknown relocation types.

Machine-readable evidence: `analysis/matching/progress60-validated-10.tsv`.

Exact frozen candidates: `matching/candidates/progress60/`.

Candidate hashes: `analysis/matching/progress60-candidate-sha256.txt`.

## Result

**321/1041 = 30.84%** strict function-level matching.

This is a function-level matching checkpoint, not a claim that the full original executable or final link layout has been reproduced.
