/*
 * SNES Station v0.23 PS2 - PPU Timing/Update Helper
 * Canonical source from progress22_more_small_helpers.c @ 0x0015d334
 */

#include <stdint.h>

extern uint8_t *g_fillram_0034e2c4;
/* Shared frontend/PPU state anchored at 0x003454e0. */
extern uint8_t g_state_003454e0[];
extern int sub_001309c4(uint32_t value);
extern void sub_00116128(int value);

void snes_ppu_timing_update_0015d334(void)
{
    uint8_t *fillram;
    int status;

    if (g_state_003454e0[0x60] != 0) {
        fillram = g_fillram_0034e2c4;
        if ((fillram[0x3030] & 0x20u) != 0 &&
            (fillram[0x303a] & 0x18u) == 0x18u) {
            if (!g_state_003454e0[0x12b] || g_state_003454e0[0x12a])
                (void)sub_001309c4(~0u);
            else
                (void)sub_001309c4((fillram[0x3039] & 1u) ? 700u : 350u);

            status = g_fillram_0034e2c4[0x3030] |
                     (g_fillram_0034e2c4[0x3031] << 8);
            if ((status & 0x8020) == 0x8000)
                sub_00116128(4);
        }
    }
}