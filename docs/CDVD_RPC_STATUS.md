# CDVD RPC — recovered C, 8/8 byte-exact

The complete EE libcdvd RPC corridor at `0x0019be70..0x0019c363` is now
recovered as readable C in `src/ps2/cdvd_rpc.c`.

Eight historical functions are present:

- `CDVD_Init`
- `CDVD_DiskReady`
- `CDVD_FindFile`
- `CDVD_Stop`
- `CDVD_TrayReq`
- `CDVD_getdir`
- `CDVD_FlushCache`
- `CDVD_GetSize`

The source family is independently supported by the preserved SNESticle
revision `9590ebf3bf768424ebd6cb018f322e724a7aade3` and the closely related
PGEN revision `f722681391fb6a1cc64a1260027a33862685e585`. The PGEN copy also
preserves `CDVD_GetSize` and command `CDVD_GETSIZE = 0x08`.

## Exact C gate

The promoted source is compiled with the repository's pinned EE GCC 3.2.2
stage-one compiler using the recovered `-Os` R5900 profile. The focused gate
then performs both function-level comparison and a second raw linked-text
comparison after applying the target addresses for the CDVD globals and called
runtime functions.

Result:

| Function | Target / object bytes | Result |
|---|---:|---|
| `CDVD_Init` | 144 / 144 | **MATCHING** |
| `CDVD_DiskReady` | 112 / 112 | **MATCHING** |
| `CDVD_FindFile` | 352 / 352 | **MATCHING** |
| `CDVD_Stop` | 88 / 88 | **MATCHING** |
| `CDVD_TrayReq` | 104 / 104 | **MATCHING** |
| `CDVD_getdir` | 284 / 284 | **MATCHING** |
| `CDVD_FlushCache` | 88 / 88 | **MATCHING** |
| `CDVD_GetSize` | 96 / 96 | **MATCHING** |

Function result: **8/8**, same-size **8/8**, total differing bytes **0**.

The raw linked `.text` gate is also exact:

- target bytes: **1,268**
- linked C bytes: **1,268**
- target SHA-256: `fc794d4ca0b492dfc0ce6c575ae9ab58c3d3caa5bf0dfc5b49c84ecfd58315d8`
- linked SHA-256: `fc794d4ca0b492dfc0ce6c575ae9ab58c3d3caa5bf0dfc5b49c84ecfd58315d8`

So this checkpoint is stronger than relocation-normalized matching alone: after
link-time relocation, the entire 1,268-byte target corridor is byte-for-byte
identical to code produced from the promoted C.

Run the focused proof with:

```bash
bash tools/run-cdvd-source-recovery.sh
```

## Historical matcher note

Progress 56 previously used `matching/candidates/cdvd_rpc_exact.S` to close
`CDVD_Init` and `CDVD_FindFile` when the correct C compiler profile had not
yet been established. That assembly remains useful historical evidence, but it
is no longer required to prove this corridor: the current canonical C source
reproduces all eight functions exactly.
