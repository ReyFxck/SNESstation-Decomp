/*
 * SNES Station v0.23 - File Buffer Load (64-byte aligned)
 * Canonical source from progress22_more_small_helpers.c (0x00104234 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct SnesFileBufferTarget {
    uint32_t reserved_00;
    uint32_t data_ee;  /* +0x04 target pointer */
    int32_t size;      /* +0x08 */
} SnesFileBufferTarget;

_Static_assert(offsetof(SnesFileBufferTarget, data_ee) == 4,
               "0x00104234 data pointer offset");
_Static_assert(offsetof(SnesFileBufferTarget, size) == 8,
               "0x00104234 size offset");

extern int fioOpen(const char *path, int mode);
extern int fioLseek(int fd, int offset, int whence);
extern int fioClose(int fd);
extern int fioRead(int fd, void *buffer, int size);
extern void *memalign(size_t alignment, size_t size);

/* 0x00104234: load an entire file into a 64-byte-aligned buffer. */
int snes_file_buffer_load_00104234(SnesFileBufferTarget *state, const char *path)
{
    int fd;
    int size;
    int rounded;
    void *buffer;

    fd = fioOpen(path, 1);
    if (fd < 0)
        return 2;

    size = fioLseek(fd, 0, 2);
    state->size = size;
    if (size == 0) {
        (void)fioClose(fd);
        return 2;
    }

    rounded = size;
    if (rounded < 0)
        rounded += 0x3f;
    rounded = (rounded >> 6) * 0x40 + 0x40;

    buffer = memalign(0x40u, (size_t)rounded);
    state->data_ee = (uint32_t)(uintptr_t)buffer;
    (void)memset(buffer, 0xff, (size_t)rounded);

    (void)fioLseek(fd, 0, 0);
    (void)fioRead(fd, buffer, state->size);
    ((uint8_t *)buffer)[state->size] = 0;
    (void)fioClose(fd);
    return 1;
}