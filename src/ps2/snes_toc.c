/*
 * SNES Station v0.23 PS2 - TOC Entry Name Formatting
 * Canonical source from progress21_small_helpers.c @ 0x00103cd4
 */

#include <stdint.h>
#include <string.h>

/* The target stores a pointer at 0x00427010, eight bytes past the logical
 * start of the first 0x90-byte entry. */
extern const uint8_t *g_snes_toc_anchor_00427010;

void snes_toc_entry_format_name_00103cd4(int index, char *output)
{
    int offset = index * 0x90;
    char *match;

    strncpy(output,
            (const char *)g_snes_toc_anchor_00427010 + offset - 0x84,
            0x80);
    output[0x80] = '\0';

    if ((g_snes_toc_anchor_00427010[offset - 0x88] & 2u) == 0u) {
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