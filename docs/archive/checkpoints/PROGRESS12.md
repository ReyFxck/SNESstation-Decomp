# Progress 12 — fixed transforms, slot runtime, and persistence/state I/O

Progress 12 continues the earlier executable-body pass started in Progress 11.
It promotes only targets whose `jal` references originate below the confirmed
code/data boundary at `0x001b0880` and whose complete reachable control flow is
present in the target disassembly.  Scanner-only data hits remain excluded.

## Reconstructed target families

Twenty additional code-referenced targets are reconstructed in this checkpoint.

### Fixed-point transform family

`src/snes9x/progress12_fixed_transform_recovered.c` covers six compact 3x3
signed Q15 transform leaves:

- row-vector forms at `0x0012d79c`, `0x0012d85c`, `0x0012d91c`;
- transposed/column forms at `0x0012d9dc`, `0x0012da9c`, `0x0012db5c`.

The target shifts each individual signed 16-bit product right by 15 *before*
summing the three terms.  The host model preserves that ordering and explicit
arithmetic-right-shift behavior rather than replacing it with a mathematically
similar matrix operation.

### 0xe0-stride slot/controller runtime

`src/snes9x/progress12_slot_runtime_recovered.c` covers:

- `0x00173f24` — indexed configure wrapper;
- `0x00173f50` — raw-axis/magnitude update and divide-by-128 scaling;
- `0x0017422c` — mode/rate period calculation, phase fold, config dispatch;
- `0x001742f8` — nonzero-slot activation and `(8,-1,0)` reconfigure;
- `0x00174618` — scale/packed-mode update and conditional stop dispatch;
- `0x001746a4` — two-global state gate around the observed `+0x4c` field;
- `0x00174830` — `+0x3c` store followed by recomputation.

Address-based names remain intentionally conservative where a historical
symbol is not proved.  The target's truncating signed division and 32-bit
wrapped multiply/shift behavior are kept explicit.

### Frontend path, persistence, memory-card, and state transfer

`src/ps2/progress12_io_state_recovered.c` covers:

- `0x00103d90` — `cdfs:%s/%s` path construction using the target's
  `index*0x90-0x84` record-path addressing;
- `0x00106824` — one-time `mcInit`, `mcGetInfo(0,0)`, `mcSync`, and card-type-2
  result;
- `0x0016fb04` / `0x0016fbb4` — dirty-save and load corridors for the compact
  record store;
- `0x00172174` — `%s:%06d:` header output followed by the payload;
- `0x00183c58` / `0x00183d40` — segmented template/backing-state copies with
  selector-derived offset and refresh ordering.

Several target quirks are deliberately retained.  The load path mirrors the
raw successful `fioRead` return into both counters rather than interpreting it
as an 8-byte record count.  The save path computes its write byte count with a
32-bit left shift and marks the store saved even when file creation fails.  The
state-transfer selector uses the target's masked variable-shift semantics and
clamps values greater than `0x20000`.

## Evidence

`analysis/functions/progress12_targets.asm` contains the focused reachable
instruction evidence for all twenty promoted targets.  The set contains 738
reachable target instructions.  Each promoted address occurs in
`analysis/jal_candidates.csv` with at least one first call site below
`0x001b0880`; no data-only scanner hit is promoted by this checkpoint.

## Accounting

On the conservative 1,137-target JAL proxy:

- **745 reconstructed — 65.52%**
- **799 mapped — 70.27%**
- **0 matching — 0.00%**

This is **+20 reconstructed and +20 mapped** over the full Progress 11 base
(725 reconstructed / 779 mapped).

## Validation

- all **77** recovered C translation units pass
  `cc -std=c11 -Wall -Wextra -Werror -fsyntax-only -Iinclude`;
- the Progress 12 host smoke test passes fixed-transform arithmetic, slot state
  transitions/scaling/callback order, path/header formatting, memory-card lazy
  initialization, persistence load/save semantics, and state-transfer round
  trips;
- `analysis/progress_targets.csv` contains **799 unique addresses**;
- `analysis/symbols.csv` contains **799 unique addresses**;
- all twenty new rows are present in the JAL-candidate scan with a first call
  site inside executable code;
- no target is promoted to MATCHING.

Matching remains **0.00%** until a candidate historical EE build is compared
byte-for-byte against target machine code with relocations/link placement
normalized.
