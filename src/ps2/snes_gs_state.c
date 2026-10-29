/*
 * SNES Station v0.23 PS2 - GS State & Asset Management
 * Canonical source from progress25_small_core_helpers.c @ 0x001019a8, 0x00101b64,
 * 0x0012b3e8, 0x0015cce4
 */

#include <stdint.h>
#include <string.h>

typedef struct {
    const void *data;
    uint32_t aux; /* IIF/BFNT texture width at header +0x04 */
} SnesAsset;

typedef struct {
    uint32_t (*get_texture_base)(void *opaque, void *driver);
    void (*texture_upload)(void *opaque, void *driver,
                           uint32_t dst, uint32_t width,
                           uint32_t x, uint32_t y, uint32_t psm,
                           const void *data, uint32_t aux);
    void (*texture_flush)(void *opaque, void *driver);
    void (*pipe_flush)(void *opaque, void *driver);
    void (*font_upload)(void *opaque, void *driver,
                        const SnesAsset *font, uint32_t dst,
                        uint32_t aux, uint32_t arg5, uint32_t arg6);

    void (*prepare_panel)(void *opaque, void *driver);
    void (*texture_set)(void *opaque, void *driver,
                        uint32_t base, uint32_t width,
                        uint32_t tw, uint32_t th, uint32_t psm,
                        uint32_t csm, uint32_t csa);
    void (*rect_flat)(void *opaque, void *driver,
                      int x1, int y1, int x2, int y2,
                      int z, uint64_t color);
    void (*rect_texture)(void *opaque, void *driver,
                         int a, int b, int c, int d,
                         int e, int f, int g);
    void *opaque;
} SnesGsHooks;

typedef struct {
    void *driver;              /* logical target DAT_001bb2c0 */
    uintptr_t font_owner_word; /* logical target DAT_001bb748 */

    SnesAsset background_iif;   /* pointer word 0x001bb310 -> 0x001fafd0 */
    SnesAsset logo_iif;         /* pointer word 0x001bb314 -> 0x00290ff0 */
    SnesAsset panel_corner_iif; /* pointer word 0x001bb318 -> 0x002ab1e0 */
    SnesAsset font_bfnt;        /* pointer word 0x001bb31c -> 0x002abb00 */
    SnesGsHooks hooks;
} SnesGsState;

/* One target channel occupies 0x16 bytes from 0x0035d366. */
typedef struct {
    uint16_t source;        /* +0x00, target 0x0035d366 */
    uint16_t mirror;        /* +0x02, target 0x0035d368 */
    uint8_t reserved04[10];
    uint8_t clear_flag;     /* +0x0e, target 0x0035d374 */
    uint8_t set_flag;       /* +0x0f, target 0x0035d375 */
    uint8_t reserved10[6];
} SnesChannel22;

typedef struct {
    uint8_t disable_source; /* target 0x0034556b */
    uint8_t source_420c;    /* target *(0x0034e2c4 + 0x420c) */

    uint8_t active_mask;    /* target 0x0035c269 */
    uint8_t reset_pending;  /* target 0x0035c26a */

    SnesChannel22 channel[8];

    /*
     * 0x0012b3e8 performs overlapping four-byte zero stores every two bytes
     * from target 0x0035d410.  18 bytes cover offsets 0..17 touched by all
     * eight stores.
     */
    uint8_t overlap_zero[18];
} SnesEightChannelState;

/* 0x001019a8: Upload three fixed frontend assets, flush, then upload font */
void snes_gs_upload_frontend_assets_001019a8(SnesGsState *state)
{
    uint32_t base;

    base = state->hooks.get_texture_base(state->hooks.opaque, state->driver);
    state->hooks.texture_upload(state->hooks.opaque, state->driver,
                                base, 0x280, 0, 0, 2,
                                state->background_iif.data,
                                state->background_iif.aux);
    state->hooks.texture_flush(state->hooks.opaque, state->driver);
    state->hooks.pipe_flush(state->hooks.opaque, state->driver);

    base = state->hooks.get_texture_base(state->hooks.opaque, state->driver);
    state->hooks.texture_upload(state->hooks.opaque, state->driver,
                                base + 0x0a0000, 0x180, 0, 0, 0,
                                state->logo_iif.data,
                                state->logo_iif.aux);
    state->hooks.texture_flush(state->hooks.opaque, state->driver);
    state->hooks.pipe_flush(state->hooks.opaque, state->driver);

    base = state->hooks.get_texture_base(state->hooks.opaque, state->driver);
    state->hooks.texture_upload(state->hooks.opaque, state->driver,
                                base + 0x0c4000, 0x20, 0, 0, 0,
                                state->panel_corner_iif.data,
                                state->panel_corner_iif.aux);
    state->hooks.texture_flush(state->hooks.opaque, state->driver);
    state->hooks.pipe_flush(state->hooks.opaque, state->driver);

    state->font_owner_word = (uintptr_t)state->driver;
    base = state->hooks.get_texture_base(state->hooks.opaque, state->driver);
    state->hooks.font_upload(state->hooks.opaque, state->driver,
                             &state->font_bfnt, base + 0x0c5000,
                             state->font_bfnt.aux, 0, 0);
}

/* 0x00101b64: Frontend panel composition */
void snes_gs_prepare_frontend_panel_00101b64(SnesGsState *state)
{
    uint32_t base;
    const uint64_t panel = UINT64_C(0x0000000040d08024);
    const uint64_t edge = UINT64_C(0xffffffff80b95c00);

    state->hooks.prepare_panel(state->hooks.opaque, state->driver);

    base = state->hooks.get_texture_base(state->hooks.opaque, state->driver);
    state->hooks.texture_set(state->hooks.opaque, state->driver,
                             base, 0x280, 10, 9, 2, 0, 0);
    state->hooks.rect_texture(state->hooks.opaque, state->driver,
                              0, 0, 0, 0, 0x280, 0x1e0, 0x280);

    base = state->hooks.get_texture_base(state->hooks.opaque, state->driver);
    state->hooks.texture_set(state->hooks.opaque, state->driver,
                             base + 0x0c4000, 0x20, 5, 5, 0, 0, 0);

    state->hooks.rect_flat(state->hooks.opaque, state->driver,
                           100, 0x6c, 0x21c, 0x168, 3, panel);
    state->hooks.rect_flat(state->hooks.opaque, state->driver,
                           0x7c, 0x54, 0x204, 0x6c, 3, panel);
    state->hooks.rect_flat(state->hooks.opaque, state->driver,
                           0x7c, 0x168, 0x204, 0x180, 3, panel);

    state->hooks.rect_flat(state->hooks.opaque, state->driver,
                           100, 0x6c, 0x67, 0x168, 3, edge);
    state->hooks.rect_flat(state->hooks.opaque, state->driver,
                           0x21c, 0x6c, 0x219, 0x168, 3, edge);
    state->hooks.rect_flat(state->hooks.opaque, state->driver,
                           0x7c, 0x54, 0x204, 0x57, 3, edge);
    state->hooks.rect_flat(state->hooks.opaque, state->driver,
                           0x7c, 0x180, 0x204, 0x17d, 3, edge);

    state->hooks.rect_texture(state->hooks.opaque, state->driver,
                              100, 0x54, 0, 0, 0x7c, 0x6c, 0x17);
    state->hooks.rect_texture(state->hooks.opaque, state->driver,
                              0x21c, 0x54, 0, 0, 0x204, 0x6c, 0x17);
    state->hooks.rect_texture(state->hooks.opaque, state->driver,
                              100, 0x180, 0, 0, 0x7c, 0x168, 0x17);
    state->hooks.rect_texture(state->hooks.opaque, state->driver,
                              0x21c, 0x180, 0, 0, 0x204, 0x168, 0x17);

    base = state->hooks.get_texture_base(state->hooks.opaque, state->driver);
    state->hooks.texture_set(state->hooks.opaque, state->driver,
                             base + 0x0a0000, 0x180, 9, 7, 0, 0, 0);
    state->hooks.rect_texture(state->hooks.opaque, state->driver,
                              0x81, 7, 0, 0, 0x1ff, 0x4b, 0x17e);
}

/* 0x0012b3e8: Select active eight-channel mask, refresh per-channel mirror/flags,
 * and perform overlapping four-byte clears.
 */
void snes_eight_channel_update_0012b3e8(SnesEightChannelState *state)
{
    unsigned i;

    state->active_mask = state->disable_source == 0
                       ? state->source_420c
                       : 0;
    state->reset_pending = 1;

    for (i = 0; i < 8; ++i) {
        if (((state->active_mask >> i) & 1u) != 0) {
            state->channel[i].mirror = state->channel[i].source;
            state->channel[i].set_flag = 1;
            state->channel[i].clear_flag = 0;
        }

        memset(&state->overlap_zero[i * 2], 0, 4);
    }
}

typedef struct {
    int mode;               /* target 0x0035d0b8 */
    uint8_t io_4201;
    uint8_t io_213f;

    uint32_t packed_flags;  /* target 0x0035d0c0 */
    uint8_t active;         /* target 0x0035bfdc */
    uint16_t x_clamped;     /* target 0x0035bfd8 */
    uint16_t line_plus_one; /* target 0x0035bfd6 */
    int line_count;         /* logical target 0x0035bff2 */

    int (*query)(void *opaque, int *x, int *line, uint32_t flags[2]);
    void *opaque;
} SnesPpuBoundsState;

/* 0x0015cce4: Mode-4 query helper with X/line clamping */
void snes_ppu_bounds_query_0015cce4(SnesPpuBoundsState *state)
{
    int x;
    int line;
    int clamped_line;
    uint32_t flags[2];

    if (state->mode != 4)
        return;
    if (state->query(state->opaque, &x, &line, flags) == 0)
        return;

    state->packed_flags =
        ((flags[0] & 1u) << 15) |
        ((flags[0] & 2u) << 13) |
        ((flags[0] & 4u) << 11) |
        ((flags[0] & 8u) << 9) |
        0xffu;

    if ((state->io_4201 & 0x80u) == 0)
        return;

    x += 0x28;
    if (x > 0x127)
        x = 0x127;
    if (x < 0x28)
        x = 0x28;

    state->active = 1;

    clamped_line = state->line_count - 1;
    if (line <= clamped_line)
        clamped_line = line;
    if (clamped_line < 0)
        clamped_line = 0;

    state->x_clamped = (uint16_t)x;
    state->line_plus_one = (uint16_t)((int16_t)clamped_line + 1);
    state->io_213f |= 0x43u;
}