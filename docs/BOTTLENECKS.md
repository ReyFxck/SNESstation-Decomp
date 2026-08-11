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
- The next library baseline is exact: **zlib 1.1.3**.
- The zlib Inflate state machine, block decoder, Huffman builders, fast path, and circular flush are reconstructed.
- The Deflate compressor engines and the complete `trees.c` Huffman writer are behaviorally reconstructed.
- zutil/Adler32 are reconstructed, and the full `gzio.c` function map is now known.

## Current binary frontier

The embedded zlib 1.1.3 corridor is now mapped from `compress2` at
`0x00190700` through `adler32` returning at `0x00198c54`. The Deflate/Inflate
core, block/codes/fast decode, dynamic/fixed Huffman trees, Deflate tree
emission, CRC32 and Adler32 are behaviorally reconstructed. The 24-function
`gzio.c` interval at `0x00193298..0x00194627` is mapped but still needs
source reconstruction.

The next module starts at **`0x00198c58` and is PS2 GS/video setup**. Initial
analysis already isolates duplicated constructor-like entry points, a large GS
initialization wrapper at `0x00198d78`, and privileged-register display setup
around `0x00199070`. Historical shared `Gep/Source/ps2/gs.c` code provides a
useful structural reference, but target signatures differ and names remain
conservative. See `docs/PS2_GS_MAP.md`.

Current work is now:

1. map the `0x00198c58..0x0019xxxx` GS/GIF/DMA helper cluster;
2. separate low-level C graphics helpers from the surrounding C++ wrapper;
3. recover object-field meanings from target call sites;
4. continue the matching-toolchain track in parallel without promoting near
   matches to MATCHING.

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
