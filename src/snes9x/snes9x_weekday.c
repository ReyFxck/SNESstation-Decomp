/*
 * SNES Station v0.23 - SNES9X Weekday Helper
 * Canonical source from progress13_runtime_more.c (0x001836d0 range)
 */

#include <stdint.h>

/* 0x001836d0 -- weekday helper over the target's decimal digit fields. */
int snes9x_weekday_001836d0(uint8_t digit8, uint8_t digit9,
                             uint8_t month, uint8_t year_hundreds,
                             uint8_t year_tens, uint8_t year_ones)
{
    static const int offset_by_month[13] = {0,1,4,4,0,2,5,0,3,6,1,4,6};
    uint32_t year = (uint32_t)year_hundreds * 100u +
                    (uint32_t)year_tens * 10u + (uint32_t)year_ones;
    uint32_t day = (uint32_t)digit9 * 10u + (uint32_t)digit8;
    uint32_t m = month < 13u ? month : 1u;
    uint32_t y = year - 900u;
    uint32_t value = y + (y >> 2) + (uint32_t)offset_by_month[m] + day;

    value -= 1u;
    if ((y & 3u) == 0 && m < 3u)
        value -= 1u;
    return (int)(value % 7u);
}