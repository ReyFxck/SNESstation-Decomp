# V111: window-11 public rodata integration

Stage 3O replaces a large, source-identifiable part of image window 11 with
freshly rebuilt public-source data. It advances the cumulative diagnostic from
644,215 to 620,746 differing bytes without claiming that window 11, the
unpacked image or the packed ELF is exact.

## Integrated evidence

The gate records 49 disjoint sections or slices totaling **33,311 bytes**:

- Snes9x 1.41-1 rodata from CHEATS, DMA, DSP1, SuperFX debug/instruction
  tables, GFX, MEMMAP, PPU, SA-1, SETA, snapshot, sound, SPC700, SPC7110 and
  S-RTC translation units;
- ten zlib 1.1.3 translation-unit sections; and
- selected, previously pinned PS2 runtime, PS2LIB, GCC 3.2.2 libgcc and
  libsupc++ sections.

Every source file is SHA-256-frozen in
`analysis/link_identity/window11_rodata.json`. Complete source sections are
fingerprinted before slicing. For all **1,517 R_MIPS_32 relocations**, the gate
allows the private reference to supply only the final four-byte relocation
result. Every byte outside those relocation fields must already match the
freshly compiled source object, and the final linked slice must match exactly.

Five older fixed fragments covered by the larger source sections are removed
from the linker script. The input aggregate, startup, earlier source/semantic
payloads and all exact windows from Stage 3N are otherwise preserved.

## Measured result

| Metric | Stage 3N | Stage 3O |
|---|---:|---:|
| Whole-image differing bytes | 644,215 | **620,746** |
| Differences removed | — | **23,469** |
| Exact 64 KiB windows | 39/51 | **39/51** |
| Window-11 differing bytes | 25,929 | **2,460** |
| Window 11 exact | No | **No** |

The exact-window roster remains 12–50. Windows 0–11 still differ, the first
remaining difference is still `0x00100114`, and this checkpoint is not a
replacement ELF.

## Public/private boundary

The committed manifest contains source identities, section geometry, hashes,
counts and claims only. It contains no reference bytes or generated payloads.
Private-reference-derived payloads, rebuilt objects, linker scripts and the
diagnostic ELF stay under ignored `build/window11-rodata/`.

The repository-only gate needs neither the reference image nor a compiler:

```bash
make window11-rodata-public-check
```

With a legally obtained reference and the bootstrapped EE GCC 3.2.2 toolchain,
the private reproduction gate recompiles the sources, verifies the relocation
boundary, links the cumulative diagnostic and compares all 51 windows:

```bash
make window11-rodata
```

The next link-identity step is to classify and reconstruct the remaining 2,460
bytes in window 11. Closing that remainder is a separate proof and must not be
inferred from this partial integration.
