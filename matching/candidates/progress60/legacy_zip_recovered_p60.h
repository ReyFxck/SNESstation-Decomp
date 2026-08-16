#ifndef SNESSTATION_LEGACY_ZIP_RECOVERED_H
#define SNESSTATION_LEGACY_ZIP_RECOVERED_H

#include <stdint.h>

#define LEGACY_ZIP_EXPLODE_WSIZE 0x8000u
#define LEGACY_ZIP_STACK_SIZE    0x2000u

typedef struct {
    uint8_t *next_in;
    uint32_t avail_in;
    uint8_t *next_out;
    uint32_t avail_out;
    uint32_t total_out;
    int32_t rest_read_compressed;
    int32_t rest_read_uncompressed;
    uint32_t general_purpose_flag;
    unsigned compression_method;
    uint8_t *read_buffer;
    uint32_t read_buffer_size;
    uint8_t slide[LEGACY_ZIP_EXPLODE_WSIZE];
    uint8_t stack[LEGACY_ZIP_STACK_SIZE];
    uint32_t crc32;
    void *opaque;
    int (*refill)(void *opaque, uint8_t *dst, uint32_t want);
    uint32_t (*crc32_update)(uint32_t crc, const uint8_t *data, uint32_t size);
} ZipIORecovered;

extern ZipIORecovered g_zip_io_recovered;
extern uint64_t g_legacy_zip_bitbuf;
extern int g_legacy_zip_bits_left;
extern uint8_t g_legacy_zip_zipeof;

int ReadByte_recovered(uint16_t *out);
void flush_recovered(unsigned w);
void flush_stack_recovered(unsigned w);
int FillBitBuffer_recovered(void);

#endif
