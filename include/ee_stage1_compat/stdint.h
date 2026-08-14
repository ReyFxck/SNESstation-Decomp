#ifndef SNESSTATION_EE_STAGE1_STDINT_H
#define SNESSTATION_EE_STAGE1_STDINT_H

/*
 * Scan-only C99 integer compatibility for the header-less EE GCC 3.2.2
 * bootstrap. Do not add this directory to EE_CFLAGS used by matching.
 *
 * Project target contract: char=8, short=16, int=32, pointer=32.
 */
typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef signed short       int16_t;
typedef unsigned short     uint16_t;
typedef signed int         int32_t;
typedef unsigned int       uint32_t;
typedef signed long long   int64_t;
typedef unsigned long long uint64_t;

typedef signed int         intptr_t;
typedef unsigned int       uintptr_t;

#ifndef __SIZEOF_INT128__
#define __SIZEOF_INT128__ 16
#endif

#define INT8_C(v)   v
#define UINT8_C(v)  v##U
#define INT16_C(v)  v
#define UINT16_C(v) v##U
#define INT32_C(v)  v
#define UINT32_C(v) v##U
#define INT64_C(v)  v##LL
#define UINT64_C(v) v##ULL

#define INT8_MIN    (-128)
#define INT8_MAX    127
#define UINT8_MAX   255U
#define INT16_MIN   (-32767 - 1)
#define INT16_MAX   32767
#define UINT16_MAX  65535U
#define INT32_MIN   (-2147483647 - 1)
#define INT32_MAX   2147483647
#define UINT32_MAX  4294967295U
#define INT64_MIN   (-9223372036854775807LL - 1LL)
#define INT64_MAX   9223372036854775807LL
#define UINT64_MAX  18446744073709551615ULL
#define UINTPTR_MAX 4294967295U

/* GCC 3.2.2 predates C11 _Static_assert. Host syntax still validates it. */
#ifndef _Static_assert
#define _Static_assert(condition, message)
#endif

#endif
