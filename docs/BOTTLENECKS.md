# Bottleneck watch — SNES Station v0.23 WIP

## Bottlenecks that did not become walls

- PS2 startup/RPC wrappers remain straightforward.
- GUI -> ROM loader -> core boundary is visible.
- LLVM R5900 decode gaps are classified (859/859 in the selected range).
- All 30 tracked macro-expanded renderer draw entry points through
  `0x0018bac0` are behaviorally reconstructed.
- 16-bit Add/Sub/Half math is proven from target masks/tables.
- Implode/Explode, Reduce and Shrink are recognized historical ZIP code.
- The surrounding unzip 0.15-style API is mapped through `0x00190628`.
- The next library baseline is now exact: **zlib 1.1.3**.

## Current binary frontier

The active frontier is `0x001908ec+`, the embedded zlib 1.1.3 Deflate/Inflate
core. Algorithm identification is no longer the hard part; the work is now:

1. split the large zlib functions at exact target boundaries;
2. compare them against zlib 1.1.3, not newer releases;
3. distinguish compiler/layout differences caused by PS2 `-mlong64`;
4. recover any local patches before promoting source to RECONSTRUCTED.

## Parallel bottleneck: true matching builds

A close PS2 SNESticle build records EE GCC `3.2.2-b1` and release flags, and
its shared legacy-ZIP `.lst` output has target-like R5900 prologues.

The matching path is now concrete:

1. reproduce the old compiler/binutils environment;
2. compile a minimal independently recovered function with candidate flags;
3. normalize relocations/addresses;
4. compare the complete machine-code function;
5. only then promote it to `MATCHING` (🟦).

## Remaining likely hard areas

1. Exact old C/C++ structure layouts and aliases used by Snes9x 1.41.
2. Hiryu-specific renderer/PPU timing outside the recovered draw family.
3. Audio glue and EE/IOP scheduling around SjPCM/AmigaMod.
4. CPU/PPU hot loops where inlining/optimization erase source boundaries.
5. Distinguishing shared library code from SNES Station-specific changes.

## Boundary caution learned from Shrink

`partial_clear` at `0x0018eeb4` is a real `jal` target but has no new
stack-frame prologue. Prologue scans remain hints, not ground truth.

## Naming discipline

`0x00151330` remains `per_rom_cleanup`, not `CMemory::Deinit`: it does not free
the large allocations made by `CMemory::Init`.
