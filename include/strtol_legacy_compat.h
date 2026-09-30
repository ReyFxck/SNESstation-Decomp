#ifndef SNESSTATION_PS2LIB_STRTOL_COMPAT_H
#define SNESSTATION_PS2LIB_STRTOL_COMPAT_H
/* PS2LIB source has one selected F_strtol body; its historical
   signed-long limits, ctype calls and target errno storage remain explicit. */
#include <limits.h>
extern int ps2lib_errno_00425a70;
#define errno ps2lib_errno_00425a70
#define E_LIB_MATH_RANGE 34
int isspace(int);
int isdigit(int);
int isalpha(int);
int isupper(int);
#endif
