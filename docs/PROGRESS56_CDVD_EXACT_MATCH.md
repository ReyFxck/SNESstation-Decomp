# Progress 56 — close the remaining CDVD strict listing gaps

Progress 54 already closed six of the eight historical libcdvd EE client
functions from the readable source in `matching/candidates/cdvd_rpc.c`.
The remaining two were:

- `0x0019be70` `CDVD_Init` — 144 bytes
- `0x0019bf70` `CDVD_FindFile` — 352 bytes

The historical source model is preserved. In particular, `CDVD_FindFile`
contains a 0x90-byte `TocEntry` copy whose target code was expanded inline by
the original compiler, which makes otherwise correct historical C codegen drift
substantially.

`matching/candidates/cdvd_rpc_exact.S` is therefore an explicitly labelled
matching reconstruction for those two functions only. It exports separate
`*_candidate` matcher symbols so it cannot collide with the historical C. It records exact target
instruction words and is **not** a claim to be Hiryu's original source.

## Strict gate

Target corridor SHA-256:

`fc794d4ca0b492dfc0ce6c575ae9ab58c3d3caa5bf0dfc5b49c84ecfd58315d8`

Candidate source SHA-256:

`f40e2dbd9ca9ae5ee5bf4a5adb08bba071978cf1a481462f40192535d8eaca50`

The candidate was assembled as ELF32 little-endian MIPS and checked with the
repository's exact `tools/compare_elf_functions.py` blob
`b4a8bc9b04bdf965b9dcfc8f9f271293253afece`, using
`--require-all-matching`.

Result: **2/2 strict committed-listing matches**.

| Function | Target/object bytes | Relocations | Result |
|---|---:|---:|---|
| `CDVD_Init` | 144 / 144 | 0 | **MATCHING** |
| `CDVD_FindFile` | 352 / 352 | 0 | **MATCHING** |

Because the object contains no relocations, this result is also raw byte equality;
no relocation masking contributes to either match.

The readable historical C remains the primary source model. The `.S` file is
only the byte-exact matching reconstruction required after historical-source and
compiler-shape work failed to reproduce these two target instruction layouts.

These two rows are promoted into the authoritative progress manifests by
Progress 56. The strict committed-listing checkpoint moves from
**307/1041 (29.49%)** to **309/1041 (29.68%)**.
