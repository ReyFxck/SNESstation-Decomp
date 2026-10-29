/*
 * SNES Station v0.23 - Write Callback Helper
 * Canonical source from progress12_io_state.c (0x00172174 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef int (*SnesWriteCallback)(void *handle, const void *data, size_t size,
                                 void *opaque);

/* 0x00172174: emit "%s:%06d:" and then the payload using the same length. */
void snes_write_header_payload_00172174(void *handle, const char *label,
                                         const void *payload, uint32_t payload_size,
                                         SnesWriteCallback write_fn, void *opaque)
{
    char header[0x240];
    int header_length;

    header_length = sprintf(header, "%s:%06d:", label, (int)payload_size);
    if (write_fn == NULL)
        return;
    (void)write_fn(handle, header, (size_t)header_length, opaque);
    (void)write_fn(handle, payload, payload_size, opaque);
}