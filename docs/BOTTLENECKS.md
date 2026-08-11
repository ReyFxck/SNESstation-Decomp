# Bottleneck watch — SNES Station v0.23 WIP

## Bottlenecks that did not become walls

- PS2 startup/RPC wrappers are structurally visible.
- LLVM R5900 decode gaps are classified (859/859 in the selected EE range).
- All 30 tracked renderer draw-family entries through `0x0018bac0` are reconstructed.
- 16-bit Add/Sub/Half color math is recovered.
- legacy PKZIP Implode/Explode, Reduce and Shrink are recovered.
- unzip 0.15-style API is mapped/reconstructed.
- the embedded baseline is exactly zlib **1.1.3**; Deflate/Inflate, trees,
  checksums and the PS2 `gzio.c` layer are behaviorally reconstructed.
- the post-zlib graphics corridor is no longer opaque: target evidence proves
  Hiryu GSLIB `gsDriver`, `gsPipe`, `gsFont`, and `hw.c` through `0x0019be6c`;
- the following CDVD/SIF RPC/FileIO/loadfile/IOP client corridor is reconstructed
  through `SifIopReset @ 0x0019d740`, with late-linked helpers at `0x0019f5d0+`.

## Current binary frontier

Two large contiguous third-party/shared-library corridors are now bounded:

1. zlib 1.1.3: `0x00190700..0x00198c54`;
2. Hiryu GSLIB slice: `0x00198c58..0x0019be6c`.

The former CDVD/RPC frontier has been crossed. `CDVD_Init @ 0x0019be70`
through `SifIopReset @ 0x0019d740` is now separated into historical libcdvd,
string helpers, SIF RPC, FileIO, loadfile and IOP-heap/control code. The next
large contiguous runtime frontier is old PS2LIB `vsnprintf.o` at `0x0019d84c`.

Current priorities:

1. reconstruct the old PS2LIB formatter core without confusing it with Newlib's later formatter;
2. recover SNES Station frontend/config/input paths around the already known GUI;
3. continue Snes9x 1.41 structure/layout recovery outside the completed draw family;
4. attack audio/SjPCM/AmigaMod glue and EE/IOP scheduling;
5. continue the exact-toolchain track without promoting near matches to MATCHING.

## Parallel bottleneck: true matching builds

A close PS2 source tree records EE GCC `3.2.2-b1` and R5900 release flags. It
is a strong compiler-family fingerprint, but not yet an exact target toolchain.
A function becomes MATCHING only after a rebuilt complete function compares
byte-for-byte after relocations are accounted for.

## Remaining likely hard areas

1. Exact old C/C++ layouts and aliases in the Snes9x 1.41-derived core.
2. Hiryu-specific PPU/timing outside the reconstructed tile draw family.
3. Audio glue and EE/IOP scheduling around SjPCM/AmigaMod.
4. Hot loops where inlining/optimization erase convenient source boundaries.
5. Toolchain/link order needed for actual matching.

## Boundary lessons

- `partial_clear @ 0x0018eeb4` is a real call target without a conventional prologue.
- historical source must not override target behavior: `WaitForNextVRstart` is a
  concrete example where compiler optimization turns intended source behavior
  into a target-visible infinite loop.
- shared source can identify lineage after binary evidence, but does not itself
  establish MATCHING.
