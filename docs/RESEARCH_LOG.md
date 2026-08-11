# Research log

## 2026-08-10 — target frozen

Primary target selected: SNES Station v0.23 WIP, 24 January 2004.

- Packed SHA-256: `4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487`
- Unpacked SHA-256: `739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`
- Raw load base: `0x00100000`
- Entry: `0x00100008`
- Unpacked size: `3,304,936` bytes

A Python SJCRUNCH unpacker was created around the host LZO runtime so the old LZO development headers are not required.

## 2026-08-10 — application boundary

Recovered startup/module loading and mapped the main application flow. The program visibly crosses from frontend into `CMemory::Init` and `CMemory::LoadROM`, then SRAM/APU/emulation, cleanup, and back to the GUI.

Embedded module payloads were identified during analysis but are deliberately not distributed in this repository.

## 2026-08-10 — Snes9x baseline

The binary contains the strings `Snes9x` and `1.41`. Snes9x 1.41 is therefore the primary upstream baseline. Close-era Snes9x 1.43 source has been useful as *post-identification validation*, not as a naming oracle.

## 2026-08-10 — renderer breakthrough

Recovered the 2/4/8-bpp tile lookup-table generation and `ConvertTile` at `0x00183e04`.

The decoded tile representation is 64 bytes per tile. An earlier hypothesis that the cache stored 16-bit decoded pixels was rejected after inspecting the converter and draw stride.

Recovered `DrawTile`, `DrawClippedTile`, `DrawTilex2`, their normal/flipped pixel writers, and `DrawLargePixel`. Mapped the start of the 16-bit renderer family.

## 2026-08-10 — R5900 disassembly gap closed

Within the selected EE code range, LLVM originally emitted 859 `<unknown>` instructions. They were classified as known R5900 forms, dominated by the EE three-operand `MULT`, plus LQ/SQ, EI/DI, MFLO1/MTLO1, MULTU1/DIVU1, and SQRT.S.

`tools/annotate_r5900_unknown.py` now annotates all 859/859 known cases in that range.

## 2026-08-10 — renderer draw-family closed

Recovered the full 30-entry draw-family block through `0x0018bac0`, including 16-bit x2/x2x2, large-pixel paths, and Add/Sub/Half/fixed-colour math. The tracked renderer subgrid is now 30/30 reconstructed.

## 2026-08-10 — legacy ZIP methods recovered

The post-renderer block was initially suspected to be generic DEFLATE, then corrected after the target's nibble-coded tree format and four-way dispatch were analyzed. It is the classic PKZIP method-6 Implode/Explode family, followed by Reduce and Shrink.

Recovered the block from `get_tree` at `0x0018c124` through `unShrink` / internal `partial_clear` at `0x0018ea64` / `0x0018eeb4`. The internal helper has no stack-frame prologue, proving that prologue-only boundary scans miss real functions.

## 2026-08-10 — compiler fingerprint lead

A public PS2 SNESticle tree contains release GAS listings for the same shared legacy-ZIP source and records EE GCC `3.2.2-b1` plus its R5900 release flags. The opening machine words/frame of `get_tree` match SNES Station. This is the first strong route toward real matching builds, but matching remains at 0% until an exact compiler reproduction and full byte comparison are available.

## Current frontier

The embedded zlib 1.1.3 corridor is now behaviorally reconstructed through its
final `adler32` return at `0x00198c54`. The active binary frontier has moved to
the PS2 GS/video block beginning at `0x00198c58`. EE GCC 3.2.2-b1 remains a
strong comparison candidate, not a proven exact compiler.

## Progress 3 — renderer completion, legacy ZIP, unzip API, zlib fingerprint

- Closed all 30 tracked renderer draw-family entry points through `0x0018bac0`.
- Recovered 16-bit color math and x2/x2x2 writers from target masks and calls.
- Identified/recovered Implode/Explode, Reduce and Shrink legacy ZIP methods.
- Mapped unzip 0.15-style API from `0x0018f010` through `0x00190628`.
- Found exact embedded zlib version string `1.1.3`; recovered `compress2`,
  `compress`, `uncompress` and `deflateInit_` wrappers.
- Current frontier moved to zlib `deflateInit2_` at `0x001908ec`.
- SNESticle PS2 GCC 3.2.2-b1 listings remain the strongest matching-toolchain
  lead, but no target is marked MATCHING yet.

## 2026-08-10 — zlib Inflate core closed

Recovered the complete zlib 1.1.3 Inflate interface and its internal DEFLATE
block decoder through `inflate_flush` at `0x001967e8`. This includes the
`inflate_blocks` state machine, slow and fast literal/length-distance decoders,
`huft_build`, dynamic/fixed Huffman tree construction, and circular-window
flush logic.

A boundary correction was made during this pass: `0x001967b0` is the leaf
`inflate_trees_fixed`; `0x001967e8` is `inflate_flush`. The leaf has no normal
stack-frame prologue, reinforcing the earlier Shrink lesson that prologue-only
scans are insufficient.

The next contiguous module begins at `0x00196980`: zlib `trees.c`, the Deflate
Huffman-output side. Initial boundaries for `_tr_init`, `init_block`,
`pqdownheap`, `gen_bitlen`, and `gen_codes` are now mapped.

## 2026-08-10 — toolchain fingerprint correction

The historical SNESticle EE GCC 3.2.2-b1 listing remains an excellent
structural reference, but it is **not** accepted as an exact matching compiler
yet. Small wrappers such as `deflateInit_` have extremely close instruction
shape, while `deflateInit2_` shows different saved-register/allocation choices
inside the function. This can come from compiler revision, flags, source
revision, or surrounding translation-unit differences. Matching therefore
remains 0.00%.

## 2026-08-10 — zlib Deflate / trees.c closed and gzio mapped

Recovered the large Deflate compressor paths that had previously remained only
identified: `deflate`, `longest_match`, `fill_window`, and the stored/fast/slow
engines. The complete `trees.c` Huffman-output side is now behaviorally
reconstructed from `_tr_init` through `copy_block`.

The target `deflate_state` layout was independently accounted for through its
full `0x16d8` allocation. This resolves the tree, heap, literal/distance buffer,
64-bit length-accounting, and bit-buffer offsets instead of treating them as an
opaque state blob.

A symbol-boundary correction was made before publishing the map:
`pqdownheap=0x00196a94`, `gen_bitlen=0x00196b98`,
`gen_codes=0x00196e80`, and `build_tree=0x00196f24`.
`bi_reverse=0x00198840` is proven directly by the target shift/OR loop.

The zutil tail and `adler32` were also recovered at `0x00198a58..0x00198c54`.
`adler32` exposes the canonical zlib constants `5552` and `65521` in the target.

Finally, the previously unnamed gap `0x00193298..0x00194627` was mapped to the
24-function zlib 1.1.3 `gzio.c` API. Evidence includes the gzip magic path,
`0x4000` I/O buffers, the `0x80`-byte gzip stream allocation, and the
`0x1070`-byte `gzprintf` stack frame around its 4096-byte formatting buffer.
The functions are currently marked IDENTIFIED, not RECONSTRUCTED.

## 2026-08-10 — Progress 4: zlib corridor closed

- Reconstructed the remaining Deflate engines: `deflate`, `longest_match`,
  `fill_window`, `deflate_stored`, `deflate_fast`, and `deflate_slow`.
- Reconstructed the complete `trees.c` Huffman-output/bitstream side from
  `_tr_init` through `copy_block`, including leaf `_tr_tally`.
- Recovered `zlibVersion`, `zError`, `zcalloc`, `zcfree`, and `adler32`.
- Corrected a research-header address typo from `0x001a8a84/0x001a8aa4` to
  the target VAs `0x00198a84/0x00198aa4`.
- Host validation: all `src/zlib/*.c` compile with
  `-std=c99 -Wall -Wextra -Werror` and combine with `ld -r`.
- Confirmed a hard module boundary: `adler32` returns at `0x00198c54`;
  `0x00198c58` begins PS2 GS/video code.
- Added a generated Petari-style SVG progress panel and moved the active
  frontier documentation to `docs/PS2_GS_MAP.md`.

## 2026-08-10 — PS2 GS frontier audited

Audited the first non-zlib block directly from the target instead of assigning
historical `gs.c` names by sequence. `0x00198c58` and `0x00198cc8` are
near-duplicate graphics wrapper entries: both call `0x00199590` and then
`0x00198d78` with the same default 320x240 display arguments.

`0x00198d78` is now classified as a partial graphics/display initializer. It
stores display parameters in an object-like state and performs the PS2 GS CSR
reset sequence at `0x12001000`. `0x00199070` is a separate partial low-level
routine that packs display geometry/offset fields and writes a 64-bit value to
privileged GS display register address `0x12000080`.

Historical shared PS2 `gs.c` contains the same classes of low-level operations
(GS reset, CRT setup, display-register packing), so it is retained as
structural validation only. The wrapper/class identity remains unresolved.

## 2026-08-10 — gzip I/O gap closed and zlib boundary finalized

A consistency audit caught that the earlier “zlib corridor closed” wording was
premature: the `gzio.c` interval at `0x00193298..0x00194627` was mapped but had
not yet been converted to C. All 24 entry points are now behaviorally
reconstructed and validated with the rest of `src/zlib/`.

The target contains PS2-specific deviations from generic zlib 1.1.3 that are
now preserved explicitly. `gzdopen` builds `"<fd:%d>"`, while `gz_open` ignores
its fd parameter and opens that text as a path; the integer file field starts
at zero; `gzflush` lacks the generic-source `fflush`; `gzerror` falls back to
the zlib error table for `Z_ERRNO`; and the emitted gzip header uses OS byte 3.

With that correction, the embedded zlib 1.1.3 corridor is behaviorally
reconstructed from `compress2` at `0x00190700` through the final `adler32`
return at `0x00198c54`. The next active module remains PS2 GS/video code at
`0x00198c58`.


## 2026-08-10 — first GS display/buffer cluster reconstructed

Recovered the first self-contained post-zlib PS2 graphics cluster at
`0x00199070..0x001993c0` without assigning an unproven class name.
`0x00199070` reproduces the target DISPLAY1 geometry arithmetic and writes the
privileged register at `0x12000080`; `0x001992f8` writes DISPFB1 at
`0x12000070` from a clamped framebuffer index, width/64 and PSM.

The object fields at `+0x50/+0x54/+0x58/+0x5c/+0x60/+0x64` now form a coherent
display/draw buffer rotation model: current display index, current draw index,
buffer count, pending draw transitions, pending display transitions, and
per-buffer stride. Small helpers from `0x00199178` through `0x00199290` were
reconstructed around that model, and `0x00199360` preserves the target
flush/select/draw-frame/flush call sequence.

The historical shared `gs.c` DISPLAY1/DISPFB formulas validate the operation
class after target-side identification, but the SNES Station wrapper/object
layout remains authoritative.


## 2026-08-10 — GS GIF FIFO and FRAME_1 writer recovered

The draw-buffer helper chain was followed through `0x00199838..0x001998f8` and
`0x00199dc0`. The target uses a double-buffered GIF/DMA list: `0x001998f8` waits
for GIF DMA completion, invokes `FlushCache(0)`, submits the active chain,
switches to the other half of the allocation, writes a DMA END tag, and resets
the packet pointers. `0x00199898` computes remaining bytes and `0x001998b8`
forces a flush below a `0x90`-byte reserve.

`0x00199dc0` is now reconstructed as the GS FRAME_1 A+D packet writer. Its
packed value contains framebuffer base (`address >> 13`), width (`width/64`),
PSM, and FBMSK, followed by register selector `0x4c`. This removes the last
opaque helper from the first recovered draw-buffer path.

## 2026-08-10 — Progress 5: Hiryu GSLIB corridor recovered

Target-first analysis resolved the entire post-zlib graphics block. The binary
itself contains the original `gsPipe` allocation/alignment diagnostics, after
which historical GSLIB by Hiryu validates the class/method names. The target
object layout is exact: `gsPipe` is 0x34 bytes and the embedded-pipe `gsDriver`
is 0x74 bytes.

This corrects an earlier project inference. The `0x74` allocation at the start
of `main`, constructed through `0x00198cc8`, is `gsDriver`; `0x001990f8` is
`gsDriver::clearScreen`. Main preserves the allocator result in `$17`, ignores
the constructor return value, and stores that original pointer globally.

The target uses an older `gsDriver::setDisplayMode` signature with explicit
x/y position and TV mode. It also preserves old behavior absent or commented in
later mirrors: width truncation with `& 0xFFC0`, possible `num_bufs-2` free-count
underflow, and no initialization of the complete-buffer counter in that method.

`gsPipe` is reconstructed from both target packet behavior and historical
lineage. Distinctive target-visible quirks include the assignment operator not
copying ZTest/Filter state, `TextureFlush` writing `0xBAD`, and `TextureSet`
using literal XOR for historical `2^texwidth` / `2^texheight` expressions.
The method order and clean boundary continue through all line/triangle/
rectangle/point/triangle-strip primitive emitters to `0x0019b7ec`.

The following block is `gsFont`: `uploadFont @ 0x0019b7f0`, `Print @
0x0019b948`, `GetCurrLineLength @ 0x0019bad0`, and `PrintLine @ 0x0019bb68`.
Its target layout puts Bold/Underline at +0x30/+0x31 and the 256-byte glyph
width table at +0x32.

Finally, `0x0019bd38..0x0019be6c` matches GSLIB `hw.c`: vertical-retrace
helpers, `DmaReset`, `SendDma02`, and `Dma02Wait`. A compiler-visible bug is
preserved: because historical `VRcount` was not volatile, optimized
`WaitForNextVRstart` sets it to zero and becomes an infinite register-only loop
for every positive argument instead of observing interrupt updates.

The complete recovered GSLIB slice ends at `0x0019be6c`; `CDVD_Init` begins at
`0x0019be70`. Matching remains 0.00% because historical source equivalence is
not a byte-identical target rebuild.

## 2026-08-11 — Progress 6: CDVD and old PS2LIB runtime corridor recovered

The clean boundary after Hiryu GSLIB was crossed at `CDVD_Init @ 0x0019be70`.
Target-side RPC IDs, commands and buffers identify an eight-function
Hiryu/Sjeep libcdvd client through `CDVD_GetSize @ 0x0019c304`. The target
contains the later `GetSize` command 8 while preserving the old `TrayReq` bug:
its `mode` argument is never copied into the shared request buffer.

The compact linked memory/string block at `0x0019c364..0x0019c687` was
reconstructed before following the call chain into old SIF RPC. `SifBindRpc @
0x0019c688` and `SifCallRpc @ 0x0019c7b0` lead into packet allocation, request
handlers, `SifInitRpc/SifExitRpc`, semaphore syscall leaves, DMA/SIF register
leaves and the cache writeback helper. The target packet/client ABI is 32-bit;
a shared recovery header now uses explicit EE address fields where host-native
pointers would otherwise corrupt offsets on a 64-bit validation machine.

FileIO was then recovered from `fioOpen @ 0x0019cfc0` through `fioGets @
0x0019d558`, including exact target request sizes: 0x110 open, 0x10 read, 0x20
write, 0x10 lseek and 0x100 mkdir. The mkdir target forces its terminator one
byte beyond the transmitted 0x100-byte payload, while `fioGets` bulk-reads then
seeks backward rather than consuming one byte at a time.

Loadfile/IOP code follows: `SifLoadModule`, `SifLoadModuleBuffer`, IOP heap
allocate/free and `SifIopReset @ 0x0019d740`. Module RPC requests are 0x200
bytes. The reset packet is exactly 0x70 bytes with DMA attr 0x44. A later
PS2SDK source revision increments `_iop_reboot_count` in this function, but the
SNES Station target does not, so the recovered source keeps the older behavior.

Late-linked helper bodies at `0x0019f5d0..0x0019fd20` resolve `SifStopDma`,
`fioInit`, `_fio_intr`, `_SifLoadModule`, `_SifLoadModuleBuffer`,
`SifInitIopHeap`, and `SifLoadFileInit`.

The next large contiguous module begins at `0x0019d84c`. Historical link-map
geometry and target wrapper boundaries identify it as old PS2LIB
`vsnprintf.o`, not Newlib `vfprintf`. `vsprintf @ 0x0019e364` and `sprintf @
0x0019e3d0` are reconstructed as small wrappers; the large formatter core stays
IDENTIFIED until its internal callbacks/state machine are audited. The
`0x0019e414` string writer is behaviorally reconstructed but remains named
`puts_like`, since the target appends no newline and therefore is not standard
`puts` semantics.

Matching remains 0.00%: archive-member order and object sizes are strong
lineage/toolchain evidence, not a substitute for a byte-identical rebuild.

## 2026-08-11 — Progress 6 continued: formatter, heap and SIF CMD closed

The initially identified `vsnprintf.o` frontier was audited instruction by
instruction. `fmtint @ 0x0019d84c`, `fmtstr @ 0x0019dba8`, `fmtchar @
0x0019dd28`, `dopr @ 0x0019de10`, its two output callbacks, and `vsnprintf @
0x0019e2e0` are now reconstructed. Target-specific non-C99 behavior is kept,
including overflow returning the supplied capacity, the unusual precision
padding paths, and the zero-value/zero-precision integer shortcut. The
late-linked `vprintf @ 0x0019faa8` has the same 0x58-byte object geometry and
0x1000-byte scratch-buffer shape seen in the close historical map, but remains
RECONSTRUCTED rather than MATCHING.

The runtime then resolves an old unlocked PS2 allocator and ASCII libc family:
`_heap_mem_fit/malloc/calloc/memalign/free`, case-insensitive comparisons,
`strtok`, `strrchr`, `strstr`, `strtol`, and ctype helpers. `strtok` preserves
its two-global-state corner cases. `strtol` visibly clamps at 0x7fffffff /
0x80000000 and writes target errno 34 on overflow.

At `0x0019f018`, direct R5900 `di/ei` helpers lead into `ps2_sbrk @ 0x0019f078`.
The target initializes the break to 0x00450c18, uses unsigned 32-bit arithmetic,
checks `EndOfHeap` via syscall 0x3e, and only restores interrupts when they were
enabled on entry.

Finally, the seven-argument ABI at `0x0019f138` independently identifies
`_SifSendCmd`. The full old SIF command chain is reconstructed through wrappers,
`change_addr/set_sreg`, `SifInitCmd/SifExitCmd`, handler registration and
`_SifCmdIntHandler @ 0x0019fbf0`. Exact target initialization data at 0x00425a88
confirms the old command-data layout and buffer addresses. Unlike later PS2SDK,
this target `SifInitCmd` has no reboot-count refresh path.

Progress reaches 390 reconstructed / 407 mapped targets (34.30% / 35.80% on the
conservative 1,137-JAL proxy). Matching remains 0.00%. The next clean
unclassified function begins at 0x0019fddc and enters floating-point math code.

## 2026-08-11 — Progress 7: mathfp, libmc, libgcc/unwind and libpad

The former floating-point frontier at `0x0019fddc` resolves to Newlib 1.10
`mathfp`. `cosf/sinf/tanf/atanf` match the historical polynomial families, while
the target replaces generic `sqrtf/fabsf` with direct EE instructions. The
`numtestf` classifier preserves an R5900/`-mlong64` sign-extension quirk, so the
recovered source documents target behavior instead of silently repairing it.

The next contiguous block is old `libmc`, reconstructed through async `mcSync`
and `SifCheckStatRpc`. It is an earlier revision than the later public PS2SDK
snapshot: `mcInit` has no reboot-count reset prelude, and several historical
quirks remain visible.

The linked GCC runtime then produced the strongest toolchain fingerprint yet.
Multiple archive objects have exact EE GCC 3.2.2-b1 historical text sizes,
including two consecutive 0x6c8 DI division/remainder objects, a sequence of
soft-double packing helpers, `unwind-dw2.o = 0x1f00`, and
`unwind-dw2-fde.o = 0x18c0`. Public `_Unwind_*` symbols retain the same offsets
inside those objects. This sharply identifies the runtime/toolchain family but
still does not count as byte-identical MATCHING.

At `0x001a8420`, the runtime changes to NEW/XPADMAN `libpad`. RPC IDs
`0x80000100/0x80000101` and commands 0x06..0x12 align with the XPADMAN path and
with modules loaded during startup. The client is reconstructed through
`padGetConnection @ 0x001a9080`; `padInit` stays PARTIAL only because the
research model does not reproduce its exact bind busy-wait timing.

Immediately afterward, `operator delete/delete[]`, a duplicated unwind-pe helper
set, GNU C++ personality helpers, terminate/unexpected, throw/rethrow, and
`operator new/new[]` establish the libsupc++ exception boundary. The next clean
frontier is `0x001a9fa8`, where `std::type_info` / RTTI begins.

Progress reaches 485 reconstructed / 544 mapped targets (42.66% / 47.85% on the
conservative 1,137-JAL proxy). Matching remains 0.00%.


## 2026-08-11 — Progress 8: libsupc++ RTTI and exception core closed

The `0x001a9fa8` frontier resolves cleanly to `std::type_info`. Target RTTI
name strings and vtable address points independently establish
`__class_type_info`, `__si_class_type_info`, `__vmi_class_type_info`,
`std::bad_cast`, `std::bad_typeid`, `std::exception`, `std::bad_exception`, and
`std::bad_alloc`. Destructor triples and the small `type_info` virtual leaves
are reconstructed directly from target assembly.

The class RTTI virtual slots then identify catch, upcast, dynamic-cast, and
public-source helpers. The compact class/si upcast leaves are reconstructed; the
large VMI ambiguity/virtual-base walkers remain IDENTIFIED so the project does
not turn a correct symbol name into an unsupported source-reconstruction claim.
`__dynamic_cast @ 0x001aae70` is likewise mapped through its vtable[-1] whole
type, vtable[-2] offset-to-top, `__do_dyncast` call, and public-source fallback.

At `0x001aafb8`, `__cxa_allocate_exception` proves a 0x50-byte exception header
and a four-slot 512-byte emergency pool. `__cxa_free_exception`,
`__cxa_begin_catch`, `__cxa_end_catch`, `std::uncaught_exception`, the shared
standard-exception `what()` body, both EH-global getters, `std::set_new_handler`,
and the final `std::bad_alloc` destructors are reconstructed through
`0x001ab3bc`.

The next instruction at `0x001ab3c0` starts an unrelated EE CP0/cache path,
providing a clean runtime boundary. Progress reaches 530 reconstructed / 595
mapped targets (46.61% / 52.33% on the conservative 1,137-JAL proxy). Matching
remains 0.00%.

## 2026-08-11 — Progress 9: cache/getset/APU tail corridor closed

The `0x001ab3c0` frontier immediately after libsupc++ resolves to two EE D-cache
synchronization functions. `SyncDCache` conditionally brackets an aligned range
with `DIntr/EIntr`; `iSyncDCache` probes both R5900 cache ways across a 0x1000
index span using `cache 0x10`, CP0 TagLo and `cache 0x14`.

At `0x001ab4e8` the binary switches directly into Snes9x mapped-memory access.
The target proves a 4 KiB map (`Address >> 12`, mask `0xfff`) with 18 special
map codes before direct pointers. `S9xGetMemPointer`, byte/word reads and writes,
`S9xSetPCBase`, `CPUShutdown`, and `GetBasePointer` are reconstructed while
preserving the target's direct-map `MemorySpeed[block]` accounting and its
0xFFF word-crossing slow path. That boundary read stores its first byte into
OpenBus before the second access. The target's SetWord slot 6 also unexpectedly
shares the fixed SPC7110 storage path with slot 13 even though byte access treats
slot 6 differently; the raw jump table was re-read from the ELF and the quirk is
preserved rather than normalized.

`CPUShutdown @ 0x001ac604` exposes the classic branch-idle optimization in the
target: repeated branches to `WaitAddress` can fast-forward CPU cycles to
`NextEvent`, optionally execute SA-1 work and run the APU opcode table until its
cycle counter catches up.

`SelectTileRenderer @ 0x001ac838` then bridges directly into the renderer work
already recovered earlier. Its three installed function pointers select the
normal/add/sub/half/fixed-color 16-bit tile, clipped and large-pixel families,
and every target address resolves to an existing reconstructed renderer entry.

Finally, `S9xAPUGetByte/SetByte` and their Z direct-page variants reconstruct the
SPC700 `0xf0..0xff` hardware window: ports, control, DSP, timer targets,
read-clear timer counters and the `0xffc0` IPL-ROM shadow rule. The latter uses
a distinct 64-byte ExtraRAM shadow: it is always updated, while the visible
direct page is left untouched whenever ShowROM is set.

The last new function ends at `0x001acd00`; `WRITE_4PIXELS` begins immediately at
`0x001acd04`. Because the existing pixel-writer tail is already reconstructed
through `gsDriver_getTexSizeFromInt @ 0x001b0790`, the contiguous recovered code
tail now reaches its return at `0x001b07d0`. Zero padding follows to
`0x001b087f`, with string/data storage beginning at `0x001b0880`.

Progress reaches 545 reconstructed / 610 mapped targets (47.93% / 53.65% on the
conservative 1,137-JAL proxy). Matching remains 0.00%.
