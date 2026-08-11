# Progress 5 summary

> Live scoreboard: [`PROGRESS.generated.md`](PROGRESS.generated.md). Update `analysis/progress_targets.csv` and run `python3 tools/update_progress.py` to refresh the README percentages and generated SVG panel.

Progress 5 closes the PS2 graphics-library corridor immediately after zlib,
recovers the complete PS2-adapted gzip I/O layer that was mapped in the prior
pass, and corrects the startup object identity in `main`.

## Current scoreboard

- **265 reconstructed targets**
- **291 mapped targets**
- **23.31% reconstructed** on the conservative 1,137-JAL proxy
- **25.59% mapped** on the same proxy
- **0.00% matching** — deliberately strict
- renderer draw-family subgrid: **30/30 reconstructed**

The project-wide visual summary is generated as [`assets/progress.svg`](../assets/progress.svg).

## zlib / gzip milestone

The embedded zlib 1.1.3 corridor is behaviorally reconstructed from
`compress2 @ 0x00190700` through the final return of `adler32 @ 0x00198c54`.
This includes the PS2-adapted 24-function `gzio.c` layer and its target-specific
quirks rather than only the generic upstream behavior.

## Hiryu GSLIB milestone

The next contiguous corridor, `0x00198c58..0x0019be6c`, is now identified and
behaviorally reconstructed as **Hiryu's GSLIB**:

- `gsDriver` including the older 10-argument display-mode interface;
- `gsPipe` constructors, copy/assignment, double-buffered GIF/DMA pipe,
  GS state setters, texture transfer/setup and all primitive emitters;
- `gsFont` upload, line measurement, aligned multi-line printing and glyph draw;
- GSLIB `hw.c` vertical-retrace and DMA helpers.

Target evidence proves `sizeof(gsPipe) == 0x34` and `sizeof(gsDriver) == 0x74`.
The latter corrects an earlier research assumption: the `0x74` object allocated
by `main` and constructed at `0x00198cc8` is `gsDriver`, not a generic frontend
object. `0x001990f8` is `gsDriver::clearScreen`.

Several historical quirks are preserved because they are visible in the target:

- old `width & 0xFFC0` display-width truncation;
- `num_bufs - 2` free-buffer underflow possibility;
- `gsPipe::operator=` omits ZTest/Filter state;
- `TextureFlush` sends `0xBAD`;
- `TextureSet` uses C XOR from historical `2^texsize` text;
- optimized `WaitForNextVRstart` can spin forever because `VRcount` was not volatile.

See [`PS2_GS_MAP.md`](PS2_GS_MAP.md).

## Main-flow correction

At `main @ 0x00104f18` the program:

1. allocates literal `0x74` bytes;
2. preserves that allocation in `$17`;
3. calls `gsDriver` constructor entry `0x00198cc8` with it;
4. ignores the constructor return value;
5. stores the `$17` pointer in the global;
6. calls `gsDriver::clearScreen @ 0x001990f8`.

`src/app/main_bootstrap.c` now mirrors that ABI/data flow.

## Matching/toolchain status

The historical GSLIB and close PS2 GCC `3.2.2-b1` listings are powerful source
and compiler-family fingerprints, but larger target functions still show
register-allocation differences. No function is promoted to MATCHING without a
candidate rebuild that compares byte-for-byte.

## Validation

- all current `src/**/*.c` files pass host `-Wall -Wextra -Werror` syntax checks;
- recovered zlib units remain independently compile/link checkable;
- generated progress data and SVG come solely from `analysis/progress_targets.csv`;
- focused GSLIB assembly extracts are included instead of a full program dump.

## Next frontier

The recovered GSLIB slice ends cleanly at `0x0019be6c`. The next known function
is `CDVD_Init @ 0x0019be70`, followed by the CDVD/RPC/runtime region. In parallel,
remaining SNES Station-specific frontend, audio, core/PPU and matching-toolchain
work continue to be higher-value targets than re-identifying already recovered
third-party library code.
