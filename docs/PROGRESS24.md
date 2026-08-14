# Progress 24 — four short P16 core helpers

Progress 24 promotes four warning-free entries from the historical Progress-16
R5900 pseudocode snapshot into typed behavioral source:

- `0x0010a264` — packed four-way blend helper using the recovered `__muldi3`
  runtime and the two target lane masks.
- `0x0010b7f8` — selected runtime-register reader with the x8/x9 per-slot
  special cases.
- `0x00115b58` — core reset sequence, including the exact three bulk memory
  initialization sizes/patterns and callback ordering.
- `0x0016ded4` — CPU stack/status push helper, including conditional
  accumulator push, PC offset bytes, status synthesis and post-push state
  updates.

The historical Progress-16 evidence remains committed.  These rows are
promotions to `BEHAVIORAL_SOURCE_MODEL`, not machine-code matching claims.

Expected audit after regeneration:

- behavioral/source-model: **821 / 1,041**
- structural pseudocode only: **220 / 1,041**
- typed promotions: **19**
- relocation-normalized matches remain **7 / 1,041**
