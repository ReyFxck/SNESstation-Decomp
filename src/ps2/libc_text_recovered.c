#include <stddef.h>
#include <stdint.h>

/*
 * Small ASCII libc helpers recovered immediately after the old PS2LIB heap
 * allocator.  These are target-behaviour models, not calls through the host's
 * locale-sensitive <ctype.h>/<strings.h> implementation.
 */

/* Target: 0x0019ee0c. */
int isupper_0019ee0c(int c)
{
    return c >= 'A' && c <= 'Z';
}

/* Target: 0x0019ee20. */
int islower_0019ee20(int c)
{
    return c >= 'a' && c <= 'z';
}

/* Target: 0x0019ee34. */
int isalpha_0019ee34(int c)
{
    if (islower_0019ee20(c))
        return 1;
    return isupper_0019ee0c(c) ? 1 : 0;
}

/* Target: 0x0019ee80. */
int isdigit_0019ee80(int c)
{
    return c >= '0' && c <= '9';
}

/* Target: 0x0019ee94. */
int isalnum_0019ee94(int c)
{
    if (isalpha_0019ee34(c))
        return 1;
    return isdigit_0019ee80(c) ? 1 : 0;
}

/* Target: 0x0019eee0. */
int iscntrl_0019eee0(int c)
{
    return c < 0x20 || c == 0x7f;
}

/* Target: 0x0019efac. */
int isspace_0019efac(int c)
{
    unsigned int tab_family = (unsigned int)(c - 9);
    if (tab_family < 5u)
        return 1;
    return c == 0x20;
}

/* Target: 0x0019eefc. */
int isgraph_0019eefc(int c)
{
    if (iscntrl_0019eee0(c))
        return 0;
    return isspace_0019efac(c) ? 0 : 1;
}

/* Target: 0x0019ef3c. */
int isprint_0019ef3c(int c)
{
    return iscntrl_0019eee0(c) ? 0 : 1;
}

/* Target: 0x0019ef5c. */
int ispunct_0019ef5c(int c)
{
    if (iscntrl_0019eee0(c))
        return 0;
    if (isalnum_0019ee94(c))
        return 0;
    return isspace_0019efac(c) ? 0 : 1;
}

/* Target: 0x0019efcc. */
int isxdigit_0019efcc(int c)
{
    if (isdigit_0019ee80(c))
        return 1;
    if ((unsigned int)(c - 'a') < 6u)
        return 1;
    return (unsigned int)(c - 'A') < 6u;
}

/* Target: 0x0019edac. */
int tolower_0019edac(int c)
{
    if (isupper_0019ee0c(c))
        return c + 0x20;
    return c;
}

/* Target: 0x0019eddc. */
int toupper_0019eddc(int c)
{
    if (islower_0019ee20(c))
        return c - 0x20;
    return c;
}

/* Target: 0x0019e8e4. */
int strncasecmp_0019e8e4(const char *left, const char *right, unsigned int count)
{
    const signed char *a = (const signed char *)left;
    const signed char *b = (const signed char *)right;

    if (count == 0u)
        return 0;

    for (;;) {
        int ca;
        int cb;

        count--;
        ca = tolower_0019edac((int)*a);
        cb = tolower_0019edac((int)*b);

        if (ca != cb || count == 0u || *a == 0 || *b == 0)
            break;

        a++;
        b++;
    }

    return tolower_0019edac((int)*(const unsigned char *)a) -
           tolower_0019edac((int)*(const unsigned char *)b);
}

/* Target errno storage is a single int at 0x00425a70. */
int ps2lib_errno_00425a70;
