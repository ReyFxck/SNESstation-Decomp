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

That makes the old EE GCC 3.2.2-b1 environment a strong reproduction target.
It does **not** justify a MATCHING status yet because:

1. we do not yet have a reproducible copy of that exact compiler installed;
2. shared source may have small revision differences;
3. global structure addresses/layouts differ between programs;
4. a full function byte comparison has not yet been produced.

The first matching experiment should use `get_tree`: it is small, mostly
self-contained, and has a historical PS2 compiler listing for comparison.
