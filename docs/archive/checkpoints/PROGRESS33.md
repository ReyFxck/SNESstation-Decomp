# Progress 33 — scan-only EE stage-one libc compatibility

Progress 32 established that all 95 non-passing translation units reach the
historical EE front end but are blocked by missing standard headers and parser
cascades.

The bootstrapped GCC 3.2.2 stage one is deliberately header-less. Newlib
1.10.0 also predates the generic Newlib `stdint.h`, while the recovered source
models use modern fixed-width spelling for clarity.

Progress 33 therefore adds a deliberately narrow scan-only compatibility
directory:

- `include/ee_stage1_compat/stdint.h`
- `include/ee_stage1_compat/string.h`

Only `EE_SOURCE_SCAN_FLAGS` sees this directory. `EE_CFLAGS`, matching objects,
and the already-closed 7/7 unwind corridor remain unchanged.

These headers are parser scaffolding, not a claim that they were part of the
original 2004 build.

## Run

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make check
make match-libgcc-unwind-listing-strict EE_CC="$EE_CC"
make ee-source-scan EE_CC="$EE_CC"
python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv --top 30
```
