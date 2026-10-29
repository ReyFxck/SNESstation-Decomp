/*
 * SNES Station v0.23 SNES9X - State Machine & Date Helpers
 * Canonical source from progress13_runtime_more.c @ 0x0015cdf8, 0x001836d0
 */

#include <stddef.h>
#include <stdint.h>

/* 0x0015cdf8 -- state machine; target intentionally falls through stages. */
typedef struct {
    uint32_t state;
    uint8_t flag_553f;
    uint8_t flag_553e;
    uint8_t flag_5611;
    uint8_t flag_553d;
} Snes9xStageState;

void snes9x_stage_advance_0015cdf8(Snes9xStageState *s)
{
    switch (s->state) {
    default:
    case 0:
        s->state = 1;
        return;
    case 1:
        if (s->flag_553f != 0) { s->state = 2; return; }
        /* fall through */
    case 2:
        if (s->flag_553f != 0) { s->state = 3; return; }
        /* fall through */
    case 3:
        if (s->flag_553e != 0) { s->state = 4; return; }
        /* fall through */
    case 4:
        if (s->flag_5611 != 0) { s->state = 5; return; }
        /* fall through */
    case 5:
        if (s->flag_5611 != 0) { s->state = 6; return; }
        /* fall through */
    case 6:
        if (s->flag_553d == 0) s->state = 1;
        else s->state = 0;
        return;
    }
}

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