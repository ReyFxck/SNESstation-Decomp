#ifndef SNES_STATION_GSLIB_RECOVERED_H
#define SNES_STATION_GSLIB_RECOVERED_H

#include <stdint.h>
#include <stddef.h>

/*
 * Target-layout views of Hiryu's GSLIB objects as embedded in
 * SNES Station v0.23. EE pointers are kept as 32-bit addresses so the
 * layouts remain correct when these files are syntax-checked on a host.
 */
typedef struct gsPipeRecovered {
    uint32_t m_DmaPipe1;       /* +0x00 */
    uint32_t m_DmaPipe2;       /* +0x04 */
    uint32_t m_MemSize;        /* +0x08 */
    uint32_t m_CurrentPipe;    /* +0x0c */
    uint32_t m_CurrentDmaAddr; /* +0x10 */
    uint32_t m_CurrentGifTag;  /* +0x14 */
    uint32_t m_Buffer;         /* +0x18 */
    int32_t  m_AlphaEnabled;   /* +0x1c */
    int32_t  m_ZBufferEnabled; /* +0x20 */
    int32_t  m_ZTestEnabled;   /* +0x24 */
    int32_t  m_OriginX;        /* +0x28 */
    int32_t  m_OriginY;        /* +0x2c */
    int32_t  m_FilterMethod;   /* +0x30 */
} gsPipeRecovered;

typedef struct gsDriverRecovered {
    gsPipeRecovered drawPipe;          /* +0x00 .. +0x33 */
    uint32_t m_FrameWidth;             /* +0x34 */
    uint32_t m_FrameHeight;            /* +0x38 */
    uint32_t m_FrameXpos;              /* +0x3c */
    uint32_t m_FrameYpos;              /* +0x40 */
    uint32_t m_FramePSM;               /* +0x44 */
    uint32_t m_ZBuffer;                /* +0x48 */
    uint32_t m_ZBufferPSM;             /* +0x4c */
    uint32_t m_CurrentDisplayBuffer;   /* +0x50 */
    uint32_t m_CurrentDrawBuffer;      /* +0x54 */
    uint32_t m_NumFrameBuffers;        /* +0x58 */
    uint32_t m_FreeBuffersAvailable;   /* +0x5c */
    uint32_t m_CompleteBuffersAvailable;/* +0x60 */
    uint32_t m_FrameSize;              /* +0x64 */
    uint32_t m_ZBufferBase;            /* +0x68 */
    uint32_t m_ZBufferSize;            /* +0x6c */
    uint32_t m_TextureBufferBase;      /* +0x70 */
} gsDriverRecovered;

_Static_assert(sizeof(gsPipeRecovered) == 0x34, "gsPipe target size");
_Static_assert(sizeof(gsDriverRecovered) == 0x74, "gsDriver target size");
_Static_assert(offsetof(gsDriverRecovered, m_FrameWidth) == 0x34, "gsDriver drawPipe size");
_Static_assert(offsetof(gsDriverRecovered, m_TextureBufferBase) == 0x70, "gsDriver tail offset");

/* gsFont 0x0019b7f0..0x0019bd34. Target bool fields are one byte. */
typedef struct gsFontRecovered {
    uint32_t m_pFontPipe;       /* +0x00 -> gsPipe */
    uint32_t m_TBbase;          /* +0x04 */
    int32_t  m_TBwidth;         /* +0x08 */
    int32_t  m_TBxpos;          /* +0x0c */
    int32_t  m_TBypos;          /* +0x10 */
    uint32_t m_TexWidth;        /* +0x14 */
    uint32_t m_TexHeight;       /* +0x18 */
    uint32_t m_NumXChars;       /* +0x1c */
    uint32_t m_NumYChars;       /* +0x20 */
    uint32_t m_CharGridWidth;   /* +0x24 */
    uint32_t m_CharGridHeight;  /* +0x28 */
    uint32_t m_PSM;             /* +0x2c */
    uint8_t  m_Bold;            /* +0x30 */
    uint8_t  m_Underline;       /* +0x31 */
    int8_t   m_CharWidth[256];  /* +0x32 */
    uint8_t  _tail_pad[2];      /* C++ object rounded to 4-byte alignment */
} gsFontRecovered;

typedef struct gsFontTexRecovered {
    char ID[4];
    uint32_t TexWidth;
    uint32_t TexHeight;
    uint32_t PSM;
    uint32_t NumXChars;
    uint32_t NumYChars;
    uint32_t CharGridWidth;
    uint32_t CharGridHeight;
    int8_t CharWidth[256];
    uint8_t PixelData[];
} gsFontTexRecovered;

_Static_assert(offsetof(gsFontRecovered, m_CharWidth) == 0x32, "gsFont width table offset");
_Static_assert(sizeof(gsFontRecovered) == 0x134, "gsFont target-aligned size");
_Static_assert(offsetof(gsFontTexRecovered, PixelData) == 0x120, "BFNT pixel offset");

/* gsPipe 0x00199480..0x0019b7ec */
void gsPipe_ctor_00199480(gsPipeRecovered *self, uint32_t size);
void gsPipe_ctor_00199590(gsPipeRecovered *self, uint32_t size);
void gsPipe_dtor_001996a0(gsPipeRecovered *self);
void gsPipe_dtor_001996d0(gsPipeRecovered *self);
gsPipeRecovered *gsPipe_assign_00199740(gsPipeRecovered *self, const gsPipeRecovered *src);
uint32_t gsPipe_getPipeSize_00199830(const gsPipeRecovered *self);
void gsPipe_InitPipe_00199838(gsPipeRecovered *self, uint32_t dma_addr);
void gsPipe_ReInit_00199848(gsPipeRecovered *self);
uint32_t gsPipe_getBytesLeft_00199898(const gsPipeRecovered *self);
void gsPipe_FlushCheck_001998b8(gsPipeRecovered *self);
void gsPipe_Flush_001998f8(gsPipeRecovered *self);
void gsPipe_FlushInt_00199970(gsPipeRecovered *self);
void gsPipe_setZBuffer_001999e8(gsPipeRecovered *self, uint32_t base, int psm, uint32_t enable);
void gsPipe_setZTestEnable_00199aa8(gsPipeRecovered *self, int enable);
void gsPipe_setAlphaEnable_00199b80(gsPipeRecovered *self, int enable);
void gsPipe_setDither_00199c58(gsPipeRecovered *self, uint32_t enable);
void gsPipe_setColClamp_00199cd0(gsPipeRecovered *self, uint32_t enable);
void gsPipe_setPrModeCont_00199d48(gsPipeRecovered *self, uint32_t enable);
void gsPipe_setDrawFrame_00199dc0(gsPipeRecovered *self, uint32_t base, uint32_t width, int psm, uint32_t mask);
void gsPipe_setOrigin_00199e58(gsPipeRecovered *self, int x, int y);
void gsPipe_setScissorRect_00199ef8(gsPipeRecovered *self, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void gsPipe_TextureUpload_00199f88(gsPipeRecovered *self, uint32_t tbp, int tbw, int xofs, int yofs, int psm, const uint8_t *tex, int width, int height);
void gsPipe_TextureDownload_0019a240(gsPipeRecovered *self, uint32_t tbp, int tbw, int xofs, int yofs, int psm, uint8_t *tex, int width, int height);
void gsPipe_TextureFlush_0019a500(gsPipeRecovered *self);
void gsPipe_setFilterMethod_0019a580(gsPipeRecovered *self, int method);
void gsPipe_TextureSet_0019a588(gsPipeRecovered *self, uint32_t tbp, int tbw,
    unsigned texwidth, unsigned texheight, uint32_t tpsm, uint32_t cbp,
    uint32_t csm, uint32_t cbw, uint32_t cpsm);
void gsPipe_RectFlat_0019adf8(gsPipeRecovered *self, int x1, int y1,
    int x2, int y2, uint32_t z, uint32_t colour);

/* gsDriver 0x00198c58..0x00199478 */
void gsDriver_ctor_00198c58(gsDriverRecovered *self);
void gsDriver_ctor_00198cc8(gsDriverRecovered *self);
void gsDriver_dtor_00198d38(gsDriverRecovered *self);
void gsDriver_dtor_00198d58(gsDriverRecovered *self);
void gsDriver_setDisplayMode_00198d78(gsDriverRecovered *self,
    uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos,
    uint32_t psm, uint32_t num_bufs, uint32_t tv_mode,
    uint32_t tv_interlace, uint32_t zbuffer, uint32_t zpsm);
void gsDriver_setDisplayPosition_00199070(gsDriverRecovered *self, uint32_t xpos, uint32_t ypos);
void gsDriver_clearScreen_001990f8(gsDriverRecovered *self);
uint32_t gsDriver_getFrameBufferBase_00199178(const gsDriverRecovered *self, uint32_t index);
uint32_t gsDriver_getTextureBufferBase_00199198(const gsDriverRecovered *self);
uint32_t gsDriver_getCurrentDisplayBuffer_001991a0(const gsDriverRecovered *self);
uint32_t gsDriver_getCurrentDrawBuffer_001991a8(const gsDriverRecovered *self);
void gsDriver_swapBuffers_001991b0(gsDriverRecovered *self);
int gsDriver_isDrawBufferAvailable_001991e8(const gsDriverRecovered *self);
int gsDriver_isDisplayBufferAvailable_001991f8(const gsDriverRecovered *self);
void gsDriver_setNextDrawBuffer_00199208(gsDriverRecovered *self);
void gsDriver_DrawBufferComplete_00199268(gsDriverRecovered *self);
void gsDriver_DisplayNextFrame_00199290(gsDriverRecovered *self);
void gsDriver_setDisplayBuffer_001992f8(gsDriverRecovered *self, uint32_t index);
void gsDriver_setDrawBuffer_00199360(gsDriverRecovered *self, uint32_t index);
uint32_t gsDriver_AddVSyncCallback_001993c8(gsDriverRecovered *self, void (*callback)(void));
void gsDriver_RemoveVSyncCallback_00199420(gsDriverRecovered *self, uint32_t id);
void gsDriver_EnableVSyncCallbacks_00199440(gsDriverRecovered *self);
void gsDriver_DisableVSyncCallbacks_00199460(gsDriverRecovered *self);

/* Remaining gsPipe methods in target order. */
void gsPipe_copy_ctor_00199700(gsPipeRecovered *self, const gsPipeRecovered *src);
void gsPipe_copy_ctor_00199720(gsPipeRecovered *self, const gsPipeRecovered *src);
void gsPipe_Line_0019a748(gsPipeRecovered *self, int x1, int y1, int x2, int y2, uint32_t z, uint32_t colour);
void gsPipe_TriangleLine_0019a838(gsPipeRecovered *self, int x1,int y1,uint32_t z1,uint32_t c1,int x2,int y2,uint32_t z2,uint32_t c2,int x3,int y3,uint32_t z3,uint32_t c3);
void gsPipe_TriangleFlat_0019a9c0(gsPipeRecovered *self, int x1,int y1,uint32_t z1,int x2,int y2,uint32_t z2,int x3,int y3,uint32_t z3,uint32_t colour);
void gsPipe_TriangleGouraud_0019aae8(gsPipeRecovered *self, int x1,int y1,uint32_t z1,uint32_t c1,int x2,int y2,uint32_t z2,uint32_t c2,int x3,int y3,uint32_t z3,uint32_t c3);
void gsPipe_TriangleTexture_0019ac40(gsPipeRecovered *self, int x1,int y1,uint32_t z1,uint32_t u1,uint32_t v1,int x2,int y2,uint32_t z2,uint32_t u2,uint32_t v2,int x3,int y3,uint32_t z3,uint32_t u3,uint32_t v3,uint32_t colour);
void gsPipe_RectLine_0019af08(gsPipeRecovered *self,int x1,int y1,int x2,int y2,uint32_t z,uint32_t colour);
void gsPipe_RectTexture_0019b028(gsPipeRecovered *self,int x1,int y1,uint32_t u1,uint32_t v1,int x2,int y2,uint32_t u2,uint32_t v2,uint32_t z,uint32_t colour);
void gsPipe_RectGouraud_0019b170(gsPipeRecovered *self,int x1,int y1,uint32_t c1,int x2,int y2,uint32_t c2,uint32_t z);
void gsPipe_Point_0019b2c8(gsPipeRecovered *self,int x,int y,uint32_t z,uint32_t colour);
void gsPipe_TriStripGouraud_0019b3a8(gsPipeRecovered *self,int x1,int y1,uint32_t z1,uint32_t c1,int x2,int y2,uint32_t z2,uint32_t c2,int x3,int y3,uint32_t z3,uint32_t c3,int x4,int y4,uint32_t z4,uint32_t c4);
void gsPipe_TriStripGouraudTexture_0019b568(gsPipeRecovered *self,int x1,int y1,uint32_t z1,uint32_t u1,uint32_t v1,uint32_t c1,int x2,int y2,uint32_t z2,uint32_t u2,uint32_t v2,uint32_t c2,int x3,int y3,uint32_t z3,uint32_t u3,uint32_t v3,uint32_t c3,int x4,int y4,uint32_t z4,uint32_t u4,uint32_t v4,uint32_t c4);

/* gsFont methods. Constructor/assignPipe were inline in the historical header. */
void gsFont_uploadFont_0019b7f0(gsFontRecovered *self, const gsFontTexRecovered *font, uint32_t tbbase, int tbwidth, int tbxpos, int tbypos);
void gsFont_Print_0019b948(gsFontRecovered *self, int x, int xend, int y, int z, uint64_t colour, int alignment, const char *text);
void gsFont_GetCurrLineLength_0019bad0(const gsFontRecovered *self, const char *text, int max_length, int *pix_length, int *char_length);
void gsFont_PrintLine_0019bb68(gsFontRecovered *self, int x, int y, int z, uint64_t colour, int length, const char *text);
uint32_t gsDriver_getTexSizeFromInt_001b0790(int texsize);

#endif
