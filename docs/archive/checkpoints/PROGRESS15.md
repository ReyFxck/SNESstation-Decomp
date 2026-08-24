# Progress 15 — pad init, renderer leaf, and soft-double add

Progress 15 continues directly from Progress 14 (799 reconstructed / 827 mapped).

This checkpoint promotes three already-mapped targets only after complete
behavioral models were added:

- `0x00143780` — isolated renderer 4bpp cache/setup leaf;
- `0x001a3380` — `_fpadd_parts_d`, including NaN/Inf/zero handling, sticky
  exponent alignment, signed add/subtract, normalization and carry renormalization;
- `0x001a8484` — `padInit`, including both XPADMAN bind/retry corridors,
  the target delay window, module-version query, eight 0x28-byte state clears,
  and final RPC command `0x10`.

## Accounting

Using the existing 1,137-target conservative JAL proxy:

- **802 reconstructed — 70.54%**
- **827 mapped — 72.74%**
- **0 matching — 0.00%**

No new scanner hit is added in this checkpoint.  The remaining PARTIAL target is
`main @ 0x00104f18`; 24 targets remain IDENTIFIED.

## Validation

`tools/apply_progress15.py`:

- verifies that the input manifests are the Progress 14 totals;
- promotes exactly the three addresses above in both manifests;
- regenerates README/progress SVG/generated progress documentation;
- checks every recovered C translation unit with
  `cc -std=c11 -Wall -Wextra -Werror -fsyntax-only -Iinclude`;
- builds and runs a Progress 15 host smoke test covering pad bind/retry/RPC
  flow, the renderer invalidation leaf, special soft-double values, and
  randomized finite binary64 additions.
