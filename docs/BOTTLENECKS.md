# Current frontier

Function recovery, unpacked-image identity and packed-file identity are
complete. The remaining items below are optional source-archaeology work; they
do not block the verified replacement ELF.

| Blocker | Current state | Required result |
|---|---|---|
| Clean historical application relink | Optional research | Replace evidence-integrated ranges with freshly compiled recovered objects while preserving exact identity |
| Complete source-level data bounds | Address identities are 1,265/1,265, but some full object/array extents remain unknown | Improve source archaeology for sizes, alignment, zero-fill and ownership |
| Loader stub / outer ELF | Closed: 12,700/12,700 bytes exact | Public SjCRUNCH 2.1 startup object, archive and linker script reproduce the wrapper |

Current whole-image result:

| Measure | Result |
|---|---:|
| Exact 64 KiB windows | **51/51** |
| Exact indices | **0–50** |
| Remaining indices | **None** |
| Remaining different bytes | **0** |
| Exact compressed blocks | **13/13** |
| Exact SJCRUNCH2 container | **714,268/714,268 bytes** |
| Loader stub / outer ELF | **12,700/12,700 bytes** |
| Complete replacement ELF | **726,968/726,968 bytes** |

`make reproduce` is the authoritative workflow. `make check` validates the
public evidence without the target, and `make reproduce-check` reruns all
available private-reference gates.

See [`ROADMAP.md`](ROADMAP.md) for the closed definition of done and
[`RECOVERY_HISTORY.md`](RECOVERY_HISTORY.md) for what has already been
recovered.
