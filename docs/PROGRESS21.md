# Progress 21 - small frontend and DI helpers

This checkpoint promotes three warning-free Progress-16 targets to typed,
independently parseable behavioral C while preserving the historical P16
pseudocode snapshot as evidence.

- `0x00103cd4`: formats a 0x90-byte browser/TOC entry name. It copies at most
  0x80 bytes, forces a terminator, strips the final extension for non-directory
  entries (`flags & 2 == 0`), then replaces underscores with spaces.
- `0x00106c08`: recognizes the memory-card save-state filename suffix used by
  the adjacent `SNES_EMU/*.00?` enumeration and returns its decimal slot.
  The two-byte data object at `0x001b1348` remains address-bound rather than
  guessing its final source ownership.
- `0x001a7638`: signed 64-bit remainder wrapper around the unsigned div/mod core
  at `0x001a7788`; magnitude conversion and numerator-sign restoration follow
  the committed R5900 decompile.

These are source-model promotions only. Relocation-normalized `MATCHING` status
still requires generated EE object evidence.

After regeneration, the source audit should move from 807 behavioral / 234
pseudocode-only to 810 behavioral / 231 pseudocode-only, with 8 typed
promotions retained over the historical P16/P17 snapshots.
