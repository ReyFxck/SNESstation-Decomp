# Whole-image code proof

The code-window gate now reconstructs every initialized byte of the unpacked
image from historical/recovered objects, committed public instruction listings
and explicitly labelled assembly proofs. All **51/51 64 KiB windows** match,
with **0 differing bytes**.

This remains an integration diagnostic, not the final replacement ELF.

## Evidence boundary

- Historical/recovered objects and the earlier exact-assembly proofs
  are selected from the frozen matching ledgers. Every selected byte is
  accepted only when all non-relocation instruction bits agree.
- MIPS relocation masks are precise per relocation type. The private oracle
  supplies only relocation-controlled result bits and final verification.
- Schedules not reproduced by the pinned old compiler are retained in
  `matching/candidates/stage3p_code_residual_exact.S` as clearly labelled,
  explicit EE instructions. The file has no binary include.
- Generated 64 KiB payloads and the privately checked linked diagnostic stay
  below ignored `build/` storage. No private target payload is committed.

## Result

| Metric | Result |
|---|---:|
| Exact 64 KiB windows | **51/51** |
| Mismatching windows | **0** |
| Whole-image differing bytes | **0** |
| Differences removed from the prior checkpoint | **618,286** |
| Integrated code evidence | **720,620 bytes** |
| Historical/recovered object bytes | **525,460** |
| Existing public instruction listings | **73,192** |
| Explicit scheduling residual | **36,340** |

The exact-window roster is 0–50. The reconstructed unpacked image SHA-256 is
`739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`.

## Gates

```bash
make code-windows-public-check
make code-windows
make reproduce-check
```

The public gate validates hashes, geometry, provenance and claim boundaries
without the private image. The private gate rebuilds the evidence objects,
links the diagnostic and compares every byte and all 51 hash windows.

## What remains

Prove the final section/archive/link identity, then reproduce SJCRUNCH2
packing. The current diagnostic must not be distributed as a replacement ELF.
