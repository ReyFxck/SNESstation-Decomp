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
resolves 323 of this aggregate's 337 source-address aliases. After V89 removes
four source-only Stage-3C adapters and V90 removes 20 source-only Stage-3E
contracts plus the duplicate `errno` external. V91 removes three compiler-libcall
artifacts introduced by the structural lift. V92 then removes the synthetic
`snprintf` dependency, so that checkpoint contained 1,892
externals and the alias-resolved map contained 1,569 contracts. It resolved
1,336 of them through 1,273 absolute address anchors and 63 semantic aliases.
Both partial-link gates leave every allocated section unchanged; the current
correction is documented in
[`V92_STAGE3D_SNPRINTF_REFACTOR.md`](V92_STAGE3D_SNPRINTF_REFACTOR.md).

V93 preserves these object fingerprints and namespace counts while refining
runtime ownership: 43 imports now identify their PS2LIB member-text recipe,
and the two rejected `puts`/`abort` candidates have an explicit runtime-override
gate. The 42 complete historical member texts are separate build products;
their default PS2LIB ABI must not be confused with the recovered-source ABI
above. See [`V93_STAGE3D_RUNTIME_MEMBERS.md`](V93_STAGE3D_RUNTIME_MEMBERS.md).

V94 again preserves every source-object fingerprint and the aggregate. It
classifies the two target-selected overrides as `recovered-runtime`, proves
their named incoming calls and 104 exact linked bytes, and closes the 53-row
runtime contract ledger. The new data audit proves minimum consumed spans for
705/1,265 unnamed contracts without using provisional lifted C types as object
sizes. Complete object bounds and final executable integration remain open.
See [`V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md`](V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md).

V95 preserves the same 97 translation units, source-object fingerprints,
defined/external ownership and namespace counts. Its downstream partial link
binds 886 unnamed absolute anchors to section-relative storage labels,
materializes 69,768 proved bytes and preserves all pre-existing allocated
sections and 12,078 source relocations to those names. The new backing ledgers
record physical range ownership without claiming original C object bounds.
The 379 unbacked addresses and whole-program integration remain open. See
[`V95_SECTION_BACKED_DATA_ALIASES.md`](V95_SECTION_BACKED_DATA_ALIASES.md).

V96 leaves these Stage-2 sources, ownership maps and object fingerprints
unchanged. Its CFG-aware access analysis raises the minimum-access ledger to
824 contracts, section backing to 961 addresses and the preserved relocation
roster to 12,434. Another 96,178 target-backed data bytes are materialized;
this is downstream data ownership, not a new claim that the behavioral source
aggregate is the final exact program. See
[`V96_CONTROL_FLOW_DATA_ACCESSES.md`](V96_CONTROL_FLOW_DATA_ACCESSES.md).

V97 intentionally changes four functions in the canonical Progress-28 source
to remove 29 ROM-relative pseudo-globals. The source-tree fingerprints and
defined/external ownership maps are refreshed: **1,863 source externals →
1,540 after aliases → 233 after contracts → 223 after assets → 0**.
The 97-TU / 96-canonical-object ownership gate remains closed. Sixteen typed
historical data intervals independently match 790,988 original bytes; the
downstream audit has 1,175 backed + 29 refactored contracts and 61 unresolved.
The entire source revision is not zero-byte; only its later rebinding step
preserves its corrected input bytes. See
[`V97_HISTORICAL_DATA_AND_ROM_OFFSETS.md`](V97_HISTORICAL_DATA_AND_ROM_OFFSETS.md).

V98 leaves the canonical 97-TU ownership/fingerprints and the live external
chain unchanged. It adds 33 historical provider intervals (49 total), closes
22 address-backing contracts and replaces private extraction of historical
backing with 695,316 freshly compiled source bytes. All 13,619 affected source
relocations and preexisting allocated input bytes survive the real link.
This is data-provider integration, not selection of the exact target text
implementations. See [`V98_SOURCE_DATA_INTEGRATION.md`](V98_SOURCE_DATA_INTEGRATION.md).

V99 through V101 keep the Stage-2 object contract intact while closing the
remaining Stage-3F address identities. V102 consumes the same real aggregate
in the first executable diagnostic: 179 fixed sections and 155 initialized
payloads are exact, but the entry and 1,883,867 bytes still differ. See
[`V102_CLEAN_STAGE3G_LINK_PROBE.md`](V102_CLEAN_STAGE3G_LINK_PROBE.md).

V104 leaves the 97-TU/96-object source ownership contract unchanged and links
the pinned historical startup as a separate exact object ahead of that
aggregate. Entry `0x00100008`, 276 startup bytes and 27 relocations are exact;
the first remaining application difference is `0x00100114`. See
[`V104_EXACT_STARTUP_INTEGRATION.md`](V104_EXACT_STARTUP_INTEGRATION.md).
