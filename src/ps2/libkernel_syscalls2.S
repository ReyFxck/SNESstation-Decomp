/*
 * Canonical assembly translation of libkernel syscall wrappers at 0x0019CE60.
 * Eleven syscall wrappers, each 16 bytes: li v0, num; syscall; jr ra; nop.
 *
 * Recovered from public listing evidence (analysis/functions/libkernel_syscalls2_0019ce60.asm).
 * Byte-identical with original SNES Station v0.23 WIP executable.
 */

#include <sys/asm.h>

/* Macro for 16-byte syscall wrapper: load syscall number, syscall, jr ra, nop */
.macro LIBCALL name:req num:req
    .type   \name, @function
    .global \name
    .ent    \name
\name:
    .frame  $sp, 0, $ra
    .mask   0x00000000, 0
    li      $2, \num
    syscall
    jr      $ra
    nop
    .end    \name
    .size   \name, .-\name
.endm

/* Syscall 0xCC (-52 / 0xFFFFFFCC) */
LIBCALL syscall_0xCC, 0xFFFFFFCC

/* Syscall 0x40 (64) */
LIBCALL syscall_0x40, 0x40

/* Syscall 0x41 (65) */
LIBCALL syscall_0x41, 0x41

/* Syscall 0xBD (-67 / 0xFFFFFFBD) */
LIBCALL syscall_0xBD, 0xFFFFFFBD

/* Syscall 0x44 (68) */
LIBCALL syscall_0x44, 0x44

/* Syscall 0x64 (100) */
LIBCALL syscall_0x64, 0x64

/* Syscall 0x98 (-104 / 0xFFFFFF98) */
LIBCALL syscall_0x98, 0xFFFFFF98

/* Syscall 0x76 (118) */
LIBCALL syscall_0x76, 0x76

/* Syscall 0x77 (119) */
LIBCALL syscall_0x77, 0x77

/* Syscall 0x79 (121) */
LIBCALL syscall_0x79, 0x79

/* Syscall 0x7A (122) */
LIBCALL syscall_0x7A, 0x7A