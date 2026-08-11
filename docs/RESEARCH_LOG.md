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

Continue into the surrounding `unzip.c` API at `0x0018f010+`, while preparing an EE GCC 3.2.2-b1 reproduction experiment around the small `get_tree` function.

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
