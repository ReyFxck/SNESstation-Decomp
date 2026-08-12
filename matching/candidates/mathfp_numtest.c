/*
 * Target 0x001a06c0, compiled separately from its non-prototyped callers.
 * The target body receives x in $f12 while callers also perform the default
 * float-to-double promotion.  Keeping the two translation units separate is
 * therefore part of the matching experiment, not a cosmetic split.
 */

#define MATHFP_NUM 3
#define MATHFP_NAN 2
#define MATHFP_INF 1

int numtestf_candidate(float x)
{
    union {
        float value;
        unsigned long word;
    } shape;
    unsigned long wx;
    unsigned long exp;

    if (x == 0.0f)
        return 0;

    shape.word = 0;
    shape.value = x;
    wx = shape.word;
    exp = (wx >> 23) & 0x7f8UL;

    if (exp == 0x7f8UL) {
        if (wx & 0x7fffffUL)
            return MATHFP_NAN;
        return MATHFP_INF;
    }

    return MATHFP_NUM;
}
