# CDVD and old PS2 libkernel runtime map

Progress 6 follows the clean boundary after Hiryu GSLIB at `0x0019be70` and
separates SNES Station's CDVD client from the old PS2DEV/PS2LIB runtime linked
behind it. Target behavior and target addresses remain authoritative; historical
sources and the close SNESticle link map are used only after the binary has
established boundaries, RPC IDs, packet sizes, calls, and constants.

None of this corridor is claimed **MATCHING** yet.

## Hard boundaries

- `0x0019be6c` — return from GSLIB `Dma02Wait`.
- `0x0019be70` — `CDVD_Init`, start of the CDVD client.
- `0x0019c364` — start of linked string/memory helpers.
- `0x0019c688` — `SifBindRpc`, start of SIF RPC client/core runtime.
- `0x0019cfc0` — old FileIO client wrappers.
- `0x0019d600` — loadfile/IOP-heap public wrappers.
- `0x0019d740` — target `SifIopReset`.
- `0x0019d84c` — next large frontier: old PS2LIB `vsnprintf.o` formatter core.

Several helper routines used by this corridor are linked later at
`0x0019f5d0..0x0019fd20`; their call behavior is recovered and recorded below.

## CDVD RPC client

The target binds server ID **`0x0B001337`**. The following command numbers and
buffer behavior are independently visible in the target and then validated
against historical Hiryu/Sjeep libcdvd sources.

| Address | Recovered name | RPC command |
|---|---|---:|
| `0x0019be70` | `CDVD_Init` | bind |
| `0x0019bf00` | `CDVD_DiskReady` | 6 |
| `0x0019bf70` | `CDVD_FindFile` | 1 |
| `0x0019c0d0` | `CDVD_Stop` | 4 |
| `0x0019c128` | `CDVD_TrayReq` | 5 |
| `0x0019c190` | `CDVD_getdir` | 2 |
| `0x0019c2ac` | `CDVD_FlushCache` | 7 |
| `0x0019c304` | `CDVD_GetSize` | 8 |

`TocEntry` is exactly `0x90` bytes in the target reconstruction.

### Preserved CDVD quirk

`CDVD_TrayReq(int mode)` receives `mode` but does **not** copy it to the shared
RPC buffer before command 5. The target executes this omission, so the recovered
source intentionally keeps it rather than correcting the API.

`CDVD_GetSize` is present in the target even though an older seven-function
snapshot does not expose it; later public libcdvd revisions provide command 8
with the same return convention.

## String / memory helpers

The compact block immediately after CDVD is reconstructed as:

```text
19c364 memcpy      19c39c memset      19c3d4 strcat
19c410 strncmp     19c458 memcmp      19c4a0 memmove
19c528 strcpy      19c550 strncpy     19c5e8 strlen
19c610 strchr      19c648 strcmp
```

`0x0019c5a8` and `0x0019c5cc` remain outside the promoted map until their exact
abort/exit identities are proven; a recognizable string is not enough to assign
an original symbol name.

## SIF RPC client/core

| Address | Recovered name |
|---|---|
| `0x0019c688` | `SifBindRpc` |
| `0x0019c7b0` | `SifCallRpc` |
| `0x0019c960` | `rpc_packet_free` |
| `0x0019c978` | `_request_end` |
| `0x0019ca0c` | `search_svdata` |
| `0x0019ca54` | `_request_bind` |
| `0x0019cb08` | `_request_call` |
| `0x0019cba8` | `_request_rdata` |
| `0x0019cc0c` | `SifInitRpc` |
| `0x0019cd4c` | `SifExitRpc` |
| `0x0019cd70` | `_rpc_get_packet` |
| `0x0019ce2c` | `_rpc_get_fpacket` |

The old RPC command family is visible directly in the target:
`0x80000008`, `0x80000009`, `0x8000000a`, and `0x8000000c`.
The packet allocator uses 64-byte slots and the recovered target client layout
is a **0x28-byte 32-bit EE structure** recorded in
`include/ps2_libkernel_recovered.h`.

A useful compiler/link fingerprint is the boundary itself: in the close
historical link map `SifBindRpc` occupies `0x128` bytes, and the target likewise
runs from `0x0019c688` to `0x0019c7b0`. This is strong lineage evidence but not
byte-identical matching proof.

### Semaphore initialization quirk

For the synchronous `SifBindRpc` / `SifCallRpc` path the target writes only the
semaphore `max_count` and `init_count` fields before `CreateSema`; it does not
initialize the later-source `option` field. The reconstruction preserves the
actual target writes.

## Leaf libkernel helpers

Recovered syscall/cache leaves include:

| Address | Name | Target syscall / behavior |
|---|---|---|
| `0x0019ce60` | `iWakeupThread` | `-0x34` |
| `0x0019ce70` | `CreateSema` | `0x40` |
| `0x0019ce80` | `DeleteSema` | `0x41` |
| `0x0019ce90` | `iSignalSema` | `-0x43` |
| `0x0019cea0` | `WaitSema` | `0x44` |
| `0x0019ceb0` | `FlushCache` | `0x64` |
| `0x0019cec0` | `iFlushCache` | `-0x68` |
| `0x0019ced0` | `SifDmaStat` | `0x76` |
| `0x0019cee0` | `SifSetDma` | `0x77` |
| `0x0019cef0` | `SifSetReg` | `0x79` |
| `0x0019cf00` | `SifGetReg` | `0x7a` |
| `0x0019cf10` | `SifWriteBackDCache` | 64-byte cache-line walk, op `0x18` |
| `0x0019f5d0` | `SifStopDma` | `0x6b` |

## Old FileIO client

| Address | Recovered name | Target request size |
|---|---|---:|
| `0x0019cfc0` | `fioOpen` | `0x110` |
| `0x0019d090` | `fioClose` | 4 |
| `0x0019d120` | `fioRead` | `0x10` |
| `0x0019d244` | `fioWrite` | `0x20` |
| `0x0019d360` | `fioLseek` | `0x10` |
| `0x0019d410` | `fioMkdir` | `0x100` |
| `0x0019d4b0` | `_fio_read_intr` | callback |
| `0x0019d534` | `fioPutc` | wrapper |
| `0x0019d558` | `fioGets` | bulk-read wrapper |

Later helper entries are `fioInit @ 0x0019f600` and `_fio_intr @ 0x0019f6e8`.
`fioInit` binds RPC server `0x80000001`.

The exact request sizes are represented with explicit 32-bit EE address fields
rather than host-native pointers, so host validation cannot silently change the
target layouts on a 64-bit machine.

### FileIO quirks preserved

- `fioMkdir` sends exactly `0x100` bytes but writes its forced terminator at the
  next local byte, outside the transmitted payload.
- `fioGets` performs one bulk read and then seeks backward relative to the
  amount consumed when it encounters NUL/newline; it is not a byte-at-a-time
  `fgets` implementation.

## Loadfile, IOP heap and reset

| Address | Recovered name |
|---|---|
| `0x0019d600` | `SifLoadModule` |
| `0x0019d620` | `SifLoadModuleBuffer` |
| `0x0019d63c` | `SifAllocIopHeap` |
| `0x0019d6b8` | `SifFreeIopHeap` |
| `0x0019d740` | `SifIopReset` |
| `0x0019f7e8` | `_SifLoadModule` |
| `0x0019f8f4` | `_SifLoadModuleBuffer` |
| `0x0019f9e8` | `SifInitIopHeap` |
| `0x0019fd20` | `SifLoadFileInit` |

The two module-load RPC request forms are exactly `0x200` bytes. IOP heap binds
server `0x80000003`; loadfile binds `0x80000006`.

`SifIopReset` constructs a **0x70-byte** reset packet, performs a DMA with attr
`0x44`, and then updates the expected SIF flags/registers. Unlike a later
PS2SDK revision, this target routine does **not** increment a global IOP reboot
counter here. The omission is preserved.

## Formatter / heap / text runtime

The old PS2LIB/libkernel formatter corridor is now reconstructed rather than
merely identified. Target boundaries are `fmtint @ 0x0019d84c`, `fmtstr @
0x0019dba8`, `fmtchar @ 0x0019dd28`, `dopr @ 0x0019de10`, output callbacks at
`0x0019e274/0x0019e288`, and public `vsnprintf @ 0x0019e2e0`. The adjacent
`vsprintf`, `printf`, `sprintf` and late-linked `vprintf @ 0x0019faa8` wrappers
are reconstructed too.

`0x0019e414` remains deliberately named `puts_like`: it calls `fioWrite(1, ...)`
without adding a newline.

The following old allocator/text runtime is also reconstructed: `_heap_mem_fit`,
`malloc`, `calloc`, `memalign`, `free`, case-insensitive string helpers,
`strtok`, `strrchr`, `strstr`, `strtol` and the target's ASCII ctype family. The
allocator intentionally omits semaphore locks present in later PS2SDK source.

## Interrupt, program-break and SIF command corridor

`DIntr @ 0x0019f018`, `EIntr @ 0x0019f060`, `ps2_sbrk @ 0x0019f078` and
`EndOfHeap @ 0x0019f5c0` are reconstructed. `ps2_sbrk` initializes the target
break to `0x00450c18` and conditionally restores interrupts after the unsigned
32-bit ceiling check.

The SIF command family is reconstructed as `_SifSendCmd @ 0x0019f138`,
`SifSendCmd`, `iSifSendCmd`, built-in `change_addr/set_sreg`, `SifInitCmd`,
`SifExitCmd`, `SifAddCmdHandler`, `SifGetSreg`, interrupt-safe DMAC wrappers and
`_SifCmdIntHandler @ 0x0019fbf0`. The target revision predates the later
`_iop_reboot_count` handling in `SifInitCmd`.

The next clean unclassified target after late libkernel support is
`0x0019fddc`, which begins a floating-point math corridor.

## Recovered source

- `include/ps2_libkernel_recovered.h`
- `src/ps2/cdvd_rpc_recovered.c`
- `src/ps2/libkernel_strings_recovered.c`
- `src/ps2/sifrpc_recovered.c`
- `src/ps2/libkernel_leaf_recovered.c`
- `src/ps2/fileio_recovered.c`
- `src/ps2/loadfile_iop_recovered.c`
- `src/ps2/libkernel_client_init_recovered.c`
- `src/ps2/stdio_wrappers_recovered.c`
- `src/ps2/ps2lib_formatter_recovered.c`
- `src/ps2/libkernel_heap_recovered.c`
- `src/ps2/libkernel_heap_runtime_recovered.c`
- `src/ps2/libc_text_recovered.c`
- `src/ps2/sifcmd_recovered.c`

Focused assembly extracts live under `analysis/functions/` instead of shipping
a full target disassembly.
