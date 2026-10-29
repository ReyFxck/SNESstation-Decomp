/*
 * SNES Station v0.23 WIP — zlib 1.1.3 buffer API recovery.
 *
 * Target range opened in progress 3: 0x00190700+.
 * The target image contains the literal version string "1.1.3" and the
 * classic zlib 1.1.3 compress/uncompress call structure. This file recovers
 * the small top-level buffer wrappers while leaving the large deflate/inflate
 * engines as separately mapped target functions.
 *
 * PS2 target note: the original build uses -mlong64. This adapter therefore
 * models uLong/uLongf as 64-bit values, but does not claim an exact host ABI
 * layout for z_stream.
 */
#include <stdint.h>
#include <string.h>

#define Z_OK 0
#define Z_STREAM_END 1
#define Z_BUF_ERROR (-5)
#define Z_FINISH 4
#define Z_DEFAULT_COMPRESSION (-1)
#define Z_DEFLATED 8
#define MAX_WBITS 15
#define DEF_MEM_LEVEL 8
#define Z_DEFAULT_STRATEGY 0
#define TARGET_Z_STREAM_SIZE 0x48
#define TARGET_ZLIB_VERSION "1.1.3"

typedef void *(*alloc_func_recovered)(void *, unsigned, unsigned);
typedef void (*free_func_recovered)(void *, void *);

typedef struct {
    uint8_t *next_in;
    uint32_t avail_in;
    uint64_t total_in;
    uint8_t *next_out;
    uint32_t avail_out;
    uint64_t total_out;
    char *msg;
    void *state;
    alloc_func_recovered zalloc;
    free_func_recovered zfree;
    void *opaque;
    int data_type;
    uint64_t adler;
    uint64_t reserved;
} z_stream_recovered;

/* Large mapped target functions. */
extern int deflateInit2__001908ec(z_stream_recovered *, int, int, int, int,
                                  int, const char *, int);
extern int deflate_00190fa0(z_stream_recovered *, int);
extern int deflateEnd_00191308(z_stream_recovered *);
extern int inflateInit__00192948(z_stream_recovered *, const char *, int);
extern int inflate_0019296c(z_stream_recovered *, int);
extern int inflateEnd_00192784(z_stream_recovered *);

/* Target VA 0x001908bc. */
int deflateInit__recovered(z_stream_recovered *strm, int level,
                           const char *version, int stream_size)
{
    return deflateInit2__001908ec(strm, level, Z_DEFLATED, MAX_WBITS,
                                  DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                  version, stream_size);
}

/* Target VA 0x00190700. */
int compress2_recovered(uint8_t *dest, uint64_t *dest_len,
                        const uint8_t *source, uint64_t source_len, int level)
{
    z_stream_recovered stream;
    int err;

    memset(&stream, 0, sizeof(stream));
    stream.next_in = (uint8_t *)source;
    stream.avail_in = (uint32_t)source_len;
    stream.next_out = dest;
    stream.avail_out = (uint32_t)*dest_len;

    /* Present in the target because uInt remains 32-bit while uLong is 64-bit. */
    if ((uint64_t)stream.avail_out != *dest_len)
        return Z_BUF_ERROR;

    stream.zalloc = NULL;
    stream.zfree = NULL;
    stream.opaque = NULL;

    err = deflateInit__recovered(&stream, level, TARGET_ZLIB_VERSION,
                                 TARGET_Z_STREAM_SIZE);
    if (err != Z_OK)
        return err;

    err = deflate_00190fa0(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        (void)deflateEnd_00191308(&stream);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }

    *dest_len = stream.total_out;
    return deflateEnd_00191308(&stream);
}

/* Target VA 0x001907cc. */
int compress_recovered(uint8_t *dest, uint64_t *dest_len,
                       const uint8_t *source, uint64_t source_len)
{
    return compress2_recovered(dest, dest_len, source, source_len,
                               Z_DEFAULT_COMPRESSION);
}

/* Target VA 0x001907e8. */
int uncompress_recovered(uint8_t *dest, uint64_t *dest_len,
                         const uint8_t *source, uint64_t source_len)
{
    z_stream_recovered stream;
    int err;

    memset(&stream, 0, sizeof(stream));
    stream.next_in = (uint8_t *)source;
    stream.avail_in = (uint32_t)source_len;
    if ((uint64_t)stream.avail_in != source_len)
        return Z_BUF_ERROR;

    stream.next_out = dest;
    stream.avail_out = (uint32_t)*dest_len;
    if ((uint64_t)stream.avail_out != *dest_len)
        return Z_BUF_ERROR;

    stream.zalloc = NULL;
    stream.zfree = NULL;

    err = inflateInit__00192948(&stream, TARGET_ZLIB_VERSION,
                                TARGET_Z_STREAM_SIZE);
    if (err != Z_OK)
        return err;

    err = inflate_0019296c(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        (void)inflateEnd_00192784(&stream);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }

    *dest_len = stream.total_out;
    return inflateEnd_00192784(&stream);
}
