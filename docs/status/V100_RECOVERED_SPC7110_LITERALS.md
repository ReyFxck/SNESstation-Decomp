# V100 — recovered SPC7110 literal backing

V100 closes seven Stage-3F address-only blockers with five minimal recovered
source-literal owners. It does **not** claim whole-object or whole-.rodata
identity for the historical Snes9x SPC7110 translation unit.

## Closed labels

- `DAT_001b8530` — inside exact `SMHT-SP7\0`
- `DAT_001b8538` — NUL terminator inside exact `SMHT-SP7\0`
- `DAT_001b8558` — inside exact `FEOEZSP7\0`
- `DAT_001b8580` — inside exact `MISC-SP7\0`
- `DAT_001b8588` — NUL terminator inside exact `MISC-SP7\0`
- `DAT_001b8590` — inside exact `SJUMPSP7\0`
- `UNK_001b85c8` — exact recovered `"/\0"` path-separator literal

## Evidence boundary

The public source lineage contains the SPC7110 pack-directory literals. Target
code independently materializes the principal literal addresses in the
`0x001806a4..0x00181418` corridor. The private gate compares every recovered
payload byte against the SHA-verified unpacked ELF before the data-backing
manifest is accepted.

This is target-guided source reconstruction, not a claim that the complete
historical `.rodata` section has one stable placement. Previous V100 probes
demonstrated conflicting valid historical `.rodata` relocation bases
(`0x001b84f0` and `0x001b8580`) across exact source-matching functions, which
is why V100 deliberately proves only these minimal literal intervals.

## Scope

After this closure, Stage-3F should report:

- `1204` section-backed addresses;
- `29` ROM-offset source refactors;
- `10` code-pointer source aliases;
- `22` remaining `NO_PROVED_BACKING`;
- `1265` total address contracts.

Replacement ELF identity is still **not** claimed. Full layout, linker order,
remaining storage bounds, and packed-ELF reproduction remain later work.
