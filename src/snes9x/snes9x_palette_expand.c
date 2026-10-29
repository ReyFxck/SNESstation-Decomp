/*
 * SNES Station v0.23 - SNES9X Palette Expand
 * Canonical source from progress13_core_helpers.c (0x001591a8 range)
 */

#include <stddef.h>
#include <stdint.h>

typedef struct {
    const uint8_t *bank_base;
    const uint8_t *selected_bank;
    uint32_t component0[256];
    uint32_t component1[256];
    uint32_t component2[256];
    uint16_t packed[256];
} Snes9xPaletteExpand;

/* 0x001591a8 -- expand 256 packed 15-bit palette entries through a 32-entry
 * component lookup. The target also selects a 32-byte bank before the loop. */
void snes9x_palette_expand_001591a8(Snes9xPaletteExpand *out,
                                     uint8_t bank, int enabled,
                                     const uint16_t source[256],
                                     const uint8_t component_lut[32])
{
    unsigned i;

    out->selected_bank = out->bank_base + ((unsigned)bank << 5);
    if (!enabled)
        return;

    for (i = 0; i < 256; ++i) {
        uint16_t raw = source[i];
        uint32_t c0 = component_lut[raw & 31u];
        uint32_t c1 = component_lut[(raw >> 5) & 31u];
        uint32_t c2 = component_lut[(raw >> 10) & 31u];
        out->component0[i] = c0;
        out->component1[i] = c1;
        out->component2[i] = c2;
        out->packed[i] = (uint16_t)(c0 | (c1 << 5) | (c2 << 10));
    }
}