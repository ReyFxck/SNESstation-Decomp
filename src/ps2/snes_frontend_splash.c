/*
 * SNES Station v0.23 - Frontend Boot Splash Animation
 * Canonical source from progress28_structural_lift.c (0x00101ef0 range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern uint8_t  g_rom_type;                    /* 0x00345565 / cRam1fc7ff52 */
extern uint32_t g_unk_1bb87c;                  /* 0x001bb87c */
extern void    *g_splash_obj;                  /* 0x001bb2c0 */
extern uint32_t g_unk_1ebd48;                  /* 0x001ebd48 */
extern uint32_t g_unk_1ebd4c;                  /* 0x001ebd4c */

/* External function declarations */
extern void   *snes_obj_alloc_001a9e88(uint32_t size);
extern void    snes_obj_init_00198cc8(void *obj);
extern void    snes_obj_config_00198d78(void *obj, uint64_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7);
extern void    snes_obj_start_001990f8(void *obj);
extern void    snes_frontend_sub_001019a8(void);
extern void    snes_frontend_sub_00101b04(void);
extern void    snes_frontend_sub_00101e8c(void);
extern void    snes_obj_callback_0019a580(void *obj, uint32_t cb);
extern uint32_t snes_obj_status_00199198(void *obj);
extern void    snes_obj_vram_0019a588(void *obj, uint32_t status, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
extern void    snes_obj_config2_0019b028(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
extern void    snes_obj_config_0019adf8(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);

/* 0x00101ef0: Boot splash screen animation */
void snes_frontend_splash_00101ef0(void)
{
    g_unk_1bb87c = 1;

    if (g_splash_obj == NULL) {
        void *obj = snes_obj_alloc_001a9e88(0x74);
        snes_obj_init_00198cc8(obj);
        g_splash_obj = obj;
    }

    /* Region check: 'E' = Europe/PAL, else NTSC */
    uint64_t mode = (g_rom_type == 'E') ? 3 : 2;

    snes_obj_config_00198d78(g_splash_obj, 0x280, 0x1e0, g_unk_1ebd48, g_unk_1ebd4c, 0, 2, mode);
    snes_obj_start_001990f8(g_splash_obj);
    snes_frontend_sub_001019a8();
    snes_obj_callback_0019a580(g_splash_obj, 0);

    /* First animation phase: 0x80 frames */
    int32_t frame = 0x80;
    do {
        snes_frontend_sub_00101b04();
        frame--;
        uint32_t status = snes_obj_status_00199198(g_splash_obj);
        snes_obj_vram_0019a588(g_splash_obj, status, 0x280, 10, 9, 2, 0, 0);
        snes_obj_config2_0019b028(g_splash_obj, 0, 0, 0, 0, 0x280, 0x1e0, 0x280);
        snes_frontend_sub_00101e8c();
    } while (frame >= 0);

    /* Second animation phase: scroll up from -0x132 to 0x55 */
    int32_t y_pos = -0x132;
    do {
        snes_frontend_sub_00101b04();
        int32_t y1 = y_pos + 0x18;
        uint32_t status = snes_obj_status_00199198(g_splash_obj);
        int32_t y2 = y_pos + 0x114;
        int32_t y3 = y_pos + 300;

        snes_obj_vram_0019a588(g_splash_obj, status, 0x280, 10, 9, 2, 0, 0);
        snes_obj_config2_0019b028(g_splash_obj, 0, 0, 0, 0, 0x280, 0x1e0, 0x280);

        int32_t status2 = snes_obj_status_00199198(g_splash_obj);
        snes_obj_vram_0019a588(g_splash_obj, status2 + 0xc4000, 0x20, 5, 5, 0, 0, 0);

        snes_obj_config_0019adf8(g_splash_obj, 100, y1, 0x21c, y2, 3, 0x40d08024);
        snes_obj_config_0019adf8(g_splash_obj, 0x7c, y_pos, 0x204, y1, 3, 0x40d08024);
        snes_obj_config_0019adf8(g_splash_obj, 0x7c, y2, 0x204, y3, 3, 0x40d08024);

        snes_obj_config_0019adf8(g_splash_obj, 100, y1, 0x67, y2, 3, 0xffffffff80b95c00);
        snes_obj_config_0019adf8(g_splash_obj, 0x21c, y1, 0x219, y2, 3, 0xffffffff80b95c00);
        snes_obj_config_0019adf8(g_splash_obj, 0x7c, y_pos, 0x204, y_pos + 3, 3, 0xffffffff80b95c00);
        snes_obj_config_0019adf8(g_splash_obj, 0x7c, y3, 0x204, y_pos + 0x129, 3, 0xffffffff80b95c00);

        snes_obj_config2_0019b028(g_splash_obj, 100, y_pos, 0, 0, 0x7c, y1, 0x17);
        int32_t y_next = y_pos + 2;
        snes_obj_config2_0019b028(g_splash_obj, 0x21c, y_pos, 0, 0, 0x204, y1, 0x17);
        snes_obj_config2_0019b028(g_splash_obj, 100, y3, 0, 0, 0x7c, y2, 0x17);
        snes_obj_config2_0019b028(g_splash_obj, 0x21c, y3, 0, 0, 0x204, y2, 0x17);

        snes_frontend_sub_00101e8c();
        y_pos = y_next;
    } while (y_next < 0x55);

    /* Third animation phase: 0xfc frames */
    frame = 0xfc;
    do {
        snes_frontend_sub_00101b04();
        frame--;
        /* ... continues */
    } while (frame >= 0);
}