/*
 * SNES Station v0.23 - PPU Bounds Mode 4 Query
 * Canonical source from progress25_small_core_helpers.c (0x0015cce4 range)
 */

#include <stdint.h>

typedef struct SnesPpuBoundsState {
    int mode;               /* target 0x0035d0b8 */
    uint8_t io_4201;
    uint8_t io_213f;

    uint32_t packed_flags;  /* target 0x0035d0c0 */
    uint8_t active;         /* target 0x0035bfdc */
    uint16_t x_clamped;     /* target 0x0035bfd8 */
    uint16_t line_plus_one; /* target 0x0035bfd6 */
    int line_count;         /* logical target 0x0035bff2 */

    int (*query)(void *opaque, int *x, int *line, uint32_t flags[2]);
    void *opaque;
} SnesPpuBoundsState;

/* 0x0015cce4
 *
 * Mode-4 query helper. Successful queries always update the packed flag word;
 * the remaining clamp/state writes are gated by bit 0x80 of target register
 * 0x4201. X is biased by 40 and clamped to [40,295], while the line is clamped
 * to [0, line_count-1].
 */
void snes_ppu_bounds_query_0015cce4(SnesPpuBoundsState *state)
{
    int x;
    int line;
    int clamped_line;
    uint32_t flags[2];

    if (state->mode != 4)
        return;
    if (state->query(state->opaque, &x, &line, flags) == 0)
        return;

    state->packed_flags =
        ((flags[0] & 1u) << 15) |
        ((flags[0] & 2u) << 13) |
        ((flags[0] & 4u) << 11) |
        ((flags[0] & 8u) << 9) |
        0xffu;

    if ((state->io_4201 & 0x80u) == 0)
        return;

    x += 0x28;
    if (x > 0x127)
        x = 0x127;
    if (x < 0x28)
        x = 0x28;

    state->active = 1;

    clamped_line = state->line_count - 1;
    if (line <= clamped_line)
        clamped_line = line;
    if (clamped_line < 0)
        clamped_line = 0;

    state->x_clamped = (uint16_t)x;
    state->line_plus_one = (uint16_t)((int16_t)clamped_line + 1);
    state->io_213f |= 0x43u;
}