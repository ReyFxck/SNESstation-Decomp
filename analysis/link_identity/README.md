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
