# Progress 26 — large DSP/runtime batch

Progress 26 intentionally switches from the earlier tiny batches to one larger
warning-free corridor migration.  Fifteen historical Progress-16 entries are
promoted to typed behavioral/source models:

- `0x00129250`: binary/BCD ADC accumulator path.
- `0x0012c02c`: 2,048-entry waveform lookup-table builder.
- `0x0012c188`: fixed-point reciprocal/normalization helper.
- `0x0012c2dc`, `0x0012c398`: fixed-point sine/cosine-style table interpolators.
- `0x0012d334`, `0x0012d4ac`, `0x0012d624`: three identical fixed-point
  matrix-building instances with distinct target storage.
- `0x0012e5b8`: packed-nibble resampling helper.
- `0x001305a0`, `0x001306f8`, `0x001309c4`: accelerator/runtime state pack,
  initialization/map construction and dispatch corridor.
- `0x0015e298`: special PPU register read path.
- `0x0015ecec`: eight-plane tile conversion path.
- `0x0015eeac`: PPU block-transfer/address-space helper.

The historical P16 pseudocode remains committed as evidence.  This batch does
not claim historical compiler matching; the relocation-normalized matching
counter remains unchanged until the EE GCC comparator proves otherwise.

Expected audit after regeneration:

- behavioral/source-model: **840 / 1,041**
- structural pseudocode only: **201 / 1,041**
  - Progress-16 remaining: **127**
  - Progress-17 remaining: **74**
- typed promotions: **38**
- relocation-normalized matches: **7 / 1,041**
