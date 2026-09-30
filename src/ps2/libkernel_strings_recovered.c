/*
 * Remaining small libkernel/libc routines linked into SNES Station v0.23.
 * Exact memcpy/memset/memmove/strcat/memcmp/strcpy/strlen/strchr/strcmp providers now live in historical assembly sources.
 * Remaining target corridor starts at 0x0019c410.
 */
#include <stddef.h>
#include <stdint.h>

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

