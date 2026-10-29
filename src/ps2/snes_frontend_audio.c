/*
 * SNES Station v0.23 PS2 - Frontend Audio Config
 * Canonical source from progress11_frontend.c @ 0x00105cb8
 */

#include <stdint.h>

typedef struct {
    uint32_t pad00[2];
    uint32_t rate;
    uint32_t arg;
    uint32_t pad10[2];
    uint32_t option;
    uint8_t valid;
} SnesFrontendAudioConfig;

int snes_frontend_audio_config_00105cb8(SnesFrontendAudioConfig *state,
                                         int mode, uint32_t option, uint32_t arg)
{
    uint32_t rate = 0u;

    state->option = option & UINT32_C(0xff);
    state->valid = 1u;
    if (mode == 1)
        rate = 12000u;
    else if (mode == 2)
        rate = 24000u;
    else if (mode == 3)
        rate = 48000u;
    state->rate = rate;
    state->arg = arg;
    return 1;
}