# V78: exact C4BitPlaneWave proof

> Frozen checkpoint. V79 subsequently promoted `C4ConvOAM`; use the current
> 43-entry frontier linked from the generated project status.

V78 promotes `C4BitPlaneWave` at `0x0010d2a8` with strict
historical-source evidence. The formal checkpoint moves from **996/1041** to
**997/1041 (95.77%)**, leaving **44** audited entries.

## Complete function gate

| Address | Historical identity | Bytes | Boundary | Result |
|---:|---|---:|---|---|
| `0x0010d2a8` | `C4BitPlaneWave` | 584 | exact next-manifest boundary at `0x0010d4f0` | `MATCH` |

The runner begins with the hash-pinned official Snes9x 1.41-1 `c4emu.cpp` and
retains the complete wave renderer. It reapplies the previously proven SNES
Station PS2 adaptations in the same translation unit before isolating the new
function gate.

Four packed 16-bit reads reproduce the EE unaligned-load forms while retaining
their distinct target load registers. The ordinary portable EE GCC 3.2.2
bootstrap then differs only in the `$t4`/`$t5` local-allocation tie-break. V78
handles that compiler fingerprint through a private, opt-in `cc1plus` profile:

- the profile is rebuilt from the hash-pinned bootstrap source and build tree;
- it swaps only local-allocation order entries 12 and 13 (`$t4` and `$t5`);
- the canonical compiler, GCC source and canonical `libbackend.a` are never
  modified;
- the runner opts in with GCC's `-B` search prefix and hashes the private
  compiler as part of its cache key.

The resulting object symbol is exactly 584 bytes. All non-relocation bytes
compare equal, all 18 relocations use known precise MIPS masks, and no
instruction byte is broadly ignored. Raw equality is false only because an
unlinked relocatable object cannot contain the target's final relocated
addresses.

The immutable proof is
[`hunt1041-v78-validated-c4bit-1.tsv`](../../analysis/matching/hunt1041-v78-validated-c4bit-1.tsv).
Reproduce it with the private reference and historical EE C++ compiler:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v78-evidence
```

## Current 44-entry frontier

The generated
[`hunt1041-v78-frontier-map-44.tsv`](../../analysis/matching/hunt1041-v78-frontier-map-44.tsv)
covers every remaining non-`MATCHING` manifest row:

| Track | Entries | Change from V77 |
|---|---:|---:|
| Frontend ownership | 26 | unchanged |
| Historical-source deltas | 18 | minus `C4BitPlaneWave` |
| Remaining C4 / `c4emu` packet | 4 | down from 5 |

The remaining C4 rows continue as one translation-unit and state-layout
problem. The packed access findings and the allocation profile are reusable
constraints, but every function still requires a complete zero-byte comparison
before promotion.

Function closure remains only the first whole-program gate. Data and read-only
data placement, constructors, vtables, archive and object order, the linker
script and SJCRUNCH2 packing are still required for a byte-identical final ELF.
