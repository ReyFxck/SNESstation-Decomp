# Current bottlenecks

The live counts are generated in
[`status/PROJECT_STATUS.generated.md`](status/PROJECT_STATUS.generated.md).
The formal manifest has **1,041/1,041 strict matches**. V81 added raw
byte-exact assembly-reconstruction evidence for the final 20 complete spans,
totaling 71,384 bytes, and generated an audited zero-entry frontier map.

## Function frontier — closed

- All audited function rows now have complete-boundary, immutable matching
  evidence. The checked zero-entry map is
  [`../analysis/matching/hunt1041-v81-frontier-map-0.tsv`](../analysis/matching/hunt1041-v81-frontier-map-0.tsv).
- The remaining work is no longer function discovery or per-function matching;
  it is coherent source ownership and whole-program identity.

## Build-ready source — closed

The frozen Stage-2 gate compiles all 97 source units with EE GCC 3.2.2, selects
96 canonical objects, retains one explicit CDVD alternate, rejects
duplicate/common definitions and records all defined/external ownership. See
[`status/BUILD_READY_SOURCE_TREE.md`](status/BUILD_READY_SOURCE_TREE.md).

The unresolved contracts now belong to the program-data, archive-identity and
link-identity gates; they are no longer an ambiguous source-tree backlog.

## Unpacked layout oracle — closed

V82 freezes the private target as one initialized SJCRUNCH2 section, thirteen
decompressed blocks and fifty-one 64 KiB hash windows. `make
compare-unpacked` now reports the exact first divergent image offset/load
address for every rebuilt candidate. See
[`status/V82_UNPACKED_LAYOUT_ORACLE.md`](status/V82_UNPACKED_LAYOUT_ORACLE.md).

This is a measurement gate, not a claim that the bytes have been rebuilt.

## Whole-program identity

Even after every function is `MATCHING`, an identical ELF still requires:

- exact old PS2LIB/PS2SDK-era archive revisions and selected members;
- exact application linker script, section placement and object/library order;
- data/rodata/string-pool/vtable layout and relocations;
- exact binutils patch level;
- reproducible SJCRUNCH2/LZO packing.

These gates are intentionally enforced by the blocked `make elf` target. The
full sequence is documented in [`REPRODUCTION.md`](REPRODUCTION.md).
