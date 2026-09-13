# Documentation

Start with the root [`README.md`](../README.md). It contains the current result,
the meaning of each headline number and the normal build commands.

## Current project

| Document | Purpose |
|---|---|
| [`status/PROJECT_STATUS.generated.md`](status/PROJECT_STATUS.generated.md) | Machine-generated current scoreboard |
| [`ROADMAP.md`](ROADMAP.md) | Closed definition of done and optional future research |
| [`BOTTLENECKS.md`](BOTTLENECKS.md) | Current research frontier after exact ELF completion |
| [`RECOVERY_HISTORY.md`](RECOVERY_HISTORY.md) | Historical origin and status of recovered code |
| [`TOOLS.md`](TOOLS.md) | Supported commands, implementation tools and versions |

## Reproduction and contribution

| Document | Purpose |
|---|---|
| [`REPRODUCTION.md`](REPRODUCTION.md) | Public checks, private reference gates and exact final artifact |
| [`MATCHING_WORKFLOW.md`](MATCHING_WORKFLOW.md) | Evidence required for a strict function match |
| [`DECOMP_PLAYBOOK.md`](DECOMP_PLAYBOOK.md) | Reverse-engineering workflow |
| [`HISTORICAL_EE_TOOLCHAIN.md`](HISTORICAL_EE_TOOLCHAIN.md) | Reproducible historical compiler build |
| [`TOOLCHAIN_FINGERPRINT.md`](TOOLCHAIN_FINGERPRINT.md) | Compiler evidence and remaining unknowns |
| [`../CONTRIBUTING.md`](../CONTRIBUTING.md) | Contribution rules |

## Target and subsystem references

| Area | Documents |
|---|---|
| Dependencies | [`DEPENDENCY_VERSIONS.md`](DEPENDENCY_VERSIONS.md) |
| Application flow | [`MAIN_FLOW.md`](MAIN_FLOW.md) |
| Renderer and GS | [`RENDERER_MAP.md`](RENDERER_MAP.md), [`PS2_GS_MAP.md`](PS2_GS_MAP.md) |
| Compression | [`ZLIB_MAP.md`](ZLIB_MAP.md), [`UNZIP_MAP.md`](UNZIP_MAP.md) |
| CD/DVD and kernel | [`CDVD_LIBKERNEL_MAP.md`](CDVD_LIBKERNEL_MAP.md), [`CDVD_RPC_STATUS.md`](CDVD_RPC_STATUS.md) |
| Embedded media | [`EMBEDDED_ASSETS.md`](EMBEDDED_ASSETS.md) |
| Legal and provenance | [`LEGAL.md`](LEGAL.md) |

## Generated and historical material

`PROGRESS.generated.md`, `SOURCE_COMPLETENESS.generated.md` and the current
status page are regenerated with `make docs` and checked by `make check`.
Machine-readable manifests under `analysis/` remain authoritative.

Numbered proof reports under `status/`, along with `archive/`, are retained so
old conclusions can be reproduced. They are evidence records, not the current
project overview, and are intentionally absent from the normal reading path.
