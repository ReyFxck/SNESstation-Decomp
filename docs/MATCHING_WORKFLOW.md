# From structural 100% to a reproducible ELF

The current 100% closes an audited **structural** target universe. It does not
mean that the repository already contains Hiryu's verbatim source tree or that
the original executable can be rebuilt. Those are later, independently
measurable claims.

## Four different completion claims

| Claim | Current state | Required evidence |
|---|---:|---|
| Structural coverage | **1,041/1,041** | Every validated entry has committed control-flow/behavior evidence. |
| Build-ready source | **Incomplete** | Typed C/C++, declarations, globals and translation-unit ownership compile together for the EE. |
| Function matching | **0/1,041** | A candidate object reproduces every non-relocation byte for a target function. |
| Replacement ELF | **Not available** | All objects, historical archives, linker script/order and binary layout reproduce the unpacked target. |

The generated [`SOURCE_COMPLETENESS.generated.md`](SOURCE_COMPLETENESS.generated.md)
and [`analysis/source_readiness.csv`](../analysis/source_readiness.csv) keep the
first two claims separate row by row.

## The proof ladder

1. **Freeze the reference.** `make reference` unpacks a legally obtained
   `original/SNES_EMU.ELF`; `tools/verify_reference.py` checks both immutable
   SHA-256 values, sizes, entry points, load base and wrapper payload.
2. **Audit the structural universe.** `make audit-source` verifies that both
   manifests agree and that all Progress-16/17 pseudocode markers have a
   corresponding validated entry.
3. **Migrate source.** Replace placeholder `DAT_*`, `undefined*` and guessed
   signatures with reviewed types and real translation units. A host syntax
   check is useful, but it is not an EE build or a correctness proof.
4. **Reproduce one object.** Compile a small corridor with a recorded compiler,
   exact flags and a pinned source revision. Do not start by linking the whole
   program.
5. **Compare functions.** `tools/compare_elf_functions.py` reads real ELF symbol
   and relocation tables. It masks only relocation bytes on both sides; size
   and every other byte must agree. It writes a report and never promotes a
   manifest row automatically.
6. **Recover the link.** Once objects match, determine the original linker
   script, section placement, archive member selection, object order and
   library order. Compare the resulting **unpacked** ELF/load image first.
7. **Recover packing.** SJCRUNCH2 reproduction is the final container step. A
   matching unpacked program is still distinct from a byte-identical packed
   release file.

## Why the Newlib math identification is not a guess

The target corridor at `0x0019fddc..0x001a073f` preserves the Newlib 1.10.0
`mathfp` Cody–Waite coefficients, branch order and function order:

| Target range | Function | Upstream evidence |
|---|---|---|
| `0x0019fddc..0x001a0023` | `cosf` | `sf_cos.c` + inlined `sf_sine.c` |
| `0x001a0024..0x001a0253` | `sinf` | `sf_sin.c` + inlined `sf_sine.c` |
| `0x001a0254..0x001a045b` | `tanf` | `sf_tan.c` coefficients and reduction flow |
| `0x001a045c..0x001a069f` | `atanf` | wrapper plus the non-`atan2` path of `sf_atangent.c` |
| `0x001a06a0..0x001a06af` | `sqrtf` | target-specific EE `sqrt.s` leaf |
| `0x001a06b0..0x001a06bf` | `fabsf` | target-specific EE `abs.s` leaf |
| `0x001a06c0..0x001a073f` | `numtestf` | Newlib structure plus observed old `-mlong64` behavior |

The generic Newlib `sqrtf` and `fabsf` bodies are **not** copied blindly: the
target replaces them with Emotion Engine hardware instructions. The target
also exposes an old-ABI quirk in `numtestf`. These differences are modeled in
`src/ps2/newlib_mathfp_recovered.c` and remain unproven until compiled output is
compared.

The official source archive is intentionally fetched rather than vendored:

```bash
make fetch-newlib
```

That command downloads
`https://sourceware.org/pub/newlib/newlib-1.10.0.tar.gz`, requires SHA-256
`69b62ad4c746a9acaf4f898772549f6da49f228f83a95efce7e88ae1d88c5a84`,
and safely extracts the historical `mathfp` source under
`build/upstream/newlib-1.10.0/`.

## Compiler and Makefile evidence

The public SNESticle PS2 Makefile at commit
[`9590ebf3bf768424ebd6cb018f322e724a7aade3`](https://github.com/iaddis/SNESticle/blob/9590ebf3bf768424ebd6cb018f322e724a7aade3/SNESticle/Project/ps2/Makefile)
records EE GCC `3.2.2-b1`, the R5900 flags reproduced by this repository's root
Makefile, and this historical link family:

```text
-nostartfiles -T../linkfile -Wl,-Map,SNESticle.map
-lmc -lpad -lps2ip -lkernel -lc -lm -lgcc -lstdc++
```

This is strong neighboring-project evidence because shared ZIP functions have
the same frame/code shape. It is **not** proof that SNES Station used the exact
same source list, linker script, archive revisions or library order. The root
Makefile therefore exposes those values as a reference and refuses to invent a
full `elf` recipe.

Use the candidate compiler like this:

```bash
make toolchain-info
make match-mathfp EE_CC=/absolute/path/to/ee-gcc
```

The report is written to `build/matching/mathfp/report.md`. A strict CI-style
run is available as `make match-mathfp-strict`; it exits unsuccessfully until
all seven rows really match.

`get_tree` at `0x0018c124` is still the simplest first compiler fingerprint
because a historical SNESticle assembly listing exists for it. The math
corridor is the first pinned library experiment because its upstream source
identity is unusually strong. Both experiments are useful and answer different
questions.

## What “all scripts are complete” would require

The analysis scripts are reproducible for the work they claim: unpacking,
wrapping, call scanning, structural manifests, source audit and object
comparison. A complete rebuild pipeline still needs new evidence-driven pieces:

- migration of 239 structural-pseudocode entries into build-ready units;
- a global/type ownership map that prevents duplicate or missing storage;
- exact startup objects and embedded-data/IRX placement;
- pinned EE compiler, assembler, linker and every selected archive member;
- the SNES Station linker script and object/library order;
- unpacked-load-image comparison and, later, SJCRUNCH2 repacking.

`make elf-status` prints these gates. `make elf` intentionally fails until they
are closed; emitting a non-matching executable would not answer whether the
original program had truly been recovered.
