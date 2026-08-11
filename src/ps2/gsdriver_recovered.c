/*
 * Recovered early Hiryu GSLIB gsDriver embedded in SNES Station v0.23.
 * Target corridor: 0x00198c58..0x00199478.
 *
 * This is older than the commonly mirrored gsDriver interface.  The target
 * still uses the historical setDisplayMode(width,height,xpos,ypos,psm,
 * num_bufs,TVmode,TVinterlace,zbuffer,zpsm) form.  That old signature survives
 * as a commented prototype in later GSLIB source and is independently proven
 * here by the EE calling convention and the 0x74-byte object layout.
 */
#include <stdint.h>

#include "../../include/gslib_recovered.h"

#define GS_PRIV_PMODE     ((uintptr_t)0x12000000u)
#define GS_PRIV_DISPFB1   ((uintptr_t)0x12000070u)
#define GS_PRIV_DISPLAY1  ((uintptr_t)0x12000080u)
#define GS_PRIV_BGCOLOUR  ((uintptr_t)0x120000e0u)
#define GS_PRIV_CSR       ((uintptr_t)0x12001000u)

#define GS_PSMCT32  0x00u
#define GS_PSMCT24  0x01u
#define GS_PSMCT16  0x02u
#define GS_PSMCT16S 0x0au
#define GS_PSGPU24  0x12u

/* The constructor reads this target configuration byte and compares it with
   ASCII 'E' (0x45) to choose PAL (3) vs NTSC (2). */
extern uint8_t target_video_mode_byte_001fc752;

/* Host-buildable stand-ins for the inline BIOS/kernel operations in target. */
extern int ps2_bios_syscall_2_recovered(int a0, int a1, int a2);
extern void ps2_gs_put_imr_recovered(uint32_t value);
extern uint32_t ps2_add_intc_handler_recovered(int cause, void (*handler)(void), void *arg);
extern void ps2_remove_intc_handler_recovered(int cause, uint32_t id);
extern void ps2_enable_intc_recovered(int cause);
extern void ps2_disable_intc_recovered(int cause);

static uint32_t bytes_per_pixel(uint32_t psm)
{
    switch (psm) {
    case GS_PSMCT32: return 4;
    case GS_PSMCT24:
    case GS_PSGPU24: return 3;
    case GS_PSMCT16:
    case GS_PSMCT16S: return 2;
    default: return 1;
    }
}

static uint32_t round_frame_8k(uint32_t size)
{
    if (size & 0x7fffu)
        size = (size & 0xffff8000u) + 0x8000u;
    return size;
}

static void write64(uintptr_t address, uint64_t value)
{
    *(volatile uint64_t *)address = value;
}

static uint64_t make_display1(uint32_t width, uint32_t height,
                              uint32_t xpos, uint32_t ypos)
{
    const uint32_t mag_div = 2560u / width;
    const uint32_t magh = ((2560u + width - 1u) / width) - 1u;
    return ((uint64_t)(height - 1u) << 44) |
           ((uint64_t)0x9ffu << 32) |
           ((uint64_t)magh << 23) |
           ((uint64_t)ypos << 12) |
           ((uint64_t)xpos * mag_div);
}

static void gsDriver_ctor_body(gsDriverRecovered *self)
{
    gsPipe_ctor_00199590(&self->drawPipe, 0x20000u);

    const uint32_t tv_mode = (target_video_mode_byte_001fc752 == 0x45u) ? 3u : 2u;
    gsDriver_setDisplayMode_00198d78(self,
        320u, 240u, 85u, 42u,
        0u, 2u, tv_mode, 0u, 1u, 0u);
}

void gsDriver_ctor_00198c58(gsDriverRecovered *self) { gsDriver_ctor_body(self); }
void gsDriver_ctor_00198cc8(gsDriverRecovered *self) { gsDriver_ctor_body(self); }
void gsDriver_dtor_00198d38(gsDriverRecovered *self) { gsPipe_dtor_001996d0(&self->drawPipe); }
void gsDriver_dtor_00198d58(gsDriverRecovered *self) { gsPipe_dtor_001996d0(&self->drawPipe); }

/* 0x00198d78 -- old GSLIB signature, not the later mode/interlace interface. */
void gsDriver_setDisplayMode_00198d78(gsDriverRecovered *self,
    uint32_t width, uint32_t height, uint32_t xpos, uint32_t ypos,
    uint32_t psm, uint32_t num_bufs, uint32_t tv_mode,
    uint32_t tv_interlace, uint32_t zbuffer, uint32_t zpsm)
{
    if (num_bufs == 0)
        num_bufs = 1;

    self->m_FrameWidth = width & 0xffc0u; /* target keeps old 64-pixel truncation */
    self->m_FrameHeight = height;
    self->m_FrameXpos = xpos;
    self->m_FrameYpos = ypos;
    self->m_FramePSM = psm;
    self->m_ZBuffer = zbuffer & 1u;
    self->m_ZBufferPSM = zpsm;
    self->m_CurrentDisplayBuffer = 0;
    self->m_CurrentDrawBuffer = (num_bufs > 1u) ? 1u : 0u;
    self->m_NumFrameBuffers = num_bufs;
    self->m_FreeBuffersAvailable = num_bufs - 2u; /* preserve unsigned underflow */
    /* m_CompleteBuffersAvailable (+0x60) is intentionally not initialized here. */

    self->m_FrameSize = round_frame_8k(
        self->m_FrameWidth * self->m_FrameHeight * bytes_per_pixel(self->m_FramePSM));
    self->m_ZBufferBase = self->m_FrameSize * self->m_NumFrameBuffers;

    if (self->m_ZBuffer) {
        self->m_ZBufferSize = round_frame_8k(
            self->m_FrameWidth * self->m_FrameHeight * bytes_per_pixel(self->m_ZBufferPSM));
    } else {
        self->m_ZBufferSize = 0;
    }
    self->m_TextureBufferBase = self->m_ZBufferBase + self->m_ZBufferSize;

    write64(GS_PRIV_CSR, UINT64_C(0x200));
    /* Target executes sync.p here. */
    write64(GS_PRIV_CSR, 0);
    ps2_gs_put_imr_recovered(0xff00u);
    (void)ps2_bios_syscall_2_recovered((int)tv_interlace, (int)tv_mode, 0);

    /* Privileged display setup is inline in the target, not a call to 0x199070. */
    write64(GS_PRIV_PMODE, UINT64_C(0xff61));
    write64(GS_PRIV_DISPLAY1,
            make_display1(self->m_FrameWidth, self->m_FrameHeight,
                          self->m_FrameXpos, self->m_FrameYpos));
    write64(GS_PRIV_BGCOLOUR, 0);

    gsDriver_setDisplayBuffer_001992f8(self, self->m_CurrentDisplayBuffer);
    gsDriver_setDrawBuffer_00199360(self, self->m_CurrentDrawBuffer);

    gsPipe_setZBuffer_001999e8(&self->drawPipe,
        self->m_ZBufferBase, (int)self->m_ZBufferPSM, self->m_ZBuffer);
    gsPipe_setZTestEnable_00199aa8(&self->drawPipe, 0);
    gsPipe_setOrigin_00199e58(&self->drawPipe, 1024, 1024);
    gsPipe_setPrModeCont_00199d48(&self->drawPipe, 1);
    gsPipe_setDither_00199c58(&self->drawPipe, 0);
    gsPipe_setColClamp_00199cd0(&self->drawPipe, 1);
    gsPipe_setScissorRect_00199ef8(&self->drawPipe, 0, 0,
                                   (int32_t)self->m_FrameWidth,
                                   (int32_t)self->m_FrameHeight);
    gsPipe_setZTestEnable_00199aa8(&self->drawPipe, (int)self->m_ZBuffer);
    gsPipe_Flush_001998f8(&self->drawPipe);
}

void gsDriver_setDisplayPosition_00199070(gsDriverRecovered *self,
                                           uint32_t xpos, uint32_t ypos)
{
    self->m_FrameXpos = xpos;
    self->m_FrameYpos = ypos;
    write64(GS_PRIV_DISPLAY1,
            make_display1(self->m_FrameWidth, self->m_FrameHeight, xpos, ypos));
}

void gsDriver_clearScreen_001990f8(gsDriverRecovered *self)
{
    gsPipe_setZTestEnable_00199aa8(&self->drawPipe, 0);
    gsPipe_RectFlat_0019adf8(&self->drawPipe, 0, 0,
                             (int)self->m_FrameWidth, (int)self->m_FrameHeight,
                             0, 0x80000000u);
    gsPipe_setZTestEnable_00199aa8(&self->drawPipe, (int)self->m_ZBuffer);
    gsPipe_Flush_001998f8(&self->drawPipe);
    gsDriver_swapBuffers_001991b0(self);
}

/* 0x00199160 is the old InitGraphField helper: syscall #2, field mode a2=0. */
int gsDriver_InitGraphField_00199160(int interlace, int mode)
{
    return ps2_bios_syscall_2_recovered(interlace, mode, 0);
}

uint32_t gsDriver_getFrameBufferBase_00199178(const gsDriverRecovered *self,
                                               uint32_t index)
{
    if (index > self->m_NumFrameBuffers - 1u)
        index = self->m_NumFrameBuffers - 1u;
    return self->m_FrameSize * index;
}

uint32_t gsDriver_getTextureBufferBase_00199198(const gsDriverRecovered *self)
{
    return self->m_TextureBufferBase;
}

uint32_t gsDriver_getCurrentDisplayBuffer_001991a0(const gsDriverRecovered *self)
{
    return self->m_CurrentDisplayBuffer;
}

uint32_t gsDriver_getCurrentDrawBuffer_001991a8(const gsDriverRecovered *self)
{
    return self->m_CurrentDrawBuffer;
}

void gsDriver_swapBuffers_001991b0(gsDriverRecovered *self)
{
    gsDriver_DrawBufferComplete_00199268(self);
    gsDriver_DisplayNextFrame_00199290(self);
    gsDriver_setNextDrawBuffer_00199208(self);
}

int gsDriver_isDrawBufferAvailable_001991e8(const gsDriverRecovered *self)
{
    return self->m_FreeBuffersAvailable != 0;
}

int gsDriver_isDisplayBufferAvailable_001991f8(const gsDriverRecovered *self)
{
    return self->m_CompleteBuffersAvailable != 0;
}

void gsDriver_setNextDrawBuffer_00199208(gsDriverRecovered *self)
{
    if (self->m_FreeBuffersAvailable == 0)
        return;
    self->m_CurrentDrawBuffer++;
    if (self->m_CurrentDrawBuffer > self->m_NumFrameBuffers - 1u)
        self->m_CurrentDrawBuffer = 0;
    gsDriver_setDrawBuffer_00199360(self, self->m_CurrentDrawBuffer);
    self->m_FreeBuffersAvailable--;
}

void gsDriver_DrawBufferComplete_00199268(gsDriverRecovered *self)
{
    if (self->m_CompleteBuffersAvailable < self->m_NumFrameBuffers - 1u)
        self->m_CompleteBuffersAvailable++;
}

void gsDriver_DisplayNextFrame_00199290(gsDriverRecovered *self)
{
    if (self->m_CompleteBuffersAvailable == 0)
        return;
    self->m_CurrentDisplayBuffer++;
    if (self->m_CurrentDisplayBuffer > self->m_NumFrameBuffers - 1u)
        self->m_CurrentDisplayBuffer = 0;
    gsDriver_setDisplayBuffer_001992f8(self, self->m_CurrentDisplayBuffer);
    self->m_CompleteBuffersAvailable--;
    self->m_FreeBuffersAvailable++;
}

void gsDriver_setDisplayBuffer_001992f8(gsDriverRecovered *self, uint32_t index)
{
    const uint32_t base = gsDriver_getFrameBufferBase_00199178(self, index);
    const uint64_t dispfb1 =
        ((uint64_t)base >> 13) |
        ((uint64_t)(self->m_FrameWidth / 64u) << 9) |
        ((uint64_t)self->m_FramePSM << 15);
    write64(GS_PRIV_DISPFB1, dispfb1);
}

void gsDriver_setDrawBuffer_00199360(gsDriverRecovered *self, uint32_t index)
{
    gsPipe_Flush_001998f8(&self->drawPipe);
    gsPipe_setDrawFrame_00199dc0(&self->drawPipe,
        gsDriver_getFrameBufferBase_00199178(self, index),
        self->m_FrameWidth, (int)self->m_FramePSM, 0);
    gsPipe_Flush_001998f8(&self->drawPipe);
}

uint32_t gsDriver_AddVSyncCallback_001993c8(gsDriverRecovered *self,
                                            void (*callback)(void))
{
    (void)self;
    const uint32_t id = ps2_add_intc_handler_recovered(2, callback, 0);
    ps2_enable_intc_recovered(2);
    return id;
}

void gsDriver_RemoveVSyncCallback_00199420(gsDriverRecovered *self, uint32_t id)
{
    (void)self;
    ps2_remove_intc_handler_recovered(2, id);
}

void gsDriver_EnableVSyncCallbacks_00199440(gsDriverRecovered *self)
{
    (void)self;
    ps2_enable_intc_recovered(2);
}

void gsDriver_DisableVSyncCallbacks_00199460(gsDriverRecovered *self)
{
    (void)self;
    ps2_disable_intc_recovered(2);
}
