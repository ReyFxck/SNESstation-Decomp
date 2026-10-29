/*
 * SNES Station v0.23 - SNES9X Palette Expansion
 * Canonical source from progress11_core.c (0x0012e2e0 range)
 */

#include <stddef.h>
#include <stdint.h>

/* 0x0012e2e0: blend with transparent nibble */
void snes9x_palette_blend_0012e2e0(uint8_t *base, uint32_t count,
                                    uint8_t transparent_nibble)
{
    uint32_t i;
    uint8_t key = transparent_nibble & 0x0fu;

    for (i = 0u; i < count; ++i) {
        uint8_t incoming = base[count + i];
        uint8_t old = base[i];
        uint8_t high = (uint8_t)(old & 0xf0u);
        uint8_t low = (uint8_t)(incoming & 0x0fu);

        if ((incoming >> 4) != key)
            high = (uint8_t)(incoming & 0xf0u);
        if ((incoming & 0x0fu) == key)
            low = (uint8_t)(old & 0x0fu);
        base[0x200u + i] = (uint8_t)(high | low);
    }
}