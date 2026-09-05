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
make link-contracts     # reduce the live aggregate to the 233-provider frontier
make private-assets     # privately materialize 10 asset providers; leave 223
make provider-frontier  # close all 223 source-link names; leave 0 externals
make named-data         # verify the closed 54/54 Stage-3C ledger and ranges
make named-contracts    # verify the closed 212/212 Stage-3E ledger and ranges
make libgcc-contracts   # verify the closed 7/7 Stage-3D libgcc subtranche
make runtime-refactors  # prove four sprintf calls; runtime shims 1 -> 0
make runtime-members    # prove 43 contracts / 42 PS2LIB member texts
make runtime-overrides  # two selected runtime overrides; Stage 3D 53/53
make unnamed-data      # 872 minimum spans; local / CFG / deterministic prefix
make historical-data   # 49 typed intervals / 810542 exact bytes; EE C++ rebuild
make data-backing      # 1265/1265 address identities; complete bounds still open
make link-layout-probe # 179 fixed sections; compare the clean Stage-3G ET_EXEC
make startup-integration # exact entry + 276-byte historical crt0; app remains open
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
4. **Source-address aliases — 333/347 proved** — alternate address-shaped
   names bind to 317 canonical global text symbols with no allocated-byte
   changes; 14 evidence blockers remain.
5. **Zero-byte link contracts — 1,297/1,530 resolved** — 1,234 frozen-address
   data anchors and 63 semantic aliases reduce the aggregate to 233 real
   providers without allocating code or data.
6. **Private embedded assets — closed** — five reference-verified bundles
   provide ten symbols and 62,736 bytes, reducing the frontier to 223 while
   remaining ignored private build products.
7. **Source-link provider namespace — closed** — all 223 remaining contracts
   resolve through audited anchors, recovered-text aliases, typed compatibility
   storage (no runtime shims remain); the relocatable aggregate has zero
   undefined globals.
8. **Original Stage-3C named data — closed** — all 54 historical rows are
   adjudicated: 50 real target objects have exact private-reference
   fingerprints and four invented source adapters are removed. Forty
   non-asset ranges form 15 clusters covering 141,159 unique bytes, and 32
   compatibility stores are replaced.
9. **Original Stage-3E named contracts — closed** — all 212 historical rows
   are adjudicated: 165 target-backed ranges/data aliases are fingerprinted,
   23 names bind to recovered text, two target entries and two external
   addresses are proved, and 20 source-only adapters are removed. All seven
   zlib peers are closed and compatibility storage reaches zero.
10. **Stage-3D libgcc — closed** — four complete GCC 3.2.2 archive-member
   `.text` sections match 3,848 target bytes after 21 relocations; three
   target-absent compiler-libcall artifacts are removed from source.
11. **Stage-3D formatter refactor — closed** — four direct calls select
   `sprintf@0x0019e3d0`, removing the source-only `snprintf` dependency and the
   last runtime shim.
12. **Stage-3D PS2LIB member text — 43 more contracts closed** — 42 complete
   texts cover 12,964 bytes after 700 relocation masks, with pinned source and
   header dependency hashes. Incompatible puts/abort member candidates remain
   rejected; member data and final relocated values are separate.
13. **Stage-3D target overrides — closed** — 15 historical named calls select
   the recovered puts/abort implementations, which link to 104 raw-exact bytes.
   The original runtime contract ledger is 53/53, not a whole-archive pedigree.
14. **Unnamed data/address identities (Stage 3F)** — 872/1,265 contracts have minimum consumed
   spans: 693 local, 146 CFG and 33 bounded deterministic-prefix witnesses.
   Full-function/analyzer hashes bind these proofs; they are not runtime
   coverage or full object bounds. The independent historical-data gate
   compiles 49 typed source intervals and proves 810,542 bytes. The backing
   gate and later V99–V101 closures resolve all 1,265 address identities.
   Twenty-nine ROM offsets are closed source refactors, never image objects.
   The current link preserves 13,671 source relocations and proves 3,627 isolated
   address relocations. Historical rows consume 695,316 freshly compiled
   source bytes with no reference-extraction fallback. Full object bounds remain
   open even though the address frontier is closed.
15. **Clean Stage-3G link diagnostic — frozen** — the real aggregate produces
   an ELF32/R5900 `ET_EXEC`; 179/179 fixed sections land exactly and 155/155
   initialized payloads match. Twelve of 51 image windows match, while
   1,883,867 bytes and the entry still differ. See
   [`status/V102_CLEAN_STAGE3G_LINK_PROBE.md`](status/V102_CLEAN_STAGE3G_LINK_PROBE.md).
16. **Exact historical startup — integrated** — pinned PS2SDK `crt0.s` produces
   `_start`, `_exit` and `_root` at the exact target entry, with 276/276 bytes,
   27 relocations and startup BSS geometry proved. The first remaining
   difference is `0x00100114`; 39/51 image windows remain different. See
   [`status/V104_EXACT_STARTUP_INTEGRATION.md`](status/V104_EXACT_STARTUP_INTEGRATION.md).
17. **Final link** — select/integrate exact implementation objects, linker script,
   section addresses, object order and library order to
   reproduce the unpacked ELF image.
18. **Pack** — the correct SJCRUNCH2/LZO revision and parameters reproduce the
   packed container and stub.
19. **Final comparison** — section/layout reports and both unpacked and packed
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
The current clean Stage-3G link diagnostic is recorded in
[`status/V102_CLEAN_STAGE3G_LINK_PROBE.md`](status/V102_CLEAN_STAGE3G_LINK_PROBE.md).
The exact startup integration is recorded in
[`status/V104_EXACT_STARTUP_INTEGRATION.md`](status/V104_EXACT_STARTUP_INTEGRATION.md).
The closed Stage-3D libgcc checkpoint is recorded in
[`status/V91_STAGE3D_LIBGCC_CLOSED.md`](status/V91_STAGE3D_LIBGCC_CLOSED.md).
The subsequent formatter refactor and its private direct-call proof are in
[`status/V92_STAGE3D_SNPRINTF_REFACTOR.md`](status/V92_STAGE3D_SNPRINTF_REFACTOR.md).
The complete PS2LIB member-text recipes and two rejected candidates are in
[`status/V93_STAGE3D_RUNTIME_MEMBERS.md`](status/V93_STAGE3D_RUNTIME_MEMBERS.md).
The target-selected override closure and minimum unnamed-data access proofs are in
[`status/V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md`](status/V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md).
The section-relative backing ownership and isolated relocation proof are in
[`status/V95_SECTION_BACKED_DATA_ALIASES.md`](status/V95_SECTION_BACKED_DATA_ALIASES.md).
The branch/loop-aware extension and updated current counts are in
[`status/V96_CONTROL_FLOW_DATA_ACCESSES.md`](status/V96_CONTROL_FLOW_DATA_ACCESSES.md).
The closed Stage-3E checkpoint is recorded in
[`status/V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md`](status/V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md).
The preceding closed Stage-3C checkpoint is recorded in
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
