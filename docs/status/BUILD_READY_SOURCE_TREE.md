# Build-ready EE source ownership checkpoint

Stage 2 is closed at the source/object boundary. The reconstructed tree has an
explicit translation-unit manifest, a compiler-checked EE ABI contract, frozen
symbol ownership maps and a duplicate-free canonical relocatable aggregate.

This checkpoint deliberately does **not** claim that the final ELF links or
matches. Program-data bytes and placement, historical archive selection, the
linker script/order and SJCRUNCH2 packing remain separate gates.

## Reproducible gate

From a clean checkout, build the pinned C-only historical compiler and rerun the
object gate:

```bash
make source-tree
```

With a compatible EE GCC 3.2.2 already available:

```bash
make source-tree-check EE_CC=/absolute/path/to/ee-gcc
```

The gate compiles the ABI contract and every source file as a real ELF32 MIPS
relocatable object, verifies the object ABI flags, audits local and global
definitions, rejects `COMMON` storage and duplicate canonical globals, and
combines the canonical set with `ee-ld -r`.

## Frozen result

| Measurement | Result |
|---|---:|
| Source files in the exact TU manifest | **97/97** |
| Canonical objects in the partial link | **96** |
| Explicit alternate objects | **1** |
| Owned defined symbols (local + global) | **1,382** |
| Unique canonical global definitions | **1,024** |
| Text / BSS / data / rodata definitions | **1,264 / 62 / 18 / 38** |
| `COMMON` or duplicate canonical definitions | **0 / 0** |
| Classified external contracts | **1,921/1,921** |
| Clone-stable canonical aggregate SHA-256 | `8088dc08ef1442a77a683fec59df9a50c091a977674d03fed31f5f90bd8a5512` |

The external contracts are assigned to their next evidence gate rather than
being filled with invented storage or modern libraries:

| Next gate | Symbols | Meaning |
|---|---:|---|
| Program data | **1,319** | Target-address globals, BSS/data, vtables and embedded private data need exact bytes and placement. |
| Link identity | **549** | Address aliases and named peer-object contracts need exact object/link resolution. |
| Archive identity | **53** | Newlib/libgcc/PS2 runtime imports need exact historical archive members. |

## EE ABI contract

The old compiler validates these widths at compile time:

| C ABI item | Bytes |
|---|---:|
| `char` | 1 |
| `short` | 2 |
| `int` | 4 |
| `long`, `long long` | 8 |
| data pointer | 4 |
| `size_t`, `ptrdiff_t` | 8 |
| `float` | 4 |
| `double` (`-fshort-double`) | 4 |
| `long double` | 8 |

The contract also requires little-endian MIPS, the R5900 code-generation
profile, `-mlong64`, `-fshort-double`, `-mno-abicalls` and `-fno-common`.

## Translation-unit and special ownership decisions

The canonical CDVD owner is
[`src/ps2/cdvd_rpc_historical_recovered.c`](../../src/ps2/cdvd_rpc_historical_recovered.c).
[`src/ps2/cdvd_rpc_recovered.c`](../../src/ps2/cdvd_rpc_recovered.c) remains a
compilable alternate, but is excluded from the aggregate because it overlaps
the same eight public functions. The gate proves that the alternate introduces
no unowned unique global.

Constructor code belongs to the frozen `gsdriver_recovered.o` and
`gspipe_recovered.o` objects. RTTI/vtable consumers are traced to
`libsupcxx_rtti_recovered.o`, `progress11_frontend_recovered.o` and
`address_leaf_recovered.o`; their eventual bytes live under the reserved
`vtable-data.o` ownership contract. Exact vtable placement is therefore an
explicit program-data task, not an accidental unresolved symbol.

The large structural-lift source remains one manifest entry. Splitting it now
would create unproven historical object boundaries, so the frozen rule is to
change that boundary only with preserved address traceability and matching
evidence.

## Committed evidence

- [`translation_units.tsv`](../../analysis/source_tree/translation_units.tsv) — exact TU inventory, deterministic gate order and canonical/alternate role; this is not a claim about historical linker order.
- [`defined_symbol_ownership.tsv`](../../analysis/source_tree/defined_symbol_ownership.tsv) — every compiler-emitted local/global definition and owning object.
- [`external_symbol_ownership.tsv`](../../analysis/source_tree/external_symbol_ownership.tsv) — every aggregate external, requester, owner class and next gate.
- [`special_ownership.tsv`](../../analysis/source_tree/special_ownership.tsv) — constructors and target vtable consumers.
- [`object_fingerprints.tsv`](../../analysis/source_tree/object_fingerprints.tsv) — SHA-256 for all 97 TUs, the ABI contract and canonical aggregate.
- [`ee_abi_contract.c`](../../analysis/source_tree/ee_abi_contract.c) — old-C-compatible compile-time ABI assertions.

Generated objects, logs, link map and JSON/Markdown reports live under ignored
`build/source-tree/`. They contain no private reference ELF or extracted asset.

## Honest boundary of this checkpoint

Stage 2 proves that the recovered sources form a coherent historical-compiler
object set with explicit ownership. It does not prove the exact original 97-file
source organization, define all target-address data, choose the original
archives, reproduce relocations/sections, or match either frozen executable
hash. Those are the Stage-3 and Stage-4 identity gates.

The downstream
[`V84_REVIEWED_SOURCE_ALIASES.md`](V84_REVIEWED_SOURCE_ALIASES.md) checkpoint
resolves 323 of this aggregate's 337 source-address aliases without changing
any allocated section bytes. Two address-trace ownership rows are corrected;
the compiled object fingerprints remain unchanged.
