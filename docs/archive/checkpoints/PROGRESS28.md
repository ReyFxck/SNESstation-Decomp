# Progress 28 — structural-source mega closure

Progress 28 converts the remaining committed Progress-16/17 R5900 decompiler
snapshots into one deterministic, host-parseable low-level C corridor.

## Scope

- pre-existing hand-reviewed P16/P17 promotions: **53**
- Progress-28 lifted entries: **186**
  - Progress 16: **112**
  - Progress 17: **74**
- historical P16/P17 entries represented by typed/buildable source after this
  step: **239/239**

The generator verifies the combined translation unit with `cc -std=c11 -Wall -Wextra -fsyntax-only` before changing the promotion registry.
`make audit-source` and `make check` remain the repository-wide gates.

## What this closes

The generated source preserves the target-address labels, decompiled control
flow, Ghidra warning comments, unresolved call labels and target-width scalar
operations.  Failed Ghidra jumptable scope spellings and non-C array-value casts
are lowered mechanically without inventing higher-level behavior.

When all 239 historical P16/P17 addresses are promoted, the
source audit should report **1,041/1,041 behavioral/source-model entries** and
**0 structural-pseudocode-only entries**.

## What this does not prove

This is **not** a claim that the original SNES Station source text, class
boundaries, global types or source-file organization have been recovered
verbatim.  Conservative `DAT_*`/RAM declarations remain where the stripped
binary does not prove a better type.  It is also **not** compiler matching:
the relocation-normalized MATCHING count is unchanged unless a separate
historical-EE object comparison proves otherwise.

The immutable P16/P17 snapshots remain the evidence record behind this generated
corridor.
