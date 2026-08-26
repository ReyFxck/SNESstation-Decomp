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
- `link_contracts.tsv` classifies every one of the 1,598 externals in the V84
  aggregate: 1,274 absolute target-address anchors, 63 semantic text aliases
  and 261 explicit provider blockers.
- `link_contract_reviews.tsv` freezes four non-mechanical semantic spellings
  with checked public evidence paths and tokens.
- `private_asset_providers.tsv` maps five verified private ranges to all ten
  asset data/size contracts, including padding, alignment and section names;
  it contains hashes and geometry only.
- `provider_frontier_closure.tsv` classifies all 251 names left by V86 into
  181 target-address anchors, nine recovered-text aliases, 44 compatibility
  storage definitions and 17 deterministic EE runtime shims.

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

Use `make link-contracts-public-check` to verify the complete public V85
frontier without a target compiler. `make link-contracts` rebuilds the V84
aggregate, applies the 1,337 proved `--defsym` contracts and writes ignored
reports under `build/link-contracts/`. Absolute address anchors assign values
only: they do not create storage or reproduce target bytes.

Use `make private-assets-public-check` to verify the byte-free V86 provider
map. With a legal reference present, `make private-assets` uses `.incbin` only
inside ignored `build/private-assets/`, verifies 62,736 contiguous provider
bytes and reduces the partial-link frontier from 261 to 251. No generated
assembly, object, report or extracted payload may be committed.

Use `make provider-frontier-public-check` to verify that the V87 manifest is
the exact V86 frontier. `make provider-frontier` then generates ignored
storage/shim sources, partially links them after the private providers and
proves **251 -> 0** undefined globals. This is source-link namespace closure;
it does not prove target initializers, archive membership or final placement.
