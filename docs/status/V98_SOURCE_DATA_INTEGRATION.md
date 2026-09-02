# V98 — source-built data in the real backing link

Exact observed main: `d432effb8f844492d0d15315fd481a7563d1a0e3` (V96).
The unpushed V97 delivery was authenticated against its 41 path fingerprints
and ZIP SHA-256 before continuation in a separate tree. V98 is cumulative
over that exact main, preserving all V97 changes. No commit or push is made.

## Verified result

| Measurement | V97 | V98 |
|---|---:|---:|
| Historical Stage-3F audit | 1,265 | 1,265 |
| Section-backed addresses | 1,175 | 1,197 |
| Closed ROM-offset refactors | 29 | 29 |
| Unresolved contracts | 61 | 39 |
| Exact historical provider intervals | 16 | 49 |
| Exact provider bytes | 790,988 | 810,542 |
| Added backing beyond Stage-3C/E/assets | 744,892 | 762,372 |
| Historical bytes freshly compiled into the backing link | 0 | 695,316 |
| Source relocations preserved by rebinding | 13,511 | 13,619 |
| Isolated address relocations verified | 3,525 | 3,591 |

The 22 newly backed names belong to DSP1 (15), SuperFX (2), C4 scalar state
(3), PPU (1), and CRC32 (1). Existing aliases retain their requesting objects.
Six formerly interior-only aliases also gain historical-source ownership.
The 170 physical sections are nonoverlapping: 66 reused and 104 newly added.
Of the new sections, 53 / 695,316 bytes are rebuilt source data; the other
51 / 67,056 bytes remain privately extracted minimum-access ranges.

No canonical C translation unit changes in V98. The source-tree ownership
remains 97 TUs, 96 canonical objects plus one alternate and 1,383 definitions.
The live external chain is unchanged: **1,863 → 1,540 → 233 → 223 → 0**.
The formal function checkpoint remains 1,041/1,041; Stage 3D remains 53/53.
Neither number means that the aggregate emulator code or final image is exact.

## Placement evidence and extent limits

[`historical_data.json`](../../analysis/link_identity/historical_data.json)
now selects 48 complete typed source objects and one complete GLOBALS data
section. It adds the DSP1 cosine/sine buffers and scalar objects, complete
SuperFX `GSU`, two SuperFX lookup arrays, two PPU lookup arrays, `crc32Table`,
and twelve C4 scalars. It does not select surrounding padding, exception
frames, adjacent objects or mismatching RTC storage as if they were proved.

The recipe uses pinned Snes9x 1.41-1, pinned Newlib headers and EE GCC 3.2.2.
C4 reuses the V75 float/math adaptation and its normal-double profile; other
units retain the reviewed V52 short-double profile. Dependencies, source
patches, typed symbol extents and full provider bytes are checked again in
fresh temporary compilation trees. No old matching trees are rewritten.

Fourteen selected functions solve and reapply every selected function
relocation. Two additional functions have explicitly narrower proof scopes:

- `S9xSetPPU` proves its `.data` references and the two typed lookup arrays.
- `CMemory::InitROM` proves the CRC address using only the relocations at
  `0x001528b8` and `0x001528c4`. The complete function is still checked against
  its immutable relocation-normalized matching evidence, but this is not a
  full-relocation proof of that function or its string pool.

This distinction matters: the compiled CRC table is at `.rodata+0xd00`, but
its target address is `0x001b6ef8`. The corresponding placement is not the
same as the MEMMAP string-pool base. A single whole-section base would be
false. Explicit PC filters, full function hashes, typed table size 1,024 and
exact table bytes prevent that conflation. Ordinary public validation cannot
prove private bytes; the private fresh-build gate performs that comparison.

## Real source-data integration

`data_backing.py link` now invokes a fresh historical rebuild itself, even
when called directly rather than through Make. It compares the complete
rebuilt manifest to the frozen evidence and obtains provider bytes directly
from the compiled objects before their temporary directories are removed.

After subtracting existing Stage-3C/E/asset ownership, every historical output
range must belong to exactly one compiled provider. Both the full provider
and selected subrange must match their SHA-256 values. Only those checked
source bytes are written below ignored `build/data-backing/historical-source/`
and fed to the assembler/linker. Missing, changed, truncated or ambiguous
providers fail; there is no fallback to copying the same range from the
private original. The original remains the comparison oracle, not the input
payload for these historical sections.

The actual aggregate preserves its preexisting allocated bytes, its complete
global symbol roster and all 13,619 affected source relocations. It has zero
undefined globals. The additional address probe verifies 1,197 pointers and
1,197 HI16/LO16 pairs, but is deliberately synthetic and is not an emulator.
This is integration of exact data providers, not yet selection/integration
of all exact target function implementations.

## Reproduction

```sh
make check
make reference
make bootstrap-ee-stage1
make bootstrap-ee-cxx-stage1
make runtime-overrides-check provider-frontier-check named-data-check data-backing-check \
  EE_CC="$PWD/build/toolchains/ee-gcc-3.2.2-stage1/prefix/bin/ee-gcc" \
  EE_STAGE1_CXX="$PWD/build/toolchains/ee-gcc-3.2.2-cxx-stage1/prefix/bin/ee-g++"
make layout-oracle-check
```

The C and C++ bootstraps should run sequentially when their shared recipe
cache is empty. Public gates need neither the reference nor an EE compiler.
The private integration needs both the original and historical C/C++ tools.
This delivery is verified on Linux x86_64; it is not a new AArch64 run.
The working tree passed **469 unit tests**, host syntax **108/108**, public
manifest/ownership checks, the complete applicable private EE chain and the
layout oracle. In fresh installer clones the one original-dependent unit
test is expected to skip before `make reference` generates its unpacked file;
the subsequent private gates compare the real original.

## Recoverable next-work checkpoint

Implementation, manifests, ownership, tests and this checkpoint are in the
single-script patch. The installer checks the exact base, repository identity,
dirty-tree state and patch checksum and supports relative invocation. V97's
authenticated complete state can be resumed without an intervening push;
unknown user edits are never overwritten. `--verify-only` resumes validation
after an interrupted check without applying the patch again.

The remaining 39-name roster is authoritative in
[`data_backing.tsv`](../../analysis/link_identity/data_backing.tsv):

```sh
python3 -c 'import csv; print("\n".join(r["symbol"] for r in csv.DictReader(open("analysis/link_identity/data_backing.tsv"), delimiter="\t") if r["status"] == "NO_PROVED_BACKING"))'
```

Next investigations:

- Fifteen code-pointer contracts, ten at already audited Draw-family entries.
  Prove code identity/selection instead of inventing storage for them.
- Three C4 table bases: public source bytes agree in exploratory compilation,
  but a reviewed placement proof is still required before promotion.
- The complete `rtc_f9` candidate has one differing byte at offset 24. Check
  its `time_t` ABI and object boundary; do not copy the adjacent target byte
  into an invented initializer. Its `control` member is a narrower future target.
- `DAT_00426820` may be a biased mapping pointer related to `Dummy-0x6000`,
  rather than an object at that numerical address. This remains a hypothesis.
- Audio addresses, SPC7110 constants, libgcc constant providers, and unzip/zlib
  masks still need typed ownership and placement evidence.

Beyond this roster: complete object/array bounds for the full image, exact
implementation selection, historical member data and archive composition,
linker layout, object order, global relocations and SJCRUNCH2/LZO packing
remain open. No final replacement ELF or image identity is claimed.

Final required SHA-256 values remain:

- Raw image: `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`.
- Packed ELF: `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487`.
