/*
 * SNES Station v0.23 - SNES9X Signed-Angle 2-D Transform
 * Canonical source from progress13_core_helpers.c (0x0016fd08 range)
 */

#include <stdint.h>
#include <math.h>

/* External intrinsic from ee_intrinsics_recovered.h */
extern int lrintf(float value);

/* 0x0016fd08 -- signed-angle 2-D transform used by frontend state. */
void snes9x_rotate_pair_0016fd08(int16_t angle, int16_t y, int16_t x,
                                  int16_t *out_sum, int16_t *out_diff)
{
    const float radians = (float)angle * 9.58738019107841e-05f;
    const float s = sinf(radians);
    const float c = cosf(radians);
    const float sum = 2.0f * (s * (float)x + c * (float)y);
    const float diff = 2.0f * (c * (float)x - s * (float)y);

    *out_sum = (int16_t)lrintf(sum);
    *out_diff = (int16_t)lrintf(diff);
}