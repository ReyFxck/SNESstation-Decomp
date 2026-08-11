# Renderer map — progress 3

## Tile pipeline

```text
SNES VRAM
  -> runtime 2/4/8-bpp lookup tables
  -> ConvertTile @ 0x00183e04
  -> 64-byte decoded tile cache
  -> DrawTile family @ 0x0018428c..0x0018bac0
  -> depth/palette/color-math pixel writers @ 0x001acd04+
```

The decoded tile stride is 64 bytes. The larger `MAX_*_TILES * 128`
allocations are inherited layout/over-allocation and are not evidence for a
16-bit decoded cache.

## Draw-family entry points

| Address | Function | Status |
|---|---|---|
| `0x0018428c` | `DrawTile` | reconstructed |
| `0x001845a8` | `DrawClippedTile` | reconstructed |
| `0x00184a40` | `DrawTilex2` | reconstructed |
| `0x00184d5c` | `DrawClippedTilex2` | reconstructed |
| `0x001851f4` | `DrawTilex2x2` | reconstructed |
| `0x00185510` | `DrawClippedTilex2x2` | reconstructed |
| `0x001859a8` | `DrawLargePixel` | reconstructed |
| `0x00185d8c` | `DrawTile16` | reconstructed |
| `0x001860a8` | `DrawClippedTile16` | reconstructed |
| `0x00186540` | `DrawTile16x2` | reconstructed |
| `0x0018685c` | `DrawClippedTile16x2` | reconstructed |
| `0x00186cf4` | `DrawTile16x2x2` | reconstructed |
| `0x00187010` | `DrawClippedTile16x2x2` | reconstructed |
| `0x001874a8` | `DrawLargePixel16` | reconstructed |
| `0x0018789c` | `DrawTile16Add` | reconstructed |
| `0x00187bb8` | `DrawClippedTile16Add` | reconstructed |
| `0x00188050` | `DrawTile16Add1_2` | reconstructed |
| `0x0018836c` | `DrawClippedTile16Add1_2` | reconstructed |
| `0x00188804` | `DrawTile16Sub` | reconstructed |
| `0x00188b20` | `DrawClippedTile16Sub` | reconstructed |
| `0x00188fb8` | `DrawTile16Sub1_2` | reconstructed |
| `0x001892d4` | `DrawClippedTile16Sub1_2` | reconstructed |
| `0x0018976c` | `DrawTile16FixedAdd1_2` | reconstructed |
| `0x00189a88` | `DrawClippedTile16FixedAdd1_2` | reconstructed |
| `0x00189f20` | `DrawTile16FixedSub1_2` | reconstructed |
| `0x0018a23c` | `DrawClippedTile16FixedSub1_2` | reconstructed |
| `0x0018a6d4` | `DrawLargePixel16Add` | reconstructed |
| `0x0018adb8` | `DrawLargePixel16Add1_2` | reconstructed |
| `0x0018b43c` | `DrawLargePixel16Sub` | reconstructed |
| `0x0018bac0` | `DrawLargePixel16Sub1_2` | reconstructed |

The tracked draw-family subgrid is therefore **30/30 reconstructed**. This is
a local milestone, not a claim that the overall renderer/PPU is complete.

## Pixel writers

Recovered writer families begin at:

- `0x001acd04` — 8-bit normal/flipped;
- `0x001acf4c` — 8-bit x2;
- `0x001ad1d4` — 8-bit x2x2;
- `0x001ad55c` — 16-bit normal/flipped;
- `0x001ad7ac` — 16-bit x2;
- `0x001ada3c` — 16-bit x2x2;
- `0x001ade2c` — ADD family;
- `0x001ae754` — half-ADD family;
- `0x001aef5c` — SUB family;
- `0x001af6c4` — half-SUB family;
- `0x001afe4c` — fixed half-ADD selector family;
- `0x001b02cc` — fixed half-SUB selector family.

See `analysis/renderer16_call_matrix.txt` and the focused assembly extracts for
exact draw-to-writer pairings.

## Recovered renderer state offsets

BG base: `0x0035d450`
GFX base: `0x0035d480`

Selected GFX fields:

| Offset | Meaning |
|---:|---|
| `+0x08` | Z buffer |
| `+0x0c` | sub Z buffer |
| `+0x14` | screen delta |
| `+0x18` | X2 lookup |
| `+0x1c` | ZERO_OR_X2 lookup |
| `+0x20` | ZERO lookup |
| `+0x24` | real pitch |
| `+0x30` | pixels per line |
| `+0x3c` | current screen pointer |
| `+0x40` | current depth pointer |
| `+0x44` | current palette pointer |
| `+0x48` | depth delta |
| `+0x4c/+0x4d` | Z1 / Z2 |
| `+0x50` | fixed colour |

Color-math binary signatures include masks `0x0421`, `0xfbde`, and `0x8420`.
