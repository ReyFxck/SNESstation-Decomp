# Progress 38 — first non-runtime local matching expansion: GSLIB hw

The historical EE front-end gate is closed at 101/101 translation units and the
committed libgcc-unwind local gate remains 7/7.  The next matching probe moves
outside Newlib/libgcc into SNES Station's recovered early Hiryu GSLIB hardware
tail.

The committed target listing spans `0x0019bd38..0x0019be70` and contains seven
small helpers:

- `VRstart_handler`
- `WaitForNextVRstart`
- `TestVRstart`
- `ClearVRcount`
- `DmaReset`
- `SendDma02`
- `Dma02Wait`

The probe deliberately compiles `src/ps2/gslib_hw_recovered.c` directly rather
than duplicating it under `matching/candidates/`.  This keeps the historical
101-TU front-end checkpoint stable and tests whether the recovered source itself
reproduces the committed machine-code corridor.

Run the non-strict probe first:

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make check
make historical-ee-gate EE_CC="$EE_CC"
make match-gslib-hw-listing EE_CC="$EE_CC"
```

Do not promote this corridor to a matching claim until the report is measured.
The original user-supplied ELF remains the formal gate.
