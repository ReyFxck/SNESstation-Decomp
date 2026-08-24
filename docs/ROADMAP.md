# Roadmap from structural closure to exact reproduction

## 1. Formal function closure

- [x] Freeze the packed and unpacked target hashes.
- [x] Audit the 1,041-entry structural universe.
- [x] Provide a behavioral/source model for every audited entry.
- [x] Promote 979 entries with strict compiler/object evidence.
- [ ] Regenerate and promote the six recovered V53 results.
- [ ] Close the remaining 56 working-frontier entries.

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
