# Progress 6 summary

> Live scoreboard: [`PROGRESS.generated.md`](PROGRESS.generated.md). Update `analysis/progress_targets.csv` and run `python3 tools/update_progress.py` to refresh the README percentages and generated SVG panel.

Progress 6 crosses the clean boundary after Hiryu GSLIB and reconstructs a large
old PS2DEV/PS2LIB runtime corridor: Hiryu/Sjeep libcdvd, compact libc helpers,
SIF RPC, FileIO, loadfile/IOP control, the old callback-driven formatter, heap
allocation, ASCII ctype/string parsing, program-break/interrupt helpers, and the
SIF command dispatcher.

## Current scoreboard

- **390 reconstructed targets**
- **407 mapped targets**
- **34.30% reconstructed** on the conservative 1,137-JAL proxy
- **35.80% mapped** on the same proxy
- **0.00% matching** — deliberately strict
- renderer draw-family subgrid: **30/30 reconstructed**

The project-wide visual summary is generated as [`assets/progress.svg`](../assets/progress.svg).

## CDVD milestone

The `0x0019be70..0x0019c363` EE client is reconstructed as Hiryu/Sjeep libcdvd.
Target evidence exposes RPC server ID `0x0B001337`, command IDs and shared-buffer
layouts before historical source is used to validate names. Recovered entries
include `CDVD_Init`, `DiskReady`, `FindFile`, `Stop`, `TrayReq`, `getdir`,
`FlushCache`, and `GetSize`.

The target's `TrayReq` quirk is preserved: the mode argument is received but
never copied into the RPC buffer.

## PS2LIB/libkernel runtime milestone

Recovered families now include:

- libc-style `memcpy/memset/str*` helpers at `0x0019c364..0x0019c687`;
- `SifBindRpc`, `SifCallRpc`, request handlers, packet allocators and
  `SifInitRpc/SifExitRpc`;
- old FileIO `fioOpen/Close/Read/Write/Lseek/Mkdir/Putc/Gets` plus callbacks;
- loadfile module/module-buffer RPC, IOP heap and `SifIopReset`;
- old PS2LIB formatter `fmtint/fmtstr/fmtchar/dopr/vsnprintf`, plus
  `vsprintf/printf/sprintf/vprintf` wrappers;
- old heap `_heap_mem_fit/malloc/calloc/memalign/free` without the locks added by
  later PS2SDK revisions;
- `strcasecmp/strncasecmp/strtok/strrchr/strstr/strtol` and ASCII ctype helpers;
- `DIntr`, `EIntr`, `ps2_sbrk`, `EndOfHeap` and adjacent syscall leaves;
- `_SifSendCmd`, `SifSendCmd`, `iSifSendCmd`, `SifInitCmd/SifExitCmd`, command
  handlers and `_SifCmdIntHandler`.

Where structure layout matters, recovered source uses explicit **32-bit EE
addresses** rather than host-native pointers.

See [`CDVD_LIBKERNEL_MAP.md`](CDVD_LIBKERNEL_MAP.md).

## Target-visible behavior preserved

Examples that are intentionally *not* modernized:

- synchronous `SifBindRpc/SifCallRpc` initialize only semaphore max/init counts;
- `fioMkdir` transmits `0x100` bytes although its forced terminator is written
  one local byte beyond that payload;
- `fioGets` bulk-reads then seeks backward instead of consuming one byte at a time;
- module-load requests are exactly `0x200` bytes;
- `SifIopReset` uses a `0x70`-byte packet and lacks the later
  `_iop_reboot_count` update;
- formatter overflow returns the supplied buffer size rather than C99's
  would-have-written length, and formatter precision/zero corner cases follow
  the target state machine;
- `malloc` uses the target's odd initial heap-alignment arithmetic and no later
  allocator semaphore locks;
- `strtok(NULL, ...)` has no initial NULL-state guard and a new all-delimiter
  input can leave older continuation state intact;
- `strtol` saturates to signed **32-bit** limits and writes target errno 34;
- `ps2_sbrk` initializes its break to `0x00450c18` and uses unsigned 32-bit
  break arithmetic under conditional `DIntr/EIntr`;
- the target `SifInitCmd` predates later reboot-count handling.

## Formatter milestone

The old PS2LIB formatter at `0x0019d84c` is no longer a frontier. Its internal
boundaries are reconstructed as:

- `fmtint @ 0x0019d84c`
- `fmtstr @ 0x0019dba8`
- `fmtchar @ 0x0019dd28`
- `dopr @ 0x0019de10`
- output callbacks at `0x0019e274/0x0019e288`
- `vsnprintf @ 0x0019e2e0`

The implementation is a callback-adapted descendant of the historical
Patrick-Powell-style `dopr` family, but target behavior remains authoritative.

## SIF command milestone

The late-linked command corridor is reconstructed from `_SifSendCmd @
0x0019f138` through `SifGetSreg @ 0x0019f57c`, with command interrupt dispatch
at `0x0019fbf0`. Exact target static data reveals the old `cmd_data` layout and
its initial addresses. The SNES Station revision omits the later PS2SDK
`_iop_reboot_count` logic in `SifInitCmd`.

## Matching/toolchain status

Historical SNESticle maps and PS2DEV sources strongly validate archive-member
order, object families and some function sizes. They are **not** matching proof.
Matching remains 0.00% until a candidate historical build produces
byte-identical machine code with relocations/link placement accounted for.

## Validation

Before packaging Progress 6:

- every current `src/**/*.c` unit is compiled with
  `-std=c99 -Wall -Wextra -Werror -fsyntax-only`;
- progress/symbol CSVs are checked for duplicate addresses;
- progress SVG/data are regenerated only from `analysis/progress_targets.csv`;
- focused assembly extracts are stored under `analysis/functions/`, not a full
  target disassembly.

## Next frontier

The recovered late PS2LIB/libkernel support reaches `SifLoadFileInit @
0x0019fd20`. The next clean unclassified function begins at **`0x0019fddc`** and
immediately enters a floating-point math corridor. That is the next contiguous
runtime target; in parallel, higher-value SNES Station-specific work remains in
frontend/config/input, SjPCM/AmigaMod audio glue, and unresolved Snes9x 1.41
core structures.
