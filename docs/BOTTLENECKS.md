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
alias-resolved live aggregate retains 1,570 unresolved contracts, down from
the post-libgcc-refactor source-tree count of 1,893, while every allocated section remains
byte-identical. See
[`status/V84_REVIEWED_SOURCE_ALIASES.md`](status/V84_REVIEWED_SOURCE_ALIASES.md).

The 14 deliberately blocked rows split into seven historical archive-member
entries, one source helper that is not equivalent to the complete historical
function and six addresses outside the audited target manifest. Those need
archive/boundary proof, not raw-address definitions.

## Zero-byte link contracts — full frontier frozen

The live contract map classifies all 1,570 post-refactor externals. It assigns
1,273 address-qualified program-data names their frozen absolute value
and binds 63 uniquely identified call contracts to existing recovered global
text. The partial link therefore reaches **1,570 -> 234 unresolved externals**
while every allocated section fingerprint remains identical. See
[`status/V85_ZERO_BYTE_LINK_FRONTIER.md`](status/V85_ZERO_BYTE_LINK_FRONTIER.md)
for the frozen pre-refactor method and
[`status/V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md`](status/V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md)
for the live counts.

An absolute anchor is not storage: V85 does not invent the symbol's size,
section, alignment or bytes. The remaining provider frontier is exact:

| Provider class | Rows |
|---|---:|
| Named link contracts | **176** |
| Named program-data storage | **32** |
| V84 source-address blockers | **14** |
| Private assets | **10** |
| Historical archive members | **2** |
| **Total** | **234** |

V86 closes the ten private-asset rows with five hash-verified bundles totaling
62,736 bytes. The private object is emitted only below ignored `build/`, and
the partial-link frontier reaches **234 -> 224** without changing an existing
allocated section. See
[`status/V86_PRIVATE_ASSET_PROVIDERS.md`](status/V86_PRIVATE_ASSET_PROVIDERS.md).

The V86 input frontier is therefore:

| Provider class | Rows |
|---|---:|
| Named link contracts | **176** |
| Named program-data storage | **32** |
| V84 source-address blockers | **14** |
| Historical archive members | **2** |
| **Total** | **224** |

The regenerated closure resolves the complete **224-name source-link namespace in one batch**:

| Resolution mechanism | Rows |
|---|---:|
| Target-address anchors | **175** |
| Recovered-text aliases | **9** |
| Typed compatibility storage | **39** |
| Deterministic EE runtime shims | **1** |
| **Total** | **224** |

The final partial link proves **224 -> 0 undefined globals** and preserves
every pre-existing allocated section fingerprint. This closes the provider
*name* frontier, not the target-identity frontier: the 39 zero-initialized
storage definitions and the remaining `snprintf` runtime shim are clearly labelled
compatibility scaffolding. Exact program-data initializers/placement and
historical archive-member identity remain live blockers. See
[`status/V87_PROVIDER_FRONTIER_CLOSED.md`](status/V87_PROVIDER_FRONTIER_CLOSED.md)
for the historical closure and the V90 report for the regenerated frontier.

## Original Stage-3C named data — closed

V89 closes the historical 54-row plan without pretending that invented source
storage exists in the target. Exactly **50 rows are real target objects** and
all 50 carry private-reference fingerprints. The other four rows are completed
source refactors: `g_Memory`, `g_memory_state_001c3ab0`,
`g_unz_ops_recovered` and `g_zip_io_recovered` are proved absent and removed.

The 40 non-asset target ranges form 15 overlap-aware clusters covering 141,159
unique bytes. Thirty-two compatibility stores are replaced, reducing the
current live storage scaffolding from **39 to seven**, and the partial link
still proves **224 -> 0 undefined globals**. The frozen V89 report retains its
historical pre-Stage-3E count of 41 to nine. See
[`status/V89_STAGE3C_CLOSED.md`](status/V89_STAGE3C_CLOSED.md).

## Original Stage-3E named contracts — closed

V90 closes the historical **212-row** tranche without treating source-lift
artifacts as target globals:

| Closed evidence class | Rows |
|---|---:|
| Aliases to recovered target text | **23** |
| Exact target ranges | **164** |
| Exact target entries | **2** |
| External MMIO/BIOS addresses | **2** |
| Canonical data alias (`errno`) | **1** |
| Completed source refactors | **20** |
| **Total** | **212/212** |

All seven zlib peers are among the exact text aliases. The 164 target ranges
plus the canonical data word carry **165 private-reference fingerprints**.
Twenty instruction/stack pseudo-symbols are removed from the source namespace,
and `errno` now aliases the already owned word at `0x00425a70`; this explains
the 21-row reduction from the V89 live source count.

Stage 3E replaces the seven storage definitions left after the current Stage
3C gate. Across both tranches, **196 exact provider names** occupy **61
overlap-aware clusters covering 167,782 unique bytes**. Compatibility storage
therefore reaches **39 -> 0**, while the private partial link proves **224 ->
0 undefined globals**. One runtime shim remains explicit for Stage 3D. See
[`status/V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md`](status/V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md).

## Stage-3D libgcc subtranche — closed

V91 adjudicates all seven compiler-runtime contracts carried by the original
Stage-3D plan. Four complete GCC 3.2.2 archive-member `.text` sections match
the target across **3,848 bytes** after masking only **21** relocation-controlled
words: `_muldi3.o`, `_floatdisf.o`, `_udivdi3.o` and `_umoddi3.o`.

The other three names are source-lift artifacts, not missing target members.
The EE archive gives `ashlti3.o` and `lshrti3.o` empty `.text` sections, and
the non-empty `_fixunssfdi.o` has zero relocation-normalized occurrences in
the complete target image. Rewriting the affected conversions and wide lift
helpers removes all three undefined references under the historical compiler.

The live source frontier becomes **1,893 -> 1,570 -> 234 -> 224 -> 0**, and
runtime shims fall from four to one. The remaining shim is `snprintf`; the
remaining Stage-3D work is libc/Newlib and PS2 runtime/archive identity. See
[`status/V91_STAGE3D_LIBGCC_CLOSED.md`](status/V91_STAGE3D_LIBGCC_CLOSED.md).

## Whole-program identity

Even after every function is `MATCHING`, an identical ELF still requires:

- exact old PS2LIB/PS2SDK-era archive revisions and selected members;
- exact application linker script, section placement and object/library order;
- data/rodata/string-pool/vtable layout and relocations;
- exact binutils patch level;
- reproducible SJCRUNCH2/LZO packing.

These gates are intentionally enforced by the blocked `make elf` target. The
full sequence is documented in [`REPRODUCTION.md`](REPRODUCTION.md).
