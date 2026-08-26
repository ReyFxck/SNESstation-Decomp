#ifndef SNESSTATION_LEGACY_ZIP_RECOVERED_H
#define SNESSTATION_LEGACY_ZIP_RECOVERED_H

#include <stddef.h>
#include <stdint.h>

#define LEGACY_ZIP_EXPLODE_WSIZE 0x8000u
#define LEGACY_ZIP_STACK_SIZE    0x2000u

/*
 * Exact target views used by the legacy decompression helpers.  EE pointers
 * remain 32-bit integers so these offsets are also checked on 64-bit hosts.
 * The target does not contain the former ZipIORecovered callback aggregate:
 * it keeps two pointer slots at 0x00424850/0x00424854 and uses shared scratch
 * storage at 0x00448200 and 0x0044e206.
 */
typedef struct LegacyZipReadState {
    uint32_t read_buffer_ee;              /* +0x00 */
    uint32_t reserved_04;                 /* +0x04 */
    uint32_t next_in_ee;                  /* +0x08 */
    uint32_t avail_in;                    /* +0x0c */
    uint64_t total_in;                    /* +0x10 */
    uint32_t next_out_ee;                 /* +0x18 */
    uint32_t avail_out;                   /* +0x1c */
    uint64_t total_out;                   /* +0x20 */
    uint8_t reserved_28[0x28];            /* +0x28 */
    uint64_t pos_in_zipfile;              /* +0x50 */
    uint64_t stream_initialised;          /* +0x58 */
    uint64_t offset_local_extrafield;     /* +0x60 */
    uint64_t size_local_extrafield;       /* +0x68 */
    uint64_t pos_local_extrafield;        /* +0x70 */
    uint64_t crc32;                       /* +0x78 */
    uint64_t crc32_wait;                  /* +0x80 */
    int64_t rest_read_compressed;         /* +0x88 */
    int64_t rest_read_uncompressed;       /* +0x90 */
    int32_t file;                         /* +0x98 */
    uint8_t reserved_9c[0x0c];            /* +0x9c */
    uint64_t byte_before_zipfile;         /* +0xa8 */
} LegacyZipReadState;

typedef struct LegacyZipArchiveState {
    uint8_t reserved_00[0x60];
    uint64_t general_purpose_flag;        /* +0x60 */
    uint64_t compression_method;          /* +0x68 */
    uint8_t reserved_70[0x70];
    uint32_t current_read_state_ee;       /* +0xe0 */
} LegacyZipArchiveState;

typedef struct LegacyShrinkWorkspace {
    int16_t prefix[0x2000];
    uint8_t suffix[0x2000];
} LegacyShrinkWorkspace;

_Static_assert(offsetof(LegacyZipReadState, next_in_ee) == 0x08,
               "legacy unzip next_in offset");
_Static_assert(offsetof(LegacyZipReadState, next_out_ee) == 0x18,
               "legacy unzip next_out offset");
_Static_assert(offsetof(LegacyZipReadState, pos_in_zipfile) == 0x50,
               "legacy unzip input position offset");
_Static_assert(offsetof(LegacyZipReadState, crc32) == 0x78,
               "legacy unzip crc offset");
_Static_assert(offsetof(LegacyZipReadState, rest_read_compressed) == 0x88,
               "legacy unzip compressed-rest offset");
_Static_assert(offsetof(LegacyZipReadState, file) == 0x98,
               "legacy unzip file offset");
_Static_assert(offsetof(LegacyZipReadState, byte_before_zipfile) == 0xa8,
               "legacy unzip byte-before offset");
_Static_assert(sizeof(LegacyZipReadState) == 0xb0,
               "legacy unzip read-state size");
_Static_assert(offsetof(LegacyZipArchiveState, general_purpose_flag) == 0x60,
               "legacy unzip flag offset");
_Static_assert(offsetof(LegacyZipArchiveState, compression_method) == 0x68,
               "legacy unzip method offset");
_Static_assert(offsetof(LegacyZipArchiveState, current_read_state_ee) == 0xe0,
               "legacy unzip current-file offset");
_Static_assert(sizeof(LegacyShrinkWorkspace) == 0x6000,
               "legacy shrink workspace extent");

extern uint32_t DAT_00424850;
extern uint32_t DAT_00424854;
extern LegacyShrinkWorkspace g_shrink_workspace_recovered;
extern uint8_t uRam0044e206[];

static inline LegacyZipReadState *legacy_zip_read_state(void)
{
    return (LegacyZipReadState *)(uintptr_t)DAT_00424850;
}

static inline LegacyZipArchiveState *legacy_zip_archive_state(void)
{
    return (LegacyZipArchiveState *)(uintptr_t)DAT_00424854;
}

static inline uint8_t *legacy_zip_read_buffer(LegacyZipReadState *state)
{
    return (uint8_t *)(uintptr_t)state->read_buffer_ee;
}

static inline uint8_t *legacy_zip_next_in(LegacyZipReadState *state)
{
    return (uint8_t *)(uintptr_t)state->next_in_ee;
}

static inline uint8_t *legacy_zip_next_out(LegacyZipReadState *state)
{
    return (uint8_t *)(uintptr_t)state->next_out_ee;
}

extern uint64_t g_legacy_zip_bitbuf;
extern int g_legacy_zip_bits_left;
extern uint8_t g_legacy_zip_zipeof;

int ReadByte_recovered(uint16_t *out);
void flush_recovered(unsigned w);
void flush_stack_recovered(unsigned w);
int FillBitBuffer_recovered(void);

#endif
