# Progress 9 — EE cache, Snes9x mapped memory, CPU shutdown and APU access

Progress 9 crosses the clean `0x001ab3c0` boundary left by the libsupc++
runtime. It reconstructs the EE D-cache synchronization leaves and the complete
Snes9x mapped-memory/APU access corridor through `0x001acd00`, where the code
joins the already-reconstructed pixel-writer tail at `0x001acd04`.

Target R5900 assembly remains authoritative. Historical Snes9x naming is only
accepted where the target's jump tables, globals, cycle accounting and call
shape independently establish the function. Nothing here is claimed
**MATCHING**.

## Hard boundaries

| Address | Function | State |
|---|---|---|
| `0x001ab3c0` | `SyncDCache` | reconstructed |
| `0x001ab440` | `iSyncDCache` | reconstructed |
| `0x001ab4e8` | `S9xGetMemPointer` | reconstructed |
| `0x001ab63c` | `S9xGetByte` | reconstructed |
| `0x001ab900` | `S9xSetByte` | reconstructed |
| `0x001abc28` | `S9xGetWord` | reconstructed |
| `0x001ac024` | `S9xSetPCBase` | reconstructed |
| `0x001ac190` | `S9xSetWord` | reconstructed |
| `0x001ac604` | `CPUShutdown` | reconstructed |
| `0x001ac734` | `GetBasePointer` | reconstructed |
| `0x001ac838` | `SelectTileRenderer` | reconstructed |
| `0x001ac994` | `S9xAPUGetByte` | reconstructed |
| `0x001aca50` | `S9xAPUSetByte` | reconstructed |
| `0x001acb3c` | `S9xAPUGetByteZ` | reconstructed |
| `0x001acbf0` | `S9xAPUSetByteZ` | reconstructed |

The focused target extract is
`analysis/functions/core_getset_apumem_001ab3c0.asm`.

## EE cache leaves

`SyncDCache @ 0x001ab3c0` reads CP0 Status, conditionally enters the target's
`DIntr/EIntr` pair, aligns both bounds to 64-byte cache lines and dispatches
`iSyncDCache @ 0x001ab440`.

The inner function iterates a 0x1000-byte cache-index range in 0x40-byte steps.
For each index it probes both R5900 cache ways using `cache 0x10`, reads CP0
TagLo, forms the physical line address and applies `cache 0x14` only when the
line lies inside the requested range. The repeated `sync` barriers are retained
in the target evidence rather than simplified away.

## 4 KiB Snes9x memory map

The memory core uses a **4 KiB** map granularity: target block indexing is
`(Address >> 12)` and the in-block mask is `0x0fff`. The special-map namespace
contains 18 entries before direct pointers (`MAP_LAST = 18`).

The target keeps separate `Map` and `WriteMap` tables. Direct-map byte/word
accesses charge `MemorySpeed[block]`; word accesses double that cost. Reads from
RAM-marked direct blocks update the CPU shutdown `WaitAddress` from the current
opcode-start PC, while writes clear `WaitAddress`.

The recovered special cases include PPU/CPU/DSP dispatch, LoROM and HiROM SRAM,
BWRAM, SA-1 RAM, C4, SPC7110 DRAM/ROM hooks, OBC RAM and SETA chip paths. The
LoROM/HiROM SRAM address formulas are kept from the target rather than replaced
with newer core implementations.

A target-visible detail is important: `S9xGetWord` and `S9xSetWord` use the
slow byte-by-byte path when `(Address & 0x0fff) == 0x0fff`, i.e. when the word
crosses a **4 KiB map block**, not merely a 64 KiB bank boundary. On the read
slow path the first byte is copied into the target OpenBus byte before the
second byte is fetched; that ordering is preserved in the recovered model.

Several dispatch details are intentionally *not* normalized to a cleaner or
newer Snes9x implementation:

- the read-side slot 5 open-bus path charges 8 cycles while slot 6 returns
  OpenBus without that charge;
- byte writes through slot 6 share the SA-1 RAM write path with slot 11;
- **word writes through slot 6 instead share the fixed SPC7110 storage path with
  slot 13**, while slot 11 remains the SA-1 RAM word path and charges only 8
  cycles;
- `S9xSetPCBase` falls back to the SRAM base for otherwise-unhandled special
  slots in this build.

The slot-6 asymmetry is present in the target jump tables themselves. Progress
9 records it as a build quirk rather than silently forcing the historical enum
semantics onto the binary.

## CPU shutdown helper

`CPUShutdown @ 0x001ac604` is reached from taken-branch CPU handlers. The target
checks the global shutdown option, compares `PC` with `WaitAddress`, and uses
`WaitCounter` plus CPU flag mask `0x880` to decide whether the idle fast path is
safe.

On the fast path it clears `WaitAddress`, optionally executes SA-1 work,
fast-forwards CPU cycles to `NextEvent`, and catches the APU up by repeatedly
charging the opcode cycle table and calling the APU opcode dispatch table.
The non-fast path preserves the target's peculiar `WaitCounter` update rather
than normalizing it.

## Renderer selection bridge

`SelectTileRenderer @ 0x001ac838` installs three function pointers: tile,
clipped-tile and large-pixel. The target's PPU color-math bits select between
the already-reconstructed normal 16-bit, add, subtract, half-add/half-subtract
and fixed-color half families.

This function is especially useful as a cross-check because every installed
address lands on an existing reconstructed renderer entry, including
`DrawTile16 @ 0x00185d8c`, `DrawTile16Add @ 0x0018789c`,
`DrawTile16Sub @ 0x00188804`, the fixed-color variants and the matching large
pixel families.

## SPC700/APU memory window

The four APU access leaves reproduce the direct-page and non-direct-page paths.
The target's `0xf0..0xff` window includes:

- output ports at `0xf4..0xf7`;
- control writes at `0xf1`;
- DSP data access at `0xf3`;
- timer targets at `0xfa..0xfc`, where a written zero becomes `0x100`;
- timer counters at `0xfd..0xff`, which are read-and-cleared;
- the `0xffc0+` IPL-ROM/RAM shadow rule in the direct-page write variant.

For that final rule, the target always writes a separate 64-byte `ExtraRAM`
shadow. If the IPL ROM is currently visible (`ShowROM != 0`), it returns without
changing the active direct page; if the ROM is hidden, it also updates the
direct-page byte. Keeping those two storages distinct avoids a subtle but real
behavioural collapse in a host-side reconstruction.

The target also updates its APU wait-address bookkeeping on the observable port
and timer reads used by the shutdown optimization.

## Contiguous tail closure

`S9xAPUSetByteZ` ends at `0x001acd00`. The very next function is the already
reconstructed `WRITE_4PIXELS @ 0x001acd04`; that renderer-helper corridor runs
through the existing color-math writers and finishes with
`gsDriver_getTexSizeFromInt @ 0x001b0790`.

That final helper returns at `0x001b07d0`. The target image is zero-filled from
`0x001b07d4` through `0x001b087f`, and string/data storage begins at
`0x001b0880` (`"%02d/%02d"`). Therefore Progress 9 closes the **contiguous
code tail** beginning at `0x001ab3c0`; there is no later executable frontier in
that tail to chase.

The next work should return to earlier unmapped SNES Station/Snes9x regions:
frontend/configuration, CPU/PPU core structure recovery, background/object and
Mode 7 rendering, audio glue and save-state serialization.

## Progress accounting

After this checkpoint the conservative 1,137-target proxy is:

- **545 reconstructed** — **47.93%**;
- **610 mapped** — **53.65%**;
- **0 matching** — **0.00%**.

Matching remains zero until candidate historical builds reproduce complete
machine code byte-for-byte after relocation handling.
