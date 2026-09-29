/*
 * SNES Station v0.23 - recovered EE libcdvd RPC client.
 *
 * Target corridor: 0x0019be70..0x0019c363.
 *
 * Historical lineage:
 *   - iaddis/SNESticle, SNESticle/Modules/libcdvd/ee/cdvd_rpc.c
 *     commit 9590ebf3bf768424ebd6cb018f322e724a7aade3
 *   - ps2homebrew/pgen, ps2/lib/cdvd_rpc.c
 *     commit f722681391fb6a1cc64a1260027a33862685e585
 *
 * The first seven functions survive in the SNESticle source family.
 * CDVD_GetSize() and CDVD_GETSIZE=0x08 survive in the closely related PGEN
 * revision and are independently present in the SNES Station target at
 * 0x0019c304.
 *
 * This file keeps historical source names and expression shapes while using
 * local ABI declarations so it can be checked without a modern PS2SDK.
 */

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned int size_t;

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef void (*SifRpcEndFunc_t)(void *);

typedef struct SifRpcClientData_recovered {
    u8 _before_server[0x24];
    void *server;
} SifRpcClientData_t;

struct TocEntry
{
    u32 fileLBA;
    u32 fileSize;
    u8 fileProperties;
    u8 padding1[3];
    char filename[128 + 1];
    u8 padding2[3];
} __attribute__((packed));

enum CDVD_getMode {
    CDVD_GET_FILES_ONLY = 1,
    CDVD_GET_DIRS_ONLY = 2,
    CDVD_GET_FILES_AND_DIRS = 3
};

#define CDVD_IRX        0x0B001337
#define CDVD_FINDFILE   0x01
#define CDVD_GETDIR     0x02
#define CDVD_STOP       0x04
#define CDVD_TRAYREQ    0x05
#define CDVD_DISKREADY  0x06
#define CDVD_FLUSHCACHE 0x07
#define CDVD_GETSIZE    0x08

extern int SifBindRpc(SifRpcClientData_t *client, int rpc_number, int mode);
extern int SifCallRpc(SifRpcClientData_t *client, int rpc_number, int mode,
                      void *send, int ssize, void *receive, int rsize,
                      SifRpcEndFunc_t end_function, void *end_param);
extern void SifWriteBackDCache(void *ptr, int size);
extern char *strncpy(char *dest, const char *src, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);

int k_sceSifDmaStat(unsigned int id);
static unsigned sbuff[0x1300] __attribute__((aligned (64)));
static SifRpcClientData_t cd0;

int cdvd_inited = 0;

/* 0x0019be70 */
int CDVD_Init()
{
    int i;

    while(1){
        if (SifBindRpc( &cd0, CDVD_IRX, 0) < 0) return -1;
        if (cd0.server != 0) break;
        i = 0x10000;
        while(i--);
    }

    cdvd_inited = 1;

    return 0;
}

/* 0x0019bf00 */
int CDVD_DiskReady(int mode)
{
    if(!cdvd_inited) return -1;

    sbuff[0] = mode;

    SifCallRpc(&cd0,CDVD_DISKREADY,0,(void*)(&sbuff[0]),4,(void*)(&sbuff[0]),4,0,0);

    return sbuff[0];
}

/* 0x0019bf70 */
int CDVD_FindFile(const char* fname, struct TocEntry* tocEntry)
{
    if(!cdvd_inited) return -1;

    strncpy((char*)&sbuff,fname,1024);

    SifCallRpc(&cd0,CDVD_FINDFILE,0,(void*)(&sbuff[0]),1024,(void*)(&sbuff[0]),sizeof(struct TocEntry)+1024,0,0);

    memcpy(tocEntry, &sbuff[256], sizeof(struct TocEntry));

    return sbuff[0];
}

/* 0x0019c0d0 */
void CDVD_Stop()
{
    if(!cdvd_inited) return;

    SifCallRpc(&cd0,CDVD_STOP,0,(void*)(&sbuff[0]),0,(void*)(&sbuff[0]),0,0,0);

    return;
}

/* 0x0019c128 */
int CDVD_TrayReq(int mode)
{
    (void)mode;

    if(!cdvd_inited) return -1;

    SifCallRpc(&cd0,CDVD_TRAYREQ,0,(void*)(&sbuff[0]),4,(void*)(&sbuff[0]),4,0,0);

    return sbuff[0];
}

/* 0x0019c190 */
int CDVD_getdir(const char* pathname, const char* extensions, enum CDVD_getMode getMode, struct TocEntry tocEntry[], unsigned int req_entries, char* new_pathname)
{
    unsigned int num_entries;

    if(!cdvd_inited) return -1;

    strncpy((char*)sbuff,pathname,1023);

    if (extensions == NULL)
    {
        sbuff[1024/4] = 0;
    }
    else
    {
        strncpy((char*)&sbuff[1024/4],extensions,127);
    }

    sbuff[1152/4] = getMode;
    sbuff[1156/4] = (int)tocEntry;
    sbuff[1160/4] = req_entries;

    SifWriteBackDCache(tocEntry, req_entries*sizeof(struct TocEntry));

    SifCallRpc(&cd0,CDVD_GETDIR,0,(void*)(&sbuff[0]),1024+128+4+4+4,(void*)(&sbuff[0]),4+1024,0,0);

    num_entries = sbuff[0];

    if (new_pathname != NULL)
        strncpy(new_pathname,(char*)&sbuff[1],1023);

    return (num_entries);
}

/* 0x0019c2ac */
void CDVD_FlushCache()
{
    if(!cdvd_inited) return;

    SifCallRpc(&cd0,CDVD_FLUSHCACHE,0,(void*)(&sbuff[0]),0,(void*)(&sbuff[0]),0,0,0);

    return;
}

/* 0x0019c304 */
unsigned int CDVD_GetSize()
{
#ifdef SNESSTATION_HOST_SYNTAX
    if(!cdvd_inited) return 0;
#else
    if(!cdvd_inited) return;
#endif

    SifCallRpc(&cd0,CDVD_GETSIZE,0,(void*)(&sbuff[0]),0,(void*)(&sbuff[0]),4,0,0);

    return sbuff[0];
}
