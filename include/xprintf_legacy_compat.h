#ifndef SNESSTATION_XPRINTF_LEGACY_COMPAT_H
#define SNESSTATION_XPRINTF_LEGACY_COMPAT_H

/*
 * Minimal ABI surface for the April 15 2004 PS2LIB EE xprintf.c lineage.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

#define PS2LIB_STR_MAX 0x1000

int fioWrite(int fd, void *buffer, int size);

#endif
