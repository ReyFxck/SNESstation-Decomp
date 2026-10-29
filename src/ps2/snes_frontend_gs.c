/*
 * SNES Station v0.23 PS2 - Frontend GS Driver & Pipe Helpers
 * Canonical source from progress11_frontend.c @ 0x001005b0, 0x00101b04, 0x00101e8c
 */

#include <stdint.h>

struct gsDriverRecovered;
struct gsPipeRecovered;

extern void gsDriver_RemoveVSyncCallback_00199420(struct gsDriverRecovered *driver,
                                                  uint32_t callback_id);
extern void gsDriver_DisableVSyncCallbacks_00199460(struct gsDriverRecovered *driver);
extern void gsPipe_setAlphaEnable_00199b80(struct gsPipeRecovered *pipe, int enable);
extern void gsPipe_RectFlat_0019adf8(struct gsPipeRecovered *pipe, int x, int y,
                                     int w, int h, uint32_t rgba, uint32_t test);
extern void gsPipe_Flush_001998f8(struct gsPipeRecovered *pipe);
extern void gsDriver_swapBuffers_001991b0(struct gsDriverRecovered *driver);

extern struct gsDriverRecovered *g_frontend_driver_001bb2c0;

void snes_frontend_gs_vsync_cleanup_001005b0(struct gsDriverRecovered *driver,
                                              uint32_t callback_id)
{
    /* 0x00101860 is an empty target leaf immediately before these calls. */
    gsDriver_RemoveVSyncCallback_00199420(driver, callback_id);
    gsDriver_DisableVSyncCallbacks_00199460(driver);
}

void snes_frontend_gs_pipe_clear_00101b04(struct gsPipeRecovered *pipe)
{
    gsPipe_setAlphaEnable_00199b80(pipe, 0);
    gsPipe_RectFlat_0019adf8(pipe, 0, 0, 640, 480, 0,
                            UINT32_C(0x80000000));
    gsPipe_setAlphaEnable_00199b80(pipe, 1);
    gsPipe_Flush_001998f8(pipe);
}

#define SNES_FRONTEND_GS_CSR \
    ((volatile uint64_t *)(uintptr_t)UINT32_C(0x12001000))

/*
 * 0x00101e8c: flush the current GS pipe, clear/wait the GS CSR VSINT bit,
 * then swap the driver buffers.  The explicit nop preserves the EE-side GS
 * CSR read delay present in the original frontend.
 */
void snes_frontend_gs_frame_swap_00101e8c(void)
{
    uint64_t csr;

    gsPipe_Flush_001998f8(&g_frontend_driver_001bb2c0->drawPipe);

    *SNES_FRONTEND_GS_CSR = *SNES_FRONTEND_GS_CSR & UINT64_C(8);
    do {
        csr = *SNES_FRONTEND_GS_CSR;
        csr &= UINT64_C(8);
        __asm__ volatile ("" ::: "memory"); /* compiler barrier replacing nop */
    } while (csr == 0);

    gsDriver_swapBuffers_001991b0(g_frontend_driver_001bb2c0);
}