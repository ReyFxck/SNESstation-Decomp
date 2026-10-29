/*
 * SNES Station v0.23 - Save State Extension Check
 * Canonical source from progress21_small_helpers.c (0x00106c08 range)
 */

#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* The adjacent frontend enumerates the save-state wildcard ending in ".00?" and labels matching
 * entries as "State #%d". The two-byte comparison operand at 0x001b1348 is
 * intentionally kept address-bound until that tiny data object is promoted. */
extern const char g_snes_state_extension_prefix_001b1348[2];

int snes_state_check_extension_00106c08(const char *path)
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