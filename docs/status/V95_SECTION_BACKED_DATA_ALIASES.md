# V95 — section-backed data addresses

Base: `6bc08fa98b4d171fa46c2f4dfcfec15257ab0dc1` (V94).

V95 connects proved storage to the real source-link aggregate. It does not
close Stage 3F, reconstruct complete C objects, or produce a replacement ELF.
The V94 705/1,265 minimum-access ledger remains unchanged.

## Verified scope

| Evidence | Result |
|---|---:|
| Original unnamed address contracts | 1,265 |
| Section-backed addresses | 886 |
| Backed with V94 direct-access evidence | 705 |
| Additional interior-address-only aliases | 181 |
| Unbacked absolute anchors, unchanged | 379 |
| Reused Stage-3C/E / private-asset sections | 61 / 5 |
| Reused bytes | 230,518 |
| New minimum-access sections / bytes | 109 / 69,768 |
| New initialized / zero-fill bytes | 69,768 / 0 |
| Total nonoverlapping backing sections / bytes | 175 / 300,286 |
| Preserved source relocations to the 886 names | 12,078 |
| Preserved source HI16 / LO16 relocations | 5,557 / 6,521 |
| Isolated R_MIPS_32 pointer values proved | 886 |
| Isolated HI16/LO16 address pairs proved | 886 |
| Total synthetic relocation records proved | 2,658 |
| Undefined globals before / after | 0 / 0 |

The 181 interior labels have **no access-width claim**. Their addresses fall
inside previously proved storage, which is enough for a location alias, not
for a complete scalar, array or structure. All 886 alias symbol sizes remain
zero. The V94 minimum-access evidence remains 705 witnessed / 560 unwitnessed;
886 backed / 379 unbacked measures a different property and is not an increase
in the number of instruction witnesses.

## Derivation and private byte checks

[`data_backing.tsv`](../../analysis/link_identity/data_backing.tsv) records
address-to-section ownership and offsets. The companion
[`data_backing_sections.tsv`](../../analysis/link_identity/data_backing_sections.tsv)
contains geometry, evidence digests and SHA-256 fingerprints, never private
bytes. The gate validates the prior Stage-3C/E, asset and direct-access inputs,
merges only overlapping or adjacent proved ranges, subtracts existing backing
and rejects duplicate owners, gaps claimed as bytes, and initialized/BSS
boundary crossings. It never derives an object size from a neighboring symbol
or a provisional lifted C declaration.

The private gate first checks the complete reference identity, re-derives the
V94 instruction/call witnesses and checks every section fingerprint. The 109
additions are materialized only under ignored `build/data-backing/` from the
user's private reference. Existing storage is reused rather than copied into
overlapping sections. Public validation can verify geometry and hash syntax,
not the content of private bytes; only private verification proves those.

## Why section-relative labels matter

The old anchors describe absolute target addresses but allocate no storage.
The new partial link assigns aliases **inside each output section**, before
its `KEEP` input mapping. For example, `DAT_0034551c` becomes offset `0x3c`
inside `.data.stage3ce.va_003454e0`, not an absolute symbol with value `0x3c`.
An external `--defsym name=base+offset` expression alone can collapse to that
incorrect absolute offset in the historical relocatable linker; the verifier
therefore checks the actual ELF section index, value, binding and zero size.

The verifier requires the same global symbol roster and unchanged identities
for all unrelated or unbacked globals. Every pre-existing allocated section
retains its type, flags, size, alignment and byte fingerprint. The complete
section/offset/type/name roster of the 12,078 source relocations to the backed
aliases remains identical; this is preservation, not a claim that all final
application relocation values have already been evaluated.

For the independent address probe, the tool extracts data sections from the
actual rebound aggregate. It links synthetic pointer and HI16/LO16 references,
places the backing sections at their target addresses, and checks every final
symbol and encoded value. Synthetic code/data live at `0x00510000` and
`0x00500000`, outside the original target image. This proves relocation and
placement semantics, **not emulator code, bootability or a replacement ELF**.

## Reproduce

Public, without original bytes or EE tools:

```sh
make check
make data-backing-public-check
```

Private hash/instruction proof, without a compiler:

```sh
make data-backing-verify
```

Complete EE source chain, section rebinding and isolated address proof:

```sh
make data-backing
# Or reuse the already bootstrapped compiler:
make data-backing-check EE_CC=build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc
make reproduce-check
```

Normal validation never refreshes fingerprints. `make data-backing-refresh`
is the explicit, review-required capture operation. Reports and generated
objects remain ignored; private bytes are not part of the public patch.

The regression suite adds 24 tests (363 total), including forged bounds,
missing rows, evidence/hash drift, unbacked anchors, absolute-vs-relative
symbol identity and signed-LO16 carry handling. Host syntax remains 108/108;
the 97-TU / 96-canonical-object checkpoint and its source ownership maps are
unchanged. This does not promote any new formal function row.

## Remaining work

- Establish storage for 379 still-unbacked addresses and complete object/array
  bounds, indexed uses and zero-fill requirements, including backed interiors.
- Select and integrate exact per-function implementations and historical member
  data into the final object set; the canonical behavioral aggregate is not
  already the original target text.
- Reproduce final layout, all application relocations, alignment and link order.
- Reproduce SJCRUNCH2 packing and both frozen image hashes.

`make elf` deliberately remains blocked. Earlier evidence is preserved in
[`V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md`](V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md).
