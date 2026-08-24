# Progress 30 — repair the EE scan and make the libgcc probe honest

Progress 29 exposed two measurement problems before it exposed 101 independent
source failures.

## 1. EE source-scan flag repair

GNU make treats commas as function-argument separators. The previous
`filter-out` expression embedded `-Wa,-al` directly, so the generated scanner
argument began with a corrupted `-al,-G0` token. Progress 30 removes `-Werror`
first and strips `-Wa,-al` with an explicit comma variable.

The next `make ee-source-scan` result is therefore the first usable historical
GCC 3.2.2 translation-unit baseline.

## 2. Listing coverage is now bounded to real bytes

`analysis/functions/libgcc_frontier_001a1b00.asm` currently stops at
`0x001a40fc`. The listing-to-binary helper zero-fills unwritten addresses, so
extending that synthetic image to `0x001a5cc0` made five unavailable functions
look like byte mismatches.

The local probe now ends at `0x001a4100` and uses a seven-row manifest. The
12-row manifest remains unchanged for the formal original-ELF comparison.

## 3. GCC unwind source-shape recovery

The four compact DWARF helpers now mirror the historical GCC unwind source shape
more closely: word-mode `_Unwind_Word`/`_Unwind_Sword` equivalents, `abort()`
after switch fallthrough, `sizeof(void *)` for absolute pointers, and the old
LEB128 sign-extension expression.

Run:

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make check
make ee-source-scan EE_CC="$EE_CC"
make match-libgcc-unwind-listing EE_CC="$EE_CC"
make match-libgcc-unwind EE_CC="$EE_CC"
```

Do not promote any new match until the formal original-ELF report agrees.
