# Progress 51 — old EE libkernel leaves + size-optimized strings

This single overlay applies directly over main `fcd40b8...`; Progress 50 is included and does not need to be applied first.

New strict gates in the round: 14 syscall leaves + DIntr/EIntr + four historical `kernel.S` `__OPTIMIZE_SIZE__` paths = **20 new matches**.

The size paths are `memcpy` (56/56), `memset` (56/56), `strncpy` (84/84), and `strlen` (40/40). Historical source is `duduclx/PS2DEV@bac0006c6302edcf1bdae253799484497b4e5032`, `ps2sdk/ee/kernel/src/kernel.S`. Historical `move` pseudo-ops are normalized to their EE `daddu` expansion for deterministic assembly; the bit-precise strict comparator is the byte proof.

Checkpoint after apply: **42/1041 MATCHING = 4.03%**. README and `assets/progress.svg` are regenerated from authoritative manifests.
