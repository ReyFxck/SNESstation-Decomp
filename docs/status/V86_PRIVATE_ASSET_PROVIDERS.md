# V86 private embedded-asset providers

V86 closes the ten `private-asset` contracts carried by the V85 frontier. A
user-supplied reference image is SHA-256 verified, five exact byte ranges plus
their target padding and 32-bit size words are materialized below `build/`, and
the resulting provider object is partially linked into the V85 aggregate.

No private byte, extracted asset, generated assembly, object or report is
tracked. The public tree contains only names, addresses, sizes and SHA-256
oracles in
[`private_asset_providers.tsv`](../../analysis/link_identity/private_asset_providers.tsv).

## Reproducible gates

The compiler/reference-free identity and frontier check is:

```bash
make private-assets-public-check
```

With `original/SNES_EMU.ELF` present, build the historical compiler, unpack the
reference and run the complete dependency chain with:

```bash
make private-assets
```

To reuse an existing EE GCC 3.2.2 installation after `make reference`:

```bash
make private-assets-check EE_CC=/absolute/path/to/ee-gcc
```

The private gate verifies the complete unpacked-image hash, each individual
asset hash, every zero padding byte, every little-endian size word, all ten
global symbol values/types/sizes, and the exact input/output undefined sets.
It also fingerprints every allocated section before and after the partial
link.

## Verified bundles

| Bundle | Asset bytes | Padding | Size word | Provider bytes | Symbols |
|---|---:|---:|---:|---:|---:|
| `cdvd_irx` | 32,212 | 0 | 4 | 32,216 | 2 |
| `sjpcm_irx` | 8,133 | 3 | 4 | 8,140 | 2 |
| `amigamod_irx` | 20,061 | 3 | 4 | 20,068 | 2 |
| `credits_text` | 1,473 | 3 | 4 | 1,480 | 2 |
| `disclaimer_text` | 826 | 2 | 4 | 832 | 2 |
| **Total** | **62,705** | **11** | **20** | **62,736** | **10** |

The IRX arrays and their size words satisfy the module-loader contracts in
`app/main_bootstrap.o`. The XOR-obfuscated credits/disclaimer arrays and size
words satisfy `app/main_flow_recovered.o`; they remain writable because the
program decodes them in place.

## Verified result

| Measurement | Result |
|---|---:|
| V85 provider frontier | **261** |
| Private bundles | **5/5** |
| Private provider symbols | **10/10** |
| Verified provider bytes | **62,736** |
| Partial-link externals | **261 → 251** |
| Existing allocated-section changes | **0** |
| New private provider sections | **5** |

The generated object uses one four-byte-aligned writable section per contiguous
asset/padding/size-word bundle. Four-byte alignment is the proved minimum for
the 32-bit size contracts; this checkpoint does not claim the final historical
output section, address, surrounding gaps or linker order.

## Exact remaining frontier

| Provider class | Rows |
|---|---:|
| Named link contracts | **197** |
| Named program-data storage | **35** |
| V84 source-address blockers | **14** |
| Historical archive members | **5** |
| **Total** | **251** |

V86 is a byte-producing provider checkpoint, not a replacement ELF. Final
data/rodata/bss ownership, historical archive selection, relocations, linker
script, section addresses, object/library order and packing remain open.
