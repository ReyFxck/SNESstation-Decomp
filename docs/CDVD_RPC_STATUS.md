# CDVD RPC — WIP / historical source recovered

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
This corridor is **NOT byte-matched yet** and is deliberately kept outside MATCHED.

Strongest compiler fingerprint found so far:
- `ee-gcc3.2-030926.tar.gz`
- GitHub release asset id `227976365`
- size `11,747,681` bytes
