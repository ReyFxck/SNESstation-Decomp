#ifndef SNESSTATION_ALLOC_LEGACY_COMPAT_H
#define SNESSTATION_ALLOC_LEGACY_COMPAT_H

/*
 * Minimal ABI surface for the April 15 2004 PS2LIB EE alloc.c lineage used
 * by SNES Station v0.23.
 */
#include <stddef.h>

typedef unsigned int u32;

void *ps2_sbrk(int increment);
void *memcpy(void *dest, const void *src, size_t size);
void *memset(void *dest, int value, size_t size);

void *malloc(size_t size);
void free(void *ptr);

#endif
