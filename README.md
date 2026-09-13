<p align="center">
  <img src="assets/snes-station-logo.png" width="520" alt="Original SNES Station homebrew logo" />
</p>

# SNES Station Decompilation

Source recovery and exact-binary preservation of **SNES Station v0.23 WIP**
(24 January 2004) for PlayStation 2.

The repository rebuilds the original frontend, Snes9x-derived emulator core,
renderer, audio and filesystem code, and historical runtime from verifiable
evidence. It is a decompilation project, not a modern rewrite or a new emulator
release.

<!-- DECOMP_PROGRESS_START -->
## Current status

| Measure | Result | What it means | Status |
|---|---:|---|---|
| Audited function entries | **1,041/1,041 (100%)** | Every entry in the frozen audit has complete-boundary matching evidence and a readable source model. | Complete |
| EE source ownership | **97/97 translation units** | All recovered units compile with the historical EE ABI; 96 canonical objects form the duplicate-free source aggregate. | Complete |
| Runtime contracts | **53/53** | Every tracked PS2LIB, libc, libgcc and target-selected runtime dependency has an evidence-backed provider or refactor. | Complete |
| Address identities | **1,265/1,265** | Every tracked program-data address has a proved identity; exact full object bounds are a separate question. | Complete |
| Whole-image windows | **51/51 (100.00%)** | Every 64 KiB window in the unpacked image matches exactly. | Complete |
| Remaining byte differences | **0** | Byte positions still different in the 3,304,936-byte unpacked image. | Complete |
| SJCRUNCH2 container | **714,268/714,268 bytes** | All 13 LZO1X-999 level-8 blocks and the complete container match exactly. | Complete |
| Replacement ELF | **726,968/726,968 bytes** | Public SjCRUNCH 2.1 artifacts rebuild the wrapper; the complete packed SHA-256 matches. | Complete |

The **1,041/1,041** result measures the audited function frontier. The
whole-image and packed-ELF rows are the direct byte-identity measures; both are
now complete.

Detailed machine-generated counts are in
[`docs/status/PROJECT_STATUS.generated.md`](docs/status/PROJECT_STATUS.generated.md).
<!-- DECOMP_PROGRESS_END -->

## Build and verify

Repository-only checks need Python 3, GNU Make and a host C compiler:

```bash
make status
make check
```

For the complete implemented pipeline, place a legally obtained reference at
`original/SNES_EMU.ELF` and run:

```bash
make reference
make reproduce-check
```

`make reproduce` is the stable one-command pipeline. It verifies every public
and private gate, fetches the hash-pinned public SjCRUNCH 2.1 package and emits
the byte-identical replacement at `build/SNES_EMU.rebuilt.ELF`.

Build the isolated historical compiler without installing it system-wide:

```bash
make bootstrap-ee-stage1
```

See [`docs/TOOLS.md`](docs/TOOLS.md) for the maintained command and tool table.

## What has been recovered

| Area | Historical basis | Result |
|---|---|---|
| Emulator core | Snes9x 1.41 | Audited function set complete; exact historical data rebuilt where provenance is known |
| Compression | zlib 1.1.3 and unzip 0.15 | Deflate/inflate and legacy ZIP paths recovered |
| PlayStation 2 runtime | Early PS2DEV/PS2LIB | 53/53 runtime contracts resolved |
| Compiler runtime | GCC 3.2.2-era libgcc/libsupc++ | Arithmetic, RTTI, exceptions and unwind evidence integrated |
| Frontend and renderer | SNES Station binary and early Hiryu gsLib lineage | Application flow and 30/30 renderer draw-family entries recovered |
| Startup and image layout | Historical PS2 startup plus hash-only private oracle | Exact entry/startup and 51/51 whole-image windows reproduced |
| Executable compression | SJCRUNCH2 plus LZO1X-999 | Level 8 identified; 13/13 blocks and the 714,268-byte container reproduce exactly |
| Loader and packed ELF | Public SjCRUNCH 2.1 archive plus EE GCC/binutils 3.2.2/2.14 toolchain | Loader, metadata and complete 726,968-byte ELF reproduce exactly |

The complete source-history table, including preserved compiler candidates and
why they remain in the repository, is in
[`docs/RECOVERY_HISTORY.md`](docs/RECOVERY_HISTORY.md).

## Tools and identified versions

| Component | Version | Use |
|---|---|---|
| Ghidra | 10.4 PUBLIC | Static analysis |
| Ghidra Emotion Engine: Reloaded | 2.1.10 | R5900 processor support |
| EE GCC | 3.2.2 (`3.2.2-b1` leading candidate) | Historical code generation |
| EE binutils | 2.14 candidate | Assembly and linking |
| Python | 3.12 in CI | Audits, tests and report generation |
| LLVM objdump | 20 | Generic disassembly pass |
| SjCRUNCH | 2.1; LZO1X-999 level 8; miniLZO 1.08 stub | Original loader and executable packing |

Evidence levels, immutable hashes and deliberately unknown revisions are
listed in [`docs/DEPENDENCY_VERSIONS.md`](docs/DEPENDENCY_VERSIONS.md).

## Target identity

| Item | Value |
|---|---|
| Release | SNES Station v0.23 WIP, 24 January 2004 |
| Packed ELF SHA-256 | `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| Unpacked image SHA-256 | `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |
| Unpacked base / entry | `0x00100000` / `0x00100008` |
| Unpacked size | 3,304,936 bytes |

## Repository layout

| Path | Purpose |
|---|---|
| `src/` | Readable recovered source grouped by subsystem |
| `include/` | Recovered declarations and EE ABI compatibility |
| `analysis/` | Authoritative manifests, maps and immutable evidence |
| `matching/` | Isolated compiler candidates and exact comparison inputs |
| `tools/` | Maintained verification and reproduction implementation |
| `tools/history/` | Frozen one-off research still needed to reproduce old evidence |
| `docs/` | Current guides, history and technical references |
| `docs/archive/` | Superseded reports retained only for provenance |
| `third_party/` | Pinned historical material and provenance records |
| `original/` | Private reference supplied by the user; ignored by Git |
| `build/` | Generated toolchains, objects, reports and private extracts; ignored by Git |

## Documentation

- [`docs/README.md`](docs/README.md) — short documentation index
- [`docs/ROADMAP.md`](docs/ROADMAP.md) — closed definition of done and optional research
- [`docs/REPRODUCTION.md`](docs/REPRODUCTION.md) — exact reproduction process
- [`docs/RECOVERY_HISTORY.md`](docs/RECOVERY_HISTORY.md) — recovered code and historical sources
- [`docs/TOOLS.md`](docs/TOOLS.md) — commands, tools and versions
- [`docs/LEGAL.md`](docs/LEGAL.md) — distribution and provenance policy

## Reference and contribution policy

The original ELF, executable/media payloads and private target-byte ranges are
not distributed. The logo above is a documentation-only PNG rendering of the
homebrew's own 382x70 title asset; its provenance and narrow exception are
recorded in [`docs/LEGAL.md`](docs/LEGAL.md).

Changes must preserve the difference between readable reconstruction, exact
machine-code evidence and complete image identity. Run `make check` before
submitting a pull request. See [`CONTRIBUTING.md`](CONTRIBUTING.md) for the
evidence requirements.
