# Current bottlenecks

The live counts are generated in
[`status/PROJECT_STATUS.generated.md`](status/PROJECT_STATUS.generated.md).
The formal manifest currently has 979 strict matches; six recovered V53 exact
results still need their compiler-side promotion evidence regenerated.

## Function frontier

- Regenerate the six V53 compiler objects, relocation reports and cache keys.
- Close the remaining 56 working-frontier entries with exact boundaries and
  immutable evidence.
- Prefer source-lineage and ABI corrections over unbounded compiler-flag
  farming.

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
