/*
 * Canonical assembly translation of libkernel syscall wrappers at 0x0019FCD0.
 * Five syscall wrappers, each 16 bytes: li v0, num; syscall; jr ra; nop.
 *
 * Recovered from public listing evidence (analysis/functions/libkernel_syscalls_0019fcd0.asm).
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

/* Syscall 0x16 (22) */
LIBCALL syscall_0x16, 0x16

/* Syscall 0x17 (23) */
LIBCALL syscall_0x17, 0x17

/* Syscall 0x42 (66) */
LIBCALL syscall_0x42, 0x42

/* Syscall 0x45 (69) */
LIBCALL syscall_0x45, 0x45

/* Syscall 0xFFFFFF88 (-120) */
LIBCALL syscall_0xFFFFFF88, 0xFFFFFF88