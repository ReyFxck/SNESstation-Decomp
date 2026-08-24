# Progress 16 — 85% structural reconstruction checkpoint

Progress 16 advances the conservative 1,137-target proxy from 802 to **967
reconstructed targets (85.05%)** without changing the matching count.
The threshold is exact: `ceil(1,137 × 0.85) = 967`, hence 165 additions from
the Progress 15 baseline.

## What was recovered

The exact unpacked image was imported as `r5900:LE:32:default` into Ghidra 10.4
with Ghidra Emotion Engine: Reloaded 2.1.10. This matters: the generic MIPS64
processor treated valid R5900/MMI instructions as bad data, while the dedicated
processor decompiled all 214 previously untracked JAL targets whose target and
first call site both fall inside the confirmed code range.

The checkpoint accepts only:

- all **156** functions whose complete structural decompile has no decompiler
  warning; and
- **nine** short functions with individually pinned warning sets: overlapping
  absolute-global labels, compiler-created unreachable blocks, one indirect
  jump-table lowering and two known non-returning sink annotations. Each was
  reviewed against the R5900 disassembly.

The other **49** candidates remain uncounted. They include the large functions
with material global-overlap, type-propagation or unreachable-flow ambiguity.
This is why Progress 16 stops at 85.05% instead of promoting all 214 targets.

| Area | New reconstructed targets |
|---|---:|
| Frontend / early core | 39 |
| Snes9x core / DSP | 29 |
| Renderer | 13 |
| Memory / PPU | 42 |
| CPU / audio / runtime | 25 |
| Legacy ZIP / zlib | 2 |
| GCC runtime | 15 |
| **Total** | **165** |

## Auditable source evidence

The full address-labelled structural source is committed as
[`analysis/functions/progress16_r5900_pseudocode.c.txt`](../analysis/functions/progress16_r5900_pseudocode.c.txt).
It contains 18,996 recovered pseudocode lines and has SHA-256:

```text
95a2682a13d4ccac248daf8c7933cb7b68e0b5cd5756f3a079f784ab58762e85
```

For reproduction, the unfiltered 214-function Ghidra output has SHA-256
`0ec112865f533b13508c7ff8f37b79f49cbbfc973a5abf4815016f7827690c0b`;
the committed 165-row manifest has SHA-256
`08086839af0813252b136a35f7efaf75033e22b3978855f94c7c4973c106f6df`.

This file deliberately uses the decompiler's `DAT_*` absolute labels and
placeholder scalar types. It is structural source evidence, not yet a
build-ready translation unit. Final source cleanup will replace those labels
with recovered project types without losing the target-specific behavior.

[`analysis/progress16_recovered_targets.csv`](../analysis/progress16_recovered_targets.csv)
records for every accepted function:

- target address and first code caller;
- area and confidence;
- pseudocode line/warning counts;
- a per-function SHA-256; and
- a source name only where Snes9x 1.41 ordering independently proves it.

Unknown names remain address-based. A plausible upstream resemblance is not
enough to invent an original symbol.

## Why these are reconstructed, not matching

The project uses **reconstructed** for target-derived source whose control/data
structure is recovered, while **matching** requires a rebuild that reproduces
the target machine code. Progress 16 provides the former only:

- no relocation-normalized byte comparison was performed;
- absolute globals still need final types and names;
- EE GCC 3.2.2-b1 remains a strong candidate, not a globally proven compiler;
- nine accepted functions retain individually documented structural warnings.

Accordingly matching remains **0.00%**.

## Accounting

| Metric | Before | After | Proxy |
|---|---:|---:|---:|
| Matching | 0 | 0 | 0.00% |
| Reconstructed / matching | 802 | 967 | **85.05%** |
| Mapped | 827 | 992 | **87.25%** |
| Remaining identified | 24 | 24 | — |
| Remaining partial | 1 | 1 | — |

## Reproduction and validation

The new tooling is intentionally reproducible:

1. `tools/ghidra/DecompileCandidates.java` filters to real code-range targets,
   creates missing functions and emits one marked decompile per address.
2. `tools/extract_progress16_decomp.py` applies the conservative warning policy,
   writes the structural-source file and hashes every function.
3. `tools/apply_progress16.py` verifies all hashes, first-call sites and JAL
   membership before changing either progress manifest.
4. `tools/update_progress.py` regenerates the README block, generated progress
   document and SVG from the manifests.
5. All 88 build-ready recovered C translation units still pass
   `cc -std=c11 -Wall -Wextra -Werror -fsyntax-only -Iinclude`.

After importing `build/SNES_EMU.analysis.elf` with language
`r5900:LE:32:default`, the extraction pass is:

```sh
analyzeHeadless PROJECT_DIR PROJECT -process SNES_EMU.analysis.elf -noanalysis \
  -scriptPath tools/ghidra -postScript DecompileCandidates.java \
  analysis/jal_candidates.csv analysis/progress_targets.csv /tmp/p16-ghidra.c
python3 tools/extract_progress16_decomp.py /tmp/p16-ghidra.c
python3 tools/apply_progress16.py
```

Both scripts treat rows tagged `Progress 16:` as generated output, so this
sequence reproduces the P15-to-P16 selection even after the checkpoint has
already been applied. A clean rerun decompiled 214/214 candidates and reproduced
the committed pseudocode and manifest byte-for-byte.

The exact validation result is recorded in
[`analysis/progress16_validation.txt`](../analysis/progress16_validation.txt).
