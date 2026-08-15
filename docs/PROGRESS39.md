# Progress 39 — make the GSLIB hw probe self-contained

The first Progress-38 run stopped before byte comparison because the isolated
stage-one EE compiler intentionally has no installed Newlib `stdint.h`.

The historical 101/101 syntax scan solved that with a broad scan-only
compatibility include path.  Matching should not inherit that whole diagnostic
shim, so this progress adds a narrower ABI-only header specifically for the
GSLIB probe:

- `uint32_t` -> 32-bit unsigned int
- `uintptr_t` -> 32-bit unsigned int

No implementation functions, C11 bridges, int128 feature macros, or standard
library behavior are injected.

Only `GSLIB_HW_EE_CFLAGS` gains `-Imatching/ee_abi_compat`; the global
`EE_CFLAGS`, mathfp, libgcc, and historical 101/101 gate remain unchanged.

## Run

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make check
make historical-ee-gate EE_CC="$EE_CC"
make match-gslib-hw-listing EE_CC="$EE_CC"
```

The last command is deliberately non-strict.  Its first useful result is the
actual number of relocation-normalized matches in the seven-function GSLIB hw
corridor.
