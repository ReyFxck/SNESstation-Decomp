# Hiryu GSLIB map in SNES Station v0.23

The post-zlib graphics corridor is no longer an unnamed GS wrapper. Target
strings, object layouts, call signatures, GS/GIF constants, and then historical
GSLIB source identify a contiguous **Hiryu GSLIB** slice in the stripped target.

## Hard boundaries

- `0x00198ac0..0x00198c54` — `adler32`, end of embedded zlib 1.1.3.
- `0x00198c58` — first `gsDriver` constructor entry.
- `0x00199480` — first `gsPipe` constructor entry.
- `0x0019b7f0` — `gsFont::uploadFont`.
- `0x0019bd38` — `hw.c` vertical-retrace/DMA helpers.
- `0x0019be6c` — final `Dma02Wait` return.
- `0x0019be70` — `CDVD_Init`, a clean boundary into the next subsystem.

Identification followed the repository rule: the target first exposed the
original `gsPipe` diagnostic strings, exact layouts, calls and MMIO behavior;
historical GSLIB was consulted afterwards to validate original names.

## Proven object layouts

### `gsPipe` — 0x34 bytes

| Offset | Historical member | Target role |
|---:|---|---|
| `+0x00` | `m_DmaPipe1` | first half of double-buffered DMA pipe |
| `+0x04` | `m_DmaPipe2` | second half |
| `+0x08` | `m_MemSize` | total allocation size |
| `+0x0c` | `m_CurrentPipe` | active DMA chain base |
| `+0x10` | `m_CurrentDmaAddr` | current DMA tag |
| `+0x14` | `m_CurrentGifTag` | current GIF packet pointer |
| `+0x18` | `m_Buffer` | allocated buffer |
| `+0x1c` | `m_AlphaEnabled` | local alpha state |
| `+0x20` | `m_ZBufferEnabled` | local Z-buffer state |
| `+0x24` | `m_ZTestEnabled` | local Z-test state |
| `+0x28/+0x2c` | `m_OriginX/Y` | drawing origin |
| `+0x30` | `m_FilterMethod` | texture filter state |

### `gsDriver` — 0x74 bytes

The first **0x34 bytes are the embedded `drawPipe`**. Driver fields therefore
begin at `+0x34` and end at `+0x70`. This proves that the `0x74` allocation in
`main @ 0x00104f18`, previously labelled a generic frontend object, is actually
`gsDriver`.

Important fields include width/height at `+0x34/+0x38`, display position at
`+0x3c/+0x40`, PSM at `+0x44`, display/draw buffer indexes at `+0x50/+0x54`,
frame-buffer count at `+0x58`, queue counts at `+0x5c/+0x60`, frame size at
`+0x64`, Z-buffer base/size at `+0x68/+0x6c`, and texture-buffer base at `+0x70`.

### `gsFont` — 0x134-byte aligned object

The target stores the `gsPipe*` at `+0x00`, texture/grid metadata through
`+0x2c`, one-byte Bold/Underline flags at `+0x30/+0x31`, and the **256-byte
character-width table at `+0x32`**. The BFNT pixel payload starts at font-resource
offset `0x120`.

## `gsDriver` address map

| Address | Recovered name |
|---|---|
| `0x00198c58` | constructor entry A |
| `0x00198cc8` | constructor entry B used by `main` |
| `0x00198d38`, `0x00198d58` | destructor entries |
| `0x00198d78` | `setDisplayMode` — older 10-argument revision |
| `0x00199070` | `setDisplayPosition` |
| `0x001990f8` | `clearScreen` |
| `0x00199160` | `InitGraphField` CRT syscall helper |
| `0x00199178` | `getFrameBufferBase` |
| `0x00199198` | `getTextureBufferBase` |
| `0x001991a0` | `getCurrentDisplayBuffer` |
| `0x001991a8` | `getCurrentDrawBuffer` |
| `0x001991b0` | `swapBuffers` |
| `0x001991e8` | `isDrawBufferAvailable` |
| `0x001991f8` | `isDisplayBufferAvailable` |
| `0x00199208` | `setNextDrawBuffer` |
| `0x00199268` | `DrawBufferComplete` |
| `0x00199290` | `DisplayNextFrame` |
| `0x001992f8` | `setDisplayBuffer` |
| `0x00199360` | `setDrawBuffer` |
| `0x001993c8` | `AddVSyncCallback` |
| `0x00199420` | `RemoveVSyncCallback` |
| `0x00199440` | `EnableVSyncCallbacks` |
| `0x00199460` | `DisableVSyncCallbacks` |

The target predates the commonly mirrored replacement signature. Its
`setDisplayMode` still takes:

```text
width, height, xpos, ypos, psm, num_bufs,
TVmode, TVinterlace, zbuffer, zpsm
```

The constructor supplies `320x240`, position `85x42`, two frame buffers and
NTSC/PAL selected from the target configuration byte.

### Driver quirks preserved

- `m_FrameWidth = width & 0xFFC0`; later GSLIB source leaves this old behavior commented out.
- `m_FreeBuffersAvailable = num_bufs - 2`, so a forced one-buffer setup can underflow the unsigned counter.
- `m_CompleteBuffersAvailable` is not initialized by this target routine.
- `main` ignores the constructor return value and stores the original allocated pointer from `$17`.

## `gsPipe` address map

The target follows the historical class order almost method-for-method:

```text
199480/199590  constructors
1996a0/1996d0  destructors
199700/199720  copy constructors
199740          operator=
199830          getPipeSize
199838          InitPipe
199848          ReInit
199898          getBytesLeft
1998b8          FlushCheck
1998f8          Flush
199970          FlushInt
1999e8          setZBuffer
199aa8          setZTestEnable
199b80          setAlphaEnable
199c58          setDither
199cd0          setColClamp
199d48          setPrModeCont
199dc0          setDrawFrame
199e58          setOrigin
199ef8          setScissorRect
199f88          TextureUpload
19a240          TextureDownload
19a500          TextureFlush
19a580          setFilterMethod
19a588          TextureSet
19a748          Line
19a838          TriangleLine
19a9c0          TriangleFlat
19aae8          TriangleGouraud
19ac40          TriangleTexture
19adf8          RectFlat
19af08          RectLine
19b028          RectTexture
19b170          RectGouraud
19b2c8          Point
19b3a8          TriStripGouraud
19b568          TriStripGouraudTexture
```

### Pipe fingerprints / preserved bugs

- Original target strings literally say `gsPipe buffer ...`, proving the class name before source comparison.
- `FlushCheck` reserves `0x90` bytes, exactly `GSPIPE_MINSPACE = 18` historical 64-bit words.
- `operator=` preserves the old omission: it copies Origin/Alpha/ZBuffer state but not ZTest or FilterMethod.
- `TextureFlush` writes **`0xBAD`** to `TEXFLUSH` before flushing the pipe.
- `TextureSet` preserves historical `2^texwidth` / `2^texheight` C code as literal **XOR**; target has `xori ..., 2`, so this must not be modernized to a shift.
- Texture upload/download use the same `0x7ff0` maximum IMAGE qword chunking.

## `gsFont` map

| Address | Recovered name |
|---|---|
| `0x0019b7f0` | `gsFont::uploadFont` |
| `0x0019b948` | `gsFont::Print` |
| `0x0019bad0` | `gsFont::GetCurrLineLength` |
| `0x0019bb68` | `gsFont::PrintLine` |
| `0x001b0790` | old `gsDriver::getTexSizeFromInt` helper used by `Print` |

The target copies 256 character widths, calls `TextureUpload`, chooses alignment,
parses newline/bold/underline control bytes, and renders glyphs with
`RectTexture`; underline uses `RectFlat`. It also preserves the historical
swapped-looking TB offset arithmetic (`charY += TBxpos`, `charX += TBypos`).

## GSLIB `hw.c` tail

| Address | Recovered name |
|---|---|
| `0x0019bd38` | `VRstart_handler` |
| `0x0019bd50` | `WaitForNextVRstart` |
| `0x0019bd78` | `TestVRstart` |
| `0x0019bd88` | `ClearVRcount` |
| `0x0019bd98` | `DmaReset` |
| `0x0019be20` | `SendDma02` |
| `0x0019be40` | `Dma02Wait` |

A notable target-only outcome is `WaitForNextVRstart`: historical `VRcount` was
not declared volatile. The optimized target sets the counter to zero and, for
any positive argument, spins on a register-only branch without reloading the
counter. The intended interrupt-driven wait therefore became an infinite loop.
The reconstruction preserves the machine behavior rather than silently fixing it.

## Recovered source

- `include/gslib_recovered.h`
- `src/ps2/gsdriver_recovered.c`
- `src/ps2/gspipe_recovered.c`
- `src/ps2/gsfont_recovered.c`
- `src/ps2/gslib_hw_recovered.c`
- `src/ps2/gs_fifo_recovered.c` — small canonical helper views

Focused target assembly lives in `analysis/functions/gslib_*` extracts. The next
hard boundary after this recovered GSLIB slice is `CDVD_Init @ 0x0019be70`.

## Matching status

These functions are **RECONSTRUCTED, not MATCHING**. Historical GSLIB validates
names and behavior after target-side identification, but a byte-identical rebuild
with the exact original compiler/flags has not yet been produced.
