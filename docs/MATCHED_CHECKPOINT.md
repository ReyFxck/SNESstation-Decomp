# Matched source checkpoint

Checkpoint date: 2026-08-14.

## Closed committed-listing gates: 21 functions

### Newlib mathfp — 7/7

- `0x0019fddc` `cosf`
- `0x001a0024` `sinf`
- `0x001a0254` `tanf`
- `0x001a045c` `atanf`
- `0x001a06a0` `sqrtf`
- `0x001a06b0` `fabsf`
- `0x001a06c0` `numtestf`

Evidence:
`analysis/matching/mathfp-listing-report.md`

### libgcc unwind — 7/7

- `0x001a3dc0` `size_of_encoded_value`
- `0x001a3e30` `base_of_encoded_value`
- `0x001a3ee8` `read_uleb128`
- `0x001a3f28` `read_sleb128`
- `0x001a40e8` `_Unwind_GetLanguageSpecificData`
- `0x001a40f0` `_Unwind_GetRegionStart`
- `0x001a40f8` `_Unwind_GetDataRelBase`

Evidence:
`analysis/matching/libgcc-unwind-leaves-listing-report.md`

### GSLIB hardware — 7/7 strict

- `0x0019bd38` `VRstart_handler`
- `0x0019bd50` `WaitForNextVRstart`
- `0x0019bd78` `TestVRstart`
- `0x0019bd88` `ClearVRcount`
- `0x0019bd98` `DmaReset`
- `0x0019be20` `SendDma02`
- `0x0019be40` `Dma02Wait`

Evidence:
`analysis/matching/gslib-hw-listing-report.md`

## Important scope

These are **relocation-normalized committed-listing matches**. They are strong
function-level evidence, but they do not claim that the complete original ELF
has already been linked and reproduced byte-for-byte.

## Current WIP

EE CDVD RPC corridor:
eight historical functions recovered structurally/source-wise, but **not yet
8/8 byte matching**. Keep CDVD experiments separate from this matched list.
