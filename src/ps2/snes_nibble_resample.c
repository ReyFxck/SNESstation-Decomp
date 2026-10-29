/*
 * SNES Station v0.23 PS2 - Packed-Nibble Resampler
 * Canonical source from progress26_dsp_runtime_batch.c @ 0x0012e5b8
 */

#include <stdint.h>

typedef struct {
    int length;
    int source_scale;
    const uint8_t *source;
    uint8_t *destination;
} SnesNibbleResampler;

void snes_nibble_resample_0012e5b8(SnesNibbleResampler *s)
{
    uint8_t temporary[512];
    int i;

    if (s->length == 0)
        __builtin_trap();

    for (i = 0; i < s->length * 2; ++i) {
        uint32_t index = (uint32_t)((i * s->source_scale) / s->length);
        uint8_t packed = s->source[index >> 1];

        temporary[i] = (index & 1u) == 0 ? (packed >> 4) : (packed & 0x0fu);
    }

    for (i = 0; i < s->length; ++i)
        s->destination[i] = (uint8_t)(temporary[i * 2] << 4) |
                            temporary[i * 2 + 1];
}