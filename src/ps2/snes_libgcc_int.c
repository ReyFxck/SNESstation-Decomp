/*
 * SNES Station v0.23 PS2 - libgcc 64-bit Integer Division Helpers
 * Canonical source from progress21_small_helpers.c @ 0x001a7638
 */

#include <stdint.h>

/* Unsigned 64-bit div/mod core with remainder output. */
extern uint64_t snes_udivmoddi4_001a7788(uint64_t numerator,
                                         uint64_t denominator,
                                         uint64_t *remainder);

static uint64_t snes_i64_magnitude(int64_t value)
{
    uint64_t bits = (uint64_t)value;
    return value < 0 ? (~bits + UINT64_C(1)) : bits;
}

/* 0x001a7638 -- Signed 64-bit remainder wrapper (__moddi3). */
int64_t snes_moddi3_001a7638(int64_t numerator, int64_t denominator)
{
    uint64_t remainder = 0;

    (void)snes_udivmoddi4_001a7788(snes_i64_magnitude(numerator),
                                   snes_i64_magnitude(denominator),
                                   &remainder);
    return numerator < 0 ? -(int64_t)remainder : (int64_t)remainder;
}