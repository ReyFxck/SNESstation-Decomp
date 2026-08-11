# Progress 14 — evidence cleanup and integration pass

Progress 14 starts from the Progress 13 **70.01%** checkpoint. This is a
conservative promotion pass: it does not add new scanner hits and does not turn
identified library names into reconstructed rows merely because an upstream
symbol is known.

## Promoted targets

Three targets already had complete behavioral source/evidence in the Progress
13 tree but their manifest status still lagged behind that evidence:

- `0x00105750` — the SRAM path builder is now represented under its address-
  stable integration spelling. The target splits the loaded ROM path, truncates
  the basename to `0x1b` bytes, seeds `mc0:SNES_EMU/`, appends the basename and
  finally `.SRM`.
- `0x0010a840` — the APU allocator performs the exact `0x10000`, `0x10000`,
  `0x40000` allocation sequence and routes any failure through the recovered
  cleanup at `0x0010a8bc` before returning false.
- `0x00151330` — the complete per-ROM cleanup wrapper first invokes the
  recovered two-buffer free/clear leaf at `0x00151360`, then calls the proven
  target `0x00150f54(memory, 0)`. The historical C++ method name remains
  deliberately unclaimed.

## Integration cleanup

`src/app/main_flow_recovered.c` already refers to address-stable names for the
SRAM path builder and APU initializer. Progress 14 makes those spellings
available from the owning translation units while retaining compatibility
wrappers for older research callers.

## Accounting

On the unchanged conservative **1,137-target JAL proxy**:

- **799 reconstructed — 70.27%**
- **827 mapped — 72.74%**
- **0 matching — 0.00%**

Relative to Progress 13 this is **+3 reconstructed, +0 mapped**. The residual
non-green mapped set becomes **26 IDENTIFIED + 2 PARTIAL**. The two remaining
PARTIAL targets are the top-level `main @ 0x00104f18` flow and the exact
NEW/XPADMAN `padInit @ 0x001a8484` bind-wait/init corridor.

## Validation

Run:

```sh
python3 tools/apply_progress14.py
```

The helper patches both manifests in lockstep, regenerates the README/progress
SVG/generated snapshot, verifies the expected 799/827 totals and, when a host
`cc` is available, syntax-checks every recovered C translation unit with the
same strict C11 warning policy used by the previous checkpoint.

Matching remains **0.00%** until a historical-toolchain candidate is built and
its complete target machine code is compared byte-for-byte under normalized
link/relocation conditions.
