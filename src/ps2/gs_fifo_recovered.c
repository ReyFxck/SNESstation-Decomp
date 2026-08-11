/*
 * SNES Station v0.23 / GSLIB gsPipe core FIFO methods.
 *
 * Target range represented here:
 *   0x00199830 getPipeSize
 *   0x00199838 InitPipe
 *   0x00199898 getBytesLeft
 *   0x001998b8 FlushCheck
 *   0x001998f8 Flush
 *   0x00199970 FlushInt
 *   0x00199dc0 setDrawFrame
 *
 * The target strings and 0x34-byte object layout identify this object as
 * Hiryu's gsPipe.  EE pointers remain explicit 32-bit addresses.
 */
#include <stdint.h>
#include <string.h>

#include "../../include/gslib_recovered.h"

static uint64_t ee_load64(uint32_t address)
{
    uint64_t value;
    memcpy(&value, (const void *)(uintptr_t)address, sizeof(value));
    return value;
}

static void ee_store64(uint32_t address, uint64_t value)
{
    memcpy((void *)(uintptr_t)address, &value, sizeof(value));
}

/* Low-level PS2 helpers; names follow independently proven hardware effects. */
extern void Dma02Wait_0019be40(void);
extern void SendDma02_0019be20(uint32_t chain_address);
extern void FlushCache_0019ceb0(int mode);
extern void iFlushCache_0019cec0(int mode);

uint32_t gsPipe_getPipeSize_00199830(const gsPipeRecovered *self)
{
    return self->m_MemSize;
}

/* C++ member method: `this` is unused; dma_addr is the second ABI argument. */
void gsPipe_InitPipe_00199838(gsPipeRecovered *self, uint32_t dma_addr)
{
    (void)self;
    ee_store64(dma_addr + 0u, UINT64_C(0x0000000070000000));
    ee_store64(dma_addr + 8u, UINT64_C(0));
}

uint32_t gsPipe_getBytesLeft_00199898(const gsPipeRecovered *self)
{
    return self->m_CurrentPipe + (self->m_MemSize >> 1) - self->m_CurrentGifTag;
}

void gsPipe_FlushCheck_001998b8(gsPipeRecovered *self)
{
    /* GSPIPE_MINSPACE=18 dwords; source and binary both reduce this to 0x90 bytes. */
    if (gsPipe_getBytesLeft_00199898(self) < 0x90u)
        gsPipe_Flush_001998f8(self);
}

static void select_other_pipe(gsPipeRecovered *self)
{
    const uint32_t next = (self->m_CurrentPipe == self->m_DmaPipe1)
                        ? self->m_DmaPipe2 : self->m_DmaPipe1;

    gsPipe_InitPipe_00199838(self, next);
    self->m_CurrentPipe = next;
    self->m_CurrentDmaAddr = next;
    self->m_CurrentGifTag = next + 0x10u;
}

void gsPipe_Flush_001998f8(gsPipeRecovered *self)
{
    Dma02Wait_0019be40();
    FlushCache_0019ceb0(0);
    SendDma02_0019be20(self->m_CurrentPipe);
    select_other_pipe(self);
}

void gsPipe_FlushInt_00199970(gsPipeRecovered *self)
{
    Dma02Wait_0019be40();
    iFlushCache_0019cec0(0);
    SendDma02_0019be20(self->m_CurrentPipe);
    select_other_pipe(self);
}

void gsPipe_setDrawFrame_00199dc0(gsPipeRecovered *self, uint32_t base,
                                  uint32_t width, int psm, uint32_t mask)
{
    uint64_t dt = ee_load64(self->m_CurrentDmaAddr);
    dt = (dt & UINT64_C(0xffffffffffff0000)) | (uint16_t)((uint16_t)dt + 2u);
    ee_store64(self->m_CurrentDmaAddr, dt);

    const uint64_t frame =
        ((uint64_t)base >> 13) |
        ((uint64_t)(width / 64u) << 16) |
        ((uint64_t)(uint32_t)psm << 24) |
        ((uint64_t)mask << 32);

    const uint32_t p = self->m_CurrentGifTag;
    ee_store64(p + 0x00u, UINT64_C(0x1000000000008001));
    ee_store64(p + 0x08u, UINT64_C(0xfffffffffffffffe));
    ee_store64(p + 0x10u, frame);
    ee_store64(p + 0x18u, UINT64_C(0x4c));
    self->m_CurrentGifTag = p + 0x20u;

    gsPipe_FlushCheck_001998b8(self);
}
