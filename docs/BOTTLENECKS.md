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

The old library/runtime corridor has now been crossed continuously through:

1. zlib 1.1.3 (`0x00190700..0x00198c54`);
2. Hiryu GSLIB (`0x00198c58..0x0019be6c`);
3. CDVD / old PS2LIB runtime and SIF command support;
4. Newlib 1.10 `mathfp`;
5. old `libmc`;
6. GCC 3.2.2-b1 DI/soft-double and DWARF unwind runtime;
7. NEW/XPADMAN `libpad`;
8. libsupc++ EH, RTTI, `__dynamic_cast`, catch state and standard exceptions
   through `0x001ab3bc`.

The next clean contiguous runtime frontier is **`0x001ab3c0`**, where the target
switches to an EE CP0/cache-maintenance path.

Current priorities:

1. finish independent source models for the already-mapped complex
   `__vmi_class_type_info` dynamic/upcast walkers;
2. recover SNES Station frontend/config/input code around the known GUI;
3. continue Snes9x 1.41 structure/layout recovery outside the completed draw
   family;
4. attack SjPCM/AmigaMod audio glue and EE/IOP scheduling;
5. reproduce the historical compiler/runtime and begin real byte comparisons.

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
