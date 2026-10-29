/*
 * SNES Station v0.23 - SNES9X PPU/Reset State and RAM I/O Defaults
 * Canonical source from progress13_core_helpers.c (0x0015d8ec range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t word14;
    uint8_t byte18;
    uint8_t byte19;
    uint8_t byte1a;
    uint8_t byte1b;
    uint8_t byte1c;
    uint16_t secondary40;
    uint16_t secondary42;
    uint32_t secondary44;
    uint64_t secondary48;
    uint8_t secondary50;
} Snes9xPpuResetState;

/* 0x0015d8ec -- PPU/reset state plus RAM I/O defaults. */
void snes9x_ppu_reset_0015d8ec(Snes9xPpuResetState *state, uint8_t *ram)
{
    state->byte18 = 0;
    state->byte19 = 0;
    state->byte1a = 0;
    state->byte1b = 0;
    state->byte1c = 0;
    state->word14 = 0;

    memset(ram + 0x2200, 0, 0x200);
    ram[0x2200] = 0x20;
    ram[0x2220] = 0;
    ram[0x2221] = 1;
    ram[0x2222] = 2;
    ram[0x2223] = 3;
    ram[0x2228] = 0xff;

    state->secondary40 = 0;
    state->secondary42 = 0;
    state->secondary44 = 0;
    state->secondary48 = 0;
    state->secondary50 = 0;
}