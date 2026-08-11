/*
 * SNES Station v0.23 WIP — legacy ZIP method 6 (Implode/Explode).
 *
 * Recovered from the target around 0x0018c124..0x0018e4c0.
 * Function names are assigned only after the target control flow, constants,
 * nibble-coded tree format, and four-way 4K/8K + literal/no-literal dispatch
 * were independently identified and then cross-checked against period
 * Info-ZIP / Mark Adler sources.
 *
 * This file is a behavior-oriented reconstruction.  The small ZipIORecovered
 * adapter replaces the original unzip library's concrete structure layout;
 * exact target field offsets are tracked separately in analysis notes.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/legacy_zip_recovered.h"

#define EXPLODE_WSIZE LEGACY_ZIP_EXPLODE_WSIZE
#define HUFT_BMAX 16
#define HUFT_NMAX 288u

struct huft_recovered {
    uint8_t e;
    uint8_t b;
    union {
        uint16_t n;
        struct huft_recovered *t;
    } v;
};


static const uint16_t mask_bits_recovered[17] = {
    0x0000, 0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f,
    0x007f, 0x00ff, 0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff,
    0x3fff, 0x7fff, 0xffff
};

static unsigned g_hufts_recovered;
static uint16_t g_bytebuf_recovered;
uint64_t g_legacy_zip_bitbuf;
unsigned g_legacy_zip_bits_left;
uint8_t g_legacy_zip_zipeof;

/* Target VA 0x0018dc60. */
int ReadByte_recovered(uint16_t *out)
{
    ZipIORecovered *io = &g_zip_io_recovered;
    if (io->avail_in == 0) {
        uint32_t want;
        int got;
        if (io->rest_read_compressed <= 0 || io->read_buffer_size == 0 ||
            io->refill == NULL) {
            return 0;
        }
        want = io->read_buffer_size;
        if ((uint32_t)io->rest_read_compressed < want)
            want = (uint32_t)io->rest_read_compressed;
        got = io->refill(io->opaque, io->read_buffer, want);
        if (got <= 0)
            return got;
        io->next_in = io->read_buffer;
        io->avail_in = (uint32_t)got;
        io->rest_read_compressed -= got;
    }
    *out = *io->next_in++;
    io->avail_in--;
    return 8;
}

/* Target VA 0x0018e318. */
void flush_recovered(unsigned w)
{
    ZipIORecovered *io = &g_zip_io_recovered;
    memmove(io->next_out, io->slide, w);
    if (io->crc32_update != NULL)
        io->crc32 = io->crc32_update(io->crc32, io->next_out, w);
    io->next_out += w;
    io->avail_out -= w;
    io->total_out += w;
}

/* Target VA 0x0018e3ac. */
void flush_stack_recovered(unsigned w)
{
    ZipIORecovered *io = &g_zip_io_recovered;
    memmove(io->next_out, io->stack, w);
    if (io->crc32_update != NULL)
        io->crc32 = io->crc32_update(io->crc32, io->next_out, w);
    io->next_out += w;
    io->avail_out -= w;
    io->total_out += w;
}

/* Target VA 0x0018e440. */
int FillBitBuffer_recovered(void)
{
    uint16_t temp;
    g_legacy_zip_zipeof = 1;
    while (g_legacy_zip_bits_left < 25 && ReadByte_recovered(&temp) == 8) {
        g_legacy_zip_bitbuf |= (uint64_t)temp << g_legacy_zip_bits_left;
        g_legacy_zip_bits_left += 8;
        g_legacy_zip_zipeof = 0;
    }
    return 0;
}

static int need_local_bits(uint32_t *b, unsigned *k, unsigned n)
{
    while (*k < n) {
        uint16_t x;
        if (ReadByte_recovered(&x) != 8)
            return 0;
        *b |= (uint32_t)x << *k;
        *k += 8;
    }
    return 1;
}

/* Target VA 0x0018c124. */
int get_tree_recovered(unsigned *lengths, unsigned expected)
{
    unsigned pairs, written = 0;
    uint16_t byte;

    if (ReadByte_recovered(&byte) != 8)
        return 4;
    pairs = (unsigned)byte + 1;

    while (pairs-- != 0) {
        unsigned bit_length, repeat;
        if (ReadByte_recovered(&byte) != 8)
            return 4;
        bit_length = ((unsigned)byte & 0x0fu) + 1u;
        repeat = (((unsigned)byte >> 4) & 0x0fu) + 1u;
        if (written + repeat > expected)
            return 4;
        while (repeat-- != 0)
            lengths[written++] = bit_length;
    }
    return written == expected ? 0 : 4;
}

/* Target VA 0x0018e2e0. */
int huft_free_recovered(struct huft_recovered *t)
{
    while (t != NULL) {
        struct huft_recovered *allocation = t - 1;
        struct huft_recovered *next = allocation->v.t;
        free(allocation);
        t = next;
    }
    return 0;
}

/* Target VA 0x0018dd6c.  Classic linked-allocation Huffman table builder. */
int huft_build_recovered(const unsigned *b, unsigned n, unsigned s,
                         const uint16_t *d, const uint16_t *e,
                         struct huft_recovered **t, int *m)
{
    unsigned a, c[HUFT_BMAX + 1], f, i, j, *p, v[HUFT_NMAX];
    int g, h, k, l, w, y;
    struct huft_recovered *q, r, *u[HUFT_BMAX];
    unsigned x[HUFT_BMAX + 1], *xp, z;

    if (n == 0 || n > HUFT_NMAX)
        return 2;
    memset(c, 0, sizeof(c));
    for (i = 0; i < n; ++i) {
        if (b[i] > HUFT_BMAX)
            return 2;
        c[b[i]]++;
    }
    if (c[0] == n) {
        *t = NULL;
        *m = 0;
        return 0;
    }

    l = *m;
    for (j = 1; j <= HUFT_BMAX && c[j] == 0; ++j) {}
    k = (int)j;
    if (l < (int)j) l = (int)j;
    for (i = HUFT_BMAX; i != 0 && c[i] == 0; --i) {}
    g = (int)i;
    if (l > (int)i) l = (int)i;
    *m = l;

    for (y = 1 << j; j < i; ++j, y <<= 1) {
        y -= (int)c[j];
        if (y < 0) return 2;
    }
    y -= (int)c[i];
    if (y < 0) return 2;
    c[i] += (unsigned)y;

    x[1] = j = 0;
    p = c + 1;
    xp = x + 2;
    for (i = (unsigned)g; --i != 0;)
        *xp++ = (j += *p++);

    for (i = 0; i < n; ++i)
        if ((j = b[i]) != 0)
            v[x[j]++] = i;

    x[0] = i = 0;
    p = v;
    h = -1;
    w = -l;
    u[0] = NULL;
    q = NULL;
    z = 0;

    for (; k <= g; ++k) {
        a = c[k];
        while (a-- != 0) {
            while (k > w + l) {
                unsigned table_bits;
                ++h;
                w += l;
                z = (unsigned)(g - w);
                if (z > (unsigned)l) z = (unsigned)l;
                table_bits = (unsigned)(k - w);
                f = 1u << table_bits;
                if (f > a + 1) {
                    f -= a + 1;
                    xp = c + k;
                    while (++table_bits < z) {
                        f <<= 1;
                        ++xp;
                        if (f <= *xp) break;
                        f -= *xp;
                    }
                }
                z = 1u << table_bits;
                q = (struct huft_recovered *)malloc((z + 1u) * sizeof(*q));
                if (q == NULL) {
                    if (h > 0) huft_free_recovered(u[0]);
                    return 3;
                }
                g_hufts_recovered += z + 1u;
                *t = q + 1;
                q->v.t = NULL;
                t = &q->v.t;
                u[h] = ++q;

                if (h != 0) {
                    x[h] = i;
                    r.b = (uint8_t)l;
                    r.e = (uint8_t)(16u + table_bits);
                    r.v.t = q;
                    j = i >> (w - l);
                    u[h - 1][j] = r;
                }
            }

            r.b = (uint8_t)(k - w);
            if (p >= v + n) {
                r.e = 99;
            } else if (*p < s) {
                r.e = (uint8_t)(*p < 256 ? 16 : 15);
                r.v.n = (uint16_t)*p++;
            } else {
                if (d == NULL || e == NULL) return 2;
                r.e = (uint8_t)e[*p - s];
                r.v.n = d[*p++ - s];
            }

            f = 1u << (k - w);
            for (j = i >> w; j < z; j += f)
                q[j] = r;

            for (j = 1u << (k - 1); (i & j) != 0; j >>= 1)
                i ^= j;
            i ^= j;

            while (h > 0 && (i & ((1u << w) - 1u)) != x[h]) {
                --h;
                w -= l;
            }
        }
    }
    return y != 0 && g != 1 ? 1 : 0;
}

static int decode_huft_value(uint32_t *b, unsigned *k,
                             struct huft_recovered *table, unsigned bits,
                             struct huft_recovered **result)
{
    unsigned e, mask = mask_bits_recovered[bits];
    struct huft_recovered *t;
    if (!need_local_bits(b, k, bits)) return 1;
    t = table + ((~*b) & mask);
    e = t->e;
    while (e > 16) {
        if (e == 99) return 1;
        *b >>= t->b; *k -= t->b;
        e -= 16;
        if (!need_local_bits(b, k, e)) return 1;
        t = t->v.t + ((~*b) & mask_bits_recovered[e]);
        e = t->e;
    }
    *b >>= t->b; *k -= t->b;
    *result = t;
    return 0;
}

static int explode_stream_recovered(struct huft_recovered *tb,
                                    struct huft_recovered *tl,
                                    struct huft_recovered *td,
                                    int bb, int bl, int bd,
                                    int coded_literals, unsigned low_dist_bits)
{
    ZipIORecovered *io = &g_zip_io_recovered;
    int32_t s = io->rest_read_uncompressed;
    uint32_t b = 0, w = 0;
    unsigned k = 0, unflushed = 1;

    while (s > 0) {
        struct huft_recovered *t;
        unsigned n, d, e;
        if (!need_local_bits(&b, &k, 1)) return 1;
        if (b & 1u) {
            uint8_t literal;
            b >>= 1; --k; --s;
            if (coded_literals) {
                if (decode_huft_value(&b, &k, tb, (unsigned)bb, &t)) return 1;
                literal = (uint8_t)t->v.n;
            } else {
                if (!need_local_bits(&b, &k, 8)) return 1;
                literal = (uint8_t)b;
                b >>= 8; k -= 8;
            }
            io->slide[w++] = literal;
            if (w == EXPLODE_WSIZE) {
                flush_recovered(w); w = 0; unflushed = 0;
            }
            continue;
        }

        b >>= 1; --k;
        if (!need_local_bits(&b, &k, low_dist_bits)) return 1;
        d = b & ((1u << low_dist_bits) - 1u);
        b >>= low_dist_bits; k -= low_dist_bits;
        if (decode_huft_value(&b, &k, td, (unsigned)bd, &t)) return 1;
        d = w - d - t->v.n;

        if (decode_huft_value(&b, &k, tl, (unsigned)bl, &t)) return 1;
        n = t->v.n;
        e = t->e;
        if (e != 0) {
            if (!need_local_bits(&b, &k, 8)) return 1;
            n += b & 0xffu;
            b >>= 8; k -= 8;
        }

        s -= (int32_t)n;
        while (n != 0) {
            unsigned chunk;
            d &= EXPLODE_WSIZE - 1u;
            chunk = EXPLODE_WSIZE - (d > w ? d : w);
            if (chunk > n) chunk = n;
            n -= chunk;
            if (unflushed && w <= d) {
                memset(io->slide + w, 0, chunk);
                w += chunk; d += chunk;
            } else if (w - d >= chunk) {
                memcpy(io->slide + w, io->slide + d, chunk);
                w += chunk; d += chunk;
            } else {
                while (chunk-- != 0)
                    io->slide[w++] = io->slide[d++];
            }
            if (w == EXPLODE_WSIZE) {
                flush_recovered(w); w = 0; unflushed = 0;
            }
        }
    }
    flush_recovered(w);
    return io->rest_read_compressed != 0 ? 5 : 0;
}

/* Target VAs 0x0018c1f8 / 0x0018c834 / 0x0018ce70 / 0x0018d3c4. */
int explode_lit8_recovered(struct huft_recovered *tb, struct huft_recovered *tl,
                           struct huft_recovered *td, int bb, int bl, int bd)
{ return explode_stream_recovered(tb, tl, td, bb, bl, bd, 1, 7); }
int explode_lit4_recovered(struct huft_recovered *tb, struct huft_recovered *tl,
                           struct huft_recovered *td, int bb, int bl, int bd)
{ return explode_stream_recovered(tb, tl, td, bb, bl, bd, 1, 6); }
int explode_nolit8_recovered(struct huft_recovered *tl, struct huft_recovered *td,
                             int bl, int bd)
{ return explode_stream_recovered(NULL, tl, td, 0, bl, bd, 0, 7); }
int explode_nolit4_recovered(struct huft_recovered *tl, struct huft_recovered *td,
                             int bl, int bd)
{ return explode_stream_recovered(NULL, tl, td, 0, bl, bd, 0, 6); }

/* Target VA 0x0018d918. */
int explode_recovered(void)
{
    ZipIORecovered *io = &g_zip_io_recovered;
    unsigned lengths[256];
    uint16_t cplen2[64], cplen3[64], extra[64], cpdist4[64], cpdist8[64];
    struct huft_recovered *tb = NULL, *tl = NULL, *td = NULL;
    int bb = 9, bl = 7, bd = io->rest_read_compressed > 200000 ? 8 : 7;
    int r;
    unsigned i;

    for (i = 0; i < 64; ++i) {
        cplen2[i] = (uint16_t)(i + 2u);
        cplen3[i] = (uint16_t)(i + 3u);
        extra[i] = (uint16_t)(i == 63 ? 8u : 0u);
        cpdist4[i] = (uint16_t)(1u + 64u * i);
        cpdist8[i] = (uint16_t)(1u + 128u * i);
    }
    g_hufts_recovered = 0;

    if ((io->general_purpose_flag & 4u) != 0) {
        if ((r = get_tree_recovered(lengths, 256)) != 0) return r;
        if ((r = huft_build_recovered(lengths, 256, 256, NULL, NULL, &tb, &bb)) != 0)
            goto fail_tb;
        if ((r = get_tree_recovered(lengths, 64)) != 0) goto fail_all;
        if ((r = huft_build_recovered(lengths, 64, 0, cplen3, extra, &tl, &bl)) != 0)
            goto fail_all;
        if ((r = get_tree_recovered(lengths, 64)) != 0) goto fail_all;
        if ((r = huft_build_recovered(lengths, 64, 0,
                                      (io->general_purpose_flag & 2u) ? cpdist8 : cpdist4,
                                      extra, &td, &bd)) != 0)
            goto fail_all;
        r = (io->general_purpose_flag & 2u)
          ? explode_lit8_recovered(tb, tl, td, bb, bl, bd)
          : explode_lit4_recovered(tb, tl, td, bb, bl, bd);
    } else {
        if ((r = get_tree_recovered(lengths, 64)) != 0) return r;
        if ((r = huft_build_recovered(lengths, 64, 0, cplen2, extra, &tl, &bl)) != 0)
            goto fail_all;
        if ((r = get_tree_recovered(lengths, 64)) != 0) goto fail_all;
        if ((r = huft_build_recovered(lengths, 64, 0,
                                      (io->general_purpose_flag & 2u) ? cpdist8 : cpdist4,
                                      extra, &td, &bd)) != 0)
            goto fail_all;
        r = (io->general_purpose_flag & 2u)
          ? explode_nolit8_recovered(tl, td, bl, bd)
          : explode_nolit4_recovered(tl, td, bl, bd);
    }

    huft_free_recovered(td);
    huft_free_recovered(tl);
    huft_free_recovered(tb);
    return r;

fail_all:
    huft_free_recovered(td);
    huft_free_recovered(tl);
fail_tb:
    huft_free_recovered(tb);
    return r;
}
