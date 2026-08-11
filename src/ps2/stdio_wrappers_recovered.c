#include <stdarg.h>
#include <stddef.h>

/*
 * Small stdio wrappers recovered at the boundary between the old ps2lib
 * libkernel formatter objects.
 *
 * Historical symbol names are validated only after target-side boundary/call
 * analysis.  The formatter core at 0x0019d84c..0x0019e363 is reconstructed
 * in ps2lib_formatter_recovered.c.
 */

/* Target: 0x0019e2e0, reconstructed in ps2lib_formatter_recovered.c. */
extern int ps2lib_vsnprintf_recovered(char *dst,
                                      size_t size,
                                      const char *fmt,
                                      va_list ap);

/* Target: 0x0019e364. */
int vsprintf_recovered(char *dst, const char *fmt, va_list ap)
{
    return ps2lib_vsnprintf_recovered(dst, 0x1000u, fmt, ap);
}

/* Target: 0x0019e3d0. */
int sprintf_recovered(char *dst, const char *fmt, ...)
{
    int result;
    va_list ap;

    va_start(ap, fmt);
    result = vsprintf_recovered(dst, fmt, ap);
    va_end(ap);

    return result;
}

extern int fioWrite_0019d244(int fd, const void *ptr, int size);

/*
 * Target: 0x0019faa8.
 * Historical PS2LIB names this vprintf.  The target owns one 0x1000-byte BSS
 * staging buffer, formats into it, writes exactly the formatter return count
 * to fd 1, ignores fioWrite's return value, and returns the format count.
 */
int vprintf_recovered(const char *fmt, va_list ap)
{
    static char stdout_buffer[0x1000];
    int length = ps2lib_vsnprintf_recovered(stdout_buffer,
                                             sizeof(stdout_buffer),
                                             fmt, ap);

    (void)fioWrite_0019d244(1, stdout_buffer, length);
    return length;
}

/* Target: 0x0019e388. */
int printf_recovered(const char *fmt, ...)
{
    int result;
    va_list ap;

    va_start(ap, fmt);
    result = vprintf_recovered(fmt, ap);
    va_end(ap);

    return result;
}

/*
 * Target: 0x0019e414.
 *
 * This is deliberately not named plain `puts`: the target does not append a
 * newline and ignores fioWrite's return value.  It returns the measured string
 * length instead.
 */
int puts_like_recovered(const char *text)
{
    int length = 0;

    while (text[length] != '\0')
        length++;

    (void)fioWrite_0019d244(1, text, length);
    return length;
}

#include "../../include/ps2_libkernel_recovered.h"

/*
 * Logical target output object used by old ps2lib vsnprintf.
 * All address/callback fields are explicitly 32-bit so the offsets remain:
 *   +00 start, +04 current, +08 end, +0c size, +10 state,
 *   +14 putc callback, +18 room-check callback.
 */
typedef struct {
    ee_addr32_t start;
    ee_addr32_t current;
    ee_addr32_t end;
    uint32_t size;
    uint32_t state;
    ee_addr32_t putc_callback;
    ee_addr32_t room_callback;
} ps2lib_snprintf_output32;

typedef char recovered_assert_snprintf_output_size[
    (sizeof(ps2lib_snprintf_output32) == 0x1c) ? 1 : -1];

/* Target: 0x0019e274. Returns nonzero if `count` bytes would pass end. */
int ps2lib_snprintf_room_0019e274(ps2lib_snprintf_output32 *out,
                                  unsigned int count)
{
    uint32_t next = out->current + count;
    return out->end < next;
}

/* Target: 0x0019e288. The callback returns 1 on overflow and 0 on success. */
int ps2lib_snprintf_putc_0019e288(ps2lib_snprintf_output32 *out, int value)
{
    unsigned char *dst;

    if (ps2lib_snprintf_room_0019e274(out, 1u))
        return 1;

    dst = (unsigned char *)ee_ptr_from_addr32(out->current);
    *dst = (unsigned char)value;
    out->current += 1u;
    return 0;
}
