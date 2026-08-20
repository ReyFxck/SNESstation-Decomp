# HUNT1000+ V45 — historical GCC runtime corridor

V45 promotes **54** strict matches, moving the project from **725/1041
(69.64%)** to **779/1041 (74.83%)**. There are **262** audited targets left.

| Corridor | Strict matches | Reproducible input |
|---|---:|---|
| GCC 3.2.2 `libgcc.a` | 22 | pinned EE GCC 3.2.2 stage-one build |
| GCC 3.2.2 `libsupc++` | 28 | original `libsupc++` sources compiled by the pinned C++ front end |
| PGEN/PS2DEV historical objects | 4 | pinned PGEN and PS2DEV commits |

Every promoted row is independently recomputed from its candidate ELF object.
The gate requires relocation-normalized byte equality, zero differing bytes, no
unknown relocation types, and either an exact next-address boundary or verified
zero padding through the next audited target. Symbol identity and evidence
cardinality are pinned so an incidental anonymous byte collision cannot be
promoted.

The C++ bootstrap uses
[`gcc-3.2.2-cxx-modern-host.patch`](../tools/patches/gcc-3.2.2-cxx-modern-host.patch)
to express GCC's old conditional lvalue as the equivalent pointer selection and
dereference accepted by modern host compilers. This is a host-build
compatibility change; the generated EE target code remains subject to the exact
object comparison gate.

## Reproduce

Keep the verified 24 January 2004 ELF at `original/SNES_EMU.ELF` locally, then
run:

```sh
make hunt1000plus-v45-evidence EE_BUILD_JOBS=8
python3 tools/promote_match_evidence.py \
  analysis/matching/hunt1000plus-v45-validated-runtime.tsv \
  --label "HUNT1000+ V45 runtime"
python3 tools/promote_match_evidence.py \
  analysis/matching/hunt1000plus-v45-validated-historical.tsv \
  --label "HUNT1000+ V45 historical"
```

The first command rebuilds both evidence files. The two following commands are
non-mutating validation passes unless `--apply` is explicitly added.

Evidence:

- [`hunt1000plus-v45-validated-runtime.tsv`](../analysis/matching/hunt1000plus-v45-validated-runtime.tsv)
- [`hunt1000plus-v45-validated-historical.tsv`](../analysis/matching/hunt1000plus-v45-validated-historical.tsv)

Only the official SNES Station v0.23 WIP target is used. Later unofficial ELF
builds are outside this evidence chain, and no original ELF or proprietary IRX
is committed.
