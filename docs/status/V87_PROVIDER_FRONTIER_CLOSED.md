# V87 complete source-link provider closure

> Historical pre-refactor checkpoint. V89 removes four invented Stage-3C
> source contracts and regenerates the live closure at 248 names. See
> [`V89_STAGE3C_CLOSED.md`](V89_STAGE3C_CLOSED.md).

V87 closes the complete 251-name frontier left by the V86 private-asset gate
in one audited batch. The historical EE compiler builds deterministic
compatibility storage/runtime shims, the linker applies reviewed address and
text aliases, and the final relocatable aggregate is required to contain
**zero undefined global symbols**.

This is a source-link namespace checkpoint, not a replacement-ELF claim.
Compatibility storage has the complete minimum extent consumed by the current
behavioral source but deliberately uses zero initializers; runtime shims are
linkable implementations but deliberately do not claim historical archive
membership. Exact target initializers, section placement and archive members
remain later gates.

## Reproducible gates

The compiler/reference-free manifest check is:

```bash
make provider-frontier-public-check
```

With a legally obtained `original/SNES_EMU.ELF`, run the complete dependency
chain and source-link proof with:

```bash
make provider-frontier
```

To reuse an existing EE GCC 3.2.2 installation after `make reference`:

```bash
make provider-frontier-check EE_CC=/absolute/path/to/ee-gcc
```

Generated C, assembly, objects, maps and reports remain below ignored
`build/provider-frontier/`. The tracked manifest contains names, resolution
kinds, audited target/canonical identities and compatibility sizes only.

## One-batch accounting

| Input provider class | Rows |
|---|---:|
| Named link contracts | **197** |
| Named program-data storage | **35** |
| V84 source-address blockers | **14** |
| Historical archive contracts | **5** |
| **V86 input frontier** | **251** |

| Resolution mechanism | Rows | Bytes emitted by this mechanism |
|---|---:|---:|
| Target-address anchors | **181** | **0** |
| Aliases to recovered global text | **9** | **0** |
| Typed compatibility storage | **44** | **185,694 logical bytes** |
| Deterministic EE runtime shims | **17** | compiler-generated |
| **Total** | **251** | — |

Alignment between the 44 storage definitions makes the generated BSS section
185,728 bytes. The link report fingerprints all generated material sections
and proves every section inherited from V86 is byte-identical.

## What the four mechanisms mean

- **Target-address anchors** retain literal identities already encoded by the
  low-level lift, including the 14 V84 `FUN_XXXXXXXX` blockers. Two reviewed
  P12 helpers use exact call addresses, and `REG_GS_CSR` binds the documented
  privileged GS address. These anchors emit no bytes.
- **Recovered-text aliases** bind alternate source-model names such as
  `S9xSync_AudioStep`, the P12 file-I/O adapters and `abort` to existing global
  definitions already carried by the aggregate.
- **Compatibility storage** gives every remaining named program global and
  lifted local artifact a real, non-common definition with its consumed type
  extent. Known target addresses are retained as evidence, but the generated
  storage is intentionally not claimed as target-initialized or target-placed.
- **Runtime shims** implement the 128-bit shifts, float conversion, formatting
  adapter, math predicates, R5900 intrinsic wrappers and six inline PS2 kernel
  contracts introduced by buildable behavioral source. They make the source
  aggregate executable/linkable without pretending to identify an old archive
  member that has not yet been proved.

## Verified result

| Measurement | Result |
|---|---:|
| V86 input externals | **251** |
| Classified/resolved contracts | **251/251** |
| Output externals | **0** |
| Existing allocated-section changes | **0** |
| Compatibility storage symbols | **44** |
| Runtime shim symbols | **17** |

The gate also rejects an input undefined set that differs by even one name,
unexpected globals or externals in the generated provider object, wrong
absolute-anchor values, semantic aliases that do not share their canonical
value, and any mutation to a pre-existing allocated section.

## Remaining exact-ELF work

The **source-link provider frontier is closed**. Stage 3 as a whole is not yet
complete because exact target identity still requires:

1. replacing the 44 compatibility definitions with proved data/rodata/BSS
   initializers, overlap relationships, alignment and target placement;
2. replacing compatibility runtime shims where applicable with exact
   historical archive members and proving library revisions;
3. recovering the linker script, relocations, string pooling, object/library
   order and matching the unpacked SHA-256; and
4. reproducing SJCRUNCH2/LZO packing for the packed SHA-256.

So V87 removes the last undefined-name blocker without collapsing those later
evidence gates into a false “ELF complete” claim.
