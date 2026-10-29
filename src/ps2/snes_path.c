/*
 * SNES Station v0.23 - Path Formatting Utilities
 * Canonical source from progress12_io_state_recovered.c
 */

#include <stdint.h>
#include <stdio.h>

/* 0x00103d90: target sprintf(dst, "cdfs:%s/%s", fixed, record_path). */
int snes_path_format_cdfs(char *dst, uint32_t index, const char *fixed,
                          const char *record_table_base)
{
    const char *record_path = (const char *)((uintptr_t)record_table_base
                                             + (uintptr_t)index * 0x90u
                                             - (uintptr_t)0x84u);
    return sprintf(dst, "cdfs:%s/%s", fixed, record_path);
}

/* 0x00172174: emit "%s:%06d:" and then the payload using the same length. */
typedef int (*SnesWriteCallback)(void *handle, const void *data, size_t size, void *opaque);

void snes_path_write_tagged(void *handle, const char *label, const void *payload,
                            uint32_t payload_size, SnesWriteCallback write_fn,
                            void *opaque)
{
    char header[0x240];
    int header_length;

    header_length = sprintf(header, "%s:%06d:", label, (int)payload_size);
    if (write_fn == NULL)
        return;
    (void)write_fn(handle, header, (size_t)header_length, opaque);
    (void)write_fn(handle, payload, payload_size, opaque);
}