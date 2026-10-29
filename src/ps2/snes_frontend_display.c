/*
 * SNES Station v0.23 PS2 - Frontend Display Defaults
 * Canonical source from progress11_frontend.c @ 0x00106054
 */

#include <stdint.h>

typedef struct {
    uint8_t enable0;
    uint8_t enable1;
    uint8_t mode;
    uint8_t zero3;
    uint8_t zero4;
    uint8_t zero5;
    uint8_t enable6;
    uint8_t pad07;
    uint32_t width_code;
    uint32_t height_code;
    uint32_t value10;
    uint32_t value14;
    uint32_t value18;
    uint32_t value1c;
} SnesFrontendDisplayDefaults;

void snes_frontend_display_defaults_00106054(SnesFrontendDisplayDefaults *state,
                                              int8_t region_byte)
{
    state->enable0 = 1u;
    state->enable1 = 1u;
    state->mode = 2u;
    state->zero3 = 0u;
    state->zero4 = 0u;
    state->zero5 = 0u;
    state->enable6 = 1u;
    if (region_byte == 0x45) {
        state->width_code = 0xaau;
        state->height_code = 0x50u;
    } else {
        state->width_code = 0x82u;
        state->height_code = 0x32u;
    }
    state->value10 = 0x3eu;
    state->value14 = 0x19u;
    state->value18 = 0x46u;
    state->value1c = 0x2du;
}