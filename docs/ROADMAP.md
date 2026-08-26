# Roadmap from structural closure to exact reproduction

## 1. Formal function closure

- [x] Freeze the packed and unpacked target hashes.
- [x] Audit the 1,041-entry structural universe.
- [x] Provide a behavioral/source model for every audited entry.
- [x] Promote all 1,041 entries with strict compiler/object evidence.
- [x] Regenerate and promote the six recovered V53 results.
- [x] Prove the V73 `S9xLoadCheatFile` and `S9xSPCDump` PS2-I/O variants.
- [x] Prove the V74 `S9xSaveSPC7110RTC` and `S9xLoadSPC7110RTC` PS2 variants.
- [x] Prove five V75 C4 float/math rows and one exact unlisted `C4Op15`
  companion without changing the audited denominator.
- [x] Regenerate the complete frontier map at 47 entries in two coordinated
  tracks.
- [x] Prove the V76 `C4SprDisintegrate` row across its complete 580-byte span
  with the historical EE C++ compiler.
- [x] Regenerate the complete frontier map at 46 entries in two coordinated
  tracks.
- [x] Prove the V77 `C4DrawWireFrame` row across its complete 472-byte span
  with the historical EE C++ compiler.
- [x] Regenerate the complete frontier map at 45 entries in two coordinated
  tracks.
- [x] Prove the V78 `C4BitPlaneWave` row across its complete 584-byte span with
  the historical EE C++ compiler and an isolated target-proven allocation
  profile.
- [x] Regenerate the complete frontier map at 44 entries in two coordinated
  tracks.
- [x] Prove the V79 `C4ConvOAM` row across its complete 952-byte span with a
  clearly labelled raw-exact EE assembly reconstruction and retained readable
  C model.
- [x] Regenerate the complete frontier map at 43 entries in two coordinated
  tracks.
- [x] Prove the V80 23-function quick-win batch across 14,244 complete bytes
  with clearly labelled raw-exact EE assembly reconstructions and retained
  readable behavioral models.
- [x] Regenerate the complete frontier map at 20 entries in two coordinated
  tracks.
- [x] Prove the V81 final 20 functions across 71,384 complete bytes with
  clearly labelled raw-exact EE assembly reconstructions and retained readable
  behavioral models.
- [x] Close the audited function frontier at 1,041/1,041 and regenerate the
  zero-entry frontier map.

## 2. Build-ready source ownership

- [x] Freeze translation-unit boundaries and symbol visibility in an exact
  97-entry manifest with 96 canonical objects and one explicit alternate.
- [x] Replace provisional-width assumptions with a compiler-checked EE ABI
  contract.
- [x] Assign compiler-emitted globals, BSS, constructors and static data to
  exact objects; assign vtable/data consumers to reserved program-data owners.
- [x] Keep the large structural-lift source unsplit until a changed boundary
  preserves traceability and matching evidence.
- [x] Produce a complete historical-compiler object set and duplicate-free
  canonical relocatable aggregate.

Checkpoint evidence: [`status/BUILD_READY_SOURCE_TREE.md`](status/BUILD_READY_SOURCE_TREE.md).
Exact program-data bytes/placement and final object order remain Stage 3.

## 3. Link identity

- [x] Freeze a byte-free unpacked-layout oracle with SJCRUNCH2 section/block
  geometry, 64 KiB hashes and an exact first-difference comparator.
- [x] Prove and zero-byte bind 257/337 source-address aliases to 242 canonical
  global text symbols without changing allocated sections.
- [x] Prove 66 additional owner-checked/reviewed aliases, reaching 323/337
  aliases against 307 canonical global text symbols.
- [x] Classify all 1,598 externals remaining after V84 and apply 1,274
  target-address data anchors plus 63 uniquely proved semantic text aliases
  without emitting code/data or changing allocated sections.
- [x] Recover five embedded asset bundles from a private hash-verified reference
  and satisfy all 10 data/size contracts without publishing their 62,736 bytes.
- [x] Close the exact 251-name source-link provider frontier: 197 named link
  contracts, 35 named program-data stores, 14 V84 address-alias blockers and
  five historical archive contracts; prove a zero-undefined relocatable
  aggregate while keeping compatibility storage/shims explicitly distinct from
  final target identity.
- [ ] Identify exact old runtime/library revisions and archive members.
- [ ] Recover the application linker script and section alignment.
- [ ] Recover object order and library order.
- [ ] Reproduce relocations, string pooling and final unpacked data layout.
- [ ] Match the unpacked image SHA-256.

## 4. Packed ELF identity

- [ ] Identify the exact SJCRUNCH2 and LZO revisions/options.
- [ ] Reproduce the loader stub and packed entry layout.
- [ ] Match the packed ELF SHA-256.
- [ ] Make `make reproduce` complete successfully from a clean checkout plus the
  private reference ELF.

## Continuous safeguards

- [x] Manifest/source audit in `make check`.
- [x] Generated-status freshness gate.
- [x] Documentation-link gate.
- [x] Stable one-command reproduction interface.
- [x] Run the repository-only checks in GitHub Actions on every change.
