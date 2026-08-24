# Progress 29 — historical EE gate and first libgcc matching frontier

Progress 28 closed the audited behavioral/source-model backlog at 1,041/1,041.
Progress 29 starts the next proof layer instead of pretending that host-valid C
is already a target rebuild.

## 1. Historical EE translation-unit baseline

`make ee-source-scan` runs every current C translation unit through the
bootstrapped EE GCC 3.2.2 front end with the project R5900 flags.  It is a
diagnostic scan, not a link:

- `PASS` means the historical compiler accepted the unit's C front end.
- `MISSING_HEADER` separates the expected stage-one lack of Newlib/PS2SDK
  headers from source-language failures.
- `EE_C_ERROR` means the source itself still needs old-GCC/target cleanup.
- `COMPILER_CRASH` is treated as a hard toolchain diagnostic.

The report is written under `build/ee-source-scan/`.  The non-strict target
returns success after producing the complete matrix; `ee-source-scan-strict`
fails until every unit passes.

## 2. Twelve-function GCC unwind fingerprint

The first post-math matching frontier is deliberately compact and strongly
identified.  `analysis/matching/libgcc_unwind_leaves.csv` covers:

- `size_of_encoded_value`
- `base_of_encoded_value`
- `read_uleb128`
- `read_sleb128`
- `_Unwind_GetLanguageSpecificData`
- `_Unwind_GetRegionStart`
- `_Unwind_GetDataRelBase`
- `_Unwind_GetTextRelBase`
- `_Unwind_GetGR`
- `_Unwind_SetGR`
- `_Unwind_GetIP`
- `_Unwind_SetIP`

The candidate is header-free so it can be compiled by the isolated stage-one
compiler.  The four context-base getters and Get/SetIP are particularly useful
compiler fingerprints because their target bodies are tiny and their offsets
are already proven by the committed disassembly/source models.

Run the local committed-listing experiment first:

```bash
EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make toolchain-probe EE_CC="$EE_CC"
make match-libgcc-unwind-listing EE_CC="$EE_CC"
```

Then run the formal original-ELF gate:

```bash
make match-libgcc-unwind EE_CC="$EE_CC"
```

Neither target edits matching status automatically.  A function becomes
`MATCHING` only after the generated object and target agree in size and every
non-relocation byte.

## 3. Honest ELF status

The Progress-16/17 pseudocode backlog is no longer an ELF blocker: it is zero.
The remaining blockers are target-build quality and provenance—EE-compatible
types and declarations, global/translation-unit ownership, startup/data
placement, exact archive membership, linker script/order, and finally packing.
