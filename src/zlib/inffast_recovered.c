/*
 * SNES Station v0.23 WIP — zlib 1.1.3 fast inflate decoder recovery.
 *
 * Target VA 0x00195b40: inflate_fast
 */
#include "../../include/zlib_1_1_3_recovered.h"

/* Target VA 0x00195b40. */
int inflate_fast_recovered(unsigned bl, unsigned bd, zr_inflate_huft *tl,
                           zr_inflate_huft *td,
                           zr_inflate_blocks_state *s, zr_stream *z)
{
    zr_inflate_huft *t;
    uint32_t e;
    uint64_t b = s->bitb;
    uint32_t k = s->bitk;
    uint8_t *p = z->next_in;
    uint32_t n = z->avail_in;
    uint8_t *q = s->write;
    uint32_t m = (uint32_t)(q < s->read ? s->read - q - 1 : s->end - q);
    uint32_t ml = zr_inflate_mask[bl];
    uint32_t md = zr_inflate_mask[bd];
    uint32_t c;
    uint32_t d;
    uint8_t *src;

#define FAST_GRAB(j) \
    do { \
        while (k < (j)) { \
            n--; \
            b |= (uint64_t)(*p++) << k; \
            k += 8; \
        } \
    } while (0)
#define FAST_DUMP(j) do { b >>= (j); k -= (j); } while (0)
#define FAST_UNGRAB() \
    do { \
        uint32_t back = k >> 3; \
        n += back; \
        p -= back; \
        k -= back << 3; \
    } while (0)
#define FAST_UPDATE() \
    do { \
        s->bitb = b; \
        s->bitk = k; \
        z->avail_in = n; \
        z->total_in += (uint64_t)(p - z->next_in); \
        z->next_in = p; \
        s->write = q; \
    } while (0)
#define FAST_RETURN(code) \
    do { FAST_UNGRAB(); FAST_UPDATE(); return (code); } while (0)

    do {
        FAST_GRAB(20);
        t = tl + ((uint32_t)b & ml);
        e = t->exop;
        if (e == 0) {
            FAST_DUMP(t->bits);
            *q++ = (uint8_t)t->base;
            m--;
            continue;
        }

        for (;;) {
            FAST_DUMP(t->bits);
            if (e & 16u) {
                e &= 15u;
                c = t->base + ((uint32_t)b & zr_inflate_mask[e]);
                FAST_DUMP(e);

                FAST_GRAB(15);
                t = td + ((uint32_t)b & md);
                e = t->exop;
                for (;;) {
                    FAST_DUMP(t->bits);
                    if (e & 16u) {
                        e &= 15u;
                        FAST_GRAB(e);
                        d = t->base + ((uint32_t)b & zr_inflate_mask[e]);
                        FAST_DUMP(e);

                        m -= c;
                        if ((uint32_t)(q - s->window) >= d) {
                            src = q - d;
                            *q++ = *src++;
                            c--;
                            *q++ = *src++;
                            c--;
                        } else {
                            e = d - (uint32_t)(q - s->window);
                            src = s->end - e;
                            if (c > e) {
                                c -= e;
                                do {
                                    *q++ = *src++;
                                } while (--e);
                                src = s->window;
                            }
                        }
                        do {
                            *q++ = *src++;
                        } while (--c);
                        break;
                    }
                    if ((e & 64u) == 0) {
                        t += t->base;
                        t += (uint32_t)b & zr_inflate_mask[e];
                        e = t->exop;
                    } else {
                        z->msg = (char *)"invalid distance code";
                        FAST_RETURN(ZR_DATA_ERROR);
                    }
                }
                break;
            }

            if ((e & 64u) == 0) {
                t += t->base;
                t += (uint32_t)b & zr_inflate_mask[e];
                e = t->exop;
                if (e == 0) {
                    FAST_DUMP(t->bits);
                    *q++ = (uint8_t)t->base;
                    m--;
                    break;
                }
            } else if (e & 32u) {
                FAST_RETURN(ZR_STREAM_END);
            } else {
                z->msg = (char *)"invalid literal/length code";
                FAST_RETURN(ZR_DATA_ERROR);
            }
        }
    } while (m >= 258 && n >= 10);

    FAST_RETURN(ZR_OK);

#undef FAST_GRAB
#undef FAST_DUMP
#undef FAST_UNGRAB
#undef FAST_UPDATE
#undef FAST_RETURN
}
