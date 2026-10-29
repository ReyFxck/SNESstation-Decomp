/*
 * SNES Station v0.23 PS2 - ADC / Decimal-Mode Accumulator Helper
 * Canonical source from progress26_dsp_runtime_batch.c @ 0x00129250
 */

#include <stdint.h>

typedef struct {
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

void snes_adc_process_00129250(SnesAdcState *s)
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