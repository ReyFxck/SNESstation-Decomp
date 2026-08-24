# Progress 35 — final historical EE front-end blockers

Progress 34 reached 98/101 passing C translation units with no compiler crash
and preserved the strict 7/7 committed-listing libgcc-unwind gate.

The three remaining front-end blockers were isolated:

- `progress21_small_helpers_recovered.c` needs `ctype.h`;
- `gsfont_recovered.c` contains C99 `for (int ...)` declarations, rewritten to
  behavior-equivalent declaration-before-loop form for GCC 3.2.2;
- `progress28_structural_lift_recovered.c` uses modern `__uint128_t` /
  `__SIZEOF_INT128__` spelling. The scan-only compatibility layer maps that
  spelling to GCC's historical `mode(TI)` integer mode.

The TI bridge exists only in `include/ee_stage1_compat/stdint.h`, used by
`EE_SOURCE_SCAN_FLAGS` and never by matching `EE_CFLAGS`.

## Run

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make check
make match-libgcc-unwind-listing-strict EE_CC="$EE_CC"
make ee-source-scan EE_CC="$EE_CC"
python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv --top 30
```

If the EE backend rejects `mode(TI)` even under `-fsyntax-only`, retain the
98/101 checkpoint and record Progress 28 as a compiler-capability exception
instead of weakening the matching compiler contract.
