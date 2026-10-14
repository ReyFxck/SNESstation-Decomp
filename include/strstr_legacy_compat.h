#ifndef SNESSTATION_PS2LIB_STRSTR_COMPAT_H
#define SNESSTATION_PS2LIB_STRSTR_COMPAT_H

/*
 * The original 2004 PS2LIB string.h declares strlen as unsigned int,
 * not the later standard size_t. Keep that ABI while compiling the
 * historical F_strstr body on EE GCC 3.2.2. Leave host builds using
 * their native libc declarations.
 */
#include <stddef.h>
#if defined(__mips__) && defined(_EE)
unsigned int strlen(const char *);
int strncmp(const char *, const char *, size_t);
char *strstr(const char *, const char *);
#else
#include <string.h>
#endif

#endif
