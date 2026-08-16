/*
 * Recovered old PS2DEV/PS2SDK SIF RPC client/core linked in SNES Station v0.23.
 * Target entries: 0x0019c688..0x0019cfaf.
 *
 * This file expresses target behavior at C level. Pointer widths in the target
 * are 32-bit; host compilation is used only as a syntax/consistency check.
 */
#include <stdint.h>
#include <stddef.h>
#include "../../include/ps2_libkernel_recovered.h"

enum {
    SIF_RPC_M_NOWAIT_RECOVERED = 1,
    SIF_RPC_M_NOWBDC_RECOVERED = 2,
    RPC_PACKET_SIZE_RECOVERED = 64,
    PACKET_F_ALLOC_RECOVERED = 1,
    E_SIF_PKT_ALLOC_RECOVERED = -0xd610,
    E_SIF_PKT_SEND_RECOVERED = -0xd611,
    E_LIB_SEMA_CREATE_RECOVERED = -0xd602
};

typedef void (*SifRpcEndFunc_recovered)(void *);

typedef struct SifRpcHeader_recovered {
    void *pkt_addr;
    uint32_t rpc_id;
    int sema_id;
    uint32_t mode;
} SifRpcHeader_recovered;

typedef struct SifRpcClientData_recovered {
    SifRpcHeader_recovered hdr;
    uint32_t command;
    void *buff;
    void *cbuff;
    SifRpcEndFunc_recovered end_function;
    void *end_param;
    struct SifRpcServerData_recovered *server;
} SifRpcClientData_recovered;

typedef struct SifRpcPktHeader_recovered {
    uint8_t sifcmd[16];
    int rec_id;
    void *pkt_addr;
    int rpc_id;
} SifRpcPktHeader_recovered;

typedef struct SifRpcBindPkt_recovered {
    SifRpcPktHeader_recovered hdr;
    SifRpcClientData_recovered *client;
    int sid;
} SifRpcBindPkt_recovered;

typedef struct SifRpcCallPkt_recovered {
    SifRpcPktHeader_recovered hdr;
    SifRpcClientData_recovered *client;
    int rpc_number;
    int send_size;
    void *receive;
    int recv_size;
    int rmode;
    struct SifRpcServerData_recovered *server;
} SifRpcCallPkt_recovered;

typedef struct SifRpcRendPkt_recovered {
    SifRpcPktHeader_recovered hdr;
    SifRpcClientData_recovered *client;
    uint32_t cid;
    struct SifRpcServerData_recovered *server;
    void *buff;
    void *cbuff;
} SifRpcRendPkt_recovered;

typedef struct SifRpcOtherDataPkt_recovered {
    SifRpcPktHeader_recovered hdr;
    void *receive;
    void *src;
    void *dest;
    int size;
} SifRpcOtherDataPkt_recovered;

typedef struct SifRpcDataQueue_recovered SifRpcDataQueue_recovered;
typedef struct SifRpcServerData_recovered {
    int sid;
    void *func;
    void *buff;
    int size;
    void *cfunc;
    void *cbuff;
    int size2;
    SifRpcClientData_recovered *client;
    void *pkt_addr;
    int rpc_number;
    void *receive;
    int rsize;
    int rmode;
    int rid;
    struct SifRpcServerData_recovered *link;
    struct SifRpcServerData_recovered *next;
    SifRpcDataQueue_recovered *base;
} SifRpcServerData_recovered;

struct SifRpcDataQueue_recovered {
    int thread_id;
    int active;
    SifRpcServerData_recovered *link;
    SifRpcServerData_recovered *start;
    SifRpcServerData_recovered *end;
    SifRpcDataQueue_recovered *next;
};

typedef struct rpc_data_recovered {
    int pid;
    uint8_t *pkt_table;
    int pkt_table_len;
    int unused1;
    int unused2;
    uint8_t *rdata_table;
    int rdata_table_len;
    uint8_t *client_table;
    int client_table_len;
    int rdata_table_idx;
    SifRpcDataQueue_recovered *active_queue;
} rpc_data_recovered;


extern int DIntr(void);              /* target helper at 0x0019f018 */
extern int EIntr(void);              /* target helper at 0x0019f060 */
extern int CreateSema(const ee_sema32 *); /* 0x0019ce70 */
extern int DeleteSema(int);          /* 0x0019ce80 */
extern int iSignalSema(int);         /* 0x0019ce90 */
extern int WaitSema(int);            /* 0x0019cea0 */
extern int iWakeupThread(int);       /* 0x0019ce60 */
extern void SifWriteBackDCache(void *, int); /* 0x0019cf10 */
extern int SifSendCmd(uint32_t, void *, int, void *, void *, int);
extern int iSifSendCmd(uint32_t, void *, int, void *, void *, int);
extern void SifInitCmd(void);
extern void SifExitCmd(void);
extern void SifAddCmdHandler(uint32_t, void (*)(void *, void *), void *);
extern uint32_t SifGetReg(uint32_t); /* 0x0019cf00 */
extern void SifSetReg(uint32_t, uint32_t); /* 0x0019cef0 */
extern uint32_t SifGetSreg(unsigned int);

static uint8_t pkt_table_recovered[2048];
static uint8_t rdata_table_recovered[2048];
static uint8_t client_table_recovered[2048];
static rpc_data_recovered sif_rpc_data_recovered = {
    1, pkt_table_recovered, 32, 0, 0,
    rdata_table_recovered, 32, client_table_recovered, 32, 0, NULL
};
static int sif_rpc_init_recovered;

/* 0x0019cd70 */
static void *rpc_get_packet_0019cd70(rpc_data_recovered *rpc)
{
    int rid;
    SifRpcPktHeader_recovered *packet;
    (void)DIntr();
    for (rid = 0; rid < rpc->pkt_table_len; ++rid) {
        packet = (SifRpcPktHeader_recovered *)(void *)(rpc->pkt_table + rid * 64);
        if (!(packet->rec_id & PACKET_F_ALLOC_RECOVERED)) {
            int pid = rpc->pid;
            if (pid != 0) {
                rpc->pid = pid + 1;
            } else {
                rpc->pid = 2;
                pid = 1;
            }
            packet->rec_id = (rid << 16) | 0x05; /* target uses 0x05 */
            packet->rpc_id = pid;
            packet->pkt_addr = packet;
            (void)EIntr();
            return packet;
        }
    }
    (void)EIntr();
    return NULL;
}

/* 0x0019ce2c */
static void *rpc_get_fpacket_0019ce2c(rpc_data_recovered *rpc)
{
    int index = rpc->rdata_table_idx % rpc->rdata_table_len;
    rpc->rdata_table_idx = index + 1;
    return rpc->rdata_table + index * 64;
}

/* 0x0019c688 */
int SifBindRpc_0019c688(SifRpcClientData_recovered *cd, int sid, int mode)
{
    ee_sema32 sema;
    SifRpcBindPkt_recovered *bind = rpc_get_packet_0019cd70(&sif_rpc_data_recovered);
    if (bind == NULL)
        return E_SIF_PKT_ALLOC_RECOVERED;

    cd->command = 0;
    cd->server = NULL;
    cd->hdr.pkt_addr = bind;
    cd->hdr.rpc_id = (uint32_t)bind->hdr.rpc_id;
    cd->hdr.sema_id = -1;
    bind->sid = sid;
    bind->hdr.pkt_addr = bind;
    bind->client = cd;

    if (mode & SIF_RPC_M_NOWAIT_RECOVERED) {
        if (!SifSendCmd(0x80000009u, bind, 64, NULL, NULL, 0))
            return E_SIF_PKT_SEND_RECOVERED;
        return 0;
    }

    /* Target writes only max_count (+4) and init_count (+8). */
    sema.max_count = 1;
    sema.init_count = 0;
    cd->hdr.sema_id = CreateSema(&sema);
    if (cd->hdr.sema_id < 0)
        return E_LIB_SEMA_CREATE_RECOVERED;
    if (!SifSendCmd(0x80000009u, bind, 64, NULL, NULL, 0))
        return E_SIF_PKT_SEND_RECOVERED;
    (void)WaitSema(cd->hdr.sema_id);
    (void)DeleteSema(cd->hdr.sema_id);
    return 0;
}

/* 0x0019c7b0 */
int SifCallRpc_0019c7b0(SifRpcClientData_recovered *cd, int rpc_number, int mode,
                         void *sendbuf, int ssize, void *recvbuf, int rsize,
                         SifRpcEndFunc_recovered endfunc, void *efarg)
{
    ee_sema32 sema;
    SifRpcCallPkt_recovered *call = rpc_get_packet_0019cd70(&sif_rpc_data_recovered);
    if (call == NULL)
        return E_SIF_PKT_ALLOC_RECOVERED;

    cd->hdr.pkt_addr = call;
    cd->hdr.rpc_id = (uint32_t)call->hdr.rpc_id;
    cd->hdr.sema_id = -1;
    cd->end_function = endfunc;
    cd->end_param = efarg;

    call->rpc_number = rpc_number;
    call->send_size = ssize;
    call->receive = recvbuf;
    call->recv_size = rsize;
    call->rmode = 1;
    call->hdr.pkt_addr = call;
    call->client = cd;
    call->server = cd->server;

    if (!(mode & SIF_RPC_M_NOWBDC_RECOVERED)) {
        if (ssize > 0) SifWriteBackDCache(sendbuf, ssize);
        if (rsize > 0) SifWriteBackDCache(recvbuf, rsize);
    }

    if (mode & SIF_RPC_M_NOWAIT_RECOVERED) {
        if (endfunc == NULL)
            call->rmode = 0;
        if (!SifSendCmd(0x8000000au, call, 64, sendbuf, cd->buff, ssize))
            return E_SIF_PKT_SEND_RECOVERED;
        return 0;
    }

    /* Target writes only max_count (+4) and init_count (+8). */
    sema.max_count = 1;
    sema.init_count = 0;
    cd->hdr.sema_id = CreateSema(&sema);
    if (cd->hdr.sema_id < 0)
        return E_LIB_SEMA_CREATE_RECOVERED;
    if (!SifSendCmd(0x8000000au, call, 64, sendbuf, cd->buff, ssize))
        return E_SIF_PKT_SEND_RECOVERED;
    (void)WaitSema(cd->hdr.sema_id);
    (void)DeleteSema(cd->hdr.sema_id);
    return 0;
}

/* 0x0019c960 */
static void rpc_packet_free_0019c960(void *packet)
{
    SifRpcPktHeader_recovered *hdr = packet;
    hdr->rpc_id = 0;
    hdr->rec_id &= ~PACKET_F_ALLOC_RECOVERED;
}

/* 0x0019c978 */
static void request_end_0019c978(void *request_ptr, void *data)
{
    SifRpcRendPkt_recovered *request = request_ptr;
    SifRpcClientData_recovered *client = request->client;
    (void)data;
    if (request->cid == 0x8000000au) {
        if (client->end_function != NULL)
            client->end_function(client->end_param);
    } else if (request->cid == 0x80000009u) {
        client->server = request->server;
        client->buff = request->buff;
        client->cbuff = request->cbuff;
    }
    if (client->hdr.sema_id >= 0)
        (void)iSignalSema(client->hdr.sema_id);
    rpc_packet_free_0019c960(client->hdr.pkt_addr);
    client->hdr.pkt_addr = NULL;
}

/* 0x0019ca0c */
static SifRpcServerData_recovered *search_svdata_0019ca0c(uint32_t sid,
                                                          rpc_data_recovered *rpc)
{
    SifRpcDataQueue_recovered *queue = rpc->active_queue;
    while (queue != NULL) {
        SifRpcServerData_recovered *server = queue->link;
        while (server != NULL) {
            if ((uint32_t)server->sid == sid)
                return server;
            server = server->link;
        }
        queue = queue->next;
    }
    return NULL;
}

/* 0x0019ca54 */
static void request_bind_0019ca54(void *bind_ptr, void *data)
{
    SifRpcBindPkt_recovered *bind = bind_ptr;
    rpc_data_recovered *rpc = data;
    SifRpcRendPkt_recovered *rend = rpc_get_fpacket_0019ce2c(rpc);
    SifRpcServerData_recovered *server;
    rend->hdr.pkt_addr = bind->hdr.pkt_addr;
    rend->client = bind->client;
    rend->cid = 0x80000009u;
    server = search_svdata_0019ca0c((uint32_t)bind->sid, rpc);
    rend->server = server;
    rend->buff = server != NULL ? server->buff : NULL;
    rend->cbuff = server != NULL ? server->cbuff : NULL;
    (void)iSifSendCmd(0x80000008u, rend, 64, NULL, NULL, 0);
}

/* 0x0019cb08 */
static void request_call_0019cb08(void *request_ptr, void *data)
{
    SifRpcCallPkt_recovered *request = request_ptr;
    SifRpcServerData_recovered *server = request->server;
    SifRpcDataQueue_recovered *base = server->base;
    (void)data;
    if (base->start != NULL) base->end->link = server;
    else base->start = server;
    base->end = server;
    server->pkt_addr = request->hdr.pkt_addr;
    server->client = request->client;
    server->rpc_number = request->rpc_number;
    server->size = request->send_size;
    server->receive = request->receive;
    server->rsize = request->recv_size;
    server->rmode = request->rmode;
    server->rid = request->hdr.rec_id;
    if (base->thread_id >= 0 && base->active != 0)
        (void)iWakeupThread(base->thread_id);
}

/* 0x0019cba8 */
static void request_rdata_0019cba8(void *rdata_ptr, void *data)
{
    SifRpcOtherDataPkt_recovered *rdata = rdata_ptr;
    SifRpcRendPkt_recovered *rend = rpc_get_fpacket_0019ce2c(data);
    rend->hdr.pkt_addr = rdata->hdr.pkt_addr;
    rend->client = rdata->receive;
    rend->cid = 0x8000000cu;
    (void)iSifSendCmd(0x80000008u, rend, 64, rdata->src, rdata->dest, rdata->size);
}

/* 0x0019cc0c */
void SifInitRpc_0019cc0c(int mode)
{
    uint32_t *cmdp;
    (void)mode;
    if (sif_rpc_init_recovered)
        return;
    sif_rpc_init_recovered = 1;
    SifInitCmd();

    (void)DIntr();
    /* Target turns all three work tables into KSEG1/uncached addresses. */
    sif_rpc_data_recovered.pkt_table = (uint8_t *)((uintptr_t)sif_rpc_data_recovered.pkt_table | 0x20000000u);
    sif_rpc_data_recovered.rdata_table = (uint8_t *)((uintptr_t)sif_rpc_data_recovered.rdata_table | 0x20000000u);
    sif_rpc_data_recovered.client_table = (uint8_t *)((uintptr_t)sif_rpc_data_recovered.client_table | 0x20000000u);

    SifAddCmdHandler(0x80000008u, request_end_0019c978, &sif_rpc_data_recovered);
    SifAddCmdHandler(0x80000009u, request_bind_0019ca54, &sif_rpc_data_recovered);
    SifAddCmdHandler(0x8000000au, request_call_0019cb08, &sif_rpc_data_recovered);
    SifAddCmdHandler(0x8000000cu, request_rdata_0019cba8, &sif_rpc_data_recovered);
    (void)EIntr();

    if (SifGetReg(0x80000002u) != 0)
        return;
    cmdp = (uint32_t *)(void *)(pkt_table_recovered + 64);
    cmdp[3] = 1;
    (void)SifSendCmd(0x80000002u, cmdp, 16, NULL, NULL, 0);
    while (SifGetSreg(0) == 0) {
    }
    SifSetReg(0x80000002u, 1);
}

/* 0x0019cd4c */
void SifExitRpc_0019cd4c(void)
{
    SifExitCmd();
    sif_rpc_init_recovered = 0;
}
