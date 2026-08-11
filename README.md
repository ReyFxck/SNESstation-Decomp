# SNESStation-Decomp

A preservation-oriented decompilation and reconstruction of **SNES Station v0.23 WIP (24 January 2004)** for the PlayStation 2.

The goal is not to make a new SNES emulator by replacing everything with modern code. The goal is to understand and preserve the original SNES Station program as faithfully as possible: its PS2 frontend, Snes9x-derived core, renderer, audio glue, filesystem code, timing decisions, quirks, and bugs.

> **Status:** active reverse engineering. The project is not yet buildable as a complete replacement ELF.

<!-- DECOMP_PROGRESS_START -->
## Decompilation progress

> Percentages are evidence-based proxies, not a claim that the complete ELF has exactly 1,137 functions. See [`docs/PROGRESS.generated.md`](docs/PROGRESS.generated.md) for the measurement rules.

- **Matching:** 0.00%
- **Reconstructed:** **8.62%** (98 tracked targets)
- **Mapped / identified:** **11.43%** (130 tracked targets)
- **Renderer draw family:** **100.0% reconstructed / 100.0% mapped**

```text
🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩
🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩
```

🟩 reconstructed · 🟨 identified · 🟧 partial · ⬜ unknown · 🟦 matching

The renderer grid contains one square per tracked draw-family function, ordered by address.
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
| `0x001513bc` | `CMemory::LoadROM` | identified / mapped |
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

## Current frontier

The R5900 instruction set is **not** the main problem anymore. All `859/859` LLVM `<unknown>` instructions in the selected EE-code range are classified, and the tracked 30-function renderer draw family is now fully reconstructed.

The current binary frontier is the surrounding legacy `unzip.c` API after `0x0018f010`. A parallel matching-build track is now practical because a close PS2 source tree records **EE GCC 3.2.2-b1** and release flags, with a `get_tree` GAS listing whose opening machine words match the SNES Station target. Matching remains 0% until that compiler is reproduced and a complete byte comparison succeeds.

See [`docs/BOTTLENECKS.md`](docs/BOTTLENECKS.md), [`docs/UNZIP_MAP.md`](docs/UNZIP_MAP.md), and [`docs/TOOLCHAIN_FINGERPRINT.md`](docs/TOOLCHAIN_FINGERPRINT.md).

## Repository policy

This repository intentionally does **not** contain the original SNES Station ELF, unpacked executable image, embedded IRX binaries, or a full program disassembly.

To reproduce the analysis, provide your own legally obtained reference binary at:

```text
original/SNES_EMU.ELF
```

Then run:

```bash
./tools/analyze.sh
```

Generated binary/disassembly files are ignored by Git.

## DroidSpaces / Debian setup

The analysis pipeline is designed to work on a Debian environment such as DroidSpaces:

```bash
apt update
apt install -y python3 binutils liblzo2-2
```

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
├── src/                 reconstructed C/C++
├── include/             recovered declarations / symbols
├── analysis/functions/  focused per-function assembly extracts
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
