/*
 * Progress 27: large typed state/CPU/memory batch from the warning-free
 * Progress-16 R5900 structural decompile.
 *
 * Target entries represented here:
 *   0x00127b78  0x00127e00  0x0012bd48  0x0012cbd8
 *   0x00150ccc  0x00150e18  0x00150f54
 *   0x0016efa0  0x0016f10c  0x0017028c  0x0017124c
 *   0x00171e0c  0x001721ec  0x001725ac  0x001726ec
 *
 * These are typed behavioral/source models.  Function names and ownership are
 * intentionally conservative where the original project types remain
 * unavailable.  Nothing in this file is promoted to machine-code MATCHING.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------- */
/* 0x00127b78 / 0x00127e00: paired CPU interrupt-vector entries             */
/* ------------------------------------------------------------------------- */

typedef struct P27InterruptCpu {
    uint16_t stack_pointer;
    uint16_t status;
    uint8_t accumulator;

    uint32_t current_pc;
    uint32_t pc_base;

    uint8_t carry_latch;
    uint8_t zero_latch;
    uint8_t negative_latch;
    uint8_t overflow_latch;

    uint8_t status_shadow;
    uint32_t cycle_counter;

    uint8_t custom_vectors_enabled;
    uint8_t io_2209;
    uint16_t io_220e;
    uint16_t io_220c;

    void (*write_byte)(void *opaque, uint8_t value, uint16_t address);
    uint16_t (*read_vector)(void *opaque, uint16_t address);
    void (*set_pc)(void *opaque, uint32_t address);
    void *opaque;
} P27InterruptCpu;

static uint8_t p27_pack_status(const P27InterruptCpu *s)
{
    uint8_t value = (uint8_t)(s->status & 0x3cu);

    value |= (uint8_t)(s->carry_latch & 1u);
    value |= (uint8_t)(s->negative_latch & 0x80u);
    value |= (uint8_t)((s->overflow_latch & 1u) << 6);
    if (s->zero_latch == 0)
        value |= 2u;
    return value;
}

static void p27_interrupt_entry(P27InterruptCpu *s, uint8_t vector_enable_mask,
                                uint16_t native_vector, uint16_t emu_vector,
                                uint16_t custom_vector)
{
    uint16_t sp_before = s->stack_pointer;
    uint32_t pc_delta;
    uint8_t packed;
    uint16_t vector;

    if ((s->status & 0x0100u) == 0) {
        s->stack_pointer = (uint16_t)(s->stack_pointer - 1u);
        s->write_byte(s->opaque, s->accumulator, sp_before);
    }

    pc_delta = s->current_pc - s->pc_base;
    s->write_byte(s->opaque, (uint8_t)((pc_delta >> 8) & 0xffu),
                  s->stack_pointer);
    s->write_byte(s->opaque, (uint8_t)(pc_delta & 0xffu),
                  (uint16_t)(s->stack_pointer - 1u));

    packed = p27_pack_status(s);
    s->write_byte(s->opaque, packed,
                  (uint16_t)(s->stack_pointer - 2u));
    s->stack_pointer = (uint16_t)(s->stack_pointer - 3u);

    s->status = (uint16_t)((s->status & 0xff00u) | packed);
    s->status_shadow = packed;
    s->accumulator = 0;
    s->status = (uint16_t)((s->status & 0xfff7u) | 4u);

    if (s->custom_vectors_enabled == 0 ||
        (s->io_2209 & vector_enable_mask) == 0) {
        vector = s->read_vector(
            s->opaque,
            (s->status & 0x0100u) == 0 ? native_vector : emu_vector);
    } else {
        vector = custom_vector;
    }

    s->set_pc(s->opaque, vector);
    s->cycle_counter += (s->status & 0x0100u) == 0 ? 12u : 6u;
}

/* 0x00127b78 */
void snes_p27_00127b78(P27InterruptCpu *s)
{
    p27_interrupt_entry(s, 0x40u, 0xffeeu, 0xfffeu, s->io_220e);
}

/* 0x00127e00 */
void snes_p27_00127e00(P27InterruptCpu *s)
{
    p27_interrupt_entry(s, 0x20u, 0xffeau, 0xfffau, s->io_220c);
}

/* ------------------------------------------------------------------------- */
/* 0x0012bd48: paired low/high table write                                   */
/* ------------------------------------------------------------------------- */

#define P27_TABLE_ENTRIES 0x200u

typedef struct P27PairedTableState {
    uint8_t phase;
    uint16_t index;
    uint8_t derived_enabled;
    uint32_t frame_counter;
    uint32_t rendered_frame;

    uint16_t entry[P27_TABLE_ENTRIES];
    uint16_t derived_low[P27_TABLE_ENTRIES];
    uint16_t derived_high[P27_TABLE_ENTRIES];
    uint16_t derived_mid[P27_TABLE_ENTRIES];
    uint16_t combined[P27_TABLE_ENTRIES];
    uint8_t lookup[32];

    uint8_t dirty;
    void (*flush_before_change)(void *opaque);
    void *opaque;
} P27PairedTableState;

static void p27_table_flush_if_needed(P27PairedTableState *s)
{
    if (s->derived_enabled != 0 &&
        s->frame_counter != s->rendered_frame &&
        s->flush_before_change != NULL) {
        s->flush_before_change(s->opaque);
    }
}

/* 0x0012bd48 */
void snes_p27_0012bd48(P27PairedTableState *s, uint32_t param)
{
    uint32_t input = param & 0xffu;
    uint32_t idx = s->index;

    if (idx >= P27_TABLE_ENTRIES)
        return;

    if (s->phase == 0) {
        if ((uint8_t)s->entry[idx] != (uint8_t)input) {
            uint16_t value;

            p27_table_flush_if_needed(s);
            s->dirty = 1;
            value = (uint16_t)((s->entry[idx] & 0x7f00u) | input);
            s->entry[idx] = value;

            if (s->derived_enabled != 0) {
                uint16_t low = s->lookup[param & 0x1fu];
                uint16_t mid = s->lookup[(value >> 5) & 0x1fu];

                s->derived_low[idx] = low;
                s->derived_mid[idx] = mid;
                s->combined[idx] = (uint16_t)(
                    low | (uint16_t)(s->derived_high[idx] << 10) |
                    (uint16_t)(mid << 5));
            }
        }
    } else {
        if ((param & 0x7fu) != ((uint32_t)s->entry[idx] >> 8)) {
            uint16_t value;

            p27_table_flush_if_needed(s);
            s->dirty = 1;
            value = (uint16_t)((s->entry[idx] & 0x00ffu) |
                               ((param & 0x7fu) << 8));
            s->entry[idx] = value;

            if (s->derived_enabled != 0) {
                uint16_t high = s->lookup[(input >> 2) & 0x1fu];
                uint16_t mid = s->lookup[(value >> 5) & 0x1fu];

                s->derived_high[idx] = high;
                s->derived_mid[idx] = mid;
                s->combined[idx] = (uint16_t)(
                    s->derived_low[idx] |
                    (uint16_t)(high << 10) |
                    (uint16_t)(mid << 5));
            }
        }

        s->index = (uint16_t)(s->index + 1u);
    }

    s->phase ^= 1u;
}

/* ------------------------------------------------------------------------- */
/* 0x0012cbd8: floating transform corridor                                   */
/* ------------------------------------------------------------------------- */

typedef struct P27FloatTransformOps {
    float (*curve)(void *opaque, float value);        /* target 0x0012bf5c */
    float (*fn_19fddc)(void *opaque, float value);
    float (*fn_1a0254)(void *opaque, float value);
    float (*fn_1a0024)(void *opaque, float value);
    float (*adjust_raw)(void *opaque, float value, uint64_t raw_constant);
    void *opaque;
} P27FloatTransformOps;

typedef struct P27FloatTransformState {
    int count;           /* target 0x003413aa */
    float ratio_input;   /* 0x003414b0 */

    float base_a;        /* 0x0034143c */
    float base_b;        /* 0x00341440 */
    float offset_x;      /* 0x00341444 */
    float offset_y;      /* 0x00341448 */
    float scale;         /* 0x0034144c */
    float secondary;     /* 0x003414b4 */

    float angle_a;       /* 0x003414a8 */
    float angle_b;       /* 0x003414ac */
    float output_x;      /* 0x003414b8 */
    float output_y;      /* 0x003414bc */

    uint64_t constant_2100;
    uint64_t constant_2108;
    uint64_t constant_2110;

    P27FloatTransformOps ops;
} P27FloatTransformState;

/* 0x0012cbd8 */
void snes_p27_0012cbd8(P27FloatTransformState *s)
{
    float value;
    float denom;
    float correction;
    float rotated;

    if (s->count == 0)
        return;

    value = s->ops.curve(s->ops.opaque,
                         s->ratio_input / (float)s->count);
    s->angle_a = s->base_a - value;
    s->angle_b = s->base_b;

    if (s->ops.fn_19fddc(s->ops.opaque, s->angle_a) == 0.0f)
        s->angle_a = s->ops.adjust_raw(
            s->ops.opaque, s->angle_a, s->constant_2100);

    if (s->ops.fn_1a0254(s->ops.opaque, s->angle_a) == 0.0f)
        s->angle_a = s->ops.adjust_raw(
            s->ops.opaque, s->angle_a, s->constant_2108);

    value = s->ops.fn_1a0024(s->ops.opaque, s->angle_b);
    denom = s->ops.fn_1a0254(s->ops.opaque, s->angle_a);
    s->output_x = (-value * s->scale) / denom + s->offset_x;

    value = s->ops.fn_19fddc(s->ops.opaque, s->angle_b) * s->scale;
    denom = s->ops.fn_1a0254(s->ops.opaque, s->angle_a);
    s->output_y = value / denom + s->offset_y;

    denom = s->ops.fn_1a0024(s->ops.opaque, s->angle_a);
    correction = (s->scale / denom) / (float)s->count;

    rotated = s->ops.adjust_raw(
        s->ops.opaque, s->angle_b, s->constant_2110);
    value = s->ops.fn_1a0024(s->ops.opaque, rotated);
    s->output_x += correction * -value * s->secondary;

    rotated = s->ops.adjust_raw(
        s->ops.opaque, s->angle_b, s->constant_2110);
    value = s->ops.fn_19fddc(s->ops.opaque, rotated);
    s->output_y += correction * value * s->secondary;
}

/* ------------------------------------------------------------------------- */
/* 0x00150ccc / 0x00150e18 / 0x00150f54: CMemory helpers                   */
/* ------------------------------------------------------------------------- */

typedef struct P27RomScoreState {
    const uint8_t *rom;
    uint32_t rom_size;
    int (*header_text_ok)(void *opaque, const uint8_t *text, size_t length);
    void *opaque;
} P27RomScoreState;

static int p27_header_sum_is_ffff(const uint8_t *h)
{
    uint32_t sum = (uint32_t)h[0xdc] +
                   (uint32_t)h[0xdd] * 0x100u +
                   (uint32_t)h[0xde] +
                   (uint32_t)h[0xdf] * 0x100u;
    return sum == 0xffffu;
}

/* 0x00150ccc -- CMemory_ScoreHiROM */
int snes_p27_00150ccc(const P27RomScoreState *s, int alternate)
{
    size_t offset = alternate ? 0x10100u : 0xff00u;
    const uint8_t *h = s->rom + offset;
    int score = (h[0xd5] & 1u) * 2;

    if (h[0xd4] == (uint8_t)' ')
        score += 2;
    if (p27_header_sum_is_ffff(h))
        score += 2;
    if (h[0xda] == (uint8_t)'3')
        score += 2;
    if ((h[0xd5] & 0x0fu) < 4u)
        score += 2;
    if ((h[0xfd] & 0x80u) == 0)
        score -= 4;
    if (s->rom_size > 0x300000u)
        score += 4;
    if ((1u << ((h[0xd7] - 7u) & 31u)) > 0x30u)
        score -= 1;
    if (!s->header_text_ok(s->opaque, h + 0xb0, 6))
        score -= 1;
    if (!s->header_text_ok(s->opaque, h + 0xc0, 0x16))
        score -= 1;

    return score;
}

/* 0x00150e18 -- CMemory_ScoreLoROM */
int snes_p27_00150e18(const P27RomScoreState *s, int alternate)
{
    size_t offset = alternate ? 0x8100u : 0x7f00u;
    const uint8_t *h = s->rom + offset;
    int score = ((h[0xd5] ^ 1u) & 1u) * 4;

    if (p27_header_sum_is_ffff(h))
        score += 2;
    if (h[0xda] == (uint8_t)'3')
        score += 2;
    if ((h[0xd5] & 0x0fu) < 4u)
        score += 2;
    if (s->rom_size < 0x1000001u)
        score += 2;
    if ((h[0xfd] & 0x80u) == 0)
        score -= 4;
    if ((1u << ((h[0xd7] - 7u) & 31u)) > 0x30u)
        score -= 1;
    if (!s->header_text_ok(s->opaque, h + 0xb0, 6))
        score -= 1;
    if (!s->header_text_ok(s->opaque, h + 0xc0, 0x16))
        score -= 1;

    return score;
}

typedef struct P27SafeString {
    char *buffer;
    size_t capacity;
    void *(*alloc)(void *opaque, size_t size);
    void (*free)(void *opaque, void *ptr);
    void *opaque;
} P27SafeString;

/* 0x00150f54 -- CMemory_Safe */
char *snes_p27_00150f54(P27SafeString *s, const char *input)
{
    size_t length;
    size_t i;

    if (input == NULL) {
        if (s->buffer != NULL)
            s->free(s->opaque, s->buffer);
        s->buffer = NULL;
        s->capacity = 0;
        return NULL;
    }

    length = strlen(input);
    if (s->buffer == NULL || length + 1u > s->capacity) {
        if (s->buffer != NULL)
            s->free(s->opaque, s->buffer);
        s->capacity = length + 1u;
        s->buffer = (char *)s->alloc(s->opaque, s->capacity);
        if (s->buffer == NULL) {
            s->capacity = 0;
            return NULL;
        }
    }

    for (i = 0; i < length; ++i) {
        unsigned char ch = (unsigned char)input[i];
        s->buffer[i] = (ch < 0x20u || ch == 0x7fu) ? '?' : (char)ch;
    }
    s->buffer[length] = '\0';
    return s->buffer;
}

/* ------------------------------------------------------------------------- */
/* 0x0016efa0 / 0x0016f10c: second CPU execution/ADC helpers                */
/* ------------------------------------------------------------------------- */

typedef struct P27DispatchCpu {
    uint32_t pending_flags;
    uint8_t pending_mask;
    uint8_t pending_pc_advance;

    uint16_t status;
    uint8_t running;
    const uint8_t *pc;
    const uint8_t *saved_pc;

    void (**dispatch)(void *opaque);
    void (*interrupt_entry)(void *opaque);
    void *opaque;
} P27DispatchCpu;

/* 0x0016efa0 */
void snes_p27_0016efa0(P27DispatchCpu *s)
{
    const uint8_t *next_pc;
    const uint8_t *saved;
    int count = 0;

    if ((s->pending_flags & 0x800u) != 0) {
        if (s->pending_mask == 0) {
            s->pending_flags &= ~0x800u;
        } else {
            if (s->pending_pc_advance != 0) {
                s->pending_pc_advance = 0;
                s->pc++;
            }
            if ((s->status & 4u) == 0 && s->interrupt_entry != NULL)
                s->interrupt_entry(s->opaque);
        }
    }

    next_pc = s->pc;
    saved = s->saved_pc;

    do {
        s->saved_pc = next_pc;
        if (s->running == 0) {
            s->pc = s->saved_pc;
            s->saved_pc = saved;
            return;
        }

        s->pc = s->saved_pc + 1;
        count++;
        s->dispatch[*s->saved_pc](s->opaque);
        next_pc = s->pc;
        saved = s->saved_pc;
    } while (count < 3);
}

typedef struct P27AdcCpu {
    uint8_t accumulator;
    uint8_t carry_latch;
    uint8_t zero_latch;
    uint8_t negative_latch;
    uint8_t overflow_latch;
    uint16_t status;

    uint8_t operand_shadow;
    uint16_t wide_sum_shadow;

    uint8_t (*read_operand)(void *opaque);
    void *opaque;
} P27AdcCpu;

/* 0x0016f10c */
void snes_p27_0016f10c(P27AdcCpu *s)
{
    uint32_t operand = s->read_operand(s->opaque);
    uint32_t a = s->accumulator;
    uint32_t result;
    uint32_t overflow_bits;

    s->operand_shadow = (uint8_t)operand;

    if ((s->status & 8u) == 0) {
        uint32_t sum = a + (operand & 0xffu) + s->carry_latch;

        s->carry_latch = (uint8_t)(sum > 0xffu);
        result = sum & 0xffu;
        s->wide_sum_shadow = (uint16_t)sum;
        overflow_bits = ~(operand ^ a) & (operand ^ result);
    } else {
        uint32_t lo = (a & 0x0fu) + (operand & 0x0fu) + s->carry_latch;
        uint32_t hi = a >> 4;

        if ((lo & 0xffu) > 9u) {
            hi++;
            lo = (lo - 10u) & 0x0fu;
        }

        hi += operand >> 4;
        s->carry_latch = (uint8_t)(hi > 9u);
        if (s->carry_latch)
            hi = (hi - 10u) & 0x0fu;

        result = (lo & 0x0fu) | ((hi << 4) & 0xf0u);
        s->wide_sum_shadow = (uint16_t)result;
        overflow_bits = ~(a ^ operand) & (operand ^ result);
    }

    s->overflow_latch = (uint8_t)((overflow_bits & 0x80u) != 0);
    s->negative_latch = (uint8_t)result;
    s->zero_latch = (uint8_t)result;
    s->accumulator = (uint8_t)result;
}

/* ------------------------------------------------------------------------- */
/* 0x0017028c / 0x001725ac: tagged stream readers                            */
/* ------------------------------------------------------------------------- */

typedef struct P27StreamOps {
    long (*read)(void *opaque, void *stream, void *buffer, size_t size);
    long (*tell)(void *opaque, void *stream);
    int (*seek)(void *opaque, void *stream, long offset, int whence);
    void *(*alloc)(void *opaque, size_t size);
    void (*free)(void *opaque, void *ptr);
    void *opaque;
} P27StreamOps;

static long p27_parse_fixed_decimal(const uint8_t *text, size_t length)
{
    char buffer[32];
    size_t n = length < sizeof(buffer) - 1u
             ? length
             : sizeof(buffer) - 1u;

    memcpy(buffer, text, n);
    buffer[n] = '\0';
    return strtol(buffer, NULL, 10);
}

static int p27_discard(P27StreamOps *ops, void *stream, size_t amount)
{
    uint8_t *temporary;
    long got;

    if (amount == 0)
        return 1;

    temporary = (uint8_t *)ops->alloc(ops->opaque, amount);
    if (temporary == NULL)
        return 0;

    got = ops->read(ops->opaque, stream, temporary, amount);
    ops->free(ops->opaque, temporary);
    return got == (long)amount;
}

/* 0x0017028c */
int snes_p27_0017028c(P27StreamOps *ops, const uint8_t magic4[4],
                      void *destination, int capacity, void *stream)
{
    uint8_t header[0x20];
    long declared;
    int to_read;
    int excess = 0;

    if (ops->read(ops->opaque, stream, header, 0x0b) != 0x0b)
        return -1;
    if (memcmp(header, magic4, 4) != 0)
        return -1;

    declared = p27_parse_fixed_decimal(header + 4, 7);
    if (declared == 0)
        return -1;

    to_read = (int)declared;
    if (capacity < to_read) {
        excess = to_read - capacity;
        to_read = capacity;
    }

    if (ops->read(ops->opaque, stream, destination, (size_t)to_read) != to_read)
        return -1;

    if (excess != 0 && !p27_discard(ops, stream, (size_t)excess))
        return -1;

    return 1;
}

/* 0x001725ac */
int snes_p27_001725ac(P27StreamOps *ops, void *stream,
                      const uint8_t magic3[3], void *destination,
                      long capacity)
{
    uint8_t header[0x20];
    long start = ops->tell(ops->opaque, stream);
    long declared;
    long to_read;
    long excess = 0;
    long got;

    if (ops->read(ops->opaque, stream, header, 0x0b) != 0x0b)
        goto fail;
    if (memcmp(header, magic3, 3) != 0 || header[3] != ':')
        goto fail;

    declared = p27_parse_fixed_decimal(header + 4, 7);
    if (declared == 0)
        goto fail;

    to_read = declared;
    if (capacity < to_read) {
        excess = to_read - capacity;
        to_read = capacity;
    }

    got = ops->read(ops->opaque, stream, destination, (size_t)to_read);
    if (got != to_read)
        goto fail;

    if (excess != 0 && !p27_discard(ops, stream, (size_t)excess))
        goto fail;

    return 1;

fail:
    if (start >= 0)
        ops->seek(ops->opaque, stream, start, 0);
    return -1;
}

/* ------------------------------------------------------------------------- */
/* 0x0017124c: state-load wrapper and error mapping                          */
/* ------------------------------------------------------------------------- */

typedef struct P27StateLoadHooks {
    int (*already_active)(void *opaque);
    int (*prepare_path)(void *opaque, const char *path);
    int (*open_read)(void *opaque, const char *path, void **handle_out);
    long (*load_state)(void *opaque, void *handle);
    void (*close)(void *opaque, void *handle);
    void (*format_detail)(void *opaque, char *buffer, size_t size);
    void (*show_error)(void *opaque, int group, int code, const char *text);
    void *opaque;

    const char *error_bad_version;
    const char *error_invalid;
    const char *error_detail_fallback;
} P27StateLoadHooks;

/* 0x0017124c */
int snes_p27_0017124c(P27StateLoadHooks *h, const char *path)
{
    void *handle = NULL;
    long result;

    if (h->already_active(h->opaque) != 0)
        return 1;
    if (h->prepare_path(h->opaque, path) != 0)
        return 1;
    if (!h->open_read(h->opaque, path, &handle))
        return 0;

    result = h->load_state(h->opaque, handle);
    if (result == 1) {
        h->close(h->opaque, handle);
        return 1;
    }

    if (result == -2) {
        h->show_error(h->opaque, 4, 14, h->error_bad_version);
    } else if (result == -1) {
        h->show_error(h->opaque, 4, 13, h->error_invalid);
    } else {
        char detail[256];

        detail[0] = '\0';
        h->format_detail(h->opaque, detail, sizeof(detail));
        h->show_error(h->opaque, 4, 15,
                      detail[0] != '\0' ? detail : h->error_detail_fallback);
    }

    h->close(h->opaque, handle);
    return 0;
}

/* ------------------------------------------------------------------------- */
/* 0x00171e0c / 0x001721ec: descriptor-driven endian serialization          */
/* ------------------------------------------------------------------------- */

typedef struct P27FieldDescriptor {
    int32_t offset;
    int32_t count;
    int32_t kind;
} P27FieldDescriptor;

typedef struct P27StateRecordOps {
    void *(*alloc)(void *opaque, size_t size);
    void (*free)(void *opaque, void *ptr);
    int (*write_record)(void *opaque, void *stream, const void *tag,
                        const void *data, size_t size);
    int (*read_record)(void *opaque, void *stream, const void *tag,
                       void *data, size_t size);
    void *opaque;
} P27StateRecordOps;

static size_t p27_field_size(const P27FieldDescriptor *d)
{
    if (d->count <= 0)
        return 0;

    switch (d->kind) {
    case 0:
    case 1:
        return (size_t)d->count;
    case 2:
        return (size_t)d->count * 2u;
    case 3:
        return (size_t)d->count * 4u;
    default:
        return 0;
    }
}

static size_t p27_descriptor_extent(const P27FieldDescriptor *d, int count)
{
    size_t extent = 0;
    int i;

    for (i = 0; i < count; ++i) {
        size_t end = (size_t)d[i].offset + p27_field_size(&d[i]);
        if (extent < end)
            extent = end;
    }
    return extent;
}

static uint16_t p27_load16(const uint8_t *p)
{
    uint16_t value;
    memcpy(&value, p, sizeof(value));
    return value;
}

static uint32_t p27_load32(const uint8_t *p)
{
    uint32_t value;
    memcpy(&value, p, sizeof(value));
    return value;
}

static uint64_t p27_load64(const uint8_t *p)
{
    uint64_t value;
    memcpy(&value, p, sizeof(value));
    return value;
}

static void p27_store16(uint8_t *p, uint16_t value)
{
    memcpy(p, &value, sizeof(value));
}

static void p27_store32(uint8_t *p, uint32_t value)
{
    memcpy(p, &value, sizeof(value));
}

static void p27_store64(uint8_t *p, uint64_t value)
{
    memcpy(p, &value, sizeof(value));
}

static uint8_t *p27_encode_one(uint8_t *dst, const uint8_t *src,
                               const P27FieldDescriptor *d)
{
    int i;

    if (d->kind == 1) {
        memcpy(dst, src + d->offset, (size_t)d->count);
        return dst + d->count;
    }

    if (d->kind == 0) {
        switch (d->count) {
        case 1:
            *dst++ = src[d->offset];
            break;
        case 2: {
            uint16_t v = p27_load16(src + d->offset);
            *dst++ = (uint8_t)(v >> 8);
            *dst++ = (uint8_t)v;
            break;
        }
        case 4: {
            uint32_t v = p27_load32(src + d->offset);
            *dst++ = (uint8_t)(v >> 24);
            *dst++ = (uint8_t)(v >> 16);
            *dst++ = (uint8_t)(v >> 8);
            *dst++ = (uint8_t)v;
            break;
        }
        case 8: {
            uint64_t v = p27_load64(src + d->offset);
            *dst++ = (uint8_t)(v >> 56);
            *dst++ = (uint8_t)(v >> 48);
            *dst++ = (uint8_t)(v >> 40);
            *dst++ = (uint8_t)(v >> 32);
            *dst++ = (uint8_t)(v >> 24);
            *dst++ = (uint8_t)(v >> 16);
            *dst++ = (uint8_t)(v >> 8);
            *dst++ = (uint8_t)v;
            break;
        }
        default:
            break;
        }
        return dst;
    }

    if (d->kind == 2) {
        for (i = 0; i < d->count; ++i) {
            uint16_t v = p27_load16(src + d->offset + i * 2);
            *dst++ = (uint8_t)(v >> 8);
            *dst++ = (uint8_t)v;
        }
        return dst;
    }

    if (d->kind == 3) {
        for (i = 0; i < d->count; ++i) {
            uint32_t v = p27_load32(src + d->offset + i * 4);
            *dst++ = (uint8_t)(v >> 24);
            *dst++ = (uint8_t)(v >> 16);
            *dst++ = (uint8_t)(v >> 8);
            *dst++ = (uint8_t)v;
        }
    }

    return dst;
}

static const uint8_t *p27_decode_one(uint8_t *dst, const uint8_t *src,
                                     const P27FieldDescriptor *d)
{
    int i;

    if (d->kind == 1) {
        memcpy(dst + d->offset, src, (size_t)d->count);
        return src + d->count;
    }

    if (d->kind == 0) {
        switch (d->count) {
        case 1:
            dst[d->offset] = *src++;
            break;
        case 2:
            p27_store16(dst + d->offset,
                        (uint16_t)((uint16_t)src[0] << 8) | src[1]);
            src += 2;
            break;
        case 4:
            p27_store32(dst + d->offset,
                        ((uint32_t)src[0] << 24) |
                        ((uint32_t)src[1] << 16) |
                        ((uint32_t)src[2] << 8) |
                        (uint32_t)src[3]);
            src += 4;
            break;
        case 8:
            p27_store64(dst + d->offset,
                        ((uint64_t)src[0] << 56) |
                        ((uint64_t)src[1] << 48) |
                        ((uint64_t)src[2] << 40) |
                        ((uint64_t)src[3] << 32) |
                        ((uint64_t)src[4] << 24) |
                        ((uint64_t)src[5] << 16) |
                        ((uint64_t)src[6] << 8) |
                        (uint64_t)src[7]);
            src += 8;
            break;
        default:
            break;
        }
        return src;
    }

    if (d->kind == 2) {
        for (i = 0; i < d->count; ++i) {
            p27_store16(dst + d->offset + i * 2,
                        (uint16_t)((uint16_t)src[0] << 8) | src[1]);
            src += 2;
        }
        return src;
    }

    if (d->kind == 3) {
        for (i = 0; i < d->count; ++i) {
            p27_store32(dst + d->offset + i * 4,
                        ((uint32_t)src[0] << 24) |
                        ((uint32_t)src[1] << 16) |
                        ((uint32_t)src[2] << 8) |
                        (uint32_t)src[3]);
            src += 4;
        }
    }

    return src;
}

/* 0x00171e0c */
int snes_p27_00171e0c(P27StateRecordOps *ops, void *stream, const void *tag,
                      const void *state, const P27FieldDescriptor *fields,
                      int field_count)
{
    size_t size = p27_descriptor_extent(fields, field_count);
    uint8_t *buffer = (uint8_t *)ops->alloc(ops->opaque, size);
    uint8_t *cursor = buffer;
    int i;
    int result;

    if (size != 0 && buffer == NULL)
        return 0;
    if (buffer != NULL)
        memset(buffer, 0, size);

    for (i = 0; i < field_count; ++i)
        cursor = p27_encode_one(cursor, (const uint8_t *)state, &fields[i]);

    result = ops->write_record(ops->opaque, stream, tag, buffer, size);
    if (buffer != NULL)
        ops->free(ops->opaque, buffer);
    return result;
}

/* 0x001721ec */
int snes_p27_001721ec(P27StateRecordOps *ops, void *stream, const void *tag,
                      void *state, const P27FieldDescriptor *fields,
                      int field_count)
{
    size_t size = p27_descriptor_extent(fields, field_count);
    uint8_t *buffer = (uint8_t *)ops->alloc(ops->opaque, size);
    const uint8_t *cursor = buffer;
    int i;
    int result;

    if (size != 0 && buffer == NULL)
        return 0;

    result = ops->read_record(ops->opaque, stream, tag, buffer, size);
    if (result == 1) {
        for (i = 0; i < field_count; ++i)
            cursor = p27_decode_one((uint8_t *)state, cursor, &fields[i]);
    }

    if (buffer != NULL)
        ops->free(ops->opaque, buffer);
    return result;
}

/* ------------------------------------------------------------------------- */
/* 0x001726ec: fixed-layout runtime dump writer                              */
/* ------------------------------------------------------------------------- */

typedef struct P27DumpWriterOps {
    void (*set_transfer_mode)(void *opaque, int enabled);
    void *(*open_write)(void *opaque, const char *path, int mode);
    int (*write_block)(void *opaque, void *handle,
                       const void *data, size_t size);
    long (*write_byte)(void *opaque, void *handle, uint8_t value);
    long (*seek)(void *opaque, void *handle, long offset, int whence);
    long (*close)(void *opaque, void *handle);
    void *opaque;
} P27DumpWriterOps;

typedef struct P27DumpWriterState {
    const uint8_t *header_24;
    uint8_t header_byte;

    uint16_t pc_offset;
    uint16_t field_da;
    uint8_t field_dc;
    uint8_t field_d8;
    uint8_t field_dd;

    const uint8_t *ram_10000;
    const uint8_t *table_100;
    const uint8_t *tail_40;
} P27DumpWriterState;

/* 0x001726ec */
int snes_p27_001726ec(P27DumpWriterOps *ops, const P27DumpWriterState *s,
                      const char *path)
{
    void *handle;
    int ok = 0;

    ops->set_transfer_mode(ops->opaque, 1);
    handle = ops->open_write(ops->opaque, path, 3);
    if (handle == NULL)
        return 0; /* target leaves transfer mode enabled on this path */

    if (ops->write_block(ops->opaque, handle, s->header_24, 0x24) != 1)
        goto done;
    if (ops->write_byte(ops->opaque, handle, s->header_byte) == -1)
        goto done;
    if (ops->seek(ops->opaque, handle, 0x25, 0) == -1)
        goto done;

    if (ops->write_byte(ops->opaque, handle,
                        (uint8_t)s->pc_offset) == -1)
        goto done;
    if (ops->write_byte(ops->opaque, handle,
                        (uint8_t)(s->pc_offset >> 8)) == -1)
        goto done;
    if (ops->write_byte(ops->opaque, handle,
                        (uint8_t)s->field_da) == -1)
        goto done;
    if (ops->write_byte(ops->opaque, handle, s->field_dc) == -1)
        goto done;
    if (ops->write_byte(ops->opaque, handle,
                        (uint8_t)(s->field_da >> 8)) == -1)
        goto done;
    if (ops->write_byte(ops->opaque, handle, s->field_d8) == -1)
        goto done;
    if (ops->write_byte(ops->opaque, handle, s->field_dd) == -1)
        goto done;

    if (ops->seek(ops->opaque, handle, 0x100, 0) == -1)
        goto done;
    if (ops->write_block(ops->opaque, handle, s->ram_10000, 0x10000) != 1)
        goto done;
    if (ops->write_block(ops->opaque, handle, s->table_100, 0x100) != 1)
        goto done;
    if (ops->seek(ops->opaque, handle, -0x80, 1) == -1)
        goto done;
    if (ops->write_block(ops->opaque, handle, s->tail_40, 0x40) != 1)
        goto done;
    if (ops->close(ops->opaque, handle) < 0) {
        handle = NULL;
        goto done;
    }

    handle = NULL;
    ok = 1;

done:
    if (handle != NULL)
        (void)ops->close(ops->opaque, handle);
    ops->set_transfer_mode(ops->opaque, 0);
    return ok;
}
