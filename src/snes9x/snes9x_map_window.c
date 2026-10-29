/*
 * SNES Station v0.23 - SNES9X Map Window Initialization
 * Canonical source from progress13_core_helpers.c (0x0015e198 range)
 */

#include <stddef.h>
#include <stdint.h>

/* 0x0015e198 -- fill the two mirrored pointer-map tables. The target uses
 * 32-bit EE addresses; uint32_t keeps the same arithmetic and table width. */
void snes9x_map_window_0015e198(unsigned region, uint8_t selector,
                                 uint32_t ram_base,
                                 uint32_t *ppu_map,
                                 uint32_t *memory_map)
{
    unsigned outer;
    unsigned selector7 = selector & 7u;
    unsigned region_index = region << 9;

    if (region >= 2u)
        region_index += 0x400u;

    for (outer = 0; outer < 256; outer += 16) {
        unsigned j;
        uint32_t base = ram_base + (selector7 << 20) + (outer << 12);
        for (j = 0; j < 16; ++j) {
            ppu_map[outer + j] = base;
            memory_map[outer + j] = base;
        }
    }

    for (outer = 0; outer < 512; outer += 16) {
        unsigned j;
        uint32_t base = ram_base + (selector7 << 20) + (outer << 11) - 0x8000u;
        for (j = 8; j < 16; ++j) {
            unsigned index = outer + j + region_index;
            ppu_map[index] = base;
            memory_map[index] = base;
        }
    }
}