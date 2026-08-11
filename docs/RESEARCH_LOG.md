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

## Current frontier

The next reconstruction target is the 16-bit color-math draw family (Add/Sub/Half variants). The expected difficulty is exact arithmetic and macro/inlining recovery, not unknown PS2 ISA behavior.
