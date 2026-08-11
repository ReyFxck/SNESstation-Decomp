# Contributing

SNESStation-Decomp aims to preserve the original program rather than replace it with a modern approximation.

## Before submitting a symbol or reconstruction

Please include:

1. original virtual address;
2. evidence from callers/callees, strings, globals, argument use, or data layout;
3. confidence level;
4. whether an upstream source comparison was performed before or after identification;
5. whether the C/C++ is only behaviorally equivalent or has been assembly-compared.

Do not name a function solely because it resembles a function in Snes9x.

## Status labels

- `TODO` — boundary or purpose not sufficiently recovered.
- `IDENTIFIED` — purpose/name proven, C/C++ not complete.
- `RECONSTRUCTED` — structural/behavioral source evidence is committed, but compiler matching has not been established.
- `MATCHING` — generated machine code matches the reference after relocation normalization under a pinned toolchain, flags and source revision.

## Matching evidence

Before promoting a row to `MATCHING`, record the compiler binary/version, all
flags, candidate source revision, object hash, target range and the generated
comparison report. `tools/compare_elf_functions.py` masks relocation sites only
and never changes manifest status automatically. The first reproducible
library corridor is described in `docs/MATCHING_WORKFLOW.md`.

## Binary policy

Do not commit:

- `SNES_EMU.ELF` or other SNES Station binaries;
- unpacked executable images;
- embedded IRX binaries extracted from SNES Station;
- ROMs or BIOS files;
- full generated disassemblies of the complete executable.

Small per-function assembly excerpts needed to document reconstruction are acceptable in `analysis/functions/`.

## Historical patches

Third-party patches and later modified builds can be valuable validation material, but must not be used to silently bootstrap names into the independent symbol map. Record when a historical source was consulted.

## Updating progress

When a function changes state, update `analysis/progress_targets.csv` and run:

```bash
python3 tools/update_progress.py
make audit-source
make check
```

The generators refresh `docs/PROGRESS.generated.md`,
`docs/SOURCE_COMPLETENESS.generated.md`, `analysis/source_readiness.csv`, the
progress SVG and the root README scoreboard. Do not hand-edit generated files.
