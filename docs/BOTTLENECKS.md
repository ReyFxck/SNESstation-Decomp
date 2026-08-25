# Current bottlenecks

The live counts are generated in
[`status/PROJECT_STATUS.generated.md`](status/PROJECT_STATUS.generated.md).
The formal manifest currently has 996 strict matches. V77 added strict
compiler-side evidence for the complete 472-byte `C4DrawWireFrame` function
and generated a complete map for the 45-entry frontier.

## Function frontier

- Close the remaining 45 function-frontier entries with exact boundaries and
  immutable evidence.
- Prefer source-lineage and ABI corrections over unbounded compiler-flag
  farming.
- Work the remaining frontier as two coordinated tracks: 26 frontend-ownership
  entries and 19 historical-source deltas. The exact address-level queue is
  [`../analysis/matching/hunt1041-v77-frontier-map-45.tsv`](../analysis/matching/hunt1041-v77-frontier-map-45.tsv).
- Continue the five remaining `c4emu` rows as one historical translation-unit
  problem. V76 and V77 have now pinned the PS2 packed 16/24-bit access forms,
  the 32-bit `memset` declaration and the narrow EE allocation constraints
  used by `C4SprDisintegrate` and `C4DrawWireFrame`.

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
