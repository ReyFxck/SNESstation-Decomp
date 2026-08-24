# Progress 44 — CDVD historical compiler-profile matrix

Progress 43 established that the recovered source family is extremely close to
the SNES Station CDVD corridor, but the first probe was 0/8.

That 0/8 is not a structural rejection:

- five functions already have exactly the target size;
- `CDVD_Init` and `CDVD_FindFile` are target-sized;
- the differences largely begin in prologue scheduling or register choice;
- the target contains `CDVD_GetSize`, which survives in the PGEN revision of
  the same old libcdvd family.

Two historical build profiles are now known:

- SNESticle: GCC 3.2.2-b1, `-O2`, freestanding, no builtins, explicit R5900.
- PGEN: `-O3 -G0 -Wall -fshort-double -mlong64`.

The runner therefore compiles the same historical source under a small,
evidence-driven profile matrix and ranks profiles by:

1. exact function matches;
2. number of functions with target-identical size;
3. total function-size delta.

It also includes `-fno-schedule-insns2` diagnostic variants because several
remaining differences are instruction-order fingerprints.

This step does not promote any profile to historical truth by itself.  It is a
compiler-fingerprint experiment.  The winning profile must still pass the
byte comparator and strict gate.
