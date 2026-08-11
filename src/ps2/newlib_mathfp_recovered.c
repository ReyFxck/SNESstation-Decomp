#include <errno.h>
#include <stdint.h>
#include <string.h>

/*
 * Newlib 1.10.0 mathfp corridor recovered from SNES Station v0.23.
 *
 * Target range:
 *   0x0019fddc  cosf
 *   0x001a0024  sinf
 *   0x001a0254  tanf
 *   0x001a045c  atanf (with atangentf non-atan2 path inlined)
 *   0x001a06a0  sqrtf EE leaf
 *   0x001a06b0  fabsf EE leaf
 *   0x001a06c0  numtestf
 *
 * The transcendental algorithms and constants independently line up with the
 * old Newlib mathfp/Cody-Waite family.  This file is a behavioural recovery of
 * the target, not a claim of compiler matching.
 *
 * Important target quirk: the program was built with the old EE long64 ABI.
 * numtestf's nominal 32-bit word is sign-extended in a 64-bit GPR before the
 * `>> 23` / `& 0x7f8` test.  Positive NaN/+Inf therefore fall through as NUM,
 * while negative NaN/-Inf can still reach the special-value branch.  The code
 * below models that observed machine-code behaviour deliberately.
 */

enum {
    MATHFP_ZERO = 0,
    MATHFP_INF  = 1,
    MATHFP_NAN  = 2,
    MATHFP_NUM  = 3
};

static const float k_half_pi = 1.570796326f;
static const float k_one_over_pi = 0.318309886f;
static const float k_two_over_pi = 0.6366197723f;
static const float k_pi = 3.14159265358979323846f;
static const float k_root3 = 1.732050807f;
static const float k_rooteps = 1.7263349182589107e-4f;

static uint32_t float_word(float x)
{
    uint32_t word;
    memcpy(&word, &x, sizeof(word));
    return word;
}

static float float_from_word(uint32_t word)
{
    float x;
    memcpy(&x, &word, sizeof(x));
    return x;
}

static float target_notanum(void)
{
    return float_from_word(UINT32_C(0xffd00000));
}

/* Target 0x001a06c0. */
int numtestf_001a06c0(float x)
{
    const int64_t sign_extended_word = (int64_t)(int32_t)float_word(x);
    const uint64_t shifted = (uint64_t)sign_extended_word >> 23;
    const uint32_t exp = (uint32_t)(shifted & UINT64_C(0x7f8));
    const uint32_t raw = (uint32_t)sign_extended_word;

    if (x == 0.0f)
        return MATHFP_ZERO;

    if (exp == UINT32_C(0x7f8)) {
        if (raw & UINT32_C(0x007fffff))
            return MATHFP_NAN;
        return MATHFP_INF;
    }

    return MATHFP_NUM;
}

/* Target 0x001a06b0: one `abs.s` in the return delay slot. */
float fabsf_001a06b0(float x)
{
    return __builtin_fabsf(x);
}

/* Target 0x001a06a0: one EE `sqrt.s` in the return delay slot. */
float sqrtf_001a06a0(float x)
{
    return __builtin_sqrtf(x);
}

static float sine_generator_target(float x, int cosine)
{
    static const float r[4] = {
        -0.1666665668f,
         0.008333025139f,
        -0.0001980741872f,
         0.000002601903036f
    };
    int sign;
    int n;
    float y;
    float xn;
    float result;

    switch (numtestf_001a06c0(x)) {
    case MATHFP_NAN:
        errno = EDOM;
        return x;
    case MATHFP_INF:
        errno = EDOM;
        return target_notanum();
    default:
        break;
    }

    if (cosine) {
        sign = 1;
        y = fabsf_001a06b0(x) + k_half_pi;
    } else if (x < 0.0f) {
        sign = -1;
        y = -x;
    } else {
        sign = 1;
        y = x;
    }

    if (y > 210828714.0f) {
        errno = ERANGE;
        return x;
    }

    if (y < 0.0f)
        n = (int)(y * k_one_over_pi - 0.5f);
    else
        n = (int)(y * k_one_over_pi + 0.5f);

    xn = (float)n;
    if (n & 1)
        sign = -sign;
    if (cosine)
        xn -= 0.5f;

    y = fabsf_001a06b0(x) - xn * k_pi;

    if (-k_rooteps < y && y < k_rooteps) {
        result = y;
    } else {
        const float g = y * y;
        const float poly = (((r[3] * g + r[2]) * g + r[1]) * g + r[0]) * g;
        result = y + y * poly;
    }

    return result * (float)sign;
}

/* Target 0x0019fddc.  The historical `sinef(x, 1)` body is inlined. */
float cosf_0019fddc(float x)
{
    return sine_generator_target(x, 1);
}

/* Target 0x001a0024.  The historical `sinef(x, 0)` body is inlined. */
float sinf_001a0024(float x)
{
    return sine_generator_target(x, 0);
}

/* Target 0x001a0254. */
float tanf_001a0254(float x)
{
    const float p0 = -0.0958017723f;
    const float q0 = -0.429135777f;
    const float q1 = 0.00971685835f;
    float y;
    float f;
    float g;
    float xn;
    float xnum;
    float xden;
    int n;

    switch (numtestf_001a06c0(x)) {
    case MATHFP_NAN:
        errno = EDOM;
        return x;
    case MATHFP_INF:
        errno = EDOM;
        return target_notanum();
    default:
        break;
    }

    y = fabsf_001a06b0(x);
    if (y > 105414357.0f) {
        errno = ERANGE;
        return y;
    }

    if (x < 0.0f)
        n = (int)(x * k_two_over_pi - 0.5f);
    else
        n = (int)(x * k_two_over_pi + 0.5f);

    xn = (float)n;
    f = x - xn * k_half_pi;

    if (-k_rooteps < f && f < k_rooteps) {
        xnum = f;
        xden = 1.0f;
    } else {
        g = f * f;
        xnum = f * (p0 * g) + f;
        xden = (q1 * g + q0) * g + 1.0f;
    }

    if (n & 1)
        return xden / -xnum;
    return xnum / xden;
}

/*
 * Target 0x001a045c.
 * The public atanf wrapper and the arctan-only branch of atangentf have been
 * folded into one target function by the old compiler/link setup.
 */
float atanf_001a045c(float x)
{
    static const float angle[4] = {
        0.0f,
        0.523598775f,
        1.570796326f,
        1.047197551f
    };
    const float q0 = 1.412500740f;
    const float p0 = -0.4708325141f;
    const float p1 = -0.05090958253f;
    float f;
    float g;
    float p;
    float q;
    float result;
    int n;

    switch (numtestf_001a06c0(x)) {
    case MATHFP_NAN:
        errno = EDOM;
        return x;
    case MATHFP_INF:
        /* Historical wrapper returns +pi/2 even for the negative-INF case. */
        return k_half_pi;
    case MATHFP_ZERO:
        return 0.0f;
    default:
        break;
    }

    f = fabsf_001a06b0(x);
    if (f > 1.0f) {
        f = 1.0f / f;
        n = 2;
    } else {
        n = 0;
    }

    if (f > (2.0f - k_root3)) {
        const float a = k_root3 - 1.0f;
        f = (((a * f - 0.5f) - 0.5f) + f) / (k_root3 + f);
        n++;
    }

    if (-k_rooteps < f && f < k_rooteps) {
        result = f;
    } else {
        g = f * f;
        p = (p1 * g + p0) * g;
        q = g + q0;
        result = f + f * (p / q);
    }

    if (n > 1)
        result = -result;
    result += angle[n];

    if (x < 0.0f)
        result = -result;

    return result;
}
