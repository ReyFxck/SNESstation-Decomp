# Progress 57 candidate: historical SifInitRpc source-shape

Status: **historical-source candidate only; NOT MATCHING yet**.

Target function:

- `SifInitRpc` at `0x0019cc0c..0x0019cd4c` (320 bytes).

Historical evidence:

- The PS2SDK history imported the older PS2LIB kernel implementation in commit
  `a80df908256955382f102278400b5d713552dbce` on 2004-04-18.
- That `SifInitRpc` source has the same major control-flow/source shape as the
  SNES Station target and predates the later IOP reboot-count logic.
- Crucially, its handshake packet uses `cmdp = (u32 *)&pkt_table[64]` after the
  RPC data table pointers have been converted to KSEG1.

Current reconstruction difference:

- `src/ps2/sifrpc_recovered.c` currently derives `cmdp` from
  `sif_rpc_data_recovered.pkt_table + 64`, i.e. from the already KSEG1-converted
  pointer.
- The target materializes the backing packet-table address for this operation,
  which is consistent with the historical `&pkt_table[64]` source shape.

Candidate correction:

```c
cmdp = (uint32_t *)(void *)(pkt_table_recovered + 64);
```

Promotion gate:

1. Compile with the historical EE GCC candidate/profile used for the neighboring
   SIFRPC functions (begin with the project-pinned GCC 3.2.x/3.2.2 profile and
   the `sdk-os` flag family).
2. Compare `0x0019cc0c..0x0019cd4c` with `tools/compare_elf_functions.py`.
3. Use relocation normalization only for actual relocation-covered bytes.
4. Do **not** mark `SifInitRpc` MATCH unless the strict comparator passes.

The one-line source correction is therefore safe to research and historically
better grounded, but this note deliberately does not increase the matched count.
