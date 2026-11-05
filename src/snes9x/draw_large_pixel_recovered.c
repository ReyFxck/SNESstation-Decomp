/*
 * SNES Station v0.23 WIP — recovered from R5900 code.
 * Original function: 0x001859a8 (DrawLargePixel)
 */
#include <stdint.h>

#define PTR32(base, off) (*(uint32_t *)((uint8_t *)(base) + (off)))
#define U32(base, off)   (*(uint32_t *)((uint8_t *)(base) + (off)))
#define U8(base, off)    (*(uint8_t  *)((uint8_t *)(base) + (off)))
#define V_FLIP 0x8000u
#define H_FLIP 0x4000u
#define BLANK_TILE 2u

extern uint8_t g_bg_blob[];
extern uint8_t g_gfx_blob[];
extern uint16_t g_screen_colors[];
extern uint16_t g_direct_colour_maps[][256];
extern uint8_t g_direct_colour_maps_need_rebuild;
extern uint8_t ConvertTile_recovered(uint8_t *, uint32_t);
extern void S9xBuildDirectColourMaps_0014308c(void);

void DrawLargePixel_recovered(uint32_t Tile, uint32_t Offset,
                              uint32_t StartPixel, uint32_t Pixels,
                              uint32_t StartLine, uint32_t LineCount)
{
    uint32_t shift = U32(g_bg_blob, 0x08);
    uint32_t addr = U32(g_bg_blob, 0x0c) + ((Tile & 0x3ffu) << shift);
    uint32_t number, ppl;
    uint8_t *cache, *valid;
    uint8_t *screen, *depth;
    uint8_t pixel;

    if ((Tile & 0x1ffu) >= 0x100u) addr += U32(g_bg_blob, 0x10);
    addr &= 0xffffu;
    number = addr >> shift;
    cache = (uint8_t *)(uintptr_t)PTR32(g_bg_blob, 0x24) + (number << 6);
    valid = (uint8_t *)(uintptr_t)PTR32(g_bg_blob, 0x28) + number;
    if (!*valid) *valid = ConvertTile_recovered(cache, addr);
    if (*valid == BLANK_TILE) return;

    if (U8(g_bg_blob, 0x2c)) {
        uint32_t map;
        if (g_direct_colour_maps_need_rebuild) S9xBuildDirectColourMaps_0014308c();
        map = (Tile >> 10) & U32(g_bg_blob, 0x20);
        PTR32(g_gfx_blob, 0x44) = (uint32_t)(uintptr_t)&g_direct_colour_maps[map][0];
    } else {
        uint32_t pal = (Tile >> 10) & U32(g_bg_blob, 0x20);
        pal = (pal << U32(g_bg_blob, 0x1c)) + U32(g_bg_blob, 0x18);
        PTR32(g_gfx_blob, 0x44) = (uint32_t)(uintptr_t)&g_screen_colors[pal];
    }

    if (Tile & H_FLIP) StartPixel = 7 - StartPixel;
    if (Tile & V_FLIP)
        pixel = cache[56 - StartLine + StartPixel];
    else
        pixel = cache[StartLine + StartPixel];
    if (!pixel) return;

    /* 8-bit render path takes the low byte of the 16-bit palette entry. */
    pixel = ((const uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x44))[(unsigned)pixel << 1];
    screen = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x3c) + Offset;
    depth  = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x40) + Offset;
    ppl = U32(g_gfx_blob, 0x30);

    while (LineCount--) {
        int z;
        for (z = (int)Pixels - 1; z >= 0; --z) {
            if (depth[z] < U8(g_gfx_blob, 0x4c)) {
                screen[z] = pixel;
                depth[z] = U8(g_gfx_blob, 0x4d);
            }
        }
        screen += ppl;
        depth += ppl;
    }
}
