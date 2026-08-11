/*
 * SNES Station v0.23 WIP — zlib 1.1.3 inflate Huffman-tree recovery.
 *
 * Target VAs:
 *   0x00195f80 huft_build
 *   0x001964d4 inflate_trees_bits
 *   0x001965d4 inflate_trees_dynamic
 *   0x001967b0 inflate_trees_fixed
 *
 * The fixed-tree entry point proves the target used pre-generated inffixed.h:
 * it returns fixed depths 9/5 and pointers at 0x00424870/0x00425870.
 */
#include "../../include/zlib_1_1_3_recovered.h"

#define ZR_BMAX 15
#define ZR_MANY 1440u

static const uint32_t zr_cplens[31] = {
    3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,
    35,43,51,59,67,83,99,115,131,163,195,227,258,0,0
};
static const uint32_t zr_cplext[31] = {
    0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,
    3,3,3,3,4,4,4,4,5,5,5,5,0,112,112
};
static const uint32_t zr_cpdist[30] = {
    1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
    257,385,513,769,1025,1537,2049,3073,4097,6145,
    8193,12289,16385,24577
};
static const uint32_t zr_cpdext[30] = {
    0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,
    7,7,8,8,9,9,10,10,11,11,12,12,13,13
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

/* Target VA 0x00195f80. */
static int huft_build_recovered(uint32_t *b, uint32_t n, uint32_t simple,
                                const uint32_t *d, const uint32_t *e,
                                zr_inflate_huft **table, uint32_t *m,
                                zr_inflate_huft *hp, uint32_t *hn,
                                uint32_t *v)
{
    uint32_t a;
    uint32_t c[ZR_BMAX + 1];
    uint32_t f;
    int g;
    int h;
    uint32_t i;
    uint32_t j;
    int k;
    int l;
    uint32_t mask;
    uint32_t *p;
    zr_inflate_huft *q;
    zr_inflate_huft r;
    zr_inflate_huft *u[ZR_BMAX];
    int w;
    uint32_t x[ZR_BMAX + 1];
    uint32_t *xp;
    int y;
    uint32_t z;

    for (i = 0; i <= ZR_BMAX; i++)
        c[i] = 0;
    p = b;
    i = n;
    do {
        c[*p++]++;
    } while (--i != 0);

    if (c[0] == n) {
        *table = NULL;
        *m = 0;
        return ZR_OK;
    }

    l = (int)*m;
    for (j = 1; j <= ZR_BMAX; j++)
        if (c[j] != 0)
            break;
    k = (int)j;
    if ((uint32_t)l < j)
        l = (int)j;
    for (i = ZR_BMAX; i != 0; i--)
        if (c[i] != 0)
            break;
    g = (int)i;
    if ((uint32_t)l > i)
        l = (int)i;
    *m = (uint32_t)l;

    for (y = 1 << j; j < i; j++, y <<= 1) {
        y -= (int)c[j];
        if (y < 0)
            return ZR_DATA_ERROR;
    }
    y -= (int)c[i];
    if (y < 0)
        return ZR_DATA_ERROR;
    c[i] += (uint32_t)y;

    x[1] = j = 0;
    p = c + 1;
    xp = x + 2;
    while (--i != 0)
        *xp++ = (j += *p++);

    p = b;
    i = 0;
    do {
        j = *p++;
        if (j != 0)
            v[x[j]++] = i;
    } while (++i < n);
    n = x[g];

    x[0] = i = 0;
    p = v;
    h = -1;
    w = -l;
    u[0] = NULL;
    q = NULL;
    z = 0;

    for (; k <= g; k++) {
        a = c[k];
        while (a-- != 0) {
            while (k > w + l) {
                h++;
                w += l;
                z = (uint32_t)(g - w);
                if (z > (uint32_t)l)
                    z = (uint32_t)l;
                f = 1u << (j = (uint32_t)(k - w));
                if (f > a + 1u) {
                    f -= a + 1u;
                    xp = c + k;
                    if (j < z) {
                        while (++j < z) {
                            f <<= 1;
                            xp++;
                            if (f <= *xp)
                                break;
                            f -= *xp;
                        }
                    }
                }
                z = 1u << j;
                if (*hn + z > ZR_MANY)
                    return ZR_MEM_ERROR;
                u[h] = q = hp + *hn;
                *hn += z;
                if (h != 0) {
                    x[h] = i;
                    r.bits = (uint8_t)l;
                    r.exop = (uint8_t)j;
                    r.pad = 0;
                    j = i >> (w - l);
                    r.base = (uint32_t)(q - u[h - 1] - j);
                    u[h - 1][j] = r;
                } else {
                    *table = q;
                }
            }

            r.bits = (uint8_t)(k - w);
            r.pad = 0;
            if (p >= v + n) {
                r.exop = 192;
                r.base = 0;
            } else if (*p < simple) {
                r.exop = (uint8_t)(*p < 256 ? 0 : 96);
                r.base = *p++;
            } else {
                r.exop = (uint8_t)(e[*p - simple] + 80u);
                r.base = d[*p++ - simple];
            }

            f = 1u << (k - w);
            for (j = i >> w; j < z; j += f)
                q[j] = r;

            for (j = 1u << (k - 1); (i & j) != 0; j >>= 1)
                i ^= j;
            i ^= j;

            mask = w == 0 ? 0 : (1u << w) - 1u;
            while ((i & mask) != x[h]) {
                h--;
                w -= l;
                mask = w == 0 ? 0 : (1u << w) - 1u;
            }
        }
    }

    return y != 0 && g != 1 ? ZR_BUF_ERROR : ZR_OK;
}

/* Target VA 0x001964d4. */
int inflate_trees_bits_recovered(uint32_t *c, uint32_t *bb,
                                 zr_inflate_huft **tb,
                                 zr_inflate_huft *hp, zr_stream *z)
{
    int r;
    uint32_t hn = 0;
    uint32_t *v = (uint32_t *)zr_alloc(z, 19, sizeof(uint32_t));

    if (v == NULL)
        return ZR_MEM_ERROR;
    r = huft_build_recovered(c, 19, 19, NULL, NULL, tb, bb, hp, &hn, v);
    if (r == ZR_DATA_ERROR)
        z->msg = (char *)"oversubscribed dynamic bit lengths tree";
    else if (r == ZR_BUF_ERROR || *bb == 0) {
        z->msg = (char *)"incomplete dynamic bit lengths tree";
        r = ZR_DATA_ERROR;
    }
    zr_free(z, v);
    return r;
}

/* Target VA 0x001965d4. */
int inflate_trees_dynamic_recovered(unsigned nl, unsigned nd, uint32_t *c,
                                    uint32_t *bl, uint32_t *bd,
                                    zr_inflate_huft **tl,
                                    zr_inflate_huft **td,
                                    zr_inflate_huft *hp, zr_stream *z)
{
    int r;
    uint32_t hn = 0;
    uint32_t *v = (uint32_t *)zr_alloc(z, 288, sizeof(uint32_t));

    if (v == NULL)
        return ZR_MEM_ERROR;

    r = huft_build_recovered(c, nl, 257, zr_cplens, zr_cplext,
                             tl, bl, hp, &hn, v);
    if (r != ZR_OK || *bl == 0) {
        if (r == ZR_DATA_ERROR)
            z->msg = (char *)"oversubscribed literal/length tree";
        else if (r != ZR_MEM_ERROR) {
            z->msg = (char *)"incomplete literal/length tree";
            r = ZR_DATA_ERROR;
        }
        zr_free(z, v);
        return r;
    }

    r = huft_build_recovered(c + nl, nd, 0, zr_cpdist, zr_cpdext,
                             td, bd, hp, &hn, v);
    if (r != ZR_OK || (*bd == 0 && nl > 257)) {
        if (r == ZR_DATA_ERROR)
            z->msg = (char *)"oversubscribed distance tree";
        else if (r == ZR_BUF_ERROR) {
            z->msg = (char *)"incomplete distance tree";
            r = ZR_DATA_ERROR;
        } else if (r != ZR_MEM_ERROR) {
            z->msg = (char *)"empty distance tree with lengths";
            r = ZR_DATA_ERROR;
        }
        zr_free(z, v);
        return r;
    }

    zr_free(z, v);
    return ZR_OK;
}

/* Standard pre-generated fixed tables embedded in the target. */
extern zr_inflate_huft fixed_tl_00424870[];
extern zr_inflate_huft fixed_td_00425870[];

/* Target VA 0x001967b0. */
int inflate_trees_fixed_recovered(uint32_t *bl, uint32_t *bd,
                                  zr_inflate_huft **tl,
                                  zr_inflate_huft **td, zr_stream *z)
{
    (void)z;
    *bl = 9;
    *bd = 5;
    *tl = fixed_tl_00424870;
    *td = fixed_td_00425870;
    return ZR_OK;
}
