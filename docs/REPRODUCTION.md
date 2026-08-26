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
make link-contracts     # reduce the live aggregate to the 258-provider frontier
make private-assets     # privately materialize 10 asset providers; leave 248
make provider-frontier  # close all 248 source-link names; leave 0 externals
make named-data         # verify the closed 54/54 Stage-3C ledger and ranges
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
4. **Source-address aliases — 323/337 proved** — alternate address-shaped
   names bind to 307 canonical global text symbols with no allocated-byte
   changes; 14 evidence blockers remain.
5. **Zero-byte link contracts — 1,336/1,594 resolved** — 1,273 frozen-address
   data anchors and 63 semantic aliases reduce the aggregate to 258 real
   providers without allocating code or data.
6. **Private embedded assets — closed** — five reference-verified bundles
   provide ten symbols and 62,736 bytes, reducing the frontier to 248 while
   remaining ignored private build products.
7. **Source-link provider namespace — closed** — all 248 remaining contracts
   resolve through audited anchors, recovered-text aliases, typed compatibility
   storage or deterministic EE shims; the relocatable aggregate has zero
   undefined globals.
8. **Original Stage-3C named data — closed** — all 54 historical rows are
   adjudicated: 50 real target objects have exact private-reference
   fingerprints and four invented source adapters are removed. Forty
   non-asset ranges form 15 clusters covering 141,159 unique bytes, and 32
   compatibility stores are replaced.
9. **Remaining program data** — replace the remaining compatibility storage
   with exact globals, constants, string pooling, vtables, BSS, initializers
   and alignment matching the unpacked target.
10. **Relocations and archives** — replace compatibility shims with exact old
   PS2 library members and prove their selection.
11. **Link** — linker script, section addresses, object order and library order
   reproduce the unpacked ELF image.
12. **Pack** — the correct SJCRUNCH2/LZO revision and parameters reproduce the
   packed container and stub.
13. **Final comparison** — section/layout reports and both unpacked and packed
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
The closed Stage-3C checkpoint is recorded in
[`status/V89_STAGE3C_CLOSED.md`](status/V89_STAGE3C_CLOSED.md).
The preceding open audit remains recorded in
[`status/V88_STAGE3C_NAMED_DATA.md`](status/V88_STAGE3C_NAMED_DATA.md).
The preceding source-link namespace checkpoint remains recorded in
[`status/V87_PROVIDER_FRONTIER_CLOSED.md`](status/V87_PROVIDER_FRONTIER_CLOSED.md).
The preceding private-asset checkpoint remains recorded in
[`status/V86_PRIVATE_ASSET_PROVIDERS.md`](status/V86_PRIVATE_ASSET_PROVIDERS.md).
The preceding zero-byte link-contract frontier remains recorded in
[`status/V85_ZERO_BYTE_LINK_FRONTIER.md`](status/V85_ZERO_BYTE_LINK_FRONTIER.md).
The preceding reviewed address-alias tranche remains recorded in
[`status/V84_REVIEWED_SOURCE_ALIASES.md`](status/V84_REVIEWED_SOURCE_ALIASES.md).

## What “original code” can mean

A compiler erases comments, formatting and many source-level names. Therefore
no binary-only decompilation can prove that every line of recovered C/C++ is
textually identical to the author's lost source. The rigorous target is a
readable reconstruction that preserves known historical source where proven
and reproduces the executable bytes exactly.

Exact assembly candidates under `matching/` remain clearly labelled evidence;
they do not silently replace readable recovered source under `src/`.
