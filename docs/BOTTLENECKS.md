# Bottleneck watch — SNES Station v0.23 WIP

## Current status

The obvious early bottlenecks have **not** materialised:

- Startup/PS2SDK wrappers are straightforward.
- GUI/ROM selection and the application/core boundary are visible.
- `CMemory::Init`, `CMemory::LoadROM`, SRAM load/save and the per-ROM cleanup
  boundary retain recognisable Snes9x structure.
- The tile converter is now recovered and the first tile drawer is identified.

## Correction: decoded tiles are 64 bytes, not 16-bit cached pixels

An earlier pass inferred 16-bit cached pixels from the allocation sizes.  That
was wrong and is intentionally corrected here.

`ConvertTile` at EE VA `0x00183e04` emits two 32-bit words per scanline for
eight scanlines: **64 bytes per decoded tile**.  `DrawTile` at `0x0018428c`
computes its cache address with `tile_index << 6`, independently proving the
same 64-byte stride.

The allocations in `CMemory::Init` are nevertheless 128 bytes per possible
tile (`0x80000`, `0x40000`, `0x20000`).  A close-era Snes9x 1.43 source tree
uses the same `MAX_*_TILES * 128` allocation pattern, so this is inherited
layout/over-allocation rather than proof of a PS2-specific 16-bit tile cache.

## The real renderer bottleneck: macro-expanded function families

The region beginning at `0x0018428c` contains a repeating family of functions
with alternating ~0x80 and ~0xb0 stack frames.  The first pair maps cleanly to
classic Snes9x-style:

- `0x0018428c` — `DrawTile(Tile, Offset, StartLine, LineCount)`
- `0x001845a8` — probable `DrawClippedTile(...)` (name still provisional)

The same preamble, `ConvertTile` cache miss path, flip-bit dispatch and pixel
writer pattern repeats many times.  This is consistent with old Snes9x renderer
macros expanding into many near-duplicate normal/x2/x2x2/transparency/color-
math functions.

That means the difficult part is **volume + exact macro/inline recovery**, not
an opaque PS2-only renderer.

## R5900 disassembly is manageable

LLVM prints some valid Emotion Engine instructions as `<unknown>`.  Restricting
the count to the likely EE-code range `0x00100000..0x001affff` gives only 859
unknown instructions:

| class | count |
|---|---:|
| R5900 3-operand `MULT` | 789 |
| `LQ` | 16 |
| `MFLO1` | 16 |
| `EI` | 8 |
| `SQ` | 8 |
| `SQRT.S` | 6 |
| `MTLO1` | 6 |
| `DI` | 6 |
| `DIVU1` | 2 |
| `MULTU1` | 2 |

So ~92% of the problem is one R5900 `MULT` encoding, and the remainder is a
small known set.  `tools/annotate_r5900_unknown.py` annotates these forms.

## Remaining likely hard areas

1. Recovering the exact old C++/macro boundaries for the large draw family.
2. Recovering exact 2003-era Snes9x structure layouts and global aliases.
3. Finding the original GCC/PS2SDK revision/flags for matching builds.
4. Separating Hiryu's PS2 renderer/timing changes from upstream Snes9x 1.41.
5. Audio glue and any EE/IOP scheduling assumptions around SjPCM/AmigaMod.

## Naming discipline

`0x00151330` remains `rom_cleanup_00151330`, **not** `CMemory::Deinit`.
It does not free the large allocations made by `0x00151074`; a stronger name
would still be an unsupported guess.
