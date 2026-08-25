# V80: 23 raw-exact quick wins

> Frozen checkpoint. V81 subsequently promoted all 20 remaining functions;
> use the zero-entry frontier linked from the generated project status.

V80 promotes the 23 smallest non-C4 spans from the frozen V79 frontier. The
formal checkpoint moves from **998/1,041 (95.87%)** to
**1,021/1,041 (98.08%)**, leaving **20** audited entries.

## Complete function gate

Every row uses its exact next-manifest boundary. Together the candidates cover
**14,244 bytes** and compare raw-equal to the hash-pinned unpacked ELF without
relocation masks.

| Address | Manifest name | Bytes | Packet | Result |
|---:|---|---:|---|---|
| `0x00100114` | `snes_p17_00100114` | 632 | frontend UI | `MATCH` |
| `0x0010038c` | `snes_p16_0010038c` | 548 | frontend UI | `MATCH` |
| `0x001005ec` | `snes_p16_001005ec` | 752 | frontend UI | `MATCH` |
| `0x001019a8` | `snes_p16_001019a8` | 348 | frontend UI | `MATCH` |
| `0x00101b64` | `snes_p16_00101b64` | 808 | frontend UI | `MATCH` |
| `0x00103b34` | `snes_p16_00103b34` | 328 | frontend UI | `MATCH` |
| `0x00103dd4` | `snes_p16_00103dd4` | 1,004 | frontend UI | `MATCH` |
| `0x00104234` | `snes_p16_00104234` | 292 | frontend UI | `MATCH` |
| `0x00104358` | `snes_p11_00104358` | 192 | frontend UI | `MATCH` |
| `0x00104a54` | `mtapPortClose_00104a54` | 360 | frontend pad | `MATCH` |
| `0x00104bbc` | `mtapGetConnection_00104bbc` | 652 | frontend pad | `MATCH` |
| `0x00105898` | `snes_p16_00105898` | 308 | frontend lifecycle | `MATCH` |
| `0x00105ae8` | `snes_p16_00105ae8` | 464 | frontend lifecycle | `MATCH` |
| `0x00105e48` | `snes_p17_00105e48` | 524 | frontend lifecycle | `MATCH` |
| `0x001060dc` | `snes_p16_001060dc` | 996 | frontend lifecycle | `MATCH` |
| `0x001064c0` | `snes_p16_001064c0` | 868 | frontend lifecycle | `MATCH` |
| `0x0010689c` | `snes_p16_0010689c` | 816 | frontend lifecycle | `MATCH` |
| `0x00107358` | `snes_p16_00107358` | 544 | frontend lifecycle | `MATCH` |
| `0x0012cbd8` | `snes_p16_0012cbd8` | 576 | DSP1 float | `MATCH` |
| `0x0012d05c` | `snes_p17_0012d05c` | 728 | DSP1 float | `MATCH` |
| `0x00151074` | `CMemory_Init` | 700 | memory PS2 | `MATCH` |
| `0x001584d0` | `CMemory_CheckForIPSPatch` | 1,188 | memory PS2 | `MATCH` |
| `0x00177a84` | `snes_p16_00177a84` | 616 | Soundux PS2 | `MATCH` |

The existing behavioral C/C++ lifts remain the readable models. The committed
[`hunt1041_v80_quickwins_exact.S`](../../matching/candidates/hunt1041_v80_quickwins_exact.S)
candidate is explicitly labelled as target-authoritative matching evidence,
not as the original authors' source. It contains 3,561 explicit EE instruction
words and no `.incbin`.

The immutable proof is
[`hunt1041-v80-validated-quickwins-23.tsv`](../../analysis/matching/hunt1041-v80-validated-quickwins-23.tsv).
Reproduce it with the private reference and historical EE assembler:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v80-evidence
```

## Frozen 20-entry frontier

The generated
[`hunt1041-v80-frontier-map-20.tsv`](../../analysis/matching/hunt1041-v80-frontier-map-20.tsv)
covers every remaining non-`MATCHING` manifest row:

| Track | Entries | Remaining packets |
|---|---:|---|
| Frontend ownership | 8 | frontend UI 6; lifecycle 2 |
| Historical-source deltas | 12 | C4 3; DSP1 1; snapshot 1; Soundux 4; SPC7110 3 |

The three deferred C4 rows remain `C4DoScaleRotate`, `C4TransformLines`, and
`S9xSetC4`. Function closure is still only the first whole-program gate: exact
data and read-only-data placement, constructors, vtables, archive and object
order, the linker script, and SJCRUNCH2 packing remain required for a
byte-identical final ELF.
