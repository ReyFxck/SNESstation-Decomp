/*
 * Progress 12: frontend path/output, memory-card probe, small persistence and
 * state-block transfer wrappers recovered from SNES Station v0.23.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* 0x00103d90: target sprintf(dst, "cdfs:%s/%s", fixed, record_path). */
int snes_p12_00103d90(uint32_t index, char *dst, const char *fixed,
                      const char *record_table_base)
{
    const char *record_path = (const char *)((uintptr_t)record_table_base
                                             + (uintptr_t)index * 0x90u
                                             - (uintptr_t)0x84u);
    return sprintf(dst, "cdfs:%s/%s", fixed, record_path);
}

typedef int (*P12WriteCallback)(void *handle, const void *data, size_t size,
                                void *opaque);

/* 0x00172174: emit "%s:%06d:" and then the payload using the same length. */
void snes_p12_00172174(void *handle, const char *label, const void *payload,
                       uint32_t payload_size, P12WriteCallback write_fn,
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

typedef int (*P12McInitCallback)(int type, void *opaque);
typedef int (*P12McGetInfoCallback)(int port, int slot, int *type,
                                    int *free_space, int *formatted,
                                    void *opaque);
typedef int (*P12McSyncCallback)(int mode, int *command, int *result,
                                 void *opaque);

typedef struct P12McProbeState {
    uint32_t initialized;
} P12McProbeState;

/* 0x00106824: lazy mcInit, mcGetInfo(0,0), mcSync(0,0,..), type == 2. */
int snes_p12_00106824(P12McProbeState *state, P12McInitCallback init_fn,
                      P12McGetInfoCallback info_fn, P12McSyncCallback sync_fn,
                      void *opaque)
{
    int type = 0;
    int free_space = 0;
    int formatted = 0;
    int sync_result = 0;

    if (state->initialized != 1u) {
        if (init_fn != NULL)
            (void)init_fn(0, opaque);
        state->initialized = 1u;
    }
    if (info_fn != NULL)
        (void)info_fn(0, 0, &type, &free_space, &formatted, opaque);
    if (sync_fn != NULL)
        (void)sync_fn(0, NULL, &sync_result, opaque);
    return type == 2;
}

typedef struct P12MemorySdd1View {
    uint8_t reserved_0000[0xb070];
    uint32_t saved;
    uint32_t current;
    uint8_t records[0x10000];
} P12MemorySdd1View;

extern P12MemorySdd1View g_p12_memory;
extern const char g_p12_sdd1_dat_extension[];
extern char *snes_p12_get_filename(const char *extension);
extern int snes_p12_compare_sdd1_entries(const void *left, const void *right);
extern void snes_p12_qsort(void *base, uint32_t count, uint32_t width,
                           int (*compare)(const void *, const void *));
extern int snes_p12_fio_open(const char *path, int flags);
extern int snes_p12_fio_read(int fd, void *data, int bytes);
extern int snes_p12_fio_write(int fd, const void *data, int bytes);
extern int snes_p12_fio_close(int fd);

/*
 * 0x0016fbb4: open/read up to 0x10000 bytes, mirror the returned value into
 * both target counters, then close.  The target intentionally stores the raw
 * fioRead result; this model does not reinterpret it as a record count.
 */
void snes_p12_0016fbb4(void)
{
    int fd;

    fd = snes_p12_fio_open(snes_p12_get_filename(g_p12_sdd1_dat_extension), 1);
    g_p12_memory.current = g_p12_memory.saved = 0;
    if (fd >= 0) {
        int result = snes_p12_fio_read(fd, g_p12_memory.records, 0x10000);
        if (result != -1)
            g_p12_memory.current = g_p12_memory.saved = (uint32_t)result;
        (void)snes_p12_fio_close(fd);
    }
}

/*
 * 0x0016fb04: if the two counters differ, sort 8-byte entries, recreate the
 * .dat file, write current*8 bytes, close, then mirror current into saved.
 */
void snes_p12_0016fb04(void)
{
    int fd;

    if (g_p12_memory.current != g_p12_memory.saved) {
        snes_p12_qsort(g_p12_memory.records, g_p12_memory.current, 8,
                       snes_p12_compare_sdd1_entries);
        fd = snes_p12_fio_open(
            snes_p12_get_filename(g_p12_sdd1_dat_extension), 0x202);
        if (fd >= 0) {
            (void)snes_p12_fio_write(fd, g_p12_memory.records,
                                     (int)(g_p12_memory.current << 3));
            (void)snes_p12_fio_close(fd);
        }
        g_p12_memory.saved = g_p12_memory.current;
    }
}

typedef struct P12TransferBlock {
    uint8_t bytes[0x1c];
} P12TransferBlock;

typedef void (*P12RefreshCallback)(void *opaque);

static size_t p12_transfer_offset(uint8_t selector)
{
    size_t offset = 0u;

    if (selector != 0u) {
        unsigned shift = ((unsigned)selector + 3u) & 31u;
        offset = (size_t)((uint32_t)0x80u << shift);
        if (offset > 0x20000u)
            offset = 0x20000u;
    }
    return offset;
}

static void p12_copy_template_to_block(uint8_t *dst, const P12TransferBlock *src)
{
    dst[0] = src->bytes[0];
    dst[1] = src->bytes[1];
    memcpy(dst + 2u, src->bytes + 2u, 13u);
    dst[15] = src->bytes[15];
    dst[16] = src->bytes[16];
    memcpy(dst + 17u, src->bytes + 20u, 8u);
}

static void p12_copy_block_to_template(P12TransferBlock *dst, const uint8_t *src)
{
    dst->bytes[0] = src[0];
    dst->bytes[1] = src[1];
    memcpy(dst->bytes + 2u, src + 2u, 13u);
    dst->bytes[15] = src[15];
    dst->bytes[16] = src[16];
    memcpy(dst->bytes + 20u, src + 17u, 8u);
}

/* 0x00183c58: refresh, then copy selected template fields into backing store. */
void snes_p12_00183c58(uint8_t enabled, uint8_t selector, uint8_t *backing,
                       const P12TransferBlock *templ,
                       P12RefreshCallback refresh, void *opaque)
{
    if (enabled == 0u)
        return;
    if (refresh != NULL)
        refresh(opaque);
    p12_copy_template_to_block(backing + p12_transfer_offset(selector), templ);
}

/* 0x00183d40: inverse copy, then refresh. */
void snes_p12_00183d40(uint8_t enabled, uint8_t selector,
                       const uint8_t *backing, P12TransferBlock *templ,
                       P12RefreshCallback refresh, void *opaque)
{
    if (enabled == 0u)
        return;
    p12_copy_block_to_template(templ,
                               backing + p12_transfer_offset(selector));
    if (refresh != NULL)
        refresh(opaque);
}
