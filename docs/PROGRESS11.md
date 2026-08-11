# Progress 11 — APU allocation and per-ROM cleanup

Progress 11 continues the target-first recovery from the Progress 10 checkpoint
(`6966ffb`, 60.25% reconstructed / 65.00% mapped).

The policy is unchanged: the SNES Station v0.23 R5900 machine code is
authoritative. Historical Snes9x/PS2 source can help with nomenclature, but a
name or implementation is not imported unless the target evidence supports it.

## Reconstructed in this checkpoint

### `0x0010a840` — APU/audio buffer allocator

The target allocates exactly three buffers in this order from the global state
at EE VA `0x00345498`:

- state `+0x04` — `0x10000` bytes;
- state `+0x20` — `0x10000` bytes;
- state `+0x24` — `0x40000` bytes.

If any allocation is null, the target calls `0x0010a8bc` and returns `0`.
Otherwise it returns `1`.

Recovered source: `src/snes9x/apu_alloc_recovered.c`.

### `0x0010a8bc` — APU/audio buffer cleanup

This JAL target frees and then nulls those same fields in exact target order:
`+0x04`, `+0x20`, `+0x24`.

The global symbol is intentionally address-labelled in the recovery because no
historical variable name has been proven.

### `0x00151330` — per-ROM cleanup orchestrator

The existing partial target is now represented behaviorally. It performs only
two calls in the recovered slice:

1. `0x00151360(memory)`;
2. `0x00150f54(memory, 0)`.

No historical C++ method name is claimed for either call.

Recovered source: `src/snes9x/memory_cleanup_recovered.c`.

### `0x00151360` — per-ROM temporary-buffer cleanup

The helper uses `memory + 0x8000` as its base in the R5900 code and frees the
32-bit pointers at effective object offsets `+0xb064` and `+0xb068`, nulling
each field immediately after `free`.

## Deliberately not promoted

`0x0012a400` remains `IDENTIFIED`.

Its focused evidence file proves the VRAM byte write and invalidation of the
2bpp/4bpp/8bpp tile-validity maps, but the captured slice ends while the second
control-flow path is still live and branches back into the parent routine.
Progress 11 therefore does **not** claim a standalone reconstructed C function
for that subentry.

This is intentional: coverage must follow complete target evidence, not the
desire to raise the percentage.

## Expected accounting

Against the existing 1,137 heuristic JAL-target proxy, when applied on top of
Progress 10:

- Matching: **0 / 1,137 = 0.00%**
- Reconstructed / matching: **689 / 1,137 = 60.60%**
- Mapped: **741 / 1,137 = 65.17%**

The two newly tracked addresses (`0x0010a8bc`, `0x00151360`) are actual entries
in `analysis/jal_candidates.csv`; the other two were already tracked as
`IDENTIFIED`/`PARTIAL` and are promoted only after their complete target
behavior was recovered.

## Validation

`tools/apply_progress11.py`:

- verifies all four addresses exist in `analysis/jal_candidates.csv`;
- updates/inserts the four manifest rows without duplicating addresses;
- regenerates the README/progress SVG through `tools/update_progress.py`;
- syntax-checks the two new C translation units with
  `-std=c11 -Wall -Wextra -Werror` when a host C compiler is available;
- writes `analysis/progress11_validation.txt` with the local results.

`MATCHING` intentionally remains zero. Byte matching still requires a
historically correct EE compiler/linker reconstruction plus relocation-aware
comparison against the target ELF.
