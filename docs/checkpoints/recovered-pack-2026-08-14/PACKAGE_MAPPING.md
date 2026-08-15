# Recovered pack → canonical repository mapping

The master checkpoint script embeds the complete
`SNESstation-Decomp-Recovered-Pack-2026-08-14.zip`.

Every regular file from that ZIP is mapped to a canonical repository path.
No package file is silently dropped.

- MATCHED source goes to normal `matching/`, `src/`, `analysis/` and `tools/`.
- CDVD remains WIP under its canonical candidate/header/analysis paths.
- Progress 30-46 are archived under `tools/history/progress/`.
- The old `apply-matched.sh` helper is archived under `tools/history/checkpoints/`.
- Package README/status/hash records are kept under `docs/checkpoints/`.

| Package path | Canonical repo path |
|---|---|
| `MATCHED/gslib_hw/PROGRESS42.md` | `docs/PROGRESS42.md` |
| `MATCHED/gslib_hw/STATUS.md` | `docs/checkpoints/recovered-pack-2026-08-14/gslib_hw_STATUS.md` |
| `MATCHED/gslib_hw/analysis/matching/gslib_hw_listing.csv` | `analysis/matching/gslib_hw_listing.csv` |
| `MATCHED/gslib_hw/src/ps2/gslib_hw_recovered.c` | `src/ps2/gslib_hw_recovered.c` |
| `MATCHED/gslib_hw/tools/run-gslib-frontier.sh` | `tools/run-gslib-frontier.sh` |
| `MATCHED/libgcc_unwind/STATUS.md` | `docs/checkpoints/recovered-pack-2026-08-14/libgcc_unwind_STATUS.md` |
| `MATCHED/libgcc_unwind/matching/candidates/libgcc_unwind_leaves.c` | `matching/candidates/libgcc_unwind_leaves.c` |
| `MATCHED/mathfp/STATUS.md` | `docs/checkpoints/recovered-pack-2026-08-14/mathfp_STATUS.md` |
| `MATCHED/mathfp/matching/candidates/mathfp.c` | `matching/candidates/mathfp.c` |
| `MATCHED/mathfp/matching/candidates/mathfp_numtest.S` | `matching/candidates/mathfp_numtest.S` |
| `MATCHED/mathfp/matching/candidates/mathfp_numtest.c` | `matching/candidates/mathfp_numtest.c` |
| `README.md` | `docs/checkpoints/RECOVERED_PACK_2026-08-14.md` |
| `SHA256SUMS.txt` | `docs/checkpoints/recovered-pack-2026-08-14/SHA256SUMS.txt` |
| `WIP/cdvd_rpc/PROGRESS45.md` | `docs/PROGRESS45.md` |
| `WIP/cdvd_rpc/STATUS.md` | `docs/CDVD_RPC_STATUS.md` |
| `WIP/cdvd_rpc/analysis/functions/cdvd_rpc_0019be70.asm` | `analysis/functions/cdvd_rpc_0019be70.asm` |
| `WIP/cdvd_rpc/analysis/matching/cdvd_rpc_listing.csv` | `analysis/matching/cdvd_rpc_listing.csv` |
| `WIP/cdvd_rpc/matching/candidates/cdvd_rpc.c` | `matching/candidates/cdvd_rpc.c` |
| `WIP/cdvd_rpc/matching/ee_abi_compat/cdvd_legacy_compat.h` | `matching/ee_abi_compat/cdvd_legacy_compat.h` |
| `WIP/cdvd_rpc/reference/cdvd_rpc_target.bin` | `analysis/matching/cdvd_rpc_target.bin` |
| `scripts/apply-matched.sh` | `tools/history/checkpoints/apply-matched-2026-08-14.sh` |
| `scripts/progress30-apply.sh` | `tools/history/progress/progress30-apply.sh` |
| `scripts/progress31-apply.sh` | `tools/history/progress/progress31-apply.sh` |
| `scripts/progress32-apply.sh` | `tools/history/progress/progress32-apply.sh` |
| `scripts/progress33-apply.sh` | `tools/history/progress/progress33-apply.sh` |
| `scripts/progress34-apply.sh` | `tools/history/progress/progress34-apply.sh` |
| `scripts/progress35-apply.sh` | `tools/history/progress/progress35-apply.sh` |
| `scripts/progress36-apply.sh` | `tools/history/progress/progress36-apply.sh` |
| `scripts/progress37-apply.sh` | `tools/history/progress/progress37-apply.sh` |
| `scripts/progress38-apply.sh` | `tools/history/progress/progress38-apply.sh` |
| `scripts/progress39-apply.sh` | `tools/history/progress/progress39-apply.sh` |
| `scripts/progress40-apply.sh` | `tools/history/progress/progress40-apply.sh` |
| `scripts/progress41-apply.sh` | `tools/history/progress/progress41-apply.sh` |
| `scripts/progress42-apply.sh` | `tools/history/progress/progress42-apply.sh` |
| `scripts/progress43-apply.sh` | `tools/history/progress/progress43-apply.sh` |
| `scripts/progress44-apply.sh` | `tools/history/progress/progress44-apply.sh` |
| `scripts/progress45-apply.sh` | `tools/history/progress/progress45-apply.sh` |
| `scripts/progress46-apply.sh` | `tools/history/progress/progress46-apply.sh` |
