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

typedef int (*P12OpenCallback)(const char *path, int flags, void *opaque);
typedef int (*P12ReadCallback)(int fd, void *data, size_t size, void *opaque);
typedef int (*P12FileWriteCallback)(int fd, const void *data, size_t size,
                                    void *opaque);
typedef int (*P12CloseCallback)(int fd, void *opaque);
typedef void (*P12Sort8Callback)(void *records, uint32_t count, void *opaque);

typedef struct P12RecordStore {
    uint8_t *records;
    uint32_t current;
    uint32_t saved;
} P12RecordStore;

/*
 * 0x0016fbb4: open/read up to 0x10000 bytes, mirror the returned value into
 * both target counters, then close.  The target intentionally stores the raw
 * fioRead result; this model does not reinterpret it as a record count.
 */
void snes_p12_0016fbb4(P12RecordStore *store, const char *path,
                       P12OpenCallback open_fn, P12ReadCallback read_fn,
                       P12CloseCallback close_fn, void *opaque)
{
    int fd;

    store->current = 0u;
    store->saved = 0u;
    if (open_fn == NULL)
        return;
    fd = open_fn(path, 1, opaque);
    if (fd < 0)
        return;
    if (read_fn != NULL) {
        int result = read_fn(fd, store->records, 0x10000u, opaque);
        if (result != -1) {
            store->current = (uint32_t)result;
            store->saved = (uint32_t)result;
        }
    }
    if (close_fn != NULL)
        (void)close_fn(fd, opaque);
}

/*
 * 0x0016fb04: if the two counters differ, sort 8-byte entries, recreate the
 * .dat file, write current*8 bytes, close, then mirror current into saved.
 */
void snes_p12_0016fb04(P12RecordStore *store, const char *path,
                       P12Sort8Callback sort_fn, P12OpenCallback open_fn,
                       P12FileWriteCallback write_fn,
                       P12CloseCallback close_fn, void *opaque)
{
    int fd;

    if (store->current == store->saved)
        return;
    if (sort_fn != NULL)
        sort_fn(store->records, store->current, opaque);
    if (open_fn != NULL) {
        fd = open_fn(path, 0x202, opaque);
        if (fd >= 0) {
            if (write_fn != NULL)
                (void)write_fn(fd, store->records,
                               (size_t)(uint32_t)(store->current << 3), opaque);
            if (close_fn != NULL)
                (void)close_fn(fd, opaque);
        }
    }
    store->saved = store->current;
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
