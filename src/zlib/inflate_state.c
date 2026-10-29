/*
 * SNES Station v0.23 WIP — zlib 1.1.3 inflate interface recovery.
 *
 * Target functions reconstructed here:
 *   0x0019272c inflateReset
 *   0x00192784 inflateEnd
 *   0x00192800 inflateInit2_
 *   0x00192948 inflateInit_
 *
 * The large state machine at 0x0019296c is also reconstructed here; these
 * setup/teardown paths and the public inflate interface are behaviorally complete.
 */
#include "../../include/zlib_1_1_3_recovered.h"

#include <stdlib.h>
#include <string.h>

static void *zr_alloc(zr_stream *strm, unsigned items, unsigned size)
{
    return strm->zalloc(strm->opaque, items, size);
}

static void zr_free(zr_stream *strm, void *p)
{
    if (p != NULL)
        strm->zfree(strm->opaque, p);
}

/* Target VA 0x0019272c. */
int inflateReset_recovered(zr_stream *z)
{
    zr_inflate_state *state;

    if (z == NULL || z->state == NULL)
        return ZR_STREAM_ERROR;
    state = (zr_inflate_state *)z->state;
    z->total_in = 0;
    z->total_out = 0;
    z->msg = NULL;
    state->mode = state->nowrap ? ZR_INF_BLOCKS : ZR_INF_METHOD;
    inflate_blocks_reset_recovered(state->blocks, z, NULL);
    return ZR_OK;
}

/* Target VA 0x00192784. */
int inflateEnd_recovered(zr_stream *z)
{
    zr_inflate_state *state;

    if (z == NULL || z->state == NULL || z->zfree == NULL)
        return ZR_STREAM_ERROR;
    state = (zr_inflate_state *)z->state;
    if (state->blocks != NULL)
        inflate_blocks_free_recovered(state->blocks, z);
    zr_free(z, state);
    z->state = NULL;
    return ZR_OK;
}

/* Target VA 0x00192800. */
int inflateInit2__recovered(zr_stream *z, int w, const char *version,
                            int stream_size)
{
    zr_inflate_state *state;

    if (version == NULL || version[0] != ZR_VERSION[0] ||
        stream_size != ZR_TARGET_STREAM_SIZE)
        return ZR_VERSION_ERROR;
    if (z == NULL)
        return ZR_STREAM_ERROR;

    z->msg = NULL;
    if (z->zalloc == NULL) {
        z->zalloc = zcalloc_00198a84;
        z->opaque = NULL;
    }
    if (z->zfree == NULL)
        z->zfree = zcfree_00198aa4;

    state = (zr_inflate_state *)zr_alloc(z, 1, ZR_TARGET_INFLATE_STATE_SIZE);
    if (state == NULL)
        return ZR_MEM_ERROR;
    memset(state, 0, sizeof(*state));
    z->state = state;
    state->blocks = NULL;
    state->nowrap = 0;

    if (w < 0) {
        w = -w;
        state->nowrap = 1;
    }
    if (w < 8 || w > 15) {
        (void)inflateEnd_recovered(z);
        return ZR_STREAM_ERROR;
    }
    state->wbits = (uint32_t)w;

    state->blocks = inflate_blocks_new_recovered(
        z, state->nowrap ? NULL : adler32_recovered, 1u << (uint32_t)w);
    if (state->blocks == NULL) {
        (void)inflateEnd_recovered(z);
        return ZR_MEM_ERROR;
    }

    (void)inflateReset_recovered(z);
    return ZR_OK;
}

/* Target VA 0x00192948. */
int inflateInit__recovered(zr_stream *z, const char *version, int stream_size)
{
    return inflateInit2__recovered(z, ZR_DEF_WBITS, version, stream_size);
}

/* Target VA 0x0019296c. */
int inflate_recovered(zr_stream *z, int flush)
{
    zr_inflate_state *state;
    int r;
    int f;
    uint32_t b;

    if (z == NULL || z->state == NULL || z->next_in == NULL)
        return ZR_STREAM_ERROR;

    state = (zr_inflate_state *)z->state;
    f = flush == ZR_FINISH ? ZR_BUF_ERROR : ZR_OK;
    r = ZR_BUF_ERROR;

    for (;;) {
        switch (state->mode) {
        case ZR_INF_METHOD:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            state->sub.method = *z->next_in++;
            if ((state->sub.method & 0x0fu) != ZR_DEFLATED) {
                state->mode = ZR_INF_BAD;
                z->msg = (char *)"unknown compression method";
                state->sub.marker = 5;
                break;
            }
            if ((state->sub.method >> 4) + 8u > state->wbits) {
                state->mode = ZR_INF_BAD;
                z->msg = (char *)"invalid window size";
                state->sub.marker = 5;
                break;
            }
            state->mode = ZR_INF_FLAG;
            /* fall through */

        case ZR_INF_FLAG:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            b = *z->next_in++;
            if (((state->sub.method << 8) + b) % 31u != 0) {
                state->mode = ZR_INF_BAD;
                z->msg = (char *)"incorrect header check";
                state->sub.marker = 5;
                break;
            }
            if ((b & 0x20u) == 0) {
                state->mode = ZR_INF_BLOCKS;
                break;
            }
            state->mode = ZR_INF_DICT4;
            /* fall through */

        case ZR_INF_DICT4:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            state->sub.check.need = (uint64_t)(*z->next_in++) << 24;
            state->mode = ZR_INF_DICT3;
            /* fall through */

        case ZR_INF_DICT3:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            state->sub.check.need += (uint64_t)(*z->next_in++) << 16;
            state->mode = ZR_INF_DICT2;
            /* fall through */

        case ZR_INF_DICT2:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            state->sub.check.need += (uint64_t)(*z->next_in++) << 8;
            state->mode = ZR_INF_DICT1;
            /* fall through */

        case ZR_INF_DICT1:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            state->sub.check.need += (uint64_t)(*z->next_in++);
            z->adler = state->sub.check.need;
            state->mode = ZR_INF_DICT0;
            return ZR_NEED_DICT;

        case ZR_INF_DICT0:
            state->mode = ZR_INF_BAD;
            z->msg = (char *)"need dictionary";
            state->sub.marker = 0;
            return ZR_STREAM_ERROR;

        case ZR_INF_BLOCKS:
            r = inflate_blocks_recovered(state->blocks, z, r);
            if (r == ZR_DATA_ERROR) {
                state->mode = ZR_INF_BAD;
                state->sub.marker = 0;
                break;
            }
            if (r == ZR_OK)
                r = f;
            if (r != ZR_STREAM_END)
                return r;
            r = f;
            inflate_blocks_reset_recovered(state->blocks, z,
                                            &state->sub.check.was);
            if (state->nowrap) {
                state->mode = ZR_INF_DONE;
                break;
            }
            state->mode = ZR_INF_CHECK4;
            /* fall through */

        case ZR_INF_CHECK4:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            state->sub.check.need = (uint64_t)(*z->next_in++) << 24;
            state->mode = ZR_INF_CHECK3;
            /* fall through */

        case ZR_INF_CHECK3:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            state->sub.check.need += (uint64_t)(*z->next_in++) << 16;
            state->mode = ZR_INF_CHECK2;
            /* fall through */

        case ZR_INF_CHECK2:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            state->sub.check.need += (uint64_t)(*z->next_in++) << 8;
            state->mode = ZR_INF_CHECK1;
            /* fall through */

        case ZR_INF_CHECK1:
            if (z->avail_in == 0)
                return r;
            r = f;
            z->avail_in--;
            z->total_in++;
            state->sub.check.need += (uint64_t)(*z->next_in++);
            if (state->sub.check.was != state->sub.check.need) {
                state->mode = ZR_INF_BAD;
                z->msg = (char *)"incorrect data check";
                state->sub.marker = 5;
                break;
            }
            state->mode = ZR_INF_DONE;
            /* fall through */

        case ZR_INF_DONE:
            return ZR_STREAM_END;

        case ZR_INF_BAD:
            return ZR_DATA_ERROR;

        default:
            return ZR_STREAM_ERROR;
        }
    }
}

/* Target VA 0x00192e60. */
int inflateSetDictionary_recovered(zr_stream *z, const uint8_t *dictionary,
                                   uint32_t dict_length)
{
    zr_inflate_state *state;
    uint32_t length = dict_length;

    if (z == NULL || z->state == NULL)
        return ZR_STREAM_ERROR;
    state = (zr_inflate_state *)z->state;
    if (state->mode != ZR_INF_DICT0)
        return ZR_STREAM_ERROR;
    if (adler32_recovered(1, dictionary, dict_length) != z->adler)
        return ZR_DATA_ERROR;

    z->adler = 1;
    if (length >= (1u << state->wbits)) {
        length = (1u << state->wbits) - 1u;
        dictionary += dict_length - length;
    }
    inflate_set_dictionary_recovered(state->blocks, dictionary, length);
    state->mode = ZR_INF_BLOCKS;
    return ZR_OK;
}

/* Target VA 0x00192f30. */
int inflateSync_recovered(zr_stream *z)
{
    static const uint8_t mark[4] = {0, 0, 0xff, 0xff};
    zr_inflate_state *state;
    uint32_t n;
    uint8_t *p;
    uint32_t m;
    uint64_t total_in;
    uint64_t total_out;

    if (z == NULL || z->state == NULL)
        return ZR_STREAM_ERROR;
    state = (zr_inflate_state *)z->state;
    if (state->mode != ZR_INF_BAD) {
        state->mode = ZR_INF_BAD;
        state->sub.marker = 0;
    }
    n = z->avail_in;
    if (n == 0)
        return ZR_BUF_ERROR;

    p = z->next_in;
    m = state->sub.marker;
    while (n != 0 && m < 4) {
        if (*p == mark[m])
            m++;
        else if (*p != 0)
            m = 0;
        else
            m = 4u - m;
        p++;
        n--;
    }

    z->total_in += (uint64_t)(p - z->next_in);
    z->next_in = p;
    z->avail_in = n;
    state->sub.marker = m;
    if (m != 4)
        return ZR_DATA_ERROR;

    total_in = z->total_in;
    total_out = z->total_out;
    (void)inflateReset_recovered(z);
    z->total_in = total_in;
    z->total_out = total_out;
    ((zr_inflate_state *)z->state)->mode = ZR_INF_BLOCKS;
    return ZR_OK;
}

/* Target VA 0x0019304c. */
int inflateSyncPoint_recovered(zr_stream *z)
{
    zr_inflate_state *state;

    if (z == NULL || z->state == NULL)
        return ZR_STREAM_ERROR;
    state = (zr_inflate_state *)z->state;
    if (state->blocks == NULL)
        return ZR_STREAM_ERROR;
    return inflate_blocks_sync_point_recovered(state->blocks);
}
