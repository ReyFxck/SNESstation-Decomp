# V83 source-address alias checkpoint

V83 closes the mechanically provable part of the Stage-3 source-address alias
gate. The frozen Stage-2 aggregate contains 337 unresolved alternate names for
target functions that may already be exported by another recovered object.
This checkpoint proves 257 of those names against 242 canonical global text
symbols and binds them in a relocatable partial link without emitting code or
data.

This is not a final-address assignment and not a replacement ELF. Each linker
definition has the form `alias=canonical_symbol`; no raw target virtual address
is passed to the linker.

## Reproducible gates

The public manifest can be checked without the EE compiler or private ELF:

```bash
make source-aliases-public-check
```

Build the Stage-2 aggregate and apply the aliases with the pinned toolchain:

```bash
make source-aliases
```

With an existing compatible EE GCC 3.2.2 installation:

```bash
make source-aliases-check EE_CC=/absolute/path/to/ee-gcc
```

The link gate verifies that all 1,921 frozen Stage-2 externals are present in
the input, applies only reviewed `ee-ld -r --defsym` relationships, checks that
each alias and canonical symbol have the same output value and type, and then
compares every allocated ELF section byte-for-byte.

## Frozen result

| Measurement | Result |
|---|---:|
| Stage-2 source-address aliases | **337** |
| Proved aliases | **257** |
| Unique canonical global text targets | **242** |
| Still blocked | **80** |
| Aggregate unresolved externals | **1,921 -> 1,664** |
| Allocated section changes | **0** |
| Code/data bytes emitted for aliases | **0** |

The two accepted evidence classes are deliberately narrow:

| Evidence | Aliases | Rule |
|---|---:|---|
| Exact progress name | **122** | The audited target name is an exported global text definition owned by the frozen source row. |
| Unique address suffix | **135** | Exactly one other exported global text definition carries the same eight-digit target-address suffix and belongs to the frozen owner set. |

Several aliases may point to the same canonical symbol. That is why 257 proved
names collapse onto 242 targets; it is expected aliasing, not duplicate code.

## Explicit blockers

| Blocker | Rows | Next evidence needed |
|---|---:|---|
| Audited target name is not an exact exported symbol | **73** | Review recovered-name transformations, visibility and ABI compatibility in small semantic batches. |
| Address absent from the 1,041-row progress manifest | **6** | Prove whether the address is a real entry, an interior label or non-code before binding it. |
| Multiple global text candidates at one suffix | **1** | Resolve the `0x0010a840` ownership collision between `apu_buffer_allocator_0010a840` and `apu_buffers_init_0010a840`. |

Blocked rows remain undefined in the V83 aggregate. The gate fails if a future
manifest change silently reclassifies one, changes an owner, removes an input
external, changes an allocated section, or resolves anything beyond the proved
set.

## Committed and generated evidence

- [`source_address_aliases.tsv`](../../analysis/link_identity/source_address_aliases.tsv)
  records all 337 decisions, their exact public evidence, owner object,
  requester and blocker detail.
- [`tools/source_aliases.py`](../../tools/source_aliases.py) regenerates and
  validates the manifest and performs the zero-byte partial link.
- [`tools/test_source_aliases.py`](../../tools/test_source_aliases.py) freezes
  the classifier, ownership rule, output-symbol invariant and V83 counts.

The alias-resolved object, link map and JSON/Markdown reports are generated
under ignored `build/source-aliases/`. They contain no private reference bytes.

## Honest boundary

V83 proves symbol identity only for the accepted 257 relationships. It does
not prove the historical object order, section virtual addresses, data/rodata
placement, archive selection, linker script or packed output. Those remain the
rest of Stage 3 and Stage 4.
