#ifndef SNESSTATION_SIFRPC_LEGACY_COMPAT_H
#define SNESSTATION_SIFRPC_LEGACY_COMPAT_H

/*
 * Minimal ABI surface for the PS2LIB 2004 EE sifrpc.c source recovered from
 * ps2dev/ps2sdk@a80df908256955382f102278400b5d713552dbce.
 *
 * Struct layouts, constants and prototypes come from the historical headers
 * consumed by the byte-exact proof. Keep this header deliberately small so
 * the recovered source can live in-tree without importing a whole PS2SDK.
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long int u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long int s64;

#ifndef NULL
#define NULL ((void *)0)
#endif

#define E_LIB_SEMA_CREATE 0xd602
#define E_SIF_PKT_ALLOC   0xd610
#define E_SIF_PKT_SEND    0xd611

#define SIF_RPC_M_NOWAIT  0x01
#define SIF_RPC_M_NOWBDC  0x02

#define DI DIntr
#define EI EIntr
#define UNCACHED_SEG(x) ((void *)(((u32)(x)) | 0x20000000))

typedef struct t_ee_sema {
    int count;
    int max_count;
    int init_count;
    int wait_threads;
    u32 attr;
    u32 option;
} ee_sema_t;

typedef struct t_SifCmdHeader {
    u32 size;
    void *dest;
    int cid;
    u32 unknown;
} SifCmdHeader_t;

typedef void *(*SifRpcFunc_t)(int, void *, int);
typedef void (*SifRpcEndFunc_t)(void *);

typedef struct t_SifRpcPktHeader {
    struct t_SifCmdHeader sifcmd;
    int rec_id;
    void *pkt_addr;
    int rpc_id;
} SifRpcPktHeader_t;

typedef struct t_SifRpcRendPkt {
    struct t_SifCmdHeader sifcmd;
    int rec_id;
    void *pkt_addr;
    int rpc_id;
    struct t_SifRpcClientData *client;
    u32 cid;
    struct t_SifRpcServerData *server;
    void *buff;
    void *cbuff;
} SifRpcRendPkt_t;

typedef struct t_SifRpcOtherDataPkt {
    struct t_SifCmdHeader sifcmd;
    int rec_id;
    void *pkt_addr;
    int rpc_id;
    struct t_SifRpcReceiveData *receive;
    void *src;
    void *dest;
    int size;
} SifRpcOtherDataPkt_t;

typedef struct t_SifRpcBindPkt {
    struct t_SifCmdHeader sifcmd;
    int rec_id;
    void *pkt_addr;
    int rpc_id;
    struct t_SifRpcClientData *client;
    int sid;
} SifRpcBindPkt_t;

typedef struct t_SifRpcCallPkt {
    struct t_SifCmdHeader sifcmd;
    int rec_id;
    void *pkt_addr;
    int rpc_id;
    struct t_SifRpcClientData *client;
    int rpc_number;
    int send_size;
    void *receive;
    int recv_size;
    int rmode;
    struct t_SifRpcServerData *server;
} SifRpcCallPkt_t;

typedef struct t_SifRpcServerData {
    int sid;
    SifRpcFunc_t func;
    void *buff;
    int size;
    SifRpcFunc_t cfunc;
    void *cbuff;
    int size2;
    struct t_SifRpcClientData *client;
    void *pkt_addr;
    int rpc_number;
    void *receive;
    int rsize;
    int rmode;
    int rid;
    struct t_SifRpcServerData *link;
    struct t_SifRpcServerData *next;
    struct t_SifRpcDataQueue *base;
} SifRpcServerData_t;

typedef struct t_SifRpcHeader {
    void *pkt_addr;
    u32 rpc_id;
    int sema_id;
    u32 mode;
} SifRpcHeader_t;

typedef struct t_SifRpcClientData {
    struct t_SifRpcHeader hdr;
    u32 command;
    void *buff;
    void *cbuff;
    SifRpcEndFunc_t end_function;
    void *end_param;
    struct t_SifRpcServerData *server;
} SifRpcClientData_t;

typedef struct t_SifRpcReceiveData {
    struct t_SifRpcHeader hdr;
    void *src;
    void *dest;
    int size;
} SifRpcReceiveData_t;

typedef struct t_SifRpcDataQueue {
    int thread_id;
    int active;
    struct t_SifRpcServerData *link;
    struct t_SifRpcServerData *start;
    struct t_SifRpcServerData *end;
    struct t_SifRpcDataQueue *next;
} SifRpcDataQueue_t;

typedef struct t_SifDmaTransfer {
    void *src;
    void *dest;
    int size;
    int attr;
} SifDmaTransfer_t;

int DIntr(void);
int EIntr(void);
int CreateSema(ee_sema_t *);
int DeleteSema(int);
int iSignalSema(int);
int WaitSema(int);
int iWakeupThread(int);
int SleepThread(void);

u32 SifSendCmd(int, void *, int, void *, void *, int);
u32 iSifSendCmd(int, void *, int, void *, void *, int);
void SifAddCmdHandler(int, void (*)(void *, void *), void *);
void SifInitCmd(void);
void SifExitCmd(void);
int SifGetSreg(int);
int SifSetReg(u32, int);
int SifGetReg(u32);
void SifWriteBackDCache(void *, int);
u32 SifSetDma(SifDmaTransfer_t *, s32);

static inline void nopdelay(void)
{
    int i = 0xfffff;
    do {
        __asm__("nop\nnop\nnop\nnop\nnop\n");
    } while (i-- != -1);
}

#endif
