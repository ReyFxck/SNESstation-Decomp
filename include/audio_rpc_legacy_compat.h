#ifndef SNESSTATION_AUDIO_RPC_LEGACY_COMPAT_H
#define SNESSTATION_AUDIO_RPC_LEGACY_COMPAT_H

/* PGEN 403f1710 / PS2DEV bac0006c EE RPC client ABI. The shared PS2LIB
 * header preserves the 40-byte client and server pointer at offset 36.
 * Function prototypes agree with the canonical SIF/IOP-heap providers.
 */
#include "sifrpc_legacy_compat.h"
#include <stdarg.h>
#include <stdio.h>

/* Historical PS2LIB/Newlib uses a 32-bit byte count. GCC's header-less
 * size_t follows -mlong64; using it here disables the EE builtin block copy
 * and fails both complete target log bodies.
 */
void *memcpy(void *, const void *, unsigned int);

typedef unsigned int u128 __attribute__((mode(TI)));

int SifBindRpc(SifRpcClientData_t *, int, int);
int SifCallRpc(SifRpcClientData_t *, int, int, void *, int, void *, int,
               SifRpcEndFunc_t, void *);
void FlushCache(int);
int SifDmaStat(u32);
void *SifAllocIopHeap(int);
int SifFreeIopHeap(void *);

#endif
