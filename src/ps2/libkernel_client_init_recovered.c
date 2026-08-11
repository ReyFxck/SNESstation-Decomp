/*
 * Recovered initialization helpers for libkernel RPC clients used by
 * SNES Station v0.23. These are outside the contiguous call-site wrappers
 * but are directly reached by fio/loadfile/iopheap paths.
 */
#include <stdint.h>
#include <string.h>

#include "../../include/ps2_libkernel_recovered.h"

extern void SifInitRpc_0019cc0c(int);
extern int SifBindRpc_0019c688(void *, int, int);
extern int CreateSema(const ee_sema32 *);
extern int iSignalSema(int);

SifRpcClientData32 fio_cd_recovered;
SifRpcClientData32 lf_cd_recovered;
SifRpcClientData32 ih_cd_recovered;
int fio_init_recovered;
int fio_completion_sema_recovered = -1;
int fio_block_mode_recovered;
static int lf_init_recovered;
static int ih_caps_recovered;

static void bind_delay_recovered(void)
{
    volatile int i;
    for (i = 0x100000; i >= 0; --i) {
    }
}

/* 0x0019f600 */
int fioInit_0019f600(void)
{
    ee_sema32 sema;
    int res;
    if (fio_init_recovered)
        return 0;

    SifInitRpc_0019cc0c(0);
    do {
        res = SifBindRpc_0019c688(&fio_cd_recovered, (int)0x80000001u, 0);
        if (res < 0)
            return res;
        if (fio_cd_recovered.server == 0)
            bind_delay_recovered();
    } while (fio_cd_recovered.server == 0);

    /* Target writes only offsets +0x04, +0x08 and +0x14 before CreateSema. */
    sema.max_count = 1;
    sema.init_count = 1;
    sema.option = 0;
    fio_completion_sema_recovered = CreateSema(&sema);
    if (fio_completion_sema_recovered < 0)
        return -0xd602;

    fio_init_recovered = 1;
    fio_block_mode_recovered = 0;
    return 0;
}

/* 0x0019f6e8 */
void fio_intr_0019f6e8(void *unused)
{
    (void)unused;
    (void)iSignalSema(fio_completion_sema_recovered);
}

/* 0x0019fd20 */
int SifLoadFileInit_0019fd20(void)
{
    int res;
    if (lf_init_recovered)
        return 0;
    SifInitRpc_0019cc0c(0);
    do {
        res = SifBindRpc_0019c688(&lf_cd_recovered, (int)0x80000006u, 0);
        if (res < 0)
            return -0xd612;
        if (lf_cd_recovered.server == 0)
            bind_delay_recovered();
    } while (lf_cd_recovered.server == 0);
    lf_init_recovered = 1;
    return 0;
}

/* 0x0019f9e8 */
int SifInitIopHeap_0019f9e8(void)
{
    int res;
    if (ih_caps_recovered)
        return 0;
    SifInitRpc_0019cc0c(0);
    do {
        res = SifBindRpc_0019c688(&ih_cd_recovered, (int)0x80000003u, 0);
        if (res < 0)
            return -0xd612;
        if (ih_cd_recovered.server == 0)
            bind_delay_recovered();
    } while (ih_cd_recovered.server == 0);
    ih_caps_recovered |= 1;
    return 0;
}
