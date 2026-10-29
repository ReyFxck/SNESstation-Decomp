/*
 * SNES Station v0.23 SNES9X - Soft Float Conversions
 * Canonical source from progress13_runtime_more_recovered.c @ 0x001a7580
 */

/* 0x001a7580 -- libgcc signed DI -> double conversion helper (__floatdidf). */
double snes9x_floatdidf(int64_t value)
{
    /* The target builds high*2^32 plus an unsigned-corrected low word. */
    int32_t high = (int32_t)(value >> 32);
    int32_t low_signed = (int32_t)(uint32_t)value;
    double result = (double)high * 65536.0 * 65536.0;
    double low = (double)low_signed;
    if (low_signed < 0)
        low += 4294967296.0;
    return result + low;
}