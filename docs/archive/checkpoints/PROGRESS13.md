# Progress 13 — 70% reconstruction milestone

Progress 13 continues the earlier-region pass from Progress 12 and deliberately
crosses the **70% reconstructed** mark without converting scanner hits or merely
named library routines into green rows.  Every newly added target is a real
`jal` target whose first observed call site is below the confirmed executable
boundary at `0x001b0880`; previously mapped targets are promoted only after an
independent behavioral model covers their complete target flow.

## Reconstructed target families

This checkpoint adds **51 reconstructed targets** over Progress 12.  Twenty-eight
of those increase mapped coverage; the other twenty-three were already mapped
and are now promoted from IDENTIFIED to RECONSTRUCTED.

### GCC/libgcc soft-float and unwind/FDE runtime

`src/ps2/progress13_libgcc_softfloat_recovered.c`,
`src/ps2/gcc_fde_runtime_recovered.c`, and
`src/snes9x/progress13_runtime_more_recovered.c` cover compact late-linked GCC
runtime leaves including:

- SF/DF unpack, pack and parts-construction helpers;
- signed 64-bit to double conversion at `0x001a7580`;
- compact FDE registration/search helpers and their target object state;
- preserved target class/sign/exponent/fraction layouts and rounding paths.

The SF/DF pack/unpack model is tested both on edge values and on randomized
finite round trips.  This is still a behavioral reconstruction, not a matching
compiler result.

### GCC 3.2.2-era C++ exception/RTTI support

`src/ps2/progress13_cpp_eh_recovered.c`,
`src/ps2/progress13_cpp_personality_recovered.c`, and the existing RTTI recovery
now cover several formerly identified-only leaves:

- `__terminate`, `__gxx_exception_cleanup`, `__cxa_throw`, `__cxa_rethrow`, and
  the target `operator new` retry/throw behavior;
- `_Unwind_DeleteException @ 0x001a5c58`;
- `parse_lsda_header @ 0x001a9488`;
- `get_ttype_entry @ 0x001a9588`;
- `get_adjusted_ptr @ 0x001a95f8`;
- `check_exception_spec @ 0x001a9698`;
- simple RTTI inheritance walkers whose full virtual-dispatch control flow is
  visible in the target.

The 32-bit EE address fields remain explicit in host models so a 64-bit test
host cannot silently change target offsets.

### Core, PPU, map, and renderer leaves

`src/snes9x/progress13_core_helpers_recovered.c`,
`src/snes9x/progress13_runtime_more_recovered.c`, and
`src/snes9x/progress13_frontend_more_recovered.c` add target-specific helpers
including:

- mirrored APU timer-target writes and PPU reset defaults;
- 256-entry palette expansion and mirrored pointer-map filling;
- `0x0010a18c` packed weighted color blending;
- `0x0012e374` 32-byte/four-bit-plane transpose;
- `0x0015e07c` complete 12-way Snes9x map-entry selector plus direct-pointer
  path;
- `0x0015f030` variable-bit shifted 32-bit read window and optional source
  advance;
- small frontend predicates, rotation/state helpers, and the seven-state
  `0x0015cdf8` progression machine.

The map selector preserves the target distinction among the main-RAM
`-0x2000/-0x4000/-0x6000` cases, secondary/default RAM cases, the alternate
`+0x28` base, and entries `>= 12` that are already direct bases.

### Frontend tables, controllers, persistence, and date helpers

The same second-pass source models reconstruct:

- the `0x00101924` four-scratch path-builder wrapper;
- the complete `0x00153674` / `0x00153780` frontend pointer/flag bank setup;
- the `0x00158974` structured-header checks and its bounded multibyte validator
  at `0x00158a58`;
- 0xe0-stride runtime/controller period and callback-mask helpers;
- exact 24-byte record write/read corridors at `0x001833a4` / `0x001834f0`;
- duplicate month-length helpers and the `0x001836d0` weekday calculation.

The date code intentionally keeps the target's simple `(year & 3)` February
rule rather than replacing it with Gregorian century handling.  The weekday
helper also preserves the target month-offset table and unsigned modulo-7
arithmetic.

## Evidence

`analysis/functions/progress13_targets.asm` contains focused assembly excerpts
for both Progress 13 passes.  The second-pass section includes complete target
ranges for every newly added helper and every IDENTIFIED target promoted by
that pass.  The source models were checked against target jump tables, delay
slots, global-offset accesses, and return conventions rather than inferred from
function names alone.

## Accounting

On the conservative 1,137-target JAL proxy:

- **796 reconstructed — 70.01%**
- **827 mapped — 72.74%**
- **0 matching — 0.00%**

Relative to Progress 12 (745 reconstructed / 799 mapped), this is
**+51 reconstructed and +28 mapped**.  The manifest still keeps **27
IDENTIFIED** and **4 PARTIAL** targets out of the green count.

## Validation

Before packaging Progress 13:

- all **83** recovered C translation units pass
  `cc -std=c11 -Wall -Wextra -Werror -fsyntax-only -Iinclude`;
- the original Progress 13 soft-float/core smoke test passes;
- finite SF/DF randomized round-trip validation passes;
- an additional second-pass smoke test passes month/date, stage-machine,
  record/RAM initialization, packed blending, map selection, controller-slot,
  bit-plane transpose, structured-header validation, PPU reload/window, and
  callback-gate checks;
- signed DI-to-double conversion matches the host IEEE conversion on **500,000
  randomized 64-bit inputs**;
- `analysis/progress_targets.csv` and `analysis/symbols.csv` each contain **827
  unique addresses** with identical status totals;
- all **21 newly mapped** second-pass addresses occur in
  `analysis/jal_candidates.csv` and have a first call site below `0x001b0880`;
- no target is promoted to MATCHING.

Matching therefore remains **0.00%** until a historical-toolchain candidate is
built and its complete machine code is compared byte-for-byte with relocation
and link placement normalized.
