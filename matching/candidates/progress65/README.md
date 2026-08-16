# Progress 65 ps2lib-era libmc source

This directory preserves the exact historical `libmc.c` source used by the
Progress 65 strict matching gate.

Provenance:

- repository: `https://github.com/ps2dev/ps2sdk.git`
- commit: `01d625018c3fde3044292446c910b8ea508adfdc`
- path: `ee/rpc/memorycard/src/libmc.c`
- source SHA-256:
  `b915d37eb9624421d41236331beea6935afa3530c05922ee8c129d22b55dff6a`

The `.c.txt` suffix is intentional. The root Makefile host-syntax target
compiles every `*.c` under `matching/candidates/`; this historical PS2SDK
snapshot is evidence for the pinned EE GCC gate, not a host C translation unit.

Progress 65 produced six relocation-normalized strict function-level matches
with profile `p65-os`, all with exact-next-function boundary proof and no
unknown relocation types.
