# SNESstation-Decomp state

Last methodology checkpoint: 2026-08-15.

## Target

SNES Station v0.23 WIP, January 24, 2004.

Packed ELF SHA-256:
`4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487`

Unpacked image SHA-256:
`739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`

## Closed local listing gates

- Newlib mathfp: 7/7.
- libgcc unwind compact gate: 7/7.
- GSLIB hardware: 7/7 strict.
- Legacy ZIP `get_tree`: 1/1 strict.
- Old EE libkernel syscall leaves: 19/19 strict.
- Old EE libkernel DIntr/EIntr: 2/2 strict.
- Old EE libkernel size-optimized strings: 4/4 strict.
- Old EE libc assembly strings/memory: 7/7 strict.

Total closed committed-listing matches: **200 functions**.

Progress 52 promotes those proven rows in the authoritative progress
manifests and regenerates the README/SVG. Matching is **200/1041 = 19.21%**;
structural/source-model coverage remains 100% and is not conflated with matching.

The syscall leaves preserve the historical `kernel.S` source form. DIntr/EIntr
retain readable historical-source behavior while using a clearly labelled exact
assembly reconstruction for the strict target gate.

These are function/listing claims, not a claim that the complete ELF links
byte-identically yet.

Progress 53 closed the GCC/libsupc++ runtime batch at 102 matches. Progress 54 closes an additional 98-function historical/library recovered-source batch, bringing the authoritative checkpoint to **200/1041 (19.21%)**.

## Current WIP: EE libcdvd RPC

Historical source family recovered for eight functions:

- CDVD_Init
- CDVD_DiskReady
- CDVD_FindFile
- CDVD_Stop
- CDVD_TrayReq
- CDVD_getdir
- CDVD_FlushCache
- CDVD_GetSize

The historical source/layout is structurally strong, but the corridor is not
yet an 8/8 byte match. Do not mark it MATCHING.

Important references:
- SNESticle commit `9590ebf3bf768424ebd6cb018f322e724a7aade3`:
  old libcdvd source plus EE3.2.2-b1 listing/object and neighboring libkernel
  archive-member evidence.
- PGEN commit `f722681391fb6a1cc64a1260027a33862685e585`:
  same historical libcdvd family with CDVD_GetSize / command 0x08.

## Compiler state

Already reproducible from repository source:
- binutils 2.14
- GCC 3.2.2 PS2/R5900 BETA 3 patch snapshot dated 2004-02-14
- stage-one C compiler

Archived fingerprint candidates:
- `ee-gcc3.2-030210-beta2.tar.gz`
- `ee-gcc3.2-030926.tar.gz`

The 030926 compiler remains the strongest pre-target fingerprint candidate for
remaining C codegen differences such as CDVD register allocation/scheduling.
Bytes decide.

## Still not globally pinned

- exact old PS2LIB snapshot used by SNES Station
- exact EE libcdvd source revision
- exact binutils patch level used by the target
- full historical libgcc/libsupc++ archive set and archive ordering
- exact application linker script/archive order
- SJCRUNCH2 packer revision

Do not silently substitute modern PS2SDK for those unknowns.

## Preserved matching checkpoint

Current authoritative matching status:
`docs/MATCHED_CHECKPOINT.md` — **200 closed committed-listing matches**.
