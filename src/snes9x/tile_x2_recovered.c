/* SNES Station v0.23 WIP — horizontal x2 renderer pieces recovered. */
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

static void write_x2_one(unsigned d, uint8_t pixel,
                         uint8_t *screen, uint8_t *depth,
                         const uint8_t *colors, uint8_t z1, uint8_t z2)
{
    if (depth[d] < z1 && pixel != 0) {
        uint8_t c = colors[(unsigned)pixel << 1];
        screen[d] = c;
        screen[d + 1] = c;
        depth[d] = z2;
        depth[d + 1] = z2;
    }
}

/* 0x001acf4c */
void WRITE_4PIXELSx2_recovered(uint32_t Offset, const uint8_t *Pixels)
{
    uint8_t *screen = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x3c) + Offset;
    uint8_t *depth  = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x40) + Offset;
    const uint8_t *colors = (const uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x44);
    uint8_t z1 = U8(g_gfx_blob, 0x4c), z2 = U8(g_gfx_blob, 0x4d);
    write_x2_one(0, Pixels[0], screen, depth, colors, z1, z2);
    write_x2_one(2, Pixels[1], screen, depth, colors, z1, z2);
    write_x2_one(4, Pixels[2], screen, depth, colors, z1, z2);
    write_x2_one(6, Pixels[3], screen, depth, colors, z1, z2);
}

/* 0x001ad090 */
void WRITE_4PIXELS_FLIPPEDx2_recovered(uint32_t Offset, const uint8_t *Pixels)
{
    uint8_t *screen = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x3c) + Offset;
    uint8_t *depth  = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x40) + Offset;
    const uint8_t *colors = (const uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x44);
    uint8_t z1 = U8(g_gfx_blob, 0x4c), z2 = U8(g_gfx_blob, 0x4d);
    write_x2_one(0, Pixels[3], screen, depth, colors, z1, z2);
    write_x2_one(2, Pixels[2], screen, depth, colors, z1, z2);
    write_x2_one(4, Pixels[1], screen, depth, colors, z1, z2);
    write_x2_one(6, Pixels[0], screen, depth, colors, z1, z2);
}

/* 0x00184a40 */
void DrawTilex2_recovered(uint32_t Tile, uint32_t Offset,
                          uint32_t StartLine, uint32_t LineCount)
{
    uint32_t shift = U32(g_bg_blob, 0x08);
    uint32_t addr = U32(g_bg_blob, 0x0c) + ((Tile & 0x3ffu) << shift);
    uint32_t number, ppl;
    uint8_t *cache, *valid;

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

    ppl = U32(g_gfx_blob, 0x30);
    if ((Tile & (V_FLIP | H_FLIP)) == 0) {
        const uint8_t *bp = cache + StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 0)) WRITE_4PIXELSx2_recovered(Offset, bp + 0);
            if (*(const uint32_t *)(bp + 4)) WRITE_4PIXELSx2_recovered(Offset + 8, bp + 4);
            bp += 8; Offset += ppl;
        }
    } else if ((Tile & V_FLIP) == 0) {
        const uint8_t *bp = cache + StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 4)) WRITE_4PIXELS_FLIPPEDx2_recovered(Offset, bp + 4);
            if (*(const uint32_t *)(bp + 0)) WRITE_4PIXELS_FLIPPEDx2_recovered(Offset + 8, bp + 0);
            bp += 8; Offset += ppl;
        }
    } else if (Tile & H_FLIP) {
        const uint8_t *bp = cache + 56 - StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 4)) WRITE_4PIXELS_FLIPPEDx2_recovered(Offset, bp + 4);
            if (*(const uint32_t *)(bp + 0)) WRITE_4PIXELS_FLIPPEDx2_recovered(Offset + 8, bp + 0);
            bp -= 8; Offset += ppl;
        }
    } else {
        const uint8_t *bp = cache + 56 - StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 0)) WRITE_4PIXELSx2_recovered(Offset, bp + 0);
            if (*(const uint32_t *)(bp + 4)) WRITE_4PIXELSx2_recovered(Offset + 8, bp + 4);
            bp -= 8; Offset += ppl;
        }
    }
}
