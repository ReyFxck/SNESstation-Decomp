# Progress 52 — historical old-EE libkernel/libc batch

Progress 52 supersedes Progress 51 and its host-syntax hotfix.

New strict rows over the 42-function checkpoint:

- seven historical old-EE libc assembly functions: `strcat`, `strncmp`, `memcmp`, `memmove`, `strcpy`, `strchr`, `strcmp`;
- five historical `SYSCALL` leaves: `_EnableDmac`, `_DisableDmac`, `SignalSema`, `PollSema`, `iSifSetDChain`.

Historical lineage is `duduclx/PS2DEV@bac0006c6302edcf1bdae253799484497b4e5032`.
The libc candidates preserve the historical assembly source form; the syscall
candidates use the historical `kernel.S` macro form. Target bytes come from
committed SNES Station focused listings. The strict comparator remains the
formal function-level gate.

Checkpoint after apply: **54/1041 MATCHING = 5.19%**.
README and `assets/progress.svg` are regenerated from authoritative manifests.

The host-only CDVD syntax guard is included: EE builds preserve the historical
bare `return;` in `CDVD_GetSize`, while modern host syntax checks see `return 0;`.

> **Revision 3 host-syntax persistence fix:** persists the Makefile host-syntax flags (`-Imatching/ee_abi_compat` and host guard) before moving on, and safely resumes a tree left partially applied by the first Progress 52 package. This does not change any matching candidate bytes or the 54-function checkpoint.
