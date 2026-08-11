/*
 * Old PS2DEV loadfile, IOP heap and IOP reset routines recovered from
 * SNES Station v0.23. Target public wrappers begin at 0x0019d600.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../../include/ps2_libkernel_recovered.h"

typedef struct {
    union { int arg_len; int result; } p;
    int modres;
    char path[252];
    char args[252];
} lf_module_load_arg_recovered;

typedef struct {
    union { ee_addr32_t ptr; int32_t result; } p;
    union { int32_t arg_len; int32_t modres; } q;
    char dummy[252];
    char args[252];
} lf_module_buffer_arg_recovered;

typedef char recovered_assert_lf_module_load_size[(sizeof(lf_module_load_arg_recovered) == 0x200) ? 1 : -1];
typedef char recovered_assert_lf_module_buffer_size[(sizeof(lf_module_buffer_arg_recovered) == 0x200) ? 1 : -1];

extern int SifCallRpc(SifRpcClientData32 *, int, int,
                      void *, int, void *, int, void (*)(void *), void *);
extern int SifLoadFileInit_recovered(void);   /* 0x0019fd20 */
extern int SifInitIopHeap_recovered(void);    /* 0x0019f9e8 */
extern void SifStopDma(void);                 /* 0x0019f5d0 */
extern uint32_t SifGetReg(uint32_t);          /* 0x0019cf00 */
extern void SifSetReg(uint32_t, uint32_t);    /* 0x0019cef0 */
extern int SifSetDma(SifDmaTransfer32 *, int); /* 0x0019cee0 */
extern void SifWriteBackDCache(void *, int);  /* 0x0019cf10 */

extern SifRpcClientData32 lf_cd_recovered;
extern SifRpcClientData32 ih_cd_recovered;

/* 0x0019f7e8 -- older five-argument _SifLoadModule variant. */
int _SifLoadModule_0019f7e8(const char *path, int arg_len, const char *args,
                            int *modres, int fno)
{
    lf_module_load_arg_recovered arg;
    if (SifLoadFileInit_recovered() < 0)
        return -0xd601;

    memset(&arg, 0, sizeof(arg));
    strncpy(arg.path, path, sizeof(arg.path));
    if (args != NULL && arg_len != 0) {
        arg.p.arg_len = arg_len > 252 ? 252 : arg_len;
        memcpy(arg.args, args, (size_t)arg.p.arg_len);
    } else {
        arg.p.arg_len = 0;
    }

    if (SifCallRpc(&lf_cd_recovered, fno, 0, &arg, (int)sizeof(arg),
                   &arg, 8, NULL, NULL) < 0)
        return -0xd613;
    if (modres != NULL)
        *modres = arg.modres;
    return arg.p.result;
}

/* 0x0019d600 */
int SifLoadModule_0019d600(const char *path, int arg_len, const char *args)
{
    return _SifLoadModule_0019f7e8(path, arg_len, args, NULL, 0);
}

/* 0x0019f8f4 */
int _SifLoadModuleBuffer_0019f8f4(void *ptr, int arg_len, const char *args,
                                  int *modres)
{
    lf_module_buffer_arg_recovered arg;
    if (SifLoadFileInit_recovered() < 0)
        return -0xd601;

    memset(&arg, 0, sizeof(arg));
    arg.p.ptr = ee_addr32_from_ptr(ptr);
    if (args != NULL && arg_len != 0) {
        int n = arg_len > 252 ? 252 : arg_len;
        memcpy(arg.args, args, (size_t)n);
        /* Target stores argument length at request +0x04. */
        arg.q.arg_len = n;
    }

    if (SifCallRpc(&lf_cd_recovered, 6, 0, &arg, (int)sizeof(arg),
                   &arg, 8, NULL, NULL) < 0)
        return -0xd613;
    if (modres != NULL)
        *modres = arg.q.modres;
    return arg.p.result;
}

/* 0x0019d620 */
int SifLoadModuleBuffer_0019d620(void *ptr, int arg_len, const char *args)
{
    return _SifLoadModuleBuffer_0019f8f4(ptr, arg_len, args, NULL);
}

/* 0x0019d63c */
void *SifAllocIopHeap_0019d63c(int size)
{
    union { int size; uint32_t addr; } arg;
    if (SifInitIopHeap_recovered() < 0)
        return NULL;
    arg.size = size;
    if (SifCallRpc(&ih_cd_recovered, 1, 0, &arg, 4, &arg, 4,
                   NULL, NULL) < 0)
        return NULL;
    return (void *)(uintptr_t)arg.addr;
}

/* 0x0019d6b8 */
int SifFreeIopHeap_0019d6b8(void *addr)
{
    union { ee_addr32_t addr; int32_t result; } arg;
    if (SifInitIopHeap_recovered() < 0)
        return -0xd601;
    arg.addr = ee_addr32_from_ptr(addr);
    if (SifCallRpc(&ih_cd_recovered, 2, 0, &arg, 4, &arg, 4,
                   NULL, NULL) < 0)
        return -0xd613;
    return arg.result;
}

/* 0x0019d740 -- target is the old SifIopReset packet path.
 * Unlike a later 2003 PS2SDK revision, this binary does not increment an
 * _iop_reboot_count before SifStopDma; that later-source behavior is omitted.
 */
int SifIopReset_0019d740(const char *arg_string, int mode)
{
    struct {
        uint32_t header[4];
        int arglen;
        int mode;
        char arg[80];
        uint8_t trailing_pad[8];
    } reset_pkt;
    SifDmaTransfer32 dmat;
    int arglen = 0;
    typedef char recovered_assert_iop_reset_packet_size[(sizeof(reset_pkt) == 0x70) ? 1 : -1];
    (void)sizeof(recovered_assert_iop_reset_packet_size);

    SifStopDma();
    memset(&reset_pkt, 0, sizeof(reset_pkt));
    reset_pkt.header[0] = 0x70;
    reset_pkt.header[2] = 0x80000003u;
    reset_pkt.mode = mode;

    if (arg_string != NULL) {
        size_t len = strlen(arg_string);
        if (len > 80) len = 80;
        strncpy(reset_pkt.arg, arg_string, len);
        arglen = (int)len;
    }
    reset_pkt.arglen = arglen;

    dmat.src = ee_addr32_from_ptr(&reset_pkt);
    dmat.dest = SifGetReg(2);
    dmat.size = 0x70;
    dmat.attr = 0x44;
    SifWriteBackDCache(&reset_pkt, 0x70);

    if (!SifSetDma(&dmat, 1))
        return 0;

    SifSetReg(4, 0x10000u);
    SifSetReg(4, 0x20000u);
    SifSetReg(0x80000002u, 0);
    SifSetReg(0x80000000u, 0);
    return 1;
}
