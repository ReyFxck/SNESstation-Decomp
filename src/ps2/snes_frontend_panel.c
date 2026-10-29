/*
 * SNES Station v0.23 - Frontend Panel Composition
 * Canonical source from progress25_small_core_helpers.c (0x00101b64 range)
 */

#include <stdint.h>

typedef struct SnesAsset {
    const void *data;
    uint32_t aux;
} SnesAsset;

typedef struct SnesGsHooks {
    void (*prepare_panel)(void *opaque, void *driver);
    uint32_t (*get_texture_base)(void *opaque, void *driver);
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

typedef struct SnesGsState {
    void *driver;              /* logical target DAT_001bb2c0 */
    uintptr_t font_owner_word; /* logical target DAT_001bb748 */

    SnesAsset background_iif;   /* pointer word 0x001bb310 -> 0x001fafd0 */
    SnesAsset logo_iif;         /* pointer word 0x001bb314 -> 0x00290ff0 */
    SnesAsset panel_corner_iif; /* pointer word 0x001bb318 -> 0x002ab1e0 */
    SnesAsset font_bfnt;        /* pointer word 0x001bb31c -> 0x002abb00 */
    SnesGsHooks hooks;
} SnesGsState;

/* 0x00101b64
 *
 * Composes the fixed frontend panel. The target constants and draw order are
 * preserved exactly from the committed R5900 decompile.
 */
void snes_frontend_panel_00101b64(SnesGsState *state)
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