# V85 zero-byte link-contract frontier

V85 freezes the complete unresolved-symbol frontier after the V84
source-address aliases. Of the 1,598 input externals, 1,337 can be resolved
without choosing an archive member or allocating a single byte. The resulting
relocatable aggregate has exactly 261 unresolved provider contracts.

This checkpoint does **not** claim that target data has been rebuilt. An
absolute address anchor gives the linker a verified value only; it has no
size, storage, section contents or alignment.

## Reproducible gates

The compiler-free manifest and all reviewed citations are checked with:

```bash
make link-contracts-public-check
```

Build the complete dependency chain with the pinned historical compiler and
apply the frozen contracts:

```bash
make link-contracts
```

With an existing compatible EE GCC 3.2.2 installation:

```bash
make link-contracts-check EE_CC=/absolute/path/to/ee-gcc
```

The final command rebuilds 97 translation units, applies the V84 aliases and
then invokes `ee-ld -r` with only audited `--defsym` relationships. It checks
the exact input/output undefined sets, every anchor value, every
alias/canonical value and type, and the complete allocated-section
fingerprints.

## Verified result

| Measurement | Result |
|---|---:|
| V84 aggregate externals | **1,598** |
| Absolute target-address anchors | **1,274** |
| Semantic text aliases | **63** |
| Contracts resolved in V85 | **1,337/1,598** |
| Stage-2 cumulative externals resolved | **1,660/1,921** |
| Remaining provider frontier | **261** |
| Allocated section changes | **0** |
| Code/data bytes emitted | **0** |

The 1,274 anchors are restricted to `program-data` symbols whose names end in
an eight-digit target address inside the frozen unpacked-image interval. They
are emitted as absolute (`A`) symbols with that exact value. V85 deliberately
does not apply this rule to private assets, function-boundary blockers or
unqualified names.

The 63 semantic aliases require one `MATCHING` progress row and exactly one
existing recovered global text definition. The accepted spelling rules are:

| Evidence class | Aliases |
|---|---:|
| Exact audited progress name | **44** |
| Strip target-address suffix | **6** |
| Strip `_like` | **5** |
| Strip `_recovered` | **3** |
| Strip address and `_like` | **1** |
| Explicit reviewed identity | **4** |
| **Total** | **63** |

The four reviewed exceptions are frozen in
[`link_contract_reviews.tsv`](../../analysis/link_identity/link_contract_reviews.tsv):

| External contract | Canonical text symbol | Reason |
|---|---|---|
| `FlushCache_0019ceb0` | `FlushCache` | Address-qualified syscall wrapper spelling. |
| `SifWriteBackDCache_0019cf10` | `SifWriteBackDCache` | Address-qualified cache-writeback spelling. |
| `fioSeek_like_0019d360` | `fioLseek_0019d360` | Old fileIO RPC uses the historical `Lseek` export name. |
| `puts` | `puts_like_recovered` | The target routine writes without appending the standard newline. |

## Exact remaining frontier

| Provider class | Rows | Required next proof |
|---|---:|---|
| Named link contracts | **197** | Identify a unique real definition or archive provider. |
| Named program data | **35** | Recover storage class, size, alignment, section and bytes. |
| V84 source-address blockers | **14** | Prove archive/boundary identity; do not assign raw addresses. |
| Private assets | **10** | Recover private bytes through the documented extraction workflow. |
| Historical archive members | **5** | Select the exact old archive revision and member. |
| **Total** | **261** | |

The authoritative row-by-row classification is
[`link_contracts.tsv`](../../analysis/link_identity/link_contracts.tsv). A
normal repository check regenerates it in memory and fails if the committed
manifest or any reviewed evidence drifts.

## Honest boundary

V85 is a Stage-3B link-frontier checkpoint, not a linkable replacement ELF.
It does not reproduce `.data`, `.rodata` or `.bss`, select historical archive
members, resolve final relocations, infer a linker script, or establish object
and library order. Those operations necessarily emit or place bytes and must
be proved in the next provider batches before byte/hash convergence begins.
