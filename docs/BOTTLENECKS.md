# Current bottlenecks

The live counts are generated in
[`status/PROJECT_STATUS.generated.md`](status/PROJECT_STATUS.generated.md).
The formal manifest currently has 995 strict matches. V76 added strict
compiler-side evidence for the complete 580-byte `C4SprDisintegrate` function
and generated a complete map for the 46-entry frontier.

## Function frontier

- Close the remaining 46 function-frontier entries with exact boundaries and
  immutable evidence.
- Prefer source-lineage and ABI corrections over unbounded compiler-flag
  farming.
- Work the remaining frontier as two coordinated tracks: 26 frontend-ownership
  entries and 20 historical-source deltas. The exact address-level queue is
  [`../analysis/matching/hunt1041-v76-frontier-map-46.tsv`](../analysis/matching/hunt1041-v76-frontier-map-46.tsv).
- Continue the six remaining `c4emu` rows as one historical translation-unit
  problem. V76 has now pinned the PS2 unaligned-word form, 32-bit `memset`
  declaration and the narrow EE register-allocation constraint used by
  `C4SprDisintegrate`.

## Build-ready source

All audited entries have a behavioral/source model, but that does not yet prove
coherent historical translation units. The main remaining work is:

- freeze global ownership and exact PS2-width types;
- separate structural-lift corridors into proven object boundaries;
- reproduce visibility, common/BSS behavior, inline decisions and C++ ABI
  ownership;
- distinguish readable recovered source from exact assembly-only evidence.

## Whole-program identity

Even after every function is `MATCHING`, an identical ELF still requires:

- exact old PS2LIB/PS2SDK-era archive revisions and selected members;
- exact application linker script, section placement and object/library order;
- data/rodata/string-pool/vtable layout and relocations;
- exact binutils patch level;
- reproducible SJCRUNCH2/LZO packing.

These gates are intentionally enforced by the blocked `make elf` target. The
full sequence is documented in [`REPRODUCTION.md`](REPRODUCTION.md).
