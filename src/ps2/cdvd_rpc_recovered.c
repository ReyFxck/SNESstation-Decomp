/*
 * SNES Station v0.23 -- recovered EE side of Hiryu/Sjeep libcdvd RPC.
 * Target corridor: 0x0019be70..0x0019c363.
 *
 * Target evidence is authoritative. Historical libcdvd sources are used only
 * to recover names/types after the RPC id, command numbers and buffer layout
 * were independently visible in the stripped binary.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../../include/ps2_libkernel_recovered.h"

typedef struct {
    uint32_t fileLBA;
    uint32_t fileSize;
    uint8_t fileProperties;
    uint8_t padding1[3];
    char filename[129];
    uint8_t padding2[3];
} TocEntry_recovered;

typedef char recovered_assert_toc_entry_size[(sizeof(TocEntry_recovered) == 0x90) ? 1 : -1];

enum {
    CDVD_IRX_RECOVERED       = 0x0b001337,
    CDVD_FINDFILE_RECOVERED  = 0x01,
    CDVD_GETDIR_RECOVERED    = 0x02,
    CDVD_STOP_RECOVERED      = 0x04,
    CDVD_TRAYREQ_RECOVERED   = 0x05,
    CDVD_DISKREADY_RECOVERED = 0x06,
    CDVD_FLUSHCACHE_RECOVERED= 0x07,
    CDVD_GETSIZE_RECOVERED   = 0x08
};

extern int SifBindRpc(SifRpcClientData32 *, int, int);
extern int SifCallRpc(SifRpcClientData32 *, int, int,
                      void *, int, void *, int, void (*)(void *), void *);
extern void SifWriteBackDCache(void *, int);

/* Target sbuff lives at 0x0043ed00 and is 0x1300 uint32_t entries. */
static uint32_t sbuff_recovered[0x1300];
static SifRpcClientData32 cd0_recovered;
static int cdvd_inited_recovered;

/* 0x0019be70 */
int CDVD_Init_0019be70(void)
{
    for (;;) {
        int delay;
        if (SifBindRpc(&cd0_recovered, CDVD_IRX_RECOVERED, 0) < 0)
            return -1;
        if (cd0_recovered.server != 0) /* target client->server @ +0x24 */
            break;
        delay = 0x10000;
        while (delay-- != 0) {
            /* target emits a deliberately empty delay loop */
        }
    }
    cdvd_inited_recovered = 1;
    return 0;
}

/* 0x0019bf00 */
int CDVD_DiskReady_0019bf00(int mode)
{
    if (!cdvd_inited_recovered)
        return -1;
    sbuff_recovered[0] = (uint32_t)mode;
    (void)SifCallRpc(&cd0_recovered, CDVD_DISKREADY_RECOVERED, 0,
                     sbuff_recovered, 4, sbuff_recovered, 4, NULL, NULL);
    return (int)sbuff_recovered[0];
}

/* 0x0019bf70 */
int CDVD_FindFile_0019bf70(const char *fname, TocEntry_recovered *entry)
{
    if (!cdvd_inited_recovered)
        return -1;
    strncpy((char *)sbuff_recovered, fname, 1024);
    (void)SifCallRpc(&cd0_recovered, CDVD_FINDFILE_RECOVERED, 0,
                     sbuff_recovered, 1024, sbuff_recovered, 1024 + 0x90,
                     NULL, NULL);
    memcpy(entry, &sbuff_recovered[256], sizeof(*entry));
    return (int)sbuff_recovered[0];
}

/* 0x0019c0d0 */
void CDVD_Stop_0019c0d0(void)
{
    if (!cdvd_inited_recovered)
        return;
    (void)SifCallRpc(&cd0_recovered, CDVD_STOP_RECOVERED, 0,
                     sbuff_recovered, 0, sbuff_recovered, 0, NULL, NULL);
}

/* 0x0019c128
 * Preserve a historical quirk: mode is never copied into sbuff[0].
 */
int CDVD_TrayReq_0019c128(int mode)
{
    (void)mode;
    if (!cdvd_inited_recovered)
        return -1;
    (void)SifCallRpc(&cd0_recovered, CDVD_TRAYREQ_RECOVERED, 0,
                     sbuff_recovered, 4, sbuff_recovered, 4, NULL, NULL);
    return (int)sbuff_recovered[0];
}

/* 0x0019c190 */
int CDVD_getdir_0019c190(const char *pathname, const char *extensions,
                         int get_mode, TocEntry_recovered *entries,
                         unsigned int req_entries, char *new_pathname)
{
    uint32_t num_entries;
    if (!cdvd_inited_recovered)
        return -1;

    strncpy((char *)sbuff_recovered, pathname, 1023);
    if (extensions != NULL)
        strncpy((char *)&sbuff_recovered[256], extensions, 127);
    else
        sbuff_recovered[256] = 0;

    sbuff_recovered[288] = (uint32_t)get_mode;
    sbuff_recovered[289] = (uint32_t)(uintptr_t)entries;
    sbuff_recovered[290] = req_entries;

    SifWriteBackDCache(entries, (int)(req_entries * sizeof(*entries)));
    (void)SifCallRpc(&cd0_recovered, CDVD_GETDIR_RECOVERED, 0,
                     sbuff_recovered, 0x48c, sbuff_recovered, 0x404,
                     NULL, NULL);

    num_entries = sbuff_recovered[0];
    if (new_pathname != NULL)
        strncpy(new_pathname, (const char *)&sbuff_recovered[1], 1023);
    return (int)num_entries;
}

/* 0x0019c2ac */
void CDVD_FlushCache_0019c2ac(void)
{
    if (!cdvd_inited_recovered)
        return;
    (void)SifCallRpc(&cd0_recovered, CDVD_FLUSHCACHE_RECOVERED, 0,
                     sbuff_recovered, 0, sbuff_recovered, 0, NULL, NULL);
}

/* 0x0019c304 -- present in the target and later public libcdvd revisions. */
unsigned int CDVD_GetSize_0019c304(void)
{
    if (!cdvd_inited_recovered)
        return 0;
    (void)SifCallRpc(&cd0_recovered, CDVD_GETSIZE_RECOVERED, 0,
                     sbuff_recovered, 0, sbuff_recovered, 4, NULL, NULL);
    return sbuff_recovered[0];
}
