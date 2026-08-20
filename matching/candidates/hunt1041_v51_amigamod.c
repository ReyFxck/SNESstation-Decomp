typedef unsigned int u128 __attribute__((mode(TI)));

typedef struct t_SifRpcClientData {
    unsigned char opaque[40];
} SifRpcClientData_t;

typedef struct t_SifDmaTransfer {
    void *src;
    void *dest;
    int size;
    int attr;
} SifDmaTransfer_t;

#define VZMOD UINT32_C(0x0badca11)
#define MOD_INIT 0x0010
#define MOD_LOAD 0x0020

#include <stdint.h>

extern int SifBindRpc(SifRpcClientData_t *client, uint32_t rpc_id, int mode);
extern int SifCallRpc(SifRpcClientData_t *client, int command, int mode,
                      void *send, int send_size, void *receive,
                      int receive_size, void *end_function, void *end_param);
extern int SifFreeIopHeap(void *address);
extern void *SifAllocIopHeap(int size);
extern uint32_t SifSetDma(SifDmaTransfer_t *transfer, int count);
extern int SifDmaStat(uint32_t id);

static SifRpcClientData_t amodCd __attribute__((aligned(64)));
static unsigned sbuff[64] __attribute__((aligned(64)));

static int ammodi = 0;
static void *iopmodimg = 0;

int amigaModInit(int nosdinit)
{
    if (ammodi)
        return -1;

    if (SifBindRpc(&amodCd, VZMOD, 0) < 0)
        return -1;

    if (!nosdinit)
    {
        char hi[16] = "amigaModInit !";
        *(u128 *)sbuff = *(u128 *)hi;
        SifCallRpc(&amodCd, MOD_INIT, 0, (void *)(&sbuff[0]), 16,
                   (void *)(&sbuff[0]), 64, 0, 0);
    }

    ammodi = 1;
    return 0;
}

/* The application initializes the IOP heap before entering this RPC layer. */
int amigaModLoad(void *moddata, int size)
{
    int i;
    SifDmaTransfer_t sdt;

    if (!ammodi)
        return -1;

    if (iopmodimg)
        SifFreeIopHeap(iopmodimg);
    iopmodimg = SifAllocIopHeap(size);

    sdt.src = moddata;
    sdt.dest = iopmodimg;
    sdt.size = size;
    sdt.attr = 0;
    i = SifSetDma(&sdt, 1);
    while (SifDmaStat(i) >= 0)
        ;

    sbuff[0] = (int)iopmodimg;
    SifCallRpc(&amodCd, MOD_LOAD, 0, (void *)(&sbuff[0]), 4,
               (void *)(&sbuff[0]), 64, 0, 0);

    return sbuff[0];
}
