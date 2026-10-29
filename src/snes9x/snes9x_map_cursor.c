/*
 * SNES Station v0.23 - SNES9X Map Cursor Selection
 * Canonical source from progress13_runtime_more.c (0x0015e07c range)
 */

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t current_ptr; /* target +0x20 */
    uint32_t current_base;/* target +0x24 */
    uint32_t alternate28; /* target +0x28 */
} Snes9xMapCursor;

/* 0x0015e07c -- Snes9x map-entry -> current base/pointer selector. */
void snes9x_set_map_cursor_0015e07c(
    uint32_t address, const uint32_t *map_entries,
    uint32_t main_ram, uint32_t secondary_ram, uint32_t default_ram,
    Snes9xMapCursor *cursor)
{
    uint32_t slot = map_entries[(address >> 12) & 0xfffu];
    uint32_t base;
    uint32_t low = address & 0xffffu;

    if (slot >= 12u) {
        base = slot;
    } else {
        switch (slot) {
        case 0: base = main_ram - 0x2000u; break;
        case 1: base = main_ram - 0x4000u; break;
        case 2: base = main_ram - 0x6000u; break;
        case 3: case 4: case 11:
            base = secondary_ram; break;
        case 8:
            base = cursor->alternate28 - 0x6000u; break;
        default:
            base = default_ram; break;
        }
    }
    cursor->current_base = base;
    cursor->current_ptr = base + low;
}