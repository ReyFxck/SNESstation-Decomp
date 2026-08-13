# Progress 23 — short audio/runtime helper migration

Progress 23 migrates four no-warning Progress-16 entries from structural
pseudocode into typed, host-parseable behavioral source:

- `0x00174564` — stores one 0xe0-stride slot configuration and forwards the
  mode-specific value/selector/scale tuple.
- `0x00174728` — recomputes the slot rate with the target unsigned-DI divide
  and optional goFast floating conversion chain.
- `0x00177cec` — initializes the audio runtime-global group and wraps the
  reset/enable/configure/error flow.
- `0x00177db0` — enforces the small per-slot mode transition state machine.

The source intentionally keeps the historical symbols address-labelled.  The
current evidence proves the target control flow and touched offsets, but does
not yet prove original source names, final owning structures, object boundary,
or compiler matching.

Evidence remains the committed
`analysis/functions/progress16_r5900_pseudocode.c.txt` snapshot.  No row is
promoted to `MATCHING`; the historical EE compiler/comparator remains the
machine-code gate.
