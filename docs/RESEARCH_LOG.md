# Research log

## 2026-08-10 — target frozen

Primary target selected: SNES Station v0.23 WIP, 24 January 2004.

- Packed SHA-256: `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487`
- Unpacked SHA-256: `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`
- Raw load base: `0x00100000`
- Entry: `0x00100008`
- Unpacked size: `3,304,936` bytes

A Python SJCRUNCH unpacker was created around the host LZO runtime so the old LZO development headers are not required.

## 2026-08-10 — application boundary

Recovered startup/module loading and mapped the main application flow. The program visibly crosses from frontend into `CMemory::Init` and `CMemory::LoadROM`, then SRAM/APU/emulation, cleanup, and back to the GUI.

Embedded module payloads were identified during analysis but are deliberately not distributed in this repository.

## 2026-08-10 — Snes9x baseline

The binary contains the strings `Snes9x` and `1.41`. Snes9x 1.41 is therefore the primary upstream baseline. Close-era Snes9x 1.43 source has been useful as *post-identification validation*, not as a naming oracle.

## 2026-08-10 — renderer breakthrough

Recovered the 2/4/8-bpp tile lookup-table generation and `ConvertTile` at `0x00183e04`.

The decoded tile representation is 64 bytes per tile. An earlier hypothesis that the cache stored 16-bit decoded pixels was rejected after inspecting the converter and draw stride.

Recovered `DrawTile`, `DrawClippedTile`, `DrawTilex2`, their normal/flipped pixel writers, and `DrawLargePixel`. Mapped the start of the 16-bit renderer family.

## 2026-08-10 — R5900 disassembly gap closed

Within the selected EE code range, LLVM originally emitted 859 `<unknown>` instructions. They were classified as known R5900 forms, dominated by the EE three-operand `MULT`, plus LQ/SQ, EI/DI, MFLO1/MTLO1, MULTU1/DIVU1, and SQRT.S.

`tools/annotate_r5900_unknown.py` now annotates all 859/859 known cases in that range.

## 2026-08-10 — renderer draw-family closed

Recovered the full 30-entry draw-family block through `0x0018bac0`, including 16-bit x2/x2x2, large-pixel paths, and Add/Sub/Half/fixed-colour math. The tracked renderer subgrid is now 30/30 reconstructed.

## 2026-08-10 — legacy ZIP methods recovered

The post-renderer block was initially suspected to be generic DEFLATE, then corrected after the target's nibble-coded tree format and four-way dispatch were analyzed. It is the classic PKZIP method-6 Implode/Explode family, followed by Reduce and Shrink.

Recovered the block from `get_tree` at `0x0018c124` through `unShrink` / internal `partial_clear` at `0x0018ea64` / `0x0018eeb4`. The internal helper has no stack-frame prologue, proving that prologue-only boundary scans miss real functions.

## 2026-08-10 — compiler fingerprint lead

A public PS2 SNESticle tree contains release GAS listings for the same shared legacy-ZIP source and records EE GCC `3.2.2-b1` plus its R5900 release flags. The opening machine words/frame of `get_tree` match SNES Station. This is the first strong route toward real matching builds, but matching remains at 0% until an exact compiler reproduction and full byte comparison are available.

## Current frontier

The embedded zlib 1.1.3 corridor is now behaviorally reconstructed through its
final `adler32` return at `0x00198c54`. The active binary frontier has moved to
the PS2 GS/video block beginning at `0x00198c58`. EE GCC 3.2.2-b1 remains a
strong comparison candidate, not a proven exact compiler.

## Progress 3 — renderer completion, legacy ZIP, unzip API, zlib fingerprint

- Closed all 30 tracked renderer draw-family entry points through `0x0018bac0`.
- Recovered 16-bit color math and x2/x2x2 writers from target masks and calls.
- Identified/recovered Implode/Explode, Reduce and Shrink legacy ZIP methods.
- Mapped unzip 0.15-style API from `0x0018f010` through `0x00190628`.
- Found exact embedded zlib version string `1.1.3`; recovered `compress2`,
  `compress`, `uncompress` and `deflateInit_` wrappers.
- Current frontier moved to zlib `deflateInit2_` at `0x001908ec`.
- SNESticle PS2 GCC 3.2.2-b1 listings remain the strongest matching-toolchain
  lead, but no target is marked MATCHING yet.

## 2026-08-10 — zlib Inflate core closed

Recovered the complete zlib 1.1.3 Inflate interface and its internal DEFLATE
block decoder through `inflate_flush` at `0x001967e8`. This includes the
`inflate_blocks` state machine, slow and fast literal/length-distance decoders,
`huft_build`, dynamic/fixed Huffman tree construction, and circular-window
flush logic.

A boundary correction was made during this pass: `0x001967b0` is the leaf
`inflate_trees_fixed`; `0x001967e8` is `inflate_flush`. The leaf has no normal
stack-frame prologue, reinforcing the earlier Shrink lesson that prologue-only
scans are insufficient.

The next contiguous module begins at `0x00196980`: zlib `trees.c`, the Deflate
Huffman-output side. Initial boundaries for `_tr_init`, `init_block`,
`pqdownheap`, `gen_bitlen`, and `gen_codes` are now mapped.

## 2026-08-10 — toolchain fingerprint correction

The historical SNESticle EE GCC 3.2.2-b1 listing remains an excellent
structural reference, but it is **not** accepted as an exact matching compiler
yet. Small wrappers such as `deflateInit_` have extremely close instruction
shape, while `deflateInit2_` shows different saved-register/allocation choices
inside the function. This can come from compiler revision, flags, source
revision, or surrounding translation-unit differences. Matching therefore
remains 0.00%.

## 2026-08-10 — zlib Deflate / trees.c closed and gzio mapped

Recovered the large Deflate compressor paths that had previously remained only
identified: `deflate`, `longest_match`, `fill_window`, and the stored/fast/slow
engines. The complete `trees.c` Huffman-output side is now behaviorally
reconstructed from `_tr_init` through `copy_block`.

The target `deflate_state` layout was independently accounted for through its
full `0x16d8` allocation. This resolves the tree, heap, literal/distance buffer,
64-bit length-accounting, and bit-buffer offsets instead of treating them as an
opaque state blob.

A symbol-boundary correction was made before publishing the map:
`pqdownheap=0x00196a94`, `gen_bitlen=0x00196b98`,
`gen_codes=0x00196e80`, and `build_tree=0x00196f24`.
`bi_reverse=0x00198840` is proven directly by the target shift/OR loop.

The zutil tail and `adler32` were also recovered at `0x00198a58..0x00198c54`.
`adler32` exposes the canonical zlib constants `5552` and `65521` in the target.

Finally, the previously unnamed gap `0x00193298..0x00194627` was mapped to the
24-function zlib 1.1.3 `gzio.c` API. Evidence includes the gzip magic path,
`0x4000` I/O buffers, the `0x80`-byte gzip stream allocation, and the
`0x1070`-byte `gzprintf` stack frame around its 4096-byte formatting buffer.
The functions are currently marked IDENTIFIED, not RECONSTRUCTED.

## 2026-08-10 — Progress 4: zlib corridor closed

- Reconstructed the remaining Deflate engines: `deflate`, `longest_match`,
  `fill_window`, `deflate_stored`, `deflate_fast`, and `deflate_slow`.
- Reconstructed the complete `trees.c` Huffman-output/bitstream side from
  `_tr_init` through `copy_block`, including leaf `_tr_tally`.
- Recovered `zlibVersion`, `zError`, `zcalloc`, `zcfree`, and `adler32`.
- Corrected a research-header address typo from `0x001a8a84/0x001a8aa4` to
  the target VAs `0x00198a84/0x00198aa4`.
- Host validation: all `src/zlib/*.c` compile with
  `-std=c99 -Wall -Wextra -Werror` and combine with `ld -r`.
- Confirmed a hard module boundary: `adler32` returns at `0x00198c54`;
  `0x00198c58` begins PS2 GS/video code.
- Added a generated Petari-style SVG progress panel and moved the active
  frontier documentation to `docs/PS2_GS_MAP.md`.

## 2026-08-10 — PS2 GS frontier audited

Audited the first non-zlib block directly from the target instead of assigning
historical `gs.c` names by sequence. `0x00198c58` and `0x00198cc8` are
near-duplicate graphics wrapper entries: both call `0x00199590` and then
`0x00198d78` with the same default 320x240 display arguments.

`0x00198d78` is now classified as a partial graphics/display initializer. It
stores display parameters in an object-like state and performs the PS2 GS CSR
reset sequence at `0x12001000`. `0x00199070` is a separate partial low-level
routine that packs display geometry/offset fields and writes a 64-bit value to
privileged GS display register address `0x12000080`.

Historical shared PS2 `gs.c` contains the same classes of low-level operations
(GS reset, CRT setup, display-register packing), so it is retained as
structural validation only. The wrapper/class identity remains unresolved.

## 2026-08-10 — gzip I/O gap closed and zlib boundary finalized

A consistency audit caught that the earlier “zlib corridor closed” wording was
premature: the `gzio.c` interval at `0x00193298..0x00194627` was mapped but had
not yet been converted to C. All 24 entry points are now behaviorally
reconstructed and validated with the rest of `src/zlib/`.

The target contains PS2-specific deviations from generic zlib 1.1.3 that are
now preserved explicitly. `gzdopen` builds `"<fd:%d>"`, while `gz_open` ignores
its fd parameter and opens that text as a path; the integer file field starts
at zero; `gzflush` lacks the generic-source `fflush`; `gzerror` falls back to
the zlib error table for `Z_ERRNO`; and the emitted gzip header uses OS byte 3.

With that correction, the embedded zlib 1.1.3 corridor is behaviorally
reconstructed from `compress2` at `0x00190700` through the final `adler32`
return at `0x00198c54`. The next active module remains PS2 GS/video code at
`0x00198c58`.


## 2026-08-10 — first GS display/buffer cluster reconstructed

Recovered the first self-contained post-zlib PS2 graphics cluster at
`0x00199070..0x001993c0` without assigning an unproven class name.
`0x00199070` reproduces the target DISPLAY1 geometry arithmetic and writes the
privileged register at `0x12000080`; `0x001992f8` writes DISPFB1 at
`0x12000070` from a clamped framebuffer index, width/64 and PSM.

The object fields at `+0x50/+0x54/+0x58/+0x5c/+0x60/+0x64` now form a coherent
display/draw buffer rotation model: current display index, current draw index,
buffer count, pending draw transitions, pending display transitions, and
per-buffer stride. Small helpers from `0x00199178` through `0x00199290` were
reconstructed around that model, and `0x00199360` preserves the target
flush/select/draw-frame/flush call sequence.

The historical shared `gs.c` DISPLAY1/DISPFB formulas validate the operation
class after target-side identification, but the SNES Station wrapper/object
layout remains authoritative.


## 2026-08-10 — GS GIF FIFO and FRAME_1 writer recovered

The draw-buffer helper chain was followed through `0x00199838..0x001998f8` and
`0x00199dc0`. The target uses a double-buffered GIF/DMA list: `0x001998f8` waits
for GIF DMA completion, invokes `FlushCache(0)`, submits the active chain,
switches to the other half of the allocation, writes a DMA END tag, and resets
the packet pointers. `0x00199898` computes remaining bytes and `0x001998b8`
forces a flush below a `0x90`-byte reserve.

`0x00199dc0` is now reconstructed as the GS FRAME_1 A+D packet writer. Its
packed value contains framebuffer base (`address >> 13`), width (`width/64`),
PSM, and FBMSK, followed by register selector `0x4c`. This removes the last
opaque helper from the first recovered draw-buffer path.

## 2026-08-10 — Progress 5: Hiryu GSLIB corridor recovered

Target-first analysis resolved the entire post-zlib graphics block. The binary
itself contains the original `gsPipe` allocation/alignment diagnostics, after
which historical GSLIB by Hiryu validates the class/method names. The target
object layout is exact: `gsPipe` is 0x34 bytes and the embedded-pipe `gsDriver`
is 0x74 bytes.

This corrects an earlier project inference. The `0x74` allocation at the start
of `main`, constructed through `0x00198cc8`, is `gsDriver`; `0x001990f8` is
`gsDriver::clearScreen`. Main preserves the allocator result in `$17`, ignores
the constructor return value, and stores that original pointer globally.

The target uses an older `gsDriver::setDisplayMode` signature with explicit
x/y position and TV mode. It also preserves old behavior absent or commented in
later mirrors: width truncation with `& 0xFFC0`, possible `num_bufs-2` free-count
underflow, and no initialization of the complete-buffer counter in that method.

`gsPipe` is reconstructed from both target packet behavior and historical
lineage. Distinctive target-visible quirks include the assignment operator not
copying ZTest/Filter state, `TextureFlush` writing `0xBAD`, and `TextureSet`
using literal XOR for historical `2^texwidth` / `2^texheight` expressions.
The method order and clean boundary continue through all line/triangle/
rectangle/point/triangle-strip primitive emitters to `0x0019b7ec`.

The following block is `gsFont`: `uploadFont @ 0x0019b7f0`, `Print @
0x0019b948`, `GetCurrLineLength @ 0x0019bad0`, and `PrintLine @ 0x0019bb68`.
Its target layout puts Bold/Underline at +0x30/+0x31 and the 256-byte glyph
width table at +0x32.

Finally, `0x0019bd38..0x0019be6c` matches GSLIB `hw.c`: vertical-retrace
helpers, `DmaReset`, `SendDma02`, and `Dma02Wait`. A compiler-visible bug is
preserved: because historical `VRcount` was not volatile, optimized
`WaitForNextVRstart` sets it to zero and becomes an infinite register-only loop
for every positive argument instead of observing interrupt updates.

The complete recovered GSLIB slice ends at `0x0019be6c`; `CDVD_Init` begins at
`0x0019be70`. Matching remains 0.00% because historical source equivalence is
not a byte-identical target rebuild.
