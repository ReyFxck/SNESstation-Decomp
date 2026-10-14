#ifndef SNESSTATION_IOPHEAP_LEGACY_COMPAT_H
#define SNESSTATION_IOPHEAP_LEGACY_COMPAT_H

/*
 * Minimal ABI surface for ps2dev/ps2sdk@a80df908 ee/kernel/src/iopheap.c.
 * This is matching-only: canonical promotion happens only after the three
 * target members are proved against SNES Station.
 */
#include "../../include/sifrpc_legacy_compat.h"

#define E_LIB_API_INIT 0xd601
#define E_SIF_RPC_BIND 0xd612
#define E_SIF_RPC_CALL 0xd613

void SifInitRpc(int mode);
int SifBindRpc(SifRpcClientData_t *client, int sid, int mode);
int SifCallRpc(SifRpcClientData_t *client, int rpc_number, int mode,
               void *sendbuf, int ssize, void *recvbuf, int rsize,
               SifRpcEndFunc_t endfunc, void *efarg);

#endif
