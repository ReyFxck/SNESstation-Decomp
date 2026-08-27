/*
 * Behavioral reconstruction of selected GCC 3.2.2-b1 EE libgcc helpers
 * embedded in SNES Station v0.23.
 *
 * This file is deliberately an independently written research model rather
 * than a verbatim copy of GCC's runtime sources.  The target stores DFmode
 * values as raw IEEE-754 binary64 words because the R5900 has no native
 * double-precision arithmetic.
 *
 * Matching status is intentionally NOT claimed here.  The historical
 * archive/object boundaries are used as provenance evidence only.
 */

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "ee_intrinsics_recovered.h"

#define isnan(value) snes_ee_isnan(value)
#define isinf(value) snes_ee_isinf(value)

static uint64_t double_to_raw(double value)
{
    uint64_t raw;
    memcpy(&raw, &value, sizeof(raw));
    return raw;
}

static double raw_to_double(uint64_t raw)
{
    double value;
    memcpy(&value, &raw, sizeof(value));
    return value;
}

/* 0x001a1b20 — _muldi3.o, exact target object size 0x78. */
uint64_t snes___muldi3(uint64_t a, uint64_t b)
{
    const uint64_t low = (uint64_t)(uint32_t)a * (uint32_t)b;
    const uint64_t cross =
        (uint64_t)(uint32_t)a * (uint32_t)(b >> 32) +
        (uint64_t)(uint32_t)(a >> 32) * (uint32_t)b;
    return low + (cross << 32);
}

/* Common unsigned 64-bit division model used by the duplicated archive bodies. */
static uint64_t udivmod64(uint64_t numerator, uint64_t denominator, uint64_t *remainder)
{
    uint64_t q = 0;
    uint64_t r = 0;
    unsigned i;

    if (denominator == 0) {
        /* The original libgcc intentionally reaches a divide-by-zero trap. */
        if (remainder != NULL)
            *remainder = numerator;
        return UINT64_MAX;
    }

    for (i = 0; i < 64; ++i) {
        const unsigned bit = 63U - i;
        const uint64_t carry = (numerator >> bit) & 1U;
        const uint64_t high = r >> 63;
        r = (r << 1) | carry;
        if (high || r >= denominator) {
            r -= denominator;
            q |= UINT64_C(1) << bit;
        }
    }

    if (remainder != NULL)
        *remainder = r;
    return q;
}

/* 0x001a1db8 — signed 64-bit division entry. */
int64_t snes___divdi3(int64_t numerator, int64_t denominator)
{
    const int neg = (numerator < 0) ^ (denominator < 0);
    const uint64_t un = numerator < 0 ? (uint64_t)(-(numerator + 1)) + 1U : (uint64_t)numerator;
    const uint64_t ud = denominator < 0 ? (uint64_t)(-(denominator + 1)) + 1U : (uint64_t)denominator;
    const uint64_t q = udivmod64(un, ud, NULL);
    return neg ? -(int64_t)q : (int64_t)q;
}

/* 0x001a25b0 — _udivdi3.o public wrapper. */
uint64_t snes___udivdi3(uint64_t numerator, uint64_t denominator)
{
    return udivmod64(numerator, denominator, NULL);
}

/* 0x001a2c78 — _umoddi3.o public wrapper. */
uint64_t snes___umoddi3(uint64_t numerator, uint64_t denominator)
{
    uint64_t remainder;
    (void)udivmod64(numerator, denominator, &remainder);
    return remainder;
}

/* 0x001a1b98 — signed DI -> SF conversion. */
float snes___floatdisf(int64_t value)
{
    return (float)value;
}

/* 0x001a1c98 — unsigned conversion from the target's raw DFmode word. */
uint64_t snes___fixunsdfdi(uint64_t raw)
{
    const double value = raw_to_double(raw);
    if (!(value > 0.0))
        return 0;
    if (value >= 18446744073709551615.0)
        return UINT64_MAX;
    return (uint64_t)value;
}

/* 0x001a3340 — SF -> DFmode, returned as the target's raw 64-bit word. */
uint64_t snes_fptodp(float value)
{
    return double_to_raw((double)value);
}

/* 0x001a35b0 / 0x001a3610 — public goFast-named DF add/sub wrappers. */
uint64_t snes_dpadd(uint64_t a, uint64_t b)
{
    return double_to_raw(raw_to_double(a) + raw_to_double(b));
}

uint64_t snes_dpsub(uint64_t a, uint64_t b)
{
    return double_to_raw(raw_to_double(a) - raw_to_double(b));
}

/* 0x001a3680 / 0x001a3950 — public DF multiply/divide wrappers. */
uint64_t snes_dpmul(uint64_t a, uint64_t b)
{
    return double_to_raw(raw_to_double(a) * raw_to_double(b));
}

uint64_t snes_dpdiv(uint64_t a, uint64_t b)
{
    return double_to_raw(raw_to_double(a) / raw_to_double(b));
}

/* 0x001a3ad8 — normal-number comparison model for dpcmp. */
int snes_dpcmp(uint64_t a, uint64_t b)
{
    const double da = raw_to_double(a);
    const double db = raw_to_double(b);
    if (isnan(da) || isnan(db))
        return 1; /* unordered; callers apply relation-specific policy */
    if (da < db)
        return -1;
    if (da > db)
        return 1;
    return 0;
}

/* 0x001a3b30 — signed SI -> raw DFmode. */
uint64_t snes_litodp(int32_t value)
{
    return double_to_raw((double)value);
}

/* 0x001a3bf0 — raw DFmode -> signed SI. */
int32_t snes_dptoli(uint64_t raw)
{
    const double value = raw_to_double(raw);
    if (isnan(value))
        return 0;
    if (value >= (double)INT32_MAX)
        return INT32_MAX;
    if (value <= (double)INT32_MIN)
        return INT32_MIN;
    return (int32_t)value;
}

/* 0x001a3cc0 — raw DFmode -> SF. */
float snes_dptofp(uint64_t raw)
{
    return (float)raw_to_double(raw);
}

/* 0x001a3d18 — raw DFmode -> unsigned 32-bit integer (goFast dptoul). */
uint32_t snes_dptoul(uint64_t raw)
{
    const double value = raw_to_double(raw);
    if (isnan(value) || value <= 0.0)
        return 0;
    if (isinf(value) || value >= 4294967295.0)
        return UINT32_MAX;
    return (uint32_t)value;
}
