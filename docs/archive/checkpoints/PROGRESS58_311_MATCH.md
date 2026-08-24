# Progress 58 — 311 strict function-level matches

Checkpoint date: 2026-08-16.

Progress 58 promotes two strict matches on top of the Progress 56 checkpoint
of 309/1041.

## Promoted functions

### `0x00199aa8` `gsPipe_setZTestEnable`

- area: GSLIB
- provenance: historical PGEN GSLIB 0.51 archive
- source: `lib/gslib051/lib/libgs.a`
- profile: `prebuilt-archive`
- object symbol: `_ZN6gsPipe14setZTestEnableEi`
- boundary proof: `object-layout-gap:0x4`
- strict result: relocation-normalized MATCH
- unknown relocations: none

### `0x0019cc0c` `SifInitRpc`

- area: SIF RPC
- provenance: recovered-source deep compiler fingerprint
- source: `src/ps2/sifrpc_recovered.c`
- profile: `deep-o2-noalignall`
- boundary proof: `exact-next-boundary`
- strict result: relocation-normalized MATCH
- unknown relocations: none

The `SifInitRpc` promotion depends on the Progress 57 historical source
correction that uses the backing `pkt_table_recovered + 64` handshake pointer.

## Evidence

Frozen machine-readable evidence:
`analysis/matching/progress58-validated-2.tsv`.

The non-mutating discovery run remains under
`build/matching/progress58-hunt350/`.

## Result

**311/1041 = 29.88%** strict function-level matching.

This is a function-level matching claim. It is not a claim that the complete
original ELF has been reproduced byte-for-byte.
