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

/* Target: 0x0019e860. */
int strcasecmp_0019e860(const char *left, const char *right)
{
    const signed char *a = (const signed char *)left;
    const signed char *b = (const signed char *)right;

    while (*a != 0) {
        int ca = tolower_0019edac((int)*a);
        int cb = tolower_0019edac((int)*b);
        if (ca != cb)
            break;
        a++;
        b++;
    }

    /* The target's final loads are unsigned even though loop loads are signed. */
    return tolower_0019edac((int)*(const unsigned char *)a) -
           tolower_0019edac((int)*(const unsigned char *)b);
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

extern char *sn_strchr_0019c610(const char *s, int c);
extern size_t sn_strlen_0019c5e8(const char *s);
extern int sn_strncmp_0019c410(const char *a, const char *b, size_t n);

/* Target: 0x0019eaa4. */
char *strrchr_0019eaa4(const char *text, int c)
{
    char *last = NULL;
    const char *scan = text;
    char *found;

    found = sn_strchr_0019c610(scan, c);
    while (found != NULL) {
        last = found;
        scan = found + 1;
        found = sn_strchr_0019c610(scan, c);
    }

    return last;
}

/* Target: 0x0019eaf8. */
char *strstr_0019eaf8(const char *haystack, const char *needle)
{
    const char *scan;
    size_t needle_len;

    if (haystack == NULL)
        return NULL;
    if (*needle == '\0')
        return (char *)haystack;
    if (*haystack == '\0')
        return NULL;

    needle_len = sn_strlen_0019c5e8(needle);
    scan = haystack;

    while (*scan != '\0') {
        if (sn_strncmp_0019c410(scan, needle, needle_len) == 0)
            return (char *)scan;
        scan++;
    }

    return NULL;
}

/*
 * strtok target state lives at 0x00445140 (current token) and 0x00445144
 * (continuation scan).  Native pointers are used here only to make the
 * behavioural model testable on a host; the target stores 32-bit EE addresses.
 */
static char *strtok_current_00445140;
static char *strtok_state_00445144;

/* Target: 0x0019e99c. */
char *strtok_0019e99c(char *text, const char *delimiters)
{
    if (text != NULL) {
        strtok_current_00445140 = text;

        while (sn_strchr_0019c610(delimiters,
                                  (int)*(signed char *)strtok_current_00445140) != NULL) {
            strtok_current_00445140++;
            if (*strtok_current_00445140 == '\0') {
                /*
                 * Target quirk: continuation state is NOT updated on this
                 * early return, so a later strtok(NULL, ...) can resume an
                 * older tokenization sequence.
                 */
                return NULL;
            }
        }
    } else {
        char *resume = strtok_state_00445144;

        /* Target performs no NULL-state guard here. */
        if (*resume == '\0')
            return NULL;
        strtok_current_00445140 = resume;

        while (sn_strchr_0019c610(delimiters,
                                  (int)*(signed char *)strtok_current_00445140) != NULL) {
            strtok_current_00445140++;
            if (*strtok_current_00445140 == '\0')
                return NULL;
        }
    }

    strtok_state_00445144 = strtok_current_00445140;
    if (*strtok_state_00445144 == '\0')
        return strtok_current_00445140;

    while (sn_strchr_0019c610(delimiters,
                              (int)*(signed char *)strtok_state_00445144) == NULL) {
        strtok_state_00445144++;
        if (*strtok_state_00445144 == '\0')
            return strtok_current_00445140;
    }

    *strtok_state_00445144 = '\0';
    strtok_state_00445144++;
    return strtok_current_00445140;
}

/* Target errno storage is a single int at 0x00425a70. */
int ps2lib_errno_00425a70;

/*
 * Target: 0x0019eb80.
 *
 * Although the EE code uses 64-bit arithmetic instructions internally, this
 * particular historical strtol clamps to the signed 32-bit range.  Preserve
 * that observable range instead of substituting the host libc's LONG_MIN/MAX.
 */
long strtol_0019eb80(const char *text, char **endptr, int base)
{
    const char *original = text;
    const char *scan = text;
    int c;
    int negative = 0;
    uint32_t limit;
    uint32_t cutoff;
    uint32_t cutlim;
    uint32_t acc = 0;
    int any = 0;

    do {
        c = (int)*(const signed char *)scan++;
    } while (isspace_0019efac(c));

    if (c == '-') {
        negative = 1;
        c = (int)*(const signed char *)scan++;
    } else if (c == '+') {
        c = (int)*(const signed char *)scan++;
    }

    /* The target tests the 0x prefix before choosing octal/decimal for base 0. */
    if ((base == 0 || base == 16) && c == '0' &&
        (*scan == 'x' || *scan == 'X')) {
        c = (int)*(const signed char *)(scan + 1);
        scan += 2;
        base = 16;
    }

    if (base == 0)
        base = (c == '0') ? 8 : 10;

    limit = negative ? UINT32_C(0x80000000) : UINT32_C(0x7fffffff);
    if (base != 0) {
        cutoff = limit / (uint32_t)base;
        cutlim = limit % (uint32_t)base;
    } else {
        /* Invalid base is outside the intended caller contract. */
        cutoff = 0;
        cutlim = 0;
    }

    for (;;) {
        int digit;

        if (isdigit_0019ee80(c)) {
            digit = c - '0';
        } else if (isalpha_0019ee34(c)) {
            digit = isupper_0019ee0c(c) ? c - 'A' + 10 : c - 'a' + 10;
        } else {
            break;
        }

        if (digit >= base)
            break;

        if (any >= 0) {
            if (acc > cutoff ||
                (acc == cutoff && (uint32_t)digit > cutlim)) {
                any = -1;
            } else {
                any = 1;
                acc = acc * (uint32_t)base + (uint32_t)digit;
            }
        }

        c = (int)*(const signed char *)scan++;
    }

    if (endptr != NULL)
        *endptr = (char *)(any != 0 ? scan - 1 : original);

    if (any < 0) {
        ps2lib_errno_00425a70 = 34; /* ERANGE */
        return negative ? (long)(int32_t)UINT32_C(0x80000000)
                        : (long)INT32_C(0x7fffffff);
    }

    if (negative)
        return -(long)acc;
    return (long)acc;
}
