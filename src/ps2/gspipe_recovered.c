/*
 * Recovered Hiryu GSLIB gsPipe implementation embedded in SNES Station v0.23.
 * Target corridor: 0x00199480..0x0019b7ec.
 *
 * Identification came from target evidence first: the executable contains the
 * original gsPipe allocation/alignment diagnostic strings, and the object is
 * exactly 0x34 bytes. Historical GSLIB is then used to validate names and
 * structure. The code below preserves target-visible quirks rather than
 * silently modernising them (notably operator= omissions and TextureSet XOR).
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../../include/gslib_recovered.h"

#define GS_ENABLE  1
#define GS_DISABLE 0
#define GS_FILTER_NEAREST 0

#define GS_PSMCT32  0x00
#define GS_PSMCT24  0x01
#define GS_PSMCT16  0x02
#define GS_PSMCT16S 0x0a
#define GS_PSMT8    0x13
#define GS_PSMT4    0x14

#define GS_REG_TEX0_1    0x06u
#define GS_REG_CLAMP_1   0x08u
#define GS_REG_TEX1_1    0x14u
#define GS_REG_XYOFFSET_1 0x18u
#define GS_REG_PRMODECONT 0x1au
#define GS_REG_TEXCLUT   0x1cu
#define GS_REG_TEXA      0x3bu
#define GS_REG_TEXFLUSH  0x3fu
#define GS_REG_SCISSOR_1 0x40u
#define GS_REG_ALPHA_1   0x42u
#define GS_REG_DTHE      0x45u
#define GS_REG_COLCLAMP  0x46u
#define GS_REG_TEST_1    0x47u
#define GS_REG_PABE      0x49u
#define GS_REG_ZBUF_1    0x4eu
#define GS_REG_BITBLTBUF 0x50u
#define GS_REG_TRXPOS    0x51u
#define GS_REG_TRXREG    0x52u
#define GS_REG_TRXDIR    0x53u

#define GS_PRIM_POINT          0u
#define GS_PRIM_LINE           1u
#define GS_PRIM_LINESTRIP      2u
#define GS_PRIM_TRIANGLE       3u
#define GS_PRIM_TRIANGLE_STRIP 4u
#define GS_PRIM_SPRITE         6u

#define IMAGE_MAX_QWORD 0x7ff0

/* Target helpers identified by their allocator/free behavior. */
extern void *memalign_like_0019e698(unsigned alignment, unsigned size);
extern void free_like_0019e784(void *ptr);
extern void *memcpy_like_0019c364(void *dst, const void *src, size_t size);

static int hw_AlphaEnabled;
static int hw_ZBufferEnabled;
static int hw_ZTestEnabled;
static int hw_OriginX;
static int hw_OriginY;

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

static void packet_store(const gsPipeRecovered *self, unsigned qword, uint64_t value)
{
    ee_store64(self->m_CurrentGifTag + qword * 8u, value);
}

static void packet_advance(gsPipeRecovered *self, unsigned qwords)
{
    self->m_CurrentGifTag += qwords * 8u;
}

static void dma_add_qwc(gsPipeRecovered *self, unsigned qwords)
{
    uint64_t dt = ee_load64(self->m_CurrentDmaAddr);
    dt = (dt & UINT64_C(0xffffffffffff0000)) |
         (uint16_t)((uint16_t)dt + (uint16_t)qwords);
    ee_store64(self->m_CurrentDmaAddr, dt);
}

static uint64_t gs_gif_tag(unsigned nloop, unsigned pre, unsigned nreg)
{
    return (uint64_t)(nloop & 0x7fffu) |
           UINT64_C(0x8000) |
           ((uint64_t)(pre & 1u) << 46) |
           ((uint64_t)(nreg & 0xfu) << 60);
}

static uint64_t gs_prim(unsigned prim, unsigned iip, unsigned tme,
                        unsigned abe, unsigned fst)
{
    return (uint64_t)prim |
           ((uint64_t)iip << 3) |
           ((uint64_t)tme << 4) |
           ((uint64_t)abe << 6) |
           ((uint64_t)fst << 8);
}

static uint64_t gs_colq(uint32_t colour)
{
    return UINT64_C(0x3f80000000000000) | colour;
}

static uint64_t gs_xyz(int x, int y, uint32_t z)
{
    const uint32_t xx = (uint32_t)(x << 4);
    const uint32_t yy = (uint32_t)(y << 4);
    return (uint64_t)xx | ((uint64_t)yy << 16) | ((uint64_t)z << 32);
}

static uint64_t gs_uv(uint32_t u, uint32_t v)
{
    return (uint64_t)(u << 4) | ((uint64_t)(v << 4) << 16);
}

static uint64_t gs_xyoffset(int x, int y)
{
    return (uint64_t)(uint32_t)(x << 4) |
           ((uint64_t)(uint32_t)(y << 4) << 32);
}

static uint64_t gs_scissor(int32_t x1, int32_t x2, int32_t y1, int32_t y2)
{
    return (uint64_t)(uint32_t)x1 |
           ((uint64_t)(uint32_t)x2 << 16) |
           ((uint64_t)(uint32_t)y1 << 32) |
           ((uint64_t)(uint32_t)y2 << 48);
}

static uint64_t gs_zbuf(uint32_t base, int psm, uint32_t mask)
{
    return ((uint64_t)base >> 13) |
           ((uint64_t)(uint32_t)psm << 24) |
           ((uint64_t)mask << 32);
}

static uint64_t gs_bitbltbuf(uint32_t dbp, int dbw, int psm)
{
    return ((uint64_t)((dbp / 256u) & 0x3fffu) << 32) |
           ((uint64_t)((uint32_t)(dbw / 64) & 0x3fu) << 48) |
           ((uint64_t)((uint32_t)psm & 0x3fu) << 56);
}

static uint64_t gs_trxpos(int x, int y)
{
    return ((uint64_t)(uint32_t)x << 32) | ((uint64_t)(uint32_t)y << 48);
}

static uint64_t gs_trxreg(int w, int h)
{
    return (uint64_t)(uint32_t)w | ((uint64_t)(uint32_t)h << 32);
}

static unsigned texture_qwords(int pixels, int psm)
{
    switch (psm) {
    case GS_PSMCT32:  return (unsigned)(pixels >> 2) + ((pixels & 3) != 0);
    case GS_PSMCT24:  return (unsigned)(pixels / 3) + ((pixels & 2) != 0);
    case GS_PSMCT16:
    case GS_PSMCT16S: return (unsigned)(pixels >> 3) + ((pixels & 7) != 0);
    case GS_PSMT8:    return (unsigned)(pixels >> 4) + ((pixels & 15) != 0);
    case GS_PSMT4:    return (unsigned)(pixels >> 5) + ((pixels & 31) != 0);
    default:           return 0;
    }
}

static void gsPipe_ctor_body(gsPipeRecovered *self, uint32_t size)
{
    self->m_MemSize = 0;
    if (size < 0x1000u) {
        printf("Requested size for gsPipe buffer was less than 0x1000 !\n");
        return;
    }

    if (self->m_Buffer != 0)
        free_like_0019e784((void *)(uintptr_t)self->m_Buffer);

    self->m_Buffer = (uint32_t)(uintptr_t)memalign_like_0019e698(64u, size);
    if (self->m_Buffer == 0) {
        printf("gsPipe buffer could not be allocated !\n");
        return;
    }
    if (self->m_Buffer & 0x0fu) {
        printf("gsPipe buffer is not aligned !\n");
        return;
    }

    self->m_DmaPipe1 = self->m_Buffer;
    /* Target uses -mlong64: (size >> 4) unsigned-long elements = size/2 bytes. */
    self->m_DmaPipe2 = self->m_DmaPipe1 + (size >> 1);
    self->m_MemSize = size;

    gsPipe_InitPipe_00199838(self, self->m_DmaPipe1);
    self->m_CurrentPipe = self->m_DmaPipe1;
    self->m_CurrentDmaAddr = self->m_DmaPipe1;
    self->m_CurrentGifTag = self->m_DmaPipe1 + 0x10u;

    self->m_OriginX = hw_OriginX;
    self->m_OriginY = hw_OriginY;
    gsPipe_setAlphaEnable_00199b80(self, GS_ENABLE);
    gsPipe_setZTestEnable_00199aa8(self, GS_DISABLE);
    gsPipe_setFilterMethod_0019a580(self, GS_FILTER_NEAREST);
}

void gsPipe_ctor_00199480(gsPipeRecovered *self, uint32_t size)
{
    gsPipe_ctor_body(self, size);
}

void gsPipe_ctor_00199590(gsPipeRecovered *self, uint32_t size)
{
    gsPipe_ctor_body(self, size);
}

static void gsPipe_dtor_body(gsPipeRecovered *self)
{
    if (self->m_Buffer != 0)
        free_like_0019e784((void *)(uintptr_t)self->m_Buffer);
}

void gsPipe_dtor_001996a0(gsPipeRecovered *self) { gsPipe_dtor_body(self); }
void gsPipe_dtor_001996d0(gsPipeRecovered *self) { gsPipe_dtor_body(self); }

/* 0x00199700 / 0x00199720 are the duplicated C++ copy-constructor entries. */
void gsPipe_copy_ctor_00199700(gsPipeRecovered *self, const gsPipeRecovered *src)
{
    (void)gsPipe_assign_00199740(self, src);
}

void gsPipe_copy_ctor_00199720(gsPipeRecovered *self, const gsPipeRecovered *src)
{
    (void)gsPipe_assign_00199740(self, src);
}

gsPipeRecovered *gsPipe_assign_00199740(gsPipeRecovered *self, const gsPipeRecovered *src)
{
    if (self == src)
        return self;

    /* These are exactly the state fields copied by the target.  ZTest and
       FilterMethod are intentionally omitted, matching the historical bug. */
    self->m_OriginX = src->m_OriginX;
    self->m_OriginY = src->m_OriginY;
    self->m_AlphaEnabled = src->m_AlphaEnabled;
    self->m_ZBufferEnabled = src->m_ZBufferEnabled;
    self->m_MemSize = src->m_MemSize;

    if (self->m_Buffer != 0)
        free_like_0019e784((void *)(uintptr_t)self->m_Buffer);

    self->m_Buffer = (uint32_t)(uintptr_t)memalign_like_0019e698(64u, self->m_MemSize);
    memcpy_like_0019c364((void *)(uintptr_t)self->m_Buffer,
                         (const void *)(uintptr_t)src->m_Buffer,
                         self->m_MemSize);

    self->m_DmaPipe1 = self->m_Buffer + (src->m_DmaPipe1 - src->m_Buffer);
    self->m_DmaPipe2 = self->m_Buffer + (src->m_DmaPipe2 - src->m_Buffer);
    self->m_CurrentPipe = self->m_Buffer + (src->m_CurrentPipe - src->m_Buffer);
    self->m_CurrentDmaAddr = self->m_Buffer + (src->m_CurrentDmaAddr - src->m_Buffer);
    self->m_CurrentGifTag = self->m_Buffer + (src->m_CurrentGifTag - src->m_Buffer);
    return self;
}

void gsPipe_ReInit_00199848(gsPipeRecovered *self)
{
    gsPipe_setZTestEnable_00199aa8(self, self->m_ZTestEnabled);
    gsPipe_setAlphaEnable_00199b80(self, self->m_AlphaEnabled);
    gsPipe_setOrigin_00199e58(self, self->m_OriginX, self->m_OriginY);
    gsPipe_Flush_001998f8(self);
}

void gsPipe_setZBuffer_001999e8(gsPipeRecovered *self, uint32_t base,
                                int psm, uint32_t enable)
{
    enable &= 1u;
    dma_add_qwc(self, 2);
    packet_store(self, 0, UINT64_C(0x1000000000008001));
    packet_store(self, 1, UINT64_C(0xfffffffffffffffe));
    packet_store(self, 2, gs_zbuf(base, psm, enable ^ 1u));
    packet_store(self, 3, GS_REG_ZBUF_1);
    packet_advance(self, 4);
    gsPipe_FlushCheck_001998b8(self);
    self->m_ZBufferEnabled = hw_ZBufferEnabled = (int)enable;
}

void gsPipe_setZTestEnable_00199aa8(gsPipeRecovered *self, int enable)
{
    enable &= 1;
    if (hw_ZBufferEnabled == 0 || self->m_ZBufferEnabled == 0)
        enable = 0;

    dma_add_qwc(self, 2);
    packet_store(self, 0, UINT64_C(0x1000000000008001));
    packet_store(self, 1, UINT64_C(0xfffffffffffffffe));
    packet_store(self, 2, enable ? UINT64_C(0x00070000) : UINT64_C(0x00030000));
    packet_store(self, 3, GS_REG_TEST_1);
    packet_advance(self, 4);
    gsPipe_FlushCheck_001998b8(self);
    self->m_ZTestEnabled = hw_ZTestEnabled = enable;
}

void gsPipe_setAlphaEnable_00199b80(gsPipeRecovered *self, int enable)
{
    enable &= 1;
    dma_add_qwc(self, 3);
    packet_store(self, 0, UINT64_C(0x1000000000008001));
    packet_store(self, 1, UINT64_C(0xfffffffffffffffe));
    packet_store(self, 2, enable ? UINT64_C(0x0000007f00000044) : 0);
    packet_store(self, 3, GS_REG_ALPHA_1);
    packet_store(self, 4, 0);
    packet_store(self, 5, GS_REG_PABE);
    packet_advance(self, 6);
    gsPipe_FlushCheck_001998b8(self);
    self->m_AlphaEnabled = hw_AlphaEnabled = enable;
}

static void emit_single_ad(gsPipeRecovered *self, uint64_t value, uint64_t reg)
{
    dma_add_qwc(self, 2);
    packet_store(self, 0, UINT64_C(0x1000000000008001));
    packet_store(self, 1, UINT64_C(0xfffffffffffffffe));
    packet_store(self, 2, value);
    packet_store(self, 3, reg);
    packet_advance(self, 4);
    gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_setDither_00199c58(gsPipeRecovered *self, uint32_t enable)
{
    emit_single_ad(self, enable, GS_REG_DTHE);
}

void gsPipe_setColClamp_00199cd0(gsPipeRecovered *self, uint32_t enable)
{
    emit_single_ad(self, enable, GS_REG_COLCLAMP);
}

void gsPipe_setPrModeCont_00199d48(gsPipeRecovered *self, uint32_t enable)
{
    emit_single_ad(self, enable, GS_REG_PRMODECONT);
}

void gsPipe_setOrigin_00199e58(gsPipeRecovered *self, int x, int y)
{
    self->m_OriginX = hw_OriginX = x;
    self->m_OriginY = hw_OriginY = y;
    emit_single_ad(self, gs_xyoffset(x, y), GS_REG_XYOFFSET_1);
}

void gsPipe_setScissorRect_00199ef8(gsPipeRecovered *self,
                                    int32_t x1, int32_t y1,
                                    int32_t x2, int32_t y2)
{
    emit_single_ad(self, gs_scissor(x1, x2, y1, y2), GS_REG_SCISSOR_1);
}

static void texture_transfer(gsPipeRecovered *self, uint32_t tbp, int tbw,
                             int xofs, int yofs, int psm,
                             uint8_t *tex, int width, int height,
                             int direction)
{
    uint64_t dt = ee_load64(self->m_CurrentDmaAddr);
    dt = (dt & UINT64_C(0xffffffff8fff0000)) |
         UINT64_C(0x10000000) |
         (uint16_t)((uint16_t)dt + 5u);
    ee_store64(self->m_CurrentDmaAddr, dt);

    packet_store(self, 0, UINT64_C(0x1000000000008004));
    packet_store(self, 1, UINT64_C(0xfffffffffffffffe));
    packet_store(self, 2, gs_bitbltbuf(tbp, tbw, psm));
    packet_store(self, 3, GS_REG_BITBLTBUF);
    packet_store(self, 4, gs_trxpos(xofs, yofs));
    packet_store(self, 5, GS_REG_TRXPOS);
    packet_store(self, 6, gs_trxreg(width, height));
    packet_store(self, 7, GS_REG_TRXREG);
    packet_store(self, 8, (uint64_t)(unsigned)direction);
    packet_store(self, 9, GS_REG_TRXDIR);

    self->m_CurrentDmaAddr = self->m_CurrentGifTag + 0x50u;
    self->m_CurrentGifTag = self->m_CurrentDmaAddr + 0x10u;
    gsPipe_FlushCheck_001998b8(self);

    unsigned numq = texture_qwords(width * height, psm);
    while (numq != 0) {
        const unsigned currq = numq > IMAGE_MAX_QWORD ? IMAGE_MAX_QWORD : numq;

        ee_store64(self->m_CurrentDmaAddr, UINT64_C(0x10000001));
        packet_store(self, 0, UINT64_C(0x0800000000000000) + currq);
        packet_store(self, 1, 0);

        self->m_CurrentDmaAddr = self->m_CurrentGifTag + 0x10u;
        const uint64_t ref_tag =
            ((uint64_t)((uint32_t)(uintptr_t)tex & 0x7fffffffu) << 32) |
            (UINT64_C(0x30000000) + currq);
        ee_store64(self->m_CurrentDmaAddr + 0u, ref_tag);
        ee_store64(self->m_CurrentDmaAddr + 8u, 0);

        self->m_CurrentDmaAddr += 0x10u;
        ee_store64(self->m_CurrentDmaAddr + 0u, UINT64_C(0x70000000));
        ee_store64(self->m_CurrentDmaAddr + 8u, 0);
        self->m_CurrentGifTag = self->m_CurrentDmaAddr + 0x10u;

        gsPipe_FlushCheck_001998b8(self);
        numq -= currq;
        tex += currq * 16u;
    }

    gsPipe_FlushCheck_001998b8(self);
    gsPipe_TextureFlush_0019a500(self);
}

void gsPipe_TextureUpload_00199f88(gsPipeRecovered *self, uint32_t tbp,
                                   int tbw, int xofs, int yofs, int psm,
                                   const uint8_t *tex, int width, int height)
{
    texture_transfer(self, tbp, tbw, xofs, yofs, psm,
                     (uint8_t *)(uintptr_t)tex, width, height, 0);
}

void gsPipe_TextureDownload_0019a240(gsPipeRecovered *self, uint32_t tbp,
                                     int tbw, int xofs, int yofs, int psm,
                                     uint8_t *tex, int width, int height)
{
    /* The target preserves the historical GSLIB implementation, including
       the same REF-chain construction as upload; only TRXDIR is changed. */
    texture_transfer(self, tbp, tbw, xofs, yofs, psm, tex, width, height, 1);
}

void gsPipe_TextureFlush_0019a500(gsPipeRecovered *self)
{
    dma_add_qwc(self, 2);
    packet_store(self, 0, UINT64_C(0x1000000000008001));
    packet_store(self, 1, UINT64_C(0xfffffffffffffffe));
    packet_store(self, 2, UINT64_C(0xbad));
    packet_store(self, 3, GS_REG_TEXFLUSH);
    packet_advance(self, 4);
    gsPipe_Flush_001998f8(self);
}

void gsPipe_setFilterMethod_0019a580(gsPipeRecovered *self, int method)
{
    self->m_FilterMethod = method;
}

void gsPipe_TextureSet_0019a588(gsPipeRecovered *self, uint32_t tbp, int tbw,
                                unsigned texwidth, unsigned texheight,
                                uint32_t tpsm, uint32_t cbp, uint32_t csm,
                                uint32_t cbw, uint32_t cpsm)
{
    (void)cbw; /* Present in the original signature but unused by the target. */
    dma_add_qwc(self, 9);

    packet_store(self, 0, UINT64_C(0x1000000000008008));
    packet_store(self, 1, UINT64_C(0xfffffffffffffffe));
    packet_store(self, 2, 4); packet_store(self, 3, GS_REG_TEXCLUT);
    packet_store(self, 4, 0); packet_store(self, 5, GS_REG_TEXFLUSH);
    packet_store(self, 6, UINT64_C(0x0000000000008080)); packet_store(self, 7, GS_REG_TEXA);

    const uint64_t tex1 = ((uint64_t)(self->m_FilterMethod & 1) << 5) |
                          ((uint64_t)(self->m_FilterMethod & 7) << 6);
    packet_store(self, 8, tex1); packet_store(self, 9, GS_REG_TEX1_1);

    const uint64_t tex0 =
        (uint64_t)(tbp / 256u) |
        ((uint64_t)((uint32_t)(tbw / 64)) << 14) |
        ((uint64_t)tpsm << 20) |
        ((uint64_t)texwidth << 26) |
        ((uint64_t)texheight << 30) |
        (UINT64_C(1) << 34) |
        ((uint64_t)(cbp / 256u) << 37) |
        ((uint64_t)cpsm << 51) |
        ((uint64_t)csm << 55) |
        (UINT64_C(1) << 61);
    packet_store(self, 10, tex0); packet_store(self, 11, GS_REG_TEX0_1);

    /* Historical source wrote `2^texwidth` and `2^texheight`; the target
       confirms XOR with xori, so do not "fix" this to exponentiation. */
    const uint64_t clamp = UINT64_C(1) | (UINT64_C(1) << 2) |
                           ((uint64_t)(2u ^ texwidth) << 14) |
                           ((uint64_t)(2u ^ texheight) << 34);
    packet_store(self, 12, clamp); packet_store(self, 13, GS_REG_CLAMP_1);
    packet_store(self, 14, UINT64_C(0x0000007f00000044)); packet_store(self, 15, GS_REG_ALPHA_1);
    packet_store(self, 16, 0); packet_store(self, 17, GS_REG_PABE);
    packet_advance(self, 18);
    gsPipe_FlushCheck_001998b8(self);
}

static void prim_begin(gsPipeRecovered *self, unsigned dma_qwords,
                       uint64_t tag, uint64_t regs, uint64_t prim)
{
    dma_add_qwc(self, dma_qwords);
    packet_store(self, 0, tag);
    packet_store(self, 1, regs);
    packet_store(self, 2, prim);
}

void gsPipe_Line_0019a748(gsPipeRecovered *self, int x1, int y1,
                          int x2, int y2, uint32_t z, uint32_t colour)
{
    x1 += self->m_OriginX; y1 += self->m_OriginY;
    x2 += self->m_OriginX; y2 += self->m_OriginY;
    prim_begin(self, 3, gs_gif_tag(1,1,4), UINT64_C(0xffffffffffff5d10),
               gs_prim(GS_PRIM_LINE,0,0,self->m_AlphaEnabled,0));
    packet_store(self, 3, gs_colq(colour));
    packet_store(self, 4, gs_xyz(x1,y1,z));
    packet_store(self, 5, gs_xyz(x2,y2,z));
    packet_advance(self, 6); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_TriangleLine_0019a838(gsPipeRecovered *self,
    int x1,int y1,uint32_t z1,uint32_t c1,
    int x2,int y2,uint32_t z2,uint32_t c2,
    int x3,int y3,uint32_t z3,uint32_t c3)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY;
    x2+=self->m_OriginX; y2+=self->m_OriginY;
    x3+=self->m_OriginX; y3+=self->m_OriginY;
    prim_begin(self,6,gs_gif_tag(1,1,9),UINT64_C(0xfffffff515151d10),
               gs_prim(GS_PRIM_LINESTRIP,1,0,self->m_AlphaEnabled,0));
    packet_store(self,3,gs_colq(c1)); packet_store(self,4,gs_xyz(x1,y1,z1));
    packet_store(self,5,gs_colq(c2)); packet_store(self,6,gs_xyz(x2,y2,z2));
    packet_store(self,7,gs_colq(c3)); packet_store(self,8,gs_xyz(x3,y3,z3));
    packet_store(self,9,gs_colq(c1)); packet_store(self,10,gs_xyz(x1,y1,z1));
    packet_store(self,11,0);
    packet_advance(self,12); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_TriangleFlat_0019a9c0(gsPipeRecovered *self,
    int x1,int y1,uint32_t z1,int x2,int y2,uint32_t z2,
    int x3,int y3,uint32_t z3,uint32_t colour)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY;
    x2+=self->m_OriginX; y2+=self->m_OriginY;
    x3+=self->m_OriginX; y3+=self->m_OriginY;
    prim_begin(self,4,gs_gif_tag(1,1,5),UINT64_C(0xfffffffffff5dd10),
               gs_prim(GS_PRIM_TRIANGLE,0,0,self->m_AlphaEnabled,0));
    packet_store(self,3,gs_colq(colour));
    packet_store(self,4,gs_xyz(x1,y1,z1)); packet_store(self,5,gs_xyz(x2,y2,z2));
    packet_store(self,6,gs_xyz(x3,y3,z3)); packet_store(self,7,0);
    packet_advance(self,8); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_TriangleGouraud_0019aae8(gsPipeRecovered *self,
    int x1,int y1,uint32_t z1,uint32_t c1,
    int x2,int y2,uint32_t z2,uint32_t c2,
    int x3,int y3,uint32_t z3,uint32_t c3)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY;
    x2+=self->m_OriginX; y2+=self->m_OriginY;
    x3+=self->m_OriginX; y3+=self->m_OriginY;
    prim_begin(self,5,gs_gif_tag(1,1,7),UINT64_C(0xfffffffff5151d10),
               gs_prim(GS_PRIM_TRIANGLE,1,0,self->m_AlphaEnabled,0));
    packet_store(self,3,gs_colq(c1)); packet_store(self,4,gs_xyz(x1,y1,z1));
    packet_store(self,5,gs_colq(c2)); packet_store(self,6,gs_xyz(x2,y2,z2));
    packet_store(self,7,gs_colq(c3)); packet_store(self,8,gs_xyz(x3,y3,z3));
    packet_store(self,9,0);
    packet_advance(self,10); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_TriangleTexture_0019ac40(gsPipeRecovered *self,
    int x1,int y1,uint32_t z1,uint32_t u1,uint32_t v1,
    int x2,int y2,uint32_t z2,uint32_t u2,uint32_t v2,
    int x3,int y3,uint32_t z3,uint32_t u3,uint32_t v3,uint32_t colour)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY;
    x2+=self->m_OriginX; y2+=self->m_OriginY;
    x3+=self->m_OriginX; y3+=self->m_OriginY;
    prim_begin(self,5,gs_gif_tag(1,1,8),UINT64_C(0xffffffff53535310),
               gs_prim(GS_PRIM_TRIANGLE,0,1,self->m_AlphaEnabled,1));
    packet_store(self,3,gs_colq(colour));
    packet_store(self,4,gs_uv(u1,v1)); packet_store(self,5,gs_xyz(x1,y1,z1));
    packet_store(self,6,gs_uv(u2,v2)); packet_store(self,7,gs_xyz(x2,y2,z2));
    packet_store(self,8,gs_uv(u3,v3)); packet_store(self,9,gs_xyz(x3,y3,z3));
    packet_advance(self,10); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_RectFlat_0019adf8(gsPipeRecovered *self, int x1,int y1,int x2,int y2,
                              uint32_t z,uint32_t colour)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY; x2+=self->m_OriginX; y2+=self->m_OriginY;
    prim_begin(self,4,gs_gif_tag(1,1,6),UINT64_C(0xffffffffff55dd10),
               gs_prim(GS_PRIM_TRIANGLE_STRIP,0,0,self->m_AlphaEnabled,0));
    packet_store(self,3,gs_colq(colour));
    packet_store(self,4,gs_xyz(x1,y1,z)); packet_store(self,5,gs_xyz(x2,y1,z));
    packet_store(self,6,gs_xyz(x1,y2,z)); packet_store(self,7,gs_xyz(x2,y2,z));
    packet_advance(self,8); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_RectLine_0019af08(gsPipeRecovered *self,int x1,int y1,int x2,int y2,
                              uint32_t z,uint32_t colour)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY; x2+=self->m_OriginX; y2+=self->m_OriginY;
    prim_begin(self,5,gs_gif_tag(1,1,8),UINT64_C(0xfffffffff5555510),
               gs_prim(GS_PRIM_LINESTRIP,0,0,self->m_AlphaEnabled,0));
    packet_store(self,3,gs_colq(colour));
    packet_store(self,4,gs_xyz(x1,y1,z)); packet_store(self,5,gs_xyz(x2,y1,z));
    packet_store(self,6,gs_xyz(x2,y2,z)); packet_store(self,7,gs_xyz(x1,y2,z));
    packet_store(self,8,gs_xyz(x1,y1,z)); packet_store(self,9,0);
    packet_advance(self,10); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_RectTexture_0019b028(gsPipeRecovered *self,
    int x1,int y1,uint32_t u1,uint32_t v1,int x2,int y2,uint32_t u2,uint32_t v2,
    uint32_t z,uint32_t colour)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY; x2+=self->m_OriginX; y2+=self->m_OriginY;
    prim_begin(self,4,gs_gif_tag(1,1,6),UINT64_C(0xffffffffff535310),
               gs_prim(GS_PRIM_SPRITE,0,1,self->m_AlphaEnabled,1));
    packet_store(self,3,gs_colq(colour));
    packet_store(self,4,gs_uv(u1,v1)); packet_store(self,5,gs_xyz(x1,y1,z));
    packet_store(self,6,gs_uv(u2,v2)); packet_store(self,7,gs_xyz(x2,y2,z));
    packet_advance(self,8); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_RectGouraud_0019b170(gsPipeRecovered *self,
    int x1,int y1,uint32_t c1,int x2,int y2,uint32_t c2,uint32_t z)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY; x2+=self->m_OriginX; y2+=self->m_OriginY;
    prim_begin(self,6,gs_gif_tag(1,1,10),UINT64_C(0xfffffff5151d1d10),
               gs_prim(GS_PRIM_TRIANGLE_STRIP,1,0,self->m_AlphaEnabled,0));
    packet_store(self,3,gs_colq(c1)); packet_store(self,4,gs_xyz(x1,y1,z));
    packet_store(self,5,gs_colq(c2)); packet_store(self,6,gs_xyz(x2,y1,z));
    packet_store(self,7,gs_colq(c2)); packet_store(self,8,gs_xyz(x1,y2,z));
    packet_store(self,9,gs_colq(c1)); packet_store(self,10,gs_xyz(x2,y2,z));
    packet_store(self,11,0);
    packet_advance(self,12); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_Point_0019b2c8(gsPipeRecovered *self,int x,int y,uint32_t z,uint32_t colour)
{
    x+=self->m_OriginX; y+=self->m_OriginY;
    prim_begin(self,3,gs_gif_tag(1,1,4),UINT64_C(0x0ffffffffffff510),
               gs_prim(GS_PRIM_POINT,0,0,self->m_AlphaEnabled,0));
    packet_store(self,3,gs_colq(colour)); packet_store(self,4,gs_xyz(x,y,z));
    packet_store(self,5,0);
    packet_advance(self,6); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_TriStripGouraud_0019b3a8(gsPipeRecovered *self,
    int x1,int y1,uint32_t z1,uint32_t c1,int x2,int y2,uint32_t z2,uint32_t c2,
    int x3,int y3,uint32_t z3,uint32_t c3,int x4,int y4,uint32_t z4,uint32_t c4)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY; x2+=self->m_OriginX; y2+=self->m_OriginY;
    x3+=self->m_OriginX; y3+=self->m_OriginY; x4+=self->m_OriginX; y4+=self->m_OriginY;
    prim_begin(self,6,gs_gif_tag(1,1,10),UINT64_C(0xfffffff5151d1d10),
               gs_prim(GS_PRIM_TRIANGLE_STRIP,1,0,self->m_AlphaEnabled,0));
    packet_store(self,3,gs_colq(c1)); packet_store(self,4,gs_xyz(x1,y1,z1));
    packet_store(self,5,gs_colq(c2)); packet_store(self,6,gs_xyz(x2,y2,z2));
    packet_store(self,7,gs_colq(c3)); packet_store(self,8,gs_xyz(x3,y3,z3));
    packet_store(self,9,gs_colq(c4)); packet_store(self,10,gs_xyz(x4,y4,z4));
    packet_store(self,11,0);
    packet_advance(self,12); gsPipe_FlushCheck_001998b8(self);
}

void gsPipe_TriStripGouraudTexture_0019b568(gsPipeRecovered *self,
    int x1,int y1,uint32_t z1,uint32_t u1,uint32_t v1,uint32_t c1,
    int x2,int y2,uint32_t z2,uint32_t u2,uint32_t v2,uint32_t c2,
    int x3,int y3,uint32_t z3,uint32_t u3,uint32_t v3,uint32_t c3,
    int x4,int y4,uint32_t z4,uint32_t u4,uint32_t v4,uint32_t c4)
{
    x1+=self->m_OriginX; y1+=self->m_OriginY; x2+=self->m_OriginX; y2+=self->m_OriginY;
    x3+=self->m_OriginX; y3+=self->m_OriginY; x4+=self->m_OriginX; y4+=self->m_OriginY;
    prim_begin(self,8,gs_gif_tag(1,1,14),UINT64_C(0xfff531531d31d310),
               gs_prim(GS_PRIM_TRIANGLE_STRIP,1,1,self->m_AlphaEnabled,1));
    packet_store(self,3,gs_colq(c1)); packet_store(self,4,gs_uv(u1,v1)); packet_store(self,5,gs_xyz(x1,y1,z1));
    packet_store(self,6,gs_colq(c2)); packet_store(self,7,gs_uv(u2,v2)); packet_store(self,8,gs_xyz(x2,y2,z2));
    packet_store(self,9,gs_colq(c3)); packet_store(self,10,gs_uv(u3,v3)); packet_store(self,11,gs_xyz(x3,y3,z3));
    packet_store(self,12,gs_colq(c4)); packet_store(self,13,gs_uv(u4,v4)); packet_store(self,14,gs_xyz(x4,y4,z4));
    packet_store(self,15,0);
    packet_advance(self,16); gsPipe_FlushCheck_001998b8(self);
}
