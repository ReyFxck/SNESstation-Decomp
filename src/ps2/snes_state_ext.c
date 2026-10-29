/*
 * SNES Station v0.23 PS2 - Save State Extension Parsing
 * Canonical source from progress21_small_helpers.c @ 0x00106c08
 */

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* The two-byte comparison operand at 0x001b1348 is the state extension prefix. */
extern const char g_snes_state_extension_prefix_001b1348[2];

int snes_state_ext_parse_00106c08(const char *path)
{
    const char *dot;
    const char *extension;

    if (path == NULL)
        return 0;
    dot = strrchr(path, '.');
    if (dot == NULL)
        return 0;
    extension = dot + 1;
    if (memcmp(extension, g_snes_state_extension_prefix_001b1348, 2) == 0) {
        if (isdigit(extension[2]))
            return (int)strtol(dot + 3, NULL, 10);
    }
    return 0;
}