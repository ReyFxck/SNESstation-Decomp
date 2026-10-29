/*
 * SNES Station v0.23 WIP — zlib 1.1.3 DEFLATE block state machine recovery.
 *
 * Target VA: 0x001947d0 inflate_blocks
 *
 * This is the large TYPE/LENS/STORED/TABLE/BTREE/DTREE/CODES/DRY/DONE/BAD
 * state machine between inflate_blocks_new and inflate_blocks_free.
 */
#include "../../include/zlib_1_1_3_recovered.h"

#include <string.h>

static const uint32_t border[19] = {
    16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15
};

static void *zr_alloc(zr_stream *z, unsigned items, unsigned size)
{
    return z->zalloc(z->opaque, items, size);
}

static void zr_free(zr_stream *z, void *p)
{
    if (p != NULL)
        z->zfree(z->opaque, p);
}

/* Target VA 0x001947d0. */
int inflate_blocks_recovered(zr_inflate_blocks_state *s, zr_stream *z, int r)
{
    uint32_t t;
    uint64_t b = s->bitb;
    uint32_t k = s->bitk;
    uint8_t *p = z->next_in;
    uint32_t n = z->avail_in;
    uint8_t *q = s->write;
    uint32_t m = (uint32_t)(q < s->read ? s->read - q - 1 : s->end - q);

#define ZR_UPDATE() do { \
    s->bitb = b; s->bitk = k; \
    z->avail_in = n; z->total_in += (uint64_t)(p - z->next_in); z->next_in = p; \
    s->write = q; \
} while (0)
#define ZR_LEAVE() do { ZR_UPDATE(); return inflate_flush_recovered(s, z, r); } while (0)
#define ZR_NEEDBYTE() do { if (n != 0) r = ZR_OK; else ZR_LEAVE(); } while (0)
#define ZR_NEXTBYTE() (n--, *p++)
#define ZR_NEEDBITS(x) do { while (k < (uint32_t)(x)) { \
    uint8_t zr_b; ZR_NEEDBYTE(); zr_b = ZR_NEXTBYTE(); b |= (uint64_t)zr_b << k; k += 8; \
} } while (0)
#define ZR_DUMPBITS(x) do { b >>= (x); k -= (uint32_t)(x); } while (0)
#define ZR_WRAP() do { if (q == s->end && s->read != s->window) { \
    q = s->window; m = (uint32_t)(q < s->read ? s->read - q - 1 : s->end - q); \
} } while (0)
#define ZR_FLUSH() do { s->write = q; r = inflate_flush_recovered(s, z, r); \
    q = s->write; m = (uint32_t)(q < s->read ? s->read - q - 1 : s->end - q); \
} while (0)
#define ZR_NEEDOUT() do { if (m == 0) { ZR_WRAP(); if (m == 0) { ZR_FLUSH(); ZR_WRAP(); if (m == 0) ZR_LEAVE(); } } r = ZR_OK; } while (0)

    for (;;) {
        switch (s->mode) {
        case ZR_BLOCK_TYPE:
            ZR_NEEDBITS(3);
            t = (uint32_t)b & 7u;
            s->last = t & 1u;
            switch (t >> 1) {
            case 0:
                ZR_DUMPBITS(3);
                t = k & 7u;
                ZR_DUMPBITS(t);
                s->mode = ZR_BLOCK_LENS;
                break;
            case 1: {
                uint32_t bl, bd;
                zr_inflate_huft *tl, *td;
                inflate_trees_fixed_recovered(&bl, &bd, &tl, &td, z);
                s->sub.decode.codes = inflate_codes_new_recovered(bl, bd, tl, td, z);
                if (s->sub.decode.codes == NULL) {
                    r = ZR_MEM_ERROR;
                    ZR_LEAVE();
                }
                ZR_DUMPBITS(3);
                s->mode = ZR_BLOCK_CODES;
                break;
            }
            case 2:
                ZR_DUMPBITS(3);
                s->mode = ZR_BLOCK_TABLE;
                break;
            default:
                ZR_DUMPBITS(3);
                s->mode = ZR_BLOCK_BAD;
                z->msg = (char *)"invalid block type";
                r = ZR_DATA_ERROR;
                ZR_LEAVE();
            }
            break;

        case ZR_BLOCK_LENS:
            ZR_NEEDBITS(32);
            if ((((~b) >> 16) & 0xffffu) != (b & 0xffffu)) {
                s->mode = ZR_BLOCK_BAD;
                z->msg = (char *)"invalid stored block lengths";
                r = ZR_DATA_ERROR;
                ZR_LEAVE();
            }
            s->sub.left = (uint32_t)b & 0xffffu;
            b = 0;
            k = 0;
            s->mode = s->sub.left ? ZR_BLOCK_STORED :
                      (s->last ? ZR_BLOCK_DRY : ZR_BLOCK_TYPE);
            break;

        case ZR_BLOCK_STORED:
            if (n == 0)
                ZR_LEAVE();
            ZR_NEEDOUT();
            t = s->sub.left;
            if (t > n)
                t = n;
            if (t > m)
                t = m;
            memmove(q, p, t);
            p += t;
            n -= t;
            q += t;
            m -= t;
            s->sub.left -= t;
            if (s->sub.left != 0)
                break;
            s->mode = s->last ? ZR_BLOCK_DRY : ZR_BLOCK_TYPE;
            break;

        case ZR_BLOCK_TABLE:
            ZR_NEEDBITS(14);
            s->sub.trees.table = t = (uint32_t)b & 0x3fffu;
            if ((t & 0x1fu) > 29u || ((t >> 5) & 0x1fu) > 29u) {
                s->mode = ZR_BLOCK_BAD;
                z->msg = (char *)"too many length or distance symbols";
                r = ZR_DATA_ERROR;
                ZR_LEAVE();
            }
            t = 258u + (t & 0x1fu) + ((t >> 5) & 0x1fu);
            s->sub.trees.blens = (uint32_t *)zr_alloc(z, t, sizeof(uint32_t));
            if (s->sub.trees.blens == NULL) {
                r = ZR_MEM_ERROR;
                ZR_LEAVE();
            }
            ZR_DUMPBITS(14);
            s->sub.trees.index = 0;
            s->mode = ZR_BLOCK_BTREE;
            /* fall through */

        case ZR_BLOCK_BTREE:
            while (s->sub.trees.index < 4u + (s->sub.trees.table >> 10)) {
                ZR_NEEDBITS(3);
                s->sub.trees.blens[border[s->sub.trees.index++]] =
                    (uint32_t)b & 7u;
                ZR_DUMPBITS(3);
            }
            while (s->sub.trees.index < 19)
                s->sub.trees.blens[border[s->sub.trees.index++]] = 0;
            s->sub.trees.bb = 7;
            t = (uint32_t)inflate_trees_bits_recovered(
                s->sub.trees.blens, &s->sub.trees.bb, &s->sub.trees.tb,
                s->hufts, z);
            if ((int)t != ZR_OK) {
                zr_free(z, s->sub.trees.blens);
                s->sub.trees.blens = NULL;
                r = (int)t;
                if (r == ZR_DATA_ERROR)
                    s->mode = ZR_BLOCK_BAD;
                ZR_LEAVE();
            }
            s->sub.trees.index = 0;
            s->mode = ZR_BLOCK_DTREE;
            /* fall through */

        case ZR_BLOCK_DTREE:
            for (;;) {
                uint32_t total;
                zr_inflate_huft *h;
                uint32_t i, j, c;

                t = s->sub.trees.table;
                total = 258u + (t & 0x1fu) + ((t >> 5) & 0x1fu);
                if (s->sub.trees.index >= total)
                    break;

                t = s->sub.trees.bb;
                ZR_NEEDBITS(t);
                h = s->sub.trees.tb + ((uint32_t)b & zr_inflate_mask[t]);
                t = h->bits;
                c = h->base;
                if (c < 16) {
                    ZR_DUMPBITS(t);
                    s->sub.trees.blens[s->sub.trees.index++] = c;
                } else {
                    i = c == 18 ? 7u : c - 14u;
                    j = c == 18 ? 11u : 3u;
                    ZR_NEEDBITS(t + i);
                    ZR_DUMPBITS(t);
                    j += (uint32_t)b & zr_inflate_mask[i];
                    ZR_DUMPBITS(i);
                    i = s->sub.trees.index;
                    if (i + j > total || (c == 16 && i < 1)) {
                        zr_free(z, s->sub.trees.blens);
                        s->sub.trees.blens = NULL;
                        s->mode = ZR_BLOCK_BAD;
                        z->msg = (char *)"invalid bit length repeat";
                        r = ZR_DATA_ERROR;
                        ZR_LEAVE();
                    }
                    c = c == 16 ? s->sub.trees.blens[i - 1] : 0;
                    do {
                        s->sub.trees.blens[i++] = c;
                    } while (--j != 0);
                    s->sub.trees.index = i;
                }
            }

            s->sub.trees.tb = NULL;
            {
                uint32_t bl = 9, bd = 6;
                zr_inflate_huft *tl, *td;
                zr_inflate_codes_state *codes;
                t = s->sub.trees.table;
                r = inflate_trees_dynamic_recovered(
                    257u + (t & 0x1fu), 1u + ((t >> 5) & 0x1fu),
                    s->sub.trees.blens, &bl, &bd, &tl, &td, s->hufts, z);
                zr_free(z, s->sub.trees.blens);
                s->sub.trees.blens = NULL;
                if (r != ZR_OK) {
                    if (r == ZR_DATA_ERROR)
                        s->mode = ZR_BLOCK_BAD;
                    ZR_LEAVE();
                }
                codes = inflate_codes_new_recovered(bl, bd, tl, td, z);
                if (codes == NULL) {
                    r = ZR_MEM_ERROR;
                    ZR_LEAVE();
                }
                s->sub.decode.codes = codes;
            }
            s->mode = ZR_BLOCK_CODES;
            /* fall through */

        case ZR_BLOCK_CODES:
            ZR_UPDATE();
            r = inflate_codes_recovered(s, z, r);
            if (r != ZR_STREAM_END)
                return inflate_flush_recovered(s, z, r);
            r = ZR_OK;
            inflate_codes_free_recovered(s->sub.decode.codes, z);
            s->sub.decode.codes = NULL;
            p = z->next_in;
            n = z->avail_in;
            b = s->bitb;
            k = s->bitk;
            q = s->write;
            m = (uint32_t)(q < s->read ? s->read - q - 1 : s->end - q);
            if (!s->last) {
                s->mode = ZR_BLOCK_TYPE;
                break;
            }
            s->mode = ZR_BLOCK_DRY;
            /* fall through */

        case ZR_BLOCK_DRY:
            ZR_FLUSH();
            if (s->read != s->write)
                ZR_LEAVE();
            s->mode = ZR_BLOCK_DONE;
            /* fall through */

        case ZR_BLOCK_DONE:
            r = ZR_STREAM_END;
            ZR_LEAVE();

        case ZR_BLOCK_BAD:
            r = ZR_DATA_ERROR;
            ZR_LEAVE();

        default:
            r = ZR_STREAM_ERROR;
            ZR_LEAVE();
        }
    }

#undef ZR_UPDATE
#undef ZR_LEAVE
#undef ZR_NEEDBYTE
#undef ZR_NEXTBYTE
#undef ZR_NEEDBITS
#undef ZR_DUMPBITS
#undef ZR_WRAP
#undef ZR_FLUSH
#undef ZR_NEEDOUT
}
