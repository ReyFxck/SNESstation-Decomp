/*
 * SNES Station v0.23 IOP reset source-model remainder.
 *
 * The historical LOADFILE and IOP heap clients were split into their exact
 * source translation units.  This file now owns only the target's old
 * SifIopReset packet path at 0x0019d740.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../../include/ps2_libkernel_recovered.h"

extern void SifStopDma(void);                  /* 0x0019f5d0 */
extern uint32_t SifGetReg(uint32_t);           /* 0x0019cf00 */
extern void SifSetReg(uint32_t, uint32_t);     /* 0x0019cef0 */
extern int SifSetDma(SifDmaTransfer32 *, int); /* 0x0019cee0 */
extern void SifWriteBackDCache(void *, int);   /* 0x0019cf10 */

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
