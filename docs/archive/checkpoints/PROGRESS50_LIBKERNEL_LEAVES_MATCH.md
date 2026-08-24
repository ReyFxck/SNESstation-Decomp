# Progress 50 — old EE libkernel leaf gates

Progress 50 closes **16 additional committed-listing matches**, taking the
checkpoint from 22 to **38 functions**.

## Closed syscall-wrapper gates

The historical EE `kernel.S` syscall-wrapper source form is preserved directly
for 14 leaves. Every target function is exactly 16 bytes: load syscall number
into `$v1`, `syscall`, `jr $ra`, `nop`.

Core SIF/RPC-adjacent gate — **11/11 strict**:

- `0x0019ce60` `iWakeupThread`
- `0x0019ce70` `CreateSema`
- `0x0019ce80` `DeleteSema`
- `0x0019ce90` `iSignalSema`
- `0x0019cea0` `WaitSema`
- `0x0019ceb0` `FlushCache`
- `0x0019cec0` `iFlushCache`
- `0x0019ced0` `SifDmaStat`
- `0x0019cee0` `SifSetDma`
- `0x0019cef0` `SifSetReg`
- `0x0019cf00` `SifGetReg`

Late libkernel gate — **3/3 strict**:

- `0x0019f5d0` `SifStopDma`
- `0x0019f5e0` `iSifSetDma`
- `0x0019f5f0` `SifSetDChain`

## DIntr / EIntr — 2/2 strict

Historical 2003-era PS2DEV `glue.c` establishes the behavior. The SNES Station
target has a specific register/scheduling layout, so the formal candidate is a
clearly labelled byte-exact `.S` reconstruction, analogous to the existing
`numtestf`/`get_tree` policy. It is not presented as Hiryu's original assembly.

- `0x0019f018` `DIntr` — 72/72 bytes
- `0x0019f060` `EIntr` — 24/24 bytes

## Progress generator repair

The authoritative machine-code status lives in `analysis/progress_targets.csv`
and `analysis/symbols.csv`. Progress 50 promotes all 38 proven rows to
`MATCHING`, regenerates the source-readiness audit, and makes
`tools/update_progress.py` read `analysis/source_readiness.csv` for the
source-form bar. This prevents the SVG from reverting to the obsolete
802/239 source-form checkpoint after later regeneration.

Expected generated README/SVG state:

- matching: **38/1041 = 3.65%**
- reconstructed or matching: **1041/1041 = 100.00%**
- source-form checkpoint: **1041 behavioral/source-model + 0 pseudocode-only**

The original ELF remains a separate formal gate when the legally obtained
reference is available locally.
