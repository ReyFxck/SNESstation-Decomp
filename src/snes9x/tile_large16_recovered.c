/*
 * SNES Station v0.23 WIP — 16-bit large/mosaic pixel renderers.
 *
 * Original entry points:
 *   0x001874a8 DrawLargePixel16
 *   0x0018a6d4 DrawLargePixel16Add
 *   0x0018adb8 DrawLargePixel16Add1_2
 *   0x0018b43c DrawLargePixel16Sub
 *   0x0018bac0 DrawLargePixel16Sub1_2
 */
#include <stdint.h>

#define PTR32(base, off) (*(uint32_t *)((uint8_t *)(base) + (off)))
#define U32(base, off)   (*(uint32_t *)((uint8_t *)(base) + (off)))
#define U16(base, off)   (*(uint16_t *)((uint8_t *)(base) + (off)))
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
extern uint16_t COLOR_ADD_recovered(uint16_t, uint16_t);
extern uint16_t COLOR_ADD1_2_recovered(uint16_t, uint16_t);
extern uint16_t COLOR_SUB_recovered(uint16_t, uint16_t);
extern uint16_t COLOR_SUB1_2_recovered(uint16_t, uint16_t);

typedef enum { LARGE_NORMAL, LARGE_ADD, LARGE_ADD_HALF, LARGE_SUB, LARGE_SUB_HALF } large_mode_t;

static uint8_t *prepare_large_tile(uint32_t Tile)
{
    uint32_t shift = U32(g_bg_blob, 0x08);
    uint32_t addr = U32(g_bg_blob, 0x0c) + ((Tile & 0x3ffu) << shift);
    uint32_t number;
    uint8_t *cache, *valid;
    if ((Tile & 0x1ffu) >= 0x100u) addr += U32(g_bg_blob, 0x10);
    addr &= 0xffffu;
    number = addr >> shift;
    cache = (uint8_t *)(uintptr_t)PTR32(g_bg_blob, 0x24) + (number << 6);
    valid = (uint8_t *)(uintptr_t)PTR32(g_bg_blob, 0x28) + number;
    if (!*valid) *valid = ConvertTile_recovered(cache, addr);
    if (*valid == BLANK_TILE) return 0;

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
    return cache;
}

static uint16_t large_math(large_mode_t mode, uint16_t main,
                           uint8_t sub_depth, uint16_t sub, uint16_t fixed)
{
    if (mode == LARGE_NORMAL || sub_depth == 0) return main;
    switch (mode) {
    case LARGE_ADD:
        return sub_depth == 1 ? COLOR_ADD_recovered(main, fixed) : COLOR_ADD_recovered(main, sub);
    case LARGE_ADD_HALF:
        /* Target/source-era behavior: fixed-colour case uses full ADD. */
        return sub_depth == 1 ? COLOR_ADD_recovered(main, fixed) : COLOR_ADD1_2_recovered(main, sub);
    case LARGE_SUB:
        return sub_depth == 1 ? COLOR_SUB_recovered(main, fixed) : COLOR_SUB_recovered(main, sub);
    case LARGE_SUB_HALF:
        /* Target/source-era behavior: fixed-colour case uses full SUB. */
        return sub_depth == 1 ? COLOR_SUB_recovered(main, fixed) : COLOR_SUB1_2_recovered(main, sub);
    case LARGE_NORMAL:
        break;
    }
    return main;
}

static void draw_large16(uint32_t Tile, uint32_t Offset,
                         uint32_t StartPixel, uint32_t Pixels,
                         uint32_t StartLine, uint32_t LineCount,
                         large_mode_t mode)
{
    uint8_t *cache = prepare_large_tile(Tile);
    const uint16_t *colors;
    uint16_t *screen;
    uint8_t *depth;
    uint8_t pixel_index;
    uint16_t pixel, fixed = U16(g_gfx_blob, 0x50);
    uint32_t ppl = U32(g_gfx_blob, 0x30);
    uint32_t delta = U32(g_gfx_blob, 0x14);
    uint32_t depth_delta = U32(g_gfx_blob, 0x48);
    uint8_t z1 = U8(g_gfx_blob, 0x4c), z2 = U8(g_gfx_blob, 0x4d);

    if (!cache) return;
    if (Tile & H_FLIP) StartPixel = 7 - StartPixel;
    pixel_index = (Tile & V_FLIP) ? cache[56 - StartLine + StartPixel]
                                  : cache[StartLine + StartPixel];
    if (!pixel_index) return;

    colors = (const uint16_t *)(uintptr_t)PTR32(g_gfx_blob, 0x44);
    pixel = colors[pixel_index];
    screen = (uint16_t *)(uintptr_t)PTR32(g_gfx_blob, 0x3c) + Offset;
    depth = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, mode == LARGE_NORMAL ? 0x40 : 0x08) + Offset;

    while (LineCount--) {
        int z;
        for (z = (int)Pixels - 1; z >= 0; --z) {
            if (depth[z] < z1) {
                uint16_t out = pixel;
                if (mode != LARGE_NORMAL) {
                    uint8_t sd = depth[z + depth_delta];
                    uint16_t sub = screen[delta + (uint32_t)z];
                    out = large_math(mode, pixel, sd, sub, fixed);
                }
                screen[z] = out;
                depth[z] = z2;
            }
        }
        screen += ppl;
        depth += ppl;
    }
}

void DrawLargePixel16_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartPixel, uint32_t Pixels, uint32_t StartLine, uint32_t LineCount)
{ draw_large16(Tile, Offset, StartPixel, Pixels, StartLine, LineCount, LARGE_NORMAL); }
void DrawLargePixel16Add_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartPixel, uint32_t Pixels, uint32_t StartLine, uint32_t LineCount)
{ draw_large16(Tile, Offset, StartPixel, Pixels, StartLine, LineCount, LARGE_ADD); }
void DrawLargePixel16Add1_2_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartPixel, uint32_t Pixels, uint32_t StartLine, uint32_t LineCount)
{ draw_large16(Tile, Offset, StartPixel, Pixels, StartLine, LineCount, LARGE_ADD_HALF); }
void DrawLargePixel16Sub_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartPixel, uint32_t Pixels, uint32_t StartLine, uint32_t LineCount)
{ draw_large16(Tile, Offset, StartPixel, Pixels, StartLine, LineCount, LARGE_SUB); }
void DrawLargePixel16Sub1_2_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartPixel, uint32_t Pixels, uint32_t StartLine, uint32_t LineCount)
{ draw_large16(Tile, Offset, StartPixel, Pixels, StartLine, LineCount, LARGE_SUB_HALF); }
