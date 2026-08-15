# Progress 43 — Historical libcdvd EE RPC corridor

This progress opens the corridor immediately following the matched Hiryu GSLIB
hardware helpers: `0x0019be70..0x0019c363`.

Historical source evidence identifies all eight functions:

1. `CDVD_Init`
2. `CDVD_DiskReady`
3. `CDVD_FindFile`
4. `CDVD_Stop`
5. `CDVD_TrayReq`
6. `CDVD_getdir`
7. `CDVD_FlushCache`
8. `CDVD_GetSize`

The first seven survive in the SNESticle libcdvd source at commit
`9590ebf3bf768424ebd6cb018f322e724a7aade3`.  `CDVD_GetSize` and command
`CDVD_GETSIZE = 0x08` survive in the closely-related PGEN copy at commit
`f722681391fb6a1cc64a1260027a33862685e585`.

The target itself confirms the old client ABI: `CDVD_Init` reads the RPC
client's `server` pointer at offset `+0x24`.

`CDVD_FindFile` in the SNES Station target contains an inline fixed-size
`memcpy` of the 0x90-byte `TocEntry`, unlike the surviving SNESticle listing
which calls `memcpy`.  The corridor runner therefore uses a local historical
compiler profile without `-ffreestanding`/`-fno-builtin`, allowing GCC 3.2.2
to expand that fixed-size copy.  This flag difference remains local to the
CDVD experiment and does not alter the repository-wide EE contract.

Run:

```bash
./tools/run-cdvd-frontier.sh
```

No byte-match status is claimed until the comparator reports it.
