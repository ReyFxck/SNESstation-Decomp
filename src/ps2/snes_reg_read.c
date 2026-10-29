/*
 * SNES Station v0.23 PS2 - Register Read State Helper
 * Canonical source from progress24_core_helpers.c @ 0x0010b7f8
 */

#include <stdint.h>

typedef struct {
    uint8_t selector_f2;
    uint8_t shadow[0x80];

    uint32_t slot_active[8];
    uint8_t slot_status_hi[8];
    uint8_t slot_status_lo[8];

    uint8_t (*read_slot_gate)(void *opaque, unsigned slot);
    void *opaque;
} SnesRegisterReadState;

uint8_t snes_reg_read_0010b7f8(SnesRegisterReadState *state)
{
    uint32_t selector = state->selector_f2 & 0x7fu;
    unsigned slot = selector >> 4;
    uint8_t result = state->shadow[selector];

    if ((selector & 0x0fu) == 8u) {
        result = state->read_slot_gate(state->opaque, slot);
    } else if ((selector & 0x0fu) == 9u) {
        result = 0;
        if (state->slot_active[slot] != 0) {
            result = state->slot_status_hi[slot] |
                     state->slot_status_lo[slot];
        }
    }

    return result;
}