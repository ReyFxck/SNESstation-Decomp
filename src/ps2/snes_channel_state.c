/*
 * SNES Station v0.23 - Eight Channel State Management
 * Canonical source from progress25_small_core_helpers.c (0x0012b3e8 range)
 */

#include <stdint.h>
#include <string.h>

/* One target channel occupies 0x16 bytes from 0x0035d366. */
typedef struct SnesChannel22 {
    uint16_t source;        /* +0x00, target 0x0035d366 */
    uint16_t mirror;        /* +0x02, target 0x0035d368 */
    uint8_t reserved04[10];
    uint8_t clear_flag;     /* +0x0e, target 0x0035d374 */
    uint8_t set_flag;       /* +0x0f, target 0x0035d375 */
    uint8_t reserved10[6];
} SnesChannel22;

typedef struct SnesEightChannelState {
    uint8_t disable_source; /* target 0x0034556b */
    uint8_t source_420c;    /* target *(0x0034e2c4 + 0x420c) */

    uint8_t active_mask;    /* target 0x0035c269 */
    uint8_t reset_pending;  /* target 0x0035c26a */

    SnesChannel22 channel[8];

    /*
     * 0x0012b3e8 performs overlapping four-byte zero stores every two bytes
     * from target 0x0035d410. 18 bytes cover offsets 0..17 touched by all
     * eight stores.
     */
    uint8_t overlap_zero[18];
} SnesEightChannelState;

/* 0x0012b3e8
 *
 * Selects the active eight-channel mask, refreshes per-channel mirror/flags
 * for active bits, and performs the target's overlapping four-byte clears.
 */
void snes_channel_state_update_0012b3e8(SnesEightChannelState *state)
{
    unsigned i;

    state->active_mask = state->disable_source == 0
                       ? state->source_420c
                       : 0;
    state->reset_pending = 1;

    for (i = 0; i < 8; ++i) {
        if (((state->active_mask >> i) & 1u) != 0) {
            state->channel[i].mirror = state->channel[i].source;
            state->channel[i].set_flag = 1;
            state->channel[i].clear_flag = 0;
        }

        memset(&state->overlap_zero[i * 2], 0, 4);
    }
}