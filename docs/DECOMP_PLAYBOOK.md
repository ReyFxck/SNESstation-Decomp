# SNES Station decompilation playbook

This file is the persistent methodology for continuing SNESstation-Decomp in a
new ChatGPT session or on another machine.

## Non-negotiable evidence rules

1. Search historical source first. Do not begin from invented pseudocode when
   a contemporaneous library/application source snapshot can be found.
2. Freeze every historical reference by immutable commit, release asset, hash,
   or exact target fingerprint.
3. Distinguish:
   - STRUCTURAL: boundaries/symbols/control flow identified.
   - RECOVERED: readable behavior/source reconstructed.
   - MATCHING: compiled candidate matches the target listing after relocation
     normalization.
   - STRICT: every function in the selected listing gate matches.
   - FORMAL ELF: the legally obtained original ELF itself is the comparison
     target after final linking/layout.
4. Same function size is not a MATCH.
5. Compiler acceptance is not a MATCH.
6. A historical-looking source file is not a MATCH.
7. Never promote WIP to MATCHING until the byte comparator says so.

## Matching workflow

1. Identify the exact target corridor and function boundaries.
2. Find the closest historical source and record provenance.
3. Identify the compiler family and, where possible, its exact build date.
4. Compile isolated candidates with the historically plausible compiler.
5. Force a fresh object before every experiment; stale `.o` files invalidate
   compiler-fingerprint conclusions.
6. Compare target vs candidate with `tools/compare_elf_functions.py`.
7. Mask only relocation-controlled bytes. All other bytes must agree.
8. When every row matches, run the strict gate.
9. Commit the source, manifest, report, provenance and compiler contract.
10. Only then move to the next corridor.

## Compiler fingerprint rule

Do not mutilate recovered C merely to compensate for the wrong compiler.
When source structure and function sizes line up but register allocation,
prologue order or scheduling differs, test historical compiler builds first.

For the January 24, 2004 SNES Station target, maintain this matrix:

- EE GCC 3.2 Beta2 / 2003-02-10: historical control candidate.
- EE GCC 3.2 / 2003-09-26: strongest pre-target candidate currently preserved.
- GCC 3.2.2 BETA 3 / 2004-02-14: reproducible public PS2DEV source recipe,
  useful and already proven for several gates, but newer than the target.

The 2003-09-26 build is a candidate, not a proven global answer. Bytes decide.

## User/assistant workflow

Prefer doing compiler/source permutations in the analysis environment. Do not
turn the user into a manual CI runner for a long sequence of speculative
variants. Ask for a device-side run only when the required compiler or target
artifact genuinely cannot be executed/accessed elsewhere.

Deliver consolidated checkpoints:
- MATCHED/closed work;
- WIP separately;
- exact commands for commit/push when needed.

## Legal/source hygiene

Do not commit or redistribute the original SNES_EMU.ELF or carved proprietary
IRX blobs. Keep hashes, sizes and offsets in the repo so a legally obtained
local copy can be verified.

Open-source historical compiler/library source and explicitly redistributable
archives may be pinned for reproducibility subject to their licenses.
