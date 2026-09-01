# V92 — Stage 3D snprintf source-contract refactor

V92 closes **one** original Stage-3D source contract: the four recovered
`snprintf` uses were lift adapters for calls to the existing `sprintf`
provider. No additional historical archive member is claimed. The final
compatibility runtime shim is removed; Stage 3D is now **8/53 closed** with
**45 archive identities still open**.

Base: V91 commit `a95946da4ac9270fe42aa29b2aa457ec8f2e77ff`.

## Direct target evidence

The SHA-verified unpacked reference contains exactly one direct `JAL` in each
of these four frozen spans. Every call selects `sprintf@0x0019e3d0`:

| Source-model identity | Target entry | Span bytes | Direct call address |
|---|---:|---:|---:|
| Snapshot path | `0x00105718` | 56 | `0x00105734` |
| `CMemory::StaticRAMSize` | `0x00156934` | 96 | `0x00156984` |
| `CMemory::Size` | `0x00156994` | 112 | `0x001569f4` |
| `CMemory::MapMode` | `0x00156c0c` | 72 | `0x00156c38` |
| **Total** | **4 spans** | **336** | **4 calls to sprintf** |

[`runtime_refactors.tsv`](../../analysis/link_identity/runtime_refactors.tsv)
records the span hashes, call addresses, callee and SHA-256 of the existing
V34/V46/V47 strict matching evidence. The private gate verifies the complete
3,304,936-byte reference hash before checking these spans and decoding the
actual JAL targets. A missing or changed call, extra call, changed span hash,
wrong private image or reintroduced `snprintf` import fails the gate.

The V47 snapshot candidate already calls `sprintf`; the official historical
[Snes9x 1.41-1 archive](https://www.lysator.liu.se/snes9x/1.41-1/snes9x-1.41-1-src.tar.gz)
also uses it in the three metadata methods. Its archive digest is
`5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b`;
the source recipe remains in `tools/history/research/hunt1000plus_v46_closure.py`.
The decisive new proof is the original direct callee, not the absence of a
`snprintf` name in the progress manifest and not a relocation-masked guess.

## Source-model boundary and safety

The three numeric outputs remain within the existing 32/32/16-byte model
buffers. The snapshot adapter's `(buffer, capacity)` signature is not the
original fixed-global-buffer ABI. It keeps its previous truncation and
NUL-termination behavior for every small capacity using a bounded literal
copy, and uses the target formatter when the complete string fits. The
capacity-zero case still does not touch the buffer.

Host regression tests cover all 256 input-byte values for each metadata
formatter and snapshot capacities 0..64 with surrounding canaries. These
tests preserve the existing model behavior; they do not assert that the
models' ABI, globals, format strings or complete compiled functions are
already the exact historical source. The immutable 1,041-function checkpoint
and its separate strict candidates are unchanged.

## Verified live link result

| Gate | V91 | V92 |
|---|---:|---:|
| EE source-tree externals | 1,893 | **1,892** |
| After 323 source-address aliases | 1,570 | **1,569** |
| Zero-byte contracts resolved | 1,336 | **1,336** |
| Before private assets | 234 | **233** |
| After ten private-asset providers | 224 | **223** |
| Final partial-link externals | 0 | **0** |
| Compatibility runtime shims | 1 | **0** |
| Closed original Stage-3D contracts | 7/53 | **8/53** |

The remaining provider ledger contains 175 anchors, nine aliases and 39
compatibility stores; Stage 3C/3E exact providers still replace all 39 stores
in the private link. The seven V91 libgcc proofs remain intact: four complete
archive members, 3,848 exact text bytes, 21 relocations and three closed
compiler-libcall refactors.

Only the two edited translation units and their canonical aggregate receive
new object fingerprints. The zero-byte invariants apply to each alias/anchor
partial-link step; V92 does **not** claim unchanged source-object text versus
V91. No private ELF, assets, object code or target bytes are included in the
public patch.

## Reproduce

Public gate (no private file or EE compiler required):

```bash
make check
make runtime-refactors-public-check
```

Complete chain with `original/SNES_EMU.ELF` and the historical compiler:

```bash
make runtime-refactors
```

Reuse an installed EE compiler:

```bash
make reference
make runtime-refactors-check EE_CC=/absolute/path/to/ee-gcc
```

`make reproduce-check` includes this gate. Reports and private providers stay
under ignored `build/`. The standalone private call proof, without rebuilding
the dependency chain, is `make runtime-refactors-verify`.

## Remaining work

The 45 remaining Stage-3D libc/Newlib/PS2 runtime identities, the 1,265
Stage-3F unnamed address contracts, final layout/link order, unpacked-image
identity and SJCRUNCH2 packing remain open. Zero undefined symbols and zero
compatibility shims do not yet produce an identical replacement ELF.
