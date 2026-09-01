# V93 — Stage-3D PS2LIB member text

Base: V92 commit `00d60ec6843f40a33bb280064c20da29b14a18a4`.

V93 proves **43 of the 45 remaining runtime contracts**, using **42 complete
member `.text` sections** rebuilt from pinned historical PS2LIB source
witnesses. Together with V91's seven contracts and V92's formatter refactor,
the original Stage-3D ledger is **51/53 closed**. `puts` and `abort` remain
explicitly blocked; their historical candidates do not match the target.

This is a complete-member **text** gate. It does not claim identical whole
archive containers, final relocated instruction values, member data/BSS
placement, or an identical replacement executable.

## Rebuilt member evidence

| Recipe group | Closed contracts | Unique selected members | Complete text bytes | Relocations masked |
|---|---:|---:|---:|---:|
| PS2LIB libc | 18 | 18 | 2,248 | 47 |
| SIF/RPC/file I/O/kernel | 22 | 21 | 5,480 | 271 |
| Syscall/cache assembly | 2 | 2 | 192 | 0 |
| Memory card | 1 | 1 | 5,044 | 382 |
| **Total** | **43** | **42** | **12,964** | **700** |

`SifInitCmd` and `SifExitCmd` share `sif_cmd_main.o`; it is counted once.
The scope extends beyond the already matching individual entry bodies:

| Complete member | Target base | Text bytes | Additional coverage |
|---|---:|---:|---|
| `libmc.o` | `0x001a0740` | 5,044 | Entire memory-card unit; `mcInit` is only 380 bytes at offset `0x1ac`. |
| `SifRpcMain.o` | `0x0019c960` | 1,040 | Internal RPC handlers and `SifInitRpc` at offset `0x2ac`. |
| `sif_cmd_main.o` | `0x0019f2dc` | 616 | Initialization helper, `SifInitCmd` and `SifExitCmd`. |
| `fio_main.o` | `0x0019f600` | 488 | Initialization plus all other text in the member. |
| `malloc.o` | `0x0019e474` | 468 | `_heap_mem_fit` plus `malloc` at offset `0x40`. |
| `strncpy.o` | `0x0019c550` | 88 | 84-byte function plus its terminal four-byte gap. |

Every member comparison uses the actual ELF MIPS relocation masks, never
whole-instruction wildcarding. Changed opcodes, helpers, unmasked bytes or
terminal padding fail. The verifier checks the complete private image hash
first, and freezes the source dependency closure, member text hash, normalized
hash, target hash, symbol-table digest, symbol offsets and sizes.

The public ledgers contain only metadata and hashes:

- [`runtime_members.tsv`](../../analysis/link_identity/runtime_members.tsv): all 45 contracts, including the two blockers and their live requesters.
- [`runtime_member_objects.tsv`](../../analysis/link_identity/runtime_member_objects.tsv): 42 selected and two rejected member recipes.
- [`runtime_member_inputs.tsv`](../../analysis/link_identity/runtime_member_inputs.tsv): 36 upstream source/header files and three compiler headers.

## Source provenance and ABI

The source witnesses are the PS2LIB files preserved during migration into
PS2SDK:

- [`694100b7`, 15 April 2004](https://github.com/ps2dev/ps2sdk/tree/694100b78ad5bc8f8248a1138143860af4f8435f): libc, assembly strings and memory-card sources.
- [`a80df908`, 18 April 2004](https://github.com/ps2dev/ps2sdk/tree/a80df908256955382f102278400b5d713552dbce): kernel/RPC sources and shared headers.
- [`94d97570`, 4 May 2004](https://github.com/ps2dev/ps2sdk/tree/94d9757035b8ea935383a11d51ed82ab3f65fc79): only `malloc.h` and `iopcontrol.h`, which were missing from the initial migration snapshots.

These dates are **not** asserted as the executable's build dates, nor as
unique original archive revisions. They pin reproducible source content whose
compiled member text agrees with the January-2004 target. The original
archive packaging/order remains a later identity question.

The member recipe uses historical EE GCC 3.2.2 with `-Os -G0 -EL` and the
compiler's default PS2LIB ABI. It intentionally does not inherit the source
model's `-mlong64` or `-fshort-double`. `-nostdinc`, explicit pinned include
directories, SHA-verified compiler headers and a checked dependency list
prevent silent fallback to host or modern SDK headers. No historical source
is patched to obtain these matches.

Selected members are assembled into four **local selection archives** under
`build/runtime-members/archives/`, then extracted again before comparison.
The two rejected objects are never inserted. These generated archives are
inputs for future layout work, not substitutes silently linked over the
current recovered-source aggregate.

## Two honest blockers

| Contract | Current target binding | Rebuilt historical candidate | Decision |
|---|---|---|---|
| `puts` | `puts_like_recovered` at `0x0019e414`, 96 bytes, writes without a newline and returns length | `F_puts`, 80 bytes, calls `putchar('\n')` and returns length + 1 | Reject the member; prove the runtime override/source lineage separately. |
| `abort` | Existing source alias selects `snes_fatal_spin_00107578`, an eight-byte spin | `terminate.o` contains a 36-byte `abort` body that prints and calls `_exit` | Reject the member; the current alias is not proof of original archive/override origin. |

The verifier recompiles these candidates and preserves their rejection. It
does not treat a zero-undefined source link, the spelling of an alias, or a
known target address as historical archive identity. The V41/V47 recovered
function evidence remains unchanged.

## Public and private gates

No private file, compiler or download is needed for:

```bash
make check
make runtime-members-public-check
```

With a legally obtained `original/SNES_EMU.ELF`:

```bash
make runtime-members
```

Reuse an installed historical compiler:

```bash
make reference
make runtime-members-check EE_CC=/absolute/path/to/ee-gcc
```

`make runtime-members-verify` rebuilds just the member proof and private
reference. `make runtime-members-refresh` is the explicit reviewed fingerprint
capture command, not an ordinary verification step. A first private run
fetches three pinned Git commits; subsequent runs reuse verified inputs.
An existing Git object cache can be supplied with
`RUNTIME_MEMBER_SOURCE_CACHE=/path/to/ps2sdk-checkout`.

`make reproduce-check` includes the new gate. The verified output is:

```text
verified Stage-3D runtime members: contracts=43/45 members=42 text_bytes=12964 relocations=700 stage3d=51/53 open=2
blocked runtime identities: abort, puts (no forced archive selection)
```

## Unchanged aggregate and remaining work

V93 changes ownership descriptions and adds historical member build inputs;
it does not edit recovered C bodies or the immutable 1,041-function checkpoint.
All 97 source-object fingerprints and the canonical aggregate remain unchanged
from V92. The existing private chain still verifies
**1,892 → 1,569 → 233 → 223 → 0** externals, with zero runtime shims and the
Stage-3C/3E replacement of all 39 compatibility stores intact.

Validation: **279 unit tests**, host syntax **107/107**, historical source tree
**97/97**, all private member proofs and the complete dependency chain.
Without the private original, one pre-existing reference test is skipped.

Next: prove the `puts`/`abort` runtime overrides, recover the 1,265 Stage-3F
unnamed data contracts, and integrate historical member data/relocations into
the final layout. Stage 3G/3H unpacked identity and Stage 4 SJCRUNCH2 identity
remain open. `make elf` deliberately stays blocked.
