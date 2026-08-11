# PS2 GS/video frontier

This is the active frontier immediately after the recovered zlib 1.1.3
corridor. Names are deliberately conservative until the SNES Station target
itself proves the exact abstraction/class boundary.

## Hard boundary

- `0x00198ac0..0x00198c54` — `adler32`, end of recovered zlib corridor.
- `0x00198c58` — first function of a different PS2 graphics/video block.


## Tracked status

| Address | Provisional name | State |
|---|---|---|
| `0x00198c58` | `graphics_wrapper_entry_A` | identified |
| `0x00198cc8` | `graphics_wrapper_entry_B` | identified |
| `0x00198d78` | `graphics_display_init` | partial |
| `0x00199070` | `gs_privileged_display_program` | partial |

These names describe observed behavior and are not claims about original C++
symbol names. A focused target extract is kept at
`analysis/functions/ps2_gs_frontier_00198c58.asm`.

## Initial target observations

### `0x00198c58` and `0x00198cc8`

These are near-duplicate entry points. Both call `0x00199590`, then enter the
large setup routine at `0x00198d78` with the same default display values.
This shape is compatible with duplicated C++ constructor entry points, but a
class name is **not** assigned yet.

### `0x00198d78`

High-level graphics/display initialization. The target:

- stores width/height/display-mode parameters in an object-like state block;
- resets the GS at `0x12001000`;
- issues the BIOS GS interrupt-mask syscall (`0x71`) after the CSR reset;
- performs CRT/display setup through downstream helpers;
- programs GS privileged registers;
- initializes drawing/display state through a cluster of `0x00199xxx`
  helpers.

This is enough to classify the block as PS2 graphics setup, but not enough to
rename the surrounding C++ object safely.

### `0x00199070`

Programs privileged GS display registers using width/height kept in the
object-like state plus call parameters. Its arithmetic and register targets
are closely related to the historical `GS_SetDispMode` implementation in the
shared PS2 graphics lineage, but its signature/layout differs, so it is kept
provisional rather than renamed directly.

## Historical comparison lead

The public SNESticle tree contains:

- `Gep/Source/ps2/gs.c`
- `Gep/Include/ps2/gs.h`
- release/debug `gs.lst` files built for EE GCC `3.2.2-b1`

That source performs the same low-level GS tasks seen in this target: display
mode register programming, CRT framebuffer selection, draw-environment setup,
GS reset, and CRT initialization. It is a strong structural reference but is
not assumed to be byte-identical SNES Station source.

## Next pass

1. map each `0x00199xxx` helper by reads/writes to GS registers and DMA/GIF
   calls;
2. separate low-level `gs.c`-style routines from the object/class wrapper at
   `0x00198c58+`;
3. use target call sites to recover the object field meanings;
4. only then assign historical names where signatures and behavior agree.
