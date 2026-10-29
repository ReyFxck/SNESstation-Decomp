# Progress Files Refactoring Plan

## Overview
All `progress*` files are intermediate Ghidra/decompilation fragments that need to be reorganized into proper canonical modules. This is the core decompilation work - turning raw decompiled output into proper C/C++ code that compiles byte-identical to the original ELF.

---

## PS2 Progress Files Analysis

### 1. `progress11_frontend.c` (already converted)
- **Content**: Frontend frame sync callback (S9xSyncSpeed)
- **Status**: Already canonical as `src/ps2/progress11_frontend.c`
- **Action**: Rename to `snes_frontend_sync.c` or integrate into existing frontend module

### 2. `progress12_io_state.c` 
- **Content**: 
  - `snes_p12_00103d90` - cdfs path sprintf
  - `snes_p12_00172174` - payload write wrapper
  - `snes_p12_00106824` - Memory card probe (mcInit/mcGetInfo/mcSync)
  - `snes_p12_0016fbb4` / `snes_p12_0016fb04` - SDD1 save data load/save
  - `snes_p12_00183c58` / `snes_p12_00183d40` - State block transfer (copy template ↔ backing)
- **Proposed Split**:
  - `snes_path.c` - path formatting functions
  - `snes_mc_probe.c` - memory card probing
  - `snes_sdd1_save.c` - SDD1 save data handling
  - `snes_state_transfer.c` - state block transfer

### 3. `progress13_cpp_eh.c`
- **Content**: C++ exception handling runtime (`__terminate`, `__gxx_exception_cleanup`, `__cxa_throw`)
- **Proposed**: `libgcc_cpp_eh.c` - C++ EH runtime

### 4. `progress13_cpp_personality.c`
- **Content**: C++ personality routine helpers (LSDA parsing, unwind cleanup)
- **Proposed**: `libgcc_cpp_personality.c` - C++ personality/unwind

### 5. `progress13_libgcc_softfloat.c`
- **Content**: IEEE 754 soft-float (single/double pack/unpack, classification)
- **Proposed**: `libgcc_softfloat.c` - soft-float implementation

### 6. `progress15_pad_init.c`
- **Content**: Pad initialization (XPADMAN bind, delay, command 0x10)
- **Proposed**: `snes_pad_init.c` - pad initialization

### 7. `progress15_softdouble_add.c`
- **Content**: `_fpadd_parts_d` - soft-double addition
- **Proposed**: Part of `libgcc_softfloat.c` or `libgcc_softdouble.c`

### 8. `progress21_small_helpers.c`
- **Content**: 
  - `snes_p21_00103cd4` - TOC entry name extraction
  - `snes_p21_00106c08` - State file extension parsing
  - `snes_p21_001a7638` - Signed 64-bit remainder
- **Proposed**: Split into appropriate modules (frontend toc, state file, libgcc divmod)

### 9. `progress22_more_small_helpers.c`
- **Content**:
  - `snes_p22_00103b34` - Frontend message slide/scissor (GS driver/font)
  - `snes_p22_00104234` - File loading into aligned buffer
- **Proposed**: 
  - `snes_frontend_msg.c` - frontend message display
  - `snes_fileio.c` - file loading utilities

### 10. `progress23_audio_helpers.c`
- **Content**: Audio slot management (8 slots, 0xE0 bytes each), hooks for reset/enable/configure
- **Proposed**: `snes_audio_runtime.c` - audio runtime state and hooks

### 11. `progress24_core_helpers.c`
- **Content**:
  - `snes_p24_0010a264` - Four-way packed blend (graphics)
  - `snes_p24_0010b7f8` - Register read state (x8/x9 special handling)
  - `snes_p24_00115b58` - Memory/PPU
  - `snes_p24_0016ded4` - 
- **Proposed**: Split into graphics blending, register state, memory modules

### 12. `progress25_small_core_helpers.c`
- **Content**: GS asset upload (background, logo, panel, font), GS state management
- **Proposed**: `snes_gs_state.c` - GS driver state and asset management

### 13. `progress26_dsp_runtime_batch.c`
- **Content**: 
  - `snes_p26_00129250` - ADC/decimal mode accumulator
  - `snes_p26_0012c02c` - 2048-entry waveform LUT construction
  - ... more DSP functions
- **Proposed**: `snes_dsp_runtime.c` - DSP core runtime

### 14. `progress27_large_state_cpu_batch.c`
- **Content**: CPU interrupt vectors, state management, cycle counters
- **Proposed**: `snes_cpu_state.c` - CPU state and interrupt handling

### 15. `progress28_structural_lift.c` (1MB, 270+ functions)
- **Per Agent Analysis**, split into:
  - **Already canonical in src/snes9x/**: c4drawwireframe.cpp, c4_math.cpp, dsp_dispatch.cpp, renderer/, snapshot.cpp, unzip/
  - **New canonical files needed**:
    - `startup.c` - P17 startup functions (_start, main, top_level_gui)
    - `frontend_core.c` - P16 frontend-core functions
    - `c4_helpers.c` - C4 line drawing helpers
    - `ppu_vram.c` - ppu_vram_cache_invalidate
    - `memory_ppu.c` - Memory/PPU functions
    - `cpu_audio_runtime.c` - CPU/Audio runtime
    - `legacy_zip.c` - ZIP/zlib functions
    - `gcc_runtime.c` - GCC runtime support
    - `unwind.c` - Stack unwinding (_Unwind_*)
    - `cpp_eh_rtti.c` - C++ EH & RTTI

---

## SNES9X Progress Files Analysis

### 1. `progress11_core.c`
- **Content**: Memory mapping (snes_p11_0016f9b0, snes_p11_0016fa18), table selection (336/3f5 tables)
- **Proposed**: `snes9x_memory_map.c` - memory mapping and table selection

### 2. `progress12_fixed_transform.c`
- **Content**: Fixed-point 3x3 matrix transforms (Q15 arithmetic, rows/columns)
- **Proposed**: `snes9x_matrix.c` - 3x3 matrix transforms

### 3. `progress12_slot_runtime.c`
- **Content**: Controller axis slots (0xE0 bytes), raw/scaled axis updates, configuration
- **Proposed**: `snes9x_controller.c` - controller/slot runtime

### 4. `progress13_core_helpers.c`
- **Content**: APU I/O write, renderer gate predicate, palette expansion (256 entries)
- **Proposed**: Split into:
  - `snes9x_apu_io.c` - APU I/O
  - `snes9x_renderer_gate.c` - renderer gating
  - `snes9x_palette.c` - palette expansion

### 5. `progress13_frontend_more.c`
- **Content**: Path construction wrapper, bitplane transpose (PPU), frontend table init
- **Proposed**: Split into:
  - `snes9x_path.c` - path construction
  - `snes9x_bitplane.c` - bitplane transpose
  - `snes9x_frontend_init.c` - frontend table initialization

### 6. `progress13_runtime_more.c`
- **Content**: libgcc `__floatdidf`, days_in_month (duplicated), state machine
- **Proposed**: Split into:
  - `libgcc_softfloat.c` - __floatdidf (add to existing)
  - `snes9x_datetime.c` - date/time helpers
  - `snes9x_state_machine.c` - stage state machine

### 7. `progress15_renderer_leaf.c`
- **Content**: Tiny renderer 4bpp setup leaf
- **Proposed**: `snes9x_renderer_leaf.c` or integrate into renderer

---

## Implementation Strategy

### Phase 1: Create New Canonical Modules (PS2)
1. Create header files for shared types
2. Split progress12_io_state.c into 4 modules
3. Combine progress13_cpp_eh + progress13_cpp_personality → libgcc_cpp_eh.c
4. Merge progress13_libgcc_softfloat + progress15_softdouble_add → libgcc_softfloat.c
4. Create snes_pad_init.c from progress15_pad_init.c
5. Split progress21, progress22 into appropriate modules
6. Create snes_audio_runtime.c from progress23
7. Split progress24, progress25 into appropriate modules
8. Create snes_dsp_runtime.c from progress26
9. Create snes_cpu_state.c from progress27
10. Split progress28 into 10+ modules (major effort)

### Phase 2: Create New Canonical Modules (SNES9X)
1. Create snes9x_memory_map.c from progress11_core.c
2. Create snes9x_matrix.c from progress12_fixed_transform.c
3. Create snes9x_controller.c from progress12_slot_runtime.c
4. Split progress13_core_helpers into 3 modules
5. Split progress13_frontend_more into 3 modules
6. Split progress13_runtime_more into 3 modules
7. Create snes9x_renderer_leaf.c from progress15_renderer_leaf.c

### Phase 3: Update Build System
- Update translation_units.tsv with new file names
- Update build_source_tree.py with SOURCE_FIXED_FLAGS for all new files
- Update report_contract.json hashes

### Phase 4: Cleanup
- Delete all progress*.c files after successful migration
- Run tests to verify byte-identical compilation

---

## Priority Order (Dependencies First)

1. **libgcc_softfloat.c** - used by many other modules
2. **libgcc_cpp_eh.c / libgcc_cpp_personality.c** - C++ runtime
3. **snes_pad_init.c** - standalone
4. **snes_path.c, snes_mc_probe.c, snes_sdd1_save.c, snes_state_transfer.c** - from progress12
5. **snes_frontend_msg.c, snes_fileio.c** - from progress22
6. **snes_audio_runtime.c** - from progress23
7. **snes_gs_state.c** - from progress25
8. **snes_dsp_runtime.c** - from progress26
9. **snes_cpu_state.c** - from progress27
10. **progress28 split** - major effort, do last
11. **SNES9X modules** - can be done in parallel

---

## Notes

- Each new module needs proper headers with forward declarations
- Must maintain byte-identical compilation
- Ghidra types (`undefined*`, `DAT_*`, `RAM_*`) must be replaced with proper types
- Address-labelled names (`snes_pXX_001XXXXX`) should be replaced with meaningful names where original symbols are known
- Functions that already have canonical implementations in src/snes9x/ should be reconciled, not duplicated