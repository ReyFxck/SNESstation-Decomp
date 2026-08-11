# Progress 17 — audited structural closure

Progress 17 closes the validated structural-analysis universe at **1,041 / 1,041
(100.00%) reconstructed**. Matching remains **0.00%**.

This percentage has a deliberately narrow meaning: every entry in the audited
target universe has a committed source reconstruction or complete
address-labelled R5900 structural decompile. It does **not** mean that the
project is a buildable replacement ELF, that placeholder types are final, that
the exact compiler function count is known, or that any function byte-matches
the reference.

## Replacing the raw proxy with an audited universe

Earlier checkpoints divided by every distinct target emitted by the raw JAL
scanner. That was useful as a conservative progress proxy, but the scanner also
walks the post-code portion of the unpacked image and therefore finds data words
whose bit pattern decodes as a JAL instruction.

Progress 17 classifies the entire set:

| Class | Count | Evidence rule |
|---|---:|---|
| Raw distinct JAL targets | 1,137 | `analysis/jal_candidates.csv` |
| Rejected: target outside confirmed code | 258 | target is outside `0x00100000..0x001b07ff` |
| Rejected: only post-code call-site patterns | 34 | target is in code, but the first—and therefore every sorted—hit is in post-code data |
| Validated direct-JAL entries | 845 | raw set minus the 292 rejected patterns |
| Independently mapped non-JAL entries | 196 | entry point, indirect-call targets, recovered internal boundaries and other binary-backed mappings |
| **Validated structural universe** | **1,041** | **845 + 196** |

Equivalently:

```text
1,137 raw JAL targets - 292 rejected data patterns + 196 non-JAL entries
= 1,041 validated structural targets
```

[`analysis/progress17_rejected_jal_candidates.csv`](../analysis/progress17_rejected_jal_candidates.csv)
records every rejected target, first observed word, hit count, classification
and reason. The 292 rejected addresses are disjoint from the final progress
manifests. Every one of the 845 accepted JAL targets is present in them.

## Structural closure pass

Progress 16 contained 992 mapped entries: 967 reconstructed, 24 identified and
one partial. Progress 17 processes 74 targets:

| Source | Count | Result |
|---|---:|---|
| Real code JAL targets absent from Progress 16 | 49 | added as reconstructed |
| Previously mapped identified/partial entries | 25 | promoted after complete structural decompilation |
| **Total closure pass** | **74** | **all retained with per-function evidence** |

The promoted set includes the unpacked entry, frontend/main flow, VRAM and
renderer cache paths, `CMemory_LoadROM`, unzip 0.15 public operations, GCC
unwind functions, C++ personality/runtime functions and VMI RTTI walkers. The
new set closes the remaining core/DSP, renderer/Mode 7, memory, snapshot,
CPU/audio and runtime targets. Historical names are assigned only when target
behavior and source ordering independently support them; otherwise the
`snes_p17_<address>` label remains.

The complete source evidence is committed as
[`analysis/functions/progress17_r5900_pseudocode.c.txt`](../analysis/functions/progress17_r5900_pseudocode.c.txt).
It contains 14,068 function-body lines across 74 marked sections. It is analysis
pseudocode, not a build-ready C translation unit.

## Warning audit

All 74 functions decompiled successfully with the R5900 processor. Seventeen
have no Ghidra warning. The other 57 retain 65 warning occurrences, each stored
verbatim and classified in
[`analysis/progress17_recovered_targets.csv`](../analysis/progress17_recovered_targets.csv):

| Warning class | Occurrences | Interpretation |
|---|---:|---|
| Absolute-global label overlap | 40 | `DAT_*`/`_` symbol boundaries need final project types |
| Compiler-created unreachable block | 14 | dead/control-flow fragments remain visible in the structural output |
| Non-returning subroutine annotation | 8 | call-site control flow ends at a known or inferred non-returning sink |
| Stack-pointer warning | 1 | `_start` changes runtime stack state; final startup source needs assembly-aware cleanup |
| Intentional infinite loop | 1 | the main shutdown/terminal loop is preserved |
| Type propagation did not settle | 1 | `snapshot_Unfreeze @ 0x001728d4` retains unresolved placeholder types |
| **Total** | **65** | |

The snapshot function's control flow, block tags, state-reset sequence and
restore calls agree with the Snes9x 1.41 `Unfreeze` implementation, but the
type-propagation warning is material and its confidence is therefore
`medium-low`. The checkpoint counts recovered structure while making that
remaining typing work explicit.

## Reproducibility fingerprints

| Artifact | SHA-256 |
|---|---|
| Raw 74-function Ghidra output (not distributed) | `e8ee13a63a1eef1dd9dc6a825e161beb4a0625e6e67a98cb59f294ba4e365cff` |
| Committed structural pseudocode | `6bb50e4c28635b490f71b2a124962ce408e58c4bbe7108008a3c731991696876` |
| 74-row recovery manifest | `2918a2194be053bf00407e097a145d9de41634fe3645dc8be1d6b507753a3655` |
| 74-row target manifest | `0f30289c18bf767a3863925ff7f58fa59897199f129366ed8e9b63d30488e259` |
| 292-row rejection manifest | `5263dec508b39829b7477815a44fcca35c4b423a9102fb08fc58c5fe8d7c16e6` |

The decompiler environment is Ghidra 10.4 PUBLIC with Ghidra Emotion Engine:
Reloaded 2.1.10 and language `r5900:LE:32:default`. The exact target and
historical dependency fingerprints remain in
[`DEPENDENCY_VERSIONS.md`](DEPENDENCY_VERSIONS.md).

A second headless decompilation of all 74 targets reproduced the reviewed raw
output byte-for-byte with the same `e8ee13a6...` SHA-256.

## Reproduction

Starting from the Progress 16 commit and a correctly imported
`SNES_EMU.analysis.elf`:

```sh
python3 tools/prepare_progress17.py

analyzeHeadless PROJECT_DIR PROJECT -process SNES_EMU.analysis.elf -noanalysis \
  -scriptPath tools/ghidra -postScript DecompileProgress17.java \
  analysis/progress17_targets.csv /tmp/progress17-ghidra.c

python3 tools/extract_progress17_decomp.py /tmp/progress17-ghidra.c
python3 tools/apply_progress17.py
```

The extraction refuses a raw output whose SHA-256 differs from the reviewed
run. The apply step verifies whole-file and per-function hashes, exact warning
text, target/rejection arithmetic, both progress manifests and all accepted JAL
memberships. It then regenerates the README/SVG and syntax-checks all 88
build-ready recovered C translation units with `-Wall -Wextra -Werror`.

## What 100% leaves to do

The next phase is qualitatively different from target discovery:

- replace absolute `DAT_*` labels and placeholder scalar types with recovered
  structures and named globals;
- migrate structural pseudocode into reviewed, build-ready C/C++ units;
- reproduce the historical EE GCC/binutils/PS2LIB corridor and per-unit flags;
- link a complete replacement ELF; and
- compare relocation-normalized machine code before assigning `MATCHING`.

Until those comparisons exist, the project reports **100.00% reconstructed,
100.00% mapped, and 0.00% matching**.
