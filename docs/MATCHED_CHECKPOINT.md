# Matched source checkpoint

Checkpoint date: 2026-08-15.

## Closed committed-listing gates: 352 functions

### Newlib mathfp — 7/7

- `0x0019fddc` `cosf`
- `0x001a0024` `sinf`
- `0x001a0254` `tanf`
- `0x001a045c` `atanf`
- `0x001a06a0` `sqrtf`
- `0x001a06b0` `fabsf`
- `0x001a06c0` `numtestf`

Evidence: `analysis/matching/mathfp-listing-report.md`

### libgcc unwind — 7/7

- `0x001a3dc0` `size_of_encoded_value`
- `0x001a3e30` `base_of_encoded_value`
- `0x001a3ee8` `read_uleb128`
- `0x001a3f28` `read_sleb128`
- `0x001a40e8` `_Unwind_GetLanguageSpecificData`
- `0x001a40f0` `_Unwind_GetRegionStart`
- `0x001a40f8` `_Unwind_GetDataRelBase`

Evidence: `analysis/matching/libgcc-unwind-leaves-listing-report.md`

### GSLIB hardware — 7/7 strict

- `0x0019bd38` `VRstart_handler`
- `0x0019bd50` `WaitForNextVRstart`
- `0x0019bd78` `TestVRstart`
- `0x0019bd88` `ClearVRcount`
- `0x0019bd98` `DmaReset`
- `0x0019be20` `SendDma02`
- `0x0019be40` `Dma02Wait`

Evidence: `analysis/matching/gslib-hw-listing-report.md`

### Legacy ZIP `get_tree` — 1/1 strict

- `0x0018c124` `get_tree`

Evidence: `analysis/matching/get-tree-listing-report.md`

The readable K&R C is preserved in `matching/candidates/get_tree.c`; the
formal matcher uses the clearly labelled `.S` reconstruction because the
surviving SNESticle EE3.2.2-b1 listing emits 208 bytes while the target is 212.

### Old EE libkernel syscall leaves — 19/19 strict

Core gate:

- `0x0019ce60` `iWakeupThread`
- `0x0019ce70` `CreateSema`
- `0x0019ce80` `DeleteSema`
- `0x0019ce90` `iSignalSema`
- `0x0019cea0` `WaitSema`
- `0x0019ceb0` `FlushCache`
- `0x0019cec0` `iFlushCache`
- `0x0019ced0` `SifDmaStat`
- `0x0019cee0` `SifSetDma`
- `0x0019cef0` `SifSetReg`
- `0x0019cf00` `SifGetReg`

Late gate:

- `0x0019f5d0` `SifStopDma`
- `0x0019f5e0` `iSifSetDma`
- `0x0019f5f0` `SifSetDChain`

IRQ-tail gate:

- `0x0019fcd0` `_EnableDmac`
- `0x0019fce0` `_DisableDmac`
- `0x0019fcf0` `SignalSema`
- `0x0019fd00` `PollSema`
- `0x0019fd10` `iSifSetDChain`

Evidence:
`analysis/matching/libkernel-syscalls-core-listing-report.md` and
`analysis/matching/libkernel-syscalls-late-listing-report.md`, and
`analysis/matching/libkernel-syscalls-irq-tail-listing-report.md`.

### Old EE libkernel interrupt state — 2/2 strict

- `0x0019f018` `DIntr`
- `0x0019f060` `EIntr`

Evidence: `analysis/matching/libkernel-intr-listing-report.md`.

Historical PS2DEV source establishes behavior; the formal `.S` records the
target's exact old compiler codegen and is explicitly labelled reconstruction.

### Old EE libkernel size-optimized strings — 4/4 strict

- `0x0019c364` `memcpy`
- `0x0019c39c` `memset`
- `0x0019c550` `strncpy`
- `0x0019c5e8` `strlen`

Evidence: `analysis/matching/libkernel-size-strings-listing-report.md`.
Historical source is the old `kernel.S` `__OPTIMIZE_SIZE__` path.

### Old EE libc assembly strings/memory — 7/7 strict

- `0x0019c3d4` `strcat`
- `0x0019c410` `strncmp`
- `0x0019c458` `memcmp`
- `0x0019c4a0` `memmove`
- `0x0019c528` `strcpy`
- `0x0019c610` `strchr`
- `0x0019c648` `strcmp`

Evidence: `analysis/matching/libkernel-libc-strings-listing-report.md`.
Historical source is the old PS2DEV/PS2LIB EE assembly family at
`duduclx/PS2DEV@bac0006c6302edcf1bdae253799484497b4e5032`.

### GCC 3.2.2/libsupc++ small runtime — 48/48 strict

EH/runtime small helpers: **11/11**. RTTI/libsupc++ small functions: **37/37**.

- `0x001a90f8` `operator_delete`
- `0x001a9118` `operator_delete_array`
- `0x001a9138` `size_of_encoded_value_eh_personality`
- `0x001a9268` `read_uleb128_eh_personality`
- `0x001a92a8` `read_sleb128_eh_personality`
- `0x001a9ca8` `__terminate`
- `0x001a9cf0` `std_terminate`
- `0x001a9d08` `__unexpected`
- `0x001a9d20` `std_unexpected`
- `0x001a9d38` `std_set_terminate`
- `0x001a9d48` `std_set_unexpected`
- `0x001a9fa8` `std_type_info_destructor_D2`
- `0x001a9fb8` `std_type_info_destructor_D1`
- `0x001a9fc8` `std_type_info_destructor_D0`
- `0x001a9ff0` `std_bad_cast_destructor_D2`
- `0x001aa018` `std_bad_cast_destructor_D1`
- `0x001aa040` `std_bad_cast_destructor_D0`
- `0x001aa078` `std_bad_typeid_destructor_D2`
- `0x001aa0a0` `std_bad_typeid_destructor_D1`
- `0x001aa0c8` `std_bad_typeid_destructor_D0`
- `0x001aa100` `std_type_info_is_pointer_p`
- `0x001aa108` `std_type_info_is_function_p`
- `0x001aa110` `std_type_info_do_catch`
- `0x001aa128` `std_type_info_do_upcast`
- `0x001aa130` `class_type_info_destructor_D2`
- `0x001aa158` `class_type_info_destructor_D1`
- `0x001aa180` `class_type_info_destructor_D0`
- `0x001aa1b8` `si_class_type_info_destructor_D2`
- `0x001aa1e0` `si_class_type_info_destructor_D1`
- `0x001aa208` `si_class_type_info_destructor_D0`
- `0x001aa240` `vmi_class_type_info_destructor_D2`
- `0x001aa268` `vmi_class_type_info_destructor_D1`
- `0x001aa290` `vmi_class_type_info_destructor_D0`
- `0x001aa390` `class_type_info_do_find_public_src`
- `0x001ab200` `std_uncaught_exception`
- `0x001ab228` `std_exception_destructor_D2`
- `0x001ab238` `std_exception_destructor_D1`
- `0x001ab248` `std_exception_destructor_D0`
- `0x001ab270` `std_bad_exception_destructor_D2`
- `0x001ab298` `std_bad_exception_destructor_D1`
- `0x001ab2c0` `std_bad_exception_destructor_D0`
- `0x001ab2f8` `std_exception_what`
- `0x001ab308` `__cxa_get_globals_fast`
- `0x001ab318` `__cxa_get_globals`
- `0x001ab328` `std_set_new_handler`
- `0x001ab338` `std_bad_alloc_destructor_D2`
- `0x001ab360` `std_bad_alloc_destructor_D1`
- `0x001ab388` `std_bad_alloc_destructor_D0`

Evidence:
`analysis/matching/cpp-eh-runtime-small-listing-report.md` and
`analysis/matching/libsupcxx-rtti-small-listing-report.md`.

Candidate: `matching/candidates/cpp_runtime_small.S`.

The candidate is an explicitly labelled exact assembly reconstruction used for
isolated matching. Existing project symbol names remain canonical where they
differ from matcher aliases. Historical lineage is GCC 3.2.2-era
libgcc/libsupc++, with the surviving SNESticle EE3.2.2-b1 release as
independent PS2 runtime-family evidence.

### Progress 54 historical/library recovered-source batch — 98/98 strict

- Broad recovered-source/compiler screen: **82/82**.
- Historical PS2DEV per-object source-shape recovery: **16/16**.

Functions:
- `0x001057fc` `snes_p16_001057fc`
- `0x0010a8bc` `snes_p11_0010a8bc`
- `0x00151330` `per_rom_cleanup`
- `0x0018339c` `snes_leaf_0018339c`
- `0x0018e2e0` `huft_free`
- `0x0018f240` `unzStringFileNameCompare`
- `0x0018f638` `unzGetGlobalInfo`
- `0x0018f654` `unzlocal_DosDateToTmuDate`
- `0x001907cc` `compress`
- `0x001908bc` `deflateInit_`
- `0x00193564` `gzopen`
- `0x00193580` `gzdopen`
- `0x001935d8` `gzsetparams`
- `0x0019371c` `check_header`
- `0x00193cd8` `gzgetc`
- `0x00193d0c` `gzgets`
- `0x00193dbc` `gzwrite`
- `0x00193f08` `gzputc`
- `0x001940ac` `gzflush`
- `0x00194378` `gztell`
- `0x00194398` `gzeof`
- `0x001943c0` `putLong`
- `0x00194498` `gzclose`
- `0x0019532c` `inflate_blocks_sync_point`
- `0x0019533c` `inflate_codes_new`
- `0x00196980` `tr_static_init`
- `0x00196988` `_tr_init`
- `0x00198aa4` `zcfree`
- `0x00198d38` `gsDriver_dtor_A`
- `0x00198d58` `gsDriver_dtor_B`
- `0x001990f8` `gsDriver_clearScreen`
- `0x00199178` `gsDriver_getFrameBufferBase`
- `0x00199198` `gsDriver_getTextureBufferBase`
- `0x001991a0` `gsDriver_getCurrentDisplayBuffer`
- `0x001991a8` `gsDriver_getCurrentDrawBuffer`
- `0x001991b0` `gsDriver_swapBuffers`
- `0x001991e8` `gsDriver_isDrawBufferAvailable`
- `0x001991f8` `gsDriver_isDisplayBufferAvailable`
- `0x00199208` `gsDriver_setNextDrawBuffer`
- `0x00199290` `gsDriver_DisplayNextFrame`
- `0x00199700` `gsPipe_copy_ctor_A`
- `0x00199720` `gsPipe_copy_ctor_B`
- `0x00199830` `gsPipe_getPipeSize`
- `0x00199848` `gsPipe_ReInit`
- `0x00199898` `gsPipe_getBytesLeft`
- `0x001998b8` `gsPipe_FlushCheck`
- `0x0019a580` `gsPipe_setFilterMethod`
- `0x0019bf00` `CDVD_DiskReady`
- `0x0019c0d0` `CDVD_Stop`
- `0x0019c128` `CDVD_TrayReq`
- `0x0019c190` `CDVD_getdir`
- `0x0019c2ac` `CDVD_FlushCache`
- `0x0019c304` `CDVD_GetSize`
- `0x0019c688` `SifBindRpc`
- `0x0019c7b0` `SifCallRpc`
- `0x0019c960` `rpc_packet_free`
- `0x0019c978` `_request_end`
- `0x0019ca0c` `search_svdata`
- `0x0019ca54` `_request_bind`
- `0x0019cb08` `_request_call`
- `0x0019cba8` `_request_rdata`
- `0x0019cd4c` `SifExitRpc`
- `0x0019cd70` `_rpc_get_packet`
- `0x0019ce2c` `_rpc_get_fpacket`
- `0x0019cfc0` `fioOpen`
- `0x0019d090` `fioClose`
- `0x0019d120` `fioRead`
- `0x0019d360` `fioLseek`
- `0x0019d534` `fioPutc`
- `0x0019d600` `SifLoadModule`
- `0x0019d620` `SifLoadModuleBuffer`
- `0x0019e274` `snprintf_room_check`
- `0x0019e860` `strcasecmp`
- `0x0019eaa4` `strrchr`
- `0x0019edac` `tolower`
- `0x0019eddc` `toupper`
- `0x0019ee0c` `isupper`
- `0x0019ee20` `islower`
- `0x0019ee34` `isalpha`
- `0x0019ee80` `isdigit`
- `0x0019ee94` `isalnum`
- `0x0019eee0` `iscntrl`
- `0x0019eefc` `isgraph`
- `0x0019ef3c` `isprint`
- `0x0019ef5c` `ispunct`
- `0x0019efac` `isspace`
- `0x0019f138` `_SifSendCmd`
- `0x0019f264` `SifSendCmd`
- `0x0019f2a0` `iSifSendCmd`
- `0x0019f2dc` `change_addr`
- `0x0019f2e8` `set_sreg`
- `0x0019f510` `SifExitCmd`
- `0x0019f544` `SifAddCmdHandler`
- `0x0019faa8` `vprintf`
- `0x0019fb00` `EnableDmac`
- `0x0019fbf0` `_SifCmdIntHandler`
- `0x001a10d0` `mcMkDir`
- `0x001a1af4` `SifCheckStatRpc`

Evidence: `analysis/matching/progress54-validated-98.tsv` and the two frozen probe TSVs.
All rows were revalidated with the repository comparator before promotion.
The original unpacked ELF remains the stronger formal full-target gate.

### Progress 55 broad historical/runtime sweep — 107/107 strict

- Baseline: **200/1041**.
- New unique strict functions: **107/107**.
- Checkpoint: **307/1041 (29.49%)**.
- Frozen union: `analysis/matching/progress55-validated-107.tsv`.
- Detailed provenance: `docs/PROGRESS55_307_MATCH.md`.

### Progress 56 CDVD exact reconstruction — 2/2 strict

- `0x0019be70` `CDVD_Init`
- `0x0019bf70` `CDVD_FindFile`

Evidence: `analysis/matching/cdvd-rpc-exact-listing-report.md`.

The readable historical source remains `matching/candidates/cdvd_rpc.c`.
`matching/candidates/cdvd_rpc_exact.S` is a clearly labelled byte-exact
matching reconstruction with separate `*_candidate` symbols. Both rows have
zero relocations and therefore match raw instruction bytes, bringing the
checkpoint to **309/1041 (29.68%)** and the eight-function CDVD corridor to
**8/8 function-level matching**.

### Progress 58 GSLIB/SIF RPC strict batch — 2/2

- `0x00199aa8` `gsPipe_setZTestEnable`
  - provenance: historical PGEN GSLIB 0.51 prebuilt `libgs.a`
  - profile: `prebuilt-archive`
  - boundary: `object-layout-gap:0x4`
- `0x0019cc0c` `SifInitRpc`
  - provenance: recovered-source deep compiler fingerprint
  - profile: `deep-o2-noalignall`
  - boundary: `exact-next-boundary`

Both rows passed relocation-normalized strict comparison with no unknown
relocation types. Evidence:
`analysis/matching/progress58-validated-2.tsv`.

Progress 58 raises the authoritative checkpoint from **309/1041 (29.68%)** to
**311/1041 (29.88%)**.

### Progress 60 source-lineage exact batch — 10/10

All ten rows use EE GCC 3.2.2 with the `p60-os` (`-Os`) profile and pass the
relocation-normalized strict comparator with `exact-next-boundary` and no
unknown relocation types.

- `0x0018e440` `FillBitBuffer`
- `0x0019d410` `fioMkdir`
- `0x0019e8e4` `strncasecmp`
- `0x001a17a4` `mcRename`
- `0x001a8420` `padGetDmaStr`
- `0x001a8690` `padPortOpen`
- `0x001a87b0` `padPortClose`
- `0x001a8864` `padRead`
- `0x001a88a8` `padGetState`
- `0x001a9080` `padGetConnection`

Exact candidates: `matching/candidates/progress60/`.

Evidence: `analysis/matching/progress60-validated-10.tsv`.

Progress 60 raises the checkpoint from **311/1041 (29.88%)** to
**321/1041 (30.84%)**.

### Progress 61 padInfoMode source-lineage match — 1/1

- `0x001a8b24` `padInfoMode`
- candidate: `libpad-newpadman-8x2-loop-lenint-infomode-reversed`
- compiler profile: `p61-os` (`-Os`)
- boundary: `exact-next-boundary`
- relocation-normalized equality: true
- unknown relocation types: none

Evidence: `analysis/matching/progress61-validated-1.tsv`.

Progress 61 raises the checkpoint from **321/1041 (30.84%)** to
**352/1041 (33.81%)**.

## Important scope

These are **relocation-normalized committed-listing matches**. They are strong
function-level evidence, but they do not claim that the complete original ELF
has already been linked and reproduced byte-for-byte.

## Current WIP

`SifInitRpc @ 0x0019cc0c` is promoted by Progress 58 after the corrected
historical source shape passed the strict `deep-o2-noalignall` compiler gate.

`AddDmacHandler`, `RemoveDmacHandler`, and `EndOfHeap` are also intentionally
not promoted in Progress 50: their identities/syscall numbers are strong, but
their complete target bytes are not present in the committed focused listings
used by this gate.
