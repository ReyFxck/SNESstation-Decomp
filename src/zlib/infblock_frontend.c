/*
 * SNES Station v0.23 WIP — zlib 1.1.3 inflate-block front-end recovery.
 *
 * Target VAs:
 *   0x00194628 inflate_blocks_reset
 *   0x001946e8 inflate_blocks_new
 *   0x001947d0 inflate_blocks
 *   0x00195280 inflate_blocks_free
 *   0x001952e8 inflate_set_dictionary
 *   0x0019532c inflate_blocks_sync_point
 *
 * The target allocation constants directly confirm sizeof(block-state)=0x48,
 * sizeof(inflate_huft)=8 and MANY=1440.
 */
#include "../../include/zlib_1_1_3_recovered.h"

#include <string.h>

#define ZR_INFLATE_HUFT_SIZE 8u
#define ZR_INFLATE_MANY 1440u
#define ZR_TARGET_BLOCK_STATE_SIZE 0x48u

static void *zr_alloc(zr_stream *z, unsigned items, unsigned size)
{
    return z->zalloc(z->opaque, items, size);
}

static void zr_free(zr_stream *z, void *p)
{
    if (p != NULL)
        z->zfree(z->opaque, p);
}

/* Target VA 0x00194628. */
void inflate_blocks_reset_recovered(zr_inflate_blocks_state *s, zr_stream *z,
                                   uint64_t *check_out)
{
    if (check_out != NULL)
        *check_out = s->check;

    if (s->mode == ZR_BLOCK_BTREE || s->mode == ZR_BLOCK_DTREE)
        zr_free(z, s->sub.trees.blens);
    if (s->mode == ZR_BLOCK_CODES)
        inflate_codes_free_recovered(s->sub.decode.codes, z);

    s->mode = ZR_BLOCK_TYPE;
    s->bitk = 0;
    s->bitb = 0;
    s->read = s->window;
    s->write = s->window;
    if (s->checkfn != NULL) {
        s->check = s->checkfn(0, NULL, 0);
        z->adler = s->check;
    }
}

/* Target VA 0x001946e8. */
zr_inflate_blocks_state *inflate_blocks_new_recovered(zr_stream *z,
                                                      zr_check_func checkfn,
                                                      uint32_t window_size)
{
    zr_inflate_blocks_state *s;

    s = (zr_inflate_blocks_state *)zr_alloc(z, 1,
                                             ZR_TARGET_BLOCK_STATE_SIZE);
    if (s == NULL)
        return NULL;
    memset(s, 0, sizeof(*s));

    s->hufts = zr_alloc(z, ZR_INFLATE_HUFT_SIZE, ZR_INFLATE_MANY);
    if (s->hufts == NULL) {
        zr_free(z, s);
        return NULL;
    }

    s->window = (uint8_t *)zr_alloc(z, 1, window_size);
    if (s->window == NULL) {
        zr_free(z, s->hufts);
        zr_free(z, s);
        return NULL;
    }

    s->end = s->window + window_size;
    s->checkfn = checkfn;
    s->mode = ZR_BLOCK_TYPE;
    inflate_blocks_reset_recovered(s, z, NULL);
    return s;
}


/* Target VA 0x00195280. */
int inflate_blocks_free_recovered(zr_inflate_blocks_state *s, zr_stream *z)
{
    inflate_blocks_reset_recovered(s, z, NULL);
    zr_free(z, s->window);
    zr_free(z, s->hufts);
    zr_free(z, s);
    return ZR_OK;
}

/* Target VA 0x001952e8. */
void inflate_set_dictionary_recovered(zr_inflate_blocks_state *s,
                                     const uint8_t *dictionary, uint32_t n)
{
    memmove(s->window, dictionary, n);
    s->read = s->window + n;
    s->write = s->window + n;
}

/* Target VA 0x0019532c. */
int inflate_blocks_sync_point_recovered(zr_inflate_blocks_state *s)
{
    return s->mode == ZR_BLOCK_LENS;
}
