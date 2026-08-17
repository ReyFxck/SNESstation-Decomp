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

Total closed committed-listing matches: **352 functions**.

After Progress 60, the authoritative manifests contain
**352/1041 = 33.81%** matching rows;
structural/source-model coverage remains 100% and is not conflated with matching.

The syscall leaves preserve the historical `kernel.S` source form. DIntr/EIntr
retain readable historical-source behavior while using a clearly labelled exact
assembly reconstruction for the strict target gate.

These are function/listing claims, not a claim that the complete ELF links
byte-identically yet.

Progress 55 adds 107 unique strict historical/runtime matches to the Progress 54 baseline, bringing the authoritative function-level checkpoint to **307/1041 (29.49%)**.

Progress 53 closed the GCC/libsupc++ runtime batch at 102 matches. Progress 54 closes an additional 98-function historical/library recovered-source batch, bringing the authoritative checkpoint to **200/1041 (19.21%)**.

Progress 56 closes the two remaining EE libcdvd RPC rows (`CDVD_Init` and
`CDVD_FindFile`) with explicitly labelled exact assembly reconstructions after
the readable historical C failed to reproduce the target register allocation
and inline-copy codegen. The function-level checkpoint is now
**309/1041 (29.68%)**.

## Closed function-level gate: EE libcdvd RPC

Historical source family recovered for eight functions:

- CDVD_Init
- CDVD_DiskReady
- CDVD_FindFile
- CDVD_Stop
- CDVD_TrayReq
- CDVD_getdir
- CDVD_FlushCache
- CDVD_GetSize

All eight now have strict function-level matching evidence. Six were closed by
Progress 54 from historical/recovered source compiler gates; Progress 56 closes
`CDVD_Init` and `CDVD_FindFile` with the separate
`matching/candidates/cdvd_rpc_exact.S` reconstruction. The exact `.S` is matcher
evidence, not a claim to be Hiryu's original source.

Important references:
- SNESticle commit `9590ebf3bf768424ebd6cb018f322e724a7aade3`:
  old libcdvd source plus EE3.2.2-b1 listing/object and neighboring libkernel
  archive-member evidence.
- PGEN commit `f722681391fb6a1cc64a1260027a33862685e585`:
  same historical libcdvd family with CDVD_GetSize / command 0x08.

## Progress 58: GSLIB + SIF RPC strict matches

Progress 58 promotes two independently screened rows:

- `0x00199aa8` `gsPipe_setZTestEnable` from the historical PGEN GSLIB 0.51
  prebuilt `libgs.a`; relocation-normalized bytes match with an object-layout
  boundary proof.
- `0x0019cc0c` `SifInitRpc` from the recovered source using the
  `deep-o2-noalignall` EE GCC 3.2.2 fingerprint; the exact next function
  boundary is proven and relocation-normalized bytes match.

Evidence is frozen in `analysis/matching/progress58-validated-2.tsv`.
The checkpoint is **311/1041 (29.88%)**.

## Progress 60: source-lineage exact batch

Progress 60 promotes **10 strict function-level matches** discovered by source-lineage correction rather than flag farming. The exact generated candidates are frozen under `matching/candidates/progress60/` and the machine-readable gate is `analysis/matching/progress60-validated-10.tsv`.

Checkpoint: **321/1041 = 30.84%**.

## Progress 61: padInfoMode historical comparison

Progress 61 promotes `padInfoMode @ 0x001a8b24` as a strict source-lineage
match. The target preserves the historical reversed signed comparison
`pdata->nrOfModes < index`; the exact PS2DEV-derived candidate compiled with
the `p61-os` profile and passed the relocation-normalized comparator with an
exact next-function boundary and no unknown relocation types.

Checkpoint: **352/1041 = 33.81%**.

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
`docs/MATCHED_CHECKPOINT.md` — **352 closed committed-listing matches**.
