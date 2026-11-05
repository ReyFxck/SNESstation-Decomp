/*
 * Progress 15 -- _fpadd_parts_d @ 0x001a3380.
 *
 * Behavioral reconstruction from the target EE assembly.  The target is the
 * GCC 3.2.2-b1 / US_SOFTWARE_GOFAST soft-double family, but target-specific
 * branches (notably signed-zero handling) are preserved from the binary.
 */
#include <stdint.h>

enum {
    SNES_P15_FP_QNAN = 0,
    SNES_P15_FP_SNAN = 1,
    SNES_P15_FP_ZERO = 2,
    SNES_P15_FP_NUMBER = 3,
    SNES_P15_FP_INFINITY = 4
};

typedef struct SnesP15DfParts {
    uint32_t fp_class;
    uint32_t sign;
    int32_t exponent;
    uint32_t pad_0c;
    uint64_t fraction;
} SnesP15DfParts;

typedef char snes_p15_df_parts_size[
    (sizeof(SnesP15DfParts) == 0x18) ? 1 : -1];

static const SnesP15DfParts snes_p15_thenan_d = {
    SNES_P15_FP_QNAN, 0, 0, 0, 0
};

static uint64_t sticky_rshift1(uint64_t x)
{
    return (x >> 1) | (x & UINT64_C(1));
}

/* 0x001a3380..0x001a35ac */
const SnesP15DfParts *snes_p15_fpadd_parts_d_001a3380(
    const SnesP15DfParts *a, const SnesP15DfParts *b, SnesP15DfParts *tmp)
{
    int32_t a_exp;
    int32_t b_exp;
    uint64_t a_frac;
    uint64_t b_frac;
    int32_t diff;

    if (a->fp_class < 2u)
        return a;
    if (b->fp_class < 2u)
        return b;

    if (a->fp_class == SNES_P15_FP_INFINITY) {
        if (b->fp_class == SNES_P15_FP_INFINITY && a->sign != b->sign)
            return &snes_p15_thenan_d;
        return a;
    }
    if (b->fp_class == SNES_P15_FP_INFINITY)
        return b;

    /*
     * The target has a distinct both-zero corridor: copy A's parts into tmp
     * and AND the signs.  This yields -0 only for (-0)+(-0).
     */
    if (b->fp_class == SNES_P15_FP_ZERO) {
        if (a->fp_class != SNES_P15_FP_ZERO)
            return a;
        *tmp = *a;
        tmp->sign = a->sign & b->sign;
        return tmp;
    }
    if (a->fp_class == SNES_P15_FP_ZERO)
        return b;

    a_exp = a->exponent;
    b_exp = b->exponent;
    a_frac = a->fraction;
    b_frac = b->fraction;

    diff = a_exp - b_exp;
    if (diff < 0)
        diff = -diff;

    if (diff < 64) {
        while (a_exp > b_exp) {
            ++b_exp;
            b_frac = sticky_rshift1(b_frac);
        }
        while (b_exp > a_exp) {
            ++a_exp;
            a_frac = sticky_rshift1(a_frac);
        }
    } else if (a_exp > b_exp) {
        b_exp = a_exp;
        b_frac = 0;
    } else {
        a_exp = b_exp;
        a_frac = 0;
    }

    if (a->sign != b->sign) {
        int64_t signed_fraction;

        if (a->sign)
            signed_fraction = (int64_t)(b_frac - a_frac);
        else
            signed_fraction = (int64_t)(a_frac - b_frac);

        tmp->exponent = a_exp;
        if (signed_fraction < 0) {
            tmp->sign = 1;
            tmp->fraction = (uint64_t)(-signed_fraction);
        } else {
            tmp->sign = 0;
            tmp->fraction = (uint64_t)signed_fraction;
        }

        while (tmp->fraction != 0 &&
               tmp->fraction < UINT64_C(0x1000000000000000)) {
            tmp->fraction <<= 1;
            --tmp->exponent;
        }
    } else {
        tmp->sign = a->sign;
        tmp->exponent = a_exp;
        tmp->fraction = a_frac + b_frac;
    }

    tmp->fp_class = SNES_P15_FP_NUMBER;
    tmp->pad_0c = 0;

    if (tmp->fraction >= UINT64_C(0x2000000000000000)) {
        tmp->fraction = sticky_rshift1(tmp->fraction);
        ++tmp->exponent;
    }

    return tmp;
}
