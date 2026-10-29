/*
 * SNES Station v0.23 WIP — DrawClippedTile recovered from R5900 code.
 * Original function: 0x001845a8
 */
#include <stdint.h>

#define PTR32(base, off) (*(uint32_t *)((uint8_t *)(base) + (off)))
#define U32(base, off)   (*(uint32_t *)((uint8_t *)(base) + (off)))
#define U8(base, off)    (*(uint8_t  *)((uint8_t *)(base) + (off)))

#define V_FLIP 0x8000u
#define H_FLIP 0x4000u
#define BLANK_TILE 2u

extern uint8_t g_bg_blob[];                 /* EE VA 0x0035d450 */
extern uint8_t g_gfx_blob[];                /* EE VA 0x0035d480 */
extern uint16_t g_screen_colors[];           /* EE VA 0x0035ceb0 */
extern uint16_t g_direct_colour_maps[][256]; /* EE VA 0x003f2f80 */
extern uint8_t g_direct_colour_maps_need_rebuild;
extern const uint32_t g_head_mask[4];        /* EE VA 0x003f4040 */
extern const uint32_t g_tail_mask[5];        /* EE VA 0x003f4050 */

extern uint8_t ConvertTile_recovered(uint8_t *pCache, uint32_t TileAddr);
extern void WRITE_4PIXELS_recovered(uint32_t Offset, const uint8_t *Pixels);
extern void WRITE_4PIXELS_FLIPPED_recovered(uint32_t Offset, const uint8_t *Pixels);
extern void S9xBuildDirectColourMaps_0014308c(void);

static uint32_t swap_dword_recovered(uint32_t x)
{
    return ((x & 0x000000ffu) << 24) |
           ((x & 0x0000ff00u) << 8)  |
           ((x & 0x00ff0000u) >> 8)  |
           ((x & 0xff000000u) >> 24);
}

void DrawClippedTile_recovered(uint32_t Tile, uint32_t Offset,
                               uint32_t StartPixel, uint32_t Width,
                               uint32_t StartLine, uint32_t LineCount)
{
    uint32_t tile_shift = U32(g_bg_blob, 0x08);
    uint32_t tile_addr = U32(g_bg_blob, 0x0c) + ((Tile & 0x3ffu) << tile_shift);
    uint32_t tile_number;
    uint8_t *cache;
    uint8_t *valid;
    uint32_t d1, d2;
    uint32_t ppl;

    if ((Tile & 0x1ffu) >= 0x100u)
        tile_addr += U32(g_bg_blob, 0x10);
    tile_addr &= 0xffffu;

    tile_number = tile_addr >> tile_shift;
    cache = (uint8_t *)(uintptr_t)PTR32(g_bg_blob, 0x24) + (tile_number << 6);
    valid = (uint8_t *)(uintptr_t)PTR32(g_bg_blob, 0x28) + tile_number;

    if (*valid == 0)
        *valid = ConvertTile_recovered(cache, tile_addr);
    if (*valid == BLANK_TILE)
        return;

    if (U8(g_bg_blob, 0x2c)) {
        uint32_t map;
        if (g_direct_colour_maps_need_rebuild)
            S9xBuildDirectColourMaps_0014308c();
        map = (Tile >> 10) & U32(g_bg_blob, 0x20);
        PTR32(g_gfx_blob, 0x44) =
            (uint32_t)(uintptr_t)&g_direct_colour_maps[map][0];
    } else {
        uint32_t palette = (Tile >> 10) & U32(g_bg_blob, 0x20);
        palette = (palette << U32(g_bg_blob, 0x1c)) + U32(g_bg_blob, 0x18);
        PTR32(g_gfx_blob, 0x44) = (uint32_t)(uintptr_t)&g_screen_colors[palette];
    }

    /* Recovered TILE_CLIP_PREAMBLE. */
    if (StartPixel < 4) {
        d1 = g_head_mask[StartPixel];
        if (StartPixel + Width < 4)
            d1 &= g_tail_mask[StartPixel + Width];
    } else {
        d1 = 0;
    }

    if (StartPixel + Width > 4) {
        d2 = StartPixel > 4 ? g_head_mask[StartPixel - 4] : 0xffffffffu;
        d2 &= g_tail_mask[StartPixel + Width - 4];
    } else {
        d2 = 0;
    }

    ppl = U32(g_gfx_blob, 0x30);

    if ((Tile & (V_FLIP | H_FLIP)) == 0) {
        const uint8_t *bp = cache + StartLine;
        while (LineCount--) {
            uint32_t dd;
            dd = *(const uint32_t *)(bp + 0) & d1;
            if (dd) WRITE_4PIXELS_recovered(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 4) & d2;
            if (dd) WRITE_4PIXELS_recovered(Offset + 4, (const uint8_t *)&dd);
            bp += 8;
            Offset += ppl;
        }
    } else if ((Tile & V_FLIP) == 0) {
        const uint8_t *bp = cache + StartLine;
        d1 = swap_dword_recovered(d1);
        d2 = swap_dword_recovered(d2);
        while (LineCount--) {
            uint32_t dd;
            dd = *(const uint32_t *)(bp + 4) & d1;
            if (dd) WRITE_4PIXELS_FLIPPED_recovered(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 0) & d2;
            if (dd) WRITE_4PIXELS_FLIPPED_recovered(Offset + 4, (const uint8_t *)&dd);
            bp += 8;
            Offset += ppl;
        }
    } else if (Tile & H_FLIP) {
        const uint8_t *bp = cache + 56 - StartLine;
        d1 = swap_dword_recovered(d1);
        d2 = swap_dword_recovered(d2);
        while (LineCount--) {
            uint32_t dd;
            dd = *(const uint32_t *)(bp + 4) & d1;
            if (dd) WRITE_4PIXELS_FLIPPED_recovered(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 0) & d2;
            if (dd) WRITE_4PIXELS_FLIPPED_recovered(Offset + 4, (const uint8_t *)&dd);
            bp -= 8;
            Offset += ppl;
        }
    } else {
        const uint8_t *bp = cache + 56 - StartLine;
        while (LineCount--) {
            uint32_t dd;
            dd = *(const uint32_t *)(bp + 0) & d1;
            if (dd) WRITE_4PIXELS_recovered(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 4) & d2;
            if (dd) WRITE_4PIXELS_recovered(Offset + 4, (const uint8_t *)&dd);
            bp -= 8;
            Offset += ppl;
        }
    }
}
