/*
 * SNES Station v0.23 SNES9X - Palette Expansion
 * Canonical source from progress13_core_helpers_recovered.c @ 0x001591a8
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* 0x001591a8 -- expand 256 packed 15-bit palette entries through a 32-entry
 * component lookup. The target also selects a 32-byte bank before the loop. */
typedef struct {
    const uint8_t *bank_base;
    const uint8_t *selected_bank;
    uint32_t component0[256];
    uint32_t component1[256];
    uint32_t component2[256];
    uint16_t packed[256];
} Snes9xPaletteExpand;

void snes9x_palette_expand(Snes9xPaletteExpand *p)
{
    /* Select 32-byte bank */
    p->selected_bank = p->bank_base + 32; /* placeholder - actual logic from target */

    /* Expand 256 packed 15-bit entries */
    for (uint32_t i = 0; i < 256; ++i) {
        uint16_t packed = p->packed[i];
        p->component0[i] = p->selected_bank[(packed >> 0) & 0x1f];
        p->component1[i] = p->selected_bank[(packed >> 5) & 0x1f];
        p->component2[i] = p->selected_bank[(packed >> 10) & 0x1f];
    }
}