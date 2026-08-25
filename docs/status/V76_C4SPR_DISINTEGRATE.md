# V76: exact C4SprDisintegrate proof

V76 promotes `C4SprDisintegrate` at `0x0010d4f0` with strict
historical-source evidence. The formal checkpoint moves from **994/1041** to
**995/1041 (95.58%)**, leaving **46** audited entries.

## Complete function gate

| Address | Historical identity | Bytes | Boundary | Result |
|---:|---|---:|---|---|
| `0x0010d4f0` | `C4SprDisintegrate` | 580 | exact next-manifest boundary at `0x0010d734` | `MATCH` |

The runner begins with the hash-pinned official Snes9x 1.41-1 `c4emu.cpp` and
retains its disintegration algorithm. The target-specific PS2 adaptation
proven by the ELF consists of:

- packed 32-bit unaligned loads followed by a 16-bit mask for `READ_WORD`;
- a local C4 RAM base held in EE register `$a3`;
- the historical 32-bit `memset` length declaration, avoiding a host-style
  64-bit size shift;
- one non-volatile inline `sll` constraint for the final signed word, which
  preserves EE GCC 3.2.2's target allocation `$a0 -> $v0 -> $s4`.

That inline fragment contains one instruction only. The prologue, remaining
prefix, nested pixel loops, epilogue and all control flow are still generated
from the C++ implementation. The resulting object symbol is exactly 580 bytes;
all non-relocation bytes compare equal, all 11 relocations use known precise
MIPS masks, and no instruction byte is broadly ignored.

The immutable proof is
[`hunt1041-v76-validated-c4spr-1.tsv`](../../analysis/matching/hunt1041-v76-validated-c4spr-1.tsv).
Reproduce it with the private reference and historical EE C++ compiler:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v76-evidence
```

## Current 46-entry frontier

The generated
[`hunt1041-v76-frontier-map-46.tsv`](../../analysis/matching/hunt1041-v76-frontier-map-46.tsv)
covers every remaining non-`MATCHING` manifest row:

| Track | Entries | Change from V75 |
|---|---:|---:|
| Frontend ownership | 26 | unchanged |
| Historical-source deltas | 20 | minus `C4SprDisintegrate` |
| Remaining C4 / `c4emu` packet | 6 | down from 7 |

The remaining C4 rows should continue to be treated as one translation-unit
and state-layout problem. The V76 packed access and ABI findings are reusable
constraints, but each function still requires its own complete zero-byte
comparison before promotion.

Function closure remains only the first whole-program gate. Data and read-only
data placement, constructors, vtables, archive and object order, the linker
script and SJCRUNCH2 packing are still required for a byte-identical final ELF.
