# Progress 41 — shape the first GSLIB hardware misses

The corrected Progress-40 probe produced **3/7** local listing matches:

- MATCH `VRstart_handler`
- MATCH `TestVRstart`
- MATCH `ClearVRcount`
- MISS `WaitForNextVRstart`
- MISS `DmaReset`
- MISS `SendDma02`
- MISS `Dma02Wait`

The first three matches prove the historical compiler/flags and recovered
`VRcount` model are viable for this corridor.

Progress 41 makes evidence-driven source-shape corrections:

- `WaitForNextVRstart` uses an unsigned count and the original counter
  comparison, allowing GCC 3.2.2 to produce the target `sltu`-based invariant
  loop after folding the non-volatile global.
- `DmaReset` uses direct volatile MMIO lvalues instead of inline store helpers,
  matching the target's absolute `lui $1` / `sw` sequence.
- `SendDma02` keeps one channel-2 base pointer, matching the target's single
  base-register sequence.
- Manifest ends for `WaitForNextVRstart` and `DmaReset` are corrected to
  exclude alignment NOPs (`0x0019bd74` and `0x0019be1c` respectively).

`Dma02Wait` is intentionally left alone for this iteration; its saved `$8`
shape may represent historical inline assembly or a more specific source idiom
and should be isolated after measuring these safer corrections.

## Run

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"

make check
rm -f build/matching/gslib_hw/gslib_hw.o
make match-gslib-hw-listing EE_CC="$EE_CC"
```
