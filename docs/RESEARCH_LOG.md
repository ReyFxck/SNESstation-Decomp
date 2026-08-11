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
