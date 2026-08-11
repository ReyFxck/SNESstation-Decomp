#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "../../include/ps2_libkernel_recovered.h"

/*
 * Old EE SIF command layer recovered at 0x0019f138..0x0019f590.
 *
 * The target predates later PS2SDK reboot-count handling.  The cmd_data
 * initializer below is copied from the target's data image at 0x00425a88,
 * so offsets and embedded target addresses stay explicit even on a 64-bit
 * host used only for syntax/behaviour checks.
 */

typedef struct SifCmdHeader32 {
    uint32_t size;
    ee_addr32_t dest;
    uint32_t cid;
    uint32_t unknown;
} SifCmdHeader32;

typedef struct SifCmdHandlerData32 {
    ee_addr32_t handler;
    ee_addr32_t harg;
} SifCmdHandlerData32;

typedef struct SifCmdData32 {
    ee_addr32_t pktbuf;             /* +0x00 */
    ee_addr32_t unused;             /* +0x04 */
    ee_addr32_t iopbuf;             /* +0x08 */
    ee_addr32_t sys_cmd_handlers;   /* +0x0c */
    uint32_t nr_sys_handlers;       /* +0x10 */
    ee_addr32_t usr_cmd_handlers;   /* +0x14 */
    uint32_t nr_usr_handlers;       /* +0x18 */
    ee_addr32_t sregs;              /* +0x1c */
} SifCmdData32;

typedef char recovered_assert_sifcmd_header_size[(sizeof(SifCmdHeader32) == 0x10) ? 1 : -1];
typedef char recovered_assert_sifcmd_handler_size[(sizeof(SifCmdHandlerData32) == 0x08) ? 1 : -1];
typedef char recovered_assert_sifcmd_data_size[(sizeof(SifCmdData32) == 0x20) ? 1 : -1];

static SifCmdData32 sif_cmd_data_00425a88 = {
    UINT32_C(0x00445180),
    UINT32_C(0x00445200),
    0,
    UINT32_C(0x00445240),
    32,
    UINT32_C(0x00445340),
    32,
    UINT32_C(0x00445440)
};
static int sif_cmd_init_00425aa8;
static int sif0_handler_id_00425aac = -1;

#if !defined(__mips__)
/* Host-only backing for the contiguous target BSS region 0x445180..0x4454bf. */
static unsigned char sifcmd_host_bss[0x340];
#endif

static void *sifcmd_target_ptr(ee_addr32_t address)
{
#if defined(__mips__)
    return ee_ptr_from_addr32(address);
#else
    ee_addr32_t physical = address & UINT32_C(0x1fffffff);
    if (physical >= UINT32_C(0x00445180) &&
        physical < UINT32_C(0x004454c0))
        return &sifcmd_host_bss[physical - UINT32_C(0x00445180)];
    return NULL;
#endif
}

extern void SifWriteBackDCache(void *address, int size);
extern int SifSetDma(SifDmaTransfer32 *transfer, int count);
extern int iSifSetDma(SifDmaTransfer32 *transfer, int count);
extern void SifSetDChain(void);
extern uint32_t SifGetReg(uint32_t reg);
extern void SifSetReg(uint32_t reg, uint32_t value);
extern int FlushCache(int operation);
extern int DIntr(void);
extern int EIntr(void);
extern int AddDmacHandler(int channel, void *handler, int arg);
extern int RemoveDmacHandler(int channel, int handler_id);

/* Forward declarations for later target functions used by SifInitCmd. */
int EnableDmac_0019fb00(int channel);
int DisableDmac_0019fb78(int channel);
int _SifCmdIntHandler_0019fbf0(void);

/* Target: 0x0019f138. */
static uint32_t _SifSendCmd_0019f138(int cid, int mode, void *packet,
                                    uint32_t packet_size, void *src,
                                    void *dest, int size)
{
    SifDmaTransfer32 dmat[2];
    SifCmdHeader32 *header = (SifCmdHeader32 *)packet;
    int count = 0;

    packet_size &= 0xffu;
    if (packet_size > 112u)
        return 0;

    header->cid = (uint32_t)cid;
    header->size = packet_size;
    header->dest = 0;

    if (size > 0) {
        header->size = packet_size | ((uint32_t)size << 8);
        header->dest = ee_addr32_from_ptr(dest);

        if ((mode & 4) != 0)
            SifWriteBackDCache(src, size);

        dmat[count].src = ee_addr32_from_ptr(src);
        dmat[count].dest = ee_addr32_from_ptr(dest);
        dmat[count].size = size;
        dmat[count].attr = 0;
        count++;
    }

    dmat[count].src = ee_addr32_from_ptr(packet);
    dmat[count].dest = sif_cmd_data_00425a88.iopbuf;
    dmat[count].size = (int32_t)packet_size;
    dmat[count].attr = 0x44;
    count++;

    SifWriteBackDCache(packet, (int)packet_size);

    if ((mode & 1) != 0)
        return (uint32_t)iSifSetDma(dmat, count);
    return (uint32_t)SifSetDma(dmat, count);
}

/* Target: 0x0019f264. */
uint32_t SifSendCmd_0019f264(int cid, void *packet, int packet_size,
                             void *src, void *dest, int size)
{
    return _SifSendCmd_0019f138(cid, 0, packet, (uint32_t)packet_size,
                                src, dest, size);
}

/* Target: 0x0019f2a0. */
uint32_t iSifSendCmd_0019f2a0(int cid, void *packet, int packet_size,
                              void *src, void *dest, int size)
{
    return _SifSendCmd_0019f138(cid, 1, packet, (uint32_t)packet_size,
                                src, dest, size);
}

/* Target static handler: 0x0019f2dc. */
static void change_addr_0019f2dc(const void *packet, SifCmdData32 *data)
{
    const uint32_t *words = (const uint32_t *)packet;
    data->iopbuf = words[4];
}

/* Target static handler: 0x0019f2e8. */
static void set_sreg_0019f2e8(const void *packet, SifCmdData32 *data)
{
    const uint32_t *words = (const uint32_t *)packet;
    uint32_t *sregs = (uint32_t *)sifcmd_target_ptr(data->sregs);
    if (sregs != NULL)
        sregs[words[4]] = words[5];
}

/* Target: 0x0019f304. */
void SifInitCmd_0019f304(void)
{
    uint32_t packet[5] = {0, 0, 0, 0, 0};
    SifCmdHandlerData32 *sys_handlers;
    uint32_t *sregs;
    unsigned i;

    if (sif_cmd_init_00425aa8)
        return;

    (void)DIntr();

    sif_cmd_data_00425a88.pktbuf |= UINT32_C(0x20000000);
    sif_cmd_data_00425a88.unused |= UINT32_C(0x20000000);

    sys_handlers = (SifCmdHandlerData32 *)
        sifcmd_target_ptr(sif_cmd_data_00425a88.sys_cmd_handlers);
    if (sys_handlers != NULL) {
        for (i = 0; i < 32; i++) {
            sys_handlers[i].handler = 0;
            sys_handlers[i].harg = 0;
        }
    }

    sregs = (uint32_t *)sifcmd_target_ptr(sif_cmd_data_00425a88.sregs);
    if (sregs != NULL) {
        for (i = 0; i < 32; i++)
            sregs[i] = 0;
    }

    if (sys_handlers != NULL) {
        /* The binary stores target VAs, not relocated host function pointers. */
        sys_handlers[0].handler = UINT32_C(0x0019f2dc);
        sys_handlers[0].harg = UINT32_C(0x00425a88);
        sys_handlers[1].handler = UINT32_C(0x0019f2e8);
        sys_handlers[1].harg = UINT32_C(0x00425a88);
    }

    (void)EIntr();
    (void)FlushCache(0);

#if defined(__mips__)
    if ((*(volatile uint32_t *)UINT32_C(0x1000e010) & UINT32_C(0x20)) != 0u)
        *(volatile uint32_t *)UINT32_C(0x1000e010) = UINT32_C(0x20);

    if ((*(volatile uint32_t *)UINT32_C(0x1000c000) & UINT32_C(0x100)) == 0u)
        SifSetDChain();
#endif

    sif0_handler_id_00425aac =
        AddDmacHandler(5, (void *)(uintptr_t)UINT32_C(0x0019fbf0), 0);
    (void)EnableDmac_0019fb00(5);
    sif_cmd_init_00425aa8 = 1;

#if defined(__mips__)
    sif_cmd_data_00425a88.iopbuf = SifGetReg(UINT32_C(0x80000000));
    if (sif_cmd_data_00425a88.iopbuf != 0u) {
        packet[4] = sif_cmd_data_00425a88.pktbuf;
        (void)SifSendCmd_0019f264((int)UINT32_C(0x80000000), packet,
                                  (int)sizeof(packet), NULL, NULL, 0);
    } else {
        while ((SifGetReg(4) & UINT32_C(0x00020000)) == 0u) {
        }
        sif_cmd_data_00425a88.iopbuf = SifGetReg(2);
        SifSetReg(UINT32_C(0x80000000), sif_cmd_data_00425a88.iopbuf);
        SifSetReg(UINT32_C(0x80000001), UINT32_C(0x00425a88));
        packet[3] = 0;
        packet[4] = sif_cmd_data_00425a88.pktbuf;
        (void)SifSendCmd_0019f264((int)UINT32_C(0x80000002), packet,
                                  (int)sizeof(packet), NULL, NULL, 0);
    }
#else
    (void)packet;
#endif
}

/* Target: 0x0019f510. */
void SifExitCmd_0019f510(void)
{
    (void)DisableDmac_0019fb78(5);
    (void)RemoveDmacHandler(5, sif0_handler_id_00425aac);
    sif_cmd_init_00425aa8 = 0;
}

/* Target: 0x0019f544. */
void SifAddCmdHandler_0019f544(int cid, void *handler, void *harg)
{
    uint32_t id = (uint32_t)cid & UINT32_C(0x7fffffff);
    ee_addr32_t table_addr = (cid < 0) ? sif_cmd_data_00425a88.sys_cmd_handlers
                                       : sif_cmd_data_00425a88.usr_cmd_handlers;
    SifCmdHandlerData32 *handlers =
        (SifCmdHandlerData32 *)sifcmd_target_ptr(table_addr);

    if (handlers != NULL) {
        handlers[id].handler = ee_addr32_from_ptr(handler);
        handlers[id].harg = ee_addr32_from_ptr(harg);
    }
}

/* Target: 0x0019f57c. */
int SifGetSreg_0019f57c(int sreg)
{
    int32_t *sregs = (int32_t *)sifcmd_target_ptr(sif_cmd_data_00425a88.sregs);
    return sregs != NULL ? sregs[sreg] : 0;
}

/*
 * Host-only dispatch helpers keep the two built-in handlers behaviour-testable
 * without pretending that arbitrary EE function addresses are host-callable.
 */
void sifcmd_recovered_test_change_addr(const void *packet)
{
    change_addr_0019f2dc(packet, &sif_cmd_data_00425a88);
}

void sifcmd_recovered_test_set_sreg(const void *packet)
{
    set_sreg_0019f2e8(packet, &sif_cmd_data_00425a88);
}

extern int _EnableDmac(int channel);
extern int _DisableDmac(int channel);

/* 0x0019fb00: interrupt-safe wrapper around syscall 0x16. */
int EnableDmac_0019fb00(int channel)
{
    int restore = 0;
    int result;

#if defined(__mips__)
    uint32_t status;
    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    restore = (status & UINT32_C(0x00010000)) != 0u;
#endif
    if (restore)
        (void)DIntr();

    result = _EnableDmac(channel);
#if defined(__mips__)
    __asm__ volatile("sync" ::: "memory");
#endif
    if (restore)
        (void)EIntr();
    return result;
}

/* 0x0019fb78: interrupt-safe wrapper around syscall 0x17. */
int DisableDmac_0019fb78(int channel)
{
    int restore = 0;
    int result;

#if defined(__mips__)
    uint32_t status;
    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    restore = (status & UINT32_C(0x00010000)) != 0u;
#endif
    if (restore)
        (void)DIntr();

    result = _DisableDmac(channel);
#if defined(__mips__)
    __asm__ volatile("sync" ::: "memory");
#endif
    if (restore)
        (void)EIntr();
    return result;
}

extern void iSifSetDChain_0019fd10(void);

/* Target interrupt handler: 0x0019fbf0. */
int _SifCmdIntHandler_0019fbf0(void)
{
    unsigned char packet[128];
    SifCmdHeader32 *incoming =
        (SifCmdHeader32 *)sifcmd_target_ptr(sif_cmd_data_00425a88.pktbuf);
    SifCmdHeader32 *local;
    SifCmdHandlerData32 *handlers;
    uint32_t size;
    uint32_t quads;
    uint32_t id;

    (void)EIntr();

    if (incoming == NULL)
        goto out;

    size = incoming->size & 0xffu;
    if (size == 0u)
        goto out;

    quads = (size + 30u) >> 4;
    incoming->size = 0;
    if (quads != 0u)
        memcpy(packet, incoming, (size_t)quads * 16u);

    iSifSetDChain_0019fd10();

    local = (SifCmdHeader32 *)packet;
    id = local->cid & UINT32_C(0x7fffffff);
    if (id >= 32u)
        goto out;

    handlers = (SifCmdHandlerData32 *)sifcmd_target_ptr(
        ((int32_t)local->cid < 0) ? sif_cmd_data_00425a88.sys_cmd_handlers
                                  : sif_cmd_data_00425a88.usr_cmd_handlers);
    if (handlers != NULL && handlers[id].handler != 0u) {
#if defined(__mips__)
        typedef void (*handler_fn)(void *, void *);
        handler_fn fn = (handler_fn)(uintptr_t)handlers[id].handler;
        fn(packet, ee_ptr_from_addr32(handlers[id].harg));
#else
        if (handlers[id].handler == UINT32_C(0x0019f2dc))
            change_addr_0019f2dc(packet, &sif_cmd_data_00425a88);
        else if (handlers[id].handler == UINT32_C(0x0019f2e8))
            set_sreg_0019f2e8(packet, &sif_cmd_data_00425a88);
#endif
    }

out:
#if defined(__mips__)
    __asm__ volatile("sync" ::: "memory");
#endif
    (void)EIntr();
    return 0;
}
