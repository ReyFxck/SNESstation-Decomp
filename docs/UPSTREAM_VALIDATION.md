# Upstream validation policy

The stripped SNES Station image itself contains the strings `Snes9x` and
`1.41`.  Therefore Snes9x 1.41 is the historical baseline to seek for exact
comparison.

A public **Snes9x 1.43** source snapshot was used in progress 2 only as a
close-era *validation aid after binary recovery*.  It is not treated as proof
that SNES Station contains 1.43 code, and code is not copied blindly from it.

Two particularly useful validations:

1. Its `CMemory::Init()` allocates 2bpp/4bpp/8bpp tile caches using
   `MAX_*_TILES * 128`, matching the seemingly oversized allocations in the
   SNES Station binary.
2. Its classic `ConvertTile(uint8 *pCache, uint32 TileAddr)` has the same
   2/4/8-bpp case structure, plane offsets, four lookup-table families and
   64-byte output shape independently recovered at SNES Station VA
   `0x00183e04`.

Rule for this project: old source can confirm a name/shape only **after** the
binary has supplied enough evidence for it.  PS2-specific differences remain
first-class reconstruction targets.


## Shared PS2 unzip validation

Progress 3 identified PKZIP Implode/Explode, Reduce and Shrink directly from the SNES Station binary. Only after that identification, the same common unzip source family and PS2 release listings were located in the public `iaddis/SNESticle` tree.

This is stronger than a loose resemblance because the PS2 release listing for `get_tree` uses the same 96-byte frame and starts with the same machine words as SNES Station `0x0018c124`. It is still treated as validation, not as permission to assume every SNES Station file/function came from SNESticle. See [`TOOLCHAIN_FINGERPRINT.md`](TOOLCHAIN_FINGERPRINT.md).

## Embedded zlib validation

The SNES Station target itself contains the literal version string `1.1.3`
and the adjacent `deflate 1.1.3 Copyright 1995-1998 Jean-loup Gailly` text.
That makes **zlib 1.1.3** the exact baseline for the library block beginning at
`0x00190700`.

The independently identified `compress2`, `compress`, `uncompress` and
`deflateInit_` control flow matches the corresponding zlib 1.1.3 wrapper
structure. Newer zlib versions are not used as the baseline for this block.
