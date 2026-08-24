# Progress 11 — earlier-core small-target recovery

Progress 11 moves back into the earlier frontend/Snes9x body now that the
high-address executable tail is closed.  The checkpoint is deliberately driven
by target call evidence rather than by percentage: short JAL targets were first
separated from scanner hits whose only apparent call sites live in post-code
data.

## Triage

The focused short-target set contained 65 candidates.  Re-reading each
candidate against `analysis/jal_candidates.csv` and the executable end at
`0x001b0880` split them into:

- **51 candidates with at least one call site in executable code**;
- **14 scanner false positives / data-only hits**, not promoted;
- **40 code-referenced targets reconstructed in this checkpoint**;
- **11 code-referenced targets deferred** because their control flow or wider
  state contract still deserves a larger reconstruction.

The 14 rejected targets are:

`0x00116040`, `0x00140010`, `0x0017a398`, `0x00143030`, `0x00141424`,
`0x00183ba8`, `0x00117044`, `0x00183c00`, `0x0013cbc4`, `0x001413fc`,
`0x0011d044`, `0x0018543c`, `0x0014203c`, `0x00142020`.

This matters because a raw `jal`-pattern scan can decode aligned data words as
calls.  Progress accounting therefore does not promote such words merely to
increase coverage.

## Reconstructed behavior

`src/ps2/progress11_frontend_recovered.c` adds compact target models for:

- VSync callback teardown and the 640x480 GSLIB clear wrapper;
- two file open/process/close wrappers;
- the small RPC command 3/4/5 leaves and two command-1 word RPC copies;
- audio-rate/default display configuration;
- two target allocation cleanup leaves;
- short/float transform glue;
- controller-axis mixing, table update and neutral-state detection.

`analysis/functions/progress11_short_targets.asm` keeps the focused target-side
instruction evidence for all 40 promoted entries.  `src/snes9x/progress11_core_recovered.c` adds:

- little-endian two-byte read/write helpers;
- two status-bit table selector families (five target copies total);
- the four-page memory-map initializer;
- the 75-entry, 0x20-byte cheat record add/apply/restore corridor;
- D/E/F special-bank pointer/byte lookup helpers;
- the 13-byte renderer code-stream reader;
- the exact 8 x 256 packed 16-bit lookup-table generator;
- transparent-nibble merge logic;
- a CPU state block-boundary/reset helper preserving the target
  `0xffffecff` flag mask.

Address-based names remain in use where an original source symbol has not been
proven.  The cheat models preserve the target distinction between a direct map
write (`Map >= 0x12`) and the fallback byte-write path through explicit host
callbacks rather than collapsing the two paths.

## Accounting

On the conservative 1,137-target JAL proxy:

- **725 reconstructed — 63.76%**
- **779 mapped — 68.51%**
- **0 matching — 0.00%**

This is +40 reconstructed and +40 mapped targets over Progress 10.

## Validation

- all **74** recovered C translation units pass
  `cc -std=c11 -Wall -Wextra -Werror -fsyntax-only -Iinclude`;
- the Progress 11 host smoke test covers byte read/write ordering, map-page
  generation, selector branches, cheat apply/restore, D/E/F bank selection,
  code-stream state transitions, lookup generation, nibble merge, CPU-state
  update, RPC packet behavior, display/audio defaults and GSLIB wrapper order;
- `analysis/progress_targets.csv` contains 779 unique addresses;
- `analysis/symbols.csv` contains 779 unique addresses;
- every one of the 40 new rows is present in the JAL-candidate scan and has a
  first call site below the executable/data boundary `0x001b0880`.

Matching remains zero.  A behavioral C model is not upgraded to MATCHING until
a historical EE toolchain candidate is rebuilt and compared against target
machine code after relocation handling.
