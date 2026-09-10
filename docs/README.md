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
- [`status/V105_HISTORICAL_CXX_TAIL_INTEGRATION.md`](status/V105_HISTORICAL_CXX_TAIL_INTEGRATION.md) — semantic frontend/tail unwind metadata and six rebuilt Snes9x data providers close four more whole-image windows (16/51 exact; 35 remain).
- [`status/V106_RUNTIME_TAIL_SOURCE_INTEGRATION.md`](status/V106_RUNTIME_TAIL_SOURCE_INTEGRATION.md) — 14 source-derived `TILE.CPP`/`libsupc++` tail sections remove 2,353 proved differences from window 50.
- [`status/V107_TAIL_METADATA_WINDOW50.md`](status/V107_TAIL_METADATA_WINDOW50.md) — 34 public-source sections and five semantic metadata sections close window 50 exactly (17/51 exact; 34 remain).
- [`status/V108_WINDOW36_SOURCE_INTEGRATION.md`](status/V108_WINDOW36_SOURCE_INTEGRATION.md) — DSP1, CPU opcode tables, `fxemu`, `fxinst` and semantic unwind metadata close window 36 exactly (18/51 exact; 33 remain).
- [`status/V109_EMBEDDED_MEDIA_INTEGRATION.md`](status/V109_EMBEDDED_MEDIA_INTEGRATION.md) — six hash-verified media containers close windows 15–34 without publishing private payload (38/51 exact; 13 remain).
- [`status/V110_WINDOW35_SOURCE_DATA.md`](status/V110_WINDOW35_SOURCE_DATA.md) — rebuilt Snes9x source data and 47 semantic FDEs close window 35 (39/51 exact; 12 remain).
- [`status/V111_WINDOW11_PUBLIC_RODATA.md`](status/V111_WINDOW11_PUBLIC_RODATA.md) — 49 public-source sections/slices remove 23,469 differences from window 11 while preserving its explicit 2,460-byte residual.
- [`status/V112_CODE_WINDOWS_1_6.md`](status/V112_CODE_WINDOWS_1_6.md) — proved historical/recovered code and a labelled 7,088-byte scheduling residual close windows 1–6 (45/51 exact; 263,765 differences remain).
- [`status/V104_EXACT_STARTUP_INTEGRATION.md`](status/V104_EXACT_STARTUP_INTEGRATION.md) — pinned historical `_start`/`_exit`/`_root`, exact entry, 276 startup bytes and 27 relocations integrated ahead of the real Stage-3F aggregate.
- [`status/V103_DECOMP_DEV_REPORTING.md`](status/V103_DECOMP_DEV_REPORTING.md) — deterministic public Objdiff Report v2 and GitHub Actions artifact for honest decomp.dev tracking.
- [`status/V102_CLEAN_STAGE3G_LINK_PROBE.md`](status/V102_CLEAN_STAGE3G_LINK_PROBE.md) — 1,265/1,265 address identities retained; first clean executable diagnostic with 179 exact fixed sections and 12/51 exact image windows.
- [`status/V101_PART5E_FINAL_STAGE3F_CLOSURE.md`](status/V101_PART5E_FINAL_STAGE3F_CLOSURE.md) — final two identities and the zero-unresolved Stage-3F address frontier.
- [`status/V97_HISTORICAL_DATA_AND_ROM_OFFSETS.md`](status/V97_HISTORICAL_DATA_AND_ROM_OFFSETS.md) — 1,175 backed addresses + 29 ROM refactors; 61 remain. Sixteen historical source intervals reproduce 790,988 data bytes.
- [`status/V98_SOURCE_DATA_INTEGRATION.md`](status/V98_SOURCE_DATA_INTEGRATION.md) — 22 more addresses closed; 39 remain. Forty-nine providers reproduce 810,542 bytes, with 695,316 source-built bytes integrated into the backing link.
- [`status/V96_CONTROL_FLOW_DATA_ACCESSES.md`](status/V96_CONTROL_FLOW_DATA_ACCESSES.md) — preceding branch/loop-aware proof: 824 witnessed contracts and 961 backed addresses.
- [`status/V95_SECTION_BACKED_DATA_ALIASES.md`](status/V95_SECTION_BACKED_DATA_ALIASES.md) — initial 886/1,265 section-backed-address checkpoint, 69,768 proved bytes and an isolated address-relocation check.
- [`status/V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md`](status/V94_RUNTIME_OVERRIDES_AND_DATA_ACCESSES.md) — runtime ledger 53/53; exact target-selected puts/abort overrides; 705/1,265 minimum data-access spans, not full Stage-3F closure.
- [`status/V93_STAGE3D_RUNTIME_MEMBERS.md`](status/V93_STAGE3D_RUNTIME_MEMBERS.md) — historical 43 runtime contracts / 42 complete PS2LIB member texts checkpoint, Stage 3D 51/53.
- [`status/V92_STAGE3D_SNPRINTF_REFACTOR.md`](status/V92_STAGE3D_SNPRINTF_REFACTOR.md) — four original sprintf calls proved; synthetic snprintf removed, zero runtime shims; historical Stage-3D 8/53 checkpoint.
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
