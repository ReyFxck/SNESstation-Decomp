# V84 reviewed source-address aliases

V84 extends the frozen V83 link-identity checkpoint without assigning any raw
virtual address. It proves 66 additional zero-byte relationships: 63 through
unique, owner-checked recovered naming conventions and three through explicit
semantic reviews backed by committed evidence. The cumulative result is
323/337 aliases bound to 307 canonical global text symbols.

Every accepted linker definition still has the form
`alias=canonical_symbol`. No trampoline, code, data or target byte is emitted.

## Reproducible gates

The public manifests and all review citations can be checked without an EE
compiler or private ELF:

```bash
make source-aliases-public-check
```

Build the Stage-2 aggregate and apply all proved aliases with the pinned
toolchain:

```bash
make source-aliases
```

With an existing compatible EE GCC 3.2.2 installation:

```bash
make source-aliases-check EE_CC=/absolute/path/to/ee-gcc
```

The partial-link gate verifies that the frozen 1,921 input externals are
present, removes exactly the 323 proved aliases, confirms equal alias/canonical
values and types, and compares every allocated section byte-for-byte.

## Cumulative result

| Measurement | Result |
|---|---:|
| Stage-2 source-address aliases | **337** |
| Proved aliases | **323** |
| Unique canonical global text targets | **307** |
| Still blocked | **14** |
| Aggregate unresolved externals | **1,921 -> 1,598** |
| Allocated section changes | **0** |
| Code/data bytes emitted for aliases | **0** |

## Accepted evidence

| Evidence class | Aliases | Rule |
|---|---:|---|
| Exact progress name | **122** | The audited target name is an owner-matched exported global text symbol. |
| Unique address suffix | **135** | Exactly one owner-matched exported symbol carries the target-address suffix. |
| `_recovered` suffix | **25** | The audited name plus the repository's explicit recovered suffix is unique. |
| `snes_` prefix | **26** | The uniquely exported compatibility spelling adds only the `snes_` prefix. |
| Stripped-leading-underscore `snes_` prefix | **7** | Runtime names such as `_Unwind_*` map uniquely to the repository compatibility spelling. |
| `snes_p13_` prefix | **5** | The uniquely exported Progress-13 compatibility spelling is owner-matched. |
| Explicit semantic review | **3** | A committed review row names the canonical symbol and a public evidence file/token. |

The two FDE helpers at `0x001a61c0` and `0x001a6240` already existed in
`gcc_fde_runtime_recovered.c`; V84 adds their missing address trace markers so
the generated Stage-2 ownership map can prove that source relationship. The
compiled object fingerprint does not change.

The three explicit reviewed identities are:

| Target alias | Canonical symbol | Decisive evidence |
|---|---|---|
| `FUN_00105898` | `S9xSyncSpeed` | The typed source-promotion row identifies the recovered callback contract. |
| `FUN_0010a840` | `apu_buffer_allocator_0010a840` | Historical S9x APU evidence distinguishes the allocator from the integration-only wrapper. |
| `FUN_001aab20` | `snes_class_type_info_do_upcast3` | The historical libsupc++ match identifies the protected three-argument upcast. |

All review rows and their checked citations live in
[`source_alias_reviews.tsv`](../../analysis/link_identity/source_alias_reviews.tsv).

## Remaining blockers

| Blocker | Rows | Why it remains undefined |
|---|---:|---|
| Address outside the audited target manifest | **6** | These values are used as function/data pointers but may be interior labels or non-entry addresses. |
| Historical archive member not exported by the aggregate | **7** | Two libgcc FDE entries and five libsupc++ EH/personality entries require archive-member selection, not a source alias. |
| Source boundary not equivalent | **1** | The table helper at `0x00142a78` models only an interior portion of the full historical `S9xGraphicsInit`. |

The 14 rows remain explicit `BLOCKED` entries. In particular, V84 does not bind
the six unmanifested addresses with `--defsym alias=0x...`, and it does not
pretend that a helper is the same entry as a larger historical function.

## Honest boundary

V84 closes the source-model name-normalization tranche, not all link identity.
Historical archive selection, program data, linker script, section placement,
relocations and object/library order remain for the next Stage-3 part. The V83
257/337 checkpoint remains preserved as the prior mechanically strict tranche.
