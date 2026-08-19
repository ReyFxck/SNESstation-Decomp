# HUNT500+ V33 — SHA-verified original ELF harvest

- Base checkpoint: **473/1041**
- Final checkpoint: **677/1041 (65.03%)**
- New strict matches: **204**
- Target gate: `verified-original-elf:/storage/emulated/0/SNES_EMU.ELF;input_sha256=4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487;unpacked_sha256=739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b;unpacked-by=tools/sjuncrunch.py`
- The original ELF is unpacked with `tools/sjuncrunch.py`; the unpacked bytes must equal `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` exactly.
- No candidate compilation is performed in V33. Existing cached objects are SHA256-deduplicated before one fixed-point global closure.
- Promotion requires exact next-boundary size, relocation-normalized equality, zero unknown relocations, and conservative identity/layout proof.
- Evidence: `analysis/matching/hunt500plus-v33-validated-204.tsv`
