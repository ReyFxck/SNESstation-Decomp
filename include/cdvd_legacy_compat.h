#ifndef SNESSTATION_CDVD_LEGACY_COMPAT_H
#define SNESSTATION_CDVD_LEGACY_COMPAT_H

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned int size_t;

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef void (*SifRpcEndFunc_t)(void *);

struct t_SifRpcServerData;

typedef struct t_SifRpcHeader
{
    void *pkt_addr;
    u32 rpc_id;
    int sema_id;
    u32 mode;
} SifRpcHeader_t;

/*
 * Historical PS2 SIF RPC client layout.  On the EE ABI used here:
 *   sizeof(SifRpcHeader_t) == 0x10
 *   offsetof(server)       == 0x24
 *   sizeof(client)         == 0x28
 *
 * The SNES Station target independently confirms server at +0x24 in
 * CDVD_Init (lw ..., 0x24($s0)).
 */
typedef struct t_SifRpcClientData
{
    struct t_SifRpcHeader hdr;
    u32 command;
    void *buff, *cbuff;
    SifRpcEndFunc_t end_function;
    void *end_param;
    struct t_SifRpcServerData *server;
} SifRpcClientData_t;

struct TocEntry
{
    u32 fileLBA;
    u32 fileSize;
    u8 fileProperties;
    u8 padding1[3];
    char filename[128+1];
    u8 padding2[3];
} __attribute__((packed));

enum CDVD_getMode {
    CDVD_GET_FILES_ONLY = 1,
    CDVD_GET_DIRS_ONLY = 2,
    CDVD_GET_FILES_AND_DIRS = 3
};

#define CDVD_IRX        0xB001337
#define CDVD_FINDFILE   0x01
#define CDVD_GETDIR     0x02
#define CDVD_STOP       0x04
#define CDVD_TRAYREQ    0x05
#define CDVD_DISKREADY  0x06
#define CDVD_FLUSHCACHE 0x07
#define CDVD_GETSIZE    0x08

int SifBindRpc(SifRpcClientData_t *client, int rpc_number, int mode);
int SifCallRpc(SifRpcClientData_t *client, int rpc_number, int mode,
               void *send, int ssize, void *receive, int rsize,
               SifRpcEndFunc_t end_function, void *end_param);
void SifWriteBackDCache(void *ptr, int size);

char *strncpy(char *dest, const char *src, size_t n);
void *memcpy(void *dest, const void *src, size_t n);

#endif
