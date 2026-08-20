float snes_p28_0012bf5c(float x)
{
    if ((x >= 1.0f) || (x <= 1.0f))
        return (float)(x / (1.0 + 0.28 * x * x));
    return (float)(3.1415926535897932384626433832795 / 2.0 -
                   snes_p28_0012bf5c(1.0f / x));
}

extern float snes_p26_cos_table[2048];
extern float snes_p26_sin_table[2048];
extern float cosf(float value);
extern float sinf(float value);

void snes_p26_0012c02c(void)
{
    unsigned int i;

    for (i = 0; i < 2048; ++i) {
        snes_p26_cos_table[i] =
            cosf((float)(2.0 * 3.1415926535897932384626433832795 * i / 2048));
        snes_p26_sin_table[i] =
            sinf((float)(2.0 * 3.1415926535897932384626433832795 * i / 2048));
    }
}
