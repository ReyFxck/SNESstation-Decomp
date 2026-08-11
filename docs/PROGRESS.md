# Progress 2 summary

> Live scoreboard: [`PROGRESS.generated.md`](PROGRESS.generated.md). Update `analysis/progress_targets.csv` and run `python3 tools/update_progress.py` to refresh the README percentages and block grid.

This pass pushed the decompilation through the classic tile renderer boundary.

Recovered / validated in this pass:

- Classified all 859 LLVM `<unknown>` instructions in the selected EE-code
  range; 789 are R5900 3-operand MULT.
- Added `tools/annotate_r5900_unknown.py` and generated
  `asm/full_r5900_annotated.asm`.
- Corrected the earlier false 16-bit decoded-tile-cache hypothesis.
- Recovered runtime tile lookup-table generation at `0x00142a78`.
- Recovered `ConvertTile` at `0x00183e04`.
- Recovered `WRITE_4PIXELS` / flipped at `0x001acd04` / `0x001ace28`.
- Recovered `DrawTile` at `0x0018428c`.
- Recovered `DrawClippedTile` at `0x001845a8` and verified HeadMask/TailMask.
- Recovered x2 pixel writers at `0x001acf4c` / `0x001ad090`.
- Recovered `DrawTilex2` at `0x00184a40`.
- Mapped x2x2 pair at `0x001851f4` / `0x00185510`.
- Recovered `DrawLargePixel` at `0x001859a8`.
- Identified the first 16-bit pair at `0x00185d8c` / `0x001860a8`.
- Built a call matrix for the renderer family through `0x0018adb8`.
- All recovered C files pass host C99 syntax checking with `-Wall -Wextra`.

Current likely bottleneck:

The renderer is no longer opaque. The next dense area is the 16-bit color-math
writer family (add/sub/half variants). Exact function naming and arithmetic
must be proven from the R5900 helpers rather than assigned from resemblance.
Matching compilation will later add a separate toolchain/compiler-fingerprint
challenge.
