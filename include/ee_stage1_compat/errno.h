#ifndef SNESSTATION_EE_STAGE1_ERRNO_H
#define SNESSTATION_EE_STAGE1_ERRNO_H

/* The target owns one errno word at 0x00425a70. */
extern int ps2lib_errno_00425a70;
#define errno ps2lib_errno_00425a70

#define EDOM   33
#define ERANGE 34

#endif
