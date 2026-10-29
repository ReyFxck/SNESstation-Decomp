/*
 * SNES Station v0.23 - SNES9X State Machine Stage Advance
 * Canonical source from progress13_runtime_more.c (0x0015cdf8 range)
 */

#include <stdint.h>

typedef struct {
    uint32_t state;
    uint8_t flag_553f;
    uint8_t flag_553e;
    uint8_t flag_5611;
    uint8_t flag_553d;
} Snes9xStageState;

/* 0x0015cdf8 -- state machine; target intentionally falls through stages. */
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