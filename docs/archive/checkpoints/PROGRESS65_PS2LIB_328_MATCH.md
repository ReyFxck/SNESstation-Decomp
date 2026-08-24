# Progress 65 — ps2lib-era libmc: 328 strict matches

Progress 65 identifies the old ps2lib lineage of the SNES Station memory-card
RPC client and promotes six strict function-level matches:

- `0x001a147c` `mcDelete`
- `0x001a1554` `mcFormat`
- `0x001a1610` `mcUnformat`
- `0x001a16cc` `mcGetEntSpace`
- `0x001a18c0` `mcChangeThreadPriority`
- `0x001a1a54` `strcpy_sjis`

Historical source:

- repository: `ps2dev/ps2sdk`
- commit: `01d625018c3fde3044292446c910b8ea508adfdc`
- path: `ee/rpc/memorycard/src/libmc.c`
- SHA-256:
  `b915d37eb9624421d41236331beea6935afa3530c05922ee8c129d22b55dff6a`

All six use the pinned EE GCC 3.2.2 stage-one compiler with profile `p65-os`
and passed the strict relocation-aware comparator with:

- `result=MATCH`
- `differing_bytes=0`
- `normalized_equal=True`
- `boundary=exact-next-boundary`
- no unknown relocation types

The historical source is preserved as:

`matching/candidates/progress65/libmc-ps2lib-migration-20040415.c.txt`

Machine-readable proof:

`analysis/matching/progress65-validated-6.tsv`

Checkpoint: **328/1041 = 31.51%** strict function-level matching.

This remains a function-level matching checkpoint; it is not a claim of a
complete byte-identical replacement ELF.
