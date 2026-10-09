#ifndef SNESSTATION_PS2LIB_STRNCASECMP_COMPAT_H
#define SNESSTATION_PS2LIB_STRNCASECMP_COMPAT_H

#include <stddef.h>

/* Historical PS2LIB F_strncasecmp calls the adjacent PS2LIB ctype helper. */
int tolower(int c);

#endif
