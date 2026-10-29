/*
 * SNES Station v0.23 - Frontend Asset Upload
 * Canonical source from progress25_small_core_helpers.c (0x001019a8 range)
 */

#include <stdint.h>
#include <string.h>

typedef struct SnesAsset {
    const void *data;
    uint32_t aux; /* IIF/BFNT texture width at header +0x04 */
} SnesAsset;

typedef struct SnesGsHooks {
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

/* 0x001019a8
 *
 * Uploads three fixed frontend assets into successive texture-buffer regions,
 * flushing after each upload, then records the current driver in the font
 * owner slot and uploads the font at texture_base + 0xc5000.
 */
void snes_frontend_init_001019a8(SnesGsState *state)
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