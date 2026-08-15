# Old EE libkernel lineage evidence

This checkpoint intentionally separates source lineage from byte proof.

Primary source snapshot:
- repository: `duduclx/PS2DEV`
- commit: `bac0006c6302edcf1bdae253799484497b4e5032`
- `ps2sdk/ee/kernel/src/kernel.S` preserves the old direct `SYSCALL(name)` macro
  shape: load `__NR_*` into `$v1`, execute `syscall`, `jr $31`, `nop`.
- `ps2sdk/ee/kernel/src/glue.c` preserves the 2003 Marcus R. Brown DIntr/EIntr
  source with COP0 Status bit `0x10000`, `di`/`ei`, and `sync.p`.

Neighboring application evidence:
- `iaddis/SNESticle` commit `9590ebf3bf768424ebd6cb018f322e724a7aade3`
  release map links old `libkernel.a` members including `DIntr.o`, `EIntr.o`,
  `AddDmacHandler.o`, `RemoveDmacHandler.o`, `EndOfHeap.o`, `SifStopDma.o`,
  `iSifSetDma.o`, and `SifSetDChain.o`.

These snapshots establish source lineage. The committed SNES Station target
listings and strict comparator remain the byte proof for the historical syscall/interrupt rows promoted by the matching checkpoints.

Progress 51 also uses the same old `kernel.S` snapshot's explicit `__OPTIMIZE_SIZE__` paths for `memcpy`, `memset`, `strncpy`, and `strlen`. The strict committed-listing gate is 4/4.

Progress 52 adds five more exact `SYSCALL` leaves from the same historical macro and seven old-EE libc assembly routines from `ps2sdk/ee/libc/src`: `strcat.S`, `strncmp.S`, `memcmp.S`, `memmove.S`, `strcpy.S`, `strchr.S`, and `strcmp.S`. Their strict committed-listing gates are 5/5 and 7/7 respectively.
