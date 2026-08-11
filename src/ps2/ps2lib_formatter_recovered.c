#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

/*
 * Old PS2LIB formatter core recovered from SNES Station v0.23.
 *
 * Target corridor:
 *   0x0019d84c fmtint
 *   0x0019dba8 fmtstr
 *   0x0019dd28 fmtchar
 *   0x0019de10 dopr
 *   0x0019e274 bounded-output room check
 *   0x0019e288 bounded-output put-char callback
 *   0x0019e2e0 vsnprintf
 *
 * The parser is recognisably descended from the old Patrick Powell snprintf
 * family, but this file models the target observed in SNES Station rather
 * than substituting a newer/general implementation.  In particular, output
 * is callback-driven, floating-point conversions are not handled here, and
 * several non-C99 quirks are intentionally retained.
 *
 * The real EE build uses 32-bit addresses and eight-byte vararg slots.  This
 * behavioural model uses uintptr_t/host va_list so it can be syntax-checked
 * on a modern host; exact 32-bit output-structure offsets are documented and
 * asserted separately in stdio_wrappers_recovered.c.
 */

enum {
    DP_F_MINUS    = 1u << 0,
    DP_F_PLUS     = 1u << 1,
    DP_F_SPACE    = 1u << 2,
    DP_F_NUM      = 1u << 3,
    DP_F_ZERO     = 1u << 4
};

typedef struct ps2lib_output_recovered ps2lib_output_recovered;
typedef int (*ps2lib_putc_fn)(ps2lib_output_recovered *, int);
typedef int (*ps2lib_room_fn)(ps2lib_output_recovered *, unsigned int);

struct ps2lib_output_recovered {
    uintptr_t start;
    uintptr_t current;
    uintptr_t end;
    uint32_t size;
    uint32_t state;
    ps2lib_putc_fn putc_cb;
    ps2lib_room_fn room_cb;
};

static int output_room(ps2lib_output_recovered *out, unsigned int count)
{
    uintptr_t next = out->current + (uintptr_t)count;
    return out->end < next;
}

static int output_putc(ps2lib_output_recovered *out, int value)
{
    if (out->room_cb(out, 1u))
        return 1;

    *(unsigned char *)out->current = (unsigned char)value;
    out->current += 1u;
    return 0;
}

static void reverse_recent(ps2lib_output_recovered *out, int count)
{
    unsigned char *first;
    unsigned char *last;

    if (count <= 1)
        return;

    first = (unsigned char *)(out->current - (uintptr_t)count);
    last = (unsigned char *)(out->current - 1u);

    while (first < last) {
        unsigned char tmp = *first;
        *first++ = *last;
        *last-- = tmp;
    }
}

/* Target: 0x0019d84c.  Returns 0 on success and 1 on output overflow. */
int ps2lib_fmtint_0019d84c(ps2lib_output_recovered *out,
                            unsigned long value,
                            unsigned int base,
                            const char *digits,
                            int min_width,
                            int max_digits,
                            unsigned int flags,
                            int negative)
{
    int emitted = 0;
    unsigned long work = value;
    int left;

    if (max_digits == -1) {
        /* Default integer precision in the target is one digit. */
        max_digits = 1;
    } else {
        /* Explicit precision disables zero field-padding. */
        flags &= ~DP_F_ZERO;

        /* Target quirk: %.0d with zero returns before width/sign/prefix. */
        if (max_digits == 0 && work == 0)
            return 0;
    }

    do {
        unsigned int digit = (unsigned int)(work % (unsigned long)base);
        if (out->putc_cb(out, (unsigned char)digits[digit]))
            return 1;
        emitted++;
        work /= (unsigned long)base;
    } while (work != 0);

    max_digits -= emitted;
    while (max_digits > 0) {
        if (out->putc_cb(out, '0'))
            return 1;
        emitted++;
        max_digits--;
    }

    /* Prefix bytes are counted before they are appended to the reversed run. */
    if (flags & DP_F_NUM) {
        if (base == 16u)
            emitted += 2;
        else if (base == 8u)
            emitted += 1;
    }

    if (flags & DP_F_ZERO) {
        int zero_count = min_width - emitted;

        if (negative || (flags & (DP_F_PLUS | DP_F_SPACE)))
            zero_count--;

        while (zero_count > 0) {
            if (out->putc_cb(out, '0'))
                return 1;
            emitted++;
            zero_count--;
        }

        /* No ordinary spaces remain after the target's zero-pad path. */
        min_width = 0;
    }

    if (flags & DP_F_NUM) {
        if (base == 16u) {
            /* digits[10] is A/a; +0x17 produces X/x in the target. */
            if (out->putc_cb(out, (unsigned char)(digits[10] + 0x17)))
                return 1;
            if (out->putc_cb(out, '0'))
                return 1;
        } else if (base == 8u) {
            if (out->putc_cb(out, '0'))
                return 1;
        }
    }

    if (negative) {
        if (out->putc_cb(out, '-'))
            return 1;
        emitted++;
    } else if (flags & DP_F_PLUS) {
        if (out->putc_cb(out, '+'))
            return 1;
        emitted++;
    } else if (flags & DP_F_SPACE) {
        if (out->putc_cb(out, ' '))
            return 1;
        emitted++;
    }

    left = (flags & DP_F_MINUS) != 0;

    /* Digits/prefix/sign were deliberately emitted backwards. */
    if (left)
        reverse_recent(out, emitted);

    min_width -= emitted;
    while (min_width > 0) {
        if (out->putc_cb(out, ' '))
            return 1;
        emitted++;
        min_width--;
    }

    /* For right justification, reversing after spaces moves padding left. */
    if (!left)
        reverse_recent(out, emitted);

    return 0;
}

/* Target: 0x0019dba8.  Returns 0 on success and 1 on output overflow. */
int ps2lib_fmtstr_0019dba8(ps2lib_output_recovered *out,
                            const char *value,
                            int min_width,
                            int max_chars,
                            unsigned int flags)
{
    int pad;
    int left = (flags & DP_F_MINUS) != 0;

    /*
     * Target quirk: with an explicit max it pads against max_chars itself,
     * not min(strlen(value), max_chars).
     */
    if (max_chars == -1) {
        int len = 0;
        while (value[len] != '\0')
            len++;
        pad = min_width - len;
    } else {
        pad = min_width - max_chars;
    }

    if (!left) {
        int n = pad;
        while (n > 0) {
            if (out->putc_cb(out, ' '))
                return 1;
            n--;
        }
    }

    if (max_chars == -1) {
        while (*value != '\0') {
            if (out->putc_cb(out, (unsigned char)*value++))
                return 1;
        }
    } else {
        int remaining = max_chars;
        while (*value != '\0' && remaining > 0) {
            if (out->putc_cb(out, (unsigned char)*value++))
                return 1;
            remaining--;
        }
    }

    if (left) {
        int n = pad;
        while (n > 0) {
            if (out->putc_cb(out, ' '))
                return 1;
            n--;
        }
    }

    return 0;
}

/* Target: 0x0019dd28.  Returns 0 on success and 1 on output overflow. */
int ps2lib_fmtchar_0019dd28(ps2lib_output_recovered *out,
                             int value,
                             int min_width,
                             unsigned int flags)
{
    int pad = min_width - 1;
    int left = (flags & DP_F_MINUS) != 0;

    if (!left) {
        int n = pad;
        while (n > 0) {
            if (out->putc_cb(out, ' '))
                return 1;
            n--;
        }
    }

    if (out->putc_cb(out, (unsigned char)value))
        return 1;

    if (left) {
        while (pad > 0) {
            if (out->putc_cb(out, ' '))
                return 1;
            pad--;
        }
    }

    return 0;
}

static unsigned long next_unsigned(va_list *ap, int is_long, int is_short)
{
    if (is_long)
        return va_arg(*ap, unsigned long);
    if (is_short)
        return (unsigned short)va_arg(*ap, unsigned int);
    return va_arg(*ap, unsigned int);
}

static long next_signed(va_list *ap, int is_long, int is_short)
{
    if (is_long)
        return va_arg(*ap, long);
    if (is_short)
        return (short)va_arg(*ap, int);
    return va_arg(*ap, int);
}

/* Target: 0x0019de10.  Returns 0 on success and -1 on output failure. */
int ps2lib_dopr_0019de10(ps2lib_output_recovered *out,
                          const char *format,
                          va_list incoming)
{
    static const char digits_upper[] = "0123456789ABCDEF";
    static const char digits_dec[] = "0123456789";
    static const char digits_oct[] = "01234567";
    static const char digits_lower[] = "0123456789abcdef";
    va_list ap;
    const unsigned char *p = (const unsigned char *)format;

    va_copy(ap, incoming);

    while (*p != '\0') {
        unsigned int ch = *p++;

        if (ch != '%') {
            if (out->putc_cb(out, (int)ch)) {
                va_end(ap);
                return -1;
            }
            continue;
        }

        {
            unsigned int flags = 0;
            int min_width = 0;
            int max_chars = -1;
            int is_long = 0;
            int is_short = 0;

            ch = *p++;

            for (;;) {
                if (ch == '-')
                    flags |= DP_F_MINUS;
                else if (ch == '+')
                    flags |= DP_F_PLUS;
                else if (ch == ' ')
                    flags |= DP_F_SPACE;
                else if (ch == '#')
                    flags |= DP_F_NUM;
                else if (ch == '0')
                    flags |= DP_F_ZERO;
                else
                    break;

                ch = *p++;
            }

            /* '+' wins over space; '-' wins over zero-padding. */
            if ((flags & (DP_F_PLUS | DP_F_SPACE)) ==
                (DP_F_PLUS | DP_F_SPACE))
                flags ^= DP_F_SPACE;
            if ((flags & (DP_F_MINUS | DP_F_ZERO)) ==
                (DP_F_MINUS | DP_F_ZERO))
                flags ^= DP_F_ZERO;

            if (ch >= '0' && ch <= '9') {
                do {
                    min_width = min_width * 10 + (int)(ch - '0');
                    ch = *p++;
                } while (ch >= '0' && ch <= '9');
            } else if (ch == '*') {
                min_width = va_arg(ap, int);
                ch = *p++;
            }

            if (ch == '.') {
                max_chars = 0;
                ch = *p++;

                if (ch >= '0' && ch <= '9') {
                    do {
                        max_chars = max_chars * 10 + (int)(ch - '0');
                        ch = *p++;
                    } while (ch >= '0' && ch <= '9');
                } else if (ch == '*') {
                    max_chars = va_arg(ap, int);
                    ch = *p++;
                }
            }

            if (ch == 'h') {
                is_short = 1;
                ch = *p++;
            } else if (ch == 'l') {
                is_long = 1;
                ch = *p++;
            }

            switch (ch) {
            case '\0':
                /* Target quirk for a trailing bare '%': emit a NUL byte. */
                if (out->putc_cb(out, 0)) {
                    va_end(ap);
                    return -1;
                }
                va_end(ap);
                return 0;

            case '%':
                if (out->putc_cb(out, '%')) {
                    va_end(ap);
                    return -1;
                }
                break;

            case 'c':
                if (ps2lib_fmtchar_0019dd28(out,
                                             (unsigned char)va_arg(ap, int),
                                             min_width,
                                             flags)) {
                    va_end(ap);
                    return -1;
                }
                break;

            case 'd':
            case 'i': {
                long signed_value = next_signed(&ap, is_long, is_short);
                int negative = signed_value < 0;
                unsigned long magnitude;

                if (negative)
                    magnitude = (unsigned long)(-(unsigned long)signed_value);
                else
                    magnitude = (unsigned long)signed_value;

                if (ps2lib_fmtint_0019d84c(out, magnitude, 10u, digits_dec,
                                            min_width, max_chars, flags,
                                            negative)) {
                    va_end(ap);
                    return -1;
                }
                break;
            }

            case 'o':
                if (ps2lib_fmtint_0019d84c(out,
                                            next_unsigned(&ap, is_long, is_short),
                                            8u, digits_oct,
                                            min_width, max_chars, flags, 0)) {
                    va_end(ap);
                    return -1;
                }
                break;

            case 'u':
                if (ps2lib_fmtint_0019d84c(out,
                                            next_unsigned(&ap, is_long, is_short),
                                            10u, digits_dec,
                                            min_width, max_chars, flags, 0)) {
                    va_end(ap);
                    return -1;
                }
                break;

            case 'x':
                if (ps2lib_fmtint_0019d84c(out,
                                            next_unsigned(&ap, is_long, is_short),
                                            16u, digits_lower,
                                            min_width, max_chars, flags, 0)) {
                    va_end(ap);
                    return -1;
                }
                break;

            case 'X':
                if (ps2lib_fmtint_0019d84c(out,
                                            next_unsigned(&ap, is_long, is_short),
                                            16u, digits_upper,
                                            min_width, max_chars, flags, 0)) {
                    va_end(ap);
                    return -1;
                }
                break;

            case 'p': {
                uintptr_t ptr_value = (uintptr_t)va_arg(ap, void *);
                /* EE pointers are 32-bit even with -mlong64. */
                if (ps2lib_fmtint_0019d84c(out,
                                            (unsigned long)(uint32_t)ptr_value,
                                            16u, digits_upper,
                                            min_width, max_chars, flags, 0)) {
                    va_end(ap);
                    return -1;
                }
                break;
            }

            case 's':
                if (ps2lib_fmtstr_0019dba8(out, va_arg(ap, const char *),
                                            min_width, max_chars, flags)) {
                    va_end(ap);
                    return -1;
                }
                break;

            case 'n': {
                int *count_out = va_arg(ap, int *);
                *count_out = (int)(out->current - out->start);
                break;
            }

            default:
                /* Unknown conversions survive literally as "%<char>". */
                if (out->putc_cb(out, '%') || out->putc_cb(out, (int)ch)) {
                    va_end(ap);
                    return -1;
                }
                break;
            }
        }
    }

    va_end(ap);
    return 0;
}

/* Target: 0x0019e2e0. */
int ps2lib_vsnprintf_recovered(char *dst,
                                size_t size,
                                const char *fmt,
                                va_list ap)
{
    ps2lib_output_recovered out;
    int result = (int)size;

    out.start = (uintptr_t)dst;
    out.current = (uintptr_t)dst;
    out.end = (uintptr_t)dst + (uintptr_t)size - 1u;
    out.size = (uint32_t)size;
    out.state = 0;
    out.putc_cb = output_putc;
    out.room_cb = output_room;

    if (ps2lib_dopr_0019de10(&out, fmt, ap) == 0)
        result = (int)(out.current - out.start);

    /* The target always writes this NUL, even after overflow or size == 0. */
    *(unsigned char *)out.current = 0;

    return result;
}
