/*
 * SNES Station v0.23 - SNES9X Fixed Record Persistence
 * Canonical source from progress13_frontend_more.c (0x001833a4, 0x001834f0 range)
 */

#include <stddef.h>
#include <stdint.h>

typedef int (*Snes9xOpenFn)(const char *path, int flags, void *opaque);
typedef int (*Snes9xIoFn)(int fd, void *buffer, int size, void *opaque);
typedef int (*Snes9xCloseFn)(int fd, void *opaque);

/* 0x001833a4 / 0x001834f0 -- fixed 24-byte record persistence. */

int snes9x_write_record_001833a4(const char *path, const uint8_t record[24],
                                  Snes9xOpenFn open_fn, Snes9xIoFn write_fn,
                                  Snes9xCloseFn close_fn, void *opaque)
{
    int fd, i;
    if (open_fn == NULL || write_fn == NULL)
        return 0;
    fd = open_fn(path, 0x202, opaque);
    if (fd < 0)
        return 0;
    for (i = 0; i < 16; ++i) {
        uint8_t b = record[i];
        (void)write_fn(fd, &b, 1, opaque);
    }
    for (i = 16; i < 24; ++i) {
        uint8_t b = record[i];
        (void)write_fn(fd, &b, 1, opaque);
    }
    if (close_fn != NULL)
        (void)close_fn(fd, opaque);
    return 1;
}

int snes9x_read_record_001834f0(const char *path, uint8_t record[24],
                                 Snes9xOpenFn open_fn, Snes9xIoFn read_fn,
                                 Snes9xCloseFn close_fn, void *opaque)
{
    int fd, i;
    if (open_fn == NULL || read_fn == NULL)
        return 0;
    fd = open_fn(path, 1, opaque);
    if (fd < 0)
        return 0;
    for (i = 0; i < 24; ++i) {
        uint8_t b = 0;
        (void)read_fn(fd, &b, 1, opaque);
        record[i] = b;
    }
    if (close_fn != NULL)
        (void)close_fn(fd, opaque);
    return 1;
}