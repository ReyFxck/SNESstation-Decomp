# Progress 3 summary

> Live scoreboard: [`PROGRESS.generated.md`](PROGRESS.generated.md). Update `analysis/progress_targets.csv` and run `python3 tools/update_progress.py` to refresh the README percentages and block grid.

Progress 3 closes the first complete renderer draw-family block, recovers the
legacy ZIP methods, maps the surrounding unzip API, and identifies the exact
embedded zlib version.

## Current scoreboard

- **98 reconstructed targets**
- **130 mapped targets**
- **8.62% reconstructed** on the conservative 1,137-JAL proxy
- **11.43% mapped** on the same proxy
- **0.00% matching** (kept deliberately strict)
- renderer draw-family subgrid: **30/30 reconstructed**

## Renderer milestone

All 30 tracked draw-family entry points from `0x0018428c` through
`0x0018bac0` now have behavior-oriented C reconstructions:

- 8-bit normal, clipped, x2 and x2x2 paths;
- 16-bit normal, clipped, x2 and x2x2 paths;
- large/mosaic pixel paths;
- Add, Sub, Add1_2, Sub1_2 and fixed-colour variants;
- normal/flipped pixel writers.

The binary proves the classic 16-bit color arithmetic through the `0x0421`,
`0xfbde` and `0x8420` masks plus the X2/ZERO lookup tables.

## Legacy ZIP + unzip API milestone

Recovered legacy methods:

- Implode/Explode: `0x0018c124..0x0018e440`
- Reduce: `0x0018e4c0..0x0018e92c`
- Shrink: `0x0018ea64..0x0018eeb4`

The surrounding unzip 0.15-style API is mapped from `0x0018f010` through
`0x00190628`. Eighteen small/medium API helpers now have C reconstructions;
five large stateful parsers/readers remain identified pending exact struct
layout work.

A notable boundary lesson remains `partial_clear` at `0x0018eeb4`: it is a
real direct-call target but has no conventional stack prologue.

## zlib 1.1.3 milestone

The target itself embeds `1.1.3` and the old deflate copyright string. The
next block therefore has an exact upstream baseline:

- `0x00190700` `compress2` — reconstructed
- `0x001907cc` `compress` — reconstructed
- `0x001907e8` `uncompress` — reconstructed
- `0x001908bc` `deflateInit_` — reconstructed
- `0x001908ec` `deflateInit2_` — identified
- `0x00190fa0` `deflate` — identified
- `0x00191308` `deflateEnd` — identified
- `0x00192784` `inflateEnd` — identified
- `0x00192948` `inflateInit_` — identified
- `0x0019296c` `inflate` — identified

The new binary frontier is the **zlib 1.1.3 Deflate/Inflate core**, beginning
at `0x001908ec`.

## Matching/toolchain lead

A close PS2 SNESticle build archive contains GCC assembly listings for shared
legacy-ZIP code. Its `get_tree` listing starts with the same R5900 prologue
machine words as SNES Station, and its Makefile records EE GCC `3.2.2-b1` plus
release flags. This remains the strongest compiler fingerprint candidate.

Matching stays at **0%** until the old toolchain is reproduced and complete
functions compare byte-for-byte.

## Validation

All current recovered C files pass host C99 syntax checking with
`-Wall -Wextra -Werror`.
