# SNESStation-Decomp

A preservation-oriented decompilation and reconstruction of **SNES Station v0.23 WIP (24 January 2004)** for the PlayStation 2.

The goal is not to make a new SNES emulator by replacing everything with modern code. The goal is to understand and preserve the original SNES Station program as faithfully as possible: its PS2 frontend, Snes9x-derived core, renderer, audio glue, filesystem code, timing decisions, quirks, and bugs.

> **Status:** active reverse engineering. The project is not yet buildable as a complete replacement ELF.

<!-- DECOMP_PROGRESS_START -->
## Decompilation progress

<p align="center">
  <img src="assets/progress.svg" width="720" alt="SNES Station v0.23 decompilation progress" />
</p>

> **100% means structural coverage of the audited 1,041-entry target universe.** It is not a claim of byte matching or an exact compiler function count. See [`docs/PROGRESS.generated.md`](docs/PROGRESS.generated.md) for the full accounting.

- **Matching:** 45.24%
- **Reconstructed:** **100.00%** (1,041/1,041 validated targets)
- **Mapped / identified:** **100.00%** (1,041/1,041 validated targets)
- **Source-form checkpoint:** **1,041 behavioral/source-model + 0 structural-pseudocode-only**
- **Complete replacement ELF:** **not yet**
- **Renderer draw family:** **100.0% reconstructed / 100.0% mapped**

The renderer-specific grid lives in [`docs/PROGRESS.generated.md`](docs/PROGRESS.generated.md); the build/matching audit lives in [`docs/SOURCE_COMPLETENESS.generated.md`](docs/SOURCE_COMPLETENESS.generated.md).
<!-- DECOMP_PROGRESS_END -->

## Target binary

The current reference binary identifies itself as:

```text
SNES-Station
v0.23 WIP
24th January 2004
Written by A.Lee (aka Hiryu)
```

Reference fingerprints:

| Item | Value |
|---|---|
| Packed ELF SHA-256 | `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| Packed entry | `0x01b00008` |
| Unpacked base | `0x00100000` |
| Unpacked entry | `0x00100008` |
| Unpacked size | `3,304,936` bytes |
| Unpacked SHA-256 | `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |

The binary itself contains the strings **`Snes9x`** and **`1.41`**, so Snes9x 1.41 is treated as the primary upstream baseline. Close-era Snes9x source is used only to validate names and structure *after* the binary gives enough independent evidence.

## Version ledger for future matching

An **unknown** entry is deliberate: it is safer than silently compiling against
a modern replacement. Exact blob hashes, evidence and matching consequences are
recorded in [`docs/DEPENDENCY_VERSIONS.md`](docs/DEPENDENCY_VERSIONS.md).

| Component used by SNES Station | Version / revision | Evidence status |
|---|---|---|
| SNES Station | `0.23 WIP`, 24 January 2004 | exact target identity |
| Snes9x core | `1.41` (not `1.41-1`) | exact target string + source-order validation |
| zlib | `1.1.3` | exact target version/copyright strings |
| Gilles Vollant unzip | `0.15` (1998) | exact target version string |
| Hiryu CDVD filesystem IOP module | `1.13` | exact embedded version string and IRX hash |
| SjPCM IOP module | `2.0` | exact embedded version string and IRX hash |
| AmigaMod IOP module | version unknown; `Vzzrzzn` build | exact embedded blob hash; no version string |
| Hiryu `gsLib` | unversioned early snapshot | binary-specific class/method/layout fingerprint |
| EE `libcdvd` client | early Hiryu/Sjeep family | paired RPC implementation; exact source revision unknown |
| Newlib `mathfp` | `1.10.0` | official-source fingerprint; selected seven-function EE corridor byte-matching |
| EE GCC application compiler | GCC `3.2.2`; external `3.2.2-b1` candidate label | strong code/object-layout fingerprint; exact PS2 patch level not proven |
| First source-available EE recipe | binutils `2.14`, GCC `3.2.2`, Newlib `1.10.0` | official PS2DEV root commit pinned and hash-verified; its BETA 3 GCC patch postdates this target |
| Reproducible compiler stage one | binutils `2.14` + GCC `3.2.2` C-only | archive/patch hashes pinned; x86_64 passes; native AArch64 artifact probes and reproduces `mathfp` on DroidSpaces |
| EE binutils assembler/linker | exact release unknown | link layout recorded; tool revision still unproven |
| linked `libgcc` / unwind / `libsupc++` | GCC `3.2.2-b1` / 3.2.2-era family | archive sizes and public offsets strongly fingerprinted |
| EE libc / allocator / stdio | old PS2LIB/Newlib-era snapshot | target behavior recovered; bundle revision unknown |
| `libpad` / `libmtap` | NEW `XPADMAN` / `XMTAPMAN` generation | exact RPC family; console-ROM revision varies |
| `libmc`, libkernel, SIF, FileIO, loadfile | old PS2DEV/PS2LIB generation | exact client ABI; archive/commit revision unknown |
| SJCRUNCH packer | SJCRUNCH2, stub built `Jul 12 2002` | exact container/stub date; packer and LZO revisions unknown |
| IOP-module compiler | GCC 2 family | embedded `gcc2_compiled.` markers; exact release unknown |
| ROM modules | `XSIO2MAN`, `XPADMAN`, `XMTAPMAN`, `XMCMAN`, `XMCSERV`, `LIBSD` | exact names; versions belong to the console ROM |

The official README additionally credits Gustavo Scotti-era PS2 kernel work and
the joypad library by `pukko`; neither credit exposes a unique source revision.

## What has been recovered so far

### PS2 / application side

- `_start` at `0x00100008`
- module-buffer loader at `0x00104e7c`
- `main` / boot flow beginning at `0x00104f18`
- IOP reset and ROM module loading sequence
- memory-card initialization path
- embedded module loading path
- GUI → ROM selection → core initialization → ROM load → SRAM load → emulation → cleanup → GUI loop
- PS2 memory-card save-path helpers

### Snes9x / renderer side

Recovered or strongly identified:

| Address | Function | State |
|---|---|---|
| `0x00151074` | `CMemory::Init` | recovered |
| `0x001513bc` | `CMemory::LoadROM` | reconstructed |
| `0x00183e04` | `ConvertTile` | recovered |
| `0x0018428c` | `DrawTile` | recovered |
| `0x001845a8` | `DrawClippedTile` | recovered |
| `0x00184a40` | `DrawTilex2` | recovered |
| `0x001851f4` | `DrawTilex2x2` | recovered |
| `0x00185510` | `DrawClippedTilex2x2` | recovered |
| `0x001859a8` | `DrawLargePixel` | recovered |
| `0x00185d8c` | `DrawTile16` | recovered |
| `0x001860a8` | `DrawClippedTile16` | recovered |
| `0x0018789c` | `DrawTile16Add` | recovered |
| `0x00188804` | `DrawTile16Sub` | recovered |
| `0x0018a6d4` | `DrawLargePixel16Add` | recovered |
| `0x0018bac0` | `DrawLargePixel16Sub1_2` | recovered |
| `0x0018c124` | `get_tree` (ZIP Implode) | recovered |
| `0x0018d918` | `explode` | recovered |
| `0x0018dd6c` | `huft_build` | recovered |
| `0x0018e4c0` | `unReduce` | recovered |
| `0x0018ea64` | `unShrink` | recovered |

The complete current renderer map is in [`docs/RENDERER_MAP.md`](docs/RENDERER_MAP.md).

### Legacy ZIP / zlib side

The old ZIP path and the embedded zlib corridor are now substantially recovered:

- PKZIP Implode/Explode, Reduce and Shrink methods;
- exact unzip 0.15 archive API;
- exact embedded zlib baseline: **1.1.3**;
- Deflate and Inflate public interfaces;
- Inflate block/codes/fast/Huffman decoder;
- Deflate longest-match engines and Huffman tree writer;
- CRC32, Adler32 and zutil wrappers.

The contiguous zlib map runs from `compress2 @ 0x00190700` through the return
of `adler32 @ 0x00198c54`. See [`docs/ZLIB_MAP.md`](docs/ZLIB_MAP.md).

## Current frontier

**Progress 17 closes the audited structural target universe at 1,041/1,041
(100.00%).** The former 1,137-target figure was a raw JAL-pattern proxy. An
exhaustive classification found 292 post-code data words that merely decode as
calls and 196 independently mapped real entries with no direct JAL hit:

```text
1,137 raw JAL targets - 292 rejected data patterns + 196 non-JAL entries
= 1,041 validated structural targets
```

The closure adds 49 real code-referenced targets and promotes the remaining 25
identified/partial entries. All 74 complete R5900 decompiles are committed with
per-function hashes and verbatim warning evidence. Seventeen are warning-free;
57 contain 65 classified warnings, chiefly absolute-global label overlap and
compiler-created unreachable blocks. The large snapshot loader at `0x001728d4`
retains a type-propagation warning and is deliberately recorded at lower
confidence. See [`docs/PROGRESS17.md`](docs/PROGRESS17.md).

The frontier is now source cleanup and proof: replace `DAT_*` labels and
placeholder types, migrate structural pseudocode into build-ready translation
units, reproduce the historical EE toolchain, and compare generated machine
code. **Matching remains 0.00%** until that byte-level work succeeds. See
[`docs/TOOLCHAIN_FINGERPRINT.md`](docs/TOOLCHAIN_FINGERPRINT.md) and the
step-by-step [`docs/MATCHING_WORKFLOW.md`](docs/MATCHING_WORKFLOW.md).

## Verify the checkpoint and start matching

The root Makefile separates structural coverage, source readiness, function
matching, whole-program linking and final packing instead of folding them into
one misleading percentage:

```bash
make check
make elf-status
```

The generated source audit currently classifies **802** entries in the earlier
behavioral/source-model checkpoint and **239** entries as Progress-16/17
structural pseudocode that still needs build-ready migration. The repository has
**88 recovered C translation units plus three isolated matching-candidate units**
(91 independently syntax-checked units in total), but no complete linked
replacement ELF. The seven-function `mathfp` corridor is now promoted to
`MATCHING`. See
[`docs/SOURCE_COMPLETENESS.generated.md`](docs/SOURCE_COMPLETENESS.generated.md).

With a legally obtained reference ELF, reproduce and verify the exact analysis
image:

```bash
make reference
```

For the first pinned library experiment, fetch the SHA-256-verified official
Newlib 1.10.0 source and compare the recovered seven-function `mathfp` corridor
with a historical EE compiler candidate:

```bash
make fetch-newlib
make fetch-ee-toolchain-recipe
make bootstrap-ee-stage1
make toolchain-info
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make toolchain-probe EE_CC="$EE_CC"
make match-get-tree EE_CC="$EE_CC"
make match-mathfp-listing EE_CC="$EE_CC"
make match-mathfp EE_CC="$EE_CC"
```

The public SNESticle Makefile supplies strong GCC 3.2.2-b1 flag and neighboring
link-family evidence. It does **not** prove SNES Station's exact linker script,
archive revisions or library order, so `make elf` deliberately remains blocked
until those facts and the remaining source migrations are recovered.
The isolated bootstrap builds no Newlib, C++, PS2SDK or final ELF. Its pinned
archives, exact hashes, modern-host compatibility scope, date conflict and
ARM64 validation status are documented in
[`docs/HISTORICAL_EE_TOOLCHAIN.md`](docs/HISTORICAL_EE_TOOLCHAIN.md).
The complete seven-function math result and its compiler/source caveats are recorded in
[`docs/MATCHING_PHASE4_MATHFP.md`](docs/MATCHING_PHASE4_MATHFP.md).

## Repository policy

This repository intentionally does **not** contain the original SNES Station ELF, unpacked executable image, embedded IRX binaries, or a full program disassembly.

To reproduce the analysis, provide your own legally obtained reference binary at:

```text
original/SNES_EMU.ELF
```

Then run:

```bash
bash tools/analyze.sh
```

Generated binary/disassembly files are ignored by Git.

## DroidSpaces / Debian setup

The analysis pipeline is designed to work on a Debian environment such as DroidSpaces:

```bash
apt update
apt install -y python3 binutils liblzo2-2 make gcc git patch
```

When `liblzo2` is installed outside the system loader path, point the unpacker
at it with `SNESSTATION_LZO_LIBRARY=/absolute/path/to/liblzo2.so.2`.

If PS2DEV is installed, the scripts prefer:

```text
mips64r5900el-ps2-elf-objdump
```

Otherwise an available LLVM objdump can be used for the generic MIPS pass and the included R5900 annotator fills the known EE-specific gaps.

## Decompilation discipline

A name is not accepted merely because a function resembles upstream Snes9x.

The workflow is:

1. Freeze the exact target hash.
2. Unpack SJCRUNCH deterministically.
3. Identify function boundaries, calls, globals, strings, and data flow from the binary.
4. Reconstruct the function in C/C++.
5. Use historical Snes9x or PS2 code only as **post-identification validation**.
6. Preserve PS2-specific differences instead of replacing them with newer implementations.
7. Mark functions as `TODO`, `IDENTIFIED`, `EQUIVALENT`, or `MATCHING` only when the evidence supports it.

Historical third-party SNES Station patches, including the 2006 Mega Man extension, are kept conceptually separate from primary identification work.

## Layout

```text
SNESStation-Decomp/
├── Makefile             reproducible audit/matching entry points
├── src/                 reconstructed C/C++
├── include/             recovered declarations / symbols
├── analysis/functions/  focused per-function assembly/pseudocode evidence
├── analysis/matching/   machine-readable function comparison corridors
├── analysis/            maps, xrefs and machine-readable analysis
├── docs/                findings, progress and research notes
├── tools/               unpacking/disassembly/analysis scripts
├── original/            user-supplied reference ELF goes here (ignored)
└── reference/           notes about historical comparison material
```

## How to help

Useful contributions include:

- identifying exact Snes9x 1.41 source snapshots;
- compiler / PS2SDK fingerprinting from 2003–2004;
- matching individual reconstructed functions;
- documenting PS2-specific renderer differences;
- matching recovered functions with the candidate EE GCC 3.2.2-b1 toolchain;
- identifying audio and IOP scheduling behavior;
- verifying symbols and structure offsets independently.

Please read [`CONTRIBUTING.md`](CONTRIBUTING.md) before assigning names to unknown functions.

## Historical references

- PS2-Home SNES Station discussion: `https://www.ps2-home.com/forum/viewtopic.php?t=3406`
- Snes9x project: `https://github.com/snes9xgit/snes9x`

## Legal / preservation note

This project is for preservation, interoperability, research, and documentation. No original SNES Station executable is distributed here. See [`docs/LEGAL.md`](docs/LEGAL.md) for repository policy and provenance notes.

## Progress 10 — 60% reconstruction checkpoint

Progress 10 reaches **685 / 1,137 reconstructed (60.25%)** and **739 / 1,137 mapped
(65.00%)** while keeping matching at **0.00%**. The checkpoint adds compact SjPCM,
AmigaMod, multitap, FDE-runtime, libc, CMemory metadata, and address-labelled target
leaf models. See [`docs/PROGRESS10.md`](docs/PROGRESS10.md).

## Progress 11 — earlier-core small-target recovery

Progress 11 reaches **725 / 1,137 reconstructed (63.76%)** and **779 / 1,137 mapped
(68.51%)**, with matching still at **0.00%**.  Forty code-referenced short targets
were reconstructed across frontend/RPC/controller glue, Snes9x memory helpers,
the cheat apply/restore corridor, special-bank lookups, renderer lookup/nibble
helpers and a CPU-state boundary leaf.  Fourteen aligned-data `jal` scanner hits
were explicitly rejected rather than counted.  See [`docs/PROGRESS11.md`](docs/PROGRESS11.md).

## Progress 12 — fixed transforms, slot runtime, and state I/O

Progress 12 reaches **745 / 1,137 reconstructed (65.52%)** and **799 / 1,137 mapped
(70.27%)**, with matching still at **0.00%**. Twenty additional real-code JAL
targets recover six Q15 transforms, seven slot/controller helpers, memory-card
and record persistence wrappers, framed output, cdfs path construction and two
state-transfer leaves. See [`docs/PROGRESS12.md`](docs/PROGRESS12.md).
