# From structural 100% to a reproducible ELF

The current 100% closes an audited **structural** target universe. It does not
mean that the repository already contains Hiryu's verbatim source tree or that
the original executable can be rebuilt. Those are later, independently
measurable claims.

## Four different completion claims

| Claim | Current state | Required evidence |
|---|---:|---|
| Structural coverage | **1,041/1,041** | Every validated entry has committed control-flow/behavior evidence. |
| Build-ready source ownership | **Closed: 97/97 TUs** | Every source compiles for the EE; 96 canonical objects partially link with frozen ABI/symbol ownership and one explicit alternate. |
| Function matching | **1,041/1,041** | A candidate object reproduces every non-relocation byte for each audited target row. |
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
3. **Freeze source ownership.** The Stage-2 manifest compiles every real
   translation unit with EE GCC 3.2.2, rejects duplicate/common definitions and
   records unresolved data/archive/link contracts without inventing providers.
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
| `0x001a045c..0x001a0693` | `atanf` | wrapper plus the non-`atan2` path of `sf_atangent.c` |
| `0x001a06a0..0x001a06a7` | `sqrtf` | target-specific EE `sqrt.s` leaf; byte-matching |
| `0x001a06b0..0x001a06b7` | `fabsf` | target-specific EE `abs.s` leaf; byte-matching |
| `0x001a06c0..0x001a073f` | `numtestf` | Newlib structure plus observed old `-mlong64` behavior |

The generic Newlib `sqrtf` and `fabsf` bodies are **not** copied blindly: the
target replaces them with Emotion Engine hardware instructions. Their actual
code ranges are eight bytes each; the following gaps are alignment, not
function bodies. The same boundary correction excludes 12 padding bytes after
`atanf`. All seven rows now match the committed target listing byte for byte.

The target also exposes an old-ABI/prototype mismatch in `numtestf`; the four
callers promote the argument while the callee reads a float from `$f12`. The
readable C model preserves that behavior. Since the surviving BETA 3 backend
does not reproduce the leaf's older instruction selection, the formal matcher
uses a clearly labeled assembly reconstruction for `numtestf`; it is not
presented as Hiryu's original source.

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
make fetch-ee-toolchain-recipe
make bootstrap-ee-stage1
make toolchain-info
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make toolchain-probe EE_CC="$EE_CC"
make match-get-tree EE_CC="$EE_CC"
make match-mathfp-listing EE_CC="$EE_CC"
make match-mathfp EE_CC="$EE_CC"
```

The fetch step pins and verifies the surviving 2004 PS2DEV recipe. The isolated
bootstrap then downloads hash-pinned GNU sources and builds EE binutils plus a
C-only GCC stage one under ignored `build/`; it does not install system files or
build Newlib, C++, PS2SDK or the final program. The probe rejects a wrong base
version, target tuple, unsupported R5900 flags or wrong ELF class/endianness
before a target comparison is attempted. See
[`HISTORICAL_EE_TOOLCHAIN.md`](HISTORICAL_EE_TOOLCHAIN.md).

The readable historical-source model for `get_tree` lives at
`matching/candidates/get_tree.c`. The surviving SNESticle EE3.2.2-b1 listing
compiles that source shape to 208 bytes, while the SNES Station target is 212
bytes with a different register allocation and one additional `lui` reload.
The formal matching candidate is therefore the clearly labelled
`matching/candidates/get_tree.S` reconstruction; its committed-listing report
is `analysis/matching/get-tree-listing-report.md`, while the reference-ELF report
remains under `build/matching/get_tree/report.md`. The assembly reconstruction
is not presented as Hiryu's original source.

`get_tree @ 0x0018c124` now closes **1/1** under the bit-precise committed-listing
strict gate. Its historical 208-vs-212-byte compiler fingerprint remains useful
for identifying the exact pre-target EE GCC build. The math corridor remains the
first pinned library experiment because its upstream source identity is unusually
strong; the two results answer different provenance questions.

## What whole-program reproduction still requires

The analysis scripts are reproducible for the work they claim: unpacking,
wrapping, call scanning, structural manifests, source audit and object
comparison. A complete rebuild pipeline still needs new evidence-driven pieces:

- exact startup objects and embedded-data/IRX placement;
- pinned EE compiler, assembler, linker and every selected archive member;
- the SNES Station linker script and object/library order;
- unpacked-load-image comparison and, later, SJCRUNCH2 repacking.

The source/object gate and maps are documented in
[`status/BUILD_READY_SOURCE_TREE.md`](status/BUILD_READY_SOURCE_TREE.md).
`make elf-status` prints the later gates. `make elf` intentionally fails until
they are closed; emitting a non-matching executable would not answer whether
the original program had truly been recovered.
