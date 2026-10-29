/*
 * SNES Station v0.23 PS2 - Trigonometric Tables & Fixed-Point Helpers
 * Canonical source from progress26_dsp_runtime_batch.c @ 0x0012c188, 0x0012c2dc, 0x0012c398
 */

#include <stdint.h>

typedef struct {
    int16_t fine[256];       /* logical target 0x00340f98 */
    int16_t coarse[256];     /* logical target 0x00341198 */
    int16_t reciprocal[128]; /* logical target 0x00340e90 */
} SnesTrigTables;

void snes_trig_normalize_reciprocal_0012c188(const SnesTrigTables *t, int16_t mantissa,
                                              int16_t exponent, int16_t *out_mantissa,
                                              int16_t *out_exponent)
{
    int32_t m = mantissa;
    int32_t e = exponent;
    int32_t sign = 1;
    int16_t result;
    int16_t e16;

    if (m == 0) {
        *out_mantissa = INT16_C(0x7fff);
        *out_exponent = INT16_C(0x002f);
        return;
    }

    if (m < 0) {
        if (m < -0x7fff)
            m = -0x7fff;
        sign = -1;
        m = (int16_t)(-m);
    }

    while (m < 0x4000) {
        m = (int16_t)(m << 1);
        e = (int16_t)(e - 1);
    }

    e16 = (int16_t)e;
    if (m == 0x4000) {
        if (sign == 1) {
            result = INT16_C(0x7fff);
        } else {
            result = (int16_t)-0x4000;
            e16 = (int16_t)(e16 - 1);
        }
    } else {
        int32_t r = t->reciprocal[(m - 0x4000) >> 7];
        int32_t x;

        x = ((-r * ((m * r) >> 15)) >> 15) + r;
        x = (int16_t)(x * 2);
        x = ((-x * ((m * x) >> 15)) >> 15) + x;
        result = (int16_t)((int16_t)(x * 2) * sign);
    }

    *out_mantissa = result;
    *out_exponent = (int16_t)(1 - e16);
}

int16_t snes_trig_sine_0012c2dc(const SnesTrigTables *t, int16_t angle)
{
    uint16_t u;
    int32_t value;

    if ((uint16_t)angle == 0x8000u)
        return 0;

    if (angle < 0)
        return (int16_t)-snes_trig_sine_0012c2dc(t, (int16_t)-angle);

    u = (uint16_t)angle;
    value = (int32_t)t->coarse[u >> 8] +
            (((int32_t)t->fine[u & 0xffu] *
              (int32_t)t->coarse[(uint16_t)(u + 0x4000u) >> 8]) >> 15);

    if (value > 0x7fff)
        value = 0x7fff;
    if (value < -0x8000)
        value = -0x7fff;
    return (int16_t)value;
}

int16_t snes_trig_cosine_0012c398(const SnesTrigTables *t, int16_t angle)
{
    uint16_t u;
    int32_t value;

    if ((uint16_t)angle == 0x8000u)
        return INT16_MIN;

    if (angle < 0)
        angle = (int16_t)-angle;

    u = (uint16_t)angle;
    value = (int32_t)t->coarse[(uint16_t)(u + 0x4000u) >> 8] -
            (((int32_t)t->fine[u & 0xffu] *
              -(int32_t)t->coarse[(uint16_t)(u + 0x8000u) >> 8]) >> 15);

    if (value > 0x7fff)
        value = 0x7fff;
    if (value < -0x8000)
        value = -0x7fff;
    return (int16_t)value;
}