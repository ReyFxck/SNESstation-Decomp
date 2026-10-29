/*
 * SNES Station v0.23 WIP — zlib 1.1.3 zutil/adler32 recovery.
 *
 * Target functions:
 *   0x00198a58 zlibVersion
 *   0x00198a64 zError
 *   0x00198a84 zcalloc
 *   0x00198aa4 zcfree
 *   0x00198ac0 adler32
 *
 * The boundaries, constants and control flow are independently visible in
 * the target. Historical zlib 1.1.3 source is used only to recover the
 * library-level names and C semantics. This is not a MATCHING claim.
 */
#include "../../include/zlib_1_1_3_recovered.h"

#include <stdlib.h>

#define ZR_ADLER_BASE 65521u
#define ZR_ADLER_NMAX 5552u

static const char *const zr_errmsg[10] = {
    "need dictionary",
    "stream end",
    "",
    "file error",
    "stream error",
    "data error",
    "insufficient memory",
    "buffer error",
    "incompatible version",
    "",
};

/* Target VA 0x00198a58. */
const char *zlibVersion_00198a58(void)
{
    return ZR_VERSION;
}

/* Target VA 0x00198a64. */
const char *zError_00198a64(int err)
{
    return zr_errmsg[ZR_NEED_DICT - err];
}

/* Target VA 0x00198a84. */
void *zcalloc_00198a84(void *opaque, unsigned items, unsigned size)
{
    (void)opaque;
    return calloc(items, size);
}

/* Target VA 0x00198aa4. */
void zcfree_00198aa4(void *opaque, void *address)
{
    (void)opaque;
    free(address);
}

/* Target VA 0x00198ac0. */
uint64_t adler32_recovered(uint64_t adler, const uint8_t *buf, unsigned len)
{
    uint64_t s1 = adler & 0xffffu;
    uint64_t s2 = (adler >> 16) & 0xffffu;

    if (buf == NULL)
        return 1u;

    while (len != 0u) {
        unsigned k = len < ZR_ADLER_NMAX ? len : ZR_ADLER_NMAX;
        len -= k;

        while (k >= 16u) {
            s1 += buf[0];  s2 += s1;
            s1 += buf[1];  s2 += s1;
            s1 += buf[2];  s2 += s1;
            s1 += buf[3];  s2 += s1;
            s1 += buf[4];  s2 += s1;
            s1 += buf[5];  s2 += s1;
            s1 += buf[6];  s2 += s1;
            s1 += buf[7];  s2 += s1;
            s1 += buf[8];  s2 += s1;
            s1 += buf[9];  s2 += s1;
            s1 += buf[10]; s2 += s1;
            s1 += buf[11]; s2 += s1;
            s1 += buf[12]; s2 += s1;
            s1 += buf[13]; s2 += s1;
            s1 += buf[14]; s2 += s1;
            s1 += buf[15]; s2 += s1;
            buf += 16;
            k -= 16u;
        }

        while (k-- != 0u) {
            s1 += *buf++;
            s2 += s1;
        }

        s1 %= ZR_ADLER_BASE;
        s2 %= ZR_ADLER_BASE;
    }

    return (s2 << 16) | s1;
}
