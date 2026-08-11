# Renderer map — progress 2

## Tile pipeline

```
SNES VRAM (global pointer @ 0x0034e2b8)
        |
        v
lookup tables built near 0x00142a78
        |
        v
ConvertTile @ 0x00183e04
        |
        +--> 64 decoded bytes/tile
        +--> validity byte = TRUE / BLANK_TILE
        |
        v
DrawTile family @ 0x0018428c+
        |
        v
pixel writers / color-depth helpers @ 0x001acd04 / 0x001ace28 (TBD)
```

## Recovered global renderer fields

- `0x0035d450` — BG/draw-state structure base (high confidence)
- `0x0035d454` — `BG.BitShift` (2 / 4 / 8)
- BG `+0x24` — current decoded tile-cache pointer
- BG `+0x28` — current validity-map pointer
- `0x0035f9a0` — `odd_high[4][16]`
- `0x0035faa0` — `odd_low[4][16]`
- `0x0035fba0` — `even_high[4][16]`
- `0x0035fca0` — `even_low[4][16]`

The lookup arrays are BSS-zero in the recovered image and are generated at
runtime by the block beginning at `0x00142a78`.

## Macro-expanded draw family boundaries

The following starts were detected from repeating prologue/preamble shapes:

```
0x18428c  DrawTile                high confidence
0x1845a8  DrawClippedTile         C recovered
0x184a40  DrawTilex2              C recovered
0x184d5c  DrawClippedTilex2       high confidence
0x1851f4  DrawTilex2x2            high confidence
0x185510  DrawClippedTilex2x2     high confidence
0x1859a8  DrawLargePixel          C recovered
0x185d8c  DrawTile16              high confidence
0x1860a8  DrawClippedTile16       high confidence
0x186540
0x18685c
0x186cf4
0x187010
0x1874a8
0x18789c
0x187bb8
0x188050
0x18836c
0x188804
0x188b20
0x188fb8
0x1892d4
0x18976c
0x189a88
0x189f20
0x18a23c
0x18a6d4
0x18adb8
```

The alternating stack-frame sizes and repeated cache-miss/flip dispatch are
consistent with the old Snes9x family of DrawTile/DrawClippedTile and scaled /
color-math variants. Exact names after the first pair remain deliberately
unassigned until argument use and pixel writers are mapped.

## Clip masks

- `0x003f4040`: HeadMask = `ffffffff ffffff00 ffff0000 ff000000`
- `0x003f4050`: TailMask = `00000000 000000ff 0000ffff 00ffffff ffffffff`

These constants plus the six-argument ABI prove `0x001845a8` is the clipped tile variant.

## x2 writers

- `0x001acf4c`: `WRITE_4PIXELSx2` — each source pixel becomes two horizontal destination pixels.
- `0x001ad090`: `WRITE_4PIXELS_FLIPPEDx2` — same expansion with source order reversed.
- This independently confirms `0x00184a40` as `DrawTilex2`; its second half uses `Offset + 8`.

## Current complexity boundary

The 8-bit family through `DrawLargePixel` is now structurally mapped. At `0x00185d8c` the code switches to 16-bit pixel writers (`sh` stores). The first pair is clearly DrawTile16/DrawClippedTile16; later pairs introduce add/sub/half color math. This is the first renderer area where exact arithmetic semantics matter more than simple macro shape.

### Additional renderer boundaries

| VA | Label | Confidence |
|---|---|---:|
| `0x001851f4` | `DrawTilex2x2` | high | x2x2 writer pair and +8 second-half offset |
| `0x00185510` | `DrawClippedTilex2x2` | high | clipped partner with same writer pair |
| `0x001859a8` | `DrawLargePixel` | very high / C recovered | six args; one cached pixel expanded over Pixels x LineCount with flip/Z logic |
| `0x00185d8c` | `DrawTile16` | high | first 16-bit (`sh`) writer pair |
| `0x001860a8` | `DrawClippedTile16` | high | clipped partner of first 16-bit pair |
