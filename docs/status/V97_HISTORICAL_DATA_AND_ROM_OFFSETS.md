# V97 — historical data providers and ROM-relative contracts

Exact delivery base: `d432effb8f844492d0d15315fd481a7563d1a0e3` (V96).
No commit or push is performed by the delivery. This checkpoint is cumulative
over that base and does not require any unpublished draft.

## Result and scope

| Measurement | V96 | V97 |
|---|---:|---:|
| Audited Stage-3F contracts | 1,265 | 1,265 |
| Physically section-backed addresses | 961 | 1,175 |
| Closed ROM-offset source refactors | 0 | 29 |
| Still unresolved | 304 | 61 |
| Minimum-access witnesses | 824 | 872 |
| Unique accessed bytes | 167,521 | 167,659 |
| Materialized bytes beyond Stage-3C/E/assets | 165,946 | 744,892 |
| Source-tree live externals | 1,892 | 1,863 |

Thus **243 of the 304 former blockers are resolved**: 214 gain proved physical
storage and 29 cease to be image-address contracts. This is not 243 new exact
functions, nor a final emulator ELF. The historical 1,265-row audit keeps the
29 closed records explicitly, while the live namespace correctly removes them.

The existing function checkpoint stays 1,041/1,041, Stage 3D stays 53/53 and
the source tree stays 97 TUs / 96 canonical objects plus one alternate.
The actual partial-link chain is **1,863 → 1,540 → 233 → 223 → 0** externals.

## Historical source providers

[`historical_data.json`](../../analysis/link_identity/historical_data.json)
records 16 exact intervals totaling **790,988 bytes**:

- One complete `GLOBALS.CPP` initialized-data section: `0x00345060`, size
  `0xaf848`, with **16 distinct named anchors** (20 cross-unit witnesses).
- Seven typed `snaporig.cpp` buffers: `OrigPPU`, `OrigDMA`, `OrigRegisters`,
  `OrigCPU`, `OrigAPU`, `OrigSoundData` and `OrigAPURegisters`.
- Eight typed `APU.CPP` objects, including the envelope-rate tables.

These are rebuilt from pinned Snes9x 1.41-1 and Newlib headers, using the
already reviewed V52 PS2 source profile and EE GCC 3.2.2. Every included
header/source is dependency-hashed. Fresh temporary recipe trees are used;
preexisting matching trees are not modified. Absolute temporary paths and
debug-section object hashes are deliberately not portable evidence.

Placement is constrained by four strictly matched functions: `ReadBlock`,
`ReadOrigSnapshot`, `S9xResetAPU` and `S9xFixEnvelope`. Their selected text
relocations are solved and reapplied, not merely ignored by a mask.
HI16/LO16 pairs follow relocation-table order, which need not be instruction
order. Conflicting provider addresses, unknown relocations, unresolved pairs,
data relocations in a selected owner, changed extents or differing bytes fail.

All complete intervals equal the original private bytes. After subtracting
already-owned Stage-3C/E and asset ranges, they contribute 675,772 bytes in
22 nonoverlapping new sections. Additional minimum-access sections bring
the downstream total to 75 added sections / 744,892 bytes, or **578,946 more
bytes than V96**. The 66 preexisting sections remain unchanged.

No executable bytes or assets are published. The public files contain
hashes, geometry, source recipes and symbolic evidence. The ordinary backing
link uses privately verified reference bytes; the independent historical
rebuild proves that its selected typed intervals can be reproduced from source.

## ROM-offset correction

[`rom_offset_refactors.tsv`](../../analysis/link_identity/rom_offset_refactors.tsv)
tracks 29 misleading `DAT_`/`UNK_` names used by `LoadROM`, `InitROM`,
`SuperFXROMMap` and `ApplyROMFixes`. These values are byte offsets relative to
the separately loaded ROM, or a ROM size limit, not objects at those numerical
addresses in the EE application image.

The canonical recovered source now uses byte-addressed `P28_ROM_AT` with
explicit 32-bit EE address arithmetic. The `0x00400001` check is an unsigned
integer size comparison. The two patch tests read unsigned bytes `0xd0` and
`0xb2`, then write `0xea`; the original instructions are `LBU`, not 64-bit
loads or negative integer comparisons. Four complete original-function
hashes and immutable matching evidence constrain these reviews.

**Numerical overlap is not ownership.** Several former ROM names happen to
fall numerically inside the snapshot buffers. They are explicitly excluded
before section-ownership classification; they are not counted as storage-backed
addresses or left behind as absolute symbols.

Four canonical source-function sizes and the containing object's fingerprints
change intentionally. Do not describe the entire V97 change as zero-byte.
The later alias/rebinding steps preserve the bytes of their corrected input.

## Additional access analysis

Partial unaligned loads invalidate only their destination register, and
partial stores preserve GPRs; neither is assigned a guessed access width.
The bounded deterministic-prefix analyzer starts at the real function entry,
follows only fully known integer branches, and records individual loop
occurrences. It stops at unknown branches, calls and unmodelled instructions.
Budget exhaustion discards the prefix proof. It never treats a first iteration
as a loop invariant, guesses memory values, or assumes callee register contents.

The ledger has 693 block-local, 146 fixed-point CFG and 33 deterministic-prefix
witnesses. Full function/analyzer hashes and the prefix execution occurrence
are frozen. The 364 names without an access witness are not synonymous with
the 61 without backing: other independently proved owners cover many of them.

## Reproduction and verification

```sh
make check
make reference
make bootstrap-ee-stage1
make bootstrap-ee-cxx-stage1
make runtime-overrides-check provider-frontier-check named-data-check data-backing-check \
  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc"
make layout-oracle-check
```

Public-only entry points are `rom-offsets-public-check`,
`historical-data-public-check`, `unnamed-data-public-check` and
`data-backing-public-check`. They do not need the original or EE toolchain.
`make historical-data` bootstraps C++ if needed and performs a fresh source
rebuild; `historical-data-verify` only checks private reference fingerprints.
Only explicit `capture`/`*-refresh` actions regenerate reviewed manifests.

Verification on the delivery host: **449 unit tests, host syntax 108/108,
historical C/C++ bootstrap, all relevant private/EE gates**. The rebind step
preserves **13,511 source relocations**; its isolated probe verifies 1,175
pointers and 1,175 HI16/LO16 pairs, **3,525 relocations total**. The probe is
not emulator code. This run is on Linux x86_64, not a new AArch64 hardware test.

## Recoverable next-work checkpoint

Base, manifests, implementation, tests and this document are included in the
single-script patch. Its checksum authenticates the payload. The installer
rejects a different base or dirty worktree, accepts relative invocation and
supports `--verify-only` after an interrupted check. It makes no Git commit.

Current blockers: **61 address contracts**, complete bounds of the remaining
objects/arrays, exact function implementation selection, historical member
data and archive composition, final global relocations, object order, linker
layout and SJCRUNCH2/LZO packing. No final hash is claimed.

Next useful investigations are the remaining DSP/C4 tables, runtime constants,
and text-pointer contracts incorrectly grouped with data. Prove their source
identities and boundaries individually; do not infer an entire array from a
single load, copy arbitrary reference ranges, or turn a code pointer into data.
The exact unresolved roster is generated by:

```sh
python3 -c 'import csv; print("\n".join(r["symbol"] for r in csv.DictReader(open("analysis/link_identity/data_backing.tsv"), delimiter="\t") if r["status"] == "NO_PROVED_BACKING"))'
```

Ultimate targets remain the raw-image SHA-256
`739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`
and packed-ELF SHA-256
`4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487`.
