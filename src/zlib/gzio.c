/*
 * Recovered gzip I/O layer from SNES Station v0.23.
 * Target range: 0x00193298..0x00194627.
 *
 * The algorithms are zlib 1.1.3 gzio.c, but this PS2 port replaced FILE *
 * with integer file handles and thin fio-style wrappers. Target-specific
 * divergences are preserved here instead of silently restoring desktop zlib:
 *   - gz_open ignores its fd argument; gzdopen therefore formats "<fd:%d>"
 *     and then attempts to open that string as a path.
 *   - file is initialized to 0, not -1; destroy skips close only for < 0.
 *   - gzflush does not call fflush after do_flush.
 *   - the gzip header OS byte is 3.
 */
#include "zlib_1_1_3_recovered.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZR_GZ_BUFSIZE 16384u
#define ZR_GZ_PRINTF_BUFSIZE 4096u
#define ZR_GZ_READ_FLAGS 0x0001
#define ZR_GZ_WRITE_FLAGS 0x0202
#define ZR_GZIP_OS_CODE 3

#define ZR_ASCII_FLAG  0x01
#define ZR_HEAD_CRC    0x02
#define ZR_EXTRA_FIELD 0x04
#define ZR_ORIG_NAME   0x08
#define ZR_COMMENT     0x10
#define ZR_RESERVED    0xe0

/* Existing PS2 file-wrapper entry points, already identified elsewhere. */
extern int fioOpen_like(const char *path, int flags);          /* 0x0019cfc0 */
extern int fioClose_like(int fd);                              /* 0x0019d090 */
extern int fioRead_like(int fd, void *dst, int size);          /* 0x0019d120 */
extern int fioWrite_like(int fd, const void *src, int size);   /* 0x0019d244 */
extern int64_t fioSeek_like_0019d360(int fd, int64_t off, int whence);
extern int fioPutc_like_0019d534(int c, int fd);

extern int deflateInit2__recovered(zr_stream *, int, int, int, int, int,
                                   const char *, int);
extern int deflateParams_recovered(zr_stream *, int, int);
extern int deflate_00190fa0(zr_stream *, int);
extern int deflateEnd_recovered(zr_stream *);
extern int inflateInit2__recovered(zr_stream *, int, const char *, int);
extern int inflate_recovered(zr_stream *, int);
extern int inflateReset_recovered(zr_stream *);
extern int inflateEnd_recovered(zr_stream *);
extern uint64_t crc32_recovered(uint64_t, const uint8_t *, uint32_t);

/* Logical host representation. Target object size is independently 0x80. */
typedef struct zr_gz_stream {
    zr_stream stream;       /* target +0x00 .. +0x47 */
    int z_err;              /* +0x48 */
    int z_eof;              /* +0x4c */
    int file;               /* +0x50 */
    uint8_t *inbuf;         /* +0x54 */
    uint8_t *outbuf;        /* +0x58 */
    uint64_t crc;           /* +0x60 */
    char *msg;              /* +0x68 */
    char *path;             /* +0x6c */
    int transparent;        /* +0x70 */
    char mode;              /* +0x74 */
    int64_t startpos;       /* +0x78 */
} zr_gz_stream;

static int get_byte_00193674(zr_gz_stream *s);
static void check_header_0019371c(zr_gz_stream *s);
static int destroy_00193914(zr_gz_stream *s);
static int do_flush_00193f88(zr_gz_stream *s, int flush);
static void putLong_001943c0(int fd, uint64_t x);
static uint64_t getLong_0019441c(zr_gz_stream *s);

static int fio_error_like_00193290(int fd)
{
    /* Target 0x00193290 is literally `return fd < 0;`. */
    return fd < 0;
}

/* 0x00193298 */
void *gz_open_00193298(const char *path, const char *mode, int fd)
{
    int err;
    int level = ZR_DEFAULT_COMPRESSION;
    int strategy = ZR_DEFAULT_STRATEGY;
    int open_flags = 0;
    const char *p = mode;
    zr_gz_stream *s;

    (void)fd; /* Proven target quirk: the third argument is never consumed. */
    if (path == NULL || mode == NULL) return NULL;

    s = (zr_gz_stream *)malloc(sizeof(*s));
    if (s == NULL) return NULL;
    memset(s, 0, sizeof(*s));

    /* Target leaves this as zero; do not change it to -1. */
    s->file = 0;
    s->crc = crc32_recovered(0, NULL, 0);

    s->path = (char *)malloc(strlen(path) + 1u);
    if (s->path == NULL) {
        (void)destroy_00193914(s);
        return NULL;
    }
    strcpy(s->path, path);

    s->mode = '\0';
    while (*p != '\0') {
        unsigned char ch = (unsigned char)*p++;
        if (ch == 'r') {
            s->mode = 'r';
            open_flags |= ZR_GZ_READ_FLAGS;
        } else if (ch == 'w' || ch == 'a') {
            s->mode = 'w';
            open_flags |= ZR_GZ_WRITE_FLAGS;
        } else if (ch >= '0' && ch <= '9') {
            level = (int)(ch - '0');
        } else if (ch == 'f') {
            strategy = ZR_FILTERED;
        } else if (ch == 'h') {
            strategy = ZR_HUFFMAN_ONLY;
        }
    }
    if (s->mode == '\0') {
        (void)destroy_00193914(s);
        return NULL;
    }

    if (s->mode == 'w') {
        err = deflateInit2__recovered(&s->stream, level, ZR_DEFLATED,
                                      -ZR_MAX_WBITS, ZR_DEF_MEM_LEVEL,
                                      strategy, ZR_VERSION,
                                      ZR_TARGET_STREAM_SIZE);
        s->stream.next_out = s->outbuf = (uint8_t *)malloc(ZR_GZ_BUFSIZE);
        if (err != ZR_OK || s->outbuf == NULL) {
            (void)destroy_00193914(s);
            return NULL;
        }
    } else {
        s->stream.next_in = s->inbuf = (uint8_t *)malloc(ZR_GZ_BUFSIZE);
        err = inflateInit2__recovered(&s->stream, -ZR_MAX_WBITS,
                                      ZR_VERSION, ZR_TARGET_STREAM_SIZE);
        if (err != ZR_OK || s->inbuf == NULL) {
            (void)destroy_00193914(s);
            return NULL;
        }
    }
    s->stream.avail_out = ZR_GZ_BUFSIZE;

    s->file = fioOpen_like(path, open_flags);
    if (s->file < 0) {
        (void)destroy_00193914(s);
        return NULL;
    }

    if (s->mode == 'w') {
        const uint8_t hdr[10] = {
            0x1f, 0x8b, ZR_DEFLATED, 0, 0, 0, 0, 0, 0, ZR_GZIP_OS_CODE
        };
        (void)fioWrite_like(s->file, hdr, 10);
        s->startpos = 10;
    } else {
        check_header_0019371c(s);
        s->startpos = fioSeek_like_0019d360(s->file, 0, SEEK_CUR) -
                      (int64_t)s->stream.avail_in;
    }
    return s;
}

/* 0x00193564 */
void *gzopen_00193564(const char *path, const char *mode)
{
    return gz_open_00193298(path, mode, -1);
}

/* 0x00193580 */
void *gzdopen_00193580(int fd, const char *mode)
{
    char name[20];
    if (fd < 0) return NULL;
    sprintf(name, "<fd:%d>", fd);
    return gz_open_00193298(name, mode, fd);
}

/* 0x001935d8 */
int gzsetparams_001935d8(void *file, int level, int strategy)
{
    zr_gz_stream *s = (zr_gz_stream *)file;
    if (s == NULL || s->mode != 'w') return ZR_STREAM_ERROR;

    if (s->stream.avail_out == 0) {
        s->stream.next_out = s->outbuf;
        if (fioWrite_like(s->file, s->outbuf, (int)ZR_GZ_BUFSIZE) !=
            (int)ZR_GZ_BUFSIZE) {
            s->z_err = ZR_ERRNO;
        }
        s->stream.avail_out = ZR_GZ_BUFSIZE;
    }
    return deflateParams_recovered(&s->stream, level, strategy);
}

/* 0x00193674 */
static int get_byte_00193674(zr_gz_stream *s)
{
    if (s->z_eof) return -1;
    if (s->stream.avail_in == 0) {
        int n = fioRead_like(s->file, s->inbuf, (int)ZR_GZ_BUFSIZE);
        s->stream.avail_in = n > 0 ? (uint32_t)n : 0u;
        if (s->stream.avail_in == 0) {
            s->z_eof = 1;
            if (fio_error_like_00193290(s->file)) s->z_err = ZR_ERRNO;
            return -1;
        }
        s->stream.next_in = s->inbuf;
    }
    s->stream.avail_in--;
    return (int)*s->stream.next_in++;
}

/* 0x0019371c */
static void check_header_0019371c(zr_gz_stream *s)
{
    static const int magic[2] = {0x1f, 0x8b};
    unsigned len;
    int method, flags, c;

    for (len = 0; len < 2; ++len) {
        c = get_byte_00193674(s);
        if (c != magic[len]) {
            if (len != 0) {
                s->stream.avail_in++;
                s->stream.next_in--;
            }
            if (c != -1) {
                s->stream.avail_in++;
                s->stream.next_in--;
                s->transparent = 1;
            }
            s->z_err = s->stream.avail_in != 0 ? ZR_OK : ZR_STREAM_END;
            return;
        }
    }

    method = get_byte_00193674(s);
    flags = get_byte_00193674(s);
    if (method != ZR_DEFLATED || (flags & ZR_RESERVED) != 0) {
        s->z_err = ZR_DATA_ERROR;
        return;
    }
    for (len = 0; len < 6; ++len) (void)get_byte_00193674(s);

    if ((flags & ZR_EXTRA_FIELD) != 0) {
        len = (unsigned)get_byte_00193674(s);
        len += (unsigned)get_byte_00193674(s) << 8;
        while (len-- != 0 && get_byte_00193674(s) != -1) {}
    }
    if ((flags & ZR_ORIG_NAME) != 0) {
        while ((c = get_byte_00193674(s)) != 0 && c != -1) {}
    }
    if ((flags & ZR_COMMENT) != 0) {
        while ((c = get_byte_00193674(s)) != 0 && c != -1) {}
    }
    if ((flags & ZR_HEAD_CRC) != 0) {
        (void)get_byte_00193674(s);
        (void)get_byte_00193674(s);
    }
    s->z_err = s->z_eof ? ZR_DATA_ERROR : ZR_OK;
}

/* 0x00193914 */
static int destroy_00193914(zr_gz_stream *s)
{
    int err = ZR_OK;
    if (s == NULL) return ZR_STREAM_ERROR;

    free(s->msg);
    if (s->stream.state != NULL) {
        if (s->mode == 'w') err = deflateEnd_recovered(&s->stream);
        else if (s->mode == 'r') err = inflateEnd_recovered(&s->stream);
    }
    if (s->file >= 0 && fioClose_like(s->file) != 0) err = ZR_ERRNO;
    if (s->z_err < 0) err = s->z_err;

    free(s->inbuf);
    free(s->outbuf);
    free(s->path);
    free(s);
    return err;
}

/* 0x00193a34 */
int gzread_00193a34(void *file, void *buf, unsigned len)
{
    zr_gz_stream *s = (zr_gz_stream *)file;
    uint8_t *start = (uint8_t *)buf;

    if (s == NULL || s->mode != 'r') return ZR_STREAM_ERROR;
    if (s->z_err == ZR_DATA_ERROR || s->z_err == ZR_ERRNO) return -1;
    if (s->z_err == ZR_STREAM_END) return 0;

    s->stream.next_out = (uint8_t *)buf;
    s->stream.avail_out = len;

    while (s->stream.avail_out != 0) {
        if (s->transparent) {
            unsigned n = s->stream.avail_in;
            if (n > s->stream.avail_out) n = s->stream.avail_out;
            if (n != 0) {
                memcpy(s->stream.next_out, s->stream.next_in, n);
                s->stream.next_out += n;
                s->stream.next_in += n;
                s->stream.avail_out -= n;
                s->stream.avail_in -= n;
            }
            if (s->stream.avail_out != 0) {
                int nr = fioRead_like(s->file, s->stream.next_out,
                                      (int)s->stream.avail_out);
                if (nr > 0) s->stream.avail_out -= (uint32_t)nr;
            }
            {
                unsigned got = len - s->stream.avail_out;
                s->stream.total_in += got;
                s->stream.total_out += got;
                if (got == 0) s->z_eof = 1;
                return (int)got;
            }
        }

        if (s->stream.avail_in == 0 && !s->z_eof) {
            int nr = fioRead_like(s->file, s->inbuf, (int)ZR_GZ_BUFSIZE);
            s->stream.avail_in = nr > 0 ? (uint32_t)nr : 0u;
            if (s->stream.avail_in == 0) {
                s->z_eof = 1;
                if (fio_error_like_00193290(s->file)) {
                    s->z_err = ZR_ERRNO;
                    break;
                }
            }
            s->stream.next_in = s->inbuf;
        }

        s->z_err = inflate_recovered(&s->stream, ZR_NO_FLUSH);
        if (s->z_err == ZR_STREAM_END) {
            uint64_t total_in, total_out;
            s->crc = crc32_recovered(s->crc, start,
                (uint32_t)(s->stream.next_out - start));
            start = s->stream.next_out;
            if (getLong_0019441c(s) != s->crc) {
                s->z_err = ZR_DATA_ERROR;
            } else {
                (void)getLong_0019441c(s);
                check_header_0019371c(s);
                if (s->z_err == ZR_OK) {
                    total_in = s->stream.total_in;
                    total_out = s->stream.total_out;
                    (void)inflateReset_recovered(&s->stream);
                    s->stream.total_in = total_in;
                    s->stream.total_out = total_out;
                    s->crc = crc32_recovered(0, NULL, 0);
                }
            }
        }
        if (s->z_err != ZR_OK || s->z_eof) break;
    }

    s->crc = crc32_recovered(s->crc, start,
        (uint32_t)(s->stream.next_out - start));
    return (int)(len - s->stream.avail_out);
}

/* 0x00193cd8 */
int gzgetc_00193cd8(void *file)
{
    unsigned char c;
    return gzread_00193a34(file, &c, 1) == 1 ? (int)c : -1;
}

/* 0x00193d0c */
char *gzgets_00193d0c(void *file, char *buf, int len)
{
    char *b = buf;
    if (buf == NULL || len <= 0) return NULL;
    while (--len > 0 && gzread_00193a34(file, buf, 1) == 1 && *buf++ != '\n') {}
    *buf = '\0';
    return (b == buf && len > 0) ? NULL : b;
}

/* 0x00193dbc */
int gzwrite_00193dbc(void *file, const void *buf, unsigned len)
{
    zr_gz_stream *s = (zr_gz_stream *)file;
    if (s == NULL || s->mode != 'w') return ZR_STREAM_ERROR;

    s->stream.next_in = (uint8_t *)(uintptr_t)buf;
    s->stream.avail_in = len;
    while (s->stream.avail_in != 0) {
        if (s->stream.avail_out == 0) {
            s->stream.next_out = s->outbuf;
            if (fioWrite_like(s->file, s->outbuf, (int)ZR_GZ_BUFSIZE) !=
                (int)ZR_GZ_BUFSIZE) {
                s->z_err = ZR_ERRNO;
                break;
            }
            s->stream.avail_out = ZR_GZ_BUFSIZE;
        }
        s->z_err = deflate_00190fa0(&s->stream, ZR_NO_FLUSH);
        if (s->z_err != ZR_OK) break;
    }
    s->crc = crc32_recovered(s->crc, (const uint8_t *)buf, len);
    return (int)(len - s->stream.avail_in);
}

/* 0x00193e8c -- deliberately uses unbounded vsprintf like the target. */
int gzprintf_00193e8c(void *file, const char *format, ...)
{
    char buf[ZR_GZ_PRINTF_BUFSIZE];
    va_list ap;
    int len;
    va_start(ap, format);
    (void)vsprintf(buf, format, ap);
    va_end(ap);
    len = (int)strlen(buf);
    if (len <= 0) return 0;
    return gzwrite_00193dbc(file, buf, (unsigned)len);
}

/* 0x00193f08 */
int gzputc_00193f08(void *file, int c)
{
    unsigned char cc = (unsigned char)c;
    return gzwrite_00193dbc(file, &cc, 1) == 1 ? (int)cc : -1;
}

/* 0x00193f44 */
int gzputs_00193f44(void *file, const char *str)
{
    return gzwrite_00193dbc(file, str, (unsigned)strlen(str));
}

/* 0x00193f88 */
static int do_flush_00193f88(zr_gz_stream *s, int flush)
{
    int done = 0;
    if (s == NULL || s->mode != 'w') return ZR_STREAM_ERROR;
    s->stream.avail_in = 0;

    for (;;) {
        unsigned len = ZR_GZ_BUFSIZE - s->stream.avail_out;
        if (len != 0) {
            int wrote = fioWrite_like(s->file, s->outbuf, (int)len);
            if (wrote != (int)len) {
                /* Target stores the wrapper return itself here. */
                s->z_err = wrote;
                return s->z_err;
            }
            s->stream.next_out = s->outbuf;
            s->stream.avail_out = ZR_GZ_BUFSIZE;
        }
        if (done) break;
        s->z_err = deflate_00190fa0(&s->stream, flush);
        if (len == 0 && s->z_err == ZR_BUF_ERROR) s->z_err = ZR_OK;
        done = s->stream.avail_out != 0 || s->z_err == ZR_STREAM_END;
        if (s->z_err != ZR_OK && s->z_err != ZR_STREAM_END) break;
    }
    return s->z_err == ZR_STREAM_END ? ZR_OK : s->z_err;
}

/* 0x001940ac -- target has no fflush call. */
int gzflush_001940ac(void *file, int flush)
{
    zr_gz_stream *s = (zr_gz_stream *)file;
    int err = do_flush_00193f88(s, flush);
    if (err != 0) return err;
    return s->z_err == ZR_STREAM_END ? ZR_OK : s->z_err;
}

int gzrewind_001942d4(void *file);

/* 0x001940ec */
int64_t gzseek_001940ec(void *file, int64_t offset, int whence)
{
    zr_gz_stream *s = (zr_gz_stream *)file;
    if (s == NULL || whence == SEEK_END || s->z_err == ZR_ERRNO ||
        s->z_err == ZR_DATA_ERROR) return -1;

    if (s->mode == 'w') {
        if (whence == SEEK_SET) offset -= (int64_t)s->stream.total_in;
        if (offset < 0) return -1;
        if (s->inbuf == NULL) {
            s->inbuf = (uint8_t *)malloc(ZR_GZ_BUFSIZE);
            if (s->inbuf == NULL) return -1;
            memset(s->inbuf, 0, ZR_GZ_BUFSIZE);
        }
        while (offset > 0) {
            unsigned size = ZR_GZ_BUFSIZE;
            int wrote;
            if (offset < (int64_t)ZR_GZ_BUFSIZE) size = (unsigned)offset;
            wrote = gzwrite_00193dbc(s, s->inbuf, size);
            if (wrote == 0) return -1;
            offset -= wrote;
        }
        return (int64_t)s->stream.total_in;
    }

    if (whence == SEEK_CUR) offset += (int64_t)s->stream.total_out;
    if (offset < 0) return -1;

    if (s->transparent) {
        s->stream.avail_in = 0;
        s->stream.next_in = s->inbuf;
        if (fioSeek_like_0019d360(s->file, offset, SEEK_SET) < 0) return -1;
        s->stream.total_in = (uint64_t)offset;
        s->stream.total_out = (uint64_t)offset;
        return offset;
    }

    if ((uint64_t)offset >= s->stream.total_out) {
        offset -= (int64_t)s->stream.total_out;
    } else if (gzrewind_001942d4(s) < 0) {
        return -1;
    }

    if (offset != 0 && s->outbuf == NULL) {
        s->outbuf = (uint8_t *)malloc(ZR_GZ_BUFSIZE);
        if (s->outbuf == NULL) return -1;
    }
    while (offset > 0) {
        unsigned size = ZR_GZ_BUFSIZE;
        int got;
        if (offset < (int64_t)ZR_GZ_BUFSIZE) size = (unsigned)offset;
        got = gzread_00193a34(s, s->outbuf, size);
        if (got <= 0) return -1;
        offset -= got;
    }
    return (int64_t)s->stream.total_out;
}

/* 0x001942d4 */
int gzrewind_001942d4(void *file)
{
    zr_gz_stream *s = (zr_gz_stream *)file;
    if (s == NULL || s->mode != 'r') return -1;

    s->z_err = ZR_OK;
    s->z_eof = 0;
    s->stream.avail_in = 0;
    s->stream.next_in = s->inbuf;
    s->crc = crc32_recovered(0, NULL, 0);

    if (s->startpos == 0) {
        (void)fioSeek_like_0019d360(s->file, 0, SEEK_SET);
        return 0;
    }
    (void)inflateReset_recovered(&s->stream);
    return (int)fioSeek_like_0019d360(s->file, s->startpos, SEEK_SET);
}

/* 0x00194378 */
int64_t gztell_00194378(void *file)
{
    return gzseek_001940ec(file, 0, SEEK_CUR);
}

/* 0x00194398 */
int gzeof_00194398(void *file)
{
    zr_gz_stream *s = (zr_gz_stream *)file;
    return (s == NULL || s->mode != 'r') ? 0 : s->z_eof;
}

/* 0x001943c0 */
static void putLong_001943c0(int fd, uint64_t x)
{
    int n;
    for (n = 0; n < 4; ++n) {
        (void)fioPutc_like_0019d534((int)(x & 0xffu), fd);
        x >>= 8;
    }
}

/* 0x0019441c */
static uint64_t getLong_0019441c(zr_gz_stream *s)
{
    uint64_t x = (uint64_t)(uint32_t)get_byte_00193674(s);
    int c;
    x += (uint64_t)(uint32_t)get_byte_00193674(s) << 8;
    x += (uint64_t)(uint32_t)get_byte_00193674(s) << 16;
    c = get_byte_00193674(s);
    if (c == -1) s->z_err = ZR_DATA_ERROR;
    x += (uint64_t)(uint32_t)c << 24;
    return x;
}

/* 0x00194498 */
int gzclose_00194498(void *file)
{
    zr_gz_stream *s = (zr_gz_stream *)file;
    if (s == NULL) return ZR_STREAM_ERROR;
    if (s->mode == 'w') {
        int err = do_flush_00193f88(s, ZR_FINISH);
        if (err != ZR_OK) return destroy_00193914(s);
        putLong_001943c0(s->file, s->crc);
        putLong_001943c0(s->file, s->stream.total_in);
    }
    return destroy_00193914(s);
}

/* 0x0019450c */
const char *gzerror_0019450c(void *file, int *errnum)
{
    static const char empty[] = "";
    zr_gz_stream *s = (zr_gz_stream *)file;
    const char *m;
    size_t need;

    if (s == NULL) {
        *errnum = ZR_STREAM_ERROR;
        return zError_00198a64(ZR_STREAM_ERROR);
    }
    *errnum = s->z_err;
    if (*errnum == ZR_OK) return empty;

    /* Target does not call strerror for Z_ERRNO; it falls back to zError. */
    m = (*errnum == ZR_ERRNO) ? empty : s->stream.msg;
    if (m == NULL || *m == '\0') m = zError_00198a64(s->z_err);

    free(s->msg);
    need = strlen(s->path) + strlen(m) + 3u;
    s->msg = (char *)malloc(need);
    /* Target does not check this allocation before strcpy. */
    strcpy(s->msg, s->path);
    strcat(s->msg, ": ");
    strcat(s->msg, m);
    return s->msg;
}
