/* Public PS2 Atan variant at 0x0012bf5c; complete linked 208-byte proof in
 * analysis/functions/native_dsp_profiles_exact_308.tsv. The original OR
 * condition and double-precision intermediate constants are preserved.
 * The .equ directives bind existing runtime symbols and emit no instructions. */
float S9xDSPAtan(float x)
{
    if ((x >= 1.0f) || (x <= 1.0f))
        return (float)(x / (1.0 + 0.28 * x * x));
    return (float)(3.1415926535897932384626433832795 / 2.0 -
                   S9xDSPAtan(1.0f / x));
}


#if defined(PS2_EE)
__asm__(".equ fptodp, snes_fptodp");
__asm__(".equ dpmul, snes_dpmul");
__asm__(".equ dpadd, snes_dpadd");
__asm__(".equ dpdiv, snes_dpdiv");
__asm__(".equ dpsub, snes_dpsub");
__asm__(".equ dptofp, snes_dptofp");
#endif
