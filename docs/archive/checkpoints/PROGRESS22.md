# Progress 22 - three more small P16 migrations

This checkpoint migrates three short, warning-free Progress-16 targets into
typed behavioral C without claiming machine-code matching.

- `0x00103b34` preserves the frontend message-slide/scissor loop, lazy
  `gsDriver` allocation, CD stop, text draw and per-frame presentation flow.
- `0x00104234` preserves the whole-file loader: open, seek-to-end size query,
  64-byte-aligned allocation with the target's extra 0x40 rounding, `0xff`
  fill, rewind/read, trailing NUL and 1/2 result convention.
- `0x0015d334` preserves the short conditional PPU path around FillRAM offsets
  `0x3030`, `0x3039` and `0x303a`, including the `-1`/`700`/`0x15e` selector
  and the final `0x8020` test.

The committed Progress-16 pseudocode snapshot remains historical evidence.
`analysis/source_promotions.csv` only overrides readiness classification for
these addresses. Exact EE compiler matching remains a separate gate.

After regeneration the expected audit is 813 behavioral/source-model entries,
228 pseudocode-only entries, and 11 typed promotions.
