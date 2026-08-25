# Documentation index

The maintained documentation is organized by purpose. Numbered Progress and
HUNT reports are historical checkpoints and live under [`archive/`](archive/).
They are evidence of how conclusions were reached, not the current scoreboard.

## Current status

- [`status/PROJECT_STATUS.generated.md`](status/PROJECT_STATUS.generated.md) — formal, recovered-evidence and working checkpoints.
- [`PROGRESS.generated.md`](PROGRESS.generated.md) — structural and matching metrics.
- [`SOURCE_COMPLETENESS.generated.md`](SOURCE_COMPLETENESS.generated.md) — source-model audit and build-readiness invariants.
- [`status/V72_V53_PROMOTED.md`](status/V72_V53_PROMOTED.md) — regenerated proof and formal promotion of the six V53 recoveries.
- [`status/V77_C4DRAW_WIREFRAME.md`](status/V77_C4DRAW_WIREFRAME.md) — strict 472-byte `C4DrawWireFrame` promotion and the current 45-entry frontier.
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
