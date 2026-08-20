# Bottleneck watch — SNES Station v0.23 WIP

The V52 formal checkpoint is **978/1041 exact function matches (93.95%)**,
leaving 63 entries. V52 closes 17 former near-matches by recovering the PS2
32-bit libc ABI, zlib stream profile, direct `fio*` SRAM path and a small set of
target-visible Snes9x source deltas. The remaining set is concentrated in 34
frontend-core and 12 CPU/audio-runtime entries; every entry still has a source
model, but only exact machine-code evidence counts as matching.

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

The old library/runtime corridor has now been crossed continuously through:

1. zlib 1.1.3 (`0x00190700..0x00198c54`);
2. Hiryu GSLIB (`0x00198c58..0x0019be6c`);
3. CDVD / old PS2LIB runtime and SIF command support;
4. Newlib 1.10 `mathfp`;
5. old `libmc`;
6. GCC 3.2.2-b1 DI/soft-double and DWARF unwind runtime;
7. NEW/XPADMAN `libpad`;
8. libsupc++ EH, RTTI, `__dynamic_cast`, catch state and standard exceptions
   through `0x001ab3bc`;
9. EE D-cache synchronization and Snes9x Map/WriteMap access through the
   CPU-shutdown and APU direct-page leaves at `0x001acd00`;
10. the already-reconstructed renderer/pixel-writer tail through the final
    helper return at `0x001b07d0`.

There is no later high-address executable frontier in this tail: zero padding
follows through `0x001b087f`, then string/data storage begins at `0x001b0880`.
The productive frontier therefore moves back to earlier unmapped application
and Snes9x core regions.

Current priorities:

1. recover SNES Station frontend/config/input code around the known GUI;
2. map CPU/PPU state and background/object/Mode 7 paths around the now-recovered
   mapped-memory core;
3. finish independent source models for the already-mapped complex
   `__vmi_class_type_info` dynamic/upcast walkers;
4. finish the still-large audio runtime and snapshot corridors;
5. reproduce the final historical link order after the remaining function
   source shapes are closed.

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
