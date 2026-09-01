# V94 — target runtime overrides and Stage-3F access evidence

Base: V93 commit `c69b1c0838ecf98409f52bf713073df5cc7cf5a3`.

The original **53-row Stage-3D runtime contract ledger is now adjudicated**:
seven libgcc contracts, one formatter source refactor, 43 PS2LIB member-text
contracts and two target-selected reconstructed overrides. This does **not**
close original whole-archive provenance/composition, member data, final global
relocations, the complete source-selection/layout gate, or the replacement ELF.

## Positive override identity, without promoting rejected archives

V93 correctly rejected the historical `puts` and `terminate.o` candidates as
providers of the two selected runtime contracts. V94 retains those rejections.
The additional proof is based on named relocations in independently rebuilt
historical **callers**, followed by a raw-exact final link of recovered bodies.

| Contract | Historical incoming witness | Selected target | Final provider proof |
|---|---|---|---|
| `abort` | Nine named `R_MIPS_26 abort` relocations in complete `unwind-dw2.o`, plus five in complete `unwind-dw2-fde.o` | `0x00107578` | Recovered non-returning spin, eight bytes, raw-exact after link. |
| `puts` | Named `R_MIPS_26 puts` relocation in the complete 36-byte weak `abort` function retained at `0x0019c5a8` | `0x0019e414` | Recovered 96-byte no-newline writer; the final `fioWrite@0x0019d244` call is also relocated and compared exactly. |

Both complete unwind members match across **14,272 bytes** after **208 precise
relocation masks**. The extra weak function adds 36 bytes and four masks. The
fifteen selected incoming calls are decoded from the private target and checked
against their real ELF relocation names/types and zero addends. A same-sized
function, an existing alias, or a matching masked JAL alone cannot pass.

The weak termination body is useful caller evidence, **not** proof that the
whole `terminate.o` matches: its adjacent `exit` implementation differs. It
also does not establish that this weak `abort` is the selected abort provider;
the unwind calls positively select the separate application spin instead.

[`matching/candidates/runtime_overrides.c`](../../matching/candidates/runtime_overrides.c)
reconstructs the already audited V41/V47 behaviors. The isolated historical
linker proof places its two functions at their target addresses and compares
all **104 final bytes without relocation masks**. Its `.proof.elf` is not an
application, and is never presented as the final emulator.

Public ledgers:

- [`runtime_overrides.tsv`](../../analysis/link_identity/runtime_overrides.tsv)
  pins source/profile hashes, canonical owners, target/linked hashes and calls.
- [`runtime_override_witnesses.tsv`](../../analysis/link_identity/runtime_override_witnesses.tsv)
  pins the two complete member witnesses and the separate complete weak function.
- The two imports now have provider kind `recovered-runtime`, not an invented
  claim of original archive origin. Their namespace bindings stay unchanged.

Native tests exercise empty and nonempty strings, high-bit bytes and failed/
short writes: `puts` returns measured length, does not append a newline and
ignores the write result, as the target does. `abort` is checked by compiled
and linked bytes; tests do not hang by executing its infinite loop.

## Stage 3F: prove accessed ranges, do not guess object sizes

The provisional structural lift declares many unrelated `DAT_*` names as
`uint64_t`. Those declarations are **not** evidence of eight-byte original
objects. V94 instead analyzes actual target instructions inside **803 strict
matched function spans** and freezes the local address-construction witnesses.

| Measured access proof | Count |
|---|---:|
| Original Stage-3F contracts retained | 1,265 |
| Contracts with a target-consumed span | 705 |
| Contracts without a direct witness | 560 |
| Distinct witnessed addresses | 685 |
| Fixed-count `memset`/`memmove` call ranges selected | 8 |
| Overlap-aware range clusters | 174 |
| Unique bytes covered within this access ledger | 70,746 |

The scanner accepts only known constant construction chains, direct aligned
EE loads/stores, and fixed-size calls to the independently proved PS2LIB
`memcpy`/`memset`/`memmove` entries. It respects branch targets, coprocessor
branches and MIPS delay slots; discards caller state after calls; treats
unknown instructions as barriers; and rejects unknown indices, uncertain
transfer widths and unsupported overflow/high-address arithmetic.

Eight selected call ranges have explicit sizes 13, 28, 64, 256, 328, 964,
2,044 and 65,536 bytes. Each range has a target byte/zero-fill hash; no bytes
are committed. The data proof verifies the exact target reference first and
re-derives every row. It also authenticates the historical memory callees.

[`unnamed_data_accesses.tsv`](../../analysis/link_identity/unnamed_data_accesses.tsv)
contains **all** 1,265 rows, including the 560 unproved ones. A proved row is
a **minimum consumed range**, not a recovered full C object or array extent.
The 70,746 bytes are not asserted to be disjoint from earlier Stage-3C/3E
providers. No new program storage is allocated and no source-field width is
silently changed. Complete bounds, indexed uses, pooling and final placement
remain open, so **Stage 3F is not closed**.

## Gates and unchanged checkpoints

```bash
make check
make runtime-overrides
make unnamed-data
make reproduce-check
```

Reuse an installed compiler without bootstrapping:

```bash
make reference
make runtime-overrides-check EE_CC=/absolute/path/to/ee-gcc
make unnamed-data-verify
```

Public checks use no original, compiler or download. Private checks reject a
wrong reference, changed source/profile/evidence hashes, modified witness
instructions, wrong relocation symbols/addends/callees, changed final provider
bytes and data-access drift. Fingerprint capture is a separate explicit
review command, never part of ordinary verification.

The 1,041-function checkpoint, all 97 source-object fingerprints and canonical
aggregate stay unchanged. The existing link chain remains
**1,892 → 1,569 → 233 → 223 → 0**, with zero runtime shims and all Stage-3C/3E
compatibility stores replaced.

The integrated public/private run passed **339 unit tests**, **108/108 host
syntax units** and **97/97 EE manifest units** (96 canonical objects and one
alternate). The host count increases by one because the new exact override
candidate is also syntax-checked; the Stage-2 manifest is unchanged.
`make reproduce-check` passed. `make elf` still exits with status 2 and the
remaining identity gates; this is not reported as successful reproduction.

V94 does not change final `make elf` into a successful dummy build. Full object
selection/integration, original member data and relocations, remaining data
bounds, the final layout and both unpacked/packed hashes must still be proved.
