# Progress 27 — large state/CPU/memory batch

Progress 27 keeps the larger-batch cadence and promotes fifteen warning-free
Progress-16 targets into typed behavioral/source models.

The batch covers:

- `0x00127b78`, `0x00127e00`: the paired CPU interrupt-vector entries,
  including stack writes, status packing, native/emulation vector selection
  and cycle increments.
- `0x0012bd48`, `0x0012cbd8`: paired PPU lookup-table writes and the compact
  floating-transform corridor.
- `0x00150ccc`, `0x00150e18`, `0x00150f54`: the already strongly named
  `CMemory_ScoreHiROM`, `CMemory_ScoreLoROM` and safe printable-string helper.
- `0x0016efa0`, `0x0016f10c`: pending-interrupt/opcode dispatch and the second
  ADC/BCD accumulator path.
- `0x0017028c`, `0x0017124c`, `0x00171e0c`, `0x001721ec`, `0x001725ac`,
  `0x001726ec`: tagged stream reads, load/error wrapping, descriptor-driven
  big-endian save-state serialization/deserialization and the fixed-layout
  runtime dump writer.

The historical P16 pseudocode remains the committed evidence.  This is source
promotion only: relocation-normalized machine-code matching remains unchanged
until an EE GCC object is compared.

Expected audit after regeneration:

- behavioral/source-model: **855 / 1,041**
- structural pseudocode only: **186 / 1,041**
  - Progress-16 remaining: **112**
  - Progress-17 remaining: **74**
- typed promotions: **53**
- relocation-normalized matches: **7 / 1,041**
