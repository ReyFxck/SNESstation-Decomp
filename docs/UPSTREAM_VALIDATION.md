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
