#ifndef SNESSTATION_GSLIB_MATCH_STDINT_H
#define SNESSTATION_GSLIB_MATCH_STDINT_H

/*
 * Matching-only ABI bridge for the isolated stage-one GCC build.
 *
 * The stage-one compiler has no Newlib header installation, while the recovered
 * GSLIB source uses only uint32_t and uintptr_t from <stdint.h>.  On the EE
 * target both are 32-bit unsigned integer types.  This header supplies names,
 * not implementation code, and is intentionally local to the GSLIB probe.
 */
typedef unsigned int uint32_t;
typedef unsigned int uintptr_t;

#endif
