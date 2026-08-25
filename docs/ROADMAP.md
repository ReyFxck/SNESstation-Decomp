# Roadmap from structural closure to exact reproduction

## 1. Formal function closure

- [x] Freeze the packed and unpacked target hashes.
- [x] Audit the 1,041-entry structural universe.
- [x] Provide a behavioral/source model for every audited entry.
- [x] Promote 997 entries with strict compiler/object evidence.
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
- [ ] Close the remaining 44 working-frontier entries.

## 2. Build-ready source ownership

- [ ] Freeze translation-unit boundaries and symbol visibility.
- [ ] Replace provisional-width types with proven EE ABI types.
- [ ] Assign globals, BSS, constructors, vtables and static data to exact
  objects.
- [ ] Split the large structural-lift source only when traceability and matching
  checks remain exact.
- [ ] Produce a complete historical-compiler object set.

## 3. Link identity

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
