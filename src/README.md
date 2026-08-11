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

### Legacy ZIP / zlib progress 3

- `unzip/explode_recovered.c` — Implode/Explode method 6 family
- `unzip/unreduce_recovered.c` — Reduce
- `unzip/unshrink_recovered.c` — Shrink + partial clear
- `unzip/unzip_api_recovered.c` — small/medium unzip 0.15-style API helpers
- `zlib/zlib_buffer_api_recovered.c` — zlib 1.1.3 compress/uncompress wrappers
