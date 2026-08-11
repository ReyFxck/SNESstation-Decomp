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
- `EQUIVALENT` — reconstructed behavior is supported, assembly differs or matching has not been established.
- `MATCHING` — generated machine code matches the reference under the documented toolchain/flags.

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
```

The script regenerates `docs/PROGRESS.generated.md` and the progress section in the root README. Do not hand-edit the generated scoreboard.
