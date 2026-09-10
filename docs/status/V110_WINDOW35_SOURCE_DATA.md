# V110 — Window-35 source-data integration

Stage 3N rebuilds the remaining historical Snes9x data and GCC 3.2.2 unwind
corridor in image window 35. The cumulative diagnostic advances from 38/51 to
39/51 exact windows.

## Integrated corridor

The contiguous range `0x00335284–0x0033ce78` is reconstructed from seven
freshly compiled Snes9x 1.41-1 `.data` slices and seven explicit semantic CFI
groups:

| Component | Evidence | Bytes |
|---|---|---:|
| 2xSaI, APU, C4, C4 emulator, CPU execution/opcode and color tables | Pinned historical source, raw hashes and relocation masks | 29,516 |
| C4, cheats, clipping, CPU and DMA unwind records | 47 explicit FDE descriptions | 2,216 |

The source rebuild contains 1,429 `R_MIPS_32` sites. Every byte outside those
sites must match the reference before the gate accepts the target-linked
pointer results. The semantic records are assembled from checked function
extents and DWARF operations; they are not copied from the target image.

Twenty-four older fixed sections inside the corridor are absorbed. Their
source-visible symbols remain absolute linker aliases, avoiding duplicate
storage.

Generated relocation payloads and all private comparison products remain
below ignored `build/`. The tracked manifest contains only source identities,
geometry, hashes, semantic tuples, metrics and explicit non-completion claims.

## Result

- Exact windows: **39/51**.
- Newly exact: **35**.
- Remaining windows: **12** (`0–11`).
- Differing bytes: **644,215**.
- Differences removed: **17,218**.
- Complete replacement ELF: **not yet**.

Public contract only:

```sh
make window35-data-public-check
```

Private rebuild and byte gate:

```sh
make window35-data
```

The remaining differences are confined to the application/code windows
`0–11`. Exact implementation selection, historical link composition and
packed-ELF reproduction remain open.
