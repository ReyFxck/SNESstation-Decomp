# Recovered source tree

`src/` contains the readable behavioral/source-model reconstruction. Isolated
compiler experiments and exact assembly gates belong under `matching/`, not
here.

## Subsystems

| Directory | Contents |
|---|---|
| `app/` | Boot, frontend and main application flow |
| `ps2/` | EE runtime, GSLIB, RPC, FileIO, memory-card, controller and C++ support |
| `snes9x/` | Snes9x-derived memory, CPU/APU helpers and renderer |
| `unzip/` | Legacy PKZIP Implode/Explode, Reduce, Shrink and unzip API |
| `zlib/` | Recovered zlib 1.1.3 corridor |

Header and ABI declarations are kept under `include/`. Exact address-to-source
traceability is generated in `analysis/source_readiness.csv`.

## Source status

All 1,041 audited entries have a behavioral/source model, but that statement is
not equivalent to any of the following:

- every model is exact historical C/C++;
- every function is formally machine-code matching;
- translation-unit ownership and global types are frozen;
- a complete replacement ELF can be linked.

The large `ps2/progress28_structural_lift_recovered.c` file closes structural
coverage while preserving address markers. It should be split into subsystem
translation units only when a change preserves:

1. every audited address trace;
2. the source-promotion audit;
3. existing exact compiler matches;
4. intended global and object ownership.

## Naming

- `_recovered.c` means a binary-derived behavioral/source reconstruction.
- `progressNN_` in a filename records historical provenance, not current status.
- `snes_pNN_<address>` and similar address labels remain until a historical
  identity is independently proven.
- Readable source and exact assembly-only evidence must never be conflated.

## Verification

Run all repository-only source checks with:

```bash
make check
```

Host syntax parsing is deliberately independent per C file and is only a
portability/sanity gate. Historical EE compilation, exact function comparison
and whole-program linking are separate proof stages documented in
[`../docs/REPRODUCTION.md`](../docs/REPRODUCTION.md).

Historical source-recovery summaries are retained under
[`../docs/archive/checkpoints/`](../docs/archive/checkpoints/).
