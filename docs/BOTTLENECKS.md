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
alias-resolved live aggregate retains 1,594 unresolved contracts, down from
the post-refactor source-tree count of 1,917, while every allocated section remains
byte-identical. See
[`status/V84_REVIEWED_SOURCE_ALIASES.md`](status/V84_REVIEWED_SOURCE_ALIASES.md).

The 14 deliberately blocked rows split into seven historical archive-member
entries, one source helper that is not equivalent to the complete historical
function and six addresses outside the audited target manifest. Those need
archive/boundary proof, not raw-address definitions.

## Zero-byte link contracts — full frontier frozen

The live contract map classifies all 1,594 post-refactor externals. It assigns
1,273 address-qualified program-data names their frozen absolute value
and binds 63 uniquely identified call contracts to existing recovered global
text. The partial link therefore reaches **1,594 -> 258 unresolved externals**
while every allocated section fingerprint remains identical. See
[`status/V85_ZERO_BYTE_LINK_FRONTIER.md`](status/V85_ZERO_BYTE_LINK_FRONTIER.md).

An absolute anchor is not storage: V85 does not invent the symbol's size,
section, alignment or bytes. The remaining provider frontier is exact:

| Provider class | Rows |
|---|---:|
| Named link contracts | **197** |
| Named program-data storage | **32** |
| V84 source-address blockers | **14** |
| Private assets | **10** |
| Historical archive members | **5** |
| **Total** | **258** |

V86 closes the ten private-asset rows with five hash-verified bundles totaling
62,736 bytes. The private object is emitted only below ignored `build/`, and
the partial-link frontier reaches **258 -> 248** without changing an existing
allocated section. See
[`status/V86_PRIVATE_ASSET_PROVIDERS.md`](status/V86_PRIVATE_ASSET_PROVIDERS.md).

The V86 input frontier is therefore:

| Provider class | Rows |
|---|---:|
| Named link contracts | **197** |
| Named program-data storage | **32** |
| V84 source-address blockers | **14** |
| Historical archive members | **5** |
| **Total** | **248** |

The regenerated closure resolves the complete **248-name source-link namespace in one batch**:

| Resolution mechanism | Rows |
|---|---:|
| Target-address anchors | **181** |
| Recovered-text aliases | **9** |
| Typed compatibility storage | **41** |
| Deterministic EE runtime shims | **17** |
| **Total** | **248** |

The final partial link proves **248 -> 0 undefined globals** and preserves
every pre-existing allocated section fingerprint. This closes the provider
*name* frontier, not the target-identity frontier: the 41 zero-initialized
storage definitions and 17 source-model shims are clearly labelled
compatibility scaffolding. Exact program-data initializers/placement and
historical archive-member identity remain live blockers. See
[`status/V87_PROVIDER_FRONTIER_CLOSED.md`](status/V87_PROVIDER_FRONTIER_CLOSED.md).

## Original Stage-3C named data — closed

V89 closes the historical 54-row plan without pretending that invented source
storage exists in the target. Exactly **50 rows are real target objects** and
all 50 carry private-reference fingerprints. The other four rows are completed
source refactors: `g_Memory`, `g_memory_state_001c3ab0`,
`g_unz_ops_recovered` and `g_zip_io_recovered` are proved absent and removed.

The 40 non-asset target ranges form 15 overlap-aware clusters covering 141,159
unique bytes. Thirty-two compatibility stores are replaced, reducing the live
storage scaffolding from **41 to nine**, and the partial link still proves
**248 -> 0 undefined globals**. See
[`status/V89_STAGE3C_CLOSED.md`](status/V89_STAGE3C_CLOSED.md).

## Whole-program identity

Even after every function is `MATCHING`, an identical ELF still requires:

- exact old PS2LIB/PS2SDK-era archive revisions and selected members;
- exact application linker script, section placement and object/library order;
- data/rodata/string-pool/vtable layout and relocations;
- exact binutils patch level;
- reproducible SJCRUNCH2/LZO packing.

These gates are intentionally enforced by the blocked `make elf` target. The
full sequence is documented in [`REPRODUCTION.md`](REPRODUCTION.md).
