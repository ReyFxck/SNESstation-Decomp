/*
 * SNES Station v0.23 PS2 - Waveform Lookup Table Construction
 * Canonical source from progress26_dsp_runtime_batch.c @ 0x0012c02c
 */

#include <stdint.h>

typedef struct {
    uint64_t (*u32_to_double)(uint32_t value);
    uint64_t (*double_mul)(uint64_t a, uint64_t b);
    float (*double_to_float)(uint64_t raw);
    float (*wave_a)(float value);
    float (*wave_b)(float value);
} SnesWaveHooks;

typedef struct {
    float table_a[0x800];
    float table_b[0x800];
    uint64_t scale_a;
    uint64_t scale_b;
    uint64_t fraction_scale;
    SnesWaveHooks hooks;
} SnesWaveTables;

void snes_wave_table_build_0012c02c(SnesWaveTables *s)
{
    uint32_t i;

    for (i = 0; i < 0x800u; ++i) {
        uint64_t raw = s->hooks.u32_to_double(i);
        float f;

        raw = s->hooks.double_mul(raw, s->scale_a);
        raw = s->hooks.double_mul(raw, s->fraction_scale);
        f = s->hooks.double_to_float(raw);
        s->table_a[i] = s->hooks.wave_a(f);

        raw = s->hooks.u32_to_double(i);
        raw = s->hooks.double_mul(raw, s->scale_b);
        raw = s->hooks.double_mul(raw, s->fraction_scale);
        f = s->hooks.double_to_float(raw);
        s->table_b[i] = s->hooks.wave_b(f);
    }
}