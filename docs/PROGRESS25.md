# Progress 25 — four small warning-free P16 helpers

Progress 25 promotes four more entries from the historical Progress-16 R5900
snapshot into typed behavioral source:

- `0x001019a8`: fixed GS/frontend asset uploads and font upload placement.
- `0x00101b64`: fixed frontend panel composition and draw ordering.
- `0x0012b3e8`: eight-channel mask/reset helper, including the target's
  overlapping four-byte zero stores.
- `0x0015cce4`: mode-4 PPU bounds/status helper with packed flags and exact
  X/line clamps.

Opaque GSLIB/project-wide ownership remains represented by narrow hooks where
the historical C++ object types are not yet proven. The committed P16 snapshot
is retained as evidence, and none of these functions is claimed as
machine-code matching.

Expected source audit after regeneration:

- behavioral/source-model: **825 / 1,041**
- structural pseudocode only: **216 / 1,041**
- typed promotions: **23**
- relocation-normalized matches: **7 / 1,041**
