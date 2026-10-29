/*
 * SNES Station v0.23 - Frontend Menu and Input Handling
 * Canonical source from progress28_structural_lift.c (0x001008dc-0x00101fff range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern uint8_t  g_rom_type;                    /* 0x00345565 */
extern uint8_t  g_unk_345608;                  /* 0x00345608 */
extern uint8_t  g_unk_34556c;                  /* 0x0034556c */
extern uint8_t  g_unk_34e2d4;                  /* 0x0034e2d4 */
extern uint32_t g_screen_width;                /* 0x0035d490 */
extern void    *g_framebuffer1;                /* 0x0035d480 */
extern void    *g_vram_base;                   /* 0x001baf08 */
extern uint32_t g_unk_1baf20;                  /* 0x001baf20 */
extern uint32_t g_unk_1bb2cc;                  /* 0x001bb2cc */
extern uint32_t g_unk_1bbd44;                  /* 0x001bbd44 */
extern uint32_t g_unk_1ebaec;                  /* 0x001ebaec */
extern uint32_t g_unk_1ebd48;                  /* 0x001ebd48 */
extern uint32_t g_unk_1ebd4c;                  /* 0x001ebd4c */
extern uint32_t g_unk_1ebd50;                  /* 0x001ebd50 */
extern uint32_t g_unk_1ebd54;                  /* 0x001ebd54 */
extern uint32_t g_unk_1ebd58;                  /* 0x001ebd58 */
extern uint32_t g_unk_1ebd5c;                  /* 0x001ebd5c */
extern uint32_t g_unk_35c278;                  /* 0x0035c278 */
extern uint32_t g_unk_3592fc;                  /* 0x003592fc */
extern uint32_t g_unk_427000;                  /* 0x00427000 */
extern uint32_t g_unk_427004;                  /* 0x00427004 */
extern uint32_t g_unk_427008;                  /* 0x00427008 */
extern uint8_t  g_unk_1fc7ff52;                /* 0x1fc7ff52 */

/* External function declarations */
extern uint32_t REG_GS_CSR;                    /* GS Control/Status Register */
extern void    syscall(int n);
extern void    snes_obj_set_00199aa8(void *obj, uint32_t val);
extern void    snes_obj_config_0019adf8(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
extern void    snes_obj_start_001990f8(void *obj);
extern void    snes_obj_callback_0019a580(void *obj, uint32_t cb);
extern uint32_t snes_obj_status_00199198(void *obj);
extern void    snes_obj_setup_00199f88(void *obj, uint32_t status, uint32_t a, uint32_t b, uint32_t c, uint32_t d, void *fb, uint32_t e);
extern void    snes_obj_vram_0019a588(void *obj, uint32_t status, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
extern void    snes_obj_config2_0019b028(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
extern void    snes_copy_0019e3d0(void *dst, uint32_t src, uint32_t a, uint32_t b);
extern void    snes_obj_blend_0019b948(uint32_t *link, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, void *src);
extern void    snes_obj_wait_001998f8(void *obj);
extern void    snes_obj_poll2_00199268(void *obj);
extern uint32_t snes_obj_poll_001991e8(void *obj);
extern void    snes_obj_wait2_00199208(void *obj);
extern uint64_t snes_obj_alloc_001a88a8(uint32_t a, uint32_t b);
extern void    snes_obj_read_001a8864(uint32_t a, uint32_t b, void *buf);
extern void    snes_obj_link_0019b7f0(uint32_t *link, void *ptr, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
extern void    snes_obj_init_00198cc8(void *obj);
extern void   *snes_obj_alloc_001a9e88(uint32_t size);
extern void    snes_obj_config_00198d78(void *obj, uint64_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7);
extern uint32_t snes_menu_init_001057fc(uint32_t mode);
extern uint32_t snes_menu_load_0019cfc0(uint32_t menu, uint32_t flag);
extern void    snes_menu_exec_0019d090(uint32_t menu);
extern void    snes_file_browse_001711e8(uint32_t menu);
extern void    snes_state_load_0017124c(uint32_t menu);
extern uint32_t snes_menu_init2_00105750(void);
extern void    snes_path_set_001534b8(uint32_t addr, uint32_t menu);
extern void    snes_path_set2_00153354(uint32_t addr, uint32_t menu);
extern void    snes_frontend_sub_001019a8(void);
extern void    snes_frontend_sub_00101b04(void);
extern void    snes_frontend_sub_00101e8c(void);
extern void    snes_frontend_sub_0010773c(uint32_t mask);
extern void    snes_frontend_sub_001a0a68(uint32_t a, uint32_t b, void *buf, uint32_t *out, void *buf2);
extern void    snes_frontend_sub_001a1974(uint32_t a, uint32_t b, void *buf);
extern void    snes_frontend_sub_001064c0(void);

/* 0x001008dc: Frontend menu input loop (settings/configuration screen) */
void snes_frontend_menu_loop_001008dc(void)
{
    int32_t val_s4 = g_unk_1ebd50;
    int32_t val_s5 = g_unk_1ebd54;

    if (g_unk_427000 != 2) {
        val_s5 = g_unk_427008;  /* unaff_s5_lo */
        val_s4 = g_unk_427004;  /* unaff_s4_lo */
        if (g_unk_427000 == 3) {
            val_s5 = g_unk_1ebd5c;
            val_s4 = g_unk_1ebd58;
        }
    }

    /* Wait for GS CSR bit 3 (VBlank?) */
    uint32_t csr = REG_GS_CSR;
    REG_GS_CSR = csr & 8;
    do {
        csr = REG_GS_CSR;
    } while ((csr & 8) == 0);

    csr = REG_GS_CSR;
    REG_GS_CSR = csr & 8;
    do {
        csr = REG_GS_CSR;
    } while ((csr & 8) == 0);

    int32_t i8 = g_unk_427008 * 0xc;
    if (i8 < 0) i8 += 0xf;
    i8 = (g_unk_427008 - (i8 >> 4)) / 2;

    while (1) {
        snes_obj_set_00199aa8(g_vram_base, 0);
        snes_obj_config_0019adf8(g_vram_base, 0, 0, g_unk_427004, g_unk_427008, 0, 0xffffffff80000000);
        snes_obj_set_00199aa8(g_vram_base, 1);

        snes_obj_vram_0019a588(g_vram_base, 0, 0, 0, i8 * 2, 1, 0xffffffff80ffffff);

        int32_t i5 = g_unk_427004;
        if (i5 < 0) i5 += 7;
        snes_obj_vram_0019a588(g_vram_base, 0, 0, i5 >> 3, 0, 1, 0xffffffff80ffffff);

        i5 = g_unk_427004;
        if (i5 < 0) i5 += 7;
        snes_obj_vram_0019a588(g_vram_base, 0, i8, i5 >> 3, i8, 1, 0xffffffff80ffffff);

        i5 = g_unk_427004 + 7;
        if (g_unk_427004 >= 0) i5 = g_unk_427004;
        snes_obj_vram_0019a588(g_vram_base, g_unk_427004 - 1, 0, (g_unk_427004 - (i5 >> 3)) - 1, 0, 1, 0xffffffff80ffffff);

        snes_obj_vram_0019a588(g_vram_base, g_unk_427004 - 1, 0, g_unk_427004 - 1, i8 * 2, 1, 0xffffffff80ffffff);

        i5 = g_unk_427004 + 7;
        if (g_unk_427004 >= 0) i5 = g_unk_427004;
        snes_obj_vram_0019a588(g_vram_base, g_unk_427004 - 1, i8, (g_unk_427004 - (i5 >> 3)) - 1, i8, 1, 0xffffffff80ffffff);

        i5 = g_unk_427004;
        if (i5 < 0) i5 += 7;
        snes_obj_vram_0019a588(g_vram_base, 0, g_unk_427008 - 1, i5 >> 3, g_unk_427008 - 1, 1, 0xffffffff80ffffff);

        snes_obj_vram_0019a588(g_vram_base, 0, g_unk_427008 - 1, 0, g_unk_427008 - 1 + i8 * -2 - 1, 1, 0xffffffff80ffffff);

        i5 = g_unk_427004;
        if (i5 < 0) i5 += 7;
        int32_t i4 = (g_unk_427008 - i8) - 1;
        snes_obj_vram_0019a588(g_vram_base, 0, i4, i5 >> 3, i4, 1, 0xffffffff80ffffff);

        i5 = g_unk_427004 + 7;
        if (g_unk_427004 >= 0) i5 = g_unk_427004;
        snes_obj_vram_0019a588(g_vram_base, g_unk_427004 - 1, g_unk_427008 - 1, (g_unk_427004 - (i5 >> 3)) - 1, g_unk_427008 - 1, 1, 0xffffffff80ffffff);

        snes_obj_vram_0019a588(g_vram_base, g_unk_427004 - 1, g_unk_427008 - 1, g_unk_427004 - 1, g_unk_427008 - 1 + i8 * -2 - 1, 1, 0xffffffff80ffffff);

        i5 = g_unk_427004 + 7;
        if (g_unk_427004 >= 0) i5 = g_unk_427004;
        i4 = (g_unk_427008 - i8) - 1;
        snes_obj_vram_0019a588(g_vram_base, g_unk_427004 - 1, i4, (g_unk_427004 - (i5 >> 3)) - 1, i4, 1, 0xffffffff80ffffff);

        snes_obj_blend_0019b948(&g_unk_1baf20, 0, g_unk_427004, (g_unk_427008 - 0x10) / 2, 2, 0x80ffffff, 2, 0x1b0890);

        uint64_t l2 = snes_obj_alloc_001a88a8(0, 0);
        if (l2 == 6) {
            uint8_t buf[2];
            snes_obj_read_001a8864(0, 0, buf);
            uint16_t buttons = ~(buf[0] | (buf[1] << 8));

            if ((buttons & 0x1000) && val_s5 > 0) val_s5--;
            if ((buttons & 0x4000) && val_s5 < 0xff) val_s5++;
            if ((buttons & 0x8000) && val_s4 > 0) val_s4--;
            if ((buttons & 0x2000) && val_s4 < 0xff) val_s4++;

            if ((buttons & 0x40) == 0) {
                g_unk_1bb2cc = 1;
            } else if (g_unk_1bb2cc == 1) {
                g_unk_1bb2cc = 0;
                int32_t new_s5 = val_s5;
                int32_t new_s4 = val_s4;
                if (g_unk_427000 != 2) {
                    new_s5 = g_unk_1ebd50;
                    new_s4 = g_unk_1ebd54;
                    if (g_unk_427000 == 3) {
                        g_unk_1ebd58 = val_s5;
                        g_unk_1ebd5c = val_s4;
                    }
                }
                g_unk_1ebd54 = new_s5;
                g_unk_1ebd50 = new_s4;
                snes_frontend_sub_001064c0();
                return;
            }
        }

        snes_obj_callback_00199070(g_vram_base, val_s4, val_s5);
        snes_obj_wait_001998f8(g_vram_base);

        csr = REG_GS_CSR;
        REG_GS_CSR = csr & 8;
        do {
            csr = REG_GS_CSR;
        } while ((csr & 8) == 0);

        snes_obj_poll_001991b0(g_vram_base);
    }
}

/* 0x00100e78: Main menu with 7 options */
void snes_frontend_main_menu_00100e78(void)
{
    int32_t menu_selection = 0;
    snes_frontend_sub_0010773c(0);
    snes_obj_wait_001998f8(g_vram_base);

    uint16_t prev_buttons = 0;

    while (1) {
        uint32_t w = 0x200;
        snes_obj_set_00199aa8(g_vram_base, 0);

        uint32_t w2 = (g_rom_type == 0) ? 0x100 : 0x200;
        uint32_t fb_count = (g_rom_type == 0) ? 0xef : 0x1de;

        snes_obj_config_0019adf8(g_vram_base, 0, 0, w2, fb_count, 0, 0xffffffff80000000);

        uint32_t status = snes_obj_status_00199198(g_vram_base);
        uint32_t w3 = (g_rom_type == 0) ? 0x102 : 0x202;
        snes_obj_setup_00199f88(g_vram_base, status, w2, 0, 0, 2, g_framebuffer1, w3);

        status = snes_obj_status_00199198(g_vram_base);
        snes_obj_vram_0019a588(g_vram_base, status, 0x100, 8, 8, 2, 0, 0);

        fb_count = (g_rom_type == 0) ? 0xef : 0x1de;
        w2 = (g_rom_type == 0) ? 0x100 : 0x200;
        snes_obj_config2_0019b028(g_vram_base, 0, 0, 0, 0, w2, fb_count, w2);

        snes_obj_config_0019adf8(g_vram_base, 0x20, 0x20, 0xe0, 0xc0, 1, 0x40d08024);

        uint64_t l5 = snes_obj_alloc_001a88a8(0, 0);
        uint16_t buttons = prev_buttons;

        if (l5 == 6) {
            uint8_t buf[2];
            snes_obj_read_001a8864(0, 0, buf);
            buttons = ~(buf[0] | (buf[1] << 8));

            if ((buttons & 0x1000) && !(prev_buttons & 0x1000)) {
                menu_selection--;
                if (menu_selection < 0) menu_selection = 6;
                if (g_unk_1ebaec != 1 && menu_selection == 4) menu_selection -= 4;
                if (g_unk_34e2d4 == 0 && menu_selection == 4) menu_selection -= 2;
            }
            if ((buttons & 0x4000) && !(prev_buttons & 0x4000)) {
                menu_selection++;
                if (menu_selection > 6) menu_selection = 0;
                if (g_unk_1ebaec != 1 && menu_selection == 1) menu_selection += 4;
                if (g_unk_34e2d4 == 0 && menu_selection == 3) menu_selection += 2;
            }
            if (buttons & 0x40) {
                do {
                    snes_obj_read_001a8864(0, 0, buf);
                    buttons = ~(buf[0] | (buf[1] << 8));
                } while (buttons & 0x40);

                switch (menu_selection) {
                case 0:  /* Load ROM */
                    goto menu_load_rom;

                case 1:  /* Option 1 */
                    {
                        uint32_t menu = snes_menu_init_001057fc(1);
                        int32_t result = snes_menu_load_0019cfc0(menu, 1);
                        if (result < 0) {
                            uint32_t buf1[2], buf2[2], buf3[2];
                            uint32_t ac;
                            snes_frontend_sub_001a0a68(0, 0, buf1, &ac, buf2);
                            snes_frontend_sub_001a1974(0, 0, buf3);
                            if (ac < 100) {
                                snes_obj_blend_0019b948(&g_unk_1baf20, 0x20, 0xe0, 0x58, 2, 0x80ffffff, 2, 0x1b08d8);
                                snes_obj_poll2_00199268(g_vram_base);
                                do {
                                    l5 = snes_obj_poll_001991e8(g_vram_base);
                                } while (l5 == 0);
                                snes_obj_wait2_00199208(g_vram_base);

                                int32_t i9 = 0;
                                do {
                                    csr = REG_GS_CSR;
                                    REG_GS_CSR = csr & 8;
                                    do {
                                        csr = REG_GS_CSR;
                                    } while ((csr & 8) == 0);
                                    i9++;
                                } while (i9 < 100);
                                continue;
                            }
                        }
                        snes_menu_exec_0019d090(result);
                        menu = snes_menu_init_001057fc(1);
                        snes_file_browse_001711e8(menu);
                    }
                    goto menu_load_rom;

                case 2:  /* Option 2 */
                    {
                        uint32_t menu = snes_menu_init_001057fc(1);
                        int32_t result = snes_menu_load_0019cfc0(menu, 1);
                        if (result >= 0) {
                            snes_menu_exec_0019d090(result);
                            menu = snes_menu_init_001057fc(1);
                            snes_state_load_0017124c(menu);
                        }
                        snes_obj_blend_0019b948(&g_unk_1baf20, 0x20, 0xe0, 0x60, 2, 0x80ffffff, 2, 0x1b0910);
                        snes_obj_poll2_00199268(g_vram_base);
                        do {
                            l5 = snes_obj_poll_001991e8(g_vram_base);
                        } while (l5 == 0);
                        snes_obj_wait2_00199208(g_vram_base);

                        int32_t i9 = 0;
                        do {
                            csr = REG_GS_CSR;
                            REG_GS_CSR = csr & 8;
                            do {
                                csr = REG_GS_CSR;
                            } while ((csr & 8) == 0);
                            i9++;
                        } while (i9 < 100);
                        continue;
                    }

                case 3:  /* Option 3 */
                    {
                        uint32_t buf1[2], buf2[2], buf3[2];
                        uint32_t ac;
                        snes_frontend_sub_001a0a68(0, 0, buf1, &ac, buf2);
                        snes_frontend_sub_001a1974(0, 0, buf3);
                        if (ac > 1) {
                            uint32_t menu = snes_menu_init2_00105750();
                            snes_path_set_001534b8(0x34e2b0, menu);
                        }
                        snes_obj_blend_0019b948(&g_unk_1baf20, 0x20, 0xe0, 0x58, 2, 0x80ffffff, 2, 0x1b0930);
                        snes_obj_poll2_00199268(g_vram_base);
                        do {
                            l5 = snes_obj_poll_001991e8(g_vram_base);
                        } while (l5 == 0);
                        snes_obj_wait2_00199208(g_vram_base);

                        int32_t i9 = 0;
                        do {
                            csr = REG_GS_CSR;
                            REG_GS_CSR = csr & 8;
                            do {
                                csr = REG_GS_CSR;
                            } while ((csr & 8) == 0);
                            i9++;
                        } while (i9 < 100);
                        continue;
                    }

                case 4:  /* Option 4 */
                    {
                        uint32_t menu = snes_menu_init2_00105750();
                        int32_t result = snes_menu_load_0019cfc0(menu, 1);
                        if (result >= 0) {
                            snes_menu_exec_0019d090(result);
                            menu = snes_menu_init2_00105750();
                            snes_path_set2_00153354(0x34e2b0, menu);
                            goto menu_load_rom;
                        }
                        snes_obj_blend_0019b948(&g_unk_1baf20, 0x20, 0xe0, 0x60, 2, 0x80ffffff, 2, 0x1b0968);
                        snes_obj_poll2_00199268(g_vram_base);
                        do {
                            l5 = snes_obj_poll_001991e8(g_vram_base);
                        } while (l5 == 0);
                        snes_obj_wait2_00199208(g_vram_base);

                        int32_t i9 = 0;
                        do {
                            csr = REG_GS_CSR;
                            REG_GS_CSR = csr & 8;
                            do {
                                csr = REG_GS_CSR;
                            } while ((csr & 8) == 0);
                            i9++;
                        } while (i9 < 100);
                        continue;
                    }

                case 5:  /* Settings/Config */
                    snes_frontend_menu_loop_001008dc();
                    break;

                case 6:  /* Exit */
                    g_unk_1bbd44 = 1;
                    g_unk_345501 = 0;
                    return;
                }
            }
        }

    menu_load_rom:
        g_unk_345501 = 0;
        snes_frontend_sub_0010773c(0x3fff);
        return;

        /* Draw menu items */
        snes_obj_blend_0019b948(&g_unk_1baf20, 0x20, 0xe0, 0x2a, 2, 0x80ffffff, 2, 0x1b0988);

        for (int32_t i9 = 0; i9 < 7; i9++) {
            uint32_t color = 0x80ffffff;
            if (i9 != menu_selection) color = 0x802060ff;

            int32_t y_pos;
            uint32_t texture;
            switch (i9) {
            case 0:
                y_pos = i9 * 0x11 + 0x42;
                texture = 0x1b09a0;
                break;
            case 1:
                if (g_unk_1ebaec != 1) color = 0x80404040;
                y_pos = i9 * 0x11 + 0x42;
                texture = 0x1b09b0;
                break;
            case 2:
                if (g_unk_1ebaec != 1) color = 0x80404040;
                y_pos = i9 * 0x11 + 0x42;
                texture = 0x1b09c0;
                break;
            case 3:
                if (g_unk_1ebaec != 1 || g_unk_34e2d4 == 0) color = 0x80404040;
                y_pos = i9 * 0x11 + 0x42;
                texture = 0x1b09d0;
                break;
            case 4:
                if (g_unk_1ebaec != 1 || g_unk_34e2d4 == 0) color = 0x80404040;
                y_pos = i9 * 0x11 + 0x42;
                texture = 0x1b09e0;
                break;
            case 5:
                y_pos = i9 * 0x11 + 0x42;
                texture = 0x1b09f0;
                break;
            case 6:
                y_pos = i9 * 0x11 + 0x42;
                texture = 0x1b0a08;
                break;
            default:
                continue;
            }
            snes_obj_blend_0019b948(&g_unk_1baf20, 0x40, 0x100, y_pos, 2, color, 1, texture);
        }

        snes_obj_poll2_00199268(g_vram_base);
        do {
            l5 = snes_obj_poll_001991e8(g_vram_base);
        } while (l5 == 0);
        snes_obj_wait2_00199208(g_vram_base);

        prev_buttons = buttons;
    }
}