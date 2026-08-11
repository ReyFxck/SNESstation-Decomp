# Progress 10 — 60% reconstruction checkpoint

Progress 10 deliberately targets small, auditable gaps rather than promoting large
unresolved functions simply to increase the percentage. Target machine code remains
authoritative; address-based names are retained when an original symbol is not proven.

## Added behavioral source

- `src/ps2/audio_rpc_recovered.c` — SjPCM and AmigaMod EE-side RPC leaves.
- `src/ps2/libmtap_recovered.c` — NEW/XPADMAN multitap RPC leaves.
- `src/ps2/libc_misc_recovered.c` — fatal sink, qsort-compatible target behavior and target RNG leaf.
- `src/ps2/gcc_fde_runtime_recovered.c` — compact `unwind-dw2-fde.o` registration/encoding helpers.
- `src/ps2/address_leaf_recovered.c` — address-labelled no-op, state, arithmetic and conversion leaves proven directly from assembly.
- `src/ps2/small_dispatch_recovered.c` — short dispatch/subentry behavioral models whose historical source names are not yet proven.
- `src/snes9x/memmap_metadata_recovered.c` — four compact CMemory ROM metadata/map helpers.

The soft-double public wrappers `dpadd`, `dpsub`, `dpmul`, `dpdiv` and `dpcmp`
are promoted only to behavioral reconstruction, not matching. Six public FDE list
registration/deregistration entries are likewise promoted after their target list
semantics were represented in source.

## Accounting

On the conservative 1,137-target JAL proxy:

- **685 reconstructed — 60.25%**
- **739 mapped — 65.00%**
- **0 matching — 0.00%**

Matching stays at zero until an actual historical EE rebuild is compared byte-for-byte
after relocation normalization.

## Validation

All 72 recovered C translation units pass host syntax validation with
`-std=c11 -Wall -Wextra -Werror` using the repository include paths. Map addresses
remain unique, and all 123 newly address-labelled source entries correspond to actual
JAL targets in `analysis/jal_candidates.csv`.
