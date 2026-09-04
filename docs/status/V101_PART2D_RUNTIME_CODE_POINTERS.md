# V101 Part 2D — runtime code-pointer refactors

Four former Stage-3F storage blockers are executable addresses selected at
runtime, not data objects:

- LAB_0012f8a8 -> PTR_FUN_00341658
- LAB_0012fb78 -> PTR_FUN_0034165c
- LAB_00170194 -> PTR_FUN_003fa628
- LAB_00170138 -> PTR_FUN_003fa62c

Private proof freezes exact writer instructions inside already-MATCHING
CMemory_ApplyROMFixes and CMemory_InitROM.  The 0x00170138 path explicitly
proves its branch-delay-slot control flow back to the shared pointer-store
block.

These contracts are closed as RUNTIME_CODE_POINTER_REFACTOR.  They receive no
backing section, no fabricated object extent, and no guessed canonical source
symbol.

Expected Stage-3F:
- 1209 SECTION_BACKED_ADDRESS
- 29 ROM-offset refactors
- 10 previously proved code-pointer source aliases
- 4 runtime code-pointer refactors
- 13 NO_PROVED_BACKING
- 1265 total contracts

Replacement ELF identity remains unproved.
