# Current bottlenecks

Function recovery is no longer the bottleneck. The remaining work is exact
whole-image and packed-file identity.

| Blocker | Current state | Required result |
|---|---|---|
| Windows 7–10 | Different | Select exact implementation objects and reproduce relocations/layout |
| Historical link | Incomplete | Exact linker script, section addresses, object/archive order and symbol binding |
| Complete data bounds | Address identities are 1,265/1,265, but some full object/array extents are still unknown | Prove complete sizes, alignment, zero-fill and ownership where the final link depends on them |
| Packer | SJCRUNCH2 container understood; exact revision and parameters still unknown | Reproduce stub, 13-block layout and packed bytes |

Current whole-image result:

| Measure | Result |
|---|---:|
| Exact 64 KiB windows | **47/51** |
| Exact indices | **0–6, 11–50** |
| Remaining indices | **7–10** |
| Remaining different bytes | **202,702** |
| Complete replacement ELF | **Not yet** |

`make reproduce` is the authoritative workflow. `make check` validates the
public evidence without the target, and `make reproduce-check` reruns all
available private-reference gates.

See [`ROADMAP.md`](ROADMAP.md) for the completion order and
[`RECOVERY_HISTORY.md`](RECOVERY_HISTORY.md) for what has already been
recovered.
