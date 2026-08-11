# Progress 7 runtime map

This checkpoint crosses the former `0x0019fddc` floating-point frontier and
maps/reconstructs several contiguous runtime libraries linked into SNES Station
v0.23. Target assembly is authoritative; historical source is used only after
independent signatures/boundaries are established.

## Newlib 1.10 mathfp

| Address | Symbol | State |
|---|---|---|
| `0x0019fddc` | `cosf` | reconstructed |
| `0x001a0024` | `sinf` | reconstructed |
| `0x001a0254` | `tanf` | reconstructed |
| `0x001a045c` | `atanf` | reconstructed |
| `0x001a06a0` | `sqrtf` | reconstructed |
| `0x001a06b0` | `fabsf` | reconstructed |
| `0x001a06c0` | `numtestf` | reconstructed |

The polynomial constants match Newlib 1.10 `mathfp` source. The target replaces
the generic C `sqrtf`/`fabsf` implementations with direct EE `sqrt.s`/`abs.s`
leaves. `numtestf` also preserves an EE/`-mlong64` sign-extension quirk rather
than being rewritten into a modern classifier.

## Memory-card library

`0x001a0740..0x001a1af4` is the old EE `libmc` client. The entire public flow is
now represented in recovered C, including `mcInit`, get/open/close/seek/read/
write/flush, directory/info operations, format/unformat, rename, async `mcSync`,
`strcpy_sjis`, and `SifCheckStatRpc`.

Target-specific differences from later PS2SDK source are retained. In
particular this `mcInit` revision predates the later `_iop_reboot_count` reset
prelude; `mcWrite` keeps its alignment staging; `mcMkDir` routes through the
open command; and `mcChangeThreadPriority` receives but does not transmit the
level argument.

## GCC 3.2.2-b1 runtime fingerprint

The `libgcc` corridor provides stronger toolchain evidence than shared-source
similarity alone. Object boundaries in the target repeatedly equal the archive
member sizes recorded by a historical EE GCC 3.2.2-b1 link:

| Target range | Recovered object | Size |
|---|---|---:|
| `0x001a25b0..0x001a2c77` | `_udivdi3.o` | `0x6c8` |
| `0x001a2c78..0x001a333f` | `_umoddi3.o` | `0x6c8` |
| `0x001a3340..0x001a337f` | SF→DF / `fptodp` | `0x40` |
| `0x001a3380..0x001a367f` | `_addsub_df.o` | `0x300` |
| `0x001a3680..0x001a394f` | `_mul_df.o` | `0x2d0` |
| `0x001a3950..0x001a3ad7` | `_div_df.o` | `0x188` |
| `0x001a3dc0..0x001a5cbf` | `unwind-dw2.o` | `0x1f00` |
| `0x001a5cc0..0x001a757f` | `unwind-dw2-fde.o` | `0x18c0` |
| `0x001a7e30..0x001a7f0f` | `_unpack_sf.o` | `0xe0` |
| `0x001a7f10..0x001a7f3f` | `_make_sf.o` | `0x30` |
| `0x001a7f40..0x001a80cf` | `_pack_df.o` | `0x190` |
| `0x001a80d0..0x001a81b7` | `_unpack_df.o` | `0xe8` |
| `0x001a81b8..0x001a82bf` | `_fpcmp_parts_df.o` | `0x108` |
| `0x001a82c0..0x001a841f` | `_pack_sf.o` | `0x160` |

Even public `_Unwind_*` entries retain the same offsets inside the two unwind
objects. This is exceptionally strong evidence for the compiler/runtime family,
but **Matching remains 0.00%** until a reproducible rebuild produces
byte-identical machine code after relocations are handled.

## Pad library / XPADMAN

`0x001a8420..0x001a90f7` is the NEW/XPADMAN `libpad` client. The target binds RPC
IDs `0x80000100` and `0x80000101`, consistent with the XPADMAN modules loaded at
startup. Recovered functions include DMA double-buffer selection, init/end,
port open/close/read/state, mode/pressure/button queries, actuator setup/direct
control, and connection query.

`padInit` remains **partial** in the research C model because the target's exact
busy-wait timing around both binds is not modeled, even though the RPC IDs,
version query, state initialization and command `0x10` are proven.

## C++ EH boundary

Immediately after libpad:

- `0x001a90f8` — `operator delete(void*)`
- `0x001a9118` — `operator delete[](void*)`
- `0x001a9138..0x001a947f` — a second `unwind-pe` helper copy linked into
  `eh_personality.o`
- `0x001a9488` — `parse_lsda_header`
- `0x001a9588` — `get_ttype_entry`
- `0x001a95f8` — `get_adjusted_ptr`
- `0x001a9698` — `check_exception_spec`
- `0x001a9728` — `__gxx_personality_v0`
- `0x001a9b88` — `__cxa_call_unexpected`
- `0x001a9ca8..0x001a9e7f` — terminate/unexpected/throw/rethrow support
- `0x001a9e88` — `operator new`
- `0x001a9f68` — `operator new[]`

The next clean frontier is **`0x001a9fa8`**, where the linked `std::type_info`
and RTTI hierarchy begins.
