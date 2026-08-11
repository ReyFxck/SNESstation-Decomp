# Legacy ZIP / unzip API map

SNES Station v0.23 carries an unusually complete historical ZIP stack. The
binary includes obsolete PKZIP methods in addition to Stored/Deflate, then the
surrounding **unzip 0.15-style** archive API.

All names below were assigned only after the target control flow/data patterns
were identified. Historical sources are used as post-identification validation.

## Implode / Explode (PKZIP method 6)

| Address | Function | Status / evidence |
|---|---|---|
| `0x0018c124` | `get_tree` | reconstructed; nibble-coded bit-length/repeat stream |
| `0x0018c1f8` | `explode_lit8` | reconstructed; coded literals + 8K dictionary |
| `0x0018c834` | `explode_lit4` | reconstructed; coded literals + 4K dictionary |
| `0x0018ce70` | `explode_nolit8` | reconstructed; raw literals + 8K dictionary |
| `0x0018d3c4` | `explode_nolit4` | reconstructed; raw literals + 4K dictionary |
| `0x0018d918` | `explode` | reconstructed; tree build + four-way flag dispatch |
| `0x0018dc60` | `ReadByte` | reconstructed; compressed-input refill helper |
| `0x0018dd6c` | `huft_build` | reconstructed; linked-allocation Huffman table builder |
| `0x0018e2e0` | `huft_free` | reconstructed |
| `0x0018e318` | `flush` | reconstructed; slide -> output + CRC/counters |
| `0x0018e3ac` | `flush_stack` | reconstructed; Shrink stack -> output |
| `0x0018e440` | `FillBitBuffer` | reconstructed; shared old-ZIP bit-buffer refill |

The `huft_build` implementation is the old Mark Adler / Info-ZIP form that
allocates `(z + 1)` entries and stores a linked-list pointer in the dummy first
entry. It is not the later preallocated `inftrees.c` workspace API.

## Reduce

| Address | Function | Status |
|---|---|---|
| `0x0018e4c0` | `unReduce` | reconstructed |
| `0x0018e92c` | `LoadFollowers` | reconstructed |

The target has the 256 x 64 follower-table model and the classic Reduce
DLE/state-machine expansion.

## Shrink

| Address | Function | Status |
|---|---|---|
| `0x0018ea64` | `unShrink` | reconstructed |
| `0x0018eeb4` | `partial_clear` | reconstructed internal callable block |

`partial_clear` is reached by a direct `jal` but has **no new stack-frame
prologue**. This is a concrete case where prologue-only function discovery
misses a real static helper.

## unzip 0.15-style archive API

The next object begins at `0x0018f010`. The archive API maps cleanly through
`0x001906ff`.

| Address | Function | Status |
|---|---|---|
| `0x0018f010` | `unzlocal_getByte` | reconstructed |
| `0x0018f070` | `unzlocal_getShort` | reconstructed |
| `0x0018f0ec` | `unzlocal_getLong` | reconstructed |
| `0x0018f1b8` | `strcmpcasenosensitive_internal` | reconstructed |
| `0x0018f240` | `unzStringFileNameCompare` | reconstructed |
| `0x0018f27c` | `unzlocal_SearchCentralDir` | reconstructed |
| `0x0018f408` | `unzOpen` | identified; exact target state layout pending |
| `0x0018f5e0` | `unzClose` | reconstructed |
| `0x0018f638` | `unzGetGlobalInfo` | reconstructed; leaf/no normal prologue |
| `0x0018f654` | `unzlocal_DosDateToTmuDate` | reconstructed |
| `0x0018f6cc` | `unzlocal_GetCurrentFileInfoInternal` | identified; large central-dir parser |
| `0x0018fab4` | `unzGetCurrentFileInfo` | reconstructed wrapper |
| `0x0018faec` | `unzGoToFirstFile` | reconstructed |
| `0x0018fb54` | `unzGoToNextFile` | reconstructed |
| `0x0018fbfc` | `unzLocateFile` | reconstructed |
| `0x0018fcec` | `unzlocal_CheckCurrentFileCoherencyHeader` | identified |
| `0x0018ff6c` | `unzOpenCurrentFile` | identified |
| `0x001900d4` | `unzReadCurrentFile` | identified |
| `0x00190458` | `unztell` | reconstructed; leaf |
| `0x00190474` | `unzeof` | reconstructed; leaf |
| `0x001904a0` | `unzGetLocalExtrafield` | reconstructed |
| `0x00190578` | `unzCloseCurrentFile` | reconstructed |
| `0x00190628` | `unzGetGlobalComment` | reconstructed |

`unzReadCurrentFile` contains the expected dispatch for **Stored, Shrunk,
Reduced1-4, Imploded and Deflated** data, tying the three recovered legacy
methods back into the public archive API.

The source file `src/unzip/unzip_api_recovered.c` intentionally uses adapter
structures. Their field order is sufficient for behavior documentation but is
**not yet a claim of exact target struct layout**; the large stateful routines
stay `IDENTIFIED` until those offsets are fully nailed down.

## Boundary after unzip.c

`0x00190700` is not another unzip helper: it starts the next embedded library
block, now identified as **zlib 1.1.3**. See `docs/ZLIB_MAP.md`.

## Historical validation

After identification from the SNES Station binary, equivalent code and PS2
GAS listings were found in the public `iaddis/SNESticle` tree:

- `Gep/Source/common/unzip/explode.c`
- `Gep/Source/common/unzip/unreduce.c`
- `Gep/Source/common/unzip/unshrink.c`
- `Gep/Source/common/unzip/unzip.c`
- `SNESticle/Project/ps2/release_EE3.2.2-b1/*.lst`

Those files are used as validation/fingerprinting material, not as evidence
that every unavailable SNES Station source file was identical to SNESticle.
