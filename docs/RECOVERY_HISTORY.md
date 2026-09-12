# Recovered code and historical origin

This page records what each recovered part of SNES Station is, where its
historical model came from and how its current claim is verified. It replaces
the old practice of using numbered update documents as the public project
history.

## Main recovered areas

| Area | Historical origin | What was recovered | Current result | Main proof |
|---|---|---|---|---|
| Application and frontend | SNES Station v0.23 binary | Boot flow, menu, input, file browser, configuration and frontend glue | All audited entries have source models and formal byte evidence | `make checkpoint-1041-check` |
| SNES emulation core | Snes9x 1.41 lineage | CPU/APU support, memory, DMA, renderer helpers and special-chip code | Function frontier complete; historical data rebuilt where source identity is known | `make check` |
| Renderer and PlayStation 2 GS | SNES Station binary plus early Hiryu `gsLib` lineage | Draw family, texture upload, GS state and frontend rendering | 30/30 audited draw-family entries reconstructed and mapped | `make check` |
| ZIP and compression | zlib 1.1.3 and Gilles Vollant unzip 0.15 | Deflate/inflate and legacy Shrink, Reduce, Implode and Explode paths | Audited functions complete; version strings and source fingerprints recorded | `make check` |
| PlayStation 2 runtime | Early PS2DEV/PS2LIB lineage | File I/O, RPC, controller, memory-card, kernel and C/C++ runtime providers | 53/53 runtime contracts resolved | `make runtime-overrides` |
| Compiler runtime | GCC 3.2.2-era `libgcc` and `libsupc++` | Arithmetic helpers, exception handling, RTTI and unwind metadata | 7/7 compiler-runtime contracts closed; selected tail metadata linked exactly | `make libgcc-contracts` |
| Startup | Historical PS2 `crt0.s` | `_start`, `_exit`, `_root`, entry point and startup BSS | 276/276 bytes, 3/3 functions and 27 relocations exact | `make startup-integration` |
| Public historical data | Pinned Snes9x, zlib, PGEN/gsLib and PS2 runtime source | Tables, constants, strings, vtables, unwind metadata and typed data intervals | 810,542 typed historical bytes plus a final 35,067-source-byte and 2,220-semantic-byte tranche that closes window 11 | `make window11-rodata-check` |
| Embedded frontend media | Hash-verified private reference | Frontend background/logo/panel, music and Memory Card icon | Six containers integrated; payload remains private and untracked | `make media-assets` |
| Whole-image code | Historical objects, recovered source and labelled exact assembly | Complete 64 KiB code regions | Windows 1–6 and 11–50 exact; 0 and 7–10 remain | `make code-windows` |
| Final executable | Historical link and SJCRUNCH2 packing | Exact object order, final relocations, linker script and packed container | In progress; 46/51 image windows exact | `make reproduce` |

## Preserved source candidates

These files are historical compiler inputs, not the normal recovered source
tree. Their former per-directory READMEs were consolidated here so the reason
for keeping each candidate is visible in one place.

| Candidate directory | Source lineage | Result it preserves |
|---|---|---|
| `matching/candidates/progress60/` | Early PS2SDK/PS2LIB source variants compiled with EE GCC 3.2.2 `-Os` | Ten exact function matches across unzip, FileIO, `libmc`, `libpad` and string helpers |
| `matching/candidates/progress61/` | NEW_PADMAN `libpad` variant | Exact `padInfoMode` at `0x001a8b24` |
| `matching/candidates/progress65/` | PS2LIB-era `libmc.c`, PS2SDK commit `01d625018c3fde3044292446c910b8ea508adfdc` | Six exact memory-card client functions |
| `matching/candidates/progress67/` | Historical NEW_PADMAN object layout | Five exact `libpad` functions and three corrected boundaries |
| `matching/candidates/progress68/` | PS2LIB-era `libpad.c` with the NEW_PADMAN selector | Exact 400-byte `padInit` function and independently verified zero-filled listing gaps |

The `.c.txt` suffix is intentional: these snapshots require the historical EE
headers and compiler, so the host C syntax pass must not treat them as ordinary
translation units. Immutable hashes and per-function results remain in
`analysis/matching/`.

## Evidence policy

- `src/` contains readable recovered source.
- `matching/` contains compiler experiments and exact byte candidates.
- `analysis/` contains the machine-readable manifests that define every count.
- `tools/history/` and `docs/archive/` preserve old investigations without
  presenting them as the current workflow.
- The original ELF and extracted private payloads are never committed. The
  README logo is the sole documentation-only exception described in
  [`LEGAL.md`](LEGAL.md).
