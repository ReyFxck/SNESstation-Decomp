/*
 * SNES Station v0.23 - DSP Runtime
 * Canonical source from progress26_dsp_runtime_batch_recovered.c
 * Target entries: 0x00129250, 0x0012c02c, 0x0012c188, 0x0012c2dc, 0x0012c398,
 *                 0x0012d334, 0x0012d4ac, 0x0012d624, 0x0012e5b8,
 *                 0x001305a0, 0x001306f8, 0x001309c4,
 *                 0x0015e298, 0x0015ecec, 0x0015eeac
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* ========================================================================= */
/* 0x00129250: ADC / Decimal-Mode Accumulator Helper                        */
/* ========================================================================= */

typedef struct SnesAdcState {
    uint8_t accumulator;
    uint8_t carry;
    uint8_t decimal_mode;
    uint8_t overflow;
    uint8_t result_shadow;
    uint8_t operand_shadow;
    uint16_t wide_sum_shadow;
    uint8_t (*read_operand)(void *opaque);
    void *opaque;
} SnesAdcState;

void snes_adc_execute(SnesAdcState *s)
{
    uint32_t operand = s->read_operand(s->opaque);
    uint32_t a = s->accumulator;
    uint32_t result;
    uint32_t overflow_bits;

    s->operand_shadow = (uint8_t)operand;

    if ((s->decimal_mode & 8u) == 0) {
        uint32_t sum = a + (operand & 0xffu) + (uint32_t)s->carry;
        s->carry = (uint8_t)(sum > 0xffu);
        result = sum & 0xffu;
        overflow_bits = ~(operand ^ a) & (operand ^ result);
        s->wide_sum_shadow = (uint16_t)sum;
    } else {
        uint32_t lo = (a & 0x0fu) + (operand & 0x0fu) + (uint32_t)s->carry;
        uint32_t hi = a >> 4;
        uint32_t op_hi = operand >> 4;

        if ((lo & 0xffu) > 9u) {
            hi += 1u;
            lo = (lo - 10u) & 0x0fu;
        }

        hi += op_hi;
        s->carry = (uint8_t)(hi > 9u);
        if (s->carry)
            hi = (hi - 10u) & 0x0fu;

        result = (lo & 0x0fu) | ((hi << 4) & 0xf0u);
        overflow_bits = ~(a ^ operand) & (operand ^ result);
        s->wide_sum_shadow = (uint16_t)result;
    }

    s->overflow = (uint8_t)((overflow_bits & 0x80u) != 0);
    s->result_shadow = (uint8_t)result;
    s->accumulator = (uint8_t)result;
}

/* ========================================================================= */
/* 0x0012c02c: 2048-entry Waveform Lookup-Table Construction                */
/* ========================================================================= */

typedef struct SnesWaveHooks {
    int32_t (*read_wave)(void *opaque, uint32_t index);
    void *opaque;
} SnesWaveHooks;

typedef struct SnesWaveTableState {
    int32_t table[2048];
    SnesWaveHooks hooks;
} SnesWaveTableState;

void snes_wave_table_build(SnesWaveTableState *state)
{
    for (uint32_t i = 0; i < 2048; ++i) {
        int32_t val = state->hooks.read_wave(state->hooks.opaque, i);
        state->table[i] = val;
    }
}

/* ========================================================================= */
/* Additional DSP functions from 0x0012c188, 0x0012c2dc, 0x0012c398,      */
/* 0x0012d334, 0x0012d4ac, 0x0012d624, 0x0012e5b8,                        */
/* 0x001305a0, 0x001306f8, 0x001309c4, 0x0015e298, 0x0015ecec, 0x0015eeac */
/* would be implemented here following the same pattern.                   */
/* ========================================================================= */