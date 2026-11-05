/*
 * Progress 12: six fixed-point 3x3 transform leaves recovered from the
 * SNES Station v0.23 R5900 target.
 *
 * The target binds each entry to a different group of globals.  These host
 * models make the matrix/vector storage explicit while preserving the exact
 * arithmetic: each signed 16-bit product is shifted right by 15 before the
 * three terms are added, and the final result is stored as signed 16-bit.
 */
#include <stdint.h>

static int32_t p12_asr15(int32_t value)
{
    if (value >= 0)
        return value / 32768;
    return -(int32_t)(((uint32_t)(-value) + 32767u) / 32768u);
}

static int16_t p12_q15_dot_rows(const int16_t *m, const int16_t *v, unsigned row)
{
    int32_t a = p12_asr15((int32_t)m[row * 3u + 0u] * (int32_t)v[0]);
    int32_t b = p12_asr15((int32_t)m[row * 3u + 1u] * (int32_t)v[1]);
    int32_t c = p12_asr15((int32_t)m[row * 3u + 2u] * (int32_t)v[2]);
    return (int16_t)(a + b + c);
}

static int16_t p12_q15_dot_columns(const int16_t *m, const int16_t *v,
                                   unsigned column)
{
    int32_t a = p12_asr15((int32_t)m[0u * 3u + column] * (int32_t)v[0]);
    int32_t b = p12_asr15((int32_t)m[1u * 3u + column] * (int32_t)v[1]);
    int32_t c = p12_asr15((int32_t)m[2u * 3u + column] * (int32_t)v[2]);
    return (int16_t)(a + b + c);
}

static void p12_transform_rows(const int16_t matrix[9], const int16_t input[3],
                               int16_t output[3])
{
    output[0] = p12_q15_dot_rows(matrix, input, 0u);
    output[1] = p12_q15_dot_rows(matrix, input, 1u);
    output[2] = p12_q15_dot_rows(matrix, input, 2u);
}

static void p12_transform_columns(const int16_t matrix[9],
                                  const int16_t input[3], int16_t output[3])
{
    output[0] = p12_q15_dot_columns(matrix, input, 0u);
    output[1] = p12_q15_dot_columns(matrix, input, 1u);
    output[2] = p12_q15_dot_columns(matrix, input, 2u);
}

/* 0x0012d79c: matrix @ target 0x00341530, vector 0x00341568 -> 0x0034156e. */
void snes_p12_0012d79c(const int16_t matrix[9], const int16_t input[3],
                       int16_t output[3])
{
    p12_transform_rows(matrix, input, output);
}

/* 0x0012d85c: matrix @ target 0x00341518, vector 0x00341574 -> 0x0034157a. */
void snes_p12_0012d85c(const int16_t matrix[9], const int16_t input[3],
                       int16_t output[3])
{
    p12_transform_rows(matrix, input, output);
}

/* 0x0012d91c: matrix @ target 0x00341500, vector 0x00341580 -> 0x00341586. */
void snes_p12_0012d91c(const int16_t matrix[9], const int16_t input[3],
                       int16_t output[3])
{
    p12_transform_rows(matrix, input, output);
}

/* 0x0012d9dc: matrix @ target 0x00341530, vector 0x0034158c -> 0x00341592. */
void snes_p12_0012d9dc(const int16_t matrix[9], const int16_t input[3],
                       int16_t output[3])
{
    p12_transform_columns(matrix, input, output);
}

/* 0x0012da9c: matrix @ target 0x00341518, vector 0x00341598 -> 0x0034159e. */
void snes_p12_0012da9c(const int16_t matrix[9], const int16_t input[3],
                       int16_t output[3])
{
    p12_transform_columns(matrix, input, output);
}

/* 0x0012db5c: matrix @ target 0x00341500, vector 0x003415a4 -> 0x003415aa. */
void snes_p12_0012db5c(const int16_t matrix[9], const int16_t input[3],
                       int16_t output[3])
{
    p12_transform_columns(matrix, input, output);
}
