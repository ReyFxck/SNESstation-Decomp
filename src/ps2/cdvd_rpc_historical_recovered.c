/*
 * Historical EE libcdvd RPC corridor recovered for SNES Station v0.23.
 * Target: 0x0019be70..0x0019c363.
 *
 * Primary historical source:
 *   iaddis/SNESticle, SNESticle/Modules/libcdvd/ee/cdvd_rpc.c
 *   commit 9590ebf3bf768424ebd6cb018f322e724a7aade3
 *
 * CDVD_GetSize() and CDVD_GETSIZE=0x08 survive in the closely-related PGEN
 * copy of the same libcdvd family:
 *   ps2homebrew/pgen, ps2/lib/cdvd_rpc.c
 *   commit f722681391fb6a1cc64a1260027a33862685e585
 *
 * This translation unit is intentionally self-contained for historical
 * compiler matching.  The compatibility declarations below preserve the
 * 32-bit EE ABI layout needed by this old RPC client without importing a
 * modern PS2SDK.
 */

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned int size_t;

typedef void (*SifRpcEndFunc_t)(void *);

/*
 * The historical client layout used by this binary has `server` at +0x24.
 * That exact offset is independently visible in CDVD_Init at 0x0019bea0.
 */
typedef struct SifRpcClientData_recovered {
    u8 _before_server[0x24];
    void *server;
} SifRpcClientData_t;

struct TocEntry {
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

static unsigned sbuff_recovered[0x1300] __attribute__((aligned(64)));
static SifRpcClientData_t cd0_recovered;

int cdvd_inited_recovered = 0;

/* 0x0019be70 */
int CDVD_Init_0019be70(void)
{
    int i;

    while (1) {
        if (SifBindRpc(&cd0_recovered, CDVD_IRX, 0) < 0)
            return -1;

        if (cd0_recovered.server != 0)
            break;

        i = 0x10000;
        while (i--)
            ;
    }

    cdvd_inited_recovered = 1;

    return 0;
}

/* 0x0019bf00 */
int CDVD_DiskReady_0019bf00(int mode)
{
    if (!cdvd_inited_recovered)
        return -1;

    sbuff_recovered[0] = mode;

    SifCallRpc(&cd0_recovered, CDVD_DISKREADY, 0,
               (void *)(&sbuff_recovered[0]), 4,
               (void *)(&sbuff_recovered[0]), 4, 0, 0);

    return sbuff_recovered[0];
}

/* 0x0019bf70 */
int CDVD_FindFile_0019bf70(const char *fname, struct TocEntry *tocEntry)
{
    if (!cdvd_inited_recovered)
        return -1;

    strncpy((char *)&sbuff_recovered, fname, 1024);

    SifCallRpc(&cd0_recovered, CDVD_FINDFILE, 0,
               (void *)(&sbuff_recovered[0]), 1024,
               (void *)(&sbuff_recovered[0]),
               sizeof(struct TocEntry) + 1024, 0, 0);

    memcpy(tocEntry, &sbuff_recovered[256], sizeof(struct TocEntry));

    return sbuff_recovered[0];
}

/* 0x0019c0d0 */
void CDVD_Stop_0019c0d0(void)
{
    if (!cdvd_inited_recovered)
        return;

    SifCallRpc(&cd0_recovered, CDVD_STOP, 0,
               (void *)(&sbuff_recovered[0]), 0,
               (void *)(&sbuff_recovered[0]), 0, 0, 0);

    return;
}

/* 0x0019c128 */
int CDVD_TrayReq_0019c128(int mode)
{
    (void)mode;

    if (!cdvd_inited_recovered)
        return -1;

    SifCallRpc(&cd0_recovered, CDVD_TRAYREQ, 0,
               (void *)(&sbuff_recovered[0]), 4,
               (void *)(&sbuff_recovered[0]), 4, 0, 0);

    return sbuff_recovered[0];
}

/* 0x0019c190 */
int CDVD_getdir_0019c190(const char *pathname, const char *extensions,
                         enum CDVD_getMode getMode,
                         struct TocEntry tocEntry[],
                         unsigned int req_entries, char *new_pathname)
{
    unsigned int num_entries;

    if (!cdvd_inited_recovered)
        return -1;

    strncpy((char *)sbuff_recovered, pathname, 1023);

    if (extensions == 0) {
        sbuff_recovered[1024 / 4] = 0;
    } else {
        strncpy((char *)&sbuff_recovered[1024 / 4], extensions, 127);
    }

    sbuff_recovered[1152 / 4] = getMode;
    sbuff_recovered[1156 / 4] = (int)tocEntry;
    sbuff_recovered[1160 / 4] = req_entries;

    SifWriteBackDCache(tocEntry, req_entries * sizeof(struct TocEntry));

    SifCallRpc(&cd0_recovered, CDVD_GETDIR, 0,
               (void *)(&sbuff_recovered[0]), 1024 + 128 + 4 + 4 + 4,
               (void *)(&sbuff_recovered[0]), 4 + 1024, 0, 0);

    num_entries = sbuff_recovered[0];

    if (new_pathname != 0)
        strncpy(new_pathname, (char *)&sbuff_recovered[1], 1023);

    return num_entries;
}

/* 0x0019c2ac */
void CDVD_FlushCache_0019c2ac(void)
{
    if (!cdvd_inited_recovered)
        return;

    SifCallRpc(&cd0_recovered, CDVD_FLUSHCACHE, 0,
               (void *)(&sbuff_recovered[0]), 0,
               (void *)(&sbuff_recovered[0]), 0, 0, 0);

    return;
}

/* 0x0019c304 */
unsigned int CDVD_GetSize_0019c304(void)
{
    /*
     * Historical source really used a bare `return;` here despite the
     * unsigned return type.  Preserve it: the target inactive path likewise
     * leaves $v0 unspecified.
     */
    if (!cdvd_inited_recovered) {
#ifdef SNESSTATION_HOST_SYNTAX
        /* Syntax-only host build: modern GCC rejects the historical bare return. */
        return 0;
#else
        /* Historical EE path: preserve the original undefined $v0 return value. */
        return;
#endif
    }

    SifCallRpc(&cd0_recovered, CDVD_GETSIZE, 0,
               (void *)(&sbuff_recovered[0]), 0,
               (void *)(&sbuff_recovered[0]), 4, 0, 0);

    return sbuff_recovered[0];
}
