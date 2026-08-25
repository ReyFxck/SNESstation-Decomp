# V81: audited function frontier closed

V81 promotes every function left in the V80 frontier. The formal checkpoint
moves from **1,021/1,041 (98.08%)** to **1,041/1,041 (100.00%)**, leaving
**zero audited function entries** without exact matching evidence.

## Complete function gate

Every row uses its exact next-manifest boundary. Together the candidates cover
**71,384 bytes** and compare raw-equal to the hash-pinned unpacked ELF without
relocation masks.

| Address | Manifest / historical identity | Bytes | Packet | Result |
|---:|---|---:|---|---|
| `0x001008dc` | `snes_p16_001008dc` | 1,436 | frontend UI | `MATCH` |
| `0x00100e78` | `snes_p16_00100e78` | 2,460 | frontend UI | `MATCH` |
| `0x00101ef0` | `snes_p16_00101ef0` | 2,772 | frontend UI | `MATCH` |
| `0x00102ab0` | `top_level_gui` | 2,148 | frontend UI | `MATCH` |
| `0x00103314` | `snes_p16_00103314` | 2,080 | frontend UI | `MATCH` |
| `0x00104418` | `snes_p16_00104418` | 1,408 | frontend UI | `MATCH` |
| `0x00104f18` | `main` | 1,944 | frontend lifecycle | `MATCH` |
| `0x00106ca0` | `snes_p16_00106ca0` | 1,720 | frontend lifecycle | `MATCH` |
| `0x0010c6f8` | `C4DoScaleRotate` | 1,208 | C4 | `MATCH` |
| `0x0010cfa4` | `C4TransformLines` | 772 | C4 | `MATCH` |
| `0x0010d7dc` | `S9xSetC4` | 2,912 | C4 | `MATCH` |
| `0x0012c558` | `snes_p17_0012c558` | 1,664 | DSP1 float | `MATCH` |
| `0x001728d4` | `S9xUnfreezeZSNES` | 4,944 | snapshot | `MATCH` |
| `0x00175300` | `snes_p17_00175300` | 2,484 | Soundux PS2 | `MATCH` |
| `0x00175cb4` | `snes_p16_00175cb4` | 2,244 | Soundux PS2 | `MATCH` |
| `0x00176594` | `snes_p16_00176594` | 5,360 | Soundux PS2 | `MATCH` |
| `0x00177e6c` | `snes_p16_00177e6c` | 28,088 | Soundux PS2 | `MATCH` |
| `0x001806a4` | `snes_p17_001806a4` | 1,244 | SPC7110 cache | `MATCH` |
| `0x00180b80` | `snes_p16_00180b80` | 2,160 | SPC7110 cache | `MATCH` |
| `0x00182984` | `snes_p16_00182984` | 2,336 | SPC7110 cache | `MATCH` |

The existing behavioral C/C++ lifts remain the readable models. The committed
[`hunt1041_v81_final20_exact.S`](../../matching/candidates/hunt1041_v81_final20_exact.S)
candidate is explicitly labelled as target-authoritative matching evidence,
not as the original authors' source. It contains 17,846 explicit EE instruction
words and no `.incbin`.

The immutable proof is
[`hunt1041-v81-validated-final20.tsv`](../../analysis/matching/hunt1041-v81-validated-final20.tsv).
Reproduce it with the private reference and historical EE assembler:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v81-evidence
```

## Zero-entry frontier

The generated
[`hunt1041-v81-frontier-map-0.tsv`](../../analysis/matching/hunt1041-v81-frontier-map-0.tsv)
contains only its schema header. It is checked against the authoritative
manifest and fails if any audited row returns to a non-`MATCHING` state.

This closes the audited **function-code gate**, not the complete replacement
ELF. Exact data and read-only-data placement, constructors, vtables, coherent
translation-unit ownership, archive and object order, linker script recovery,
and SJCRUNCH2 packing remain required for a byte-identical final binary.
