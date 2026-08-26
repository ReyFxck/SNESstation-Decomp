# V89 — Stage 3C closed

V89 closes the original 54-row Stage-3C tranche. The final accounting does not
invent target storage for names that existed only in the behavioral source:

| Closed evidence class | Rows |
|---|---:|
| Exact private-asset objects | **10** |
| Exact non-asset target ranges | **40** |
| Completed source refactors for target-absent adapters | **4** |
| **Historical Stage-3C total** | **54/54** |

All **50 target-backed rows** have a target address, a reviewed nonzero extent
and a SHA-256 fingerprint for precisely that range. No address-only row
remains. The four refactor rows have neither an address nor an extent because
the target contains no such objects.

## Correction to the open V88 audit

V88 correctly identified five visible blockers but its provisional 49-range
count included a false four-byte `g_Memory` object at `0x0035e2b0`. Target
disassembly shows that `main` materializes `0x0034e2b0` directly; that address
is the already tracked `g_p12_memory` object, not the contents of a separate
pointer slot.

V89 therefore retracts the false `g_Memory` range, proves the font and vtable
ranges, and reclassifies `g_memory_state_001c3ab0` as another absent source
adapter. The net result is **50 fingerprinted target ranges**, not a
mechanically inflated 51.

## Boundary proofs completed

| Symbol | Exact target range | Proof |
|---|---|---|
| `g_frontend_font_001bb748` | `0x001bb748..0x001bb87c` (`0x134`) | `gsFontRecovered` has a compiler-checked `0x134` target size and the independently anchored `DAT_001bb87c` starts at the exclusive end. |
| `snes_vtable_00426c28` | `0x00426c28..0x00426c38` (`0x10`) | Four method entries occupy the address point; the following typeinfo object starts at `0x00426c38`. |

The compact public record is
[`stage3c_boundary_proofs.tsv`](../../analysis/link_identity/stage3c_boundary_proofs.tsv).
The private bytes remain untracked; only their hashes are committed in
[`named_data.tsv`](../../analysis/link_identity/named_data.tsv).

## Four target-absent adapters removed

| Removed source name | Target evidence and replacement |
|---|---|
| `g_Memory` | `main` passes `0x0034e2b0` directly; source now passes `g_p12_memory`. |
| `g_memory_state_001c3ab0` | `frontend_shutdown` also passes `0x0034e2b0`; source now passes `g_p12_memory`. |
| `g_unz_ops_recovered` | unzip code calls `fioRead`, `fioLseek`, `fioClose` and `inflateEnd` directly; the six-callback table was removed. |
| `g_zip_io_recovered` | target state is split across pointer slots `DAT_00424850`/`DAT_00424854`, the `0x00448200` workspace and `uRam0044e206`; source now uses exact checked field offsets. |

The source refresh consequently reduces the live Stage-2 external set from the
historical **1,921-plan snapshot to 1,917 real contracts**. The original plan
remains stable because the four removals stay recorded as closed historical
Stage-3C rows.

## Exact range and link result

The 40 non-asset ranges form **15 overlap-aware clusters** covering **141,159
unique bytes**. Generated range bytes and assembly stay below ignored
`build/named-data/`.

| Gate | Result |
|---|---:|
| Source-tree externals | **1,917** |
| After 323 source-address aliases | **1,594** |
| Zero-byte contracts | **1,336/1,594 resolved** (`1,273` anchors + `63` aliases) |
| Provider frontier before private assets | **258** |
| After ten private-asset providers | **248** |
| Live provider closure | **181 anchors + 9 aliases + 41 storage + 17 shims** |
| Exact Stage-3C compatibility replacements | **32** |
| Compatibility storage after Stage 3C | **41 → 9** |
| Final partial-link externals | **248 → 0** |

## Reproducible gates

The public gate needs neither the ELF nor the EE compiler:

```bash
make named-data-public-check
```

With a legally obtained `original/SNES_EMU.ELF`, run the full private proof:

```bash
make named-data
```

To reuse an existing compiler:

```bash
make named-data-check EE_CC=/absolute/path/to/ee-gcc
```

## Claim boundary and next tranche

Stage 3C is closed. This is not yet a complete replacement ELF. The next
original tranche is **Stage 3D**: identify and reproduce the 53 historical
libgcc, libc/Newlib and PS2 runtime contracts with exact archive revision and
member-selection evidence. Stage 3E, 3F, final link/layout (3G) and the exact
3,304,936-byte unpacked image/hash gate (3H) remain after it.
