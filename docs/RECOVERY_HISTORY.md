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
for all runners. The tree now contains **161 canonical translation units**;
1866 external contracts remain; all frozen whole-image claims are preserved.
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

The ST010 getter's isolated Memory prefix now retains all four original
pointer slots (`RAM`, `ROM`, `VRAM`, `SRAM`), placing SRAM at byte offset 12.
Its complete 68 raw object bytes must equal the historical body as well as
passing normalized comparison. This corrects the prior isolated declaration's
SRAM offset without changing the already exact rotation or dispatch bodies.

## Five original SPC7110 state and persistence helpers

`src/snes9x/spc7110_helpers.cpp` restores RTC update, ROM base lookup and
register/bank50 reset. The RTC retains the target's 32-bit persisted timestamp
and disabled wall clock. ROM lookup reads the original standalone ROM pointer;
the 64 KiB bank50 reset and original register defaults are preserved. All three
bodies match **1092/1092 provider-linked bytes**. Separate function sections
place them across the intervening unrecovered loader without changing any
instruction bytes. The existing exact days-in-month helper is reused.

`src/snes9x/spc7110_rtc_io.cpp` restores the V74 PS2 FIO save/load adaptation.
Both functions retain their 24-byte little-endian record layout, byte-at-a-time
I/O and original ignored transfer results. They match **692/692 provider-linked
bytes**. The following eight-byte empty leaf is outside the 360-byte reader
proof and remains separately owned.

Run `python3 tools/run-spc7110-source-recovery.py` for all five bodies:
**1784/1784 bytes**, with 52 known relocations. The runner rebuilds the pinned
V72/V74 source objects, checks frozen historical fingerprints, compares exact
normalized instructions and links to the recorded real target providers.
Complete public listings also independently check the base lookup and reset.
The source tree has 150 canonical units; all frozen whole-image captures and
claims remain unchanged, with no new private-image rerun.

## Original SPC7110 register and ROM access

`src/snes9x/spc7110_access.cpp` restores the complete register reader/writer
switches and direct ROM byte reader. All **4596/4596 historical instruction
bytes** reproduce the pinned V72 source. The writer matches its frozen
**2480/2480-byte target digest**, and the direct ROM read matches the public
**136/136-byte target listing**. Both real read providers now close former
external contracts used by canonical Snes9x memory access.

The register reader also matches **1980/1980 fully linked historical-reference
bytes**. This distinct proof checks every provider value, shared-state field
and jump-table addend against the rebuilt original module at the recorded core
layout; it does not assert a freshly captured private-target digest for that
reader. The isolated pair of jump tables retains all 132 entries and occupies
528 bytes at `0x001b85dc`. Setter/direct-ROM byte proofs independently validate
the same providers and placement.

Run `python3 tools/run-spc7110-access-source-recovery.py`. The namespace now
contains 1844 external contracts, 1513 link contracts (243 blocked), and 233
provider-frontier entries (189 address anchors). Reductions reflect the two
canonical read functions. All frozen whole-image results and claims remain
unchanged, with no new private-image rerun.

## Complete original CPU reset module

`src/snes9x/cpu_reset.cpp` restores all four original functions:
`S9xResetSuperFX`, `S9xResetCPU`, `S9xReset`, and `S9xSoftReset`.
The complete **956/956 raw historical instruction bytes** match the pinned
Snes9x 1.41-1 CPU module. The full raw comparison retains every state-field
addend, with no relocation masking needed for the whole-module identity.
All four bodies additionally match fully linked historical references at
`0x001159f4` through `0x00115daf`. The hard reset retains its prior normalized
MATCH witness; no new private-target byte digest is asserted.

Shared storage comes from existing address contracts. The native 64-bit
`long` CPU timing fields and original register/status behavior remain intact.
A rebuild of the already frozen **718920-byte GLOBALS data interval** checks
CPU, ICPU, registers, settings, memory, and the 24-byte SuperFX state at
`0x0035b770`. The earlier context-argument reset models do not have the native
callee ABI, so eight native external call contracts retain their recorded
actual target entries instead of binding to incompatible behavioral functions.
This does not claim newly recovered implementations of those eight callees.

Run `python3 tools/run-cpu-reset-source-recovery.py`. The canonical tree now
contains 152 translation units, 1853 external contracts, 1522 link contracts
(251 blocked), and 241 provider-frontier rows (197 address anchors). These
increases record the original module's real dependencies. All frozen
whole-image results and claims remain unchanged.

## Complete original CPU execution module

`src/snes9x/cpu_execution.cpp` restores the entire main loop, IRQ set/clear,
and horizontal-blank processing from Snes9x 1.41-1. All **2448/2448 complete
raw historical instruction bytes** reproduce the public frozen module,
including all 201 relocation addends. Each of the four full functions also
matches a freshly linked historical reference, with native call addresses
and the recorded shared-state layout. No new private-target digest is claimed.

The original APU stepping loop, interrupt delays and WAI wakeup, SA1 dispatch,
frame/vblank transitions, HDMA calls, PPU timers, and SPC700 timer updates
remain readable source. CPU timing uses the original 64-bit `long` fields.
The already frozen complete GLOBALS data interval independently verifies
CPU, APU, PPU, SA1, register, settings, memory and debug-state addresses.
Twelve newly exposed native callee contracts retain their target ABIs;
three bind to existing compatible functions and nine retain address anchors.
implementations of those callees remain separately tracked.

Run `python3 tools/run-cpu-execution-source-recovery.py`. There are now 153
canonical translation units, 1866 external contracts, 1535 link contracts
(260 blocked), and 250 frontier rows (206 address anchors). The original
216-row named-contract and 1265-row address-data ledgers remain intact;
these later native dependencies are validated separately. Frozen whole-image
results and claims remain unchanged.

## Native C4, DMA and SA1 reset implementations

`src/snes9x/native_chip_resets.cpp` restores three real no-argument native
reset routines used by the recovered CPU module. All **416/416 complete raw
historical instruction bytes** match their Snes9x 1.41-1 bodies. The DMA and
SA1 routines match all **376/376 bytes of the public target listing** after
binding the actual shared DMA, memory and SA1 state. The C4 initializer
matches its frozen **40/40-byte original instruction window** and a fully
linked historical reference. No new private-target digest is claimed for C4.

The original eight 22-byte DMA records, sentinel values, register initialization,
SA1 interrupt/arithmetic state, and 8192-byte C4 RAM reset are retained.
No replacement state is allocated. `S9xInitC4`, `S9xResetDMA` and `S9xSA1Init`
now resolve to these original native implementations instead of address anchors.

Run `python3 tools/run-native-chip-resets-source-recovery.py`. The canonical
tree has 154 translation units, 1863 external contracts, 1532 link contracts
(257 blocked), and 247 provider-frontier rows (203 address anchors). The three
reductions close real native call dependencies. All frozen results and claims
remain unchanged.

## Original DSP1 initialization, reset and native dispatch

`src/snes9x/dsp_dispatch.cpp` restores four complete original functions,
covering **200/200 historical instruction bytes** and **200/200 fully linked
historical-reference bytes**. The reset and both dispatch wrappers retain
all 140 raw original instruction bytes. Initializer isolation changes only
the original data relocation for its shared once-only flag.

The initializer reuses `0x00341660`, rather than allocating a new flag;
the original DSP1 state and two function-pointer slots remain at their
recorded target addresses. The original full DSP data-section placement and
the already frozen complete GLOBALS interval check every state/provider
addend. The remaining `InitDSP` math initializer retains its independently
recorded native entry; its implementation remains a separate task.
No new private-target byte digest is claimed for these wrappers.

`S9xGetDSP`, `S9xSetDSP`, and `S9xResetDSP1` now close three real native
contracts used by memory access and CPU reset. Run
`python3 tools/run-dsp-dispatch-source-recovery.py`. The canonical tree has
155 translation units and 1863 external contracts. There are 1532 link
contracts (1277 resolved, 255 blocked) and 245 frontier rows (201 address
anchors). The namespace total is unchanged because three closed calls are
replaced by one remaining initializer and two actual shared-state addresses.
All frozen results and claims remain unchanged.

## Provider-linked native DSP table initialization

`src/snes9x/dsp_table_init.c` restores `InitDSP` through its native
`S9xInitDSP` source contract. The readable historical table-building loop
uses the already proved V51 PS2 adaptation: 2048 float entries per table,
normal 64-bit double angle calculations, and native `cosf`/`sinf` calls.
All **272/272 fully provider-linked target bytes** match the frozen V51
target digest, including all 20 relocation values. The two double constants
retain their actual locations at `0x001b20c8` and `0x001b20d0`.

Both existing 8192-byte cosine/sine tables are reused. No replacement table
or initializer flag is allocated. The native initializer now closes the
last DSP reset-chain math call. Symbol aliases route compiler libcalls to
existing behavioral providers while preserving their prior claim levels;
this does not claim original archive identity for the runtime or math library.

Run `python3 tools/run-dsp-table-init-source-recovery.py`. The canonical
tree now contains 156 translation units, 1862 external contracts, 1531 link
contracts (254 blocked), and 244 frontier rows (200 address anchors).
The historical data and whole-image results/claims remain unchanged,
with no new private-image rerun.

## Complete provider-linked native APU reset

`src/snes9x/apu_reset.cpp` restores original `S9xResetAPU` at `0x0010a934`.
All **1044/1044 provider-linked target bytes** match the frozen V52 digest,
and the complete historical raw instruction stream retains every state-field
addend and all 51 native relocations. The PS2 32-bit memory byte-count ABI
is preserved along with the original 224-byte APU, 60-byte internal APU,
8-byte register state, 64-byte boot ROM, DSP dump and two 256-entry cycle tables.
The full historical GLOBALS data digest independently verifies their geometry.

This closes the native APU call from the CPU reset module. The two original
sound callees remain explicit native address contracts at `0x00177a84` and
`0x00174120`; no signature-incompatible earlier model replaces either call.
No RAM, ROM, table or DSP dump storage is duplicated. The historical address
and named-contract ledgers retain their frozen rosters and claims.

Run `python3 tools/run-apu-reset-source-recovery.py`. The canonical tree now
contains 157 units, 1864 external contracts, 1533 link contracts (255 blocked),
and 245 frontier rows (201 address anchors). No fresh private-image rerun or
whole-image identity is claimed. The namespace-count refresh also restores
the ELF relocation-type parser's original `0xff` mask; count changes must
never alter bit masks or unrelated constants.

## Complete native audio reset, echo and playback rate

`src/snes9x/native_sound_reset.cpp` restores three historical native routines,
with **844/844 fully provider-linked public target bytes**. Echo enable covers
228 bytes at `0x00174120`. The previously address-labelled 616-byte span at
`0x00177a84` is now precisely identified as two original functions: 436-byte
`S9xResetSound` and 180-byte `S9xSetPlaybackRate` at `0x00177c38`.
The playback routine retains the target-proved float intermediate before its
normal-double calculation, and both constants retain `0x001b8440`/`0x001b8448`.

The two native APU sound callees now resolve to original source. The full
historical GLOBALS data digest verifies the 1864-byte sound state (eight
224-byte channels), 48-byte sound status, original echo buffers and filter
arrays. No sound state or buffer is copied or replaced. Existing behavioral
soft-double providers preserve their prior claim level; no archive identity
is inferred. Playback's echo-delay and sound-frequency calls retain their
actual native ABI and addresses for subsequent recovery.

Run `python3 tools/run-native-sound-reset-source-recovery.py`. The canonical
tree contains 158 units, 1866 external contracts, 1535 link contracts (255
blocked), and the same 245-row provider frontier. Historical frozen ledgers,
results and claims remain unchanged, with no new private-image rerun.

## Complete native echo-delay/write and sound-frequency controls

`src/snes9x/native_sound_controls.cpp` restores three more original audio
routines, with **452/452 fully provider-linked target bytes**: 136-byte
`S9xSetEchoDelay`, 52-byte `S9xSetEchoWriteEnable`, and 264-byte
`S9xSetSoundFrequency`. Both echo routines match complete public instruction
listings; frequency matches its frozen V52 target digest. The PS2 float
intermediate, native unsigned 64-bit division/conversion calls, 0.98 double
constant at `0x001b83e8`, and existing 128-byte noise-frequency table are retained.

Playback's native echo-delay and frequency contracts now resolve to original
source, and the echo routines call the newly recovered native echo-enable
implementation. Full historical GLOBALS data verifies the shared sound,
channel and noise-table geometry. Existing runtime providers retain their
prior claim level. No sound state, table or buffer is duplicated.

Run `python3 tools/run-native-sound-controls-source-recovery.py`. The canonical
tree contains 159 units, 1864 external contracts, 1533 link contracts (253
blocked), and 243 frontier rows (199 address anchors). Frozen historical
ledgers, results and claims remain unchanged; no private-image rerun is claimed.

## Complete native PPU resets and palette/controller helpers

`src/snes9x/ppu_reset.cpp` restores five original routines, totalling 3372
linked historical instruction bytes. Both native PPU resets (1280 and 1212
bytes) and 516-byte mouse processing retain every raw historical instruction
and state-field addend, match a fully linked historical reference, and retain
their frozen normalized MATCH witnesses. These three routines have no frozen
complete target digest: their claim remains **linked-historical-reference**.
The 192-byte BGR555 brightness and 172-byte next-controller helpers additionally
match all **364 complete public target-listing bytes**.

The full GLOBALS and Window-35 color-table digests verify original PPU/IPPU,
Settings, Memory and the 512-byte brightness table. The native four-argument
mouse-position call resolves to the already proved eight-byte zero-return leaf
at `0x00104e50`, which writes no output references. The controller's seven-entry
jump table retains `0x001b7dbc`. No PPU, palette or mouse state is duplicated.

Both CPU-reset PPU callees now resolve to native source. The complete Settings
declarations also retain the independently proved PS2 port byte preceding
`ChuckRock`: full target checks require its resulting Justifier field offset.
The earlier CPU/APU/audio routines do not read the shifted fields, and their
existing complete instruction proofs remain unchanged.

Run `python3 tools/run-ppu-reset-source-recovery.py`. The canonical tree has
160 units, 1863 external contracts, 1532 link contracts (251 blocked), and
241 frontier rows (197 address anchors). Frozen historical ledgers, results
and claims remain unchanged. No private-image rerun or full-image identity
is claimed; unselected reference code/data is comparison-only and never executed.

## Complete original native Super FX control module

`src/snes9x/native_fxemu.cpp`, with its original licensed native headers,
restores **all 21 functions and the complete 3456-byte historical text** of
`fxemu.cpp`. The full original raw window retains its frozen digest and all
168 relocations. Every linked instruction matches the original fully linked
historical module, including native register/cache handling, screen pointers,
512-byte reset, execution, breakpoints, stepping and accessors. Code claims
remain **linked-historical-reference**; no fresh private code digest is inferred.

The complete 2280-byte original state/data section independently matches its
frozen provider-linked target digest, including all original dispatch-pointer
and mode-selection cells. The complete 6672-byte instruction-table data section
also retains its frozen raw digest and geometry. The native 2044-byte GSU,
all dispatch arrays and static mode tables are reused through extern aliases.
No replacement GSU or function tables are allocated.

The native `S9xFxReset(FxInit_s*)` contract now closes the last unresolved
callee introduced by the original CPU reset module. Original C++ ABI names
are retained for all other Super FX exports. Run
`python3 tools/run-native-fxemu-source-recovery.py`.

The canonical tree now contains 161 units, 1867 external contracts, 1536 link
contracts (250 blocked), and 240 frontier rows (196 address anchors). Frozen
historical ledgers, results and claims remain unchanged. No fresh private-image
rerun or full-image identity is claimed.

The decomp.dev report groups the same fixed 1041-function universe into 106
current source groups after adding the native Super FX module (previously 105).
Both group-count guards are updated to the actually derived grouping. Function
counts, code-byte totals, frozen image/chunk metrics and target hashes remain
unchanged; source grouping is not a new binary comparison.

## Native Super FX execution and CPU IRQ bridge

The original 184-byte `S9xSuperFXExec` body is now part of
`src/snes9x/ppu_reset.cpp`, and all **184/184 provider-linked target bytes**
match the frozen V51 digest. The routine retains the actual PS2 Settings
layout, register checks, native `FxEmulate(uint32)` calls at `0x001309c4`,
and native `S9xSetIRQ(uint32)` at `0x00116128`. Both callees now have canonical
native implementations, so CPU execution's Super FX address anchor is closed.

The earlier five PPU routines retain their exact proof scope and bytes. Their
shared TU now contains six routines and 3556 native instruction bytes; the
184-byte execution bridge has its own complete target proof. Run
`python3 tools/run-superfx-execution-source-recovery.py`.

The canonical tree remains at 161 units, with 1866 external contracts, 1535
link contracts (249 blocked), and 239 frontier rows (195 address anchors).
The report's current 106 source groups and all frozen historical binary
results/claims remain unchanged. No fresh private-image rerun is claimed.


### Native HDMA setup and scanline execution

Recovered the original native `S9xStartHDMA` (176 bytes at `0x0012b3e8`)
and `S9xDoHDMA` (1292 bytes at `0x0012b498`) in `src/snes9x/native_hdma.cpp`.
Both CPU execution callees now resolve to canonical implementations.
The original channel flags, line counters, indirect addressing, all eight
transfer modes, repeat behavior, Hook VRAM exclusion, Uniracers OAM fix and
64-bit CPU cycle accounting are retained. No native state is duplicated.

The public source proof rebuilds the pinned Snes9x 1.41-1 DMA module with
EE GCC 3.2.2 and checks its frozen complete 9320-byte code-window hash.
Isolation retains every instruction and field addend; only the scanline
jump-table addend changes by 24 bytes when its unused DMA prefix is removed.
Placing that table at `0x001b1f38` produces all 1468 identical bytes against
a fully linked original-source historical reference and reproduces its table.
The frozen normalized MATCH witnesses remain unchanged. These are linked
historical-reference proofs, with no independently captured complete target
digest or fresh private-image rerun claimed.

The frozen full GLOBALS data hash proves the geometry of the original DMA,
CPU, PPU, Settings, debug state, seven-byte game-fix structure, both eight-entry
pointer tables and mode-byte-count table. Native memory callees retain their
already proved ABI and entry addresses; register writes retain the actual
`S9xSetPPU` address contract. Unselected historical DMA context is comparison
context only and is neither executed nor promoted by this proof.

### Native PPU/CPU register access and horizontal timer

Recovered `S9xUpdateHTimer`, `S9xSetPPU`, `S9xGetPPU`, `S9xSetCPU`,
`S9xGetCPU` and the PPU-local OAM/CGRAM helpers in
`src/snes9x/native_ppu_registers.cpp`. All seven routines preserve their original
native signatures and complete historical instructions, totaling **13580 bytes**.
The public proof reproduces the complete frozen 18580-byte historical PPU code
window, the full original GLOBALS data hash and the geometry of shared state.

All selected provider-linked bytes match a fully linked original-source
reference. `S9xGetPPU`, `S9xSetCPU` and the CGRAM helper additionally match
**6292 complete target bytes** from previously frozen public digests. The other
7288 bytes retain linked historical-reference and normalized MATCH evidence;
they are not promoted to independently captured complete target hashes.
The original 2724 jump-table bytes and both eight-byte VRAM tables retain their
placement and values. No CPU, PPU, APU, DMA or controller state is duplicated.

Memory readers and writers and native HDMA now call canonical native register
implementations. `CMemory::FixROMSpeed`, SRTC, SPC7110, IRQ, brightness and Super
FX callees keep their established ABIs. The original zero-argument `rand` uses
its original address contract: the earlier explicit-state model cannot supply
that signature. Run `python3 tools/run-native-ppu-registers-source-recovery.py`.

### Native general DMA and its audited partitions

Recovered the full **6388-byte** `S9xDoDMA` in `src/snes9x/native_dma.cpp`, together
with its local OAM (748 bytes) and CGRAM (532 bytes) helpers. The two audited
boundaries at `0x00129af4` and `0x0012a400` are partitions of the same contiguous
original routine, rather than separate implementations. Both complete partitions
match their frozen target digests after all providers are placed. CGRAM also
matches its complete target digest: **6920 provider-linked target bytes** total.
All **7668 selected bytes** match a fully linked original historical reference;
the OAM helper retains its narrower historical-reference proof scope.

The proof rebuilds the pinned complete 9320-byte DMA code window, preserves
all 24 general-DMA jump-table bytes and checks original shared-state geometry.
All transfer modes, DMA directions, VRAM/OAM/CGRAM writes, S-DD1 lookup,
SPC7110 bank-50 wrapping, SA-1 character conversion, APU execution, HBlank
processing and 64-bit CPU cycle accounting retain the original instructions.
Native allocation calls retain `_Znaj` and `_ZdaPv` address contracts because
the earlier callback/opaque models have incompatible signatures. Run
`python3 tools/run-native-dma-source-recovery.py`.

The integrated tree now has **164 canonical translation units**, **1866 external
contracts**, **1535 link contracts** (1287 resolved, 248 blocked), and **238 provider
frontier rows** (194 address anchors, 39 compatibility-storage providers and five
semantic aliases). The source-address-alias ledger remains 331/345 proved, with
14 intentionally blocked aliases. Generated source-unit totals now derive from
the canonical manifest rather than stale literal counts.

All **71 maintained source-recovery proofs pass**. The report still derives 106
function source groups. All 1041 function-audit results, complete image metrics,
payload hashes and original full-file proof claims remain frozen. Public
requester/provenance hashes were refreshed; no fresh private-image rerun or new
replacement-ELF comparison is claimed.

### Partial-link validation after retiring provisional data consumers

The source-address and link-contract gates now honor the explicit
`retired-target-data` rows in `analysis/source_tree/special_ownership.tsv`.
Those 15 historical identities remain in public ledgers, while their removed
provisional consumers remain absent from the compiled aggregate. The gates
still reject unexpected missing references or a retired consumer returning.
The semantic-alias check also accepts a strong text alias to weak canonical
text, as emitted by EE binutils for `memmove_like` and `memmove`, while rejecting
data symbols and differing addresses. Regression tests cover both cases.

Fresh partial-link checks preserve allocated sections through both zero-byte
gates: 1851 live externals become 1520 after source aliases, then 248 after link
contracts. A namespace-only check using the ten frozen private-asset addresses
closes the remaining **238 -> 0 undefined globals** with the reviewed provider
plan and compatibility storage. It does not consume or verify private asset
payloads, establish image identity or replace the original private-asset gate.

Final repository validation: `make check` passes, including 604 tool tests
(one existing skip), generated-document checks, host syntax and every public
identity/provenance gate. `make source-recovery-check` passes all 71 proofs.

### Native SA-1 memory, registers, DMA and character conversion

Recovered the remaining original `sa1.cpp` module in `src/snes9x/native_sa1.cpp`:
**16 routines, 6172 instruction bytes**, with 12 global native entries and four
local helpers. This covers reset, snapshot restoration, BWRAM mapping, byte/word
access, PC mapping, register reads/writes, character conversion, internal DMA,
variable-length reads and opcode selection. The already recovered 192-byte
`S9xSA1Init` remains in `native_chip_resets.cpp`; initialization and shared state
are not duplicated.

The proof recompiles the pinned Snes9x 1.41-1 archive with EE GCC 3.2.2. It checks
all 6364 original code bytes, including the frozen 6256-byte code-window prefix,
then compares every selected instruction against a fully linked historical
reference. Eight routines additionally match **1632 complete target instruction
bytes** from committed assembly listings. The other 4540 bytes retain their
linked historical-reference and frozen normalized MATCH evidence. All **632
jump-table bytes** match the original linked reference and frozen target digest.
Run `python3 tools/run-native-sa1-source-recovery.py`.

The full frozen GLOBALS data hash validates CPU, Memory, SA-1, SA-1 registers and
OpenBus addresses and extents. The complete **21872-byte original SA1CPU data
section** is also reproduced, proving the four 1024-byte opcode-table provider
geometries. `M0X0` starts at `0x003f5840` and `M0X1` at `0x003f5c40`; these original
identities are checked before linking. The local SA-1 cycle helper is proved at
`0x0015f15c` using its complete target instructions, independently of older
relocation-normalized CPU-helper identity guesses.

Native PPU register access and DMA now call canonical `S9xGetSA1` and `S9xSetSA1`.
The tree contains **165 canonical translation units**, **1868 external contracts**,
**1537 link contracts** (1291 resolved, 246 blocked), and **236 provider-frontier
rows** (192 address anchors, 39 compatibility-storage providers, five semantic
aliases). The four new native opcode-table imports have their own source proof;
the historical 1265-row unnamed-data roster remains unchanged. Runtime requester
refreshes preserve all captured hashes and validate before dependent data gates.

Fresh partial-link checks preserve allocated bytes through both alias gates:
**1853 -> 1522 -> 246** live externals. The namespace-only check binds the ten
frozen private-asset addresses and closes **236 -> 0 undefined globals**, without
supplying private asset payloads. The report now derives **107 function source
groups** from current ownership. The original 1041 function results, 722892 code
bytes and all complete image metrics, payload hashes and proof claims remain
frozen; no fresh private-image or replacement-ELF comparison is claimed.

Validation passes: `make source-tree-check` verifies all 165 EE translation
units, `make check` runs 604 tool tests (one existing skip) and every public gate,
and `make source-recovery-check` passes all **72 maintained proofs**. The native
SA-1 proof also passes after the final source formatting cleanup.

### Native SA-1 instruction dispatch and IRQ entry

Recovered `S9xSA1MainLoop` (256 bytes at `0x0016efa0`) and
`S9xSA1Opcode_IRQ` (296 bytes at `0x0016ded4`) in
`src/snes9x/native_sa1_execution.cpp`. The loop retains pending-IRQ handling,
wakeup from WAI, the three-opcode execution limit, early stopping and the
original PC-at-opcode-start tracking. IRQ entry preserves bank/PC/status stack
writes, OpenBus, decimal/IRQ flags and the original register-selected vector.
Both routines reuse canonical native SA-1 memory callees and shared state.
CPU execution now calls the canonical loop rather than an absolute address.

All **552 raw instruction bytes** preserve every original state-field addend
before relocation and match a fully linked historical reference afterward.
The proof recompiles the complete frozen **67560-byte SA1CPU code window**
and retains both normalized MATCH witnesses. These selected code claims are
linked historical-reference proofs; no independent complete target-code digest
or fresh private-image comparison is inferred from them.

The full GLOBALS source hash checks all 24 historical shared-provider geometries.
Every import in the complete reference object uses its actual original address,
including the native byte/word memory routines and C++ personality. The complete
**21872-byte original SA1CPU data section**, with 1411 relocations, reproduces
its frozen linked target digest `bb4e64679d46f3cc1fa54fc08ccaee1dc1102e8a910a039116a9583dd52d7a64`.
This also checks all four 1024-byte opcode tables and their original helper
addresses. Only the two selected routines are promoted; the full historical
object is comparison context, and no shared data or tables are duplicated.
Run `python3 tools/run-native-sa1-execution-source-recovery.py`.

The canonical tree now has **166 translation units**, **1867 external contracts**
and **1536 link contracts** (1291 resolved, 245 blocked). The provider frontier
contains **235 rows**: 191 address anchors, 39 compatibility-storage providers
and five semantic aliases. Fresh zero-byte alias gates preserve allocated bytes
through **1852 -> 1521 -> 245** live externals; the namespace-only check uses the
ten frozen private-asset addresses and closes **235 -> 0 undefined globals**.
Private payloads are not supplied by that namespace check. The 107 report source
groups, 1041 function-audit results and complete image proof fields remain
unchanged; consumer ownership and public input hashes are refreshed.

Validation passes: canonical EE compilation covers all 166 translation units,
`make source-recovery-check` passes all **73 maintained proofs**, and
`make check` passes all public gates and 604 tool tests (one existing skip).

### Native main-CPU IRQ and NMI entries

Recovered `S9xOpcode_IRQ` at `0x00127b78` and `S9xOpcode_NMI` at
`0x00127e00`, each 648 instruction bytes, in
`src/snes9x/native_cpu_interrupts.cpp`. Both retain native/emulation stack
writes, packed status, OpenBus, decimal/IRQ flags, bank clearing, SA-1 vector
overrides and the original six/twelve-cycle interrupt entry adjustment.
The functions reuse shared CPU, internal CPU, registers, Settings and Memory
storage, plus the canonical native memory callees; no state is duplicated.

All **1296 raw instruction bytes** preserve every original field addend and
match the complete selected instructions in a fully linked historical
reference. The proof independently recompiles the frozen **78772-byte CPUOPS
code window** and GLOBALS state section, checking 28 historical shared-provider
geometries, including the 36-byte internal CPU object. Every import in the full
reference uses its actual address. The frozen normalized target MATCH witnesses
remain unchanged; no independent complete target-code digest is inferred.
Run `python3 tools/run-native-cpu-interrupts-source-recovery.py`.

CPU execution now imports the original C++ symbols `_Z13S9xOpcode_IRQv` and
`_Z13S9xOpcode_NMIv`, replacing its two plain-name address contracts. Its full
**2448 raw and linked historical instruction bytes** remain unchanged. The
canonical `S9xSetPCBase` build also now uses the existing historical source-proof
profile: `VAR_CYCLES`, the original ABI and optimization options restore its
**364 bytes** and timing updates, replacing the prior 284-byte default-profile
object. Its proof uses that same canonical profile and still reproduces all
364 public target-listing bytes with 29 relocations.

The canonical tree has **167 translation units**, **1613 owned definitions**,
**1865 external contracts** and **1534 link contracts** (1291 resolved, 243
blocked). The provider frontier contains **233 rows**: 189 address anchors,
39 compatibility-storage providers and five semantic aliases. Fresh partial
links preserve allocated bytes through **1850 -> 1519 -> 243** live externals;
the namespace-only check binds the ten frozen private-asset addresses and
closes **233 -> 0 undefined globals** without private asset payloads.

The 107 report function groups, 1041 original audit entries, 722892 code bytes
and complete-image proof claims, hashes and results remain unchanged. Consumer
ownership and public input hashes are refreshed. Validation passes:
`make source-recovery-check` runs **74/74 maintained proofs**, canonical EE
compilation covers all 167 units, and `make check` verifies all public gates
and 604 tool tests (one existing skip). No fresh private-image or replacement
ELF comparison is claimed.

### Native controller updates, peripheral callbacks and C4/ST018 register IO

Recovered three PPU controller paths in `src/snes9x/native_controllers.cpp`:
`ProcessSuperScope` (276 bytes at `0x0015cce4`), `S9xUpdateJustifiers` (536 at
`0x0015cea4`) and `S9xUpdateJoypads` (632 at `0x0015d0bc`). They retain opposite
joypad-direction filtering, automatic input-register population, port swapping,
mouse/Super Scope dispatch, beam latching and alternating Justifier players.
Four original eight-byte PS2 callbacks now export their native signatures:
`JustifierOffscreen`, `JustifierButtons`, `S9xReadSuperScopePosition` and
`S9xReadMousePosition`. Their frozen leaf behavior is unchanged. PPU mouse
processing now imports the canonical native mouse callback; all 3372 historical
PPU reset/helper instruction bytes still match the existing source proof.

`src/snes9x/native_chip_io.cpp` supplies `S9xGetC4` (24 bytes at `0x0010c328`),
`S9xGetST018` (8 at `0x001701fc`) and `S9xSetST018` (40 at `0x00170204`).
C4 reuses the original shared RAM pointer. ST018 retains the original `0x81`
dummy read and diagnostic write, including its complete 24-byte target string.
No writable shared state is duplicated. The original Justifier last-player
byte is imported at `0x0042e888`; the independent source has a zero-fill
section, and its consumed one-byte extent and zero hash match the frozen range.

All **1548 selected instruction bytes** match historical source or independent
frontend leaf reconstruction. Six routines reproduce **672 complete target
instruction bytes**, including the full frozen 632-byte joypad target digest.
The remaining four use complete linked historical-reference proofs with frozen
normalized target witnesses. The proof independently recompiles the full
18580-byte PPU code window and GLOBALS data section, checks shared-state geometry,
retains the frozen 24-byte C4 source slice and reproduces all 24 ST018 target data
bytes. Unselected historical context remains comparison-only.
Run `python3 tools/run-native-controller-chip-io-source-recovery.py`.

CPU execution now calls the canonical `S9xUpdateJoypads`. Its one-argument PS2
`S9xReadJoypad` dependency remains a reviewed address provider at `0x00104bbc`;
the earlier multitap model uses an additional RPC context and cannot supply this
ABI. Its later source contract is verified separately from the frozen 216-row
named-data tranche. Original function matching, payload hashes and image claims
are preserved; consumer ownership and dependent public manifest hashes change.

The canonical tree has **169 translation units**, **1624 owned definitions**,
**1862 external contracts** and **1531 link contracts** (1291 resolved, 240
blocked). The provider frontier has **230 rows**: 186 address anchors,
39 compatibility-storage providers and five semantic aliases. Fresh partial
links preserve allocated bytes through **1847 -> 1516 -> 240** live externals,
and the namespace-only check binds ten frozen private-asset addresses and
closes **230 -> 0 undefined globals** without private payloads. The report keeps
107 function groups and all 1041 original audit entries; 337 explicit source
promotions now have checked evidence.

Validation passes: canonical EE compilation covers all 169 units,
`make source-recovery-check` passes **75/75 maintained proofs**, and `make check`
passes every public gate and 604 tool tests (one existing skip). No fresh
private-image or replacement-ELF comparison is claimed.

### 2026-10-08 — Native mode-2 ROM conversion and PS2 message callback

Recovered `S9xDeinterleaveMode2` (544 bytes at `0x001520b8`) and the real
three-argument `S9xMessage` callback (8 bytes at `0x001056b0`) in
`src/snes9x/native_rom_deinterleave.cpp`. The ROM path retains the original
Settings display-color update, block permutation, allocation-failure behavior,
three-way 32-KiB swaps, ROM reinitialization and reset sequence. Settings and
Memory reuse their original shared providers; no writable state is duplicated.
The PS2 message callback retains its independently frozen no-op behavior.

Both routines reproduce **552 complete target instruction bytes**. The isolated
ROM routine differs from historical raw code only at the verified diagnostic
string LO16 addend: the historical literal sits at `.rodata+0xe8`, whereas the
isolated literal begins at zero. Placing both at the original literal address
`0x001b64c0` reproduces the complete frozen target digest. All 56 literal bytes,
including padding, match the independently frozen MEMMAP rodata prefix. The
runner also recompiles the complete historical GLOBALS data section and checks
Settings/Memory geometry. The native `CMemory::InitROM(bool8)` dependency is a
separate reviewed ABI/address contract at `0x001522d8`, not an alias to an older
context model; unselected historical methods remain comparison-only.
Run `python3 tools/run-native-rom-deinterleave-source-recovery.py`.

The canonical tree has **170 translation units**, **1627 owned definitions**,
**1862 external contracts** and **1531 link contracts** (1291 resolved, 240
blocked). The provider frontier remains **230 rows**: 186 address anchors,
39 compatibility-storage providers and five semantic aliases. Fresh partial
links preserve allocated bytes through **1847 -> 1516 -> 240** live externals;
ten frozen private-asset addresses then close **230 -> 0 undefined globals**
in the namespace-only check, without supplying or verifying private payloads.
All 1041 original audit entries and 107 function groups are retained; there are
338 checked source promotions. Frozen code/data results and image claims remain
unchanged; only consumer ownership and public build metadata are refreshed.

Validation: all 170 canonical EE units compile, all **76/76 maintained recovery
proofs** pass, and `make check` passes the public gates and 604 tool tests (one
existing skip). No fresh private-image or replacement-ELF comparison is claimed.

### 2026-10-08 — Native C4 line drawing and sprite disintegration

Recovered `C4DrawLine` (540 bytes at `0x0010cbb0`) and `C4SprDisintegrate`
(580 at `0x0010d4f0`) in `src/snes9x/native_c4_raster.cpp`. The line helper
retains the two endpoint transforms, signed fixed-point stepping, clipping and
two bitplane writes. Sprite disintegration retains its scale/center arithmetic,
packed pixel reads, four-plane writes and 32-bit memset length. Both share the
original C4 RAM pointer and signed-short math state; no writable state is added.
The existing canonical wireframe implementation now calls the native line
helper directly, retiring its previous reviewed semantic link alias.

All **1120 selected instruction bytes**, including original relocation addends,
match the independent archive-pinned PS2 historical source profile. Complete
selected historical placements also match. Sprite disintegration additionally
reproduces **580 complete frozen target instruction bytes**; line drawing keeps
its frozen normalized target witness and linked historical-reference proof.
The runner verifies the frozen source code slices, shared state geometry and
canonical math/memset callee ownership. Unselected historical code remains
comparison-only. Run `python3 tools/run-native-c4-raster-source-recovery.py`.

The canonical tree has **171 translation units**, **1630 owned definitions**,
**1861 external contracts** and **1530 link contracts** (1290 resolved, 240
blocked: 1255 address anchors and 35 semantic aliases). The provider frontier
remains **230 rows**: 186 address anchors, 39 compatibility-storage providers
and five semantic aliases. Fresh partial links preserve allocated bytes through
**1846 -> 1515 -> 240** live externals. The namespace-only check then binds ten
frozen private-asset addresses and closes **230 -> 0 undefined globals** without
supplying or verifying private payloads. All 1041 audit entries, 107 report
function groups and 338 checked source promotions are retained. Original payload
hashes, results and image claims are unchanged; public consumer ownership changes.

Validation: all 171 canonical EE units compile, **77/77 maintained recovery
proofs** pass, and `make check` passes every public gate and 604 tool tests (one
existing skip). No fresh private-image or replacement-ELF comparison is claimed.

### 2026-10-08 — Native C4 wave renderer and bitmap table

Recovered `C4BitPlaneWave` (584 bytes at `0x0010d2a8`) and its original 80-byte
bitmap offset table at `0x00335a00` in `src/snes9x/native_c4_wave.cpp`. The routine
retains wave height sampling, alternating bitplane masks, packed PS2 reads and
bytewise writes. Its C4 RAM comes from the existing original Memory provider.
The table is the sole local data object; no independent runtime state is added.

All **584 complete target instruction bytes** and **80 target data bytes** match
the frozen independent evidence. Four historical table LO16 addends change from
`.data+0x30` to zero in the isolated TU; each difference is checked explicitly,
and placing the native table at the original address reproduces the full target
code digest. The independent source recipe verifies the official archive,
PS2 packed-access patches and V78 allocation profile. Its complete 2176-byte
C4 table prefix matches the frozen window-35 data witness. Unselected historical
code and metadata remain comparison-only.
Run `python3 tools/run-native-c4-wave-source-recovery.py`.

The canonical build now automatically constructs the already-proved
`mips-local-t5-before-t4` compiler profile for this TU alone. It reuses the
historical C++ bootstrap's host build objects and leaves the canonical compiler
unchanged. All **171 prior translation-unit object fingerprints** remain exactly
unchanged; the proof also verifies the canonical cc1plus hash before/after profile
construction. This requires the complete historical C++ bootstrap build tree,
as the existing V78 source recipe already did.

The canonical tree has **172 translation units**, **1632 owned definitions**,
**1861 external contracts** and **1530 link contracts** (1290 resolved, 240
blocked). The provider frontier remains **230 rows**: 186 address anchors,
39 compatibility-storage providers and five semantic aliases. Fresh partial
links preserve allocated bytes through **1846 -> 1515 -> 240** live externals;
ten frozen private-asset addresses then close **230 -> 0 undefined globals**
in the namespace-only check without supplying or verifying private payloads.
All 1041 audit entries, 107 function groups and 338 checked source promotions
are retained. Frozen code/data results, source pins and image claims are preserved;
consumer provenance and canonical build metadata are refreshed.

Validation: all 172 canonical EE units compile, **78/78 maintained recovery
proofs** pass, and `make check` passes every public gate and 604 tool tests (one
existing skip). No fresh private-image or replacement-ELF comparison is claimed.

### 2026-10-08 — Native PS2 sound callback

Recovered the native C-ABI `S9xGenerateSound` frontend callback (8 bytes at
`0x00101904`) in `src/ps2/native_sound_callback.c`. The original PS2 callback is
a no-op; the canonical CPU execution path now resolves directly to this source.
Its archive-pinned declaration/caller and independently frozen raw-equal leaf
reproduce **all eight complete target instruction bytes**. No storage or imports
are added. Run `python3 tools/run-native-sound-callback-source-recovery.py`.
The later CPU address provider is retired without changing the historical
216-row named-data tranche or the existing CPU execution instruction proof.

The canonical tree has **173 translation units**, **1633 owned definitions**,
**1860 external contracts** and **1529 link contracts** (1290 resolved, 239
blocked). The provider frontier has **229 rows**: 185 address anchors,
39 compatibility-storage providers and five semantic aliases. Fresh partial
links preserve allocated bytes through **1845 -> 1514 -> 239** live externals;
ten frozen private-asset addresses then close **229 -> 0 undefined globals**
in the namespace-only check, without supplying or verifying private payloads.
All 172 preceding canonical object fingerprints, 1041 original audit entries,
107 report function groups and frozen code/data results are preserved. There
are now 339 checked source promotions.

Validation: all 173 canonical EE units compile, **79/79 maintained recovery
proofs** pass, and `make check` passes every public gate and 604 tool tests (one
existing skip). No fresh private-image or replacement-ELF comparison is claimed.

Next C4 investigation: the independent high-level `C4DoScaleRotate` body produces
1208 bytes with the portable historical SAR fallback, but its register allocation
and linked digest still differ from the frozen V81 target witness. A second-mask
read variant produces 1240 bytes and also differs. These probes remain research
artifacts outside canonical source; no matching promotion is made for them.
