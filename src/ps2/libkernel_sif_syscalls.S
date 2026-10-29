/*
 * Canonical assembly for libkernel SIF syscall wrappers at 0x0019F5D0.
 * Three syscall wrappers, each 16 bytes: load syscall number, syscall, jr ra, nop.
 * Matches original SNES Station v0.23 WIP executable byte-for-byte.
 */

	.text
	.p2align 3

#define LIBCALL(name, number) \
	.set	push;		\
	.set	noreorder;	\
	.text;			\
	.align 4;		\
	.globl	name;		\
	.type	name,@function;	\
	.ent	name,0;		\
name:	li	$2, number;	\
	syscall;		\
	jr	$31;		\
	nop;			\
	.end	name;		\
	.size	name,.-name;	\
	.set	pop;		\

/* Syscall 0x6B (107) - SifStopDma */
LIBCALL(syscall_0x6B, 0x6B)

/* Syscall 0x89 (-119 / 0xFFFFFF89) - uses full 32-bit load */
LIBCALL(syscall_0x89, 0xFFFFFF89)

/* Syscall 0x78 (120) */
LIBCALL(syscall_0x78, 0x78)