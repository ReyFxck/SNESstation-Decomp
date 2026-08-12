/*
 * Byte-matching candidates for the Newlib 1.10.0 mathfp corridor.
 *
 * This is intentionally separate from src/ps2/newlib_mathfp_recovered.c.
 * The src/ version is a portable behavioural model; this file preserves the
 * historical expression types and evaluation order needed by EE GCC 3.2.2.
 * No target C library headers are required, so the isolated stage-one compiler
 * can compile the experiment before Newlib itself has been installed.
 */

#define MATHFP_NUM 3
#define MATHFP_NAN 2
#define MATHFP_INF 1

#define MATHFP_EDOM 33
#define MATHFP_ERANGE 34

#define MATHFP_PI 3.14159265358979323846
#define MATHFP_PI_OVER_TWO 1.57079632679489661923132

typedef const union {
    unsigned long l;
    float f;
} mathfp_ufloat;

extern int errno;
extern mathfp_ufloat z_notanum_f;
extern float z_rooteps_f;

/*
 * Intentionally non-prototyped at these historical call sites.  Their
 * default float-to-double promotion matches the target.  The separately
 * compiled definition accepts float, reproducing the observed ABI mismatch.
 */
extern int numtestf_candidate();

/* Target 0x001a06a0: `jr ra; sqrt.s f0,f12` plus no function body. */
float sqrtf_candidate(float x)
{
    float result;

    __asm__("sqrt.s %0,%1" : "=f"(result) : "f"(x));
    return result;
}

/* Target 0x001a06b0: `jr ra; abs.s f0,f12` plus no function body. */
float fabsf_candidate(float x)
{
    return __builtin_fabsf(x);
}

static const float sine_half_pi = 1.570796326;
static const float sine_one_over_pi = 0.318309886;
static const float sine_r[] = {
    -0.1666665668,
     0.8333025139e-02,
    -0.1980741872e-03,
     0.2601903036e-5
};

static __inline__ __attribute__((always_inline))
float sine_generator_candidate(float x, int cosine)
{
    int sign;
    int n;
    float y;
    float xn;
    float g;
    float poly;
    float result;
    float maximum = 210828714.0;

    switch (numtestf_candidate(x)) {
    case MATHFP_NAN:
        errno = MATHFP_EDOM;
        return x;
    case MATHFP_INF:
        errno = MATHFP_EDOM;
        return z_notanum_f.f;
    }

    if (cosine) {
        y = __builtin_fabsf(x) + sine_half_pi;
        __asm__ __volatile__("" : "+f"(y));
        sign = 1;
    } else {
        if (x < 0.0) {
            sign = -1;
            y = -x;
        } else {
            sign = 1;
            y = x;
        }
    }

    if (y > maximum) {
        errno = MATHFP_ERANGE;
        return x;
    }

    if (y < 0.0)
        n = (int)(y * sine_one_over_pi - 0.5);
    else
        n = (int)(y * sine_one_over_pi + 0.5);
    if (n & 1)
        sign = -sign;
    if (cosine)
        xn = (float)n - 0.5;
    else
        xn = (float)n;

    y = __builtin_fabsf(x) - xn * MATHFP_PI;

    if (-z_rooteps_f < y && y < z_rooteps_f) {
        result = y;
    } else {
        g = y * y;
        poly = (((sine_r[3] * g + sine_r[2]) * g + sine_r[1]) * g
                + sine_r[0]) * g;
        result = y + y * poly;
    }

    result *= sign;
    return result;
}

float cosf_candidate(float x)
{
    return sine_generator_candidate(x, 1);
}

float sinf_candidate(float x)
{
    return sine_generator_candidate(x, 0);
}

float tanf_candidate(float x)
{
    static const float two_over_pi = 0.6366197723;
    static const float p[] = { -0.958017723e-1 };
    static const float q[] = { -0.429135777, 0.971685835e-2 };
    float y;
    float f;
    float g;
    float xnum;
    float xden;
    float result;
    int n;

    switch (numtestf_candidate(x)) {
    case MATHFP_NAN:
        errno = MATHFP_EDOM;
        return x;
    case MATHFP_INF:
        errno = MATHFP_EDOM;
        return z_notanum_f.f;
    default:
        break;
    }

    y = __builtin_fabsf(x);
    if (y > 105414357.0) {
        errno = MATHFP_ERANGE;
        return y;
    }

    if (x < 0.0)
        n = (int)(x * two_over_pi - 0.5);
    else
        n = (int)(x * two_over_pi + 0.5);
    f = x - n * MATHFP_PI_OVER_TWO;

    if (-z_rooteps_f < f && f < z_rooteps_f) {
        xnum = f;
        xden = 1.0;
    } else {
        g = f * f;
        xnum = f * (p[0] * g) + f;
        xden = (q[1] * g + q[0]) * g + 1.0;
    }

    if (n & 1) {
        xnum = -xnum;
        result = xden / xnum;
    } else {
        result = xnum / xden;
    }

    return result;
}

float atanf_candidate(float x)
{
    static const float root3 = 1.732050807;
    static const float angle[] = {
        0.0, 0.523598775, 1.570796326, 1.047197551
    };
    static const float q[] = { 0.1412500740e+1 };
    static const float p[] = { -0.4708325141, -0.5090958253e-1 };
    float f;
    float g;
    float ratio;
    float poly;
    float denominator;
    float adjustment;
    float result;
    int n;

    switch (numtestf_candidate(x)) {
    case MATHFP_NAN:
        errno = MATHFP_EDOM;
        return x;
    case MATHFP_INF:
        return MATHFP_PI_OVER_TWO;
    case 0:
        return 0.0;
    default:
        break;
    }

    f = __builtin_fabsf(x);
    if (f > 1.0) {
        f = 1.0 / f;
        n = 2;
    } else {
        n = 0;
    }

    if (f > (2.0 - root3)) {
        adjustment = root3 - 1.0;
        f = (((adjustment * f - 0.5) - 0.5) + f) / (root3 + f);
        n++;
    }

    if (-z_rooteps_f < f && f < z_rooteps_f) {
        result = f;
    } else {
        g = f * f;
        poly = (p[1] * g + p[0]) * g;
        denominator = g + q[0];
        ratio = poly / denominator;
        result = f + f * ratio;
    }

    if (n > 1)
        result = -result;
    result += angle[n];

    if (x < 0.0)
        result = -result;

    return result;
}
