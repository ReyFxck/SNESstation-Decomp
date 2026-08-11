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
