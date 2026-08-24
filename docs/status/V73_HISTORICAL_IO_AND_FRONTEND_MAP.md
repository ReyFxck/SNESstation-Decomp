# V73: historical PS2 I/O proof and frontend ownership map

V73 runs the two closing tracks together without weakening the formal gate.
The historical-source track promoted two PS2 file-I/O adaptations, raising the
checkpoint from **985/1041** to **987/1041** and leaving **54** audited entries.
The translation-unit track classified all **40** remaining frontend-facing
entries into five bounded work packets.

> This is a frozen V73 checkpoint. V74 promotes the two SPC7110 rows and
> corrects the provisional multitap/APU packet labels in the complete
> [`V74_SPC7110_RTC_AND_FRONTIER_MAP.md`](V74_SPC7110_RTC_AND_FRONTIER_MAP.md).

## Two new strict matches

| Address | Historical identity | Bytes | Boundary | Proven target delta |
|---:|---|---:|---|---|
| `0x00114544` | `S9xLoadCheatFile` | 304 | terminal `jr $ra`; next function prologue pinned | `fopen`/`fread`/`fclose` replaced by `fioOpen`/`fioRead`/`fioClose` |
| `0x001726ec` | `S9xSPCDump` | 488 | exact next-manifest boundary | PS2 `fio` calls, one-result writes and the `-128` DSP-tail seek |

Both rows pass against the hash-pinned unpacked ELF with zero differing
non-relocation bytes and no unknown MIPS relocation type. The immutable result
is [`hunt1041-v73-validated-2.tsv`](../../analysis/matching/hunt1041-v73-validated-2.tsv).

Reproduce it with the private reference and historical EE C++ compiler:

```bash
cp /path/to/SNES_EMU.ELF original/SNES_EMU.ELF
make hunt1041-v73-evidence
```

## Historical track still open: 14

The other members of the investigated 16-entry batch remain `RECONSTRUCTED`.
They were not promoted from source resemblance alone.

| Corridor | Entries | Current blocker |
|---|---:|---|
| DSP-1 floating transforms | 3 | SNES Station-specific floating implementation; official 1.40/1.42 functions differ in size and instructions |
| Memory initialization and IPS | 2 | allocator/layout and PS2 stream/patch behavior differ from the official memory object |
| ZSNES snapshot import | 1 | custom `fio` path plus target-specific unaligned reads and state layout |
| Sound mixer and sample runtime | 5 | PS2 channel/state layout and mixer arithmetic differ from the official `SOUNDUX` object |
| SPC7110 cache/index I/O | 3 | custom file cache, packed records and PS2 `fio` ownership span several historical functions |

## Frontend track: 40 entries, five work packets

| Work packet | Entries | Working translation unit | First gate |
|---|---:|---|---|
| Frontend UI / GS ownership | 15 | `src/ps2/frontend_ui_recovered.cpp` | split class/global ownership before per-function comparison |
| Multitap | 2 | `src/ps2/libmtap_recovered.c` | pin the old PS2SDK/libmtap ABI and rebuild the omitted calls |
| Main and frontend lifecycle | 9 | `src/app/main_frontend_recovered.cpp` | freeze path, memory-card and application globals plus link order |
| APU core corridor | 12 | `src/snes9x/apu_ps2_recovered.cpp` | recover identities and the PS2 APU state layout in one historical object |
| SPC7110 record I/O | 2 | `src/snes9x/spc7110_ps2_io_recovered.cpp` | rebuild the fixed 24-byte `fio` record path and prove its terminal boundaries |

The complete address-level queue is
[`hunt1041-v73-frontend-map-40.tsv`](../../analysis/matching/hunt1041-v73-frontend-map-40.tsv).
The proposed files are working ownership boundaries, not claims about original
filenames. Each entry still requires an EE GCC 3.2.2 object comparison against
the private ELF before promotion.

Function closure is only the first whole-program gate. Data/rodata placement,
constructors, vtables, archive and object order, the linker script and
SJCRUNCH2 packing remain necessary for a byte-identical final ELF.
