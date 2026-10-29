/*
 * SNES Station v0.23 PS2 - Frontend Message Display
 * Canonical source from progress22_more_small_helpers.c @ 0x00103b34
 */

#include <stddef.h>
#include <stdint.h>

struct gsDriverRecovered;
struct gsFontRecovered;

/* Forward declarations for external dependencies */
extern void *operator_new_001a9e88(size_t size);
extern void gsDriver_ctor_00198cc8(struct gsDriverRecovered *driver);
extern void sub_001019a8(void);
extern void CDVD_Stop(void);
extern void sub_00101b64(void);
extern void snes_p20_00101e8c(void);
extern void gsPipe_setScissorRect_00199ef8(struct gsDriverRecovered *drawPipe,
                                           int x1, int y1, int x2, int y2);
extern void gsFont_Print_0019b948(struct gsFontRecovered *font, int x, int y,
                                   int z, int w, uint64_t color, int scale,
                                   const char *text);

extern struct gsDriverRecovered *g_frontend_driver_001bb2c0;
extern struct gsFontRecovered g_frontend_font_001bb748;

/* 0x00103b34: frontend message slide / scissor helper */
void snes_frontend_msg_slide_00103b34(const char *text, int travel)
{
    int y = 0x186;
    const int limit = 0x186 - travel;

    if (g_frontend_driver_001bb2c0 == NULL) {
        g_frontend_driver_001bb2c0 = operator_new_001a9e88(0x74u);
        gsDriver_ctor_00198cc8(g_frontend_driver_001bb2c0);
    }

    sub_001019a8();
    CDVD_Stop();

    if (limit < 0x186) {
        do {
            gsPipe_setScissorRect_00199ef8(&g_frontend_driver_001bb2c0->drawPipe,
                                           0, 0, 0x280, 0x1e0);
            sub_00101b64();
            gsPipe_setScissorRect_00199ef8(&g_frontend_driver_001bb2c0->drawPipe,
                                           100, 0x74, 0x21c, 0x166);
            gsFont_Print_0019b948(&g_frontend_font_001bb748,
                                  0x84, 0x1fc, y, 2,
                                  UINT64_C(0x80ffffff), 2, text);
            --y;
            snes_p20_00101e8c();
        } while (limit < y);
    }

    gsPipe_setScissorRect_00199ef8(&g_frontend_driver_001bb2c0->drawPipe,
                                   0, 0, 0x280, 0x1e0);
}