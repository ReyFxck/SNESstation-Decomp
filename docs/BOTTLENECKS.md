# Current bottlenecks

Function recovery and unpacked-image identity are complete. The remaining work
is exact ELF link identity and packed-file identity.

| Blocker | Current state | Required result |
|---|---|---|
| Historical link | Incomplete | Exact linker script, section addresses, object/archive order and symbol binding |
| Complete data bounds | Address identities are 1,265/1,265, but some full object/array extents are still unknown | Prove complete sizes, alignment, zero-fill and ownership where the final link depends on them |
| Loader stub / outer ELF | 12,700 bytes remain outside the now-exact compressed container | Rebuild the stub, ELF/program/section headers and trailing metadata from public source |

Current whole-image result:

| Measure | Result |
|---|---:|
| Exact 64 KiB windows | **51/51** |
| Exact indices | **0–50** |
| Remaining indices | **None** |
| Remaining different bytes | **0** |
| Exact compressed blocks | **13/13** |
| Exact SJCRUNCH2 container | **714,268/714,268 bytes** |
| Loader stub / outer ELF remaining | **12,700 bytes** |
| Complete replacement ELF | **Not yet** |

`make reproduce` is the authoritative workflow. `make check` validates the
public evidence without the target, and `make reproduce-check` reruns all
available private-reference gates.

See [`ROADMAP.md`](ROADMAP.md) for the completion order and
[`RECOVERY_HISTORY.md`](RECOVERY_HISTORY.md) for what has already been
recovered.
