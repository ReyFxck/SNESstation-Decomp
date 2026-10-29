/*
 * SNES Station v0.23 PS2 - Frontend Controller Mix State
 * Canonical source from progress11_frontend.c @ 0x0017409c, 0x00173ff0, 0x001744d0
 */

#include <stdint.h>

typedef struct {
    uint8_t selector;
    uint8_t force_center;
    uint32_t preserve_axis;
    int16_t mixed_x;
    int16_t y;
    int32_t slot_value[2];
    int32_t slot_pair_value[2];
} SnesFrontendControllerMixState;

typedef struct {
    uint32_t values[8];
    uint8_t neutral_only;
} SnesFrontendControllerTableState;

static int16_t snes_frontend_axis_mix(int16_t x, int16_t y)
{
    int32_t ax = x;
    int32_t ay = y;

    if (ax < 0)
        ax = -ax;
    if (ay < 0)
        ay = -ay;
    return (int16_t)((ax + ay) / 2);
}

void snes_frontend_controller_mix_pair_0017409c(SnesFrontendControllerMixState *state,
                                                 int16_t x, int16_t y)
{
    uint32_t slot = (uint32_t)(state->selector & 1u);
    uint32_t other = slot ^ 1u;
    int16_t mixed = x;

    if (state->preserve_axis == 0u)
        mixed = snes_frontend_axis_mix(x, y);
    state->slot_pair_value[slot] = mixed;
    state->mixed_x = mixed;
    state->y = y;
    state->slot_pair_value[other] = y;
}

void snes_frontend_controller_mix_single_00173ff0(SnesFrontendControllerMixState *state,
                                                   int16_t x, int16_t y)
{
    uint32_t slot;
    uint32_t other;
    int16_t mixed = x;

    if (state->force_center != 0u) {
        state->mixed_x = 0x7f;
        state->y = 0x7f;
        state->slot_value[0] = 0x7f;
        state->slot_value[1] = 0x7f;
        return;
    }
    if (state->preserve_axis == 0u)
        mixed = snes_frontend_axis_mix(x, y);
    state->mixed_x = mixed;
    state->y = y;
    slot = (uint32_t)(state->selector & 1u);
    other = slot ^ 1u;
    state->slot_value[slot] = mixed;
    state->slot_value[other] = y;
}

void snes_frontend_controller_table_update_001744d0(SnesFrontendControllerTableState *state,
                                                     uint32_t index, uint32_t value)
{
    unsigned i;
    uint8_t neutral = 0u;

    state->values[index & 7u] = value;
    if (state->values[0] == 0u || state->values[0] == 0x7fu) {
        neutral = 1u;
        for (i = 1u; i < 8u; ++i) {
            if (state->values[i] != 0u) {
                neutral = 0u;
                break;
            }
        }
    }
    state->neutral_only = neutral;
}