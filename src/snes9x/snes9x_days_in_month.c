/*
 * SNES Station v0.23 - SNES9X Month Length Helpers
 * Canonical source from progress13_runtime_more.c (0x001825e4, 0x00183778 range)
 */

#include <stdint.h>

static int snes9x_days_in_month_impl(unsigned month, unsigned year)
{
    unsigned index = month - 2u;
    if (index >= 10u)
        return 31;
    if (index == 0u)
        return (year & 3u) != 0 ? 28 : 29;
    switch (index) {
    case 1: case 3: case 5: case 6: case 8:
        return 31;
    default:
        return 30;
    }
}

int snes9x_days_in_month_001825e4(unsigned month, unsigned year)
{
    return snes9x_days_in_month_impl(month, year);
}

int snes9x_days_in_month_00183778(unsigned month, unsigned year)
{
    return snes9x_days_in_month_impl(month, year);
}