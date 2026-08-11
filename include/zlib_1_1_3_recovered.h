#ifndef SNESSTATION_ZLIB_1_1_3_RECOVERED_H
#define SNESSTATION_ZLIB_1_1_3_RECOVERED_H

#include <stddef.h>
#include <stdint.h>

/*
 * Logical reconstruction of the zlib 1.1.3 ABI embedded in SNES Station.
 *
 * The PS2 target was built with -mlong64, so zlib uLong is 64-bit while
 * uInt remains 32-bit. Pointer sizes in this host-side research header are
 * intentionally native; this is not yet a byte-layout header for rebuilding
 * the original ELF.
 */

enum {
    ZR_OK = 0,
    ZR_STREAM_END = 1,
    ZR_NEED_DICT = 2,
    ZR_ERRNO = -1,
    ZR_STREAM_ERROR = -2,
    ZR_DATA_ERROR = -3,
    ZR_MEM_ERROR = -4,
    ZR_BUF_ERROR = -5,
    ZR_VERSION_ERROR = -6,

    ZR_NO_FLUSH = 0,
    ZR_PARTIAL_FLUSH = 1,
    ZR_SYNC_FLUSH = 2,
    ZR_FULL_FLUSH = 3,
    ZR_FINISH = 4,

    ZR_DEFLATED = 8,
    ZR_DEFAULT_COMPRESSION = -1,
    ZR_DEFAULT_STRATEGY = 0,
    ZR_FILTERED = 1,
    ZR_HUFFMAN_ONLY = 2,
    ZR_UNKNOWN = 2,
};

#define ZR_VERSION "1.1.3"
#define ZR_TARGET_STREAM_SIZE 0x48
#define ZR_TARGET_DEFLATE_STATE_SIZE 0x16d8u
#define ZR_TARGET_INFLATE_STATE_SIZE 0x28u
#define ZR_MAX_WBITS 15
#define ZR_DEF_WBITS 15
#define ZR_DEF_MEM_LEVEL 8
#define ZR_MAX_MEM_LEVEL 9
#define ZR_MIN_MATCH 3u
#define ZR_MAX_MATCH 258u
#define ZR_MIN_LOOKAHEAD (ZR_MAX_MATCH + ZR_MIN_MATCH + 1u)

struct zr_stream;
typedef void *(*zr_alloc_func)(void *opaque, unsigned items, unsigned size);
typedef void (*zr_free_func)(void *opaque, void *address);
typedef uint64_t (*zr_check_func)(uint64_t, const uint8_t *, unsigned);

typedef struct zr_stream {
    uint8_t *next_in;
    uint32_t avail_in;
    uint64_t total_in;
    uint8_t *next_out;
    uint32_t avail_out;
    uint64_t total_out;
    char *msg;
    void *state;
    zr_alloc_func zalloc;
    zr_free_func zfree;
    void *opaque;
    int data_type;
    uint64_t adler;
    uint64_t reserved;
} zr_stream;

typedef uint16_t zr_pos;

typedef struct zr_ct_data {
    union {
        uint16_t freq;
        uint16_t code;
    } fc;
    union {
        uint16_t dad;
        uint16_t len;
    } dl;
} zr_ct_data;

typedef struct zr_static_tree_desc {
    const zr_ct_data *static_tree;
    const int *extra_bits;
    int extra_base;
    int elems;
    int max_length;
} zr_static_tree_desc;

typedef struct {
    zr_ct_data *dyn_tree;
    int max_code;
    zr_static_tree_desc *stat_desc;
} zr_tree_desc;

#define ZR_LENGTH_CODES 29
#define ZR_LITERALS 256
#define ZR_L_CODES (ZR_LITERALS + 1 + ZR_LENGTH_CODES)
#define ZR_D_CODES 30
#define ZR_BL_CODES 19
#define ZR_HEAP_SIZE (2 * ZR_L_CODES + 1)
#define ZR_MAX_BITS 15

typedef int (*zr_compress_func)(void *state, int flush);

typedef struct {
    uint16_t good_length;
    uint16_t max_lazy;
    uint16_t nice_length;
    uint16_t max_chain;
    zr_compress_func func;
} zr_config;

/*
 * The logical fields used by the recovered deflate.c and trees.c modules are
 * modeled by name. This host-side struct is intentionally not asserted to have
 * the target byte layout because native pointer width differs from the EE ABI.
 * The target allocation remains the observed 0x16d8 bytes.
 */
typedef struct zr_deflate_state {
    zr_stream *strm;
    int status;
    uint8_t *pending_buf;
    uint64_t pending_buf_size;
    uint8_t *pending_out;
    int pending;
    int noheader;
    uint8_t data_type;
    uint8_t method;
    int last_flush;

    uint32_t w_size;
    uint32_t w_bits;
    uint32_t w_mask;
    uint8_t *window;
    uint64_t window_size;
    zr_pos *prev;
    zr_pos *head;
    uint32_t ins_h;
    uint32_t hash_size;
    uint32_t hash_bits;
    uint32_t hash_mask;
    uint32_t hash_shift;
    int64_t block_start;
    uint32_t match_length;
    uint32_t prev_match;
    int match_available;
    uint32_t strstart;
    uint32_t match_start;
    uint32_t lookahead;
    uint32_t prev_length;
    uint32_t max_chain_length;
    uint32_t max_lazy_match;
    int level;
    int strategy;
    uint32_t good_match;
    int nice_match;

    zr_ct_data dyn_ltree[ZR_HEAP_SIZE];
    zr_ct_data dyn_dtree[2 * ZR_D_CODES + 1];
    zr_ct_data bl_tree[2 * ZR_BL_CODES + 1];
    zr_tree_desc l_desc;
    zr_tree_desc d_desc;
    zr_tree_desc bl_desc;
    uint16_t bl_count[ZR_MAX_BITS + 1];
    int heap[ZR_HEAP_SIZE];
    int heap_len;
    int heap_max;
    uint8_t depth[ZR_HEAP_SIZE];
    uint8_t *l_buf;
    uint32_t lit_bufsize;
    uint32_t last_lit;
    uint16_t *d_buf;
    uint64_t opt_len;
    uint64_t static_len;
    uint32_t matches;
    int last_eob_len;
    uint16_t bi_buf;
    int bi_valid;
} zr_deflate_state;

/* Independently verified target offsets for the 32-bit-pointer/-mlong64 ABI. */
enum {
    ZR_OFF_DYN_LTREE = 0x00a4,
    ZR_OFF_DYN_DTREE = 0x0998,
    ZR_OFF_BL_TREE = 0x0a8c,
    ZR_OFF_L_DESC = 0x0b28,
    ZR_OFF_D_DESC = 0x0b34,
    ZR_OFF_BL_DESC = 0x0b40,
    ZR_OFF_BL_COUNT = 0x0b4c,
    ZR_OFF_HEAP = 0x0b6c,
    ZR_OFF_HEAP_LEN = 0x1460,
    ZR_OFF_HEAP_MAX = 0x1464,
    ZR_OFF_DEPTH = 0x1468,
    ZR_OFF_L_BUF = 0x16a8,
    ZR_OFF_LIT_BUFSIZE = 0x16ac,
    ZR_OFF_LAST_LIT = 0x16b0,
    ZR_OFF_D_BUF = 0x16b4,
    ZR_OFF_OPT_LEN = 0x16b8,
    ZR_OFF_STATIC_LEN = 0x16c0,
    ZR_OFF_MATCHES = 0x16c8,
    ZR_OFF_LAST_EOB_LEN = 0x16cc,
    ZR_OFF_BI_BUF = 0x16d0,
    ZR_OFF_BI_VALID = 0x16d4,
};

typedef enum {
    ZR_INF_METHOD = 0,
    ZR_INF_FLAG,
    ZR_INF_DICT4,
    ZR_INF_DICT3,
    ZR_INF_DICT2,
    ZR_INF_DICT1,
    ZR_INF_DICT0,
    ZR_INF_BLOCKS,
    ZR_INF_CHECK4,
    ZR_INF_CHECK3,
    ZR_INF_CHECK2,
    ZR_INF_CHECK1,
    ZR_INF_DONE,
    ZR_INF_BAD,
} zr_inflate_mode;


typedef struct zr_inflate_huft {
    uint8_t exop;
    uint8_t bits;
    uint16_t pad;
    uint32_t base;
} zr_inflate_huft;

typedef enum {
    ZR_CODE_START = 0,
    ZR_CODE_LEN,
    ZR_CODE_LENEXT,
    ZR_CODE_DIST,
    ZR_CODE_DISTEXT,
    ZR_CODE_COPY,
    ZR_CODE_LIT,
    ZR_CODE_WASH,
    ZR_CODE_END,
    ZR_CODE_BADCODE,
} zr_inflate_codes_mode;

typedef struct zr_inflate_codes_state {
    zr_inflate_codes_mode mode;
    uint32_t len;
    union {
        struct {
            zr_inflate_huft *tree;
            uint32_t need;
        } code;
        uint32_t lit;
        struct {
            uint32_t get;
            uint32_t dist;
        } copy;
    } sub;
    uint8_t lbits;
    uint8_t dbits;
    zr_inflate_huft *ltree;
    zr_inflate_huft *dtree;
} zr_inflate_codes_state;

typedef enum {
    ZR_BLOCK_TYPE = 0,
    ZR_BLOCK_LENS,
    ZR_BLOCK_STORED,
    ZR_BLOCK_TABLE,
    ZR_BLOCK_BTREE,
    ZR_BLOCK_DTREE,
    ZR_BLOCK_CODES,
    ZR_BLOCK_DRY,
    ZR_BLOCK_DONE,
    ZR_BLOCK_BAD,
} zr_inflate_block_mode;

typedef struct zr_inflate_blocks_state {
    zr_inflate_block_mode mode;
    union {
        uint32_t left;
        struct {
            uint32_t table;
            uint32_t index;
            uint32_t *blens;
            uint32_t bb;
            zr_inflate_huft *tb;
        } trees;
        struct {
            zr_inflate_codes_state *codes;
        } decode;
    } sub;
    uint32_t last;
    uint32_t bitk;
    uint64_t bitb;
    zr_inflate_huft *hufts;
    uint8_t *window;
    uint8_t *end;
    uint8_t *read;
    uint8_t *write;
    zr_check_func checkfn;
    uint64_t check;
} zr_inflate_blocks_state;


typedef struct zr_inflate_state {
    zr_inflate_mode mode;
    union {
        uint32_t method;
        struct {
            uint64_t was;
            uint64_t need;
        } check;
        uint32_t marker;
    } sub;
    int nowrap;
    uint32_t wbits;
    void *blocks;
} zr_inflate_state;

/* zutil / block-engine functions identified elsewhere in the target. */
extern const char *zlibVersion_00198a58(void);
extern const char *zError_00198a64(int);
extern const uint32_t zr_inflate_mask[17];
extern void *zcalloc_00198a84(void *, unsigned, unsigned);
extern void zcfree_00198aa4(void *, void *);
extern uint64_t adler32_recovered(uint64_t, const uint8_t *, unsigned);
extern void inflate_blocks_reset_recovered(zr_inflate_blocks_state *, zr_stream *, uint64_t *);
extern int inflate_blocks_free_recovered(zr_inflate_blocks_state *, zr_stream *);
extern zr_inflate_blocks_state *inflate_blocks_new_recovered(zr_stream *, zr_check_func, unsigned);
extern int inflate_blocks_recovered(zr_inflate_blocks_state *, zr_stream *, int);
extern void inflate_set_dictionary_recovered(zr_inflate_blocks_state *, const uint8_t *, unsigned);
extern int inflate_blocks_sync_point_recovered(zr_inflate_blocks_state *);
extern zr_inflate_codes_state *inflate_codes_new_recovered(unsigned, unsigned,
    zr_inflate_huft *, zr_inflate_huft *, zr_stream *);
extern int inflate_codes_recovered(zr_inflate_blocks_state *, zr_stream *, int);
extern void inflate_codes_free_recovered(zr_inflate_codes_state *, zr_stream *);
extern int inflate_fast_recovered(unsigned, unsigned, zr_inflate_huft *,
    zr_inflate_huft *, zr_inflate_blocks_state *, zr_stream *);
extern int inflate_trees_bits_recovered(uint32_t *, uint32_t *,
    zr_inflate_huft **, zr_inflate_huft *, zr_stream *);
extern int inflate_trees_dynamic_recovered(unsigned, unsigned, uint32_t *,
    uint32_t *, uint32_t *, zr_inflate_huft **, zr_inflate_huft **,
    zr_inflate_huft *, zr_stream *);
extern int inflate_trees_fixed_recovered(uint32_t *, uint32_t *,
    zr_inflate_huft **, zr_inflate_huft **, zr_stream *);
extern int inflate_flush_recovered(zr_inflate_blocks_state *, zr_stream *, int);

/* trees.c recovery, target range 0x00196980..0x00198a80. */
extern void _tr_init_recovered(zr_deflate_state *);
extern void _tr_stored_block_recovered(zr_deflate_state *, uint8_t *, uint64_t, int);
extern void _tr_align_recovered(zr_deflate_state *);
extern void _tr_flush_block_recovered(zr_deflate_state *, uint8_t *, uint64_t, int);
extern int _tr_tally_recovered(zr_deflate_state *, unsigned, unsigned);

#endif
