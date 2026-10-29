/*
 * SNES Station v0.23 WIP — zlib 1.1.3 deflate engine recovery.
 *
 * Target functions:
 *   0x00190fa0 deflate
 *   0x001917b0 longest_match
 *   0x001919a4 fill_window
 *   0x00191b64 deflate_stored
 *   0x00191d4c deflate_fast
 *   0x001921a8 deflate_slow
 *
 * The binary pins the library to zlib 1.1.3. This file keeps that version's
 * algorithms and SNES Station's observed target boundaries. It is behavioral
 * recovery only, not a byte-matching build claim.
 */
#include "../../include/zlib_1_1_3_recovered.h"

#include <stdint.h>
#include <string.h>

#define ZR_INIT_STATE 42
#define ZR_BUSY_STATE 113
#define ZR_FINISH_STATE 666
#define ZR_PRESET_DICT 0x20u
#define ZR_TOO_FAR 4096u
#define ZR_NIL 0u

/* zlib block_state order in the 1.1.3 source. */
enum {
    ZR_BSTATE_NEED_MORE = 0,
    ZR_BSTATE_BLOCK_DONE = 1,
    ZR_BSTATE_FINISH_STARTED = 2,
    ZR_BSTATE_FINISH_DONE = 3,
};

extern void putShortMSB_recovered(zr_deflate_state *, uint32_t);
extern void flush_pending_recovered(zr_stream *);
extern int read_buf_recovered(zr_stream *, uint8_t *, unsigned);

static unsigned max_dist(const zr_deflate_state *s)
{
    return s->w_size - ZR_MIN_LOOKAHEAD;
}

static void clear_hash(zr_deflate_state *s)
{
    s->head[s->hash_size - 1u] = 0;
    memset(s->head, 0, (s->hash_size - 1u) * sizeof(*s->head));
}

static uint32_t insert_string(zr_deflate_state *s, uint32_t str)
{
    uint32_t head;
    s->ins_h = ((s->ins_h << s->hash_shift) ^
                s->window[str + (ZR_MIN_MATCH - 1u)]) & s->hash_mask;
    head = s->head[s->ins_h];
    s->prev[str & s->w_mask] = (zr_pos)head;
    s->head[s->ins_h] = (zr_pos)str;
    return head;
}

/* Target VA 0x001917b0. */
static uint32_t longest_match(zr_deflate_state *s, uint32_t cur_match)
{
    unsigned chain_length = s->max_chain_length;
    uint8_t *scan = s->window + s->strstart;
    int best_len = (int)s->prev_length;
    int nice_match = s->nice_match;
    uint32_t limit = s->strstart > max_dist(s) ? s->strstart - max_dist(s) : 0;
    zr_pos *prev = s->prev;
    uint32_t wmask = s->w_mask;
    uint8_t scan_end1;
    uint8_t scan_end;

    if (s->prev_length >= s->good_match)
        chain_length >>= 2;
    if ((uint32_t)nice_match > s->lookahead)
        nice_match = (int)s->lookahead;

    scan_end1 = scan[best_len - 1];
    scan_end = scan[best_len];

    do {
        uint8_t *match = s->window + cur_match;
        int len;

        if (match[best_len] != scan_end ||
            match[best_len - 1] != scan_end1 ||
            match[0] != scan[0] || match[1] != scan[1])
            goto next_match;

        len = 2;
        while (len < (int)ZR_MAX_MATCH && scan[len] == match[len])
            ++len;

        if (len > best_len) {
            s->match_start = cur_match;
            best_len = len;
            if (len >= nice_match)
                break;
            scan_end1 = scan[best_len - 1];
            scan_end = scan[best_len];
        }

next_match:
        cur_match = prev[cur_match & wmask];
    } while (cur_match > limit && --chain_length != 0);

    return (uint32_t)best_len <= s->lookahead ? (uint32_t)best_len : s->lookahead;
}

/* Target VA 0x001919a4. */
static void fill_window(zr_deflate_state *s)
{
    unsigned wsize = s->w_size;

    do {
        unsigned more = (unsigned)(s->window_size - s->lookahead - s->strstart);

        if (more == 0 && s->strstart == 0 && s->lookahead == 0) {
            more = wsize;
        } else if (more == (unsigned)-1) {
            --more;
        } else if (s->strstart >= wsize + max_dist(s)) {
            unsigned n;
            memmove(s->window, s->window + wsize, wsize);
            s->match_start -= wsize;
            s->strstart -= wsize;
            s->block_start -= (int64_t)wsize;

            for (n = 0; n < s->hash_size; ++n) {
                unsigned m = s->head[n];
                s->head[n] = (zr_pos)(m >= wsize ? m - wsize : ZR_NIL);
            }
            for (n = 0; n < wsize; ++n) {
                unsigned m = s->prev[n];
                s->prev[n] = (zr_pos)(m >= wsize ? m - wsize : ZR_NIL);
            }
            more += wsize;
        }

        if (s->strm->avail_in == 0)
            return;

        s->lookahead += (uint32_t)read_buf_recovered(
            s->strm, s->window + s->strstart + s->lookahead, more);

        if (s->lookahead >= ZR_MIN_MATCH) {
            s->ins_h = s->window[s->strstart];
            s->ins_h = ((s->ins_h << s->hash_shift) ^
                        s->window[s->strstart + 1]) & s->hash_mask;
        }
    } while (s->lookahead < ZR_MIN_LOOKAHEAD && s->strm->avail_in != 0);
}

static int flush_block_only(zr_deflate_state *s, int eof)
{
    uint8_t *buf = s->block_start >= 0 ? s->window + (unsigned)s->block_start : NULL;
    uint64_t len = (uint64_t)((int64_t)s->strstart - s->block_start);
    _tr_flush_block_recovered(s, buf, len, eof);
    s->block_start = s->strstart;
    flush_pending_recovered(s->strm);
    return s->strm->avail_out == 0;
}

/* Target VA 0x00191b64. */
int deflate_stored_00191b64(void *state, int flush)
{
    zr_deflate_state *s = (zr_deflate_state *)state;
    uint64_t max_block_size = 0xffffu;
    uint64_t max_start;

    if (max_block_size > s->pending_buf_size - 5u)
        max_block_size = s->pending_buf_size - 5u;

    for (;;) {
        if (s->lookahead <= 1) {
            fill_window(s);
            if (s->lookahead == 0 && flush == ZR_NO_FLUSH)
                return ZR_BSTATE_NEED_MORE;
            if (s->lookahead == 0)
                break;
        }

        s->strstart += s->lookahead;
        s->lookahead = 0;
        max_start = (uint64_t)s->block_start + max_block_size;
        if (s->strstart == 0 || (uint64_t)s->strstart >= max_start) {
            s->lookahead = (uint32_t)((uint64_t)s->strstart - max_start);
            s->strstart = (uint32_t)max_start;
            if (flush_block_only(s, 0))
                return ZR_BSTATE_NEED_MORE;
        }
        if (s->strstart - (uint32_t)s->block_start >= max_dist(s)) {
            if (flush_block_only(s, 0))
                return ZR_BSTATE_NEED_MORE;
        }
    }

    if (flush_block_only(s, flush == ZR_FINISH))
        return flush == ZR_FINISH ? ZR_BSTATE_FINISH_STARTED : ZR_BSTATE_NEED_MORE;
    return flush == ZR_FINISH ? ZR_BSTATE_FINISH_DONE : ZR_BSTATE_BLOCK_DONE;
}

/* Target VA 0x00191d4c. */
int deflate_fast_00191d4c(void *state, int flush)
{
    zr_deflate_state *s = (zr_deflate_state *)state;
    uint32_t hash_head = ZR_NIL;

    for (;;) {
        int bflush;
        if (s->lookahead < ZR_MIN_LOOKAHEAD) {
            fill_window(s);
            if (s->lookahead < ZR_MIN_LOOKAHEAD && flush == ZR_NO_FLUSH)
                return ZR_BSTATE_NEED_MORE;
            if (s->lookahead == 0)
                break;
        }

        if (s->lookahead >= ZR_MIN_MATCH)
            hash_head = insert_string(s, s->strstart);

        if (hash_head != ZR_NIL && s->strstart - hash_head <= max_dist(s) &&
            s->strategy != ZR_HUFFMAN_ONLY)
            s->match_length = longest_match(s, hash_head);

        if (s->match_length >= ZR_MIN_MATCH) {
            bflush = _tr_tally_recovered(s, s->strstart - s->match_start,
                                          s->match_length - ZR_MIN_MATCH);
            s->lookahead -= s->match_length;

            if (s->match_length <= s->max_lazy_match &&
                s->lookahead >= ZR_MIN_MATCH) {
                --s->match_length;
                do {
                    ++s->strstart;
                    hash_head = insert_string(s, s->strstart);
                } while (--s->match_length != 0);
                ++s->strstart;
            } else {
                s->strstart += s->match_length;
                s->match_length = 0;
                s->ins_h = s->window[s->strstart];
                s->ins_h = ((s->ins_h << s->hash_shift) ^
                            s->window[s->strstart + 1]) & s->hash_mask;
            }
        } else {
            bflush = _tr_tally_recovered(s, 0, s->window[s->strstart]);
            --s->lookahead;
            ++s->strstart;
        }

        if (bflush && flush_block_only(s, 0))
            return ZR_BSTATE_NEED_MORE;
    }

    if (flush_block_only(s, flush == ZR_FINISH))
        return flush == ZR_FINISH ? ZR_BSTATE_FINISH_STARTED : ZR_BSTATE_NEED_MORE;
    return flush == ZR_FINISH ? ZR_BSTATE_FINISH_DONE : ZR_BSTATE_BLOCK_DONE;
}

/* Target VA 0x001921a8. */
int deflate_slow_001921a8(void *state, int flush)
{
    zr_deflate_state *s = (zr_deflate_state *)state;
    uint32_t hash_head = ZR_NIL;
    int bflush = 0;

    for (;;) {
        if (s->lookahead < ZR_MIN_LOOKAHEAD) {
            fill_window(s);
            if (s->lookahead < ZR_MIN_LOOKAHEAD && flush == ZR_NO_FLUSH)
                return ZR_BSTATE_NEED_MORE;
            if (s->lookahead == 0)
                break;
        }

        if (s->lookahead >= ZR_MIN_MATCH)
            hash_head = insert_string(s, s->strstart);

        s->prev_length = s->match_length;
        s->prev_match = s->match_start;
        s->match_length = ZR_MIN_MATCH - 1u;

        if (hash_head != ZR_NIL && s->prev_length < s->max_lazy_match &&
            s->strstart - hash_head <= max_dist(s) &&
            s->strategy != ZR_HUFFMAN_ONLY) {
            s->match_length = longest_match(s, hash_head);
            if (s->match_length <= 5 &&
                (s->strategy == ZR_FILTERED ||
                 (s->match_length == ZR_MIN_MATCH &&
                  s->strstart - s->match_start > ZR_TOO_FAR)))
                s->match_length = ZR_MIN_MATCH - 1u;
        }

        if (s->prev_length >= ZR_MIN_MATCH &&
            s->match_length <= s->prev_length) {
            uint32_t max_insert = s->strstart + s->lookahead - ZR_MIN_MATCH;
            bflush = _tr_tally_recovered(s,
                    s->strstart - 1u - s->prev_match,
                    s->prev_length - ZR_MIN_MATCH);
            s->lookahead -= s->prev_length - 1u;
            s->prev_length -= 2u;
            do {
                if (++s->strstart <= max_insert)
                    hash_head = insert_string(s, s->strstart);
            } while (--s->prev_length != 0);
            s->match_available = 0;
            s->match_length = ZR_MIN_MATCH - 1u;
            ++s->strstart;
            if (bflush && flush_block_only(s, 0))
                return ZR_BSTATE_NEED_MORE;
        } else if (s->match_available) {
            bflush = _tr_tally_recovered(s, 0, s->window[s->strstart - 1u]);
            if (bflush)
                (void)flush_block_only(s, 0);
            ++s->strstart;
            --s->lookahead;
            if (s->strm->avail_out == 0)
                return ZR_BSTATE_NEED_MORE;
        } else {
            s->match_available = 1;
            ++s->strstart;
            --s->lookahead;
        }
    }

    if (s->match_available) {
        (void)_tr_tally_recovered(s, 0, s->window[s->strstart - 1u]);
        s->match_available = 0;
    }

    if (flush_block_only(s, flush == ZR_FINISH))
        return flush == ZR_FINISH ? ZR_BSTATE_FINISH_STARTED : ZR_BSTATE_NEED_MORE;
    return flush == ZR_FINISH ? ZR_BSTATE_FINISH_DONE : ZR_BSTATE_BLOCK_DONE;
}

static int (*const zr_compressors[10])(void *, int) = {
    deflate_stored_00191b64,
    deflate_fast_00191d4c, deflate_fast_00191d4c, deflate_fast_00191d4c,
    deflate_slow_001921a8, deflate_slow_001921a8, deflate_slow_001921a8,
    deflate_slow_001921a8, deflate_slow_001921a8, deflate_slow_001921a8
};

/* Target VA 0x00190fa0. */
int deflate_00190fa0(zr_stream *strm, int flush)
{
    zr_deflate_state *s;
    int old_flush;

    if (strm == NULL || strm->state == NULL || flush > ZR_FINISH || flush < 0)
        return ZR_STREAM_ERROR;
    s = (zr_deflate_state *)strm->state;

    if (strm->next_out == NULL ||
        (strm->next_in == NULL && strm->avail_in != 0) ||
        (s->status == ZR_FINISH_STATE && flush != ZR_FINISH))
        return ZR_STREAM_ERROR;
    if (strm->avail_out == 0)
        return ZR_BUF_ERROR;

    s->strm = strm;
    old_flush = s->last_flush;
    s->last_flush = flush;

    if (s->status == ZR_INIT_STATE) {
        uint32_t header = (ZR_DEFLATED + ((s->w_bits - 8u) << 4)) << 8;
        uint32_t level_flags = (uint32_t)(s->level - 1) >> 1;
        if (level_flags > 3)
            level_flags = 3;
        header |= level_flags << 6;
        if (s->strstart != 0)
            header |= ZR_PRESET_DICT;
        header += 31u - (header % 31u);
        s->status = ZR_BUSY_STATE;
        putShortMSB_recovered(s, header);
        if (s->strstart != 0) {
            putShortMSB_recovered(s, (uint32_t)(strm->adler >> 16));
            putShortMSB_recovered(s, (uint32_t)(strm->adler & 0xffffu));
        }
        strm->adler = 1;
    }

    if (s->pending != 0) {
        flush_pending_recovered(strm);
        if (strm->avail_out == 0) {
            s->last_flush = -1;
            return ZR_OK;
        }
    } else if (strm->avail_in == 0 && flush <= old_flush && flush != ZR_FINISH) {
        return ZR_BUF_ERROR;
    }

    if (s->status == ZR_FINISH_STATE && strm->avail_in != 0)
        return ZR_BUF_ERROR;

    if (strm->avail_in != 0 || s->lookahead != 0 ||
        (flush != ZR_NO_FLUSH && s->status != ZR_FINISH_STATE)) {
        int bstate = zr_compressors[s->level](s, flush);
        if (bstate == ZR_BSTATE_FINISH_STARTED || bstate == ZR_BSTATE_FINISH_DONE)
            s->status = ZR_FINISH_STATE;
        if (bstate == ZR_BSTATE_NEED_MORE || bstate == ZR_BSTATE_FINISH_STARTED) {
            if (strm->avail_out == 0)
                s->last_flush = -1;
            return ZR_OK;
        }
        if (bstate == ZR_BSTATE_BLOCK_DONE) {
            if (flush == ZR_PARTIAL_FLUSH) {
                _tr_align_recovered(s);
            } else {
                _tr_stored_block_recovered(s, NULL, 0, 0);
                if (flush == ZR_FULL_FLUSH)
                    clear_hash(s);
            }
            flush_pending_recovered(strm);
            if (strm->avail_out == 0) {
                s->last_flush = -1;
                return ZR_OK;
            }
        }
    }

    if (flush != ZR_FINISH)
        return ZR_OK;
    if (s->noheader)
        return ZR_STREAM_END;

    putShortMSB_recovered(s, (uint32_t)(strm->adler >> 16));
    putShortMSB_recovered(s, (uint32_t)(strm->adler & 0xffffu));
    flush_pending_recovered(strm);
    s->noheader = -1;
    return s->pending != 0 ? ZR_OK : ZR_STREAM_END;
}
