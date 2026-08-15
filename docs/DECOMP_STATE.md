# SNESstation-Decomp state

Last methodology checkpoint: 2026-08-14.

## Target

SNES Station v0.23 WIP, January 24, 2004.

Packed ELF SHA-256:
`4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487`

Unpacked image SHA-256:
`739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b`

## Closed local listing gates

- Newlib mathfp corridor: 7/7 relocation-normalized matches.
- libgcc unwind compact gate: 7/7 relocation-normalized matches.
- GSLIB hardware corridor: 7/7 relocation-normalized matches; strict gate OK.

These are function/listing claims, not a claim that the complete ELF links
byte-identically yet.

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
- SNESticle commit 9590ebf3bf768424ebd6cb018f322e724a7aade3:
  old libcdvd source plus EE3.2.2-b1 listing/object.
- PGEN commit f722681391fb6a1cc64a1260027a33862685e585:
  same historical libcdvd family with CDVD_GetSize / command 0x08.

## Compiler state

Already reproducible from repository source:
- binutils 2.14
- GCC 3.2.2 PS2/R5900 BETA 3 patch snapshot dated 2004-02-14
- stage-one C compiler

Archived fingerprint candidates:
- ee-gcc3.2-030210-beta2.tar.gz
- ee-gcc3.2-030926.tar.gz

The 030926 compiler is the strongest pre-target fingerprint candidate for the
remaining CDVD register-allocation/scheduling differences, but it must be
validated by byte comparisons.

## C library

Pinned:
- Newlib 1.10.0 official source archive
- PS2DEV newlib-1.10.0 patch from immutable ps2toolchain commit
  16a47184b3a5fdf4aea45fcc8fee082d3c4d4183e

## Still not globally pinned

- exact old PS2LIB snapshot used by SNES Station
- exact EE libcdvd source revision
- exact binutils patch level used by the target
- full historical libgcc/libsupc++ archive set and archive ordering
- exact application linker script/archive order
- SJCRUNCH2 packer revision

Do not silently substitute modern PS2SDK for those unknowns.

## Preserved matching checkpoint and scripts

The repository also stores:
- `docs/MATCHED_CHECKPOINT.md` — the 21 closed committed-listing matches.
- `tools/history/progress/` — archived Progress 42-46 scripts.

Script status is explicit in `tools/history/progress/README.md`:
Progress 42 is the closed GSLIB gate; Progress 43-46 remain CDVD WIP/fingerprint
experiments and must never be counted as matching progress.
