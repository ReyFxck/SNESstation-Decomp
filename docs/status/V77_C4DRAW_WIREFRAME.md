# V77: exact C4DrawWireFrame proof

> Frozen checkpoint. V78 subsequently promoted `C4BitPlaneWave`; use the
> current 44-entry frontier linked from the generated project status.

V77 promotes `C4DrawWireFrame` at `0x0010cdcc` with strict
historical-source evidence. The formal checkpoint moves from **995/1041** to
**996/1041 (95.68%)**, leaving **45** audited entries.

## Complete function gate

| Address | Historical identity | Bytes | Boundary | Result |
|---:|---|---:|---|---|
| `0x0010cdcc` | `C4DrawWireFrame` | 472 | exact next-manifest boundary at `0x0010cfa4` | `MATCH` |

The runner begins with the hash-pinned official Snes9x 1.41-1 `c4emu.cpp` and
retains the complete wireframe algorithm. It also retains the V76 PS2
adaptations elsewhere in the translation unit so that the prior
`C4SprDisintegrate` proof remains reproducible.

The new target-proven adaptation is confined to `READ_3WORD`:

- a packed 32-bit access generates the EE `lwl`/`lwr` unaligned-load pair;
- the result is masked to 24 bits before it is passed to
  `S9xGetMemPointer`;
- an empty pointer constraint preserves the historical load form;
- declaring the mask before the loaded value reproduces EE GCC 3.2.2's target
  register allocation without embedding any target instruction.

The resulting object symbol is exactly 472 bytes. All non-relocation bytes
compare equal, all 15 relocations use known precise MIPS masks, and no
instruction byte is broadly ignored. Raw equality is false only because an
unlinked relocatable object cannot contain the target's final relocated
addresses.

The immutable proof is
[`hunt1041-v77-validated-c4draw-1.tsv`](../../analysis/matching/hunt1041-v77-validated-c4draw-1.tsv).
Reproduce it with the private reference and historical EE C++ compiler:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v77-evidence
```

## Frozen 45-entry frontier

The generated
[`hunt1041-v77-frontier-map-45.tsv`](../../analysis/matching/hunt1041-v77-frontier-map-45.tsv)
covers every remaining non-`MATCHING` manifest row:

| Track | Entries | Change from V76 |
|---|---:|---:|
| Frontend ownership | 26 | unchanged |
| Historical-source deltas | 19 | minus `C4DrawWireFrame` |
| Remaining C4 / `c4emu` packet | 5 | down from 6 |

The remaining C4 rows continue as one translation-unit and state-layout
problem. The packed 16-bit and 24-bit access findings are reusable constraints,
but every function still requires a complete zero-byte comparison before
promotion.

Function closure remains only the first whole-program gate. Data and read-only
data placement, constructors, vtables, archive and object order, the linker
script and SJCRUNCH2 packing are still required for a byte-identical final ELF.
