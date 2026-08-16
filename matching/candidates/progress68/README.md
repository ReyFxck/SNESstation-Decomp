# Progress 68 ps2lib-era NEW_PADMAN source

This directory preserves the exact source variant that produced the strict
`padInit` match at `0x001a8484`.

Historical base source:

- repository: `https://github.com/ps2dev/ps2sdk.git`
- commit: `01d625018c3fde3044292446c910b8ea508adfdc`
- path: `ee/rpc/pad/src/libpad.c`
- original SHA-256:
  `4d714966fd10ffee85f0511305aa289152096bd782fa8d54c42dbc1471c44971`

Variant:

- `ROM_PADMAN` selector replaced with `NEW_PADMAN`
- compiler profile: pinned EE GCC 3.2.2 with `-Os`
- frozen variant SHA-256: `4d96a31c5f8d3f814f3bec872fadf6afd9a4cb67c12705053a9073ae4f8e76d1`

Strict result for `padInit`:

- address: `0x001a8484`
- size: `400` bytes
- `result=MATCH`
- `differing_bytes=0`
- `normalized_equal=True`
- boundary: `exact-next-boundary`
- no unknown relocation types

The committed objdump listing suppresses four zero-word runs inside `padInit`
using literal `...` markers. Progress 68 reconstructs only those bracketed
libpad-local gaps:

- `0x001a84d4..0x001a84e8` — 20 bytes
- `0x001a84f0..0x001a84fc` — 12 bytes
- `0x001a8534..0x001a8548` — 20 bytes
- `0x001a8550..0x001a855c` — 12 bytes

All 64 inferred bytes were independently verified to be literal zero bytes in
the matching historical candidate object.

The `.c.txt` suffix is intentional so the repository host-syntax target does
not compile this historical PS2SDK source as a normal host translation unit.
