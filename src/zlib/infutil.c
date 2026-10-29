/*
 * SNES Station v0.23 WIP — zlib 1.1.3 inflate utility recovery.
 *
 * Target VA 0x001967e8: inflate_flush
 */
#include "../../include/zlib_1_1_3_recovered.h"

#include <string.h>

const uint32_t zr_inflate_mask[17] = {
    0x0000u, 0x0001u, 0x0003u, 0x0007u, 0x000fu, 0x001fu,
    0x003fu, 0x007fu, 0x00ffu, 0x01ffu, 0x03ffu, 0x07ffu,
    0x0fffu, 0x1fffu, 0x3fffu, 0x7fffu, 0xffffu
};

/* Target VA 0x001967e8. */
int inflate_flush_recovered(zr_inflate_blocks_state *s, zr_stream *z, int r)
{
    uint32_t n;
    uint8_t *p;
    uint8_t *q;

    q = s->read;
    n = (uint32_t)(q <= s->write ? s->write - q : s->end - q);
    if (n > z->avail_out)
        n = z->avail_out;
    if (n != 0 && r == ZR_BUF_ERROR)
        r = ZR_OK;

    z->avail_out -= n;
    z->total_out += n;
    if (s->checkfn != NULL) {
        s->check = s->checkfn(s->check, q, n);
        z->adler = s->check;
    }
    p = z->next_out;
    memmove(p, q, n);
    p += n;
    q += n;

    if (q == s->end) {
        q = s->window;
        if (s->write == s->end)
            s->write = s->window;
        n = (uint32_t)(s->write - q);
        if (n > z->avail_out)
            n = z->avail_out;
        if (n != 0 && r == ZR_BUF_ERROR)
            r = ZR_OK;

        z->avail_out -= n;
        z->total_out += n;
        if (s->checkfn != NULL) {
            s->check = s->checkfn(s->check, q, n);
            z->adler = s->check;
        }
        memmove(p, q, n);
        p += n;
        q += n;
    }

    z->next_out = p;
    s->read = q;
    return r;
}
