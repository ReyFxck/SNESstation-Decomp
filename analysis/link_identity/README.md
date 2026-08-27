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
- `link_contracts.tsv` classifies every one of the 1,573 live post-refactor
  externals: 1,273 absolute target-address anchors, 63 semantic text aliases
  and 237 explicit provider blockers.
- `link_contract_reviews.tsv` freezes four non-mechanical semantic spellings
  with checked public evidence paths and tokens.
- `private_asset_providers.tsv` maps five verified private ranges to all ten
  asset data/size contracts, including padding, alignment and section names;
  it contains hashes and geometry only.
- `provider_frontier_closure.tsv` classifies all 227 live names left after the
  private providers into 175 target-address anchors, nine recovered-text
  aliases, 39 compatibility storage definitions and four historical-runtime
  shims.
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
bytes and reduces the partial-link frontier from 237 to 227. No generated
assembly, object, report or extracted payload may be committed.

Use `make provider-frontier-public-check` to verify the exact live frontier.
`make provider-frontier` then generates ignored storage/shim sources, partially
links them after the private providers and proves **227 -> 0** undefined
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
compatibility storage reaches **39 -> 0** and the aggregate reaches **227 ->
0** undefined globals. Generated bytes remain below ignored
`build/named-contracts/`; final placement remains Stage 3G, and four runtime
shims remain for Stage 3D.
