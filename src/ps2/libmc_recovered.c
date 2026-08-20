#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
 * Old PS2 memory-card RPC client recovered from SNES Station v0.23.
 *
 * Target corridor:
 *   0x001a0740 mcGetInfoApdx
 *   0x001a07c8 mcReadFixAlign
 *   0x001a086c mcStoreDir
 *   0x001a08ec mcInit
 *   0x001a0a68 mcGetInfo
 *   0x001a0ba0 mcOpen
 *   0x001a0c7c mcClose
 *   0x001a0d2c mcSeek
 *   0x001a0df0 mcRead
 *   0x001a0ee8 mcWrite
 *   0x001a1020 mcFlush
 *   0x001a10d0 mcMkDir
 *   0x001a10fc mcChdir
 *   0x001a1204 mcGetDir
 *   0x001a130c mcSetFileInfo
 *   0x001a147c mcDelete
 *   0x001a1554 mcFormat
 *   0x001a1610 mcUnformat
 *   0x001a16cc mcGetEntSpace
 *   0x001a17a4 mcRename
 *   0x001a18c0 mcChangeThreadPriority
 *   0x001a1974 mcSync
 *   0x001a1a54 strcpy_sjis
 *
 * A close 2004 PS2SDK snapshot is useful validation, but the target is an
 * older revision: notably mcInit lacks the later _iop_reboot_count/mcReset
 * path.  This file follows the target behavior, not the later revision.
 */

enum {
    MC_TYPE_MC = 0,
    MC_TYPE_XMC = 1
};

enum {
    MC_FUNC_NONE = 0x00,
    MC_FUNC_GET_INFO = 0x01,
    MC_FUNC_OPEN = 0x02,
    MC_FUNC_CLOSE = 0x03,
    MC_FUNC_SEEK = 0x04,
    MC_FUNC_READ = 0x05,
    MC_FUNC_WRITE = 0x06,
    MC_FUNC_FLUSH = 0x0a,
    MC_FUNC_MK_DIR = 0x0b,
    MC_FUNC_CH_DIR = 0x0c,
    MC_FUNC_GET_DIR = 0x0d,
    MC_FUNC_SET_INFO = 0x0e,
    MC_FUNC_DELETE = 0x0f,
    MC_FUNC_FORMAT = 0x10,
    MC_FUNC_UNFORMAT = 0x11,
    MC_FUNC_GET_ENT = 0x12,
    MC_FUNC_RENAME = 0x13,
    MC_FUNC_CHG_PRITY = 0x14
};

enum {
    MC_RPCCMD_INIT = 0,
    MC_RPCCMD_GET_INFO,
    MC_RPCCMD_OPEN,
    MC_RPCCMD_CLOSE,
    MC_RPCCMD_SEEK,
    MC_RPCCMD_READ,
    MC_RPCCMD_WRITE,
    MC_RPCCMD_FLUSH,
    MC_RPCCMD_CH_DIR,
    MC_RPCCMD_GET_DIR,
    MC_RPCCMD_SET_INFO,
    MC_RPCCMD_DELETE,
    MC_RPCCMD_FORMAT,
    MC_RPCCMD_UNFORMAT,
    MC_RPCCMD_GET_ENT,
    MC_RPCCMD_CHG_PRITY,
    MC_RPCCMD_CHECKBLOCK
};

static const int mc_rpc_cmd[2][17] = {
    {0x70,0x78,0x71,0x72,0x75,0x73,0x74,0x7a,0x7b,0x76,0x7c,0x79,0x77,0x80,0x7e,0x7d,0x7f},
    {0xfe,0x01,0x02,0x03,0x04,0x05,0x06,0x0a,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x14,0x33}
};

typedef struct {
    int32_t port;
    int32_t slot;
    int32_t flags;
    int32_t maxent;
    uint32_t table;
    char name[1024];
} mc_name_param32;

typedef struct {
    int32_t fd;
    int32_t port;
    int32_t slot;
    int32_t size;
    int32_t offset;
    int32_t origin;
    uint32_t buffer;
    uint32_t param;
    unsigned char data[16];
} mc_desc_param32;

typedef struct {
    unsigned char metadata[32];
    unsigned char name[32];
} mc_table64;

typedef struct {
    uint32_t packet;
    uint32_t rpc_id;
    int32_t sema_id;
    uint32_t mode;
    uint32_t command;
    uint32_t buff;
    uint32_t cbuff;
    uint32_t end_function;
    uint32_t end_param;
    uint32_t server;
} mc_rpc_client32;

typedef char assert_mc_name_size[(sizeof(mc_name_param32) == 1044) ? 1 : -1];
typedef char assert_mc_desc_size[(sizeof(mc_desc_param32) == 48) ? 1 : -1];
typedef char assert_mc_table_size[(sizeof(mc_table64) == 64) ? 1 : -1];

static mc_name_param32 g_name_param;
static mc_desc_param32 g_desc_param;
static mc_rpc_client32 g_cdata;
static unsigned char g_rdata[2048];
static int *g_p_type;
static int *g_p_free;
static int *g_p_format;
static int g_end_parameter[48];
static char g_cur_dir[1024];
static mc_table64 g_file_info;
static int g_mclib_inited;
static unsigned int g_current_cmd;
static int g_mc_type;

extern void SifInitRpc_0019cc0c(int mode);
extern int SifBindRpc_0019c688(void *client, int sid, int mode);
extern int SifCallRpc_0019c7b0(void *client, int fno, int mode,
                               void *send, int ssize,
                               void *recv, int rsize,
                               void (*end_func)(void *), void *end_param);
extern void SifWriteBackDCache_0019cf10(const void *ptr, int size);
extern void FlushCache_0019ceb0(int mode);
extern int SifCheckStatRpc_001a1af4(void *client);

static void *uncached_ptr(const void *p)
{
    return (void *)((uintptr_t)p | (uintptr_t)0x20000000u);
}

static uint32_t ee_addr32(const void *p)
{
    return (uint32_t)(uintptr_t)p;
}

static int mc_begin(void)
{
    if (!g_mclib_inited)
        return -1;
    if (g_current_cmd != MC_FUNC_NONE)
        return (int)g_current_cmd;
    return 0;
}

static int mc_call_name(int rpc_cmd, int sync_cmd, void (*cb)(void *), void *cb_arg)
{
    int ret = SifCallRpc_0019c7b0(&g_cdata, mc_rpc_cmd[g_mc_type][rpc_cmd], 1,
                                  &g_name_param, (int)sizeof(g_name_param),
                                  g_rdata, 4, cb, cb_arg);
    if (ret != 0)
        return ret;
    g_current_cmd = (unsigned int)sync_cmd;
    return ret;
}

static int mc_call_desc(int rpc_cmd, int sync_cmd, void (*cb)(void *), void *cb_arg)
{
    int ret = SifCallRpc_0019c7b0(&g_cdata, mc_rpc_cmd[g_mc_type][rpc_cmd], 1,
                                  &g_desc_param, (int)sizeof(g_desc_param),
                                  g_rdata, 4, cb, cb_arg);
    if (ret != 0)
        return ret;
    g_current_cmd = (unsigned int)sync_cmd;
    return ret;
}

/* Target: 0x001a0740. */
void mcGetInfoApdx_001a0740(void *info_raw)
{
    volatile int *info = (volatile int *)uncached_ptr(info_raw);

    if (g_p_type)
        *g_p_type = info[0];
    if (g_p_free)
        *g_p_free = info[1];
    if (g_p_format) {
        if (g_mc_type == MC_TYPE_MC)
            *g_p_format = (info[0] == 0) ? 0 : 1;
        else if (g_mc_type == MC_TYPE_XMC)
            *g_p_format = info[36];
    }
}

/* Target: 0x001a07c8. */
void mcReadFixAlign_001a07c8(void *data_raw)
{
    volatile int *ptr = (volatile int *)uncached_ptr(data_raw);
    unsigned char *dest;
    const unsigned char *src;
    int i;

    dest = (unsigned char *)(uintptr_t)(uint32_t)ptr[2];
    src = (const unsigned char *)(ptr + 4);
    for (i = 0; i < ptr[0]; ++i)
        dest[i] = src[i];

    dest = (unsigned char *)(uintptr_t)(uint32_t)ptr[3];
    src = (const unsigned char *)(ptr + (g_mc_type == MC_TYPE_MC ? 8 : 20));
    for (i = 0; i < ptr[1]; ++i)
        dest[i] = src[i];
}

/* Target: 0x001a086c.  The odd >=1024 fallback is intentionally retained. */
void mcStoreDir_001a086c(void *arg)
{
    char *current = (char *)uncached_ptr(g_cur_dir);
    int len = (int)strlen(current);

    if (len >= 1024)
        len = (int)strlen(current + 1023);
    memcpy(arg, current, (size_t)len);
    current[len] = '\0';
}

/* Target: 0x001a08ec.  Older target revision: no reboot-count/mcReset prelude. */
int mcInit_001a08ec(int type)
{
    int ret = 0;

    if (g_mclib_inited)
        return -1;

    SifInitRpc_0019cc0c(0);
    g_mc_type = type;

    do {
        ret = SifBindRpc_0019c688(&g_cdata, (int)0x80000400u, 0);
        if (ret < 0)
            return ret;
    } while (g_cdata.server == 0);

    if (g_mc_type == MC_TYPE_XMC) {
        ret = SifCallRpc_0019c7b0(&g_cdata, mc_rpc_cmd[g_mc_type][MC_RPCCMD_INIT], 0,
                                  &g_desc_param, (int)sizeof(g_desc_param),
                                  g_rdata, 12, NULL, NULL);
        if (ret < 0) {
            g_mclib_inited = 0;
            return ret - 100;
        }
        if (*(volatile int32_t *)uncached_ptr(g_rdata + 4) < 0x205) {
            g_mclib_inited = 0;
            return -120;
        }
        if (*(volatile int32_t *)uncached_ptr(g_rdata + 8) < 0x206) {
            g_mclib_inited = 0;
            return -121;
        }
        ret = *(volatile int32_t *)uncached_ptr(g_rdata);
    }

    g_mclib_inited = 1;
    g_current_cmd = MC_FUNC_NONE;
    return ret;
}

int mcGetInfo_001a0a68(int port, int slot, int *type, int *free_space, int *format)
{
    int pre = mc_begin();
    if (pre) return pre;

    g_desc_param.port = port;
    g_desc_param.slot = slot;
    if (g_mc_type == MC_TYPE_MC) {
        g_desc_param.size = type != NULL;
        g_desc_param.offset = free_space != NULL;
        g_desc_param.origin = format != NULL;
    } else {
        g_desc_param.size = format != NULL;
        g_desc_param.offset = free_space != NULL;
        g_desc_param.origin = type != NULL;
    }
    g_desc_param.param = ee_addr32(g_end_parameter);
    g_p_type = type;
    g_p_free = free_space;
    g_p_format = format;
    SifWriteBackDCache_0019cf10(g_end_parameter, 192);
    return mc_call_desc(MC_RPCCMD_GET_INFO, MC_FUNC_GET_INFO,
                        mcGetInfoApdx_001a0740, g_end_parameter);
}

int mcOpen_001a0ba0(int port, int slot, const char *name, int mode)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_name_param.port = port;
    g_name_param.slot = slot;
    g_name_param.flags = mode;
    strncpy(g_name_param.name, name, 1023);
    g_name_param.name[1023] = '\0';
    return mc_call_name(MC_RPCCMD_OPEN, MC_FUNC_OPEN, NULL, NULL);
}

int mcClose_001a0c7c(int fd)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_desc_param.fd = fd;
    return mc_call_desc(MC_RPCCMD_CLOSE, MC_FUNC_CLOSE, NULL, NULL);
}

int mcSeek_001a0d2c(int fd, int offset, int origin)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_desc_param.fd = fd;
    g_desc_param.offset = offset;
    g_desc_param.origin = origin;
    return mc_call_desc(MC_RPCCMD_SEEK, MC_FUNC_SEEK, NULL, NULL);
}

int mcRead_001a0df0(int fd, void *buffer, int size)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_desc_param.fd = fd;
    g_desc_param.size = size;
    g_desc_param.buffer = ee_addr32(buffer);
    g_desc_param.param = ee_addr32(g_end_parameter);
    SifWriteBackDCache_0019cf10(buffer, size);
    SifWriteBackDCache_0019cf10(g_end_parameter, 192);
    return mc_call_desc(MC_RPCCMD_READ, MC_FUNC_READ,
                        mcReadFixAlign_001a07c8, g_end_parameter);
}

int mcWrite_001a0ee8(int fd, const void *buffer, int size)
{
    const unsigned char *src = (const unsigned char *)buffer;
    int prefix;
    int i;
    int pre = mc_begin();
    if (pre) return pre;

    g_desc_param.fd = fd;
    if (size < 17) {
        g_desc_param.size = 0;
        g_desc_param.origin = size;
        g_desc_param.buffer = 0;
    } else {
        uintptr_t b = (uintptr_t)buffer;
        prefix = (int)(((b - 1u) & ~(uintptr_t)0x0fu) - (b - 16u));
        g_desc_param.size = size - prefix;
        g_desc_param.origin = prefix;
        g_desc_param.buffer = (uint32_t)(b + (uintptr_t)prefix);
    }
    for (i = 0; i < g_desc_param.origin; ++i)
        g_desc_param.data[i] = src[i];
    FlushCache_0019ceb0(0);
    return mc_call_desc(MC_RPCCMD_WRITE, MC_FUNC_WRITE, NULL, NULL);
}

int mcFlush_001a1020(int fd)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_desc_param.fd = fd;
    return mc_call_desc(MC_RPCCMD_FLUSH, MC_FUNC_FLUSH, NULL, NULL);
}

int mcMkDir_001a10d0(int port, int slot, const char *name)
{
    int ret = mcOpen_001a0ba0(port, slot, name, 0x40);
    if (ret != 0)
        g_current_cmd = MC_FUNC_MK_DIR;
    return ret;
}

int mcChdir_001a10fc(int port, int slot, const char *new_dir, char *current_dir)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_name_param.port = port;
    g_name_param.slot = slot;
    g_name_param.table = ee_addr32(g_cur_dir);
    strncpy(g_name_param.name, new_dir, 1023);
    g_name_param.name[1023] = '\0';
    SifWriteBackDCache_0019cf10(g_cur_dir, 1024);
    return mc_call_name(MC_RPCCMD_CH_DIR, MC_FUNC_CH_DIR,
                        mcStoreDir_001a086c, current_dir);
}

int mcGetDir_001a1204(int port, int slot, const char *name,
                      unsigned int mode, int maxent, mc_table64 *table)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_name_param.port = port;
    g_name_param.slot = slot;
    g_name_param.flags = (int32_t)mode;
    g_name_param.maxent = maxent;
    g_name_param.table = ee_addr32(table);
    strncpy(g_name_param.name, name, 1023);
    g_name_param.name[1023] = '\0';
    SifWriteBackDCache_0019cf10(table, maxent * (int)sizeof(*table));
    return mc_call_name(MC_RPCCMD_GET_DIR, MC_FUNC_GET_DIR, NULL, NULL);
}

int mcSetFileInfo_001a130c(int port, int slot, const char *name,
                           const mc_table64 *info, unsigned int flags)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_name_param.port = port;
    g_name_param.slot = slot;
    g_name_param.flags = (int32_t)flags;
    g_name_param.table = ee_addr32(&g_file_info);
    memcpy(&g_file_info, info, sizeof(g_file_info));
    strncpy(g_name_param.name, name, 1023);
    g_name_param.name[1023] = '\0';
    FlushCache_0019ceb0(0);
    return mc_call_name(MC_RPCCMD_SET_INFO, MC_FUNC_SET_INFO, NULL, NULL);
}

int mcDelete_001a147c(int port, int slot, const char *name)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_name_param.port = port;
    g_name_param.slot = slot;
    g_name_param.flags = 0;
    strncpy(g_name_param.name, name, 1023);
    g_name_param.name[1023] = '\0';
    return mc_call_name(MC_RPCCMD_DELETE, MC_FUNC_DELETE, NULL, NULL);
}

int mcFormat_001a1554(int port, int slot)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_desc_param.port = port;
    g_desc_param.slot = slot;
    return mc_call_desc(MC_RPCCMD_FORMAT, MC_FUNC_FORMAT, NULL, NULL);
}

int mcUnformat_001a1610(int port, int slot)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_desc_param.port = port;
    g_desc_param.slot = slot;
    return mc_call_desc(MC_RPCCMD_UNFORMAT, MC_FUNC_UNFORMAT, NULL, NULL);
}

int mcGetEntSpace_001a16cc(int port, int slot, const char *path)
{
    int pre;
    if (!g_mclib_inited || g_mc_type == MC_TYPE_MC)
        return -1;
    pre = (g_current_cmd != MC_FUNC_NONE) ? (int)g_current_cmd : 0;
    if (pre) return pre;
    g_name_param.port = port;
    g_name_param.slot = slot;
    strncpy(g_name_param.name, path, 1023);
    g_name_param.name[1023] = '\0';
    return mc_call_name(MC_RPCCMD_GET_ENT, MC_FUNC_GET_ENT, NULL, NULL);
}

int mcRename_001a17a4(int port, int slot, const char *old_name, const char *new_name)
{
    int pre = mc_begin();
    if (pre) return pre;
    g_name_param.port = port;
    g_name_param.slot = slot;
    g_name_param.flags = 0x10;
    g_name_param.table = ee_addr32(&g_file_info);
    strncpy(g_name_param.name, old_name, 1023);
    g_name_param.name[1023] = '\0';
    strncpy((char *)g_file_info.name, new_name, 32);
    g_file_info.name[31] = '\0';
    FlushCache_0019ceb0(0);
    return mc_call_name(MC_RPCCMD_GET_ENT, MC_FUNC_RENAME, NULL, NULL);
}

int mcChangeThreadPriority_001a18c0(int level)
{
    int pre;
    (void)level; /* Historical and target quirk: the argument is never sent. */
    if (!g_mclib_inited || g_mc_type == MC_TYPE_MC)
        return -1;
    pre = (g_current_cmd != MC_FUNC_NONE) ? (int)g_current_cmd : 0;
    if (pre) return pre;
    return mc_call_desc(MC_RPCCMD_CHG_PRITY, MC_FUNC_CHG_PRITY, NULL, NULL);
}

int mcSync_001a1974(int mode, int *cmd, int *result)
{
    int executing;
    int i;

    if (g_current_cmd == MC_FUNC_NONE)
        return -1;

    executing = SifCheckStatRpc_001a1af4(&g_cdata);
    if (mode == 0) {
        while (SifCheckStatRpc_001a1af4(&g_cdata))
            for (i = 0; i < 100000; ++i) { }
        executing = 0;
    }

    if (cmd)
        *cmd = (int)g_current_cmd;
    if (executing == 1)
        return 0;

    g_current_cmd = MC_FUNC_NONE;
    if (result)
        *result = *(int32_t *)g_rdata;
    return 1;
}

/* Target: 0x001a1a54.  ASCII to the two-byte SJIS pattern used by icon titles. */
int strcpy_sjis_001a1a54(uint16_t *dst, const char *src)
{
    int i;
    int len = (int)strlen(src);

    for (i = 0; i < len; ++i) {
        int c = (signed char)src[i];
        uint16_t sjis;
        if (c < 0x61)
            sjis = (uint16_t)(((c + 0x1f) << 8) | 0x82);
        else
            sjis = (uint16_t)(((c + 0x20) << 8) | 0x82);
        dst[i] = sjis;
    }
    dst[len] = 0;
    return len;
}

/* Target: 0x001a1af4; pulled by libmc and used by mcSync. */
int SifCheckStatRpc_001a1af4(void *client_raw)
{
    mc_rpc_client32 *client = (mc_rpc_client32 *)client_raw;
    if (client->packet == 0)
        return 0;

    /* hdr.mode is compared with the packet's rpc id/status bit in the target.
       The compact model keeps the observable busy/not-busy result. */
    {
        const uint32_t *packet = (const uint32_t *)(uintptr_t)client->packet;
        return (packet[6] == client->rpc_id) ? (int)(packet[4] & 1u) : 0;
    }
}
