# One-command exact reproduction

The project exposes one stable entry point:

```bash
make reproduce
```

The command is intentionally useful before final completion. It validates the
repository, verifies the private reference ELF and enters the final build gate.
At present `make elf` refuses to emit a pretend replacement, so reproduction
stops with a precise list of unproven requirements. The interface does not need
to change as those requirements are implemented.

Useful modes:

```bash
make reproduce-status   # no private ELF required
make reproduce-check    # verify repository + private reference + blockers
make reproduce          # full pipeline; currently stops at final-link gate
```

## Proof ladder

An exact function count is only the first half of whole-program reproduction.
The final pipeline must prove every layer:

1. **Function code** — every audited entry has exact machine-code evidence.
2. **Translation units** — source ownership, symbol visibility and object
   boundaries reproduce the historical compilation units.
3. **Program data** — globals, constants, string pooling, vtables, BSS and
   alignment match the unpacked target.
4. **Relocations and archives** — the exact old PS2 libraries and their member
   selection are known.
5. **Link** — linker script, section addresses, object order and library order
   reproduce the unpacked ELF image.
6. **Pack** — the correct SJCRUNCH2/LZO revision and parameters reproduce the
   packed container and stub.
7. **Final comparison** — section/layout reports and both unpacked and packed
   SHA-256 values match the frozen reference.

The required reference hashes are:

| Form | SHA-256 |
|---|---|
| Packed ELF | `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487` |
| Unpacked image | `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b` |

## What “original code” can mean

A compiler erases comments, formatting and many source-level names. Therefore
no binary-only decompilation can prove that every line of recovered C/C++ is
textually identical to the author's lost source. The rigorous target is a
readable reconstruction that preserves known historical source where proven
and reproduces the executable bytes exactly.

Exact assembly candidates under `matching/` remain clearly labelled evidence;
they do not silently replace readable recovered source under `src/`.
