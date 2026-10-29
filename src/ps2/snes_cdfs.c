/*
 * SNES Station v0.23 - CDFS Path Formatting
 * Canonical source from progress12_io_state.c (0x00103d90 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* 0x00103d90: target sprintf(dst, "cdfs:%s/%s", fixed, record_path). */
int snes_cdfs_format_path_00103d90(uint32_t index, char *dst, const char *fixed,
                                   const char *record_table_base)
{
    const char *record_path = (const char *)((uintptr_t)record_table_base
                                             + (uintptr_t)index * 0x90u
                                             - (uintptr_t)0x84u);
    return sprintf(dst, "cdfs:%s/%s", fixed, record_path);
}