# V91 — Stage 3D libgcc subtranche closed

V91 closes all **seven compiler-runtime contracts** in the libgcc part of the
original Stage-3D plan. Four names select complete GCC 3.2.2 EE archive-member
text sections, while three were compiler libcalls introduced only by the
reconstructed structural lift.

| Closed evidence class | Rows |
|---|---:|
| Exact `libgcc.a` member text | **4** |
| Completed source refactors | **3** |
| **Stage-3D libgcc total** | **7/7** |

The public
[`libgcc_contracts.tsv`](../../analysis/link_identity/libgcc_contracts.tsv)
ledger records archive members, target ranges, complete-member hashes and
relocation counts. It contains no target or archive bytes.

## Four exact archive members

The historical GCC 3.2.2 EE archive reported by `ee-gcc
-print-libgcc-file-name` supplies four complete `.text` sections that agree
with the target after masking only relocation-controlled instruction bits:

| Contract | Archive member | Target range | Bytes | Relocations |
|---|---|---:|---:|---:|
| `__muldi3` | `_muldi3.o` | `0x001a1b20..0x001a1b98` | **120** | **0** |
| `__floatdisf` | `_floatdisf.o` | `0x001a1b98..0x001a1c98` | **256** | **7** |
| `__udivdi3` | `_udivdi3.o` | `0x001a25b0..0x001a2c78` | **1,736** | **7** |
| `__umoddi3` | `_umoddi3.o` | `0x001a2c78..0x001a3340` | **1,736** | **7** |
| **Total** | **4 members** |  | **3,848** | **21** |

The comparison covers each member's entire `.text`, including terminal gaps
and the internal `__udivmoddi4` bodies selected by the two division wrappers.
`__muldi3` is raw exact; the other three agree after applying the precise MIPS
relocation masks derived from their object files.

## Three source-only libcalls removed

The remaining historical names do not identify linked target members:

| Contract | Archive evidence | Closure |
|---|---|---|
| `__ashlti3` | `ashlti3.o` has an empty `.text` section for this EE compiler | Wide lift helpers now operate explicitly on bytes/64-bit pieces. |
| `__lshrti3` | `lshrti3.o` has an empty `.text` section for this EE compiler | Concatenated shift expressions are narrowed to their real 64-bit width. |
| `__fixunssfdi` | `_fixunssfdi.o` has `0x130` text bytes and 12 relocations, but zero relocation-normalized occurrences across the complete target image | Float-to-unsigned conversion is expressed with explicit exponent/mantissa integer logic. |

Recompiling all 97 translation units with the historical compiler proves that
none of these three names remains undefined. This reduces the live source
external count from V90's 1,896 to **1,893** without inventing target code.

## Live link result

| Gate | Live V91 result |
|---|---:|
| Source-tree externals | **1,893** |
| After 323 source-address aliases | **1,570** |
| Zero-byte contracts | **1,336/1,570 resolved** (`1,273` anchors + `63` aliases) |
| Provider frontier before private assets | **234** |
| After ten private-asset providers | **224** |
| Live provider closure | **175 anchors + 9 aliases + 39 storage + 1 shim** |
| Final partial-link externals | **224 → 0** |

The sole compatibility runtime shim is now `snprintf`. The Stage-3C and
Stage-3E exact providers continue to replace all 39 compatibility storage
definitions in the private partial link; no private bytes are committed.

## Reproducible gates

The public gate requires neither the private ELF nor an EE compiler:

```bash
make libgcc-contracts-public-check
```

With a legally obtained `original/SNES_EMU.ELF`, run the complete source-tree,
partial-link, archive-member and private-reference proof:

```bash
make libgcc-contracts
```

To reuse an existing compiler:

```bash
make reference
make libgcc-contracts-check EE_CC=/absolute/path/to/ee-gcc
```

## Claim boundary and next work

This closes **7 of the original 53 Stage-3D contracts**. The remaining 46
libc/Newlib and PS2 runtime contracts still require exact archive revision,
member selection and placement proof. A zero-undefined partial link is not an
exact final link: Stage 3F unnamed data, Stage 3G layout/link order, Stage 3H
unpacked identity and SJCRUNCH2 packed identity remain open.
