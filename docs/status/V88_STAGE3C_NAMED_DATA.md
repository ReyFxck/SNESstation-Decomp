# V88 — original Stage-3C named-data oracle

V88 returns the whole-program work to the original Stage-3 partition. Earlier
V82–V87 version numbers are checkpoints inside that plan; they are not aliases
for Stage 3A–3F.

The frozen Stage-2 ownership map contains exactly 1,921 external contracts:

| Original tranche | Contract class | Rows |
|---|---|---:|
| 3B | Source-address aliases | **337** |
| 3C | Named globals, vtable/RTTI and private assets | **54** |
| 3D | Historical compiler/C/PS2 runtime | **53** |
| 3E | Named link contracts and zlib peers | **212** |
| 3F | Address-labelled `DAT_*`/`UNK_*` data | **1,265** |
| **Total** |  | **1,921** |

Stage 3A is the separate unpacked-layout infrastructure gate frozen by V82.

The honest current state of the original plan is:

| Tranche | State after V88 |
|---|---|
| 3A | **Closed.** The unpacked layout oracle is reproducible. |
| 3B | **Closed at the address/link level.** 323/337 names are canonical source aliases; the other 14 are bound to their exact target addresses without claiming semantic alias identity. |
| 3C | **Advanced, not closed.** This checkpoint leaves five explicit blockers. |
| 3D | **Linkable, not exact.** Compatibility shims do not prove historical archive revisions or selected members. |
| 3E | **Namespace closed, providers not exact.** Named contracts and zlib peers link, but compatibility providers are not target-identity proof. |
| 3F | **Addresses anchored, ranges/bytes open.** All 1,265 names have target values; this is not recovered storage. |
| 3G | **Open.** Final linker script, order, alignment, relocations and pooling remain. |
| 3H | **Open.** No 3,304,936-byte replacement with the frozen unpacked SHA-256 exists yet. |

## V88 result

[`analysis/link_identity/named_data.tsv`](../../analysis/link_identity/named_data.tsv)
classifies the exact 54-row Stage-3C tranche without publishing target bytes:

| Evidence state | Rows | Meaning |
|---|---:|---|
| `PRIVATE_BYTES_PROVED` | **10** | Five V86 private asset bundles provide ten data/size symbols. |
| `RANGE_PROVED` | **39** | Target address, consumed extent and private bytes/zero-fill are fingerprinted. |
| `ADDRESS_PROVED` | **3** | Target address is known; the complete historical extent is not. |
| `SOURCE_REFACTOR` | **2** | The recovered source invented a contiguous adapter that does not exist as one target object. |
| **Total** | **54** | **49 fingerprinted; 52 addressed.** |

Two additional target ranges were recovered during the review:

- `g_memory_card_available` is the four-byte slot at `0x001ebaec`, proved by
  the target store in `main`.
- `g_shrink_workspace_recovered` is the `0x6000`-byte zero-fill workspace at
  `0x00448200`, proved by `unShrink` constructing that base address.

The private link gate replaces **33** V87 compatibility-storage definitions
with **nine** overlap-aware exact range providers covering **140,785 unique
bytes**. Compatibility storage falls from **44 to 11**, every pre-existing
allocated-section fingerprint remains unchanged, and the aggregate still
proves **251 -> 0 undefined globals**. The ten embedded-asset symbols were
already supplied by the V86 input and are therefore not part of the 33
replacement count.

Generated assembly, extracted range bytes, objects and reports remain below
ignored `build/named-data/`. The public manifests contain only addresses,
extents, classifications, evidence pointers and SHA-256 fingerprints.

## Gates

The repository-only gate needs neither the private ELF nor the EE compiler:

```bash
make named-data-public-check
```

With `original/SNES_EMU.ELF` present, the complete private gate rebuilds the
Stage-2 aggregate, verifies the reference and emits the exact range providers:

```bash
make named-data
```

`make named-data-verify` recomputes all 49 public fingerprints without linking.

## Claim boundary and remaining Stage 3C work

V88 **advances but does not close Stage 3C**. It does not claim original symbol
names, final section placement or complete object boundaries merely from a
minimum consumed extent.

The five explicit blockers are:

1. prove the complete extent of `g_frontend_font_001bb748`;
2. prove the complete extent of `g_memory_state_001c3ab0`;
3. prove the entry count and overlap boundary of `snes_vtable_00426c28`;
4. remove the synthetic `g_unz_ops_recovered` operation-table adapter; and
5. split the synthetic `g_zip_io_recovered` aggregate into the target's actual
   pointer/scalar/workspace layout.

Final virtual placement, alignment, relocations and pooling remain Stage 3G.
