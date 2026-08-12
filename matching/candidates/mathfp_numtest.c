/*
 * Readable behavioral model for target 0x001a06c0.  The byte matcher builds
 * mathfp_numtest.S because the surviving BETA 3 backend does not reproduce the
 * target's older instruction selection from C.
 *
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
        unsigned int word;
    } shape;
    long sign_extended_word;
    unsigned long wx;
    unsigned long exp;

    if (x == 0.0f)
        return 0;

    shape.value = x;
    sign_extended_word = (long)(int)shape.word;
    wx = (unsigned long)sign_extended_word;
    exp = (wx >> 23) & 0x7f8UL;

    if (exp == 0x7f8UL) {
        if (wx & 0x7fffffUL)
            return MATHFP_NAN;
        else
            return MATHFP_INF;
    }
    else
        return MATHFP_NUM;
}
