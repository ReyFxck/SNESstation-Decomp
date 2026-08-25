# Current bottlenecks

The live counts are generated in
[`status/PROJECT_STATUS.generated.md`](status/PROJECT_STATUS.generated.md).
The formal manifest currently has 1,021 strict matches. V80 added raw
byte-exact assembly-reconstruction evidence for 23 complete non-C4 spans
totaling 14,244 bytes and generated a complete map for the 20-entry frontier.

## Function frontier

- Close the remaining 20 function-frontier entries with exact boundaries and
  immutable evidence.
- Prefer source-lineage and ABI corrections over unbounded compiler-flag
  farming.
- Work the remaining frontier as two coordinated tracks: 8 frontend-ownership
  entries and 12 historical-source deltas. The exact address-level queue is
  [`../analysis/matching/hunt1041-v80-frontier-map-20.tsv`](../analysis/matching/hunt1041-v80-frontier-map-20.tsv).
- Continue the three remaining `c4emu` rows as one historical translation-unit
  problem. V76 through V79 have pinned the PS2 packed 16/24-bit access forms,
  the 32-bit `memset` declaration and narrow EE allocation constraints used by
  `C4SprDisintegrate`, `C4DrawWireFrame`, `C4BitPlaneWave`, and `C4ConvOAM`.

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
