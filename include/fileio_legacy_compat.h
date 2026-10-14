#ifndef SNESSTATION_FILEIO_LEGACY_COMPAT_H
#define SNESSTATION_FILEIO_LEGACY_COMPAT_H

/*
 * Minimal ABI surface for the April 18 2004 PS2LIB EE fileio.c lineage used
 * by SNES Station v0.23.
 */
#include "sifrpc_legacy_compat.h"

#define FIO_PATH_MAX 256

#define FIO_WAIT       0
#define FIO_NOWAIT     1
#define FIO_COMPLETE   1
#define FIO_INCOMPLETE 0

#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif

#ifndef E_LIB_UNSUPPORTED
#define E_LIB_UNSUPPORTED 0xd605
#endif

#define ALIGNED(x) __attribute__((aligned((x))))
#define IS_UNCACHED_SEG(x) (((u32)(x)) & 0x20000000)

void SifInitRpc(int mode);
int SifBindRpc(SifRpcClientData_t *client, int rpc_number, int mode);
int SifCallRpc(SifRpcClientData_t *client, int rpc_number, int mode,
               void *sendbuf, int ssize, void *recvbuf, int rsize,
               SifRpcEndFunc_t endfunc, void *efarg);

int SignalSema(int sema_id);
int PollSema(int sema_id);

#endif
