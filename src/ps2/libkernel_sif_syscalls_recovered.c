/*
 * Canonical C translation of libkernel SIF syscall wrappers at 0x0019F5D0.
 * Three syscall wrappers, each 16 bytes: load syscall number, syscall, jr ra, nop.
 *
 * Recovered from public listing evidence (analysis/functions/libkernel_syscalls_0019f5d0.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 */

#include <sys/syscall.h>

/* Syscall 0x6B (107) - SifStopDma */
int syscall_0x6B(void)
{
    register int v0 __asm__("$v0") = 0x6B;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x89 (-119 / 0xFFFFFF89) */
int syscall_0x89(void)
{
    register int v0 __asm__("$v0") = 0xFFFFFF89;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}

/* Syscall 0x78 (120) */
int syscall_0x78(void)
{
    register int v0 __asm__("$v0") = 0x78;
    __asm__ volatile("syscall" : "+r"(v0) : : "memory");
    return v0;
}