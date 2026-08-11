/*
 * SNES Station v0.23 WIP — legacy ZIP Reduce decompressor.
 * Target VAs: unReduce 0x0018e4c0, LoadFollowers 0x0018e92c.
 *
 * Behavior-oriented recovery.  The target shares the same bit-buffer and
 * output helpers as the adjacent Implode/Explode module.
 */
#include <stdint.h>
#include <string.h>
#include "../../include/legacy_zip_recovered.h"

#define REDUCE_DLE 144
#define REDUCE_WINDOW 0x4000u

static uint8_t followers_recovered[256][64];
static uint8_t slen_recovered[256];

static const int l_table_recovered[5] = {0, 0x7f, 0x3f, 0x1f, 0x0f};
static const int d_shift_recovered[5] = {0, 7, 6, 5, 4};
static const int d_mask_recovered[5] = {0, 1, 3, 7, 15};

static unsigned bits_needed_for_followers(unsigned n)
{
    unsigned bits = 0, cap = 1;
    if (n == 0) return 8;
    while (cap < n) { cap <<= 1; ++bits; }
    return bits == 0 ? 1 : bits;
}

static unsigned read_bits_recovered(unsigned n)
{
    uint64_t mask;
    while (g_legacy_zip_bits_left < n && !g_legacy_zip_zipeof)
        FillBitBuffer_recovered();
    if (n == 64) mask = UINT64_MAX;
    else mask = (UINT64_C(1) << n) - 1u;
    {
        unsigned value = (unsigned)(g_legacy_zip_bitbuf & mask);
        g_legacy_zip_bitbuf >>= n;
        g_legacy_zip_bits_left -= n;
        return value;
    }
}

/* Target VA 0x0018e92c. */
void LoadFollowers_recovered(void)
{
    int x;
    for (x = 255; x >= 0; --x) {
        unsigned i;
        slen_recovered[x] = (uint8_t)read_bits_recovered(6);
        for (i = 0; i < slen_recovered[x]; ++i)
            followers_recovered[x][i] = (uint8_t)read_bits_recovered(8);
    }
}

/* Target VA 0x0018e4c0. */
void unReduce_recovered(void)
{
    ZipIORecovered *st = &g_zip_io_recovered;
    int lchar = 0, ex_state = 0, v = 0, len = 0;
    int32_t remaining = st->rest_read_compressed;
    unsigned w = 0, unflushed = 1;
    unsigned factor = st->compression_method;

    if (factor > 4) return;
    LoadFollowers_recovered();

    while (remaining > 0 && !g_legacy_zip_zipeof) {
        int nchar;
        if (slen_recovered[lchar] == 0) {
            nchar = (int)read_bits_recovered(8);
        } else if (read_bits_recovered(1) != 0) {
            nchar = (int)read_bits_recovered(8);
        } else {
            unsigned bits = bits_needed_for_followers(slen_recovered[lchar]);
            unsigned follower = read_bits_recovered(bits);
            nchar = followers_recovered[lchar][follower];
        }

        switch (ex_state) {
        case 0:
            if (nchar != REDUCE_DLE) {
                --remaining;
                st->slide[w++] = (uint8_t)nchar;
                if (w == REDUCE_WINDOW) { flush_recovered(w); w = 0; unflushed = 0; }
            } else ex_state = 1;
            break;
        case 1:
            if (nchar != 0) {
                v = nchar;
                len = v & l_table_recovered[factor];
                ex_state = len == l_table_recovered[factor] ? 2 : 3;
            } else {
                --remaining;
                st->slide[w++] = REDUCE_DLE;
                if (w == REDUCE_WINDOW) { flush_recovered(w); w = 0; unflushed = 0; }
                ex_state = 0;
            }
            break;
        case 2:
            len += nchar;
            ex_state = 3;
            break;
        case 3: {
            unsigned n = (unsigned)len + 3u;
            unsigned d = w - (((((unsigned)v >> d_shift_recovered[factor]) &
                               (unsigned)d_mask_recovered[factor]) << 8) +
                               (unsigned)nchar + 1u);
            remaining -= (int32_t)n;
            while (n != 0) {
                unsigned chunk;
                d &= REDUCE_WINDOW - 1u;
                chunk = REDUCE_WINDOW - (d > w ? d : w);
                if (chunk > n) chunk = n;
                n -= chunk;
                if (unflushed && w <= d) {
                    memset(st->slide + w, 0, chunk); w += chunk; d += chunk;
                } else if (w - d >= chunk) {
                    memcpy(st->slide + w, st->slide + d, chunk); w += chunk; d += chunk;
                } else {
                    while (chunk-- != 0) st->slide[w++] = st->slide[d++];
                }
                if (w == REDUCE_WINDOW) { flush_recovered(w); w = 0; unflushed = 0; }
            }
            ex_state = 0;
            break;
        }
        default:
            ex_state = 0;
            break;
        }
        lchar = nchar;
    }
    flush_recovered(w);
}
