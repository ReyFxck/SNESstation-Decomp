/*
 * SNES Station v0.23 WIP — recovered lookup-table block.
 * Original block begins inside the graphics initializer at 0x00142a78.
 *
 * This helper represents only the self-contained table-generation loop.
 * The containing function continues into other graphics-state setup and is
 * intentionally not named yet.
 */
#include <stdint.h>

extern uint32_t g_odd_high[4][16];
extern uint32_t g_odd_low[4][16];
extern uint32_t g_even_high[4][16];
extern uint32_t g_even_low[4][16];

void InitTileLookupTables_recovered(void)
{
    uint32_t odd_bit = 1;
    uint32_t even_bit = 2;
    unsigned plane;

    for (plane = 0; plane < 4; ++plane) {
        unsigned i;
        for (i = 0; i < 16; ++i) {
            uint32_t odd = 0;
            uint32_t even = 0;

            if (i & 8) { odd |= odd_bit;  even |= even_bit; }
            if (i & 4) { odd |= odd_bit  << 8; even |= even_bit << 8; }
            if (i & 2) { odd |= odd_bit  << 16; even |= even_bit << 16; }
            if (i & 1) { odd |= odd_bit  << 24; even |= even_bit << 24; }

            /* The binary stores identical packing tables for high/low nibble. */
            g_odd_high[plane][i] = odd;
            g_odd_low[plane][i] = odd;
            g_even_high[plane][i] = even;
            g_even_low[plane][i] = even;
        }
        odd_bit <<= 2;
        even_bit <<= 2;
    }
}
