# Progress 32 — lock the 7/7 unwind probe and expose real EE parser blockers

Progress 31 closed the trustworthy committed-listing unwind corridor at
**7/7 relocation-normalized matches** with the bootstrapped EE GCC 3.2.2.

That local result is a compiler/source fingerprint and a regression gate. It
does not replace the formal original-ELF gate, which still requires the user's
legally obtained `original/SNES_EMU.ELF`.

## Why the 95-TU triage needed one more fix

The historical source scan reported 95 `EE_C_ERROR` translation units, but many
stored first diagnostics were warnings. GCC 3.2.x often prints hard parser
failures using older wording such as `parse error before ...` without the modern
literal `error:` prefix.

Progress 32 makes the diagnostic pass intentionally quiet with `-w`, extends
the diagnostic selector for old GCC wording, and recognizes the stage-one
compiler's old `header: No such file or directory` form as `MISSING_HEADER`.

No recovered source semantics are changed in this progress step.

## Run

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make check
make match-libgcc-unwind-listing-strict EE_CC="$EE_CC"
make ee-source-scan EE_CC="$EE_CC"
python3 tools/summarize_ee_scan.py build/ee-source-scan/report.csv --top 30
```

The resulting grouped diagnostics are the input to the next compatibility
batch. Fix the largest real parser/type blocker first rather than editing
translation units one by one.
