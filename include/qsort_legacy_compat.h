#ifndef SNESSTATION_NEWLIB_QSORT_COMPAT_H
#define SNESSTATION_NEWLIB_QSORT_COMPAT_H

/*
 * Newlib 1.10 compatibility surface used by the historical qsort.c object.
 * The target used the PS2 toolchain profile where size_t was 32-bit even
 * though the surrounding application build selected -mlong64.
 */
#define _PARAMS(parameters) parameters
#define _DEFUN(name, arglist, args) name(args)
#define _DEFUN_VOID(name) name(void)
#define _AND ,
#define size_t int

/* Keep the repository's existing address-qualified canonical symbol. */
#define qsort snes_qsort_001080cc

#endif
