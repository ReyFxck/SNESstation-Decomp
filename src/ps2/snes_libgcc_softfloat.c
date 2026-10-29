/*
 * SNES Station v0.23 PS2 - libgcc Soft-Float Helpers
 * Canonical source from progress13_libgcc_softfloat.c @ 0x001a7e30, 0x001a80d0,
 * 0x001a82c0, 0x001a7f40, 0x001a7f10, 0x001a3c90, 0x001a81b8
 */

#include <stdint.h>
#include <string.h>

enum {
    SNES_FP_CLASS_QNAN = 0,
    SNES_FP_CLASS_SNAN = 1,
    SNES_FP_CLASS_ZERO = 2,
    SNES_FP_CLASS_NUMBER = 3,
    SNES_FP_CLASS_INFINITY = 4
};

typedef struct {
    uint32_t fp_class; /* +0x00 */
    uint32_t sign;     /* +0x04 */
    int32_t exponent;  /* +0x08 */
    uint32_t fraction; /* +0x0c, hidden bit normalized at bit 30 */
} SnesSfParts;

typedef struct {
    uint32_t fp_class; /* +0x00 */
    uint32_t sign;     /* +0x04 */
    int32_t exponent;  /* +0x08 */
    uint32_t pad_0c;   /* +0x0c */
    uint64_t fraction; /* +0x10, hidden bit normalized at bit 60 */
} SnesDfParts;

static uint32_t snes_pack_sf_number(uint32_t sign, int32_t exponent,
                                    uint32_t fraction)
{
    uint32_t biased;
    uint32_t frac_field;

    if (fraction == 0)
        return sign << 31;

    /* Target subnormal corridor begins when exponent < -126. */
    if (exponent < -126) {
        unsigned shift = (unsigned)(-126 - exponent);
        uint32_t sticky = 0;
        uint32_t reduced;

        if (shift >= 32) {
            reduced = 0;
            sticky = fraction != 0;
        } else {
            uint32_t mask = shift == 0 ? 0u : ((UINT32_C(1) << shift) - 1u);
            sticky = (fraction & mask) != 0;
            reduced = fraction >> shift;
        }
        fraction = reduced | sticky;
        biased = 0;
    } else if (exponent >= 128) {
        return (sign << 31) | UINT32_C(0x7f800000);
    } else {
        biased = (uint32_t)(exponent + 127);
    }

    /* Target keeps seven guard/round/sticky bits before packing. */
    {
        uint32_t low = fraction & 0x7fu;
        uint32_t add = 0x3fu;
        if (low == 0x40u && (fraction & 0x80u) != 0)
            add = 0x40u; /* ties-to-even */
        fraction += add;
    }

    if ((fraction & UINT32_C(0x80000000)) != 0) {
        fraction >>= 1;
        if (++biased >= 0xffu)
            return (sign << 31) | UINT32_C(0x7f800000);
    }

    frac_field = (fraction >> 7) & UINT32_C(0x007fffff);
    return (sign << 31) | ((biased & 0xffu) << 23) | frac_field;
}

static uint64_t snes_pack_df_number(uint32_t sign, int32_t exponent,
                                    uint64_t fraction)
{
    uint32_t biased;
    uint64_t frac_field;

    if (fraction == 0)
        return (uint64_t)sign << 63;

    if (exponent < -1022) {
        unsigned shift = (unsigned)(-1022 - exponent);
        uint64_t sticky = 0;
        uint64_t reduced;

        if (shift >= 64) {
            reduced = 0;
            sticky = fraction != 0;
        } else {
            uint64_t mask = shift == 0 ? 0 : ((UINT64_C(1) << shift) - 1u);
            sticky = (fraction & mask) != 0;
            reduced = fraction >> shift;
        }
        fraction = reduced | sticky;
        biased = 0;
    } else if (exponent >= 1024) {
        return ((uint64_t)sign << 63) | UINT64_C(0x7ff0000000000000);
    } else {
        biased = (uint32_t)(exponent + 1023);
    }

    /* DF target carries eight low rounding bits. */
    {
        uint64_t low = fraction & UINT64_C(0xff);
        uint64_t add = UINT64_C(0x7f);
        if (low == UINT64_C(0x80) && (fraction & UINT64_C(0x100)) != 0)
            add = UINT64_C(0x80);
        fraction += add;
    }

    if ((fraction & UINT64_C(0x2000000000000000)) != 0) {
        fraction >>= 1;
        if (++biased >= 0x7ffu)
            return ((uint64_t)sign << 63) | UINT64_C(0x7ff0000000000000);
    }

    frac_field = (fraction >> 8) & UINT64_C(0x000fffffffffffff);
    return ((uint64_t)sign << 63) | ((uint64_t)(biased & 0x7ffu) << 52) |
           frac_field;
}

/* 0x001a7e30 -- __unpack_f. */
void snes_unpack_f_001a7e30(const uint32_t *raw_ptr, SnesSfParts *out)
{
    uint32_t raw = *raw_ptr;
    uint32_t exp = (raw >> 23) & 0xffu;
    uint32_t mantissa = raw & UINT32_C(0x007fffff);

    out->sign = raw >> 31;

    if (exp == 0) {
        if (mantissa == 0) {
            out->fp_class = SNES_FP_CLASS_ZERO;
            return;
        }

        mantissa <<= 7;
        out->fp_class = SNES_FP_CLASS_NUMBER;
        out->exponent = -126;
        while (mantissa <= UINT32_C(0x3fffffff)) {
            mantissa <<= 1;
            --out->exponent;
        }
        out->fraction = mantissa;
        return;
    }

    if (exp != 0xffu) {
        out->fp_class = SNES_FP_CLASS_NUMBER;
        out->exponent = (int32_t)exp - 127;
        out->fraction = (mantissa << 7) | UINT32_C(0x40000000);
        return;
    }

    if (mantissa == 0) {
        out->fp_class = SNES_FP_CLASS_INFINITY;
        return;
    }

    out->fraction = mantissa << 7;
    out->fp_class = (out->fraction & UINT32_C(0x00100000)) != 0
                        ? SNES_FP_CLASS_SNAN
                        : SNES_FP_CLASS_QNAN;
}

/* 0x001a80d0 -- __unpack_d. */
void snes_unpack_d_001a80d0(const uint64_t *raw_ptr, SnesDfParts *out)
{
    uint64_t raw = *raw_ptr;
    uint32_t exp = (uint32_t)((raw >> 52) & 0x7ffu);
    uint64_t mantissa = raw & UINT64_C(0x000fffffffffffff);

    out->sign = (uint32_t)(raw >> 63);
    out->pad_0c = 0;

    if (exp == 0) {
        if (mantissa == 0) {
            out->fp_class = SNES_FP_CLASS_ZERO;
            return;
        }

        mantissa <<= 8;
        out->fp_class = SNES_FP_CLASS_NUMBER;
        out->exponent = -1022;
        while (mantissa <= UINT64_C(0x0fffffffffffffff)) {
            mantissa <<= 1;
            --out->exponent;
        }
        out->fraction = mantissa;
        return;
    }

    if (exp != 0x7ffu) {
        out->fp_class = SNES_FP_CLASS_NUMBER;
        out->exponent = (int32_t)exp - 1023;
        out->fraction = (mantissa << 8) | UINT64_C(0x1000000000000000);
        return;
    }

    if (mantissa == 0) {
        out->fp_class = SNES_FP_CLASS_INFINITY;
        return;
    }

    out->fraction = mantissa << 8;
    out->fp_class = (out->fraction & UINT64_C(0x0010000000000000)) != 0
                        ? SNES_FP_CLASS_SNAN
                        : SNES_FP_CLASS_QNAN;
}

/* 0x001a82c0 -- __pack_f. */
uint32_t snes_pack_f_001a82c0(const SnesSfParts *parts)
{
    uint32_t sign = parts->sign & 1u;

    if (parts->fp_class < 2u) {
        uint32_t payload = parts->fraction | UINT32_C(0x00100000);
        return (sign << 31) | UINT32_C(0x7f800000) |
               (payload & UINT32_C(0x007fffff));
    }
    if (parts->fp_class == SNES_FP_CLASS_ZERO)
        return sign << 31;
    if (parts->fp_class == SNES_FP_CLASS_INFINITY)
        return (sign << 31) | UINT32_C(0x7f800000);
    return snes_pack_sf_number(sign, parts->exponent, parts->fraction);
}

/* 0x001a7f40 -- __pack_d. */
uint64_t snes_pack_d_001a7f40(const SnesDfParts *parts)
{
    uint32_t sign = parts->sign & 1u;

    if (parts->fp_class < 2u) {
        uint64_t payload = parts->fraction | UINT64_C(0x0008000000000000);
        return ((uint64_t)sign << 63) | UINT64_C(0x7ff0000000000000) |
               (payload & UINT64_C(0x000fffffffffffff));
    }
    if (parts->fp_class == SNES_FP_CLASS_ZERO)
        return (uint64_t)sign << 63;
    if (parts->fp_class == SNES_FP_CLASS_INFINITY)
        return ((uint64_t)sign << 63) | UINT64_C(0x7ff0000000000000);
    return snes_pack_df_number(sign, parts->exponent, parts->fraction);
}

/* 0x001a7f10 -- __make_fp: stack-builds the target SF parts then packs it. */
uint32_t snes_make_fp_001a7f10(uint32_t fp_class, uint32_t sign, int32_t exponent,
                               uint32_t fraction)
{
    SnesSfParts parts;
    parts.fp_class = fp_class;
    parts.sign = sign;
    parts.exponent = exponent;
    parts.fraction = fraction;
    return snes_pack_f_001a82c0(&parts);
}

/* 0x001a3c90 -- __make_dp: same four-field constructor for DF parts. */
uint64_t snes_make_dp_001a3c90(uint32_t fp_class, uint32_t sign, int32_t exponent,
                               uint64_t fraction)
{
    SnesDfParts parts;
    parts.fp_class = fp_class;
    parts.sign = sign;
    parts.exponent = exponent;
    parts.pad_0c = 0;
    parts.fraction = fraction;
    return snes_pack_d_001a7f40(&parts);
}

/* 0x001a81b8 -- __fpcmp_parts_d.  Return convention is -1/0/+1. */
int snes_fpcmp_parts_d_001a81b8(const SnesDfParts *a, const SnesDfParts *b)
{
    uint32_t ac = a->fp_class;
    uint32_t bc = b->fp_class;

    /* NaN classes make the comparison unordered; target returns +1. */
    if (ac < 2u || bc < 2u)
        return 1;

    if (ac == SNES_FP_CLASS_INFINITY) {
        if (bc == SNES_FP_CLASS_INFINITY)
            return (int)b->sign - (int)a->sign;
        return a->sign ? -1 : 1;
    }
    if (bc == SNES_FP_CLASS_INFINITY)
        return b->sign ? 1 : -1;

    if (ac == SNES_FP_CLASS_ZERO) {
        if (bc == SNES_FP_CLASS_ZERO)
            return 0;
        return b->sign ? 1 : -1;
    }
    if (bc == SNES_FP_CLASS_ZERO)
        return a->sign ? -1 : 1;

    if (a->sign != b->sign)
        return a->sign ? -1 : 1;

    if (a->exponent != b->exponent) {
        int r = a->exponent < b->exponent ? -1 : 1;
        return a->sign ? -r : r;
    }
    if (a->fraction != b->fraction) {
        int r = a->fraction < b->fraction ? -1 : 1;
        return a->sign ? -r : r;
    }
    return 0;
}

/* 0x001a3380..0x001a35ac -- _fpadd_parts_d (soft-double addition). */
static const SnesDfParts snes_thenan_d = {
    SNES_FP_CLASS_QNAN, 0, 0, 0, 0
};

static uint64_t snes_sticky_rshift1(uint64_t x)
{
    return (x >> 1) | (x & UINT64_C(1));
}

const SnesDfParts *snes_fpadd_parts_d_001a3380(
    const SnesDfParts *a, const SnesDfParts *b, SnesDfParts *tmp)
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

    if (a->fp_class == SNES_FP_CLASS_INFINITY) {
        if (b->fp_class == SNES_FP_CLASS_INFINITY && a->sign != b->sign)
            return &snes_thenan_d;
        return a;
    }
    if (b->fp_class == SNES_FP_CLASS_INFINITY)
        return b;

    /*
     * The target has a distinct both-zero corridor: copy A's parts into tmp
     * and AND the signs.  This yields -0 only for (-0)+(-0).
     */
    if (b->fp_class == SNES_FP_CLASS_ZERO) {
        if (a->fp_class != SNES_FP_CLASS_ZERO)
            return a;
        *tmp = *a;
        tmp->sign = a->sign & b->sign;
        return tmp;
    }
    if (a->fp_class == SNES_FP_CLASS_ZERO)
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
            b_frac = snes_sticky_rshift1(b_frac);
        }
        while (b_exp > a_exp) {
            ++a_exp;
            a_frac = snes_sticky_rshift1(a_frac);
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

    tmp->fp_class = SNES_FP_CLASS_NUMBER;
    tmp->pad_0c = 0;

    if (tmp->fraction >= UINT64_C(0x2000000000000000)) {
        tmp->fraction = snes_sticky_rshift1(tmp->fraction);
        ++tmp->exponent;
    }

    return tmp;
}