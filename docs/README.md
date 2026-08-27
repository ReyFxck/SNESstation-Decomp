# Documentation index

The maintained documentation is organized by purpose. Numbered Progress and
HUNT reports are historical checkpoints and live under [`archive/`](archive/).
They are evidence of how conclusions were reached, not the current scoreboard.

## Current status

- [`status/PROJECT_STATUS.generated.md`](status/PROJECT_STATUS.generated.md) — formal, recovered-evidence and working checkpoints.
- [`PROGRESS.generated.md`](PROGRESS.generated.md) — structural and matching metrics.
- [`SOURCE_COMPLETENESS.generated.md`](SOURCE_COMPLETENESS.generated.md) — source-model audit and build-readiness invariants.
- [`status/FUNCTION_FRONTIER_1041_CHECKPOINT.md`](status/FUNCTION_FRONTIER_1041_CHECKPOINT.md) — immutable tag, hashes and clean-checkout gates for the closed 1,041-function frontier.
- [`status/BUILD_READY_SOURCE_TREE.md`](status/BUILD_READY_SOURCE_TREE.md) — frozen EE ABI, 97-unit manifest, symbol ownership and canonical partial-link gate.
- [`status/V91_STAGE3D_LIBGCC_CLOSED.md`](status/V91_STAGE3D_LIBGCC_CLOSED.md) — closed 7/7 libgcc subtranche: four exact GCC 3.2.2 archive members and three completed source refactors.
- [`status/V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md`](status/V90_STAGE3E_NAMED_CONTRACTS_CLOSED.md) — closed 212/212 Stage-3E ledger, seven exact zlib peers and zero remaining compatibility storage.
- [`status/V89_STAGE3C_CLOSED.md`](status/V89_STAGE3C_CLOSED.md) — closed 54/54 Stage-3C ledger: 50 exact target objects and four completed source refactors.
- [`status/V88_STAGE3C_NAMED_DATA.md`](status/V88_STAGE3C_NAMED_DATA.md) — preceding open Stage-3C audit, retained as historical evidence.
- [`status/V87_PROVIDER_FRONTIER_CLOSED.md`](status/V87_PROVIDER_FRONTIER_CLOSED.md) — pre-refactor 251-name source-link namespace checkpoint.
- [`status/V86_PRIVATE_ASSET_PROVIDERS.md`](status/V86_PRIVATE_ASSET_PROVIDERS.md) — pre-refactor checkpoint for the same five privately verified bundles and ten asset contracts.
- [`status/V85_ZERO_BYTE_LINK_FRONTIER.md`](status/V85_ZERO_BYTE_LINK_FRONTIER.md) — pre-refactor zero-byte link-contract checkpoint.
- [`status/V84_REVIEWED_SOURCE_ALIASES.md`](status/V84_REVIEWED_SOURCE_ALIASES.md) — cumulative 323/337 zero-byte aliases, reviewed semantic identities and the explicit 14-row remainder.
- [`status/V83_SOURCE_ADDRESS_ALIASES.md`](status/V83_SOURCE_ADDRESS_ALIASES.md) — 257/337 mechanically proved zero-byte aliases and the explicit 80-row remainder.
- [`status/V82_UNPACKED_LAYOUT_ORACLE.md`](status/V82_UNPACKED_LAYOUT_ORACLE.md) — byte-free SJCRUNCH2/layout hashes and exact first-difference gate for Stage 3.
- [`status/V72_V53_PROMOTED.md`](status/V72_V53_PROMOTED.md) — regenerated proof and formal promotion of the six V53 recoveries.
- [`status/V81_FUNCTION_FRONTIER_CLOSED.md`](status/V81_FUNCTION_FRONTIER_CLOSED.md) — final 20 raw-exact promotions and the zero-entry function frontier.
- [`status/V80_QUICK_WINS_23.md`](status/V80_QUICK_WINS_23.md) — frozen 23-function quick-win checkpoint and its superseded 20-entry frontier.
- [`status/V79_C4CONV_OAM.md`](status/V79_C4CONV_OAM.md) — frozen raw-exact 952-byte `C4ConvOAM` checkpoint and its superseded 43-entry frontier.
- [`status/V78_C4BIT_PLANE_WAVE.md`](status/V78_C4BIT_PLANE_WAVE.md) — frozen strict 584-byte `C4BitPlaneWave` checkpoint and its superseded 44-entry frontier.
- [`status/V77_C4DRAW_WIREFRAME.md`](status/V77_C4DRAW_WIREFRAME.md) — frozen strict 472-byte `C4DrawWireFrame` checkpoint and its superseded 45-entry frontier.
- [`status/V76_C4SPR_DISINTEGRATE.md`](status/V76_C4SPR_DISINTEGRATE.md) — frozen strict 580-byte `C4SprDisintegrate` checkpoint and its superseded 46-entry frontier.
- [`status/V75_C4_FLOAT_MATH_AND_FRONTIER.md`](status/V75_C4_FLOAT_MATH_AND_FRONTIER.md) — frozen prior checkpoint with five C4 promotions, one exact companion and its superseded 47-entry frontier.
- [`status/V74_SPC7110_RTC_AND_FRONTIER_MAP.md`](status/V74_SPC7110_RTC_AND_FRONTIER_MAP.md) — frozen prior checkpoint; its 52-entry frontier is superseded by V75.
- [`status/V73_HISTORICAL_IO_AND_FRONTEND_MAP.md`](status/V73_HISTORICAL_IO_AND_FRONTEND_MAP.md) — frozen prior checkpoint; its provisional frontend packet labels are superseded by V74.
- [`BOTTLENECKS.md`](BOTTLENECKS.md) — current technical blockers.
- [`ROADMAP.md`](ROADMAP.md) — work remaining after structural closure.

The generated files above are refreshed with `make docs` and checked by
`make check`. `analysis/progress_targets.csv` remains the authoritative formal
matching manifest.

## Reproduction and matching

- [`REPRODUCTION.md`](REPRODUCTION.md) — proof ladder for the eventual byte-identical ELF.
- [`MATCHING_WORKFLOW.md`](MATCHING_WORKFLOW.md) — evidence requirements for promotion.
- [`MATCH_MINER.md`](MATCH_MINER.md) — cached address-anchored compiler search.
- [`DECOMP_PLAYBOOK.md`](DECOMP_PLAYBOOK.md) — disciplined reverse-engineering workflow.
- [`HISTORICAL_EE_TOOLCHAIN.md`](HISTORICAL_EE_TOOLCHAIN.md) — reproducible GCC/binutils candidate.
- [`TOOLCHAIN_FINGERPRINT.md`](TOOLCHAIN_FINGERPRINT.md) — evidence and remaining compiler unknowns.
- [`CONTRIBUTING.md`](../CONTRIBUTING.md) — contribution rules.

## Target and subsystem references

- [`DEPENDENCY_VERSIONS.md`](DEPENDENCY_VERSIONS.md) — exact, candidate and unknown dependency revisions.
- [`MAIN_FLOW.md`](MAIN_FLOW.md) — application boot and emulation flow.
- [`RENDERER_MAP.md`](RENDERER_MAP.md), [`PS2_GS_MAP.md`](PS2_GS_MAP.md) — renderer and GS mapping.
- [`ZLIB_MAP.md`](ZLIB_MAP.md), [`UNZIP_MAP.md`](UNZIP_MAP.md) — compressed-data corridors.
- [`CDVD_LIBKERNEL_MAP.md`](CDVD_LIBKERNEL_MAP.md), [`CDVD_RPC_STATUS.md`](CDVD_RPC_STATUS.md) — old PS2 runtime corridor.
- [`EMBEDDED_ASSETS.md`](EMBEDDED_ASSETS.md) — private asset ranges and extraction hashes.
- [`LEGAL.md`](LEGAL.md) — distribution and provenance policy.

## Archive policy

Historical reports, package-recovery notes and one-off root notes are retained
under [`archive/`](archive/) so evidence is not lost. New status claims must not
be added there; update the manifests and regenerate the maintained status
instead.
