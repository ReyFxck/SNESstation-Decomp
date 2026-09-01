# Link-identity evidence

This directory contains public, byte-free evidence for the Stage-3 link gate.

- `unpacked_layout.json` freezes the verified packed/unpacked hashes,
  SJCRUNCH2 section and block geometry, BSS span and 64 KiB hash windows.
- `source_address_aliases.tsv` records all 337 Stage-2 address-shaped external
  names: 323 are proved against canonical global text symbols and 14 retain an
  explicit evidence blocker.
- `source_alias_reviews.tsv` freezes three semantic identity proofs and eight
  deliberately blocked boundary/archive decisions, each with a checked public
  evidence path and token.
- `link_contracts.tsv` classifies every one of the 1,569 live post-refactor
  externals: 1,273 absolute target-address anchors, 63 semantic text aliases
  and 233 explicit provider blockers.
- `link_contract_reviews.tsv` freezes four non-mechanical semantic spellings
  with checked public evidence paths and tokens.
- `private_asset_providers.tsv` maps five verified private ranges to all ten
  asset data/size contracts, including padding, alignment and section names;
  it contains hashes and geometry only.
- `provider_frontier_closure.tsv` classifies all 223 live names left after the
  private providers into 175 target-address anchors, nine recovered-text
  aliases, 39 compatibility storage definitions and zero runtime shims.
- `named_data.tsv` closes the original 54-row Stage-3C tranche: 50 real target
  objects carry private-reference fingerprints and four source-only adapters
  have completed removal records; no address-only rows remain.
- `named_data_reviews.tsv` records the non-mechanical address, extent and
  source-refactor decisions with checked public evidence paths and tokens.
- `stage3c_boundary_proofs.tsv` records the reviewed exclusive-end boundaries
  for the frontend font object and four-entry vtable address point.
- `named_contracts.tsv` closes the historical 212-row Stage-3E tranche: 165
  target-backed ranges/data aliases are fingerprinted, 23 names bind to
  recovered text, two target entries and two external addresses are proved,
  and 20 source-only instruction/stack contracts are removed.
- `libgcc_contracts.tsv` closes the seven-contract Stage-3D libgcc subtranche:
  four complete GCC 3.2.2 archive-member text sections are exact, and three
  source-only compiler-libcall artifacts have completed removal records.
- `runtime_refactors.tsv` closes the `snprintf` source contract: four frozen
  target spans record direct call addresses, the existing `sprintf` callee,
  target hashes and immutable matching-evidence hashes.
- `runtime_members.tsv` adjudicates the remaining 45 runtime contracts: 43
  select exact complete member text and two historical candidates stay rejected.
- `runtime_member_objects.tsv` freezes 42 selected and two rejected PS2LIB
  source recipes, full-text geometry/hashes, dependency hashes and symbol maps.
- `runtime_member_inputs.tsv` pins 36 source/header inputs from historical
  PS2LIB migration snapshots plus three historical compiler headers.
- `runtime_overrides.tsv` and `runtime_override_witnesses.tsv` prove the two
  target-selected reconstructed overrides via 15 historical named calls and
  104 exact linked bytes; the original runtime ledger reaches 53/53.
- `unnamed_data_accesses.tsv` retains all 1,265 unnamed contracts: 824 have
  minimum target-consumed spans (167,521 unique bytes), and 441 lack
  witnesses. Complete object/array extents and Stage-3F closure remain open.

The manifest deliberately contains no original executable bytes, extracted
assets, local paths or encoded binary payloads. Regenerate it only from a
legally obtained private reference with `make layout-oracle-refresh`; ordinary
verification uses `make layout-oracle` and must leave the tracked file
unchanged.

Use `make compare-unpacked CANDIDATE_RAW=/path/to/rebuilt.bin` to write an
ignored comparison report under `build/layout-oracle/`. A mismatch reports the
first image offset/load address and all differing hash windows without copying
either full image into the public tree.

Use `make source-aliases-public-check` for the compiler-free manifest gate and
`make source-aliases` to build the Stage-2 aggregate and apply only proved
`alias=canonical_symbol` relationships. The latter writes ignored artifacts
under `build/source-aliases/` and verifies that allocated sections are
unchanged.

Use `make link-contracts-public-check` to verify the complete public contract
frontier without a target compiler. `make link-contracts` rebuilds the live
aggregate, applies the 1,336 proved `--defsym` contracts and writes ignored
reports under `build/link-contracts/`. Absolute address anchors assign values
only: they do not create storage or reproduce target bytes.

Use `make private-assets-public-check` to verify the byte-free V86 provider
map. With a legal reference present, `make private-assets` uses `.incbin` only
inside ignored `build/private-assets/`, verifies 62,736 contiguous provider
bytes and reduces the partial-link frontier from 233 to 223. No generated
assembly, object, report or extracted payload may be committed.

Use `make provider-frontier-public-check` to verify the exact live frontier.
`make provider-frontier` then generates ignored storage sources, partially
links them after the private providers and proves **223 -> 0** undefined
globals. This is source-link namespace closure;
it does not prove target initializers, archive membership or final placement.

Use `make named-data-public-check` to validate the byte-free closed ledger and
the historical 337/54/53/212/1,265 Stage-3 plan. With a legal private
reference, `make named-data` verifies all 50 target-object fingerprints and
materializes 40 non-asset ranges in 15 overlap-aware clusters. Their 141,159
unique bytes, generated assembly and objects stay below ignored
`build/named-data/`; four completed refactors carry no invented bytes. Stage 3C
is closed, while final placement belongs to Stage 3G.

Use `make named-contracts-public-check` to validate the byte-free 212-row
Stage-3E ledger. With a legal private reference, `make named-contracts`
rechecks 165 fingerprints and materializes the 164 exact Stage-3E ranges
together with the 32 exact Stage-3C storage replacements. The combined 196
provider names form 61 overlap-aware clusters covering 167,782 unique bytes;
compatibility storage reaches **39 -> 0** and the aggregate reaches **223 ->
0** undefined globals. Generated bytes remain below ignored
`build/named-contracts/`; final placement remains Stage 3G. V92 removes the
last compatibility runtime shim.

Use `make libgcc-contracts-public-check` to validate the seven-row hash-only
ledger without private files. With the reference and historical EE compiler,
`make libgcc-contracts` extracts the selected `libgcc.a` members into ignored
`build/libgcc-contracts/`, compares complete `.text` sections across 3,848
target bytes after 21 relocations, proves the three refactors remain absent
from the live aggregate.

Use `make runtime-refactors-public-check` to validate the four-call ledger
and prove the removed `snprintf` namespace stays absent. `make runtime-refactors`
runs the full EE/private dependency chain and checks each original direct
JAL to `sprintf@0x0019e3d0` against the SHA-verified reference. There are zero
runtime shims; this does not claim whole-function or final-ELF identity for the
source models.

Use `make runtime-members-public-check` for the 45-contract ledger and
`make runtime-members` for the full private chain plus historical member
rebuild. The verifier checks 12,964 complete text bytes after 700 precise
relocation masks, including internal helpers and padding. The V93 checkpoint
was 51/53; the separate V94 override gate closes the remaining two contracts
without selecting the rejected `puts`/`abort` candidates. Pinned migration sources
are reproducible witnesses, not proof of a particular historical archive
container. Selected `.a` files and rejected objects stay below ignored
`build/runtime-members/`; member data, final relocation values and global link
order remain separate gates. See
[`V93_STAGE3D_RUNTIME_MEMBERS.md`](../../docs/status/V93_STAGE3D_RUNTIME_MEMBERS.md).

`make runtime-overrides` verifies the complete dependency chain and links the
two recovered providers at their exact target addresses. `make unnamed-data`
re-derives consumed data spans from the private target; ordinary validation
does not update hashes. Both have repository-only `*-public-check` gates.
See [`V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md`](../../docs/status/V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md).

`data_backing.tsv` preserves the 1,265-name roster and assigns 961 zero-sized
section-relative aliases; 304 remain absolute anchors without proved storage.
`data_backing_sections.tsv` owns 183 nonoverlapping physical ranges (66 reused,
117 materialized), with hashes only. Its 137 interior-address-only aliases do
not gain access-width or object-bound claims. V96 adds the proof kind, full
function hash and analyzer hash to the access ledger; 131 entries now use
must-constant CFG witnesses, including 119 newly witnessed contracts.

`make data-backing-public-check` re-derives that geometry and ownership without
private bytes. `make data-backing-verify` verifies the reference hashes and
instruction evidence without requiring a compiler. `make data-backing` also
links the real aggregate, preserves original allocated sections and all 12,434
affected source relocations, and proves 2,883 synthetic address relocations in
an isolated data-placement ELF. It does not produce an emulator. Only explicit
`make data-backing-refresh` replaces the reviewed hash ledger. See
[`V96_CONTROL_FLOW_DATA_ACCESSES.md`](../../docs/status/V96_CONTROL_FLOW_DATA_ACCESSES.md).
