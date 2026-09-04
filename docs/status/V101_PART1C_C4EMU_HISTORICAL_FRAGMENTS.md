# V101 Part 1C — c4emu.cpp source fragments

The V101 lineage probe identified Snes9x 1.41-1 `c4emu.cpp` as the exact
source provider for three C4-family Stage-3F addresses.

Fresh EE GCC 3.2.2 `-Os` builds prove:
- `.data + 0x000`, size `0x080`: `C4TestPattern` + local `bmpdata`;
- `.data + 0x080`, size `0x400`: `C4SinTable[512]`;
- `.data + 0x480`, size `0x400`: `C4CosTable[512]`.

All three complete spans map to target `.data` base `0x003359d0`. The gate
requires exact symbol geometry, no data relocations in the claimed 0x880-byte
range, a fresh rebuild from the hash-pinned Snes9x 1.41-1 source, and complete
byte equality against the SHA-pinned target.

These are scoped source fragments; no whole-TU or replacement-ELF identity is
claimed.

Expected Stage-3F: 1209 section-backed / 17 NO_PROVED_BACKING.
