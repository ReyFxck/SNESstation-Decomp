/*
 * Small PS2 EE libkernel leaves recovered from SNES Station v0.23.
 *
 * The wrappers at 0x0019ce60..0x0019cf00 are one-syscall functions in the
 * target. SifWriteBackDCache at 0x0019cf10 is the adjacent 64-byte cache-line
 * loop. Host builds use harmless fallbacks; the MIPS path documents the actual
 * EE calling convention used by the stripped binary ($3 = syscall number).
 */
#include <stdint.h>
#include <stddef.h>

#include "../../include/ps2_libkernel_recovered.h"

#if defined(__mips__)
static intptr_t ee_syscall2_recovered(int number, uintptr_t a0v, uintptr_t a1v)
{
    register intptr_t v0 __asm__("$2");
    register intptr_t v1 __asm__("$3") = number;
    register uintptr_t a0 __asm__("$4") = a0v;
    register uintptr_t a1 __asm__("$5") = a1v;
    __asm__ volatile("syscall"
                     : "=r"(v0), "+r"(v1), "+r"(a0), "+r"(a1)
                     :
                     : "memory");
    return v0;
}

static intptr_t ee_syscall3_recovered(int number, uintptr_t a0v, uintptr_t a1v, uintptr_t a2v)
{
    register intptr_t v0 __asm__("$2");
    register intptr_t v1 __asm__("$3") = number;
    register uintptr_t a0 __asm__("$4") = a0v;
    register uintptr_t a1 __asm__("$5") = a1v;
    register uintptr_t a2 __asm__("$6") = a2v;
    __asm__ volatile("syscall"
                     : "=r"(v0), "+r"(v1), "+r"(a0), "+r"(a1), "+r"(a2)
                     :
                     : "memory");
    return v0;
}

static void ee_cache_wb_inv_line_recovered(uintptr_t line)
{
    __asm__ volatile("sync\n\tcache 0x18, 0(%0)\n\tsync"
                     : : "r"(line) : "memory");
}
#else
static intptr_t ee_syscall2_recovered(int number, uintptr_t a0v, uintptr_t a1v)
{
    (void)number;
    (void)a0v;
    (void)a1v;
    return 0;
}

static intptr_t ee_syscall3_recovered(int number, uintptr_t a0v, uintptr_t a1v, uintptr_t a2v)
{
    (void)number;
    (void)a0v;
    (void)a1v;
    (void)a2v;
    return 0;
}

static void ee_cache_wb_inv_line_recovered(uintptr_t line)
{
    (void)line;
}
#endif

/* 0x0019ce60: v1=-0x34; syscall; return. */
int iWakeupThread(int thread_id)
{
    return (int)ee_syscall2_recovered(-0x34, (uintptr_t)thread_id, 0);
}

/* 0x0019ce70 */
int CreateSema(const ee_sema32 *sema)
{
    return (int)ee_syscall2_recovered(0x40, (uintptr_t)sema, 0);
}

/* 0x0019ce80 */
int DeleteSema(int sema_id)
{
    return (int)ee_syscall2_recovered(0x41, (uintptr_t)sema_id, 0);
}

/* 0x0019ce90 */
int iSignalSema(int sema_id)
{
    return (int)ee_syscall2_recovered(-0x43, (uintptr_t)sema_id, 0);
}

/* 0x0019cea0 */
int WaitSema(int sema_id)
{
    return (int)ee_syscall2_recovered(0x44, (uintptr_t)sema_id, 0);
}

/* 0x0019ceb0 */
int FlushCache(int operation)
{
    return (int)ee_syscall2_recovered(0x64, (uintptr_t)operation, 0);
}

/* 0x0019cec0 */
int iFlushCache(int operation)
{
    return (int)ee_syscall2_recovered(-0x68, (uintptr_t)operation, 0);
}

/* 0x0019ced0 */
int SifDmaStat(int id)
{
    return (int)ee_syscall2_recovered(0x76, (uintptr_t)id, 0);
}

/* 0x0019cee0 */
int SifSetDma(SifDmaTransfer32 *transfer, int count)
{
    return (int)ee_syscall2_recovered(0x77, (uintptr_t)transfer, (uintptr_t)count);
}

/* 0x0019cef0 */
void SifSetReg(uint32_t reg, uint32_t value)
{
    (void)ee_syscall2_recovered(0x79, reg, value);
}

/* 0x0019cf00 */
uint32_t SifGetReg(uint32_t reg)
{
    return (uint32_t)ee_syscall2_recovered(0x7a, reg, 0);
}

/*
 * 0x0019cf10
 * Aligns the requested interval to 64-byte cache lines and performs cache op
 * 0x18 with sync before/after each line. The target unrolls groups of eight
 * lines; this C keeps the same set/order of cache lines without the unroll.
 */
void SifWriteBackDCache(void *address, int size)
{
    uintptr_t first;
    uintptr_t last;
    uintptr_t line;

    if (size <= 0)
        return;

    first = (uintptr_t)address & ~(uintptr_t)0x3f;
    last = ((uintptr_t)address + (uintptr_t)size - 1u) & ~(uintptr_t)0x3f;
    for (line = first; line <= last; line += 0x40u)
        ee_cache_wb_inv_line_recovered(line);
}

/* 0x0019f5d0: v1=0x6b; syscall; return. */
void SifStopDma(void)
{
    (void)ee_syscall2_recovered(0x6b, 0, 0);
}

/* 0x0019f5a0: AddDmacHandler, syscall 0x12. */
int AddDmacHandler(int channel, void *handler, int arg)
{
    return (int)ee_syscall3_recovered(0x12, (uintptr_t)channel,
                                      (uintptr_t)handler, (uintptr_t)arg);
}

/* 0x0019f5b0: RemoveDmacHandler, syscall 0x13. */
int RemoveDmacHandler(int channel, int handler_id)
{
    return (int)ee_syscall2_recovered(0x13, (uintptr_t)channel,
                                      (uintptr_t)handler_id);
}

/* 0x0019f5e0: interrupt-context SIF DMA syscall -0x77. */
int iSifSetDma(SifDmaTransfer32 *transfer, int count)
{
    return (int)ee_syscall2_recovered(-0x77, (uintptr_t)transfer,
                                      (uintptr_t)count);
}

/* 0x0019f5f0: SifSetDChain, syscall 0x78. */
void SifSetDChain(void)
{
    (void)ee_syscall2_recovered(0x78, 0, 0);
}

/* 0x0019fd10: interrupt-context SIF receive-chain syscall -0x78. */
void iSifSetDChain_0019fd10(void)
{
    (void)ee_syscall2_recovered(-0x78, 0, 0);
}

/* 0x0019fcd0: raw _EnableDmac syscall 0x16. */
int _EnableDmac(int channel)
{
    return (int)ee_syscall2_recovered(0x16, (uintptr_t)channel, 0);
}

/* 0x0019fce0: raw _DisableDmac syscall 0x17. */
int _DisableDmac(int channel)
{
    return (int)ee_syscall2_recovered(0x17, (uintptr_t)channel, 0);
}

/* 0x0019fcf0: SignalSema syscall 0x42. */
int SignalSema(int sema_id)
{
    return (int)ee_syscall2_recovered(0x42, (uintptr_t)sema_id, 0);
}

/* 0x0019fd00: PollSema syscall 0x45. */
int PollSema(int sema_id)
{
    return (int)ee_syscall2_recovered(0x45, (uintptr_t)sema_id, 0);
}
