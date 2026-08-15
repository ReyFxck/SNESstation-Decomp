# Matched source checkpoint

Checkpoint date: 2026-08-15.

## Closed committed-listing gates: 102 functions

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

## Important scope

These are **relocation-normalized committed-listing matches**. They are strong
function-level evidence, but they do not claim that the complete original ELF
has already been linked and reproduced byte-for-byte.

## Current WIP

EE CDVD RPC corridor: eight historical functions recovered structurally and
source-wise, but **not yet 8/8 byte matching**. Keep CDVD experiments separate
from this matched list.

`AddDmacHandler`, `RemoveDmacHandler`, and `EndOfHeap` are also intentionally
not promoted in Progress 50: their identities/syscall numbers are strong, but
their complete target bytes are not present in the committed focused listings
used by this gate.
