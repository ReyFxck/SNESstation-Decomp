# Progress 4 summary

> Live scoreboard: [`PROGRESS.generated.md`](PROGRESS.generated.md). Update `analysis/progress_targets.csv` and run `python3 tools/update_progress.py` to refresh the README percentages and the generated SVG progress panel.

Progress 4 closes the main zlib 1.1.3 Deflate/Inflate core, maps the previously
unnamed gzip I/O corridor, adds an evidence-based Petari-style progress graphic,
and moves active binary analysis into the PS2 GS/video subsystem.

## Current scoreboard

- **165 reconstructed targets**
- **219 mapped targets**
- **14.51% reconstructed** on the conservative 1,137-JAL proxy
- **19.26% mapped** on the same proxy
- **0.00% matching** (kept deliberately strict)
- renderer draw-family subgrid: **30/30 reconstructed**

The project-wide visual summary is generated as [`assets/progress.svg`](../assets/progress.svg).
It uses 200 cells as a quantized view of the same proxy; it is a visualization,
not a replacement for the underlying per-address status CSV.

## Renderer milestone remains closed

All 30 tracked draw-family entry points from `0x0018428c` through
`0x0018bac0` have behavior-oriented C reconstructions, including 8/16-bit,
clipped, x2/x2x2, large-pixel, Add/Sub/Half/fixed-colour paths and their
normal/flipped writers.

## Legacy ZIP / unzip milestone

The old archive path is now well bounded:

- Implode/Explode recovered;
- Reduce recovered;
- Shrink recovered, including internal `partial_clear` boundary handling;
- unzip 0.15-style API mapped through `0x00190628`.

This work also established an important boundary rule: real direct-call targets
can exist without a conventional stack-frame prologue.

## zlib 1.1.3 core milestone

The target itself identifies the embedded library as **zlib 1.1.3**. Progress 4
recovers the main compression/decompression machinery rather than merely naming
it from upstream source.

Behaviorally reconstructed target code now includes:

- `compress2`, `compress`, `uncompress`;
- Deflate initialization/state management;
- `deflate`, `longest_match`, `fill_window`;
- stored/fast/slow Deflate engines;
- Inflate interface and 14-state stream machine;
- block decoder, slow codes decoder and fast path;
- dynamic/fixed Huffman tree construction and circular output flush;
- complete `trees.c` Huffman-output side through `copy_block`;
- CRC32, Adler32, `zlibVersion`, `zError`, `zcalloc`, and `zcfree`.

The target `deflate_state` allocation is independently accounted for through
**0x16d8 bytes**, including the dynamic trees, heap, literal/distance buffers,
64-bit length fields and bit buffer. See [`ZLIB_MAP.md`](ZLIB_MAP.md).

### gzip I/O gap

The interval `0x00193298..0x00194627` is now mapped to the 24-function zlib
1.1.3 `gzio.c` API (`gz_open`, `gzread`, `gzwrite`, `gzseek`, `gzclose`, etc.).
Those functions are intentionally still marked **IDENTIFIED**, not
RECONSTRUCTED. Evidence includes the `0x80`-byte gzip stream object, `0x4000`
I/O buffers, gzip header handling, and the large local format buffer in
`gzprintf`.

Therefore the zlib corridor is **fully mapped at the module level but not yet
fully reconstructed**.

## PS2 GS/video frontier

`adler32` returns at `0x00198c54`. The next function at `0x00198c58` is already
PS2 graphics code.

The first audited GS pass now tracks:

| Address | Provisional name | State |
|---|---|---|
| `0x00198c58` | `graphics_wrapper_entry_A` | identified |
| `0x00198cc8` | `graphics_wrapper_entry_B` | identified |
| `0x00198d78` | `graphics_display_init` | partial |
| `0x00199070` | `gs_privileged_display_program` | partial |

The two wrapper entries are near duplicates and both call `0x00199590` before
entering `0x00198d78` with the same default 320x240 setup. The larger routine
resets GS CSR at `0x12001000`; `0x00199070` writes a packed 64-bit display value
to privileged GS register address `0x12000080`.

Historical shared PS2 `gs.c` code is useful structural validation, but no class
name has been assigned and no historical function name is treated as proven
unless the target signature agrees. See [`PS2_GS_MAP.md`](PS2_GS_MAP.md).

## Matching/toolchain status

A close historical PS2 build records EE GCC `3.2.2-b1` plus R5900 release
flags. It remains a strong compiler-family fingerprint, not a proven exact
compiler. Larger functions such as `deflateInit2_` show different register
allocation even when structure is very similar.

**Matching remains 0.00%.** A function becomes matching only after an actual
candidate-toolchain rebuild compares byte-for-byte with the target.

## Validation

- all **34** current `src/**/*.c` translation units pass host C99 syntax checks
  with `-Wall -Wextra -Werror`;
- all **13** `src/zlib/*.c` units also compile to objects and combine with
  `ld -r` into one relocatable research object without duplicate definitions;
- `git diff --check` is clean.
