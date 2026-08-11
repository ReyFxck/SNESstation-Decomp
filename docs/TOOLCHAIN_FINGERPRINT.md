# Toolchain fingerprint

## Current candidate: EE GCC 3.2.2-b1

This is a **candidate**, not yet a proven global compiler identity for the
SNES Station v0.23 target.

A public PS2 SNESticle build tree contains release GAS listings for shared
legacy-ZIP C files and records this EE toolchain in its Makefile:

```text
EE_GCC_VERSION = 3.2.2-b1
```

Recorded release C flags:

```text
-G0 -O2 -EL -pipe -Wall -Werror -Wa,-al
-fomit-frame-pointer -fstrict-aliasing -fno-common
-ffreestanding -fno-builtin -fshort-double
-mlong64 -mhard-float -mno-abicalls
-march=r5900 -mtune=r5900
```

Recorded common defines:

```text
-DPS2_EE -D_EE -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3
```

C++ adds:

```text
-fno-exceptions -fno-common -fno-rtti
```

## Why this matters

The SNESticle release listing for `get_tree` starts with:

```text
A0FFBD27   addiu/subu sp, sp, -96
0000B0FF   sd s0, 0(sp)
```

SNES Station `get_tree` at `0x0018c124` starts with the same machine words and
the same 96-byte frame, followed by the same high-level register/data flow.
`unReduce` also has the same 176-byte frame and begins with the same first
machine word as the release listing.

The complete `get_tree` streams are not identical: the SNESticle listing is
`0xd0` bytes, while the SNES Station target range is `0xd4` bytes and uses a
different saved-register assignment plus one extra global-address reload.
This is useful negative evidence, not a MATCHING result. The isolated candidate
and reproducible comparator are exposed through `make match-get-tree`.

That makes the old EE GCC 3.2.2-b1 environment a strong reproduction target.
It does **not** justify a MATCHING status yet because:

1. we do not yet have a reproducible copy of that exact compiler installed;
2. shared source may have small revision differences;
3. global structure addresses/layouts differ between programs;
4. a full function byte comparison has not yet been produced.

The first matching experiment should use `get_tree`: it is small, mostly
self-contained, and has a historical PS2 compiler listing for comparison.

## Important negative evidence from zlib

The zlib pass added a useful correction. Historical EE GCC 3.2.2-b1 output is
close enough to establish translation-unit lineage and optimization style, but
`deflateInit2_` does **not** preserve the same saved-register assignment as the
SNES Station target. The outer structure/frame is very close while internal
register allocation differs.

Therefore the candidate should not be promoted from “strong lead” to “exact
compiler” on the basis of matching prologues. Future matching work should test
compiler patch level, flags, and exact source revision systematically.


## Progress 7: archive-level fingerprint

Progress 7 materially strengthens the GCC 3.2.2-b1 hypothesis. Independent
analysis of the target found consecutive archive-member boundaries whose sizes
are exactly those recorded by the historical EE link map: `_udivdi3.o` and
`_umoddi3.o` are each `0x6c8`; `unwind-dw2.o` is `0x1f00`;
`unwind-dw2-fde.o` is `0x18c0`; and the soft-float support sequence
`_unpack_sf/_make_sf/_pack_df/_unpack_df/_fpcmp_parts_df/_pack_sf` preserves
`0xe0/0x30/0x190/0xe8/0x108/0x160` respectively.

The two unwind objects go further: public `_Unwind_*` functions occur at the
same offsets within the object as in the historical map. This is the strongest
compiler/runtime-family fingerprint in the project so far.

It is still not blue/matching proof. Object layout can identify the linked
runtime revision while application translation units still differ due to source
revision, flags or compiler patch level. A matching status requires a
reproducible compiler plus direct byte comparison.
