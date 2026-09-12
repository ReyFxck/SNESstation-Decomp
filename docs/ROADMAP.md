# Roadmap to a byte-identical SNES Station ELF

The source and function audit is finished. The remaining work is whole-program
identity: close six image windows, reproduce the historical link and reproduce
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
| Whole-image comparison | **45/51 windows** | Windows 1–6 and 12–50 match the unpacked reference exactly |

## Remaining

| Priority | Work | Completion condition |
|---:|---|---|
| 1 | Close image windows **7–10** | Each 64 KiB hash equals the unpacked reference using proved source, relocations and explicitly labelled exact reconstruction where necessary |
| 2 | Finish image window **11** | Remove its remaining 2,460 differences and reproduce its complete data/code boundary |
| 3 | Close image window **0** | Integrate the remaining application code and layout around the already exact startup |
| 4 | Reproduce the final link | Exact linker script, section addresses, object/archive order, symbol binding and relocation results |
| 5 | Reproduce packing | Correct SJCRUNCH2/LZO revision, stub and parameters |
| 6 | Final comparison | Both unpacked and packed SHA-256 values match the frozen reference |

The current unpacked image still differs at **263,765 byte positions**. That
number and the 45/51 window count are generated from the current manifests;
they are not estimated percentages.

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
