/*
 * Small libkernel/libc routines linked into SNES Station v0.23.
 * Target corridor: 0x0019c364..0x0019c687.
 */
#include <stddef.h>
#include <stdint.h>

/* 0x0019c364 */
void *sn_memcpy_0019c364(void *dst, const void *src, size_t n)
{
    uint8_t *d = dst;
    const uint8_t *s = src;
    while (n-- != 0)
        *d++ = *s++;
    return dst;
}

/* 0x0019c39c */
void *sn_memset_0019c39c(void *dst, int value, size_t n)
{
    uint8_t *d = dst;
    while (n-- != 0)
        *d++ = (uint8_t)value;
    return dst;
}

/* 0x0019c3d4 */
char *sn_strcat_0019c3d4(char *dst, const char *src)
{
    char *ret = dst;
    while (*dst != '\0')
        ++dst;
    while ((*dst++ = *src++) != '\0') {
    }
    return ret;
}

/* 0x0019c410 */
int sn_strncmp_0019c410(const char *a, const char *b, size_t n)
{
    while (n != 0) {
        unsigned char ca = (unsigned char)*a++;
        unsigned char cb = (unsigned char)*b++;
        if (ca != cb)
            return (int)ca - (int)cb;
        if (ca == 0)
            return 0;
        --n;
    }
    return 0;
}

/* 0x0019c458 */
int sn_memcmp_0019c458(const void *a, const void *b, size_t n)
{
    const uint8_t *p = a;
    const uint8_t *q = b;
    while (n-- != 0) {
        if (*p != *q)
            return (int)*p - (int)*q;
        ++p;
        ++q;
    }
    return 0;
}

/* 0x0019c4a0 */
void *sn_memmove_0019c4a0(void *dst, const void *src, size_t n)
{
    uint8_t *d = dst;
    const uint8_t *s = src;
    if (d <= s) {
        while (n-- != 0)
            *d++ = *s++;
    } else {
        d += n;
        s += n;
        while (n-- != 0)
            *--d = *--s;
    }
    return dst;
}

/* 0x0019c528 */
char *sn_strcpy_0019c528(char *dst, const char *src)
{
    char *ret = dst;
    while ((*dst++ = *src++) != '\0') {
    }
    return ret;
}

/* 0x0019c550 */
char *sn_strncpy_0019c550(char *dst, const char *src, size_t n)
{
    char *ret = dst;
    while (n != 0 && *src != '\0') {
        *dst++ = *src++;
        --n;
    }
    while (n-- != 0)
        *dst++ = '\0';
    return ret;
}

/* 0x0019c5e8 */
size_t sn_strlen_0019c5e8(const char *s)
{
    const char *p = s;
    while (*p != '\0')
        ++p;
    return (size_t)(p - s);
}

/* 0x0019c610 */
char *sn_strchr_0019c610(const char *s, int ch)
{
    const unsigned char wanted = (unsigned char)ch;
    do {
        if ((unsigned char)*s == wanted)
            return (char *)s;
    } while (*s++ != '\0');
    return NULL;
}

/* 0x0019c648 */
int sn_strcmp_0019c648(const char *a, const char *b)
{
    while (*a != '\0' && *a == *b) {
        ++a;
        ++b;
    }
    return (int)(unsigned char)*a - (int)(unsigned char)*b;
}
