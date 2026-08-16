# CDVD RPC — 8/8 strict function-level matching

Eight functions have historical source-family identification:
- CDVD_Init
- CDVD_DiskReady
- CDVD_FindFile
- CDVD_Stop
- CDVD_TrayReq
- CDVD_getdir
- CDVD_FlushCache
- CDVD_GetSize

Corrected boundaries give the expected body sizes after alignment padding is excluded.

Progress 54 closed six functions through historical/recovered-source compiler
gates. Progress 56 closes the two remaining rows:

- `CDVD_Init` — 144/144 raw bytes, 0 relocations
- `CDVD_FindFile` — 352/352 raw bytes, 0 relocations

The Progress 56 matcher uses `matching/candidates/cdvd_rpc_exact.S`, an
explicitly labelled exact assembly reconstruction with non-colliding candidate
symbols. The readable historical source remains
`matching/candidates/cdvd_rpc.c`; the exact assembly is **not** claimed to be
Hiryu's original source.

Function-level CDVD result: **8/8 MATCHING**.
This does not claim that the complete ELF links byte-identically yet.

Strongest compiler fingerprint found so far:
- `ee-gcc3.2-030926.tar.gz`
- GitHub release asset id `227976365`
- size `11,747,681` bytes

That compiler remains useful for historical-codegen research, but it no longer
blocks the closed function-level CDVD checkpoint.
