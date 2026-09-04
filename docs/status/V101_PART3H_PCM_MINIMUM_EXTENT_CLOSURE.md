# V101 Part 3H — PS2 PCM minimum-consumed-extent closure

Eight former Stage-3F blockers are aliases into two PS2 PCM output buffers.

Historical strict matching proves target `0x001078f8` is `SjPCM_Enqueue`.

The MATCHING `main` control-flow graph proves the unique enqueue-size store
`DAT_001ebae0` can receive exactly `800` or `960`, with no dynamic reaching
definition. The recovered target control model proves mixer `sample_count`
can reach at most `1920`.

Worst proven low-buffer access:

`((1920 - 1) << 3) + 6 = 0x3bfe`

The final halfword ends at byte `0x3bff`, proving a minimum consumed extent of
`0x3c00` bytes per channel:

- `pcm_left`:  `0x001bbd80..0x001bf980`
- `pcm_right`: `0x001d3480..0x001d7080`

The eight aliases are closed as `PCM_BUFFER_MINIMUM_EXTENT`.

This does not claim that `0x3c00` is the complete original C-array size.
No fabricated `.data` or `.bss` section is created.

Expected Stage-3F:
- 1209 `SECTION_BACKED_ADDRESS`
- 4 `RUNTIME_CODE_POINTER_REFACTOR`
- 8 `PCM_BUFFER_MINIMUM_EXTENT`
- 5 `NO_PROVED_BACKING`
- 1265 total

Replacement ELF identity remains unproved.
