# V96 — control-flow-aware data access proofs

Base: `de757c4486e2db408b820078fe8b2699de3c6887` (V95).

This extends the existing private-reference data proof, not the formal
function denominator. It does **not** close Stage 3F or produce a replacement
ELF. No original code bytes are copied into a new implementation.

## Measured delta

| Gate | V95 | V96 |
|---|---:|---:|
| Unnamed contract roster | 1,265 | 1,265 |
| Contracts with minimum-access witnesses | 705 | 824 |
| No access witness | 560 | 441 |
| Unique witnessed access bytes | 70,746 | 167,521 |
| Fixed-count memory-call ranges | 8 | 10 |
| Section-backed addresses | 886 | 961 |
| Interior-only labels without access-width claims | 181 | 137 |
| Still-unbacked absolute anchors | 379 | 304 |
| Reused Stage-3C/E and asset sections | 66 | 66 |
| Materialized minimum-access sections | 109 | 117 |
| Materialized minimum-access bytes | 69,768 | 165,946 |
| Total nonoverlapping backing bytes | 300,286 | 396,464 |
| Preserved source relocations to backed names | 12,078 | 12,434 |
| Isolated pointer/HI16/LO16 relocation records proved | 2,658 | 2,883 |

There are **119 newly witnessed contracts** and **12 wider minimum spans**.
The selected evidence is 693 block-local witnesses plus 131 CFG witnesses.
The physical-storage increase is **96,178 bytes**, distinct from the 96,775
increase in witnessed-access coverage because existing sections already own
some of the additional consumed bytes. All new materialized bytes in this
batch are initialized; no BSS boundaries were invented.

## What changed

[`ee_dataflow.py`](../../tools/ee_dataflow.py) adds a small, conservative
must-constant analysis for ordinary R5900 control flow. It tracks only known
low-positive GPR values and their defining instructions:

- At joins, every reachable predecessor must agree on the value. Differing or
  missing incoming values become unknown; no favorable path is selected.
- Facts are emitted only after the fixed point. A loop's first-iteration
  constant cannot masquerade as an invariant.
- Ordinary branch delays execute on both paths; branch-likely delays are
  annulled on the untaken path. Calls write the link register before their
  delay slot and invalidate all tracked GPRs on return.
- Known FPU-only operations preserve GPRs; `mfc1`/`cfc1` invalidate their GPR
  destination. Reviewed R5900 MMI instructions invalidate only their real
  destination. Other instructions remain barriers, not inferred semantics.
- Unknown indirect jumps, malformed delay-slot graphs and internal calls
  disable this extra CFG proof. No memory contents are treated as immutable
  pointers, and unknown indexes never become array bounds.

The exact opcode masks and destination roles follow the pinned EE binutils
2.14 `opcodes/mips-opc.c` used by the
[`historical toolchain`](../HISTORICAL_EE_TOOLCHAIN.md). This is a restricted
static proof inside the selected matched function, not a R5900 emulator or a
claim of runtime path coverage.

[`unnamed_data.py`](../../tools/unnamed_data.py) retains the older block-local
proof and prefers it for equal widths. Each accepted ledger row now includes
`proof_kind`, `function_sha256` and `analysis_sha256`. A CFG row fingerprints
the **whole function**, not just the linear address-construction window; its
other predecessors and backedges matter. Both analyzer sources are hashed, so
changing the analysis requires explicit private recapture and review. The
normal public/private verification paths never refresh evidence automatically.

## Sound echo buffer witness

The strongest new range is `DAT_003ab748`, used by the sound echo code. In the
strict matched function at `0x00174120`, the branch delay at `0x00174168`
loads `a2 = 0x10000`. The taken path reaches the named `memset` call at
`0x001741e0`; its delay at `0x001741e4` ORs in `0x7700`, giving **96,000 bytes**.
The destination is independently constructed as `0x003ab748`; the callee is
the already-proved `memset@0x0019c39c`. The old block-only scanner discarded
the count at the branch target.

This proves that consumed clear range. It does not assert the original C
array declaration, a whole-object upper bound, or bounds of every indexed
echo access. A second new fixed-count clear proves 544 bytes at `0x0035c002`.

## Storage, relocations and privacy

The existing backing gate consumes the updated witnesses without relaxing its
checks. It reuses the 66 fixed Stage-3C/E and asset sections, subtracts their
coverage, and generates only uncovered proved ranges under ignored `build/`.
The public manifests contain addresses, ownership, extents and hashes only.
No private bytes are shipped in this patch.

All allocated sections of the Stage-3E input remain byte/metadata-identical.
The complete global-symbol roster is unchanged, unrelated/unbacked globals
retain their identities, and the aggregate stays at zero undefined globals.
The 12,434 affected source relocation records (5,724 HI16 / 6,710 LO16) retain
their section, offset, type and symbol identity. Repartitioning generated
access ranges does not claim that V95's generated section names are immutable.

An isolated final address probe verifies 961 pointer values and 961 HI16/LO16
pairs at the target data addresses: 2,883 relocation records. Its synthetic
code is **not emulator code**, and its ELF is not a replacement application.

## Validation

```sh
make check
make unnamed-data
make data-backing
make reproduce-check
```

With the already-installed EE compiler, `make data-backing-check` avoids
bootstrapping. The private tests require the same legally obtained original
at `original/SNES_EMU.ELF`. Only explicit `make unnamed-data-refresh` followed
by `make data-backing-refresh` captures new evidence.

The test suite adds 35 dataflow tests and three manifest-regression tests
(401 total). These cover conflicting/missing predecessors, loop invariants
versus transient values, annulled delay slots, calls, FPU/MMI register effects,
invalid graphs, signed immediates, hash drift, and 200 deterministic diamonds
checked against an independent concrete-path interpreter. Stage-2 remains
97 translation units / 96 canonical objects, host syntax 108/108, and formal
matching 1,041/1,041. Runtime adjudication remains 53/53; sources and canonical
object fingerprints are unchanged.

## What is not finished

304 unnamed addresses still lack proved backing. The full object/array bounds
remain open even for backed labels; indexed/pointer contracts need actual
evidence, not widths guessed from adjacent addresses. The final program also
still needs exact implementation selection, historical member data, final
layout/relocations/link order and SJCRUNCH2 packing with both frozen hashes.
`make elf` remains intentionally blocked.

Prior checkpoint:
[`V95_SECTION_BACKED_DATA_ALIASES.md`](V95_SECTION_BACKED_DATA_ALIASES.md).
