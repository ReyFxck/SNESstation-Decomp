# Roadmap

## Phase 1 — analysis foundation

- [x] freeze target hash
- [x] SJCRUNCH unpacking
- [x] analysis ELF wrapper
- [x] string/xref and call scanners
- [x] classify R5900 instructions missed by generic LLVM disassembly
- [x] independent symbol map

## Phase 2 — frontend / boot

- [x] startup and IOP reset path
- [x] module-buffer loader
- [x] main application/core boundary
- [x] memory-card initialization path
- [x] SRAM path helpers
- [ ] reconstruct remaining GUI/frontend functions
- [ ] map input/configuration structures

## Phase 3 — Snes9x core mapping

- [x] `CMemory::Init`
- [x] identify `CMemory::LoadROM`
- [x] SRAM load/save path
- [ ] reconstruct ROM loader in smaller verified units
- [ ] map CPU/PPU/APU global structure layouts
- [ ] map save-state serialization

## Phase 4 — renderer

- [x] tile lookup tables
- [x] `ConvertTile`
- [x] 8-bit normal/clipped/x2 rendering family (major pieces)
- [x] `DrawLargePixel`
- [ ] finish x2x2 pair
- [ ] 16-bit normal/clipped pair
- [ ] Add/Sub/Half color-math families
- [ ] background/object renderer mapping
- [ ] Mode 7 path

## Phase 5 — audio / PS2 glue

- [ ] SjPCM interface
- [ ] AmigaMod interface
- [ ] SPU2 buffering/timing
- [ ] EE/IOP scheduling assumptions

## Phase 6 — matching / rebuild

- [ ] fingerprint historical GCC/binutils/PS2SDK
- [ ] establish matching build environment
- [ ] compile independent recovered objects
- [ ] compare assembly function-by-function
- [ ] link progressively larger replacement executable
