/*
 * Canonical libgcc soft-float implementation for SNES Station v0.23 (EE/R5900 GCC 3.2.2).
 *
 * Consolidated from:
 *   - progress13_libgcc_softfloat_recovered.c (single/double pack/unpack, classification)
 *   - progress15_softdouble_add_recovered.c (_fpadd_parts_d)
 *
 * The stripped target remains authoritative. These routines model the exact
 * target part layouts and IEEE class/sign/exponent/fraction transformations.
 */

#include <stdint.h>
#include <string.h>

/* ========================================================================= */
/* IEEE 754 Class Enumeration                                               */
/* ========================================================================= */

enum {
    SNES_FP_CLASS_QNAN     = 0,
    SNES_FP_CLASS_SNAN     = 1,
    SNES_FP_CLASS_ZERO     = 2,
    SNES_FP_CLASS_NUMBER   = 3,
    SNES_FP_CLASS_INFINITY = 4
};

/* ========================================================================= */
/* Single-Precision (32-bit) Parts                                          */
/* ========================================================================= */

typedef struct {
    uint32_t fp_class;  /* +0x00 */
    uint32_t sign;      /* +0x04 */
    int32_t  exponent;  /* +0x08 */
    uint32_t fraction;  /* +0x0c, hidden bit normalized at bit 30 */
} SnesSfParts;

/* ========================================================================= */
/* Double-Precision (64-bit) Parts                                          */
/* ========================================================================= */

typedef struct {
    uint32_t fp_class;  /* +0x00 */
    uint32_t sign;      /* +0x04 */
    int32_t  exponent;  /* +0x08 */
    uint32_t pad_0c;    /* +0x0c */
    uint64_t fraction;  /* +0x10, hidden bit normalized at bit 60 */
} SnesDfParts;

typedef char snes_df_parts_size[(sizeof(SnesDfParts) == 0x18) ? 1 : -1];

/* ========================================================================= */
/* Canonical Quiet NaN (double)                                             */
/* ========================================================================= */

static const SnesDfParts snes_canonical_qnan_d = {
    SNES_FP_CLASS_QNAN, 0, 0, 0, 0
};

/* ========================================================================= */
/* Utility: Sticky Right Shift by 1                                         */
/* ========================================================================= */

static uint64_t snes_sticky_rshift1(uint64_t x)
{
    return (x >> 1) | (x & UINT64_C(1));
}

/* ========================================================================= */
/* Single-Precision Pack                                                    */
/* ========================================================================= */

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

    return (sign << 31) | (biased << 23) | (fraction >> 8);
}

/* ========================================================================= */
/* Double-Precision Pack                                                    */
/* ========================================================================= */

static uint64_t snes_pack_df_number(uint32_t sign, int32_t exponent,
                                    uint64_t fraction)
{
    uint32_t biased;

    if (fraction == 0)
        return (uint64_t)sign << 63;

    /* Target subnormal corridor begins when exponent < -1022. */
    if (exponent < -1022) {
        unsigned shift = (unsigned)(-1022 - exponent);
        uint64_t sticky = 0;
        uint64_t reduced;

        if (shift >= 64) {
            reduced = 0;
            sticky = fraction != 0;
        } else {
            uint64_t mask = shift == 0 ? 0u : ((UINT64_C(1) << shift) - 1u);
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

    /* Target keeps seven guard/round/sticky bits before packing. */
    {
        uint64_t low = fraction & 0x7fu;
        uint64_t add = 0x3fu;
        if (low == 0x40u && (fraction & 0x80u) != 0)
            add = 0x40u; /* ties-to-even */
        fraction += add;
    }

    if ((fraction & UINT64_C(0x8000000000000000)) != 0) {
        fraction >>= 1;
        if (++biased >= 0x7ffu)
            return ((uint64_t)sign << 63) | UINT64_C(0x7ff0000000000000);
    }

    return ((uint64_t)sign << 63) | ((uint64_t)biased << 52) | (fraction >> 11);
}

/* ========================================================================= */
/* Single-Precision Unpack / Classification                                 */
/* ========================================================================= */

void snes_unpack_sf(uint32_t raw, SnesSfParts *out)
{
    uint32_t sign = raw >> 31;
    uint32_t exp  = (raw >> 23) & 0xffu;
    uint32_t frac = raw & 0x7fffffu;

    out->sign = sign;

    if (exp == 0) {
        if (frac == 0) {
            out->fp_class = SNES_FP_CLASS_ZERO;
            out->exponent = 0;
            out->fraction = 0;
        } else {
            out->fp_class = SNES_FP_CLASS_NUMBER;
            /* Subnormal: normalize fraction */
            int lz = __builtin_clz(frac) - 8; /* 23 bits -> leading zeros in 32-bit */
            out->exponent = -126 - lz;
            out->fraction = frac << (lz + 1); /* hidden bit at bit 30 */
        }
    } else if (exp == 0xff) {
        if (frac == 0) {
            out->fp_class = SNES_FP_CLASS_INFINITY;
            out->exponent = 0;
            out->fraction = 0;
        } else {
            out->fp_class = (frac & 0x400000) ? SNES_FP_CLASS_QNAN : SNES_FP_CLASS_SNAN;
            out->exponent = 0;
            out->fraction = frac << 8; /* hidden bit at bit 30 */
        }
    } else {
        out->fp_class = SNES_FP_CLASS_NUMBER;
        out->exponent = (int32_t)exp - 127;
        out->fraction = (frac | 0x800000) << 7; /* hidden bit at bit 30 */
    }
}

/* ========================================================================= */
/* Double-Precision Unpack / Classification                                 */
/* ========================================================================= */

void snes_unpack_df(uint64_t raw, SnesDfParts *out)
{
    uint32_t sign = (uint32_t)(raw >> 63);
    uint32_t exp  = (uint32_t)((raw >> 52) & 0x7ffu);
    uint64_t frac = raw & UINT64_C(0xfffffffffffff);

    out->sign = sign;
    out->pad_0c = 0;

    if (exp == 0) {
        if (frac == 0) {
            out->fp_class = SNES_FP_CLASS_ZERO;
            out->exponent = 0;
            out->fraction = 0;
        } else {
            out->fp_class = SNES_FP_CLASS_NUMBER;
            /* Subnormal: normalize fraction */
            int lz = __builtin_clzll(frac) - 11; /* 52 bits -> leading zeros in 64-bit */
            out->exponent = -1022 - lz;
            out->fraction = frac << (lz + 1); /* hidden bit at bit 60 */
        }
    } else if (exp == 0x7ff) {
        if (frac == 0) {
            out->fp_class = SNES_FP_CLASS_INFINITY;
            out->exponent = 0;
            out->fraction = 0;
        } else {
            out->fp_class = (frac & UINT64_C(0x8000000000000)) ? SNES_FP_CLASS_QNAN : SNES_FP_CLASS_SNAN;
            out->exponent = 0;
            out->fraction = frac << 11; /* hidden bit at bit 60 */
        }
    } else {
        out->fp_class = SNES_FP_CLASS_NUMBER;
        out->exponent = (int32_t)exp - 1023;
        out->fraction = (frac | UINT64_C(0x10000000000000)) << 10; /* hidden bit at bit 60 */
    }
}

/* ========================================================================= */
/* Double-Precision Addition (_fpadd_parts_d @ 0x001a3380)                */
/* ========================================================================= */

const SnesDfParts *snes_fpadd_parts_d(const SnesDfParts *a, const SnesDfParts *b,
                                       SnesDfParts *tmp)
{
    int32_t a_exp, b_exp;
    uint64_t a_frac, b_frac;
    int32_t diff;

    /* Handle NaNs */
    if (a->fp_class < 2u)
        return a;
    if (b->fp_class < 2u)
        return b;

    /* Handle infinities */
    if (a->fp_class == SNES_FP_CLASS_INFINITY) {
        if (b->fp_class == SNES_FP_CLASS_INFINITY && a->sign != b->sign)
            return &snes_canonical_qnan_d;
        return a;
    }
    if (b->fp_class == SNES_FP_CLASS_INFINITY)
        return b;

    /* Both zero: distinct corridor - copy A, AND the signs.
     * Yields -0 only for (-0)+(-0). */
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

    if (diff > 0) {
        if (diff >= 64) {
            b_frac = 0;
        } else {
            uint64_t sticky = b_frac & ((UINT64_C(1) << diff) - 1);
            b_frac >>= diff;
            b_frac |= sticky;
        }
    } else if (diff < 0) {
        diff = -diff;
        if (diff >= 64) {
            a_frac = 0;
        } else {
            uint64_t sticky = a_frac & ((UINT64_C(1) << diff) - 1);
            a_frac >>= diff;
            a_frac |= sticky;
        }
        a_exp = b_exp;
    }

    if (a->sign == b->sign) {
        a_frac += b_frac;
        if ((a_frac & UINT64_C(0x8000000000000000)) != 0) {
            a_frac = snes_sticky_rshift1(a_frac);
            if (++a_exp >= 1024) {
                tmp->fp_class = SNES_FP_CLASS_INFINITY;
                tmp->sign = a->sign;
                tmp->exponent = 0;
                tmp->pad_0c = 0;
                tmp->fraction = 0;
                return tmp;
            }
        }
        tmp->sign = a->sign;
    } else {
        if (a_frac >= b_frac) {
            a_frac -= b_frac;
            tmp->sign = a->sign;
        } else {
            a_frac = b_frac - a_frac;
            tmp->sign = b->sign;
        }

        if (a_frac == 0) {
            tmp->fp_class = SNES_FP_CLASS_ZERO;
            tmp->sign = a->sign & b->sign; /* -0 only if both -0 */
            tmp->exponent = 0;
            tmp->pad_0c = 0;
            tmp->fraction = 0;
            return tmp;
        }

        /* Normalize result */
        int lz = __builtin_clzll(a_frac) - 1;
        a_frac <<= lz;
        a_exp -= lz;
    }

    /* Pack with rounding */
    uint64_t packed = snes_pack_df_number(tmp->sign, a_exp, a_frac);
    snes_unpack_df(packed, tmp);
    return tmp;
}

/* ========================================================================= */
/* Single-Precision Addition (uses double internally per target ABI)        */
/* ========================================================================= */

const SnesSfParts *snes_fpadd_parts_f(const SnesSfParts *a, const SnesSfParts *b,
                                       SnesSfParts *tmp)
{
    /* Promote to double, add, then demote - per target behavior */
    SnesDfParts da, db, dt;

    da.fp_class = a->fp_class;
    da.sign = a->sign;
    da.exponent = a->exponent;
    da.pad_0c = 0;
    da.fraction = (uint64_t)a->fraction << 30; /* align hidden bit */

    db.fp_class = b->fp_class;
    db.sign = b->sign;
    db.exponent = b->exponent;
    db.pad_0c = 0;
    db.fraction = (uint64_t)b->fraction << 30;

    const SnesDfParts *dr = snes_fpadd_parts_d(&da, &db, &dt);

    if (dr->fp_class == SNES_FP_CLASS_QNAN || dr->fp_class == SNES_FP_CLASS_SNAN) {
        tmp->fp_class = dr->fp_class;
        tmp->sign = dr->sign;
        tmp->exponent = 0;
        tmp->fraction = 0;
        return tmp;
    }

    uint64_t raw = snes_pack_df_number(dr->sign, dr->exponent, dr->fraction);
    uint32_t raw_f = (uint32_t)(raw >> 32); /* truncate to single */
    snes_unpack_sf(raw_f, tmp);
    return tmp;
}

/* ========================================================================= */
/* Float <-> Int Conversions                                                */
/* ========================================================================= */

int32_t snes_float_to_int_sf(const SnesSfParts *a)
{
    if (a->fp_class != SNES_FP_CLASS_NUMBER)
        return 0; /* Target behavior for NaN/Inf/Zero */

    if (a->exponent >= 31)
        return a->sign ? INT32_MIN : INT32_MAX;
    if (a->exponent < 0)
        return 0;

    uint32_t val = a->fraction >> (30 - a->exponent);
    return a->sign ? -(int32_t)val : (int32_t)val;
}

int32_t snes_float_to_int_df(const SnesDfParts *a)
{
    if (a->fp_class != SNES_FP_CLASS_NUMBER)
        return 0;

    if (a->exponent >= 31)
        return a->sign ? INT32_MIN : INT32_MAX;
    if (a->exponent < 0)
        return 0;

    uint64_t val = a->fraction >> (60 - a->exponent);
    return a->sign ? -(int32_t)val : (int32_t)val;
}

/* ========================================================================= */
/* Int -> Float Conversions                                                 */
/* ========================================================================= */

void snes_int_to_float_sf(int32_t val, SnesSfParts *out)
{
    if (val == 0) {
        out->fp_class = SNES_FP_CLASS_ZERO;
        out->sign = 0;
        out->exponent = 0;
        out->fraction = 0;
        return;
    }

    out->sign = val < 0;
    uint32_t abs_val = out->sign ? (uint32_t)(-val) : (uint32_t)val;

    int lz = __builtin_clz(abs_val) - 1;
    out->exponent = 30 - lz;
    out->fraction = abs_val << (lz + 1);
    out->fp_class = SNES_FP_CLASS_NUMBER;
}

void snes_int_to_float_df(int32_t val, SnesDfParts *out)
{
    if (val == 0) {
        out->fp_class = SNES_FP_CLASS_ZERO;
        out->sign = 0;
        out->exponent = 0;
        out->pad_0c = 0;
        out->fraction = 0;
        return;
    }

    out->sign = val < 0;
    uint32_t abs_val = out->sign ? (uint32_t)(-val) : (uint32_t)val;

    int lz = __builtin_clz(abs_val) - 1;
    out->exponent = 30 - lz;
    out->fraction = (uint64_t)abs_val << (lz + 31);
    out->fp_class = SNES_FP_CLASS_NUMBER;
    out->pad_0c = 0;
}