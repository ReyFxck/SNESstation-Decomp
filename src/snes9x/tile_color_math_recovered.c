/*
 * SNES Station v0.23 WIP — 16-bit colour-math tile renderer family.
 *
 * Binary signatures prove the old Snes9x RGB565-ish blending forms:
 *   low-bit mask       0x0421
 *   remove-low mask    0xfbde
 *   subtraction guard  0x8420
 * and GFX table pointers at +0x18/+0x1c/+0x20.
 *
 * The generic local render helpers below factor macro-expanded binary code;
 * they are not claimed to exist as original symbols.
 */
#include <stdint.h>

#define PTR32(base, off) (*(uint32_t *)((uint8_t *)(base) + (off)))
#define U32(base, off)   (*(uint32_t *)((uint8_t *)(base) + (off)))
#define U16(base, off)   (*(uint16_t *)((uint8_t *)(base) + (off)))
#define U8(base, off)    (*(uint8_t  *)((uint8_t *)(base) + (off)))
#define V_FLIP 0x8000u
#define H_FLIP 0x4000u
#define BLANK_TILE 2u
#define RGB_LOW_BITS 0x0421u
#define RGB_REMOVE_LOW 0xfbdeu
#define RGB_HI_GUARD_X2 0x8420u

extern uint8_t g_bg_blob[];
extern uint8_t g_gfx_blob[];
extern uint16_t g_screen_colors[];
extern uint16_t g_direct_colour_maps[][256];
extern uint8_t g_direct_colour_maps_need_rebuild;
extern const uint32_t g_head_mask[4];
extern const uint32_t g_tail_mask[5];
extern uint8_t ConvertTile_recovered(uint8_t *, uint32_t);
extern void S9xBuildDirectColourMaps_0014308c(void);

typedef enum {
    CM_ADD,
    CM_ADD_HALF,
    CM_SUB,
    CM_SUB_HALF,
    CM_FIXED_ADD_HALF,
    CM_FIXED_SUB_HALF
} colour_mode_t;

typedef void (*colour_writer_t)(uint32_t, const uint8_t *);

static uint32_t swap32_local(uint32_t x)
{
    return ((x & 0xffu) << 24) | ((x & 0xff00u) << 8) |
           ((x & 0xff0000u) >> 8) | ((x & 0xff000000u) >> 24);
}

static uint8_t *prepare_colour_tile(uint32_t Tile)
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

/* GFX +0x18: X2 table.  The OR of differing low bits is visible in R5900. */
uint16_t COLOR_ADD_recovered(uint16_t c1, uint16_t c2)
{
    const uint16_t *x2 = (const uint16_t *)(uintptr_t)PTR32(g_gfx_blob, 0x18);
    uint32_t idx = (((uint32_t)(c1 & RGB_REMOVE_LOW) + (c2 & RGB_REMOVE_LOW)) >> 1)
                 + (c1 & c2 & RGB_LOW_BITS);
    return (uint16_t)(x2[idx] | ((c1 ^ c2) & RGB_LOW_BITS));
}

/* No lookup table in the target path; this exact mask/average pattern is inline. */
uint16_t COLOR_ADD1_2_recovered(uint16_t c1, uint16_t c2)
{
    return (uint16_t)((((uint32_t)(c1 & RGB_REMOVE_LOW) + (c2 & RGB_REMOVE_LOW)) >> 1)
                    + (c1 & c2 & RGB_LOW_BITS));
}

/* GFX +0x1c: ZERO_OR_X2. */
uint16_t COLOR_SUB_recovered(uint16_t c1, uint16_t c2)
{
    const uint16_t *zero_or_x2 = (const uint16_t *)(uintptr_t)PTR32(g_gfx_blob, 0x1c);
    uint32_t idx = (((uint32_t)c1 | RGB_HI_GUARD_X2) - (c2 & RGB_REMOVE_LOW)) >> 1;
    int32_t low = (int32_t)(c1 & RGB_LOW_BITS) - (int32_t)(c2 & RGB_LOW_BITS);
    return (uint16_t)((int32_t)zero_or_x2[idx] + low);
}

/* GFX +0x20: ZERO. */
uint16_t COLOR_SUB1_2_recovered(uint16_t c1, uint16_t c2)
{
    const uint16_t *zero = (const uint16_t *)(uintptr_t)PTR32(g_gfx_blob, 0x20);
    uint32_t idx = (((uint32_t)c1 | RGB_HI_GUARD_X2) - (c2 & RGB_REMOVE_LOW)) >> 1;
    return zero[idx];
}

static uint16_t apply_colour_math(colour_mode_t mode, uint16_t main,
                                  uint8_t sub_depth, uint16_t sub_pixel)
{
    uint16_t fixed = U16(g_gfx_blob, 0x50);
    switch (mode) {
    case CM_ADD:
        if (sub_depth == 0) return main;
        return sub_depth == 1 ? COLOR_ADD_recovered(main, fixed)
                              : COLOR_ADD_recovered(main, sub_pixel);
    case CM_ADD_HALF:
        if (sub_depth == 0) return main;
        /* Original Select3 uses full fixed-colour add for sub-depth 1. */
        return sub_depth == 1 ? COLOR_ADD_recovered(main, fixed)
                              : COLOR_ADD1_2_recovered(main, sub_pixel);
    case CM_SUB:
        if (sub_depth == 0) return main;
        return sub_depth == 1 ? COLOR_SUB_recovered(main, fixed)
                              : COLOR_SUB_recovered(main, sub_pixel);
    case CM_SUB_HALF:
        if (sub_depth == 0) return main;
        return sub_depth == 1 ? COLOR_SUB_recovered(main, fixed)
                              : COLOR_SUB1_2_recovered(main, sub_pixel);
    case CM_FIXED_ADD_HALF:
        return sub_depth == 1 ? COLOR_ADD1_2_recovered(main, fixed) : main;
    case CM_FIXED_SUB_HALF:
        return sub_depth == 1 ? COLOR_SUB1_2_recovered(main, fixed) : main;
    }
    return main;
}

static void write_colour4(uint32_t Offset, const uint8_t *Pixels,
                          int flipped, colour_mode_t mode)
{
    uint16_t *screen = (uint16_t *)(uintptr_t)PTR32(g_gfx_blob, 0x3c) + Offset;
    uint8_t *depth = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x08) + Offset;
    uint8_t *sub_depth = (uint8_t *)(uintptr_t)PTR32(g_gfx_blob, 0x0c) + Offset;
    const uint16_t *colors = (const uint16_t *)(uintptr_t)PTR32(g_gfx_blob, 0x44);
    uint32_t delta = U32(g_gfx_blob, 0x14);
    uint8_t z1 = U8(g_gfx_blob, 0x4c), z2 = U8(g_gfx_blob, 0x4d);
    unsigned i;

    for (i = 0; i < 4; ++i) {
        uint8_t p = Pixels[flipped ? 3 - i : i];
        if (depth[i] < z1 && p != 0) {
            uint16_t main = colors[p];
            uint16_t sub = screen[delta + i];
            screen[i] = apply_colour_math(mode, main, sub_depth[i], sub);
            depth[i] = z2;
        }
    }
}

#define DECL_WRITER(name, addr, flip, mode) \
    void name##_recovered(uint32_t Offset, const uint8_t *Pixels) \
    { (void)(addr); write_colour4(Offset, Pixels, flip, mode); }

DECL_WRITER(WRITE_4PIXELS16_ADD,             0x001ade2c, 0, CM_ADD)
DECL_WRITER(WRITE_4PIXELS16_FLIPPED_ADD,     0x001ae2c0, 1, CM_ADD)
DECL_WRITER(WRITE_4PIXELS16_ADD1_2,          0x001ae754, 0, CM_ADD_HALF)
DECL_WRITER(WRITE_4PIXELS16_FLIPPED_ADD1_2,  0x001aeb58, 1, CM_ADD_HALF)
DECL_WRITER(WRITE_4PIXELS16_SUB,             0x001aef5c, 0, CM_SUB)
DECL_WRITER(WRITE_4PIXELS16_FLIPPED_SUB,     0x001af310, 1, CM_SUB)
DECL_WRITER(WRITE_4PIXELS16_SUB1_2,          0x001af6c4, 0, CM_SUB_HALF)
DECL_WRITER(WRITE_4PIXELS16_FLIPPED_SUB1_2,  0x001afa88, 1, CM_SUB_HALF)
DECL_WRITER(WRITE_4PIXELS16_ADDF1_2,         0x001afe4c, 0, CM_FIXED_ADD_HALF)
DECL_WRITER(WRITE_4PIXELS16_FLIPPED_ADDF1_2, 0x001b008c, 1, CM_FIXED_ADD_HALF)
DECL_WRITER(WRITE_4PIXELS16_SUBF1_2,         0x001b02cc, 0, CM_FIXED_SUB_HALF)
DECL_WRITER(WRITE_4PIXELS16_FLIPPED_SUBF1_2, 0x001b052c, 1, CM_FIXED_SUB_HALF)

#undef DECL_WRITER

static void render_colour(uint32_t Tile, uint32_t Offset,
                          uint32_t StartLine, uint32_t LineCount,
                          colour_writer_t normal, colour_writer_t flipped)
{
    uint8_t *cache = prepare_colour_tile(Tile);
    uint32_t ppl = U32(g_gfx_blob, 0x30);
    const uint8_t *bp;
    if (!cache) return;

    if ((Tile & (V_FLIP | H_FLIP)) == 0) {
        bp = cache + StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 0)) normal(Offset, bp + 0);
            if (*(const uint32_t *)(bp + 4)) normal(Offset + 4, bp + 4);
            bp += 8; Offset += ppl;
        }
    } else if ((Tile & V_FLIP) == 0) {
        bp = cache + StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 4)) flipped(Offset, bp + 4);
            if (*(const uint32_t *)(bp + 0)) flipped(Offset + 4, bp + 0);
            bp += 8; Offset += ppl;
        }
    } else if (Tile & H_FLIP) {
        bp = cache + 56 - StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 4)) flipped(Offset, bp + 4);
            if (*(const uint32_t *)(bp + 0)) flipped(Offset + 4, bp + 0);
            bp -= 8; Offset += ppl;
        }
    } else {
        bp = cache + 56 - StartLine;
        while (LineCount--) {
            if (*(const uint32_t *)(bp + 0)) normal(Offset, bp + 0);
            if (*(const uint32_t *)(bp + 4)) normal(Offset + 4, bp + 4);
            bp -= 8; Offset += ppl;
        }
    }
}

static void render_colour_clipped(uint32_t Tile, uint32_t Offset,
                                  uint32_t StartPixel, uint32_t Width,
                                  uint32_t StartLine, uint32_t LineCount,
                                  colour_writer_t normal, colour_writer_t flipped)
{
    uint8_t *cache = prepare_colour_tile(Tile);
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
            if (dd) normal(Offset + 4, (const uint8_t *)&dd);
            bp += 8; Offset += ppl;
        }
    } else if ((Tile & V_FLIP) == 0) {
        bp = cache + StartLine; d1 = swap32_local(d1); d2 = swap32_local(d2);
        while (LineCount--) {
            uint32_t dd = *(const uint32_t *)(bp + 4) & d1;
            if (dd) flipped(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 0) & d2;
            if (dd) flipped(Offset + 4, (const uint8_t *)&dd);
            bp += 8; Offset += ppl;
        }
    } else if (Tile & H_FLIP) {
        bp = cache + 56 - StartLine; d1 = swap32_local(d1); d2 = swap32_local(d2);
        while (LineCount--) {
            uint32_t dd = *(const uint32_t *)(bp + 4) & d1;
            if (dd) flipped(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 0) & d2;
            if (dd) flipped(Offset + 4, (const uint8_t *)&dd);
            bp -= 8; Offset += ppl;
        }
    } else {
        bp = cache + 56 - StartLine;
        while (LineCount--) {
            uint32_t dd = *(const uint32_t *)(bp + 0) & d1;
            if (dd) normal(Offset, (const uint8_t *)&dd);
            dd = *(const uint32_t *)(bp + 4) & d2;
            if (dd) normal(Offset + 4, (const uint8_t *)&dd);
            bp -= 8; Offset += ppl;
        }
    }
}

#define DRAW_PAIR(base, clipped, normal_writer, flipped_writer) \
    void base##_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartLine, uint32_t LineCount) \
    { render_colour(Tile, Offset, StartLine, LineCount, normal_writer##_recovered, flipped_writer##_recovered); } \
    void clipped##_recovered(uint32_t Tile, uint32_t Offset, uint32_t StartPixel, uint32_t Width, uint32_t StartLine, uint32_t LineCount) \
    { render_colour_clipped(Tile, Offset, StartPixel, Width, StartLine, LineCount, normal_writer##_recovered, flipped_writer##_recovered); }

DRAW_PAIR(DrawTile16Add, DrawClippedTile16Add, WRITE_4PIXELS16_ADD, WRITE_4PIXELS16_FLIPPED_ADD)
DRAW_PAIR(DrawTile16Add1_2, DrawClippedTile16Add1_2, WRITE_4PIXELS16_ADD1_2, WRITE_4PIXELS16_FLIPPED_ADD1_2)
DRAW_PAIR(DrawTile16Sub, DrawClippedTile16Sub, WRITE_4PIXELS16_SUB, WRITE_4PIXELS16_FLIPPED_SUB)
DRAW_PAIR(DrawTile16Sub1_2, DrawClippedTile16Sub1_2, WRITE_4PIXELS16_SUB1_2, WRITE_4PIXELS16_FLIPPED_SUB1_2)
DRAW_PAIR(DrawTile16FixedAdd1_2, DrawClippedTile16FixedAdd1_2, WRITE_4PIXELS16_ADDF1_2, WRITE_4PIXELS16_FLIPPED_ADDF1_2)
DRAW_PAIR(DrawTile16FixedSub1_2, DrawClippedTile16FixedSub1_2, WRITE_4PIXELS16_SUBF1_2, WRITE_4PIXELS16_FLIPPED_SUBF1_2)

#undef DRAW_PAIR
