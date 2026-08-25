# V75: C4 float/math proof and 47-entry frontier

> Frozen checkpoint. V76 subsequently promoted `C4SprDisintegrate` and V77
> promoted `C4DrawWireFrame`; use the current 45-entry frontier linked from the
> project status documentation.

V75 promotes five C4 functions with strict historical-source evidence. The
formal checkpoint moves from **989/1041** to **994/1041 (95.49%)**, leaving
**47** audited entries. A sixth exact C4 function, `C4Op15`, is recorded as an
auxiliary companion because its address is not a row in the closed 1,041-entry
manifest; it is not added to the formal count.

## Five new strict matches

| Address | Historical identity | Bytes | Boundary |
|---:|---|---:|---|
| `0x0010b8a4` | `C4TransfWireFrame` | 808 | exact next-manifest boundary |
| `0x0010bbcc` | `C4TransfWireFrame2` | 768 | exact next-manifest boundary |
| `0x0010becc` | `C4CalcWireFrame` | 456 | exact next-manifest boundary |
| `0x0010c094` | `C4Op1F` | 224 | exact object-symbol boundary; following companion pinned |
| `0x0010c1f8` | `C4Op0D` | 264 | exact next-manifest boundary |

The object starts from the hash-pinned official Snes9x 1.41-1 `c4.cpp`. The
target-proven SNES Station variant uses `float` C4 temporaries, `cosf`, `sinf`,
`atanf`, external `fabsf`, and `__builtin_sqrtf`. It is compiled at `-Os` with
the historical EE GCC 3.2.2 C++ frontend, normal double ABI (without
`-fshort-double`) and `-fno-builtin`. Every promoted span has zero differing
non-relocation bytes and no unknown MIPS relocation type.

The target corridor also proves that the audited row at `0x0010c1f8` is
`C4Op0D`, not the provisional `C4Op15` identity used by the V74 planning map.
The real `C4Op15` occupies `0x0010c174..0x0010c1f8`, is exactly 132 bytes, and
pins the complete gap after `C4Op1F` without changing the manifest denominator.

The immutable evidence is split by counting scope:

- [`hunt1041-v75-validated-c4-5.tsv`](../../analysis/matching/hunt1041-v75-validated-c4-5.tsv) — five formal manifest promotions.
- [`hunt1041-v75-c4-companion-1.tsv`](../../analysis/matching/hunt1041-v75-c4-companion-1.tsv) — one exact auxiliary boundary companion.

Reproduce both files with the private reference and historical EE C++ compiler:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v75-evidence
```

## Complete map for the 47 remaining entries

The generated
[`hunt1041-v75-frontier-map-47.tsv`](../../analysis/matching/hunt1041-v75-frontier-map-47.tsv)
covers exactly every non-`MATCHING` manifest row:

| Track | Work packet | Entries | Immediate gate |
|---|---|---:|---|
| Frontend ownership | UI / GS | 15 | class, global and GS ownership |
| Frontend ownership | Pad setup and polling | 2 | pad state layout and exact calls |
| Frontend ownership | Main / lifecycle | 9 | paths, memory-card globals and object order |
| Historical source | Remaining C4 / c4emu | 7 | unaligned-word access, state layout and PS2 deltas |
| Historical source | DSP-1 float | 3 | floating implementation and DSP globals |
| Historical source | Memory / IPS | 2 | allocator and PS2 stream behavior |
| Historical source | ZSNES snapshot | 1 | `fio`, unaligned reads and state layout |
| Historical source | Sound mixer | 5 | PS2 channel layout and mixer arithmetic |
| Historical source | SPC7110 cache | 3 | cache/index records and `fio` ownership |

The next C4 priority is `C4SprDisintegrate` at `0x0010d4f0`: its official
algorithm already has the exact 580-byte size, and the target proves that its
remaining compiler delta is concentrated in PS2 unaligned `READ_WORD` access
and the resulting prologue scheduling. It stays `RECONSTRUCTED` until that
source-level gate reaches a strict zero-byte comparison.

Function closure remains only the first whole-program gate. Data/rodata
placement, constructors, vtables, archive and object order, the linker script
and SJCRUNCH2 packing are still required for a byte-identical final ELF.
