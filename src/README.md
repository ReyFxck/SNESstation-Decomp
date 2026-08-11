Reconstructed source lives here.

Suggested order:
1. startup/runtime helpers
2. libc-like helpers and formatting
3. pad/input
4. file I/O and memory card
5. GUI drawing
6. ROM/header loading
7. save state/SRAM
8. SNES CPU/APU/PPU
9. enhancement chips (SuperFX etc.)

## Legacy ZIP recovery

`src/unzip/` contains behavior-oriented reconstructions of the old PKZIP
Implode/Explode, Reduce, and Shrink blocks at `0x0018c124..0x0018f010`.
The adapter in `include/legacy_zip_recovered.h` intentionally abstracts the
still-unmapped concrete unzip structure offsets; the algorithms/function
boundaries themselves are high-confidence recoveries.

### Legacy ZIP / zlib recovery

- `unzip/explode_recovered.c` — Implode/Explode method 6 family
- `unzip/unreduce_recovered.c` — Reduce
- `unzip/unshrink_recovered.c` — Shrink + partial clear
- `unzip/unzip_api_recovered.c` — unzip 0.15-style API helpers
- `zlib/zlib_buffer_api_recovered.c` — zlib 1.1.3 compress/uncompress wrappers
- `zlib/deflate_state_recovered.c` / `deflate_engine_recovered.c` — Deflate state and compressor engines
- `zlib/inflate_state_recovered.c` — public Inflate state machine
- `zlib/infblock_*`, `infcodes_recovered.c`, `inffast_recovered.c`, `inftrees_recovered.c`, `infutil_recovered.c` — DEFLATE decoder core
- `zlib/trees_recovered.c` — complete Deflate Huffman-output writer
- `zlib/crc32_recovered.c` / `zutil_adler_recovered.c` — checksums and zutil tail
- `zlib/gzio_recovered.c` — complete PS2-adapted gzip I/O layer, including target quirks

The contiguous embedded zlib 1.1.3 corridor is now behaviorally reconstructed; byte matching remains a separate toolchain task.


## PS2 Hiryu GSLIB recovery

The complete post-zlib graphics slice is now represented by:

- `ps2/gsdriver_recovered.c` — older GSLIB `gsDriver`, display/buffer management and VSync callback wrappers;
- `ps2/gspipe_recovered.c` — `gsPipe` GIF/DMA buffering, GS state, textures and primitives;
- `ps2/gsfont_recovered.c` — BFNT upload and text rendering;
- `ps2/gslib_hw_recovered.c` — vertical-retrace and DMA MMIO helpers;
- `ps2/gs_fifo_recovered.c` — compact canonical helper views used during analysis.

The target proves `gsPipe`/`gsDriver` layouts and preserves several historical
quirks. See `docs/PS2_GS_MAP.md`.


## PS2 CDVD / libkernel runtime recovery

Progress 6 adds:

- `ps2/cdvd_rpc_recovered.c` — Hiryu/Sjeep libcdvd EE client;
- `ps2/libkernel_strings_recovered.c` — compact linked string/memory helpers;
- `ps2/sifrpc_recovered.c` — old SIF RPC packet/client core;
- `ps2/libkernel_leaf_recovered.c` — EE syscall/cache leaves;
- `ps2/fileio_recovered.c` — old FileIO RPC wrappers and callbacks;
- `ps2/loadfile_iop_recovered.c` — loadfile, IOP heap and reset paths;
- `ps2/libkernel_client_init_recovered.c` — later-linked FileIO/loadfile/heap bind helpers;
- `ps2/stdio_wrappers_recovered.c` — old PS2LIB stdio/vprintf wrappers and target output callbacks.
- `ps2/ps2lib_formatter_recovered.c` — callback-driven `fmtint/fmtstr/fmtchar/dopr/vsnprintf` core.
- `ps2/libkernel_heap_recovered.c` — old target heap allocator.
- `ps2/libkernel_heap_runtime_recovered.c` — `DIntr/EIntr`, `ps2_sbrk` and `EndOfHeap`.
- `ps2/libc_text_recovered.c` — ASCII ctype plus string/token/integer parsing helpers.
- `ps2/sifcmd_recovered.c` — old SIF command send/init/dispatch corridor.

Target-sized RPC structures are defined in `include/ps2_libkernel_recovered.h`.
See `docs/CDVD_LIBKERNEL_MAP.md`.


## Progress 7 runtime recovery

- `ps2/newlib_mathfp_recovered.c` — Newlib 1.10 single-precision trig/classification corridor with target EE leaves preserved;
- `ps2/libmc_recovered.c` — old memory-card RPC client and async helpers;
- `ps2/libgcc_runtime_recovered.c` — independently written DI/soft-double runtime models and proven archive boundaries;
- `ps2/gcc_unwind_pe_recovered.c` — small DWARF encoding/context helpers recovered from the target;
- `ps2/libpad_recovered.c` — NEW/XPADMAN pad client corridor through `padGetConnection`;
- `ps2/cpp_runtime_recovered.c` — small delete/terminate/new[] runtime leaves.

See `docs/RUNTIME_PROGRESS7.md`. The next clean runtime frontier is the
`std::type_info` / RTTI hierarchy at `0x001a9fa8`.


## Progress 8 RTTI / exception runtime recovery

- `ps2/libsupcxx_rtti_recovered.c` — independent behavioral models for the
  `std::type_info` hierarchy, standard exception destructor families, old
  `__cxa_allocate_exception` emergency pool, catch-state bookkeeping, EH globals,
  `std::uncaught_exception`, `std::exception::what`, and `std::set_new_handler`.
- `analysis/functions/libsupcxx_rtti_001a9fa8.asm` — focused target assembly
  evidence for `0x001a9fa8..0x001ab3bf`.

The complex VMI dynamic/upcast walkers remain mapped but intentionally not
claimed reconstructed. See `docs/RUNTIME_PROGRESS8.md`. The next clean runtime
frontier is `0x001ab3c0`.


## Progress 9 Snes9x mapped-memory / APU tail recovery

- `ps2/ee_cache_recovered.c` — target `SyncDCache` / `iSyncDCache` CP0/cache walk.
- `snes9x/getset_recovered.c` — `S9xGetMemPointer`, byte/word get/set,
  `S9xSetPCBase`, and `GetBasePointer` with the target 4 KiB map behavior.
- `snes9x/cpu_shutdown_renderer_select_recovered.c` — CPU idle fast-forward /
  APU catch-up helper plus PPU color-math renderer-function selector.
- `snes9x/apumem_recovered.c` — SPC700/APU byte and direct-page access leaves,
  ports/DSP/timers and IPL-ROM shadow behavior.
- `analysis/functions/core_getset_apumem_001ab3c0.asm` — focused target evidence
  through `0x001acd00`.

This closes the former `0x001ab3c0` frontier and connects directly to the
already reconstructed pixel-writer tail at `0x001acd04`. See
`docs/CORE_PROGRESS9.md`.

## Progress 12 fixed-transform / slot / state-I/O recovery

- `snes9x/progress12_fixed_transform_recovered.c` — six target Q15 3x3 row and
  transposed transform leaves with per-product arithmetic shifts preserved;
- `snes9x/progress12_slot_runtime_recovered.c` — seven compact 0xe0-stride
  slot/controller state, scaling, period and dispatch helpers;
- `ps2/progress12_io_state_recovered.c` — cdfs path formatting, memory-card
  probe, record-store load/save, framed output and two state-transfer wrappers;
- `analysis/functions/progress12_targets.asm` — focused target evidence for all
  twenty Progress 12 promotions.

See `docs/PROGRESS12.md`.  These are behavioral reconstructions; matching stays
at 0.00% until a historical EE toolchain rebuild proves machine-code identity.
