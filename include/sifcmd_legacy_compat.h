#ifndef SNESSTATION_SIFCMD_LEGACY_COMPAT_H
#define SNESSTATION_SIFCMD_LEGACY_COMPAT_H

/*
 * Minimal ABI surface for the 2004 PS2LIB EE sifcmd.c source recovered from
 * ps2dev/ps2sdk@a80df908256955382f102278400b5d713552dbce.
 *
 * Keep these declarations aligned with the historical tamtypes.h, sifdma.h,
 * sifcmd.h and kernel.h consumed by the byte-exact proof.
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long int u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long int s64;

#if defined(__GNUC__)
typedef unsigned int u128 __attribute__((mode(TI)));
#else
typedef unsigned long long u128;
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define SYSTEM_CMD 0x80000000
#define SIF_DMA_INT_I 0x2
#define SIF_DMA_INT_O 0x4

#define DI DIntr
#define EI EIntr
#define EE_SYNC() __asm__ volatile ("sync")
#define UNCACHED_SEG(x) ((void *)(((u32)(x)) | 0x20000000))

enum {
    SIF_REG_MAINADDR = 1,
    SIF_REG_SUBADDR,
    SIF_REG_MSFLAG,
    SIF_REG_SMFLAG,
};

static inline u32 _lw(u32 addr)
{
    return *(volatile u32 *)addr;
}

static inline void _sw(u32 val, u32 addr)
{
    *(volatile u32 *)addr = val;
}

typedef struct t_SifDmaTransfer
{
    void *src;
    void *dest;
    int size;
    int attr;
} SifDmaTransfer_t;

typedef struct t_SifCmdHeader
{
    u32 size;
    void *dest;
    int cid;
    u32 unknown;
} SifCmdHeader_t;

typedef struct t_SifCmdHandlerData
{
    void (*handler)(void *, void *);
    void *harg;
} SifCmdHandlerData_t;

typedef void (*SifCmdHandler_t)(void *, void *);

int DIntr(void);
int EIntr(void);
int EnableDmac(int);
int DisableDmac(int);

s32 AddDmacHandler(s32, s32 (*)(s32), s32);
s32 RemoveDmacHandler(s32, s32);

void FlushCache(s32);
void SifWriteBackDCache(void *, int);

u32 SifSetDma(SifDmaTransfer_t *, s32);
u32 iSifSetDma(SifDmaTransfer_t *, s32);
void SifSetDChain(void);
void iSifSetDChain(void);
int SifSetReg(u32, int);
int SifGetReg(u32);

u32 SifSendCmd(int, void *, int, void *, void *, int);
u32 iSifSendCmd(int, void *, int, void *, void *, int);
void SifAddCmdHandler(int, void (*)(void *, void *), void *);
void SifInitCmd(void);
void SifExitCmd(void);
int SifGetSreg(int);

#endif
