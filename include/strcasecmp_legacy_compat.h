#ifndef SNESSTATION_PS2LIB_STRCASECMP_COMPAT_H
#define SNESSTATION_PS2LIB_STRCASECMP_COMPAT_H

/* The one enabled historical PS2LIB F_strcasecmp body uses tolower.
   Keep the original external call and the EE GCC 3.2.2 ABI. */
int tolower(int c);

#endif
