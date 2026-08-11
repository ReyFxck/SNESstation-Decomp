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
- [x] recover CDVD + old SIF RPC/FileIO/loadfile runtime corridor
- [ ] reconstruct remaining GUI/frontend functions
- [x] recover linked NEW/XPADMAN libpad client
- [ ] map application input/configuration structures

## Phase 3 — Snes9x core mapping

- [x] `CMemory::Init`
- [x] identify `CMemory::LoadROM`
- [x] SRAM load/save path
- [ ] reconstruct ROM loader in smaller verified units
- [x] reconstruct mapped-memory get/set and PC-base access corridor
- [x] recover CPU shutdown / APU catch-up helper
- [x] reconstruct SPC700/APU byte and direct-page access leaves
- [ ] map remaining CPU/PPU/APU global structure layouts
- [ ] map save-state serialization

## Phase 4 — renderer

- [x] tile lookup tables
- [x] `ConvertTile`
- [x] 8-bit normal/clipped/x2 rendering family
- [x] `DrawLargePixel`
- [x] x2x2 pair
- [x] 16-bit normal/clipped pair
- [x] Add/Sub/Half/fixed-color color-math families
- [x] recover target tile-renderer selector and function-pointer wiring
- [ ] background/object renderer mapping
- [ ] Mode 7 path

## Phase 5 — audio / PS2 glue

- [x] recover Hiryu GSLIB `gsDriver` / `gsPipe` / `gsFont` / DMA corridor
- [x] map and reconstruct CDVD + core PS2LIB client runtime used by startup
- [x] reconstruct old PS2LIB `vsnprintf.o` formatter core
- [ ] SjPCM interface
- [ ] AmigaMod interface
- [ ] SPU2 buffering/timing
- [x] recover EE D-cache range synchronization leaves
- [ ] EE/IOP scheduling assumptions

## Phase 6 — matching / rebuild

- [x] fingerprint historical GCC/binutils/PS2SDK runtime family
- [x] pin and verify the first public GCC 3.2.2/binutils 2.14/Newlib 1.10.0 recipe
- [ ] establish matching build environment
- [ ] compile independent recovered objects
- [ ] compare assembly function-by-function
- [ ] link progressively larger replacement executable

- [x] reconstruct old SIF command send/init/interrupt-dispatch corridor
- [x] classify/reconstruct floating-point runtime beginning at `0x0019fddc`
- [x] map GCC 3.2.2-b1 libgcc/unwind object corridor
- [x] map/reconstruct libsupc++ RTTI and exception core through `0x001ab3bc`
- [ ] reconstruct complex `__vmi_class_type_info` dynamic/upcast walkers
