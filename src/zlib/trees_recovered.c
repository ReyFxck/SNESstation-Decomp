/*
 * SNES Station v0.23 WIP — zlib 1.1.3 trees.c behavioral recovery.
 *
 * Target range: 0x00196980..0x00198a80.
 *
 * The target contains the pre-generated zlib Huffman tables in .rodata, so
 * tr_static_init() compiles to an empty function at 0x00196980.  The table
 * symbols below are deliberately address-named and left external until the
 * read-only data layout is reconstructed in a separate source file.
 *
 * This source is a semantic reconstruction, not a MATCHING claim.
 */
#include "../../include/zlib_1_1_3_recovered.h"

#include <stddef.h>
#include <stdint.h>

#define ZR_MAX_BL_BITS 7
#define ZR_END_BLOCK 256
#define ZR_REP_3_6 16
#define ZR_REPZ_3_10 17
#define ZR_REPZ_11_138 18
#define ZR_STORED_BLOCK 0
#define ZR_STATIC_TREES 1
#define ZR_DYN_TREES 2
#define ZR_BINARY 0
#define ZR_ASCII 1
#define ZR_SMALLEST 1

#define FREQ(t, i) ((t)[(i)].fc.freq)
#define CODE(t, i) ((t)[(i)].fc.code)
#define DAD(t, i)  ((t)[(i)].dl.dad)
#define LEN(t, i)  ((t)[(i)].dl.len)

/* Read-only tables proven in the target image. */
extern const zr_ct_data zr_static_ltree_001b98c0[ZR_L_CODES + 2];
extern const zr_ct_data zr_static_dtree_001b9d40[ZR_D_CODES];
extern const uint8_t zr_dist_code_001b9db8[512];
extern const uint8_t zr_length_code_001b9fb8[ZR_MAX_MATCH - ZR_MIN_MATCH + 1];
extern const int zr_base_length_001ba0b8[ZR_LENGTH_CODES];
extern const int zr_base_dist_001ba130[ZR_D_CODES];

static const int zr_extra_lbits[ZR_LENGTH_CODES] = {
    0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0
};
static const int zr_extra_dbits[ZR_D_CODES] = {
    0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13
};
static const int zr_extra_blbits[ZR_BL_CODES] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7
};
static const uint8_t zr_bl_order[ZR_BL_CODES] = {
    16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15
};

static zr_static_tree_desc zr_static_l_desc = {
    zr_static_ltree_001b98c0, zr_extra_lbits, ZR_LITERALS + 1,
    ZR_L_CODES, ZR_MAX_BITS
};
static zr_static_tree_desc zr_static_d_desc = {
    zr_static_dtree_001b9d40, zr_extra_dbits, 0,
    ZR_D_CODES, ZR_MAX_BITS
};
static zr_static_tree_desc zr_static_bl_desc = {
    NULL, zr_extra_blbits, 0, ZR_BL_CODES, ZR_MAX_BL_BITS
};

static void put_byte(zr_deflate_state *s, uint8_t b)
{
    s->pending_buf[s->pending++] = b;
}

static void put_short(zr_deflate_state *s, uint16_t w)
{
    put_byte(s, (uint8_t)(w & 0xffu));
    put_byte(s, (uint8_t)(w >> 8));
}

static void send_bits(zr_deflate_state *s, unsigned value, int length)
{
    int len = length;
    if (s->bi_valid > 16 - len) {
        unsigned val = value;
        s->bi_buf = (uint16_t)(s->bi_buf | (uint16_t)(val << s->bi_valid));
        put_short(s, s->bi_buf);
        s->bi_buf = (uint16_t)(val >> (16 - s->bi_valid));
        s->bi_valid += len - 16;
    } else {
        s->bi_buf = (uint16_t)(s->bi_buf | (uint16_t)(value << s->bi_valid));
        s->bi_valid += len;
    }
}

static void send_code(zr_deflate_state *s, unsigned c, const zr_ct_data *tree)
{
    send_bits(s, CODE(tree, c), LEN(tree, c));
}

static unsigned d_code(unsigned dist)
{
    return dist < 256u ? zr_dist_code_001b9db8[dist]
                       : zr_dist_code_001b9db8[256u + (dist >> 7)];
}

/* Target VA 0x00196980. Empty because generated trees.h data is compiled in. */
static void tr_static_init(void)
{
}

/* Target VA 0x00196a00. */
static void init_block(zr_deflate_state *s)
{
    int n;
    for (n = 0; n < ZR_L_CODES; ++n)
        FREQ(s->dyn_ltree, n) = 0;
    for (n = 0; n < ZR_D_CODES; ++n)
        FREQ(s->dyn_dtree, n) = 0;
    for (n = 0; n < ZR_BL_CODES; ++n)
        FREQ(s->bl_tree, n) = 0;

    FREQ(s->dyn_ltree, ZR_END_BLOCK) = 1;
    s->opt_len = 0;
    s->static_len = 0;
    s->last_lit = 0;
    s->matches = 0;
}

/* Target VA 0x00196988. */
void _tr_init_recovered(zr_deflate_state *s)
{
    tr_static_init();
    s->l_desc.dyn_tree = s->dyn_ltree;
    s->l_desc.stat_desc = &zr_static_l_desc;
    s->d_desc.dyn_tree = s->dyn_dtree;
    s->d_desc.stat_desc = &zr_static_d_desc;
    s->bl_desc.dyn_tree = s->bl_tree;
    s->bl_desc.stat_desc = &zr_static_bl_desc;
    s->bi_buf = 0;
    s->bi_valid = 0;
    s->last_eob_len = 8;
    init_block(s);
}

static int smaller(const zr_ct_data *tree, int n, int m, const uint8_t *depth)
{
    return FREQ(tree, n) < FREQ(tree, m) ||
           (FREQ(tree, n) == FREQ(tree, m) && depth[n] <= depth[m]);
}

/* Target VA 0x00196a94. */
static void pqdownheap(zr_deflate_state *s, zr_ct_data *tree, int k)
{
    int v = s->heap[k];
    int j = k << 1;

    while (j <= s->heap_len) {
        if (j < s->heap_len &&
            smaller(tree, s->heap[j + 1], s->heap[j], s->depth))
            ++j;
        if (smaller(tree, v, s->heap[j], s->depth))
            break;
        s->heap[k] = s->heap[j];
        k = j;
        j <<= 1;
    }
    s->heap[k] = v;
}

/* Target VA 0x00196b98. */
static void gen_bitlen(zr_deflate_state *s, zr_tree_desc *desc)
{
    zr_ct_data *tree = desc->dyn_tree;
    int max_code = desc->max_code;
    const zr_ct_data *stree = desc->stat_desc->static_tree;
    const int *extra = desc->stat_desc->extra_bits;
    int base = desc->stat_desc->extra_base;
    int max_length = desc->stat_desc->max_length;
    int h, n, m, bits, xbits;
    int overflow = 0;

    for (bits = 0; bits <= ZR_MAX_BITS; ++bits)
        s->bl_count[bits] = 0;

    LEN(tree, s->heap[s->heap_max]) = 0;
    for (h = s->heap_max + 1; h < ZR_HEAP_SIZE; ++h) {
        uint16_t f;
        n = s->heap[h];
        bits = LEN(tree, DAD(tree, n)) + 1;
        if (bits > max_length) {
            bits = max_length;
            ++overflow;
        }
        LEN(tree, n) = (uint16_t)bits;
        if (n > max_code)
            continue;
        ++s->bl_count[bits];
        xbits = n >= base ? extra[n - base] : 0;
        f = FREQ(tree, n);
        s->opt_len += (uint64_t)f * (uint64_t)(bits + xbits);
        if (stree != NULL)
            s->static_len += (uint64_t)f * (uint64_t)(LEN(stree, n) + xbits);
    }

    if (overflow == 0)
        return;

    do {
        bits = max_length - 1;
        while (s->bl_count[bits] == 0)
            --bits;
        --s->bl_count[bits];
        s->bl_count[bits + 1] = (uint16_t)(s->bl_count[bits + 1] + 2);
        --s->bl_count[max_length];
        overflow -= 2;
    } while (overflow > 0);

    for (bits = max_length; bits != 0; --bits) {
        n = s->bl_count[bits];
        while (n != 0) {
            m = s->heap[--h];
            if (m > max_code)
                continue;
            if (LEN(tree, m) != (unsigned)bits) {
                s->opt_len += (int64_t)(bits - (int)LEN(tree, m)) *
                              (int64_t)FREQ(tree, m);
                LEN(tree, m) = (uint16_t)bits;
            }
            --n;
        }
    }
}

/* Forward declaration for the target leaf helper at 0x00198840. */
static unsigned bi_reverse(unsigned code, int len);

/* Target VA 0x00196e80. */
static void gen_codes(zr_ct_data *tree, int max_code, uint16_t *bl_count)
{
    uint16_t next_code[ZR_MAX_BITS + 1];
    uint16_t code = 0;
    int bits, n;

    next_code[0] = 0;
    for (bits = 1; bits <= ZR_MAX_BITS; ++bits)
        next_code[bits] = code = (uint16_t)((code + bl_count[bits - 1]) << 1);

    for (n = 0; n <= max_code; ++n) {
        int len = LEN(tree, n);
        if (len == 0)
            continue;
        CODE(tree, n) = (uint16_t)bi_reverse(next_code[len]++, len);
    }
}

/* Target VA 0x00196f24. */
static void build_tree(zr_deflate_state *s, zr_tree_desc *desc)
{
    zr_ct_data *tree = desc->dyn_tree;
    const zr_ct_data *stree = desc->stat_desc->static_tree;
    int elems = desc->stat_desc->elems;
    int n, m, max_code = -1, node;

    s->heap_len = 0;
    s->heap_max = ZR_HEAP_SIZE;
    for (n = 0; n < elems; ++n) {
        if (FREQ(tree, n) != 0) {
            s->heap[++s->heap_len] = max_code = n;
            s->depth[n] = 0;
        } else {
            LEN(tree, n) = 0;
        }
    }

    while (s->heap_len < 2) {
        node = s->heap[++s->heap_len] = max_code < 2 ? ++max_code : 0;
        FREQ(tree, node) = 1;
        s->depth[node] = 0;
        --s->opt_len;
        if (stree != NULL)
            s->static_len -= LEN(stree, node);
    }
    desc->max_code = max_code;

    for (n = s->heap_len / 2; n >= 1; --n)
        pqdownheap(s, tree, n);

    node = elems;
    do {
        n = s->heap[ZR_SMALLEST];
        s->heap[ZR_SMALLEST] = s->heap[s->heap_len--];
        pqdownheap(s, tree, ZR_SMALLEST);
        m = s->heap[ZR_SMALLEST];
        s->heap[--s->heap_max] = n;
        s->heap[--s->heap_max] = m;
        FREQ(tree, node) = (uint16_t)(FREQ(tree, n) + FREQ(tree, m));
        s->depth[node] = (uint8_t)((s->depth[n] >= s->depth[m] ?
                                    s->depth[n] : s->depth[m]) + 1);
        DAD(tree, n) = DAD(tree, m) = (uint16_t)node;
        s->heap[ZR_SMALLEST] = node++;
        pqdownheap(s, tree, ZR_SMALLEST);
    } while (s->heap_len >= 2);

    s->heap[--s->heap_max] = s->heap[ZR_SMALLEST];
    gen_bitlen(s, desc);
    gen_codes(tree, max_code, s->bl_count);
}

/* Target VA 0x001971ec. */
static void scan_tree(zr_deflate_state *s, zr_ct_data *tree, int max_code)
{
    int n;
    int prevlen = -1;
    int curlen;
    int nextlen = LEN(tree, 0);
    int count = 0;
    int max_count = 7;
    int min_count = 4;

    if (nextlen == 0) {
        max_count = 138;
        min_count = 3;
    }
    LEN(tree, max_code + 1) = 0xffffu;

    for (n = 0; n <= max_code; ++n) {
        curlen = nextlen;
        nextlen = LEN(tree, n + 1);
        if (++count < max_count && curlen == nextlen)
            continue;
        if (count < min_count) {
            FREQ(s->bl_tree, curlen) = (uint16_t)(FREQ(s->bl_tree, curlen) + count);
        } else if (curlen != 0) {
            if (curlen != prevlen)
                ++FREQ(s->bl_tree, curlen);
            ++FREQ(s->bl_tree, ZR_REP_3_6);
        } else if (count <= 10) {
            ++FREQ(s->bl_tree, ZR_REPZ_3_10);
        } else {
            ++FREQ(s->bl_tree, ZR_REPZ_11_138);
        }
        count = 0;
        prevlen = curlen;
        if (nextlen == 0) {
            max_count = 138;
            min_count = 3;
        } else if (curlen == nextlen) {
            max_count = 6;
            min_count = 3;
        } else {
            max_count = 7;
            min_count = 4;
        }
    }
}

/* Target VA 0x001972f4. */
static void send_tree(zr_deflate_state *s, zr_ct_data *tree, int max_code)
{
    int n;
    int prevlen = -1;
    int curlen;
    int nextlen = LEN(tree, 0);
    int count = 0;
    int max_count = 7;
    int min_count = 4;

    if (nextlen == 0) {
        max_count = 138;
        min_count = 3;
    }

    for (n = 0; n <= max_code; ++n) {
        curlen = nextlen;
        nextlen = LEN(tree, n + 1);
        if (++count < max_count && curlen == nextlen)
            continue;
        if (count < min_count) {
            do {
                send_code(s, (unsigned)curlen, s->bl_tree);
            } while (--count != 0);
        } else if (curlen != 0) {
            if (curlen != prevlen) {
                send_code(s, (unsigned)curlen, s->bl_tree);
                --count;
            }
            send_code(s, ZR_REP_3_6, s->bl_tree);
            send_bits(s, (unsigned)(count - 3), 2);
        } else if (count <= 10) {
            send_code(s, ZR_REPZ_3_10, s->bl_tree);
            send_bits(s, (unsigned)(count - 3), 3);
        } else {
            send_code(s, ZR_REPZ_11_138, s->bl_tree);
            send_bits(s, (unsigned)(count - 11), 7);
        }
        count = 0;
        prevlen = curlen;
        if (nextlen == 0) {
            max_count = 138;
            min_count = 3;
        } else if (curlen == nextlen) {
            max_count = 6;
            min_count = 3;
        } else {
            max_count = 7;
            min_count = 4;
        }
    }
}

/* Target VA 0x00197874. */
static int build_bl_tree(zr_deflate_state *s)
{
    int max_blindex;
    scan_tree(s, s->dyn_ltree, s->l_desc.max_code);
    scan_tree(s, s->dyn_dtree, s->d_desc.max_code);
    build_tree(s, &s->bl_desc);

    for (max_blindex = ZR_BL_CODES - 1; max_blindex >= 3; --max_blindex) {
        if (LEN(s->bl_tree, zr_bl_order[max_blindex]) != 0)
            break;
    }
    s->opt_len += (uint64_t)(3 * (max_blindex + 1) + 5 + 5 + 4);
    return max_blindex;
}

/* Target VA 0x00197910. */
static void send_all_trees(zr_deflate_state *s, int lcodes, int dcodes, int blcodes)
{
    int rank;
    send_bits(s, (unsigned)(lcodes - 257), 5);
    send_bits(s, (unsigned)(dcodes - 1), 5);
    send_bits(s, (unsigned)(blcodes - 4), 4);
    for (rank = 0; rank < blcodes; ++rank)
        send_bits(s, LEN(s->bl_tree, zr_bl_order[rank]), 3);
    send_tree(s, s->dyn_ltree, lcodes - 1);
    send_tree(s, s->dyn_dtree, dcodes - 1);
}

static void bi_flush(zr_deflate_state *s);
static void bi_windup(zr_deflate_state *s);
static void copy_block(zr_deflate_state *s, uint8_t *buf, unsigned len, int header);
static void compress_block(zr_deflate_state *s, const zr_ct_data *ltree,
                           const zr_ct_data *dtree);
static void set_data_type(zr_deflate_state *s);

/* Target VA 0x00197bf4. */
void _tr_stored_block_recovered(zr_deflate_state *s, uint8_t *buf,
                                uint64_t stored_len, int eof)
{
    send_bits(s, (unsigned)((ZR_STORED_BLOCK << 1) + eof), 3);
    copy_block(s, buf, (unsigned)stored_len, 1);
}

/* Target VA 0x00197cb0. */
void _tr_align_recovered(zr_deflate_state *s)
{
    send_bits(s, ZR_STATIC_TREES << 1, 3);
    send_code(s, ZR_END_BLOCK, zr_static_ltree_001b98c0);
    bi_flush(s);

    if (1 + s->last_eob_len + 10 - s->bi_valid < 9) {
        send_bits(s, ZR_STATIC_TREES << 1, 3);
        send_code(s, ZR_END_BLOCK, zr_static_ltree_001b98c0);
        bi_flush(s);
    }
    s->last_eob_len = 7;
}

/* Target VA 0x00197f64. */
void _tr_flush_block_recovered(zr_deflate_state *s, uint8_t *buf,
                               uint64_t stored_len, int eof)
{
    uint64_t opt_lenb;
    uint64_t static_lenb;
    int max_blindex = 0;

    if (s->level > 0) {
        if (s->data_type == ZR_UNKNOWN)
            set_data_type(s);
        build_tree(s, &s->l_desc);
        build_tree(s, &s->d_desc);
        max_blindex = build_bl_tree(s);
        opt_lenb = (s->opt_len + 3 + 7) >> 3;
        static_lenb = (s->static_len + 3 + 7) >> 3;
        if (static_lenb <= opt_lenb)
            opt_lenb = static_lenb;
    } else {
        opt_lenb = static_lenb = stored_len + 5;
    }

    if (stored_len + 4 <= opt_lenb && buf != NULL) {
        _tr_stored_block_recovered(s, buf, stored_len, eof);
    } else if (static_lenb == opt_lenb) {
        send_bits(s, (unsigned)((ZR_STATIC_TREES << 1) + eof), 3);
        compress_block(s, zr_static_ltree_001b98c0, zr_static_dtree_001b9d40);
    } else {
        send_bits(s, (unsigned)((ZR_DYN_TREES << 1) + eof), 3);
        send_all_trees(s, s->l_desc.max_code + 1,
                       s->d_desc.max_code + 1, max_blindex + 1);
        compress_block(s, s->dyn_ltree, s->dyn_dtree);
    }

    init_block(s);
    if (eof)
        bi_windup(s);
}

/* Target VA 0x001981e8. */
int _tr_tally_recovered(zr_deflate_state *s, unsigned dist, unsigned lc)
{
    s->d_buf[s->last_lit] = (uint16_t)dist;
    s->l_buf[s->last_lit++] = (uint8_t)lc;

    if (dist == 0) {
        ++FREQ(s->dyn_ltree, lc);
    } else {
        unsigned code;
        ++s->matches;
        --dist;
        code = zr_length_code_001b9fb8[lc];
        ++FREQ(s->dyn_ltree, code + ZR_LITERALS + 1);
        ++FREQ(s->dyn_dtree, d_code(dist));
    }
    return s->last_lit == s->lit_bufsize - 1u;
}

/* Target VA 0x00198308. */
static void compress_block(zr_deflate_state *s, const zr_ct_data *ltree,
                           const zr_ct_data *dtree)
{
    unsigned lx = 0;

    if (s->last_lit != 0) {
        do {
            unsigned dist = s->d_buf[lx];
            int lc = s->l_buf[lx++];
            if (dist == 0) {
                send_code(s, (unsigned)lc, ltree);
            } else {
                unsigned code = zr_length_code_001b9fb8[lc];
                int extra;
                send_code(s, code + ZR_LITERALS + 1, ltree);
                extra = zr_extra_lbits[code];
                if (extra != 0) {
                    lc -= zr_base_length_001ba0b8[code];
                    send_bits(s, (unsigned)lc, extra);
                }
                --dist;
                code = d_code(dist);
                send_code(s, code, dtree);
                extra = zr_extra_dbits[code];
                if (extra != 0) {
                    dist -= (unsigned)zr_base_dist_001ba130[code];
                    send_bits(s, dist, extra);
                }
            }
        } while (lx < s->last_lit);
    }
    send_code(s, ZR_END_BLOCK, ltree);
    s->last_eob_len = LEN(ltree, ZR_END_BLOCK);
}

/* Target VA 0x0019879c. */
static void set_data_type(zr_deflate_state *s)
{
    int n = 0;
    unsigned ascii_freq = 0;
    unsigned bin_freq = 0;

    while (n < 7)
        bin_freq += FREQ(s->dyn_ltree, n++);
    while (n < 128)
        ascii_freq += FREQ(s->dyn_ltree, n++);
    while (n < ZR_LITERALS)
        bin_freq += FREQ(s->dyn_ltree, n++);

    s->data_type = (uint8_t)(bin_freq > (ascii_freq >> 2) ? ZR_BINARY : ZR_ASCII);
}

/* Target VA 0x00198840. */
static unsigned bi_reverse(unsigned code, int len)
{
    unsigned res = 0;
    do {
        res |= code & 1u;
        code >>= 1;
        res <<= 1;
    } while (--len > 0);
    return res >> 1;
}

/* Target VA 0x0019886c. */
static void bi_flush(zr_deflate_state *s)
{
    if (s->bi_valid == 16) {
        put_short(s, s->bi_buf);
        s->bi_buf = 0;
        s->bi_valid = 0;
    } else if (s->bi_valid >= 8) {
        put_byte(s, (uint8_t)s->bi_buf);
        s->bi_buf >>= 8;
        s->bi_valid -= 8;
    }
}

/* Target VA 0x00198904. */
static void bi_windup(zr_deflate_state *s)
{
    if (s->bi_valid > 8)
        put_short(s, s->bi_buf);
    else if (s->bi_valid > 0)
        put_byte(s, (uint8_t)s->bi_buf);
    s->bi_buf = 0;
    s->bi_valid = 0;
}

/* Target VA 0x0019897c. */
static void copy_block(zr_deflate_state *s, uint8_t *buf, unsigned len, int header)
{
    bi_windup(s);
    s->last_eob_len = 8;
    if (header) {
        put_short(s, (uint16_t)len);
        put_short(s, (uint16_t)~len);
    }
    while (len-- != 0)
        put_byte(s, *buf++);
}
