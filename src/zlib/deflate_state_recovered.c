/*
 * SNES Station v0.23 WIP — zlib 1.1.3 deflate front-end recovery.
 *
 * Target functions reconstructed here:
 *   0x001908ec deflateInit2_
 *   0x00190bb4 deflateSetDictionary
 *   0x00190d10 deflateReset
 *   0x00190dc0 deflateParams
 *   0x00190ec8 putShortMSB
 *   0x00190ef8 flush_pending
 *   0x00191308 deflateEnd
 *   0x00191400 deflateCopy
 *   0x0019165c read_buf
 *   0x0019170c lm_init
 *
 * The exact zlib 1.1.3 baseline is independently pinned by strings in the
 * target. Historical PS2 listings are used only to validate function order,
 * frames and control flow; this file is not claimed MATCHING.
 */
#include "../../include/zlib_1_1_3_recovered.h"

#include <stdlib.h>
#include <string.h>

#define ZR_INIT_STATE 42
#define ZR_BUSY_STATE 113
#define ZR_FINISH_STATE 666

extern int deflate_00190fa0(zr_stream *, int);
extern int deflateEnd_00191308(zr_stream *);
extern int deflateReset_00190d10(zr_stream *);

/* Large compression engines are mapped but not reconstructed in this file. */
extern int deflate_stored_00191b64(void *, int);
extern int deflate_fast_00191d4c(void *, int);
extern int deflate_slow_001921a8(void *, int);
void lm_init_recovered(zr_deflate_state *s);

static const zr_config configuration_table_recovered[10] = {
    {0, 0, 0, 0, deflate_stored_00191b64},
    {4, 4, 8, 4, deflate_fast_00191d4c},
    {4, 5, 16, 8, deflate_fast_00191d4c},
    {4, 6, 32, 32, deflate_fast_00191d4c},
    {4, 4, 16, 16, deflate_slow_001921a8},
    {8, 16, 32, 32, deflate_slow_001921a8},
    {8, 16, 128, 128, deflate_slow_001921a8},
    {8, 32, 128, 256, deflate_slow_001921a8},
    {32, 128, 258, 1024, deflate_slow_001921a8},
    {32, 258, 258, 4096, deflate_slow_001921a8},
};

static void *zr_alloc(zr_stream *strm, unsigned items, unsigned size)
{
    return strm->zalloc(strm->opaque, items, size);
}

static void zr_free(zr_stream *strm, void *p)
{
    if (p != NULL)
        strm->zfree(strm->opaque, p);
}

/* Target VA 0x001908ec. */
int deflateInit2__recovered(zr_stream *strm, int level, int method,
                            int window_bits, int mem_level, int strategy,
                            const char *version, int stream_size)
{
    zr_deflate_state *s;
    int noheader = 0;
    uint16_t *overlay;

    if (version == NULL || version[0] != ZR_VERSION[0] ||
        stream_size != ZR_TARGET_STREAM_SIZE)
        return ZR_VERSION_ERROR;
    if (strm == NULL)
        return ZR_STREAM_ERROR;

    strm->msg = NULL;
    if (strm->zalloc == NULL) {
        strm->zalloc = zcalloc_00198a84;
        strm->opaque = NULL;
    }
    if (strm->zfree == NULL)
        strm->zfree = zcfree_00198aa4;

    if (level == ZR_DEFAULT_COMPRESSION)
        level = 6;

    if (window_bits < 0) {
        noheader = 1;
        window_bits = -window_bits;
    }

    if (mem_level < 1 || mem_level > ZR_MAX_MEM_LEVEL ||
        method != ZR_DEFLATED || window_bits < 8 || window_bits > 15 ||
        level < 0 || level > 9 || strategy < 0 || strategy > ZR_HUFFMAN_ONLY)
        return ZR_STREAM_ERROR;

    s = (zr_deflate_state *)zr_alloc(strm, 1, ZR_TARGET_DEFLATE_STATE_SIZE);
    if (s == NULL)
        return ZR_MEM_ERROR;
    memset(s, 0, sizeof(*s));
    strm->state = s;
    s->strm = strm;
    s->noheader = noheader;
    s->w_bits = (uint32_t)window_bits;
    s->w_size = 1u << s->w_bits;
    s->w_mask = s->w_size - 1u;
    s->hash_bits = (uint32_t)mem_level + 7u;
    s->hash_size = 1u << s->hash_bits;
    s->hash_mask = s->hash_size - 1u;
    s->hash_shift = (s->hash_bits + ZR_MIN_MATCH - 1u) / ZR_MIN_MATCH;

    s->window = (uint8_t *)zr_alloc(strm, s->w_size, 2u);
    s->prev = (zr_pos *)zr_alloc(strm, s->w_size, sizeof(zr_pos));
    s->head = (zr_pos *)zr_alloc(strm, s->hash_size, sizeof(zr_pos));

    s->lit_bufsize = 1u << ((uint32_t)mem_level + 6u);
    overlay = (uint16_t *)zr_alloc(strm, s->lit_bufsize,
                                   (unsigned)(sizeof(uint16_t) + 2u));
    s->pending_buf = (uint8_t *)overlay;
    s->pending_buf_size = (uint64_t)s->lit_bufsize * 4u;

    if (s->window == NULL || s->prev == NULL || s->head == NULL ||
        s->pending_buf == NULL) {
        strm->msg = (char *)"insufficient memory";
        (void)deflateEnd_00191308(strm);
        return ZR_MEM_ERROR;
    }

    s->d_buf = overlay + s->lit_bufsize / sizeof(uint16_t);
    s->l_buf = s->pending_buf + (1u + sizeof(uint16_t)) * s->lit_bufsize;
    s->level = level;
    s->strategy = strategy;
    s->method = (uint8_t)method;

    return deflateReset_00190d10(strm);
}

/* Target VA 0x00190bb4. */
int deflateSetDictionary_recovered(zr_stream *strm, const uint8_t *dictionary,
                                   uint32_t dict_length)
{
    zr_deflate_state *s;
    uint32_t length = dict_length;
    uint32_t n;
    uint32_t hash_head = 0;

    if (strm == NULL || strm->state == NULL || dictionary == NULL)
        return ZR_STREAM_ERROR;
    s = (zr_deflate_state *)strm->state;
    if (s->status != ZR_INIT_STATE)
        return ZR_STREAM_ERROR;

    strm->adler = adler32_recovered(strm->adler, dictionary, dict_length);
    if (length < ZR_MIN_MATCH)
        return ZR_OK;

    if (length > s->w_size - ZR_MIN_LOOKAHEAD) {
        length = s->w_size - ZR_MIN_LOOKAHEAD;
        dictionary += dict_length - length;
    }

    memmove(s->window, dictionary, length);
    s->strstart = length;
    s->block_start = length;
    s->ins_h = s->window[0];
    s->ins_h = ((s->ins_h << s->hash_shift) ^ s->window[1]) & s->hash_mask;

    for (n = 0; n <= length - ZR_MIN_MATCH; n++) {
        s->ins_h = ((s->ins_h << s->hash_shift) ^
                    s->window[n + (ZR_MIN_MATCH - 1u)]) & s->hash_mask;
        s->prev[n & s->w_mask] = (zr_pos)(hash_head = s->head[s->ins_h]);
        s->head[s->ins_h] = (zr_pos)n;
    }
    (void)hash_head;
    return ZR_OK;
}

/* Target VA 0x00190d10. */
int deflateReset_recovered(zr_stream *strm)
{
    zr_deflate_state *s;

    if (strm == NULL || strm->state == NULL ||
        strm->zalloc == NULL || strm->zfree == NULL)
        return ZR_STREAM_ERROR;

    strm->total_in = 0;
    strm->total_out = 0;
    strm->msg = NULL;
    strm->data_type = ZR_UNKNOWN;

    s = (zr_deflate_state *)strm->state;
    s->pending = 0;
    s->pending_out = s->pending_buf;
    if (s->noheader < 0)
        s->noheader = 0;
    s->status = s->noheader ? ZR_BUSY_STATE : ZR_INIT_STATE;
    strm->adler = 1;
    s->last_flush = ZR_NO_FLUSH;

    _tr_init_recovered(s);
    lm_init_recovered(s);
    return ZR_OK;
}

/* Target VA 0x00190dc0. */
int deflateParams_recovered(zr_stream *strm, int level, int strategy)
{
    zr_deflate_state *s;
    zr_compress_func old_func;
    int err = ZR_OK;

    if (strm == NULL || strm->state == NULL)
        return ZR_STREAM_ERROR;
    s = (zr_deflate_state *)strm->state;

    if (level == ZR_DEFAULT_COMPRESSION)
        level = 6;
    if (level < 0 || level > 9 || strategy < 0 || strategy > ZR_HUFFMAN_ONLY)
        return ZR_STREAM_ERROR;

    old_func = configuration_table_recovered[s->level].func;
    if (old_func != configuration_table_recovered[level].func &&
        strm->total_in != 0)
        err = deflate_00190fa0(strm, ZR_PARTIAL_FLUSH);

    if (s->level != level) {
        s->level = level;
        s->max_lazy_match = configuration_table_recovered[level].max_lazy;
        s->good_match = configuration_table_recovered[level].good_length;
        s->nice_match = configuration_table_recovered[level].nice_length;
        s->max_chain_length = configuration_table_recovered[level].max_chain;
    }
    s->strategy = strategy;
    return err;
}

/* Target VA 0x00190ec8. */
void putShortMSB_recovered(zr_deflate_state *s, uint32_t value)
{
    s->pending_buf[s->pending++] = (uint8_t)(value >> 8);
    s->pending_buf[s->pending++] = (uint8_t)value;
}

/* Target VA 0x00190ef8. */
void flush_pending_recovered(zr_stream *strm)
{
    zr_deflate_state *s = (zr_deflate_state *)strm->state;
    uint32_t len = (uint32_t)s->pending;

    if (len > strm->avail_out)
        len = strm->avail_out;
    if (len == 0)
        return;

    memmove(strm->next_out, s->pending_out, len);
    strm->next_out += len;
    s->pending_out += len;
    strm->total_out += len;
    strm->avail_out -= len;
    s->pending -= (int)len;
    if (s->pending == 0)
        s->pending_out = s->pending_buf;
}

/* Target VA 0x00191308. */
int deflateEnd_recovered(zr_stream *strm)
{
    zr_deflate_state *s;
    int status;

    if (strm == NULL || strm->state == NULL)
        return ZR_STREAM_ERROR;
    s = (zr_deflate_state *)strm->state;
    status = s->status;
    if (status != ZR_INIT_STATE && status != ZR_BUSY_STATE &&
        status != ZR_FINISH_STATE)
        return ZR_STREAM_ERROR;

    zr_free(strm, s->pending_buf);
    zr_free(strm, s->head);
    zr_free(strm, s->prev);
    zr_free(strm, s->window);
    zr_free(strm, s);
    strm->state = NULL;

    return status == ZR_BUSY_STATE ? ZR_DATA_ERROR : ZR_OK;
}

/* Target VA 0x00191400. */
int deflateCopy_recovered(zr_stream *dest, zr_stream *source)
{
    zr_deflate_state *ds;
    zr_deflate_state *ss;
    uint16_t *overlay;

    if (source == NULL || dest == NULL || source->state == NULL)
        return ZR_STREAM_ERROR;
    ss = (zr_deflate_state *)source->state;
    *dest = *source;

    ds = (zr_deflate_state *)zr_alloc(dest, 1, ZR_TARGET_DEFLATE_STATE_SIZE);
    if (ds == NULL)
        return ZR_MEM_ERROR;
    memcpy(ds, ss, sizeof(*ds));
    dest->state = ds;
    ds->strm = dest;

    ds->window = (uint8_t *)zr_alloc(dest, ds->w_size, 2u);
    ds->prev = (zr_pos *)zr_alloc(dest, ds->w_size, sizeof(zr_pos));
    ds->head = (zr_pos *)zr_alloc(dest, ds->hash_size, sizeof(zr_pos));
    overlay = (uint16_t *)zr_alloc(dest, ds->lit_bufsize,
                                   (unsigned)(sizeof(uint16_t) + 2u));
    ds->pending_buf = (uint8_t *)overlay;

    if (ds->window == NULL || ds->prev == NULL || ds->head == NULL ||
        ds->pending_buf == NULL) {
        (void)deflateEnd_recovered(dest);
        return ZR_MEM_ERROR;
    }

    memmove(ds->window, ss->window, ds->w_size * 2u);
    memmove(ds->prev, ss->prev, ds->w_size * sizeof(zr_pos));
    memmove(ds->head, ss->head, ds->hash_size * sizeof(zr_pos));
    memmove(ds->pending_buf, ss->pending_buf, (size_t)ds->pending_buf_size);

    ds->pending_out = ds->pending_buf + (ss->pending_out - ss->pending_buf);
    ds->d_buf = overlay + ds->lit_bufsize / sizeof(uint16_t);
    ds->l_buf = ds->pending_buf + (1u + sizeof(uint16_t)) * ds->lit_bufsize;
    ds->l_desc.dyn_tree = ds->dyn_ltree;
    ds->d_desc.dyn_tree = ds->dyn_dtree;
    ds->bl_desc.dyn_tree = ds->bl_tree;
    return ZR_OK;
}

/* Target VA 0x0019165c. */
int read_buf_recovered(zr_stream *strm, uint8_t *buf, unsigned size)
{
    unsigned len = strm->avail_in;

    if (len > size)
        len = size;
    if (len == 0)
        return 0;

    strm->avail_in -= len;
    if (!((zr_deflate_state *)strm->state)->noheader)
        strm->adler = adler32_recovered(strm->adler, strm->next_in, len);
    memmove(buf, strm->next_in, len);
    strm->next_in += len;
    strm->total_in += len;
    return (int)len;
}

/* Target VA 0x0019170c. */
void lm_init_recovered(zr_deflate_state *s)
{
    const zr_config *cfg = &configuration_table_recovered[s->level];

    s->window_size = (uint64_t)2u * s->w_size;
    if (s->hash_size != 0) {
        s->head[s->hash_size - 1u] = 0;
        memset(s->head, 0, (s->hash_size - 1u) * sizeof(*s->head));
    }

    s->max_lazy_match = cfg->max_lazy;
    s->good_match = cfg->good_length;
    s->nice_match = cfg->nice_length;
    s->max_chain_length = cfg->max_chain;
    s->strstart = 0;
    s->block_start = 0;
    s->lookahead = 0;
    s->match_length = ZR_MIN_MATCH - 1u;
    s->prev_length = ZR_MIN_MATCH - 1u;
    s->match_available = 0;
    s->ins_h = 0;
}
