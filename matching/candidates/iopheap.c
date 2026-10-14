/*
 * Historical iopheap.c candidate for SNES Station v0.23.
 *
 * Source lineage:
 *   ps2dev/ps2sdk@a80df908256955382f102278400b5d713552dbce
 *   ee/kernel/src/iopheap.c
 *
 * Target members:
 *   0x0019d63c SifAllocIopHeap
 *   0x0019d6b8 SifFreeIopHeap
 *   0x0019f9e8 SifInitIopHeap
 */

#include "iopheap_legacy_compat.h"

#define IH_C_BOUND 0x0001

extern SifRpcClientData_t _ih_cd;
extern int _ih_caps;

#ifdef F_SifInitIopHeap
SifRpcClientData_t _ih_cd;
int _ih_caps = 0;

int SifInitIopHeap()
{
    int res;

    if (_ih_caps)
        return 0;

    SifInitRpc(0);

    while ((res = SifBindRpc(&_ih_cd, 0x80000003, 0)) >= 0 && !_ih_cd.server)
        nopdelay();

    if (res < 0)
        return -E_SIF_RPC_BIND;

    _ih_caps |= IH_C_BOUND;

    return 0;
}
#endif

#ifdef F_SifExitIopHeap
void SifExitIopHeap()
{
    _ih_caps = 0;
}
#endif

#ifdef F_SifAllocIopHeap
void * SifAllocIopHeap(int size)
{
    union { int size; u32 addr; } arg;

    if (!_ih_caps && SifInitIopHeap() < 0)
        return NULL;

    arg.size = size;

    if (SifCallRpc(&_ih_cd, 1, 0, &arg, 4, &arg, 4, NULL, NULL) < 0)
        return NULL;

    return (void *)arg.addr;
}
#endif

#ifdef F_SifFreeIopHeap
int SifFreeIopHeap(void *addr)
{
    union { void *addr; int result; } arg;

    if (!_ih_caps && SifInitIopHeap() < 0)
        return -E_LIB_API_INIT;

    arg.addr = addr;

    if (SifCallRpc(&_ih_cd, 2, 0, &arg, 4, &arg, 4, NULL, NULL) < 0)
        return -E_SIF_RPC_CALL;

    return arg.result;
}
#endif
