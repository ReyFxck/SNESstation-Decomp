/*
 * SNES Station v0.23 - SNES9X Packed-Channel Color Blend
 * Canonical source from progress13_runtime_more.c (0x0010a18c range)
 */

#include <stdint.h>

/* 0x0010a18c -- packed-channel weighted blend using the target masks. */
uint32_t snes9x_color_blend_0010a18c(uint32_t a, uint32_t b,
                                      uint32_t control,
                                      uint32_t low_mask,
                                      uint32_t high_mask)
{
    unsigned weight_b;
    uint64_t packed_a, packed_b, sum;
    if (a == b)
        return b;

    weight_b = (control >> 11) & 31u;
    packed_a = (uint64_t)(a & low_mask) |
               ((uint64_t)(a & high_mask) << 16);
    packed_b = (uint64_t)(b & low_mask) |
               ((uint64_t)(b & high_mask) << 16);
    sum = (uint64_t)(32u - weight_b) * packed_a +
          (uint64_t)weight_b * packed_b;
    return (uint32_t)(((sum >> 5) & low_mask) |
                      ((sum >> 21) & high_mask));
}