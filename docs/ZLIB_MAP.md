# Embedded zlib map

The target image contains the literal version string **`1.1.3`** followed by
`deflate 1.1.3 Copyright 1995-1998 Jean-loup Gailly`. This pins the embedded
zlib baseline much more precisely than a generic "old zlib" identification.

## Whole-buffer wrappers

| Address | Function | Status |
|---|---|---|
| `0x00190700` | `compress2` | reconstructed |
| `0x001907cc` | `compress` | reconstructed |
| `0x001907e8` | `uncompress` | reconstructed |
| `0x001908bc` | `deflateInit_` | reconstructed thin wrapper |

The target's `z_stream` size argument is `0x48` (72 bytes) and the build uses
`-mlong64`, which explains the 64-bit `uLong` checks visible around 32-bit
`avail_in/avail_out` fields.

## Large engines mapped so far

| Address | Function | Status |
|---|---|---|
| `0x001908ec` | `deflateInit2_` | identified |
| `0x00190fa0` | `deflate` | identified |
| `0x00191308` | `deflateEnd` | identified |
| `0x00192784` | `inflateEnd` | identified |
| `0x00192948` | `inflateInit_` | identified |
| `0x0019296c` | `inflate` | identified |

These names are supported by their argument constants/call graph and by the
exact zlib 1.1.3 wrapper structure. The next decomp target is the large
Deflate/Inflate implementation beginning at `0x001908ec`.

Recovered behavior for the small wrappers lives in
`src/zlib/zlib_buffer_api_recovered.c`.
