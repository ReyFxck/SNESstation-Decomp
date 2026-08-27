#ifndef SNESSTATION_EE_INTRINSICS_RECOVERED_H
#define SNESSTATION_EE_INTRINSICS_RECOVERED_H

/*
 * Source-local forms of operations that are instructions or inline SDK
 * wrappers in the target.  Keeping them inline prevents the buildable source
 * model from inventing external Stage-3E provider symbols.
 */

#include <stdint.h>

#if defined(__GNUC__)
#define SNES_EE_ALWAYS_INLINE static __inline__ __attribute__((__always_inline__))
#else
#define SNES_EE_ALWAYS_INLINE static inline
#endif

SNES_EE_ALWAYS_INLINE int snes_ee_isnan(double value)
{
    union { float f; uint32_t u; } bits;
    bits.f = (float)value;
    return (bits.u & UINT32_C(0x7fffffff)) > UINT32_C(0x7f800000);
}

SNES_EE_ALWAYS_INLINE int snes_ee_isinf(double value)
{
    union { float f; uint32_t u; } bits;
    bits.f = (float)value;
    return (bits.u & UINT32_C(0x7fffffff)) == UINT32_C(0x7f800000);
}

SNES_EE_ALWAYS_INLINE long snes_ee_lrintf(float value)
{
#if defined(__mips__)
    long result;
    __asm__ volatile(
        "cvt.w.s $f0,%1\n\tmfc1 %0,$f0"
        : "=r"(result) : "f"(value) : "$f0");
    return result;
#else
    return value < 0.0f ? (long)(value - 0.5f) : (long)(value + 0.5f);
#endif
}

SNES_EE_ALWAYS_INLINE int snes_ee_sqrt_trunc(float value)
{
#if defined(__mips__)
    int result;
    __asm__ volatile(
        "sqrt.s $f0,%1\n\ttrunc.w.s $f0,$f0\n\tmfc1 %0,$f0"
        : "=r"(result) : "f"(value) : "$f0");
    return result;
#else
    float estimate;
    unsigned iteration;
    if (value <= 0.0f)
        return 0;
    estimate = value > 1.0f ? value : 1.0f;
    for (iteration = 0; iteration < 8; ++iteration)
        estimate = 0.5f * (estimate + value / estimate);
    return (int)estimate;
#endif
}

SNES_EE_ALWAYS_INLINE void snes_ee_ei(void)
{
#if defined(__mips__)
    __asm__ volatile("ei" : : : "memory");
#endif
}

SNES_EE_ALWAYS_INLINE void snes_ee_syscall(void)
{
#if defined(__mips__)
    __asm__ volatile("syscall" : : : "memory");
#endif
}

SNES_EE_ALWAYS_INLINE void snes_ee_trap(void)
{
#if defined(__mips__)
    __asm__ volatile("break 7" : : : "memory");
#endif
}

SNES_EE_ALWAYS_INLINE int snes_ee_syscall3(
    int number, uint32_t a0_value, uint32_t a1_value, uint32_t a2_value)
{
#if defined(__mips__)
    register int v0 __asm__("$2");
    register int v1 __asm__("$3") = number;
    register uint32_t a0 __asm__("$4") = a0_value;
    register uint32_t a1 __asm__("$5") = a1_value;
    register uint32_t a2 __asm__("$6") = a2_value;
    __asm__ volatile(
        "syscall"
        : "=r"(v0), "+r"(v1), "+r"(a0), "+r"(a1), "+r"(a2)
        : : "memory");
    return v0;
#else
    (void)number;
    (void)a0_value;
    (void)a1_value;
    (void)a2_value;
    return 0;
#endif
}

SNES_EE_ALWAYS_INLINE void snes_ee_gs_put_imr(uint32_t value)
{
#if defined(__mips__)
    *(volatile uint64_t *)(uintptr_t)UINT32_C(0x12001010) = (uint64_t)value;
#else
    (void)value;
#endif
}

#undef SNES_EE_ALWAYS_INLINE

#endif
