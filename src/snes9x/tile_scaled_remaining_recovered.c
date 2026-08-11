/*
 * SNES Station v0.23 WIP — remaining 8-bit scaled tile paths.
 *
 * Original draw entry points:
 *   0x00184d5c DrawClippedTilex2
 *   0x001851f4 DrawTilex2x2
 *   0x00185510 DrawClippedTilex2x2
 * Original x2x2 writers:
 *   0x001ad1d4 WRITE_4PIXELSx2x2
 *   0x001ad398 WRITE_4PIXELS_FLIPPEDx2x2
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
extern const uint32_t g_head_mask[4];
extern const uint32_t g_tail_mask[5];
extern uint8_t ConvertTile_recovered(uint8_t *, uint32_t);
extern void S9xBuildDirectColourMaps_0014308c(void);
extern void WRITE_4PIXELSx2_recovered(uint32_t, const uint8_t *);
extern void WRITE_4PIXELS_FLIPPEDx2_recovered(uint32_t, const uint8_t *);

typedef void (*writer8_t)(uint32_t, const uint8_t *);

static uint32_t swap32_local(uint32_t x)
{
    return ((x & 0xffu) << 24) | ((x & 0xff00u) << 8) |
           ((x & 0xff0000u) >> 8) | ((x & 0xff000000u) >> 24);
}

static uint8_t *prepare_scaled(uint32_t Tile)
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

static void write_x2x2_one(unsigned d, uint8_t pixel, uint8_t *screen,
                           uint8_t *depth, const uint8_t *colors,
                           uint8_t z1, uint8_t z2, uint32_t next_row)
{
    if (depth[d] < z1 && pixel != 0) {
        uint8_t c = colors[(unsigned)pixel << 1];
        screen[d] = c; screen[d + 1] = c;
        screen[next_row + d] = c; screen[next_row + d + 1] = c;
        depth[d] = z2; depth[d + 1] = z2;
        depth[next_row + d] = z2; depth[next_row + d + 1] = z2;
    }
}

void WRITE_4PIXELSx2x2_recovered(uint32_t Offset, const uint8_t *Pixels)
{
    uint8_t *screen = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x3c) + Offset;
    uint8_t *depth = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x40) + Offset;
    const uint8_t *colors = (const uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x44);
    uint32_t next_row = U32(g_gfx_blob, 0x24);
    uint8_t z1 = U8(g_gfx_blob, 0x4c), z2 = U8(g_gfx_blob, 0x4d);
    unsigned i;
    for (i = 0; i < 4; ++i)
        write_x2x2_one(i * 2, Pixels[i], screen, depth, colors, z1, z2, next_row);
}

void WRITE_4PIXELS_FLIPPEDx2x2_recovered(uint32_t Offset, const uint8_t *Pixels)
{
    uint8_t *screen = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x3c) + Offset;
    uint8_t *depth = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x40) + Offset;
    const uint8_t *colors = (const uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x44);
    uint32_t next_row = U32(g_gfx_blob, 0x24);
    uint8_t z1 = U8(g_gfx_blob, 0x4c), z2 = U8(g_gfx_blob, 0x4d);
    unsigned i;
    for (i = 0; i < 4; ++i)
        write_x2x2_one(i * 2, Pixels[3 - i], screen, depth, colors, z1, z2, next_row);
}

static void render8(uint32_t Tile, uint32_t Offset, uint32_t StartLine,
                    uint32_t LineCount, writer8_t normal, writer8_t flipped)
{
    uint8_t *cache = prepare_scaled(Tile);
    uint32_t ppl = U32(g_gfx_blob, 0x30);
    const uint8_t *bp;
    if (!cache) return;
    if ((Tile & (V_FLIP | H_FLIP)) == 0) {
        bp = cache + StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 0)) normal(Offset, bp + 0);
            if (*(const uint32_t *)(bp + 4)) normal(Offset + 8, bp + 4);
            bp += 8; Offset += ppl;
        }
    } else if ((Tile & V_FLIP) == 0) {
        bp = cache + StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 4)) flipped(Offset, bp + 4);
            if (*(const uint32_t *)(bp + 0)) flipped(Offset + 8, bp + 0);
            bp += 8; Offset += ppl;
        }
    } else if (Tile & H_FLIP) {
        bp = cache + 56 - StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 4)) flipped(Offset, bp + 4);
            if (*(const uint32_t *)(bp + 0)) flipped(Offset + 8, bp + 0);
            bp -= 8; Offset += ppl;
        }
    } else {
        bp = cache + 56 - StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 0)) normal(Offset, bp + 0);
            if (*(const uint32_t *)(bp + 4)) normal(Offset + 8, bp + 4);
            bp -= 8; Offset += ppl;
        }
    }
}

static void render8_clipped(uint32_t Tile, uint32_t Offset,
                            uint32_t StartPixel, uint32_t Width,
                            uint32_t StartLine, uint32_t LineCount,
                            writer8_t normal, writer8_t flipped)
{
    uint8_t *cache = prepare_scaled(Tile);
    uint32_t d1, d2, ppl = U32(g_gfx_blob, 0x30);
    const uint8_t *bp;
    if (!cache) return;
    if (StartPixel < 4) {
        d1 = g_head_mask[StartPixel];
        if (StartPixel + Width < 4) d1 &= g_tail_mask[StartPixel + Width];
    } else d1 = 0;
    if (StartPixel + Width > 4) {
        d2 = StartPixel > 4 ? g_head_mask[StartPixel - 4] : 0xffffffffu;
        d2 &= g_tail_mask[StartPixel + Width - 4];
    } else d2 = 0;

    if ((Tile & (V_FLIP | H_FLIP)) == 0) {
        bp = cache + StartLine;
        while (LineCount--) {
            uint32_t dd = *(const uint32_t *)(bp + 0) & d1;
            if (dd) normal(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 4) & d2;
            if (dd) normal(Offset + 8, (const uint8_t *)&dd);
            bp += 8; Offset += ppl;
        }
    } else if ((Tile & V_FLIP) == 0) {
        bp = cache + StartLine; d1 = swap32_local(d1); d2 = swap32_local(d2);
        while (LineCount--) {
            uint32_t dd = *(const uint32_t *)(bp + 4) & d1;
            if (dd) flipped(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 0) & d2;
            if (dd) flipped(Offset + 8, (const uint8_t *)&dd);
            bp += 8; Offset += ppl;
        }
    } else if (Tile & H_FLIP) {
        bp = cache + 56 - StartLine; d1 = swap32_local(d1); d2 = swap32_local(d2);
        while (LineCount--) {
            uint32_t dd = *(const uint32_t *)(bp + 4) & d1;
            if (dd) flipped(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 0) & d2;
            if (dd) flipped(Offset + 8, (const uint8_t *)&dd);
            bp -= 8; Offset += ppl;
        }
    } else {
        bp = cache + 56 - StartLine;
        while (LineCount--) {
            uint32_t dd = *(const uint32_t *)(bp + 0) & d1;
            if (dd) normal(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 4) & d2;
            if (dd) normal(Offset + 8, (const uint8_t *)&dd);
            bp -= 8; Offset += ppl;
        }
    }
}

void DrawClippedTilex2_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartPixel, uint32_t Width, uint32_t StartLine, uint32_t LineCount)
{ render8_clipped(Tile, Offset, StartPixel, Width, StartLine, LineCount, WRITE_4PIXELSx2_recovered, WRITE_4PIXELS_FLIPPEDx2_recovered); }

void DrawTilex2x2_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartLine, uint32_t LineCount)
{ render8(Tile, Offset, StartLine, LineCount, WRITE_4PIXELSx2x2_recovered, WRITE_4PIXELS_FLIPPEDx2x2_recovered); }

void DrawClippedTilex2x2_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartPixel, uint32_t Width, uint32_t StartLine, uint32_t LineCount)
{ render8_clipped(Tile, Offset, StartPixel, Width, StartLine, LineCount, WRITE_4PIXELSx2x2_recovered, WRITE_4PIXELS_FLIPPEDx2x2_recovered); }
