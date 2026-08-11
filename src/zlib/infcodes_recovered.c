/*
 * SNES Station v0.23 WIP — zlib 1.1.3 inflate code decoder recovery.
 *
 * Target VAs:
 *   0x0019533c inflate_codes_new
 *   0x001953ac inflate_codes
 *   0x00195b10 inflate_codes_free
 *
 * The target matches the classic zlib 1.1.3 ten-state literal/length and
 * distance decoder. This is a behavioral reconstruction, not a MATCHING build.
 */
#include "../../include/zlib_1_1_3_recovered.h"

static void *zr_alloc(zr_stream *z, unsigned items, unsigned size)
{
    return z->zalloc(z->opaque, items, size);
}

static void zr_free(zr_stream *z, void *p)
{
    if (p != NULL)
        z->zfree(z->opaque, p);
}

/* Target VA 0x0019533c. */
zr_inflate_codes_state *inflate_codes_new_recovered(
    unsigned bl, unsigned bd, zr_inflate_huft *tl, zr_inflate_huft *td,
    zr_stream *z)
{
    zr_inflate_codes_state *c;

    c = (zr_inflate_codes_state *)zr_alloc(z, 1, 0x1cu);
    if (c != NULL) {
        c->mode = ZR_CODE_START;
        c->lbits = (uint8_t)bl;
        c->dbits = (uint8_t)bd;
        c->ltree = tl;
        c->dtree = td;
    }
    return c;
}

#define ZR_UPDATE() \
    do { \
        s->bitb = b; \
        s->bitk = k; \
        z->avail_in = n; \
        z->total_in += (uint64_t)(p - z->next_in); \
        z->next_in = p; \
        s->write = q; \
    } while (0)

#define ZR_LEAVE() \
    do { \
        ZR_UPDATE(); \
        return inflate_flush_recovered(s, z, r); \
    } while (0)

#define ZR_NEEDBITS(j) \
    do { \
        while (k < (j)) { \
            if (n == 0) \
                ZR_LEAVE(); \
            r = ZR_OK; \
            n--; \
            b |= (uint64_t)(*p++) << k; \
            k += 8; \
        } \
    } while (0)

#define ZR_DUMPBITS(j) \
    do { \
        b >>= (j); \
        k -= (j); \
    } while (0)

#define ZR_RELOAD_OUT() \
    do { \
        q = s->write; \
        m = (uint32_t)(q < s->read ? s->read - q - 1 : s->end - q); \
    } while (0)

#define ZR_WRAP_OUT() \
    do { \
        if (q == s->end && s->read != s->window) { \
            q = s->window; \
            m = (uint32_t)(q < s->read ? s->read - q - 1 : s->end - q); \
        } \
    } while (0)

#define ZR_FLUSH_OUT() \
    do { \
        s->write = q; \
        r = inflate_flush_recovered(s, z, r); \
        ZR_RELOAD_OUT(); \
    } while (0)

#define ZR_NEEDOUT() \
    do { \
        if (m == 0) { \
            ZR_WRAP_OUT(); \
            if (m == 0) { \
                ZR_FLUSH_OUT(); \
                ZR_WRAP_OUT(); \
                if (m == 0) \
                    ZR_LEAVE(); \
            } \
        } \
        r = ZR_OK; \
    } while (0)

/* Target VA 0x001953ac. */
int inflate_codes_recovered(zr_inflate_blocks_state *s, zr_stream *z, int r)
{
    uint32_t j;
    zr_inflate_huft *t;
    uint32_t e;
    uint64_t b = s->bitb;
    uint32_t k = s->bitk;
    uint8_t *p = z->next_in;
    uint32_t n = z->avail_in;
    uint8_t *q = s->write;
    uint32_t m = (uint32_t)(q < s->read ? s->read - q - 1 : s->end - q);
    uint8_t *f;
    zr_inflate_codes_state *c = s->sub.decode.codes;

    for (;;) {
        switch (c->mode) {
        case ZR_CODE_START:
            if (m >= 258 && n >= 10) {
                ZR_UPDATE();
                r = inflate_fast_recovered(c->lbits, c->dbits, c->ltree,
                                           c->dtree, s, z);
                b = s->bitb;
                k = s->bitk;
                p = z->next_in;
                n = z->avail_in;
                ZR_RELOAD_OUT();
                if (r != ZR_OK) {
                    c->mode = r == ZR_STREAM_END ? ZR_CODE_WASH :
                                                   ZR_CODE_BADCODE;
                    break;
                }
            }
            c->sub.code.need = c->lbits;
            c->sub.code.tree = c->ltree;
            c->mode = ZR_CODE_LEN;
            /* fall through */

        case ZR_CODE_LEN:
            j = c->sub.code.need;
            ZR_NEEDBITS(j);
            t = c->sub.code.tree + ((uint32_t)b & zr_inflate_mask[j]);
            ZR_DUMPBITS(t->bits);
            e = t->exop;
            if (e == 0) {
                c->sub.lit = t->base;
                c->mode = ZR_CODE_LIT;
                break;
            }
            if (e & 16u) {
                c->sub.copy.get = e & 15u;
                c->len = t->base;
                c->mode = ZR_CODE_LENEXT;
                break;
            }
            if ((e & 64u) == 0) {
                c->sub.code.need = e;
                c->sub.code.tree = t + t->base;
                break;
            }
            if (e & 32u) {
                c->mode = ZR_CODE_WASH;
                break;
            }
            c->mode = ZR_CODE_BADCODE;
            z->msg = (char *)"invalid literal/length code";
            r = ZR_DATA_ERROR;
            ZR_LEAVE();

        case ZR_CODE_LENEXT:
            j = c->sub.copy.get;
            ZR_NEEDBITS(j);
            c->len += (uint32_t)b & zr_inflate_mask[j];
            ZR_DUMPBITS(j);
            c->sub.code.need = c->dbits;
            c->sub.code.tree = c->dtree;
            c->mode = ZR_CODE_DIST;
            /* fall through */

        case ZR_CODE_DIST:
            j = c->sub.code.need;
            ZR_NEEDBITS(j);
            t = c->sub.code.tree + ((uint32_t)b & zr_inflate_mask[j]);
            ZR_DUMPBITS(t->bits);
            e = t->exop;
            if (e & 16u) {
                c->sub.copy.get = e & 15u;
                c->sub.copy.dist = t->base;
                c->mode = ZR_CODE_DISTEXT;
                break;
            }
            if ((e & 64u) == 0) {
                c->sub.code.need = e;
                c->sub.code.tree = t + t->base;
                break;
            }
            c->mode = ZR_CODE_BADCODE;
            z->msg = (char *)"invalid distance code";
            r = ZR_DATA_ERROR;
            ZR_LEAVE();

        case ZR_CODE_DISTEXT:
            j = c->sub.copy.get;
            ZR_NEEDBITS(j);
            c->sub.copy.dist += (uint32_t)b & zr_inflate_mask[j];
            ZR_DUMPBITS(j);
            c->mode = ZR_CODE_COPY;
            /* fall through */

        case ZR_CODE_COPY:
            if ((uint32_t)(q - s->window) < c->sub.copy.dist)
                f = s->end - (c->sub.copy.dist - (uint32_t)(q - s->window));
            else
                f = q - c->sub.copy.dist;
            while (c->len != 0) {
                ZR_NEEDOUT();
                *q++ = *f++;
                m--;
                if (f == s->end)
                    f = s->window;
                c->len--;
            }
            c->mode = ZR_CODE_START;
            break;

        case ZR_CODE_LIT:
            ZR_NEEDOUT();
            *q++ = (uint8_t)c->sub.lit;
            m--;
            c->mode = ZR_CODE_START;
            break;

        case ZR_CODE_WASH:
            if (k > 7) {
                k -= 8;
                n++;
                p--;
            }
            ZR_FLUSH_OUT();
            if (s->read != s->write)
                ZR_LEAVE();
            c->mode = ZR_CODE_END;
            /* fall through */

        case ZR_CODE_END:
            r = ZR_STREAM_END;
            ZR_LEAVE();

        case ZR_CODE_BADCODE:
            r = ZR_DATA_ERROR;
            ZR_LEAVE();

        default:
            r = ZR_STREAM_ERROR;
            ZR_LEAVE();
        }
    }
}

/* Target VA 0x00195b10. */
void inflate_codes_free_recovered(zr_inflate_codes_state *c, zr_stream *z)
{
    zr_free(z, c);
}
