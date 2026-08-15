# SNESstation-Decomp recovered source checkpoint

This is the consolidated source package that should have been delivered earlier.

## MATCHED — 21 functions

- **MathFP: 7/7** — cosf, sinf, tanf, atanf, sqrtf, fabsf, numtestf.
- **libgcc unwind: 7/7** — seven compact unwind helpers.
- **GSLIB hardware: 7/7 strict** — VRstart_handler, WaitForNextVRstart, TestVRstart, ClearVRcount, DmaReset, SendDma02, Dma02Wait.

These are local committed-listing / relocation-normalized matches. This is not yet a claim that the complete original ELF links byte-identically.

## WIP — CDVD RPC

Eight historical CDVD functions are recovered under `WIP/cdvd_rpc/`, with ABI header, manifest and target bytes/listing. They are deliberately not labeled MATCHED because the compiler fingerprint is still open.

Strongest compiler candidate located: `ee-gcc3.2-030926.tar.gz`, GitHub release asset `227976365`, size 11,747,681 bytes.

## Scripts

`scripts/apply-matched.sh` applies only the proven GSLIB checkpoint and never promotes CDVD WIP. Older Progress30–46 scripts are included for reproducibility.
