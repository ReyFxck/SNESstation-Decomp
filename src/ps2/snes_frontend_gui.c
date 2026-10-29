/*
 * SNES Station v0.23 - Frontend Top-Level GUI
 * Canonical source from progress28_structural_lift.c (0x00102ab0 range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern uint8_t  g_rom_type;                    /* cRam1fc7ff52 / 0x00345565 */
extern uint32_t g_unk_1bb2cc;                  /* 0x001bb2cc */
extern void    *g_gui_list_mem;                /* 0x00427010 */
extern void    *g_vram_base;                   /* 0x001baf08 / DAT_001bb2c0 */
extern uint32_t g_unk_1bb87c;                  /* 0x001bb87c */
extern uint32_t g_unk_1ebd48;                  /* 0x001ebd48 */
extern uint32_t g_unk_1ebd4c;                  /* 0x001ebd4c */
extern uint32_t g_unk_1bb2c4;                  /* 0x001bb2c4 */
extern uint32_t g_unk_1bb2c8;                  /* 0x001bb2c8 (float as u64) */
extern uint32_t g_unk_1bb340;                  /* 0x001bb340 */
extern uint32_t g_unk_1ebaec;                  /* 0x001ebaec */
extern uint32_t g_unk_1ebd40;                  /* 0x001ebd40 */
extern uint32_t g_unk_1ebd41;                  /* 0x001ebd41 */
extern uint32_t g_unk_1ebd42;                  /* 0x001ebd42 */
extern uint32_t g_unk_1ebd43;                  /* 0x001ebd43 */
extern uint32_t g_unk_1ebd44;                  /* 0x001ebd44 */
extern uint32_t g_unk_1ebd45;                  /* 0x001ebd45 */
extern uint32_t g_unk_1ebd46;                  /* 0x001ebd46 */
extern uint32_t g_unk_2ec1f4;                  /* 0x002ec1f4 */
extern uint32_t g_unk_2ebc30;                  /* 0x002ebc30 */
extern uint32_t g_unk_2ec200;                  /* 0x002ec200 */
extern uint32_t g_unk_2ec53c;                  /* 0x002ec53c */
extern uint32_t g_unk_1b0ba8;                  /* 0x001b0ba8 */
extern uint32_t g_unk_1b0ba0;                  /* 0x001b0ba0 */
extern uint32_t g_unk_1bbd44;                  /* 0x001bbd44 */
extern uint32_t g_unk_345501;                  /* 0x00345501 */

/* External function declarations */
extern void   *snes_obj_alloc_001a9e88(uint32_t size);
extern void    snes_obj_init_00198cc8(void *obj);
extern void    snes_obj_config_00198d78(void *obj, uint64_t p1, uint32_t p2, uint32_t p3, uint32_t p3b, uint32_t p4, uint32_t p5, uint32_t p6);
extern void    snes_frontend_sub_001019a8(void);
extern void    snes_obj_enable_00199b80(void *obj, uint32_t enable);
extern uint32_t snes_gui_func_00103c7c(uint32_t addr);
extern void    snes_gui_func_0019c0d0(void);
extern uint64_t snes_obj_alloc_001a88a8(uint32_t a, uint32_t b);
extern void    snes_obj_read_001a8864(uint32_t a, uint32_t b, void *buf);
extern uint32_t snes_float_ops_001a3340(float f);
extern uint32_t snes_float_ops_001a35b0(uint32_t a, uint32_t b);
extern uint32_t snes_float_ops_001a3610(uint32_t a, uint32_t b);
extern uint32_t snes_float_ops_001a3cc0(uint32_t a);
extern uint64_t snes_float_to_u64(float f);
extern void    snes_frontend_options_00103314(void);
extern void    snes_wait_controller_001029c4(void);
extern void    snes_mem_set_0019e414(uint32_t addr);
extern uint32_t snes_gui_func_00103cb0(uint32_t idx);
extern void    snes_gui_func_00103d90(uint32_t idx, uint64_t param);
extern void    snes_mem_free_0019e784(void *mem);
extern void    snes_mem_copy_0019c3d4(uint32_t dst, uint32_t src);
extern void    snes_obj_callback_0019a580(void *obj, uint32_t cb);
extern void    snes_frontend_sub_00101b64(void);
extern void    snes_obj_blend_0019b948(uint32_t *link, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, void *src);
extern uint32_t snes_obj_status_00199198(void *obj);
extern void    snes_obj_vram_0019a588(void *obj, uint32_t status, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
extern void    snes_obj_config2_0019b028(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
extern void    snes_obj_wait_001998f8(void *obj);
extern void    snes_obj_setup_00199ef8(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
extern uint64_t snes_gui_func_00103cb0(uint32_t idx);
extern void    snes_frontend_sub_00101e8c(void);
extern void    snes_gui_func_00103cd4(uint32_t idx, uint32_t offset);

/* 0x00102ab0: Top-level GUI main loop */
void snes_top_level_gui_00102ab0(uint64_t param_1)
{
    g_unk_1bb2cc = 1;
    g_gui_list_mem = (void *)snes_mem_alloc_0019e698(0x40, 0x8ca00);

    if (g_vram_base == NULL) {
        void *obj = snes_obj_alloc_001a9e88(0x74);
        snes_obj_init_00198cc8(obj);
        g_vram_base = obj;
    }

    if (g_unk_1bb87c == 1) {
        g_unk_1bb87c = 0;
    } else {
        uint64_t mode = (g_rom_type == 'E') ? 3 : 2;
        snes_obj_config_00198d78(g_vram_base, 0x280, 0x1e0, g_unk_1ebd48, g_unk_1ebd4c, 0, 2, mode);
    }

    snes_frontend_sub_001019a8();
    snes_obj_enable_00199b80(g_vram_base, 1);

    while (1) {
        int32_t count = snes_gui_func_00103c7c(0x1bb340);
        int32_t visible = (count < 0xe) ? count : 0xe;
        snes_gui_func_0019c0d0();

        while (1) {
            uint64_t l2 = snes_obj_alloc_001a88a8(0, 0);
            if (l2 == 6) {
                uint8_t buf[4];
                snes_obj_read_001a8864(0, 0, buf);
                uint16_t buttons = ~(buf[1] | (buf[2] << 8));
                float f = *(float *)&g_unk_1bb2c8;

                if (count > 0) {
                    if ((buttons & 0x40) == 0) {
                        g_unk_1bb2cc = 1;
                    } else if (g_unk_1bb2cc == 1) {
                        break;
                    }

                    if ((buttons & 0x1000) == 0) {
                        if ((buttons & 0x4000) == 0) {
                            if ((buttons & 8) == 0) {
                                if ((buttons & 2) == 0) {
                                    f = 1.0f;
                                    if (((buttons & 4) == 0) && (buttons & 1)) {
                                        f = (float)count;
                                    }
                                } else {
                                    f = *(float *)&g_unk_1bb2c8 + 1.0f;
                                }
                            } else {
                                f = *(float *)&g_unk_1bb2c8 - 1.0f;
                            }
                            goto update_float;
                        }
                        uint32_t u5 = snes_float_ops_001a3340(*(float *)&g_unk_1bb2c8);
                        u5 = snes_float_ops_001a35b0(u5, g_unk_1b0ba8);
                    } else {
                        uint32_t u5 = snes_float_ops_001a3340(*(float *)&g_unk_1bb2c8);
                        u5 = snes_float_ops_001a3610(u5, g_unk_1b0ba0);
                    }
                    f = *(float *)snes_float_ops_001a3cc0(u5);
                }
            update_float:
                g_unk_1bb2c8 = snes_float_to_u64(f);

                if (buttons & 0x10) {
                    snes_frontend_options_00103314();
                }
                if (buttons & 0x80) {
                    *(uint8_t *)(g_unk_2ebc30 + g_unk_2ec1f4) = 0;
                    snes_gui_func_00103b34(0x2ebc30, 0x514);
                }
                if (buttons & 0x100) {
                    snes_wait_controller_001029c4();
                    count = snes_gui_func_00103c7c(0x1bb340);
                    visible = (count < 0xe) ? count : 0xe;
                    g_unk_1bb2c8 = snes_float_to_u64(1.0f);
                    snes_gui_func_0019c0d0();
                }

                /* Analog stick handling */
                if ((buf[0] >> 4) == 7) {
                    float stick_y = (float)(buf[3] - 0x80);
                    if (32.0f < stick_y) {
                        stick_y -= 32.0f;
                        g_unk_1bb2c8 = snes_float_to_u64(*(float *)&g_unk_1bb2c8 + stick_y / 96.0f);
                    }
                    if (stick_y < -32.0f) {
                        g_unk_1bb2c8 = snes_float_to_u64(*(float *)&g_unk_1bb2c8 + (stick_y + 32.0f) / 96.0f);
                    }
                }

                if (*(float *)&g_unk_1bb2c8 < 1.0f) {
                    g_unk_1bb2c8 = snes_float_to_u64(1.0f);
                }
                if (count < (int)*(float *)&g_unk_1bb2c8) {
                    g_unk_1bb2c8 = snes_float_to_u64((float)count);
                }
            }

            int32_t half = visible / 2;
            int32_t sel = (int)*(float *)&g_unk_1bb2c8;
            int32_t start = 1;
            if ((half < sel) && (start = sel - half, (count - half) + 1 <= sel)) {
                start = (count - visible) + 1;
            }

            if (count > 0 && visible > 0) {
                for (int32_t i = 0; i < visible; i++) {
                    int32_t offset = i * 0x100;
                    int32_t idx = start + i;
                    snes_gui_func_00103cd4(idx, offset + 0x427018);
                }
            }

            snes_frontend_sub_00101b64();
            snes_obj_blend_0019b948(0x1bb748, 0, 0x78, 0x37, 4, 0x80ffffff, 2, 0x1bb2f0);
            snes_obj_blend_0019b948(0x1bb748, 0x21c, 0x280, 0x37, 4, 0x80ffffff, 2, 0x1bb2d0);

            if (count < 1) {
                snes_obj_blend_0019b948(0x1bb748, 100, 0x21c, 0xed, 4, 0x80ffffff, 2, 0x1b0a98);
            } else {
                snes_obj_setup_00199ef8(g_vram_base, 0x78, 0x54, 0x208, 0x186);
                for (int32_t i = 0; i < visible; i++) {
                    uint64_t l2 = snes_gui_func_00103cb0(start + i);
                    int32_t y = i * 0x12 + g_unk_1bb2c4;
                    uint32_t width = 0x80;
                    void *src;
                    uint32_t color;

                    if (l2 == 0) {
                        if (i != (int)*(float *)&g_unk_1bb2c8 - start) {
                            width = 0x80;
                            y = i * 0x12 + g_unk_1bb2c4;
                            goto draw_item;
                        }
                        width = 0x80;
                        y = i * 0x12 + g_unk_1bb2c4;
                        color = 0x80ffffff;
                    } else {
                        y = i * 0x12;
                        snes_obj_blend_0019b948(0x1bb748, 0x80, 0x280, g_unk_1bb2c4 + y, 4, 0x80808080, 1, 0x1b0af8);
                        if (i == (int)*(float *)&g_unk_1bb2c8 - start) {
                            y = g_unk_1bb2c4 + y;
                            width = 0x94;
                            color = 0x80ffffff;
                            goto draw_item;
                        }
                        y = g_unk_1bb2c4 + y;
                        width = 0x94;
                        color = 0x802060ff;
                    }

                draw_item:
                    int32_t offset = i * 0x100;
                    snes_obj_blend_0019b948(0x1bb748, width, 0x280, y, 4, color, 1, offset + 0x427018);
                }
                snes_obj_setup_00199ef8(g_vram_base, 0, 0, 0x280, 0x1e0);
            }

            snes_obj_blend_0019b948(0x1bb748, 0x1b8, 0x271, 0x1ae, 4, 0x80ffffff, 3,
                         "Press Triangle for Options\nPress Square for Credits\n");
            snes_frontend_sub_00101e8c();
        }

        g_unk_1bb2cc = 0;
        snes_mem_set_0019e414(0x1b0b00);
        snes_mem_set_0019e414(0x1b0b18);

        uint64_t l2 = snes_gui_func_00103cb0((int)*(float *)&g_unk_1bb2c8);
        if (l2 != 1) {
            snes_mem_set_0019e414(0x1b0b38);
            snes_gui_func_00103d90((int)*(float *)&g_unk_1bb2c8, param_1);
            snes_mem_set_0019e414(0x1b0b70);
            snes_mem_free_0019e784(g_gui_list_mem);
            snes_mem_set_0019e414(0x1b0b88);
            return;
        }

        snes_mem_copy_0019c3d4(0x1bb340, 0x1b0b30);
        snes_mem_copy_0019c3d4(0x1bb340, (int)*(float *)&g_unk_1bb2c8 * 0x90 + (uint32_t)g_gui_list_mem - 0x84);
        g_unk_1bb2c8 = snes_float_to_u64(1.0f);
    }
}