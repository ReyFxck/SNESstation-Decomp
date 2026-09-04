# V101 Part 5E — final Stage-3F closure

V101 Part 5E closes the final two Stage-3F unresolved addresses without
inventing target storage sections.

## `DAT_0042355a` -> `rtc_f9.control`

The target block `0x00423548..0x00423560` is exactly 0x18 bytes and matches
the historical `spc7110.o::rtc_f9` object byte-for-byte.

Historical `S7RTC` layout under the EE ABI32 target:

- `reg[16]` at `+0x00`
- `index` at `+0x10`
- `control` at `+0x12`
- `init` at `+0x13`
- `last_used` at `+0x14`
- total extent `0x18`

Therefore `DAT_0042355a` is exactly `rtc_f9.control`.

The same historical object is corroborated by strict matching identities for
`S9xGetSPC7110`, `S9xSetSPC7110`, and `S9xUpdateRTC`.

Closure status:

`HISTORICAL_OBJECT_INTERIOR_FIELD`

## `DAT_00426820` -> linked `.eh_frame` FDE LSDA field byte

The private target contains a valid DWARF CIE/FDE pair:

- CIE: `0x004267f0`
- CIE version: 1
- augmentation: `zPL`
- personality encoding: `DW_EH_PE_absptr`
- LSDA encoding: `DW_EH_PE_absptr`
- EE/ELF32 absolute-pointer width: 4 bytes
- FDE: `0x0042680c`
- described function: `0x001a9e88`
- range: `0xdc`
- historical identity: `operator_new / _Znwj`
- FDE augmentation length: 4
- LSDA field: `0x0042681d..0x00426821`
- linked LSDA value: `0x00426bdc`

`DAT_00426820` is exactly the final byte of that four-byte LSDA field. The
operator-new FDE is unique for the exact function/range in the surrounding
linked exception-metadata corridor.

Closure status:

`TARGET_NATIVE_EH_FRAME_RELOCATION_BYTE`

## Final Stage-3F state

- 1209 `SECTION_BACKED_ADDRESS`
- 4 `RUNTIME_CODE_POINTER_REFACTOR`
- 8 `PCM_BUFFER_MINIMUM_EXTENT`
- 2 `RUNTIME_MEMBER_DATA_OBJECT`
- 1 `RUNTIME_INTERNAL_CODE_LABEL`
- 1 `HISTORICAL_OBJECT_INTERIOR_FIELD`
- 1 `TARGET_NATIVE_EH_FRAME_RELOCATION_BYTE`
- 0 `NO_PROVED_BACKING`
- 1265 / 1265 resolved

The final two custom closures claim identity only at their proved level and do
not create fake data-backing sections.

This closes the Stage-3F blocker frontier. It does **not** by itself prove a
replacement ELF. Full integrated linker layout, object/library ordering,
replacement unpacked ELF identity, and later packed-ELF reproduction remain
separate milestones.
