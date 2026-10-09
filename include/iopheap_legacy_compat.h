#ifndef SNESSTATION_IOPHEAP_LEGACY_COMPAT_H
#define SNESSTATION_IOPHEAP_LEGACY_COMPAT_H

/*
 * Minimal ABI surface for the April 18 2004 PS2LIB EE iopheap.c lineage
 * recovered for SNES Station v0.23.
 */
#include "sifrpc_legacy_compat.h"

#define E_LIB_API_INIT 0xd601
#define E_SIF_RPC_BIND 0xd612
#define E_SIF_RPC_CALL 0xd613

void SifInitRpc(int mode);
int SifBindRpc(SifRpcClientData_t *client, int sid, int mode);
int SifCallRpc(SifRpcClientData_t *client, int rpc_number, int mode,
               void *sendbuf, int ssize, void *recvbuf, int rsize,
               SifRpcEndFunc_t endfunc, void *efarg);
int SifInitIopHeap(void);

#endif
