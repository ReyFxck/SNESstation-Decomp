# Embedded zlib 1.1.3 map

SNES Station v0.23 embeds **zlib 1.1.3**. This version is pinned by data in the
target itself: the `1.1.3` version string and the classic
`deflate 1.1.3 Copyright 1995-1998 Jean-loup Gailly` string are both present.

This document records target virtual addresses established from the SNES
Station binary and then checked against historical zlib 1.1.3 source. The C in
`src/zlib/` is a behavioral/structural reconstruction; **none of this module is
claimed MATCHING yet**.

## Buffer API and Deflate side

| Address | Function | State |
|---|---|---|
| `0x00190700` | `compress2` | reconstructed |
| `0x001907cc` | `compress` | reconstructed |
| `0x001907e8` | `uncompress` | reconstructed |
| `0x001908bc` | `deflateInit_` | reconstructed |
| `0x001908ec` | `deflateInit2_` | reconstructed |
| `0x00190bb4` | `deflateSetDictionary` | reconstructed |
| `0x00190d10` | `deflateReset` | reconstructed |
| `0x00190dc0` | `deflateParams` | reconstructed |
| `0x00190ec8` | `putShortMSB` | reconstructed |
| `0x00190ef8` | `flush_pending` | reconstructed |
| `0x00190fa0` | `deflate` | reconstructed |
| `0x00191308` | `deflateEnd` | reconstructed |
| `0x00191400` | `deflateCopy` | reconstructed |
| `0x0019165c` | `read_buf` | reconstructed |
| `0x0019170c` | `lm_init` | reconstructed |
| `0x001917b0` | `longest_match` | reconstructed |
| `0x001919a4` | `fill_window` | reconstructed |
| `0x00191b64` | `deflate_stored` | reconstructed |
| `0x00191d4c` | `deflate_fast` | reconstructed |
| `0x001921a8` | `deflate_slow` | reconstructed |

The target Deflate state allocation is `0x16d8` bytes. The research header
models the logical fields used by the recovered code, while separately
recording proven target offsets for the Huffman/bitstream tail of the state.
It is intentionally not treated as a host-native byte-layout struct.

## Inflate interface

| Address | Function | State |
|---|---|---|
| `0x0019272c` | `inflateReset` | reconstructed |
| `0x00192784` | `inflateEnd` | reconstructed |
| `0x00192800` | `inflateInit2_` | reconstructed |
| `0x00192948` | `inflateInit_` | reconstructed |
| `0x0019296c` | `inflate` | reconstructed |
| `0x00192e60` | `inflateSetDictionary` | reconstructed |
| `0x00192f30` | `inflateSync` | reconstructed |
| `0x0019304c` | `inflateSyncPoint` | reconstructed |
| `0x00193094` | `get_crc_table` | reconstructed |
| `0x001930a0` | `crc32` | reconstructed |

The main `inflate()` state machine has all fourteen zlib 1.1.3 modes recovered:
`METHOD`, `FLAG`, `DICT4..DICT0`, `BLOCKS`, `CHECK4..CHECK1`, `DONE`, and `BAD`.

## gzip I/O (`gzio.c`)

| Address | Function | State |
|---|---|---|
| `0x00193298` | `gz_open` | reconstructed |
| `0x00193564` | `gzopen` | reconstructed |
| `0x00193580` | `gzdopen` | reconstructed |
| `0x001935d8` | `gzsetparams` | reconstructed |
| `0x00193674` | `get_byte` | reconstructed |
| `0x0019371c` | `check_header` | reconstructed |
| `0x00193914` | `destroy` | reconstructed |
| `0x00193a34` | `gzread` | reconstructed |
| `0x00193cd8` | `gzgetc` | reconstructed |
| `0x00193d0c` | `gzgets` | reconstructed |
| `0x00193dbc` | `gzwrite` | reconstructed |
| `0x00193e8c` | `gzprintf` | reconstructed |
| `0x00193f08` | `gzputc` | reconstructed |
| `0x00193f44` | `gzputs` | reconstructed |
| `0x00193f88` | `do_flush` | reconstructed |
| `0x001940ac` | `gzflush` | reconstructed |
| `0x001940ec` | `gzseek` | reconstructed |
| `0x001942d4` | `gzrewind` | reconstructed |
| `0x00194378` | `gztell` | reconstructed |
| `0x00194398` | `gzeof` | reconstructed |
| `0x001943c0` | `putLong` | reconstructed |
| `0x0019441c` | `getLong` | reconstructed |
| `0x00194498` | `gzclose` | reconstructed |
| `0x0019450c` | `gzerror` | reconstructed |

The target gzip object is `0x80` bytes with the embedded `zr_stream` occupying
`+0x00..+0x47`; the fd, input/output buffers, CRC, message/path pointers, mode,
and start position follow at the offsets recorded in
`src/zlib/gzio_recovered.c`. Several PS2-port quirks differ from desktop zlib
1.1.3 and are deliberately preserved:

- `gzdopen` formats `"<fd:%d>"`, but `gz_open` never consumes its `fd` argument
  and always opens the formatted path instead;
- the integer file-handle field starts at zero, while cleanup skips close only
  for negative handles, so a sufficiently early open failure can attempt to
  close fd 0;
- `gzflush` does not perform the extra `fflush` present in generic zlib source;
- `gzerror` does not use `strerror` for `Z_ERRNO`; it falls back to the embedded
  zlib error table;
- the emitted gzip header uses OS byte `3`.

These differences come from the target binary and are not normalized back to
upstream behavior.

## Inflate block/Huffman engine

| Address | Function | State |
|---|---|---|
| `0x00194628` | `inflate_blocks_reset` | reconstructed |
| `0x001946e8` | `inflate_blocks_new` | reconstructed |
| `0x001947d0` | `inflate_blocks` | reconstructed |
| `0x00195280` | `inflate_blocks_free` | reconstructed |
| `0x001952e8` | `inflate_set_dictionary` | reconstructed |
| `0x0019532c` | `inflate_blocks_sync_point` | reconstructed |
| `0x0019533c` | `inflate_codes_new` | reconstructed |
| `0x001953ac` | `inflate_codes` | reconstructed |
| `0x00195b10` | `inflate_codes_free` | reconstructed |
| `0x00195b40` | `inflate_fast` | reconstructed |
| `0x00195f80` | `huft_build` | reconstructed |
| `0x001964d4` | `inflate_trees_bits` | reconstructed |
| `0x001965d4` | `inflate_trees_dynamic` | reconstructed |
| `0x001967b0` | `inflate_trees_fixed` | reconstructed |
| `0x001967e8` | `inflate_flush` | reconstructed |

Target-specific layout evidence recovered in this block:

- block state allocation: `0x48` bytes;
- Huffman workspace: `1440` entries × `8` bytes;
- code decoder state allocation: `0x1c` bytes;
- fixed literal/length table VA: `0x00424870`;
- fixed distance table VA: `0x00425870`;
- lower-bit mask table VA: `0x00425970`.

A boundary correction from this pass is worth preserving: `0x001967b0` is the
small `inflate_trees_fixed` leaf and `0x001967e8` is `inflate_flush`. A
prologue-only scanner would miss the leaf and shift the whole map.

## Deflate Huffman writer (`trees.c`)

| Address | Function | State |
|---|---|---|
| `0x00196980` | `tr_static_init` | reconstructed; empty leaf in this build |
| `0x00196988` | `_tr_init` | reconstructed |
| `0x00196a00` | `init_block` | reconstructed |
| `0x00196a94` | `pqdownheap` | reconstructed |
| `0x00196b98` | `gen_bitlen` | reconstructed |
| `0x00196e80` | `gen_codes` | reconstructed |
| `0x00196f24` | `build_tree` | reconstructed |
| `0x001971ec` | `scan_tree` | reconstructed |
| `0x001972f4` | `send_tree` | reconstructed |
| `0x00197874` | `build_bl_tree` | reconstructed |
| `0x00197910` | `send_all_trees` | reconstructed |
| `0x00197bf4` | `_tr_stored_block` | reconstructed |
| `0x00197cb0` | `_tr_align` | reconstructed |
| `0x00197f64` | `_tr_flush_block` | reconstructed |
| `0x001981e8` | `_tr_tally` | reconstructed |
| `0x00198308` | `compress_block` | reconstructed |
| `0x0019879c` | `set_data_type` | reconstructed |
| `0x00198840` | `bi_reverse` | reconstructed |
| `0x0019886c` | `bi_flush` | reconstructed |
| `0x00198904` | `bi_windup` | reconstructed |
| `0x0019897c` | `copy_block` | reconstructed |

`_tr_tally` is another real leaf that does not look like a conventional
stack-frame function boundary. This independently reinforces the earlier
Shrink/Inflate boundary lesson.

## zutil / Adler tail and exact module boundary

| Address | Function | State |
|---|---|---|
| `0x00198a58` | `zlibVersion` | reconstructed |
| `0x00198a64` | `zError` | reconstructed |
| `0x00198a84` | `zcalloc` | reconstructed |
| `0x00198aa4` | `zcfree` | reconstructed |
| `0x00198ac0` | `adler32` | reconstructed |

The target `adler32` exposes the classic `BASE=65521` (`0xfff1`) and
`NMAX=5552` (`0x15b0`) constants and a 16-byte unrolled accumulation loop.
`zlibVersion` directly returns the embedded `1.1.3` string; `zError` indexes the
embedded zlib error-message table.

`adler32` returns at `0x00198c54`. **`0x00198c58` is already a different PS2
GS/video module**, giving the current zlib corridor a clean ending.

## Validation state

Every `src/zlib/*.c` translation unit currently passes host-side
`-std=c99 -Wall -Wextra -Werror`, and the resulting objects can be combined
with `ld -r` without duplicate definitions. Remaining unresolved symbols in
that relocatable research object are expected target tables/address-named
wrappers or normal libc calls; this is not yet a runnable replacement library.

## Matching status

A historical PS2 source tree records EE GCC `3.2.2-b1` and very similar R5900
listings. It remains a **candidate**, not a proven exact toolchain: larger
functions such as `deflateInit2_` already show register-allocation differences.
Matching therefore remains **0.00%** until an actual rebuilt target is byte
compared.
