/*
 * SNES Station v0.23 WIP — DrawTile recovered from R5900 code.
 * Original function: 0x0018428c
 *
 * The macro structure was independently visible in the binary and was then
 * validated against a close-era Snes9x tile.h. Exact GFX/BG C++ struct types
 * are still being recovered, so byte-offset access is used where necessary.
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
extern uint8_t g_direct_colour_maps_need_rebuild; /* EE VA 0x0035c26f */

extern uint8_t ConvertTile_recovered(uint8_t *pCache, uint32_t TileAddr);
extern void WRITE_4PIXELS_recovered(uint32_t Offset, const uint8_t *Pixels);
extern void WRITE_4PIXELS_FLIPPED_recovered(uint32_t Offset, const uint8_t *Pixels);
extern void S9xBuildDirectColourMaps_0014308c(void);

void DrawTile_recovered(uint32_t Tile, uint32_t Offset,
                        uint32_t StartLine, uint32_t LineCount)
{
    uint32_t tile_shift = U32(g_bg_blob, 0x08);
    uint32_t tile_addr = U32(g_bg_blob, 0x0c) + ((Tile & 0x3ffu) << tile_shift);
    uint32_t tile_number;
    uint8_t *cache;
    uint8_t *valid;
    uint32_t lines;
    uint32_t ppl;

    if ((Tile & 0x1ffu) >= 0x100u)
        tile_addr += U32(g_bg_blob, 0x10); /* NameSelect */

    tile_addr &= 0xffffu;
    tile_number = tile_addr >> tile_shift;

    cache = (uint8_t *)(uintptr_t)PTR32(g_bg_blob, 0x24) + (tile_number << 6);
    valid = (uint8_t *)(uintptr_t)PTR32(g_bg_blob, 0x28) + tile_number;

    if (*valid == 0)
        *valid = ConvertTile_recovered(cache, tile_addr);
    if (*valid == BLANK_TILE)
        return;

    /* Select palette / direct-colour map. */
    if (U8(g_bg_blob, 0x2c)) {
        uint32_t map;
        if (g_direct_colour_maps_need_rebuild)
            S9xBuildDirectColourMaps_0014308c();
        map = (Tile >> 10) & U32(g_bg_blob, 0x20); /* PaletteMask */
        PTR32(g_gfx_blob, 0x44) =
            (uint32_t)(uintptr_t)&g_direct_colour_maps[map][0];
    } else {
        uint32_t palette = (Tile >> 10) & U32(g_bg_blob, 0x20);
        palette = (palette << U32(g_bg_blob, 0x1c)) + U32(g_bg_blob, 0x18);
        PTR32(g_gfx_blob, 0x44) = (uint32_t)(uintptr_t)&g_screen_colors[palette];
    }

    ppl = U32(g_gfx_blob, 0x30);
    lines = LineCount;

    if ((Tile & (V_FLIP | H_FLIP)) == 0) {
        const uint8_t *bp = cache + StartLine;
        while (lines--) {
            if (*(const uint32_t *)(bp + 0))
                WRITE_4PIXELS_recovered(Offset, bp + 0);
            if (*(const uint32_t *)(bp + 4))
                WRITE_4PIXELS_recovered(Offset + 4, bp + 4);
            bp += 8;
            Offset += ppl;
        }
    } else if ((Tile & V_FLIP) == 0) {
        const uint8_t *bp = cache + StartLine;
        while (lines--) {
            if (*(const uint32_t *)(bp + 4))
                WRITE_4PIXELS_FLIPPED_recovered(Offset, bp + 4);
            if (*(const uint32_t *)(bp + 0))
                WRITE_4PIXELS_FLIPPED_recovered(Offset + 4, bp + 0);
            bp += 8;
            Offset += ppl;
        }
    } else if (Tile & H_FLIP) {
        const uint8_t *bp = cache + 56 - StartLine;
        while (lines--) {
            if (*(const uint32_t *)(bp + 4))
                WRITE_4PIXELS_FLIPPED_recovered(Offset, bp + 4);
            if (*(const uint32_t *)(bp + 0))
                WRITE_4PIXELS_FLIPPED_recovered(Offset + 4, bp + 0);
            bp -= 8;
            Offset += ppl;
        }
    } else {
        const uint8_t *bp = cache + 56 - StartLine;
        while (lines--) {
            if (*(const uint32_t *)(bp + 0))
                WRITE_4PIXELS_recovered(Offset, bp + 0);
            if (*(const uint32_t *)(bp + 4))
                WRITE_4PIXELS_recovered(Offset + 4, bp + 4);
            bp -= 8;
            Offset += ppl;
        }
    }
}
