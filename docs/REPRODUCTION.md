# One-command exact reproduction

The project exposes one stable entry point:

```bash
make reproduce
```

The command is intentionally useful before final completion. It validates the
repository, verifies the private reference ELF and enters the final build gate.
Stage 2 now compiles and audits the complete source/object tree before the
private-reference checks. `make elf` still refuses to emit a pretend
replacement, so reproduction stops with a precise list of Stage-3/4
requirements. The interface does not need to change as those requirements are
implemented.

Useful modes:

```bash
make reproduce-status   # no private ELF required
make reproduce-check    # verify repository + private reference + blockers
make reproduce          # full pipeline; currently stops at final-link gate
make layout-oracle      # verify the private image against the public Stage-3 oracle
make source-aliases     # build Stage 2 and apply proved zero-byte aliases
make compare-unpacked CANDIDATE_RAW=/path/to/rebuilt.bin
```

## Proof ladder

An exact function count is only the first half of whole-program reproduction.
The final pipeline must prove every layer:

1. **Function code** — every audited entry has exact machine-code evidence.
2. **Translation units — closed** — 97/97 sources compile with the historical
   EE compiler; 96 canonical objects partially link with frozen ownership and
   no duplicate/common definitions.
3. **Unpacked layout oracle — closed** — one section, thirteen decompressed
   blocks and fifty-one 64 KiB hash windows freeze the target geometry and
   locate the exact first rebuilt-byte difference.
4. **Source-address aliases — 257/337 proved** — alternate address-shaped
   names bind to 242 canonical global text symbols with no allocated-byte
   changes; 80 evidence blockers remain.
5. **Program data** — globals, constants, string pooling, vtables, BSS and
   alignment match the unpacked target.
6. **Relocations and archives** — the exact old PS2 libraries and their member
   selection are known.
7. **Link** — linker script, section addresses, object order and library order
   reproduce the unpacked ELF image.
8. **Pack** — the correct SJCRUNCH2/LZO revision and parameters reproduce the
   packed container and stub.
9. **Final comparison** — section/layout reports and both unpacked and packed
   SHA-256 values match the frozen reference.

The required reference hashes are:

| Form | SHA-256 |
|---|---|
| Packed ELF | `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| Unpacked image | `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |

The Stage-2 evidence and its exact claim boundary are recorded in
[`status/BUILD_READY_SOURCE_TREE.md`](status/BUILD_READY_SOURCE_TREE.md).
The first Stage-3 measurement gate is recorded in
[`status/V82_UNPACKED_LAYOUT_ORACLE.md`](status/V82_UNPACKED_LAYOUT_ORACLE.md).
The first Stage-3 link-identity tranche is recorded in
[`status/V83_SOURCE_ADDRESS_ALIASES.md`](status/V83_SOURCE_ADDRESS_ALIASES.md).

## What “original code” can mean

A compiler erases comments, formatting and many source-level names. Therefore
no binary-only decompilation can prove that every line of recovered C/C++ is
textually identical to the author's lost source. The rigorous target is a
readable reconstruction that preserves known historical source where proven
and reproduces the executable bytes exactly.

Exact assembly candidates under `matching/` remain clearly labelled evidence;
they do not silently replace readable recovered source under `src/`.
