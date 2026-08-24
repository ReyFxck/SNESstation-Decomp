# HUNT1000+ V34 — complete-target exact cluster miner

- Base checkpoint: **677/1041**
- Final checkpoint: **693/1041 (66.57%)**
- New strict matches: **16**
- Target gate: `verified-original-elf:/storage/emulated/0/SNES_EMU.ELF;input_sha256=4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487;unpacked_sha256=739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b;reused-v33-verified-unpacked`
- V34 compiles no candidates. It reuses the V33 SHA-deduplicated cache.
- New proof modes are limited to deterministic recovered-name aliases and coherent multi-function exact object placement.
- Cluster promotion never uses target bytes as candidate source and still requires exact next-boundary size, relocation-normalized equality, and zero unknown relocations for every promoted function.
- The progress CSV has 1041 address rows, while the standard next-address boundary loader yields 1040 bounded spans; V34 does not invent a size for the final row.
- Evidence: `analysis/matching/hunt1000plus-v34-validated-16.tsv`
