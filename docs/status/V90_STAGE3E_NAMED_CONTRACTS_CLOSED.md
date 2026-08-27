# V90 — Stage 3E named contracts closed

V90 closes the original Stage-3E plan: **205 named external contracts plus
seven zlib peers, 212/212 rows total**. The closure distinguishes target-backed
identities from names introduced only by the buildable source lift.

| Closed evidence class | Rows |
|---|---:|
| Aliases to recovered target text | **23** |
| Exact target ranges | **164** |
| Exact target entries | **2** |
| External MMIO/BIOS addresses | **2** |
| Canonical data alias | **1** |
| Completed source refactors | **20** |
| **Historical Stage-3E total** | **212/212** |

The 164 ranges and canonical `errno` word carry **165 exact
private-reference fingerprints**. The public manifest records only addresses,
extents, classifications and SHA-256 values; no target bytes are tracked.

## Seven zlib peers closed

All seven historical zlib peers bind to already recovered, formally matched
target text:

| Historical contract | Canonical recovered text |
|---|---|
| `deflateEnd_00191308` | `deflateEnd_recovered` |
| `deflateInit2__001908ec` | `deflateInit2__recovered` |
| `deflateReset_00190d10` | `deflateReset_recovered` |
| `gzopen` | `gzopen_00193564` |
| `inflateEnd_00192784` | `inflateEnd_recovered` |
| `inflateInit__00192948` | `inflateInit__recovered` |
| `inflate_0019296c` | `inflate_recovered` |

This closes the Stage-3E zlib subset without adding wrapper functions or
publishing a second copy of target text.

## Twenty target-absent contracts removed

The following names were compiler instructions, inline SDK/BIOS operations or
stack-local artifacts in the recovered source model. They are not global
providers in the target and are now expressed locally:

| Source-lift class | Removed contracts |
|---|---|
| EE/math intrinsics | `EI`, `SQRT`, `isinf`, `isnan`, `lrintf`, `syscall`, `trap` |
| Inline PS2 operations | `ps2_add_intc_handler_recovered`, `ps2_bios_syscall_2_recovered`, `ps2_disable_intc_recovered`, `ps2_enable_intc_recovered`, `ps2_gs_put_imr_recovered`, `ps2_remove_intc_handler_recovered` |
| Stack/base pseudo-symbols | `SUB_00002214`, `_auStack_10e4`, `stack0x00000000`, `stack0xffffef20`, `stack0xffffef90`, `stack0xffffef98`, `stack0xffffefa4` |

`errno` is the separate canonical-data case: the source now aliases it to the
already owned four-byte `ps2lib_errno_00425a70` word instead of allocating a
second compatibility object.

These 21 source-namespace corrections reduce the live source external count
from V89's 1,917 to **1,896** while preserving the historical 212-row ledger.

## Exact ranges and provider result

The 164 Stage-3E ranges form **49 overlap-aware clusters covering 26,633
unique bytes**. When combined with the exact Stage-3C ranges, the private link
materializes **196 provider names in 61 clusters covering 167,782 unique
bytes**.

| Gate | Live V90 result |
|---|---:|
| Source-tree externals | **1,896** |
| After 323 source-address aliases | **1,573** |
| Zero-byte contracts | **1,336/1,573 resolved** (`1,273` anchors + `63` aliases) |
| Provider frontier before private assets | **237** |
| After ten private-asset providers | **227** |
| Live provider closure | **175 anchors + 9 aliases + 39 storage + 4 shims** |
| Exact Stage-3C compatibility replacements | **32** |
| Exact Stage-3E provider replacements | **164** |
| Combined exact provider replacements | **196** |
| Compatibility storage | **39 → 0** |
| Final partial-link externals | **227 → 0** |

Every previously allocated section fingerprint remains unchanged during the
relocatable link. Generated range bytes, assembly, objects, maps and reports
stay below ignored `build/named-contracts/`.

## Reproducible gates

The public gate requires neither the private ELF nor an EE compiler:

```bash
make named-contracts-public-check
```

With a legally obtained `original/SNES_EMU.ELF`, run the complete historical
compiler and private-reference proof:

```bash
make named-contracts
```

To reuse an existing compiler:

```bash
make reference
make named-contracts-check EE_CC=/absolute/path/to/ee-gcc
```

## Claim boundary and next work

Stage 3E is closed, but V90 is not a replacement ELF. The exact ranges are
emitted into isolated relocatable sections; their final placement, alignment,
relocations and object order remain Stage 3G.

Four compatibility runtime shims remain explicit: `__ashlti3`,
`__fixunssfdi`, `__lshrti3` and `snprintf`. They belong to Stage 3D's exact
historical archive-member proof. Stage 3F still has 1,265 address-qualified
data contracts requiring exact ranges/bytes, followed by Stage 3G layout,
Stage 3H unpacked-image identity and packed SJCRUNCH2 identity.
