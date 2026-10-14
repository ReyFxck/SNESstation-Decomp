#ifndef SNESSTATION_PS2LIB_SBRK_COMPAT_H
#define SNESSTATION_PS2LIB_SBRK_COMPAT_H

/*
 * Minimal ABI surface for the 15-Apr-2004 PS2LIB sbrk.c source.
 * size_t is the historical 32-bit EE type. EndOfHeap is still carried by the
 * existing recovered syscall leaf until its own historical provider is promoted.
 * The historical ps2_sbrk export is bound to the target-address canonical name.
 */
typedef unsigned int size_t;

#if defined(SNESSTATION_HOST_SYNTAX)
#define asm __asm__
#endif

int DIntr(void);
int EIntr(void);
void *EndOfHeap_0019f5c0(void);

#define DI() DIntr()
#define EI() EIntr()
#define EndOfHeap EndOfHeap_0019f5c0
#define ps2_sbrk ps2_sbrk_0019f078

#endif
