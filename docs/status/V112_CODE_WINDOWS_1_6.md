# V112: proved-source code windows 1–6

Stage 3P replaces image windows 1 through 6 with a byte-exact reconstruction
assembled from previously proved historical/recovered objects and explicit
assembly proofs. The integrated
diagnostic advances from 39/51 to **45/51 exact 64 KiB windows** and reduces
the whole-image difference from 620,746 to **263,765 bytes**.

This remains an integration diagnostic, not the final replacement ELF.

## Evidence boundary

- Historical/recovered objects and the existing V80/V81 exact-assembly proofs
  are selected from the frozen matching ledgers. Every selected byte is
  accepted only when all non-relocation instruction bits agree.
- MIPS relocation masks are precise per relocation type. The private oracle
  supplies only relocation-controlled result bits and final verification.
- Ten schedules not reproduced by the pinned old compiler are retained in
  `matching/candidates/stage3p_code_residual_exact.S` as clearly labelled,
  explicit EE instructions. The file has no binary include.
- Generated 64 KiB payloads and the privately checked linked diagnostic stay
  below ignored `build/` storage. No private target payload is committed.

## Result

| Metric | Stage 3O | Stage 3P |
|---|---:|---:|
| Exact 64 KiB windows | 39/51 | **45/51** |
| Mismatching windows | 12 | **6** |
| Whole-image differing bytes | 620,746 | **263,765** |
| Differences removed | — | **356,981** |
| Total proved code bytes | — | **393,216** |
| Historical/recovered object bytes | — | **381,272** |
| Existing V80/V81 exact assembly | — | **4,856** |
| New explicit scheduling residual | — | **7,088** |

The exact-window roster is 1–6 and 12–50. Windows 0 and 7–11 remain open.
The first whole-image difference remains `0x00100114` because window 0 is not
part of this tranche.

## Gates

```bash
make code-windows-public-check
make code-windows
make reproduce-check
```

The public gate validates hashes, geometry, provenance and claim boundaries
without the private image. The private gate rebuilds the evidence objects,
links the diagnostic, checks all six windows byte for byte and compares all 51
hash windows.

## What remains

Close window 0, windows 7–11 (including the 2,460-byte window-11 residual),
then prove the final section/archive/link identity and reproduce SJCRUNCH2
packing. The current ELF must not be distributed as a replacement binary.
