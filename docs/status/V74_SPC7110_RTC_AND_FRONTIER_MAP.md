# V74: SPC7110 RTC proof and complete frontier map

V74 promotes both SPC7110 RTC record functions with strict historical-source
evidence. The formal checkpoint moves from **987/1041** to **989/1041
(95.00%)**, leaving **52** audited entries. It also replaces the partial V73
frontend queue with one generated map covering the complete remaining frontier.

> This is a frozen V74 checkpoint. V75 promotes five C4 rows, corrects the
> `0x0010c1f8` identity to `C4Op0D`, and regenerates the live frontier at 47
> entries in
> [`V75_C4_FLOAT_MATH_AND_FRONTIER.md`](V75_C4_FLOAT_MATH_AND_FRONTIER.md).

## Two new strict matches

| Address | Historical identity | Bytes | Boundary | Proven target delta |
|---:|---|---:|---|---|
| `0x001833a4` | `S9xSaveSPC7110RTC` | 332 | exact next-manifest boundary | `fioOpen(…, 0x202)`, 24 one-byte `fioWrite` operations and `fioClose` |
| `0x001834f0` | `S9xLoadSPC7110RTC` | 360 | terminal `jr $ra`; following 8-byte empty leaf and next prologue pinned | `fioOpen(…, 1)`, 24 one-byte `fioRead` operations and `fioClose` |

Both functions come from the hash-pinned official Snes9x 1.41-1
`spc7110.cpp` after two target-proven PS2 adaptations: stdio becomes `fio`, and
`S7RTC::last_used` is fixed to a 32-bit `int`. The EE GCC 3.2.2 objects compare
with zero differing non-relocation bytes and no unknown MIPS relocation type.

The immutable result is
[`hunt1041-v74-validated-spc7110-rtc-2.tsv`](../../analysis/matching/hunt1041-v74-validated-spc7110-rtc-2.tsv).
Reproduce it with the private reference and historical EE C++ compiler:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v74-evidence
```

## One map for all 52 remaining entries

The generated
[`hunt1041-v74-frontier-map-52.tsv`](../../analysis/matching/hunt1041-v74-frontier-map-52.tsv)
covers the exact set of non-`MATCHING` manifest rows. It balances the work as
two coordinated tracks of 26 entries each:

| Track | Work packet | Entries | Working translation unit | Immediate gate |
|---|---|---:|---|---|
| Frontend ownership | UI / GS | 15 | `src/ps2/frontend_ui_recovered.cpp` | class, global and GS ownership |
| Frontend ownership | Pad setup and polling | 2 | `src/ps2/frontend_pad_recovered.cpp` | pad state layout and exact calls |
| Frontend ownership | Main / lifecycle | 9 | `src/app/main_frontend_recovered.cpp` | paths, memory-card globals and object order |
| Historical source | C4 core | 12 | `src/snes9x/c4_ps2_recovered.cpp` | SNES Station float/math and state-layout deltas |
| Historical source | DSP-1 float | 3 | `src/snes9x/dsp1_ps2_recovered.cpp` | floating implementation and DSP globals |
| Historical source | Memory / IPS | 2 | `src/snes9x/memory_ps2_recovered.cpp` | allocator and PS2 stream behavior |
| Historical source | ZSNES snapshot | 1 | `src/snes9x/snapshot_ps2_recovered.cpp` | `fio`, unaligned reads and state layout |
| Historical source | Sound mixer | 5 | `src/snes9x/soundux_ps2_recovered.cpp` | PS2 channel layout and mixer arithmetic |
| Historical source | SPC7110 cache | 3 | `src/snes9x/spc7110_ps2_recovered.cpp` | cache/index records and `fio` ownership |

This means structure/ownership and byte matching continue together. Layout work
is done at translation-unit scope, while promotion remains per function and
requires the same strict ELF gate.

## Corrections to the V73 working classification

Target-body inspection supersedes two provisional V73 packet labels:

- `0x00104a54` and `0x00104bbc` belong to frontend pad setup/polling work, not
  to a two-function `libmtap` leaf packet.
- the 12 unmatched entries in `0x0010b8a4..0x0010d7dc` are the C4
  coprocessor corridor, not APU core. The V74 map records all 12 proven
  historical identities from `C4TransfWireFrame` through `S9xSetC4`.

The address-based manifest labels are retained where an exact historical
symbol has not yet been promoted; the corrected work packets do not pretend
that a provisional original filename is proven.

Function closure is only the first whole-program gate. Data/rodata placement,
constructors, vtables, archive and object order, the linker script and
SJCRUNCH2 packing remain necessary for a byte-identical final ELF.
