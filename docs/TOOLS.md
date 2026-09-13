# Tools and reproducible commands

The supported interface is the root `Makefile`. Run `make help` instead of
calling individual Python files unless you are developing a proof tool.

## Everyday commands

| Command | Purpose | Needs the private ELF |
|---|---|:---:|
| `make status` | Print the current audited counts | No |
| `make check` | Validate manifests, generated documentation, links, host syntax and unit tests | No |
| `make docs` | Regenerate the public progress files from the manifests | No |
| `make reference` | Verify and unpack `original/SNES_EMU.ELF` | Yes |
| `make reproduce-check` | Run every implemented public and private reproduction gate | Yes |
| `make reproduce` | Build and privately compare the complete byte-identical packed ELF | Yes |
| `make sjcrunch-packing-check` | Rebuild and compare all 13 compressed blocks and the complete container | Yes |
| `make sjcrunch-source` | Fetch and hash-check the public SjCRUNCH 2.1 archive | No |
| `make elf` | Build the final ELF after the exact container and toolchain are prepared | No |
| `make bootstrap-ee-stage1` | Build the pinned historical EE C compiler locally | No |
| `make bootstrap-ee-cxx-stage1` | Build its C and C++ variant | No |

## Maintained implementation tools

| Tool | Role | Called by |
|---|---|---|
| `tools/update_progress.py` | Generates the current status and public progress views | `make docs`, `make check` |
| `tools/verify_reference.py` | Verifies the legally obtained packed target | `make reference` |
| `tools/build_source_tree.py` | Compiles the frozen EE translation-unit set and checks ownership | `make source-tree` |
| `tools/compare_elf_functions.py` | Performs strict function and relocation-aware comparison | Matching targets |
| `tools/layout_oracle.py` | Checks packed/unpacked geometry and 64 KiB image hashes | `make layout-oracle` |
| `tools/code_windows.py` | Rebuilds and verifies the currently exact whole-image code windows | `make code-windows` |
| `tools/sjcrunch_pack.py` | Rebuilds the hash-frozen SJCRUNCH2 container with LZO1X-999 level 8 | `make sjcrunch-packing-check` |
| `tools/sjcrunch_outer_elf.py` | Fetches verified SjCRUNCH 2.1 inputs and rebuilds the exact loader and packed ELF | `make sjcrunch-source`, `make elf` |
| `tools/decompdev_report.py` | Produces the public Objdiff Report v2 artifact | `make decompdev-report` |
| `tools/bootstrap_ee_gcc_stage1.py` | Builds isolated binutils/GCC candidates from pinned sources | Compiler bootstrap targets |
| `tools/reproduce.sh` | Orders all implemented reproduction gates | `make reproduce-check`, `make reproduce` |

The remaining Python modules implement individual proof gates and are tested
by `make check`. Completed one-off research lives in `tools/history/`; it is
kept because current exact-image gates can rerun that evidence, but it is not
part of the normal interface.

## Version ledger

| Component | Version used or identified | Role |
|---|---|---|
| Python | 3.12 in GitHub Actions | Documentation, audits and proof tools |
| GNU Make | Host-provided | Single command interface |
| Ghidra | 10.4 PUBLIC | Static analysis |
| Ghidra Emotion Engine: Reloaded | 2.1.10 | R5900 processor support |
| Ghidra language | `r5900:LE:32:default` | Target disassembly/decompilation model |
| LLVM objdump | 20 | Generic disassembly pass |
| EE GCC | 3.2.2; `3.2.2-b1` is the leading application-compiler candidate | Historical R5900 compilation |
| EE binutils | 2.14 candidate | Assembly and linking |
| Snes9x | 1.41 | Primary emulator-core source baseline |
| zlib | 1.1.3 | Compression source baseline |
| Gilles Vollant unzip | 0.15 | Historical ZIP API baseline |
| SjCRUNCH | 2.1; LZO1X-999 level 8; miniLZO 1.08 stub | Original wrapper and packed-container format |

Exact hashes, confidence levels and unknown revisions are kept in
[`DEPENDENCY_VERSIONS.md`](DEPENDENCY_VERSIONS.md). Compiler flags and the
reason `3.2.2-b1` is still a candidate are documented in
[`TOOLCHAIN_FINGERPRINT.md`](TOOLCHAIN_FINGERPRINT.md).
