# Recovered code and historical origin

This page records what each recovered part of SNES Station is, where its
historical model came from and how its current claim is verified. It replaces
the old practice of using numbered update documents as the public project
history.

## Main recovered areas

| Area | Historical origin | What was recovered | Current result | Main proof |
|---|---|---|---|---|
| Application and frontend | SNES Station v0.23 binary | Boot flow, menu, input, file browser, configuration and frontend glue | All audited entries have source models and formal byte evidence | `make checkpoint-1041-check` |
| SNES emulation core | Snes9x 1.41 lineage | CPU/APU support, memory, DMA, renderer helpers and special-chip code | Function frontier complete; historical data rebuilt where source identity is known | `make check` |
| Renderer and PlayStation 2 GS | SNES Station binary plus early Hiryu `gsLib` lineage | Draw family, texture upload, GS state and frontend rendering | 30/30 audited draw-family entries reconstructed and mapped | `make check` |
| ZIP and compression | zlib 1.1.3 and Gilles Vollant unzip 0.15 | Deflate/inflate and legacy Shrink, Reduce, Implode and Explode paths | Audited functions complete; version strings and source fingerprints recorded | `make check` |
| PlayStation 2 runtime | Early PS2DEV/PS2LIB lineage | File I/O, RPC, controller, memory-card, kernel and C/C++ runtime providers | 53/53 runtime contracts resolved | `make runtime-overrides` |
| Compiler runtime | GCC 3.2.2-era `libgcc` and `libsupc++` | Arithmetic helpers, exception handling, RTTI and unwind metadata | 7/7 compiler-runtime contracts closed; selected tail metadata linked exactly | `make libgcc-contracts` |
| Startup | Historical PS2 `crt0.s` | `_start`, `_exit`, `_root`, entry point and startup BSS | 276/276 bytes, 3/3 functions and 27 relocations exact | `make startup-integration` |
| Public historical data | Pinned Snes9x, zlib, PGEN/gsLib and PS2 runtime source | Tables, constants, strings, vtables, unwind metadata and typed data intervals | 810,542 typed historical bytes plus a final 35,067-source-byte and 2,220-semantic-byte tranche that closes window 11 | `make window11-rodata-check` |
| Embedded frontend media | Hash-verified private reference | Frontend background/logo/panel, music and Memory Card icon | Six containers integrated; payload remains private and untracked | `make media-assets` |
| Whole-image code | Historical objects, recovered source, public instruction listings and labelled exact assembly | Complete 64 KiB code regions | 51/51 image windows exact; unpacked image hash matches | `make code-windows` |
| Executable packing | SjCRUNCH 2.1 and LZO | LZO1X-999 level 8, 13 blocks and exact container geometry | 714,268/714,268 container bytes exact | `make sjcrunch-packing-check` |
| Loader wrapper | Public SjCRUNCH 2.1 source/object archive by Sjeep | Startup object, loader/miniLZO archive, linker script and empty `.pdr` section | 12,700/12,700 wrapper and metadata bytes exact | `make sjcrunch-outer-elf-check` |
| Final executable | Exact integrated image plus public SjCRUNCH wrapper | Complete packed file and private-reference identity | 726,968/726,968 bytes exact; packed SHA-256 matches | `make reproduce` |

## Exact APU memory source

[`src/snes9x/apumem.cpp`](../src/snes9x/apumem.cpp) isolates the four historical
Snes9x 1.42 `apumem.h` bodies from the archive pinned by SHA-256
`136c7c9bf826bf9dba91073aae14e4b315ab78e67eee189322dec2637e949ffb`.
EE GCC 3.2.2 `-Os` reproduces all **880/880 bytes** at
`0x001ac994..0x001acd04`, including **43 relocations**. The proof checks the
historical object fingerprints and compares every byte after linking the two
state objects and three callbacks to their target addresses. The audit's
Get/SetByte labels exchange the historical Z/non-Z names; the evidence table
records the actual C++ symbols without changing the frozen audit labels.

Run `python3 tools/run-apumem-source-recovery.py` for this corridor, or
`make source-recovery-check` for every maintained recovery proof. The latter
discovers Python and shell runners, includes CDVD and SIF RPC, and keeps each
log plus the full pass/fail list under `build/source-recovery/`. Public
provenance updates preserve frozen payload hashes and comparison results;
changes to section geometry or historical source pins require recapture.

## Historical renderer selector and audio RPC clients

`src/snes9x/selecttilerenderer.cpp` restores the Snes9x 1.42 renderer selector.
Its 66 relocations resolve to the existing renderer owners and target data
addresses; all **348/348 provider-linked bytes** match the committed listing.
The compiler profile and source archive pin are recorded beside the source.

`src/ps2/sjpcm_rpc.c` and `src/ps2/amigamod_rpc.c` restore the public PGEN
clients from `ps2homebrew/pgen@403f1710e5eacb7d04e5031e1cb0a40435ff9d33`.
Minimal historical declarations preserve the 40-byte SIF client and DMA ABI.
AmigaMod initialization uses the target-proven quadword copy, and loading
uses the IOP heap already initialized by the application. The frozen V51
evidence proves these two SNES Station differences from the upstream file.

The audio proof reproduces **18/18 function bodies and 2892/2892 bytes**.
Sixteen historical bodies retain their frozen object fingerprints across
the two RPC suffixes and the two variants. The two log bodies now reproduce
their complete target instructions with the historical 32-bit `memcpy` length;
using the bootstrap's 64-bit `size_t` disables the compiler's EE block copy.
Three SjPCM wrappers and both logs additionally match **872/872 bytes** after
provider linkage. Several old audit names identify different historical
functions; the sidecar records each actual identity at its target address.

Run `python3 tools/run-selecttilerenderer-source-recovery.py` and
`python3 tools/run-audio-rpc-source-recovery.py`, or `make source-recovery-check`
for all runners. The tree now contains **148 canonical translation units**;
1846 external contracts remain; all frozen whole-image claims are preserved.
The SjPCM license notice remains in both source and header, with the complete
LGPL 2.1 text in [`licenses/LGPL-2.1.txt`](licenses/LGPL-2.1.txt).

## Complete cheat management and RAM searches

`src/snes9x/cheats2.cpp` restores all twelve historical cheat-management
functions from the pinned Snes9x 1.41-1 archive. The target uses PS2 FIO
file descriptors, a 32-bit memory-length ABI, and no file removal when the
cheat list is empty. The original ignored `enable` argument, deletion behavior
and saved-byte state are preserved. Twelve bodies reproduce **1768 bytes**;
eleven additionally match **1712/1712 bytes** after resolving real providers.
The save-file witness contains two following comparators; this recovery claims
only the actual 340-byte save function.

`src/snes9x/cheat_search.cpp` restores the four historical RAM-search functions
present in the target build. The three preceding text-code parsers are omitted
by that build. The upstream full object reproduces the frozen `cheats_prefix`
and `cheats_text_tail` raw hashes. Per-function hashes derived from those
validated slices prove **23936/23936 bytes** in the isolated source with only
**15 known relocation fields** normalized. The two large comparison/value
searches reproduce **23364/23364 raw bytes**, with no relocations. Removing
parser strings changes the output routine's string addends; its historical,
isolated and normalized hashes are all recorded separately.

Run `python3 tools/run-cheats-source-recovery.py` or `make source-recovery-check`.
The readable sources retain the full historical copyright and license notice.

## Complete S-RTC source and memory base pointer

`src/snes9x/srtc.cpp` restores all nine functions in the historical Snes9x
1.41-1 S-RTC file. The V48 evidence pins the SNES Station timestamp to signed
32 bits at offset `0x14` and replaces wall-clock reads with zero. The original
pad keeps the eight-byte timestamp/save-state copy intact. The source retains
the historical command state machine, date rollover, SRAM serialization and
calendar rules. Existing canonical data and division providers are shared.

`src/snes9x/rtc_days.cpp` restores the SPC7110 month helper, and
`src/snes9x/getbasepointer.cpp` restores the memory-map base-pointer switch.
`tools/run-calendar-memory-source-recovery.py` links these sources at their
target addresses with their real providers and jump-table placements. All
**11/11 functions, 2300/2300 instruction bytes and 92 relocations** match the
complete public listings. The sidecar preserves both historical full-file
object hashes and isolated object hashes, since table offsets can change
during isolation. Nine source promotions are new; two replace earlier low-level
models with the proved historical bodies. Old models remain available as evidence.

## Preserved source candidates

These files are historical compiler inputs, not the normal recovered source
tree. Their former per-directory READMEs were consolidated here so the reason
for keeping each candidate is visible in one place.

| Candidate directory | Source lineage | Result it preserves |
|---|---|---|
| `matching/candidates/progress60/` | Early PS2SDK/PS2LIB source variants compiled with EE GCC 3.2.2 `-Os` | Ten exact function matches across unzip, FileIO, `libmc`, `libpad` and string helpers |
| `matching/candidates/progress61/` | NEW_PADMAN `libpad` variant | Exact `padInfoMode` at `0x001a8b24` |
| `matching/candidates/progress65/` | PS2LIB-era `libmc.c`, PS2SDK commit `01d625018c3fde3044292446c910b8ea508adfdc` | Six exact memory-card client functions |
| `matching/candidates/progress67/` | Historical NEW_PADMAN object layout | Five exact `libpad` functions and three corrected boundaries |
| `matching/candidates/progress68/` | PS2LIB-era `libpad.c` with the NEW_PADMAN selector | Exact 400-byte `padInit` function and independently verified zero-filled listing gaps |

The `.c.txt` suffix is intentional: these snapshots require the historical EE
headers and compiler, so the host C syntax pass must not treat them as ordinary
translation units. Immutable hashes and per-function results remain in
`analysis/matching/`.

## Evidence policy

- `src/` contains readable recovered source.
- `matching/` contains compiler experiments and exact byte candidates.
- `analysis/` contains the machine-readable manifests that define every count.
- `tools/history/` and `docs/archive/` preserve old investigations without
  presenting them as the current workflow.
- The original ELF and extracted private payloads are never committed. The
  README logo is the sole documentation-only exception described in
  [`LEGAL.md`](LEGAL.md).

## Historical memory-map and ROM metadata methods

`src/snes9x/memmap_methods.cpp` restores eleven methods from the pinned
Snes9x 1.41-1 source: ROM-speed mapping, write protection, SPC7110 SRAM mode,
TV standard, speed, map type, SRAM size, ROM size, cartridge contents, map mode
and ROM identifier. The V51-proved extra PS2 Settings byte keeps `BS` at
`0x12d` and `SETA` at `0x134`. Static return buffers, lowercase `%02x`, and
the original SRAM-mask calculation remain intact.

The full historical object reproduces all eleven frozen code-window hashes;
the isolated source reproduces **1168/1168 bytes** after normalizing only
**98 known relocation fields**. Four methods retain their complete historical
raw object fingerprints (**256 bytes**). String, pointer-table and buffer
addends change when the methods are isolated, so both historical and isolated
hashes are recorded. This proof does not add a complete provider-linked byte
comparison. Run `python3 tools/run-memmap-methods-source-recovery.py`.

## Complete OBC1 and S-DD1 bank mapping

`src/snes9x/obc1.cpp` restores the full five-function OBC1 source with the
V72-proved R5900 packed-word read sequence and original bytewise writes.
The OBC1 RAM pointer and Memory object use the existing target state owners.
The full source reproduces **1276/1276 historical object bytes**; `SetOBC1`
additionally matches the frozen **1116/1116-byte target digest** after resolving
state, jump-table and memset providers. Its real exported `GetOBC1` and
`SetOBC1` definitions close two former external function contracts.

`src/snes9x/sdd1_map.cpp` restores bank mapping, reset, post-load mapping and
the original logged-record comparator: **340/340 historical object bytes**.
Bank mapping and reset additionally match **204/204 provider-linked target
bytes**. PS2 FIO log persistence retains its existing exact source owners.
Run `python3 tools/run-otherchips-source-recovery.py` for all nine bodies.

The current source namespace contains 1848 external contracts, 1517 link
contracts, and 237 provider-frontier entries. These reductions close real
canonical functions; no historical byte proof or whole-image claim changed.

## Complete S-DD1 log persistence and cheat loader linkage

`src/snes9x/sdd1_log_save.cpp` and `src/snes9x/sdd1_log_load.cpp` restore both
S-DD1 log persistence bodies with the V51-proved PS2 FIO adaptation. Together
with the four mapping helpers, all six historical S-DD1 functions now have
canonical C++ source. The writer retains its dirty-counter check, eight-byte
sort and byte-sized file write. The reader preserves the raw `fioRead` return
value in both counters. Shared target state, filename, extension and sorting
providers are retained.

Both historical object fingerprints and all **324/324 provider-linked bytes**
match. The writer uses `-Os`; the reader uses `-O2 -falign-functions=4` because
its target entry `0x0016fbb4` is four-byte aligned. This removes only section
alignment padding; all 148 instruction bytes retain the exact V51 hash.
Run `python3 tools/run-sdd1-logs-source-recovery.py`.

The cheat proof now resolves `fioRead` at its recorded target entry
`0x0019d120` and compares the 304-byte cheat loader to its frozen V73 target
hash. Eleven management bodies now have **1712/1712 provider-linked bytes**;
the 56-byte initialization body retains its historical object proof.
Public ownership refreshes also preserve all captured named-data hashes and
geometry while updating requester lists. Range/hash/roster changes fail before
writing, with dedicated regression coverage.

## Complete historical C4 mathematics

`src/snes9x/c4_math.cpp` preserves the complete six-function Snes9x 1.41-1
C4 mathematics module with the target-proved V75 float/math changes. Both
wireframe transforms, line stepping, angle, distance and vector scaling now
have original readable C++ source. The 132-byte `C4Op15` is retained as an
auxiliary companion between `C4Op1F` and `C4Op0D`; it does not add a row to the
frozen 1041-function audit.

All **2652/2652 provider-linked bytes** and 268 known relocation fields pass.
The runner rebuilds the complete hash-pinned V75 upstream object, verifies its
frozen code-window fingerprint, compares exact normalized instructions, then
links the canonical module to the recorded data, math and runtime addresses.
Signed short operands and float scratch state share the existing target data.
The distance output at `0x0033594c` now has a live canonical consumer; its
existing data contract replaces the retired-consumer marker without changing
the captured range or hash. Fifteen other retired data aliases remain pinned.
The target uses normal 64-bit double constants, `-Os` and `-fno-builtin`.
Compiler libcalls are routed to existing behavioral providers by symbol aliases;
this does not assert new exact runtime-library implementations.

Run `python3 tools/run-c4-math-source-recovery.py`. No private ELF is required,
and all frozen whole-image result and claim fields remain unchanged.

## Original SETA dispatch and ST010 helper source

`src/snes9x/seta_dispatch.cpp` preserves both historical indirect-call
wrappers and shares the existing SETA function-pointer slots. Their real
`S9xGetSetaDSP` and `S9xSetSetaDSP` definitions close two external contracts.
`src/snes9x/st010_helpers.cpp` preserves the SRAM getter, doubled signed
multiply helper and original sine/cosine rotation. The rotation uses the
original short conversion rather than the old model's `lrintf` rounding.
The getter retains its original read logging.

All five bodies reproduce **440/440 historical instruction bytes**. The
rotation additionally matches **248/248 provider-linked bytes** in the public
listing. The getter lies outside the frozen 1041-row audit and is recorded as
an auxiliary recovered body. The ST010 command writer remains outside this
recovery; the frozen 880-byte historical prefix also contains part of that
writer, so this recovery claims only the three complete helper bodies.

Run `python3 tools/run-seta-source-recovery.py`. The runner compiles the
unaltered pinned upstream modules and checks their frozen code windows before
comparing the isolated canonical sources. Frozen whole-image evidence is
unchanged.

The current namespace has 1846 external contracts, 1515 link contracts
(245 blocked), and 235 provider-frontier entries (191 address anchors).
The fixed counts follow the two new canonical SETA definitions; no captured
binary result, range, payload hash or whole-image claim changes.
