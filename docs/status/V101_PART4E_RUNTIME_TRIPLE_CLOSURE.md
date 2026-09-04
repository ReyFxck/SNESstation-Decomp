# V101 Part 4E — GCC runtime triple closure

Three former Stage-3F blockers now have exact GCC 3.2.2 runtime identities.

## `DAT_001babc8` -> `__clz_tab`

- archive member: `_clz.o`
- section: `.rodata`
- member offset: `0x0`
- extent: `0x100`
- target/full-object SHA-256:
  `14a5d850c255623f9472e3c650abce0c78d32f0276b315b3a276a0462d97a1ac`
- exact full-object match in all four local GCC 3.2.2 libgcc copies
- target references occur from the recovered `__udivmoddi4` family

Closure status: `RUNTIME_MEMBER_DATA_OBJECT`.

## `UNK_001ba7e0` -> `__thenan_df`

- archive member: `_thenan_df.o`
- section: `.rodata`
- member offset: `0x0`
- extent: `0x18`
- SHA-256:
  `9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0`
- exact match in all four local GCC 3.2.2 libgcc copies
- direct target address witnesses:
  - `_fpadd_parts`: `0x001a33c8 / 0x001a33cc`
  - `_fpmul_parts`: `0x001a371c / 0x001a3744`
  - `_fpdiv_parts`: `0x001a39fc / 0x001a3a14`

Closure status: `RUNTIME_MEMBER_DATA_OBJECT`.

## `UNK_001a6320` -> `fde_unencoded_compare`

- archive member: `unwind-dw2-fde.o`
- section: `.text`
- member offset: `0x660`
- local symbol: `fde_unencoded_compare`
- symbol size: `0x28`
- exact target bytes revalidated in all four GCC 3.2.2 libgcc copies
- `init_object` materializes `0x001a6320` at the frozen target witnesses

Closure status: `RUNTIME_INTERNAL_CODE_LABEL`.

These closures do not fabricate Stage-3F target storage sections. The exact
runtime member identities are tracked by `runtime_residual_identities.py`.

Expected state after Part 4E:

- 1209 `SECTION_BACKED_ADDRESS`
- 4 `RUNTIME_CODE_POINTER_REFACTOR`
- 8 `PCM_BUFFER_MINIMUM_EXTENT`
- 2 `RUNTIME_MEMBER_DATA_OBJECT`
- 1 `RUNTIME_INTERNAL_CODE_LABEL`
- 2 `NO_PROVED_BACKING`
- 1265 total

The two remaining blockers are expected to be the isolated high-address
providers and require separate owner/layout proof.

Replacement ELF identity remains unproved.
