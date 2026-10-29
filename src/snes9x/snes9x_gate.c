/*
 * SNES Station v0.23 - SNES9X Renderer/Core Gate Predicate
 * Canonical source from progress13_core_helpers.c (0x001308f8 range)
 */

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t cursor_3c;
    uint32_t mode_4c;
    uint32_t window_58;
    uint8_t enabled_5ec;
    uint8_t object98_byte3a;
    uint8_t final_byte3a;
} Snes9xGateState;

/* 0x001308f8 -- renderer/core gate predicate from the +0x342a38 state block. */
int snes9x_gate_predicate_001308f8(const Snes9xGateState *s)
{
    if (s->enabled_5ec != 0 && s->cursor_3c >= s->window_58 &&
        s->cursor_3c < s->window_58 + 0x200u)
        return 1;

    if (s->mode_4c < 0x40u && s->cursor_3c <= 0x7fffu)
        return 0;

    if ((uint32_t)(s->mode_4c - 0x60u) < 0x10u)
        return 0;
    if (s->mode_4c >= 0x74u)
        return 0;

    if ((uint32_t)(s->mode_4c - 0x70u) < 4u &&
        (s->object98_byte3a & 0x08u) == 0)
        return 0;

    return (s->final_byte3a & 0x10u) != 0;
}