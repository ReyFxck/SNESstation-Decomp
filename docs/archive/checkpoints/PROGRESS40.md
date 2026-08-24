# Progress 40 — fix GSLIB matching flag expansion

The first real GSLIB listing probe reported 0/7, but its compiler command
showed that the probe had lost the historical EE compiler flags entirely:

```text
ee-gcc -Imatching/ee_abi_compat -c ...
```

The cause was GNU make evaluation order. `GSLIB_HW_EE_CFLAGS` used `:=` before
`EE_CFLAGS` was defined, so `$(EE_CFLAGS)` was expanded immediately to an empty
string.

Progress 40 changes that assignment to recursive `=` expansion:

```make
GSLIB_HW_EE_CFLAGS = $(EE_CFLAGS) -Imatching/ee_abi_compat
```

Now `EE_CFLAGS` is evaluated when the recipe runs, after its definition exists.

The previous 0/7 result is invalid as a compiler-match measurement.

## Run

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make check
rm -f build/matching/gslib_hw/gslib_hw.o
make match-gslib-hw-listing EE_CC="$EE_CC"
```

The compile line must visibly contain the full historical flags, including
`-G0 -O2 -fomit-frame-pointer -march=r5900 -mtune=r5900`.
