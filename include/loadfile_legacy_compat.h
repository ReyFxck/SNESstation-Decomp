#ifndef SNESSTATION_LOADFILE_LEGACY_COMPAT_H
#define SNESSTATION_LOADFILE_LEGACY_COMPAT_H

/*
 * Minimal ABI surface for the April 18 2004 PS2LIB EE loadfile.c lineage
 * recovered byte-exact for SNES Station v0.23.
 */
#include "sifrpc_legacy_compat.h"

#define E_LIB_API_INIT   0xd601
#define E_SIF_RPC_BIND   0xd612
#define E_SIF_RPC_CALL   0xd613

#define LF_PATH_MAX 252
#define LF_ARG_MAX  252

#define ALIGNED(x) __attribute__((aligned((x))))

typedef struct {
    u32 epc;
    u32 gp;
} t_ExecData;

void SifInitRpc(int mode);
int SifBindRpc(SifRpcClientData_t *client, int sid, int mode);
int SifCallRpc(SifRpcClientData_t *client, int rpc_number, int mode,
               void *sendbuf, int ssize, void *recvbuf, int rsize,
               SifRpcEndFunc_t endfunc, void *efarg);

int SifLoadFileInit(void);

#endif
