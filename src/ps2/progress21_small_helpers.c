/*
 * Progress 21: three small Progress-16 targets migrated from structural
 * R5900 pseudocode into independently buildable behavioral C.
 *
 * No machine-code matching claim is made here.  Address-labelled names are
 * retained where the historical source symbol is not independently proven.
 */
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * 0x00103cd4
 *
 * The target indexes 0x90-byte directory/TOC records.  Relative to the
 * decompiler's 0x00427010 anchor, the flags byte is at -0x88 and the display
 * name begins four bytes later at -0x84.  Rebase that evidence to a compact
 * entry view instead of pretending the final global owner is already known.
 */
/* The target stores a pointer at 0x00427010, eight bytes past the logical
 * start of the first 0x90-byte entry. */
extern const uint8_t *g_p21_toc_anchor_00427010;

void snes_p21_00103cd4(int index, char *output)
{
    int offset = index * 0x90;
    char *match;

    strncpy(output,
            (const char *)g_p21_toc_anchor_00427010 + offset - 0x84,
            0x80);
    output[0x80] = '\0';

    if ((g_p21_toc_anchor_00427010[offset - 0x88] & 2u) == 0u) {
        match = strrchr(output, '.');
        if (match != NULL)
            *match = '\0';
    }

    match = strchr(output, '_');
    while (match != NULL) {
        *match = ' ';
        match = strchr(output, '_');
    }
}

/*
 * 0x00106c08
 *
 * The adjacent frontend enumerates the save-state wildcard ending in ".00?" and labels matching
 * entries as "State #%d".  The two-byte comparison operand at 0x001b1348 is
 * intentionally kept address-bound until that tiny data object is promoted.
 */
extern const char g_p21_state_extension_prefix_001b1348[2];

int snes_p21_00106c08(const char *path)
{
    const char *dot;
    const char *extension;

    if (path == NULL)
        return 0;
    dot = strrchr(path, '.');
    if (dot == NULL)
        return 0;
    extension = dot + 1;
    if (memcmp(extension, g_p21_state_extension_prefix_001b1348, 2) == 0) {
        if (isdigit(extension[2]))
            return (int)strtol(dot + 3, NULL, 10);
    }
    return 0;
}

/*
 * 0x001a7638
 *
 * Signed 64-bit remainder wrapper.  The target takes absolute magnitudes,
 * calls the still-structural 0x001a7788 unsigned div/mod core with a remainder
 * output pointer, then reapplies only the numerator sign to that remainder.
 */
extern uint64_t snes_udivmoddi4_001a7788(uint64_t numerator,
                                         uint64_t denominator,
                                         uint64_t *remainder);

static uint64_t p21_i64_magnitude(int64_t value)
{
    uint64_t bits = (uint64_t)value;
    return value < 0 ? (~bits + UINT64_C(1)) : bits;
}

int64_t snes_p21_001a7638(int64_t numerator, int64_t denominator)
{
    uint64_t remainder = 0;

    (void)snes_udivmoddi4_001a7788(p21_i64_magnitude(numerator),
                                   p21_i64_magnitude(denominator),
                                   &remainder);
    return numerator < 0 ? -(int64_t)remainder : (int64_t)remainder;
}
