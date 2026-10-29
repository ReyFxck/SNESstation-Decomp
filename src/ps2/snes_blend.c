/*
 * SNES Station v0.23 PS2 - Four-Way Packed Blend
 * Canonical source from progress24_core_helpers.c @ 0x0010a264
 */

#include <stdint.h>

/* Existing recovered libgcc helper used by the target at 0x0010a264. */
extern uint64_t snes___muldi3(uint64_t a, uint64_t b);

/*
 * 0x0010a264 -- Four-way packed blend.
 * The target derives two 5-bit weights from the top inputs, builds four packed
 * lane values with the two global masks, multiplies them through __muldi3 and
 * folds the accumulated 64-bit result back into the original lane positions.
 */
uint32_t snes_blend_four_way_0010a264(uint32_t mask_lo, uint32_t mask_hi,
                                       uint32_t value1, uint32_t value2,
                                       uint32_t value3, uint32_t value4,
                                       uint32_t weight_src1, uint32_t weight_src2)
{
    uint32_t w1 = (weight_src1 >> 11) & 0x1fu;
    uint32_t w2 = (weight_src2 >> 11) & 0x1fu;
    int64_t cross = (int64_t)((w1 * w2) >> 5);
    int64_t c3 = (int64_t)w2 - cross;
    int64_t c4 = cross;
    int64_t c1 = (cross - (int64_t)w1 - (int64_t)w2) + 0x20;
    int64_t c2 = (int64_t)w1 - cross;
    uint32_t p1 = (value1 & mask_lo) | ((value1 & mask_hi) << 16);
    uint32_t p2 = (value2 & mask_lo) | ((value2 & mask_hi) << 16);
    uint32_t p3 = (value3 & mask_lo) | ((value3 & mask_hi) << 16);
    uint32_t p4 = (value4 & mask_lo) | ((value4 & mask_hi) << 16);
    uint64_t sum;

    sum  = snes___muldi3((uint64_t)c3, (uint64_t)p3);
    sum += snes___muldi3((uint64_t)c4, (uint64_t)p4);
    sum += snes___muldi3((uint64_t)c1, (uint64_t)p1);
    sum += snes___muldi3((uint64_t)c2, (uint64_t)p2);

    return ((uint32_t)(sum >> 5) & mask_lo) |
           ((uint32_t)(sum >> 21) & mask_hi);
}