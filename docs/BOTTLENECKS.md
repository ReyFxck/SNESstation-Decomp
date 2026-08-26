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

## Source-address aliases — reviewed tranche closed

V84 proves 323/337 address-shaped external names against 307 canonical global
text definitions and applies them with zero-byte linker aliases. The
alias-resolved aggregate retains 1,598 unresolved contracts, down from the
frozen Stage-2 count of 1,921, while every allocated section remains
byte-identical. See
[`status/V84_REVIEWED_SOURCE_ALIASES.md`](status/V84_REVIEWED_SOURCE_ALIASES.md).

The 14 deliberately blocked rows split into seven historical archive-member
entries, one source helper that is not equivalent to the complete historical
function and six addresses outside the audited target manifest. Those need
archive/boundary proof, not raw-address definitions.

## Zero-byte link contracts — full frontier frozen

V85 classifies every one of the 1,598 externals in the V84 aggregate. It
assigns 1,274 address-qualified program-data names their frozen absolute value
and binds 63 uniquely identified call contracts to existing recovered global
text. The partial link therefore reaches **1,598 -> 261 unresolved externals**
while every allocated section fingerprint remains identical. See
[`status/V85_ZERO_BYTE_LINK_FRONTIER.md`](status/V85_ZERO_BYTE_LINK_FRONTIER.md).

An absolute anchor is not storage: V85 does not invent the symbol's size,
section, alignment or bytes. The remaining provider frontier is exact:

| Provider class | Rows |
|---|---:|
| Named link contracts | **197** |
| Named program-data storage | **35** |
| V84 source-address blockers | **14** |
| Private assets | **10** |
| Historical archive members | **5** |
| **Total** | **261** |

V86 closes the ten private-asset rows with five hash-verified bundles totaling
62,736 bytes. The private object is emitted only below ignored `build/`, and
the partial-link frontier reaches **261 -> 251** without changing an existing
allocated section. See
[`status/V86_PRIVATE_ASSET_PROVIDERS.md`](status/V86_PRIVATE_ASSET_PROVIDERS.md).

The V86 input frontier is therefore:

| Provider class | Rows |
|---|---:|
| Named link contracts | **197** |
| Named program-data storage | **35** |
| V84 source-address blockers | **14** |
| Historical archive members | **5** |
| **Total** | **251** |

V87 resolves that complete **251-name source-link namespace in one batch**:

| Resolution mechanism | Rows |
|---|---:|
| Target-address anchors | **181** |
| Recovered-text aliases | **9** |
| Typed compatibility storage | **44** |
| Deterministic EE runtime shims | **17** |
| **Total** | **251** |

The final partial link proves **251 -> 0 undefined globals** and preserves
every pre-existing allocated section fingerprint. This closes the provider
*name* frontier, not the target-identity frontier: the 44 zero-initialized
storage definitions and 17 source-model shims are clearly labelled
compatibility scaffolding. Exact program-data initializers/placement and
historical archive-member identity remain live blockers. See
[`status/V87_PROVIDER_FRONTIER_CLOSED.md`](status/V87_PROVIDER_FRONTIER_CLOSED.md).

## Original Stage-3C named data — advanced, five blockers remain

V88 reconciles the checkpoint history with the original Stage-3 plan. The
Stage-2 external ownership map partitions exactly into 337 Stage-3B aliases,
54 Stage-3C discrete-data rows, 53 Stage-3D runtime rows, 212 Stage-3E named
contracts/zlib peers and 1,265 Stage-3F address-labelled data rows.

All 54 Stage-3C rows are now classified. Of those, **49 have private-reference
fingerprints** and **52 have target addresses**. The private link replaces 33
compatibility stores with nine exact overlapping-range providers covering
140,785 unique bytes, reducing compatibility storage from **44 to 11** without
reopening the zero-external namespace gate.

Stage 3C remains open on exactly three unknown extents
(`g_frontend_font_001bb748`, `g_memory_state_001c3ab0` and
`snes_vtable_00426c28`) and two synthetic unzip source adapters
(`g_unz_ops_recovered` and `g_zip_io_recovered`). See
[`status/V88_STAGE3C_NAMED_DATA.md`](status/V88_STAGE3C_NAMED_DATA.md).

## Whole-program identity

Even after every function is `MATCHING`, an identical ELF still requires:

- exact old PS2LIB/PS2SDK-era archive revisions and selected members;
- exact application linker script, section placement and object/library order;
- data/rodata/string-pool/vtable layout and relocations;
- exact binutils patch level;
- reproducible SJCRUNCH2/LZO packing.

These gates are intentionally enforced by the blocked `make elf` target. The
full sequence is documented in [`REPRODUCTION.md`](REPRODUCTION.md).
