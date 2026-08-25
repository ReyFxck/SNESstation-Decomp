# V79: exact C4ConvOAM proof

V79 promotes `C4ConvOAM` at `0x0010c340` with a raw byte-exact EE assembly
reconstruction and a retained readable Snes9x 1.41-1 C model. The formal
checkpoint moves from **997/1041** to **998/1041 (95.87%)**, leaving **43**
audited entries.

## Complete function gate

| Address | Historical identity | Bytes | Boundary | Result |
|---:|---|---:|---|---|
| `0x0010c340` | `C4ConvOAM` | 952 | exact next-manifest boundary at `0x0010c6f8` | `MATCH` |

The historical Snes9x function supplies the readable behavioral model. A
portable EE GCC 3.2.2 reconstruction reached the exact 952-byte boundary and
target register layout, but retained narrow instruction-scheduling drift from
the unavailable original backend. V79 therefore uses the repository's existing
exact-assembly evidence policy:

- [`c4convoam_exact.S`](../../matching/candidates/c4convoam_exact.S) is clearly
  labelled as a matching reconstruction, not as Hiryu's original source;
- the candidate contains the complete 238-instruction function, not an
  `.incbin` of the private reference;
- its symbol is exactly 952 bytes and compares raw-equal to the hash-pinned
  target span;
- no relocation mask or ignored instruction byte is required.

The immutable proof is
[`hunt1041-v79-validated-c4conv-1.tsv`](../../analysis/matching/hunt1041-v79-validated-c4conv-1.tsv).
Reproduce it with the private reference and historical EE assembler:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v79-evidence
```

## Current 43-entry frontier

The generated
[`hunt1041-v79-frontier-map-43.tsv`](../../analysis/matching/hunt1041-v79-frontier-map-43.tsv)
covers every remaining non-`MATCHING` manifest row:

| Track | Entries | Change from V78 |
|---|---:|---:|
| Frontend ownership | 26 | unchanged |
| Historical-source deltas | 17 | minus `C4ConvOAM` |
| Remaining C4 / `c4emu` packet | 3 | down from 4 |

The remaining C4 rows are `C4DoScaleRotate`, `C4TransformLines`, and `S9xSetC4`.
Function closure remains only the first whole-program gate: exact data and
read-only-data placement, constructors, vtables, archive and object order, the
linker script, and SJCRUNCH2 packing are still required for a byte-identical
final ELF.
