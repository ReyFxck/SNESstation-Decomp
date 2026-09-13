# Roadmap to a byte-identical SNES Station ELF

The source/function audit and unpacked-image reconstruction are finished. The
remaining work is whole-program identity: reproduce the historical link and
the packed executable.

## Completed

| Work | Result | Why it matters |
|---|---:|---|
| Audited function entries | **1,041/1,041** | Every tracked entry has complete-boundary byte evidence and a source model |
| EE source ownership | **97/97 translation units** | The recovered source compiles into 96 canonical objects plus one explicit alternate |
| Source-link providers | **223/223** | The recovered aggregate has no undefined global providers |
| Runtime contracts | **53/53** | PS2LIB, libc, libgcc and target-selected runtime behavior is accounted for |
| Address identities | **1,265/1,265** | Every tracked program-data address has a proved identity |
| Startup | **276/276 bytes; 3/3 functions** | The target entry, startup code, 27 relocations and startup BSS are exact |
| Whole-image comparison | **51/51 windows; 0 differences** | The complete unpacked image matches its frozen SHA-256 |
| SJCRUNCH2 compression | **13/13 blocks; 714,268/714,268 bytes** | LZO1X-999 level 8 reproduces the complete compressed container exactly |

## Remaining

| Priority | Work | Completion condition |
|---:|---|---|
| 1 | Reproduce the loader stub and outer ELF | Rebuild the remaining 12,700 bytes from public source with exact headers, sections and BSS geometry |
| 2 | Reproduce the historical application link | Exact linker script, section addresses, object/archive order, symbol binding and relocation results |
| 3 | Final comparison | The complete packed SHA-256 matches the frozen reference |

The current unpacked image differs at **0 byte positions**. The 51/51 count is
generated from the current manifest; it is not an estimated percentage.

## Definition of done

| Artifact | Required SHA-256 |
|---|---|
| Packed `SNES_EMU.ELF` | `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| Unpacked image | `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |

`make reproduce` remains the single maintained entry point. Until every row
above is complete, it must stop at the first unproved operation rather than
constructing a plausible but unaudited replacement.

Historical proof reports are retained for reproducibility, but current work
should update the manifests and this direct scoreboard instead of creating a
new numbered status document.
