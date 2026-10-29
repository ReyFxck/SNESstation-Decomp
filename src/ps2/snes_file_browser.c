/*
 * SNES Station v0.23 - File Browser
 * Canonical source from progress28_structural_lift.c (0x00106ca0 range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern void    *g_vram_base;                   /* 0x001bb2c0 */
extern uint32_t g_unk_1bbd44;                  /* 0x001bbd44 */
extern uint32_t g_unk_1bb2c4;                  /* 0x001bb2c4 */
extern uint32_t g_unk_1b1470;                  /* 0x001b1470 */
extern uint32_t g_unk_1b1468;                  /* 0x001b1468 */
extern uint32_t g_unk_1b1370;                  /* 0x001b1370 */
extern uint32_t g_unk_1b13c0;                  /* 0x001b13c0 */
extern uint32_t g_unk_1b1380;                  /* 0x001b1380 */
extern uint32_t g_unk_1b1350;                  /* 0x001b1350 */
extern uint32_t g_unk_1b1360;                  /* 0x001b1360 */
extern uint32_t g_unk_1b13f8;                  /* 0x001b13f8 */
extern uint32_t g_unk_1b13d0;                  /* 0x001b13d0 */
extern uint32_t g_unk_1b1378;                  /* 0x001b1378 */
extern uint32_t g_unk_1b1458;                  /* 0x001b1458 */
extern uint32_t g_unk_1b1450;                  /* 0x001b1450 */
extern uint32_t g_unk_1b1460;                  /* 0x001b1460 */
extern uint32_t g_unk_1b1478;                  /* 0x001b1478 */
extern uint32_t g_unk_1b1480;                  /* 0x001b1480 */

/* External function declarations */
extern void    snes_frontend_sub_001019a8(void);
extern void    snes_obj_func_001a1204(uint32_t a, uint32_t b, uint32_t path, uint32_t c, uint32_t d, void *buf);
extern void    snes_file_stat_001a1974(uint32_t a, uint32_t b, uint32_t *out);
extern uint64_t snes_obj_alloc_001a88a8(uint32_t a, uint32_t b);
extern void    snes_obj_read_001a8864(uint32_t a, uint32_t b, void *buf);
extern uint32_t snes_float_ops_001a3340(float f);
extern uint32_t snes_float_ops_001a35b0(uint32_t a, uint32_t b);
extern uint32_t snes_float_ops_001a3610(uint32_t a, uint32_t b);
extern uint32_t snes_float_ops_001a3cc0(uint32_t a);
extern void    snes_frontend_sub_00101b64(void);
extern void    snes_obj_blend_0019b948(uint32_t *link, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, void *src);
extern void    snes_obj_setup_00199ef8(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
extern uint32_t snes_file_func_00103cb0(uint32_t idx);
extern int32_t snes_file_func_00106bcc(uint32_t path);
extern int32_t snes_file_func_00106c08(uint32_t path, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
extern void    snes_mem_copy_0019e3d0(void *dst, uint32_t src, uint32_t val);
extern void    snes_mem_set_0019c528(void *dst, uint32_t val);
extern void    snes_obj_setup2_00199ef8b(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
extern void    snes_frontend_sub_00101e8c(void);
extern void   *snes_mem_alloc_0019e698(uint32_t align, uint32_t size);
extern void    snes_mem_free_0019e784(void *ptr);
extern uint32_t REG_GS_CSR;                    /* GS Control/Status Register */
extern void    snes_obj_wait_001998f8(void *obj);
extern void    snes_obj_config_0019adf8(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);

/* 0x00106ca0: File browser */
void snes_file_browser_00106ca0(void)
{
    int32_t local_d0 = 0;
    int32_t local_c0 = 0;
    void *list_mem = snes_mem_alloc_0019e698(0x40, 64000);
    snes_frontend_sub_001019a8();

    while (1) {
        int32_t local_c8 = 0xe;
        snes_obj_func_001a1204(0, 0, 0x1b1350, 0, 1000, list_mem);
        int32_t local_c4 = 0;
        snes_file_stat_001a1974(0, 0, &local_d0);
        int32_t i10 = local_d0;
        float f13 = 1.0f;

        snes_obj_func_001a1204(0, 0, 0x1b1360, 0, 1000 - local_d0, (uint32_t)list_mem + local_d0 * 0x40);
        snes_file_stat_001a1974(0, 0, &local_d0);
        i10 += local_d0;

        if (i10 < 0xe) local_c8 = i10;

        while (1) {
            uint64_t l2 = snes_obj_alloc_001a88a8(0, 0);
            if (l2 == 6) {
                uint8_t buf[2];
                snes_obj_read_001a8864(0, 0, buf);
                uint16_t buttons = ~(buf[0] | (buf[1] << 8));

                if (i10 > 0) {
                    if ((buttons & 0x1000) == 0) {
                        if ((buttons & 0x4000) == 0) {
                            if ((buttons & 8) == 0) {
                                if ((buttons & 2) != 0) f13 += 1.0f;
                            } else {
                                f13 -= 1.0f;
                            }
                            goto update_selection;
                        }
                        uint32_t u3 = snes_float_ops_001a3340(f13);
                        u3 = snes_float_ops_001a35b0(u3, g_unk_1b1470);
                    } else {
                        uint32_t u3 = snes_float_ops_001a3340(f13);
                        u3 = snes_float_ops_001a3610(u3, g_unk_1b1468);
                    }
                    f13 = *(float *)snes_float_ops_001a3cc0(u3);
                }

            update_selection:
                if ((buttons & 0x80) != 0) {
                    snes_frontend_sub_00101b64();
                    snes_obj_blend_0019b948(0x1bb748, 100, 0x21c, 0xcd, 2, 0x80ffffff, 2, 0x1b13f8);
                    snes_frontend_sub_00101e8c();
                    do {
                        l2 = snes_obj_alloc_001a88a8(0, 0);
                        if (l2 == 6) {
                            snes_obj_read_001a8864(0, 0, buf);
                            if ((~(buf[0] | (buf[1] << 8)) & 0x100) != 0) {
                                local_c4 = 1;
                                snes_mem_copy_0019e3d0((void *)0x427018, 0x1b1458, (uint32_t)list_mem + (int)f13 * 0x40 - 0x20);
                                snes_file_func_001a147c(0, 0, (void *)0x427018);
                                snes_file_stat_001a1974(0, 0, (void *)0x427018);
                                goto exit_browser;
                            }
                            if ((~(buf[0] | (buf[1] << 8)) & 0x20) != 0) goto show_preview;
                        }
                        uint32_t csr = REG_GS_CSR;
                        REG_GS_CSR = csr & 8;
                        do { csr = REG_GS_CSR; } while ((csr & 8) == 0);
                    } while (1);
                }

                if ((buttons & 0x20) == 0) {
                    local_c0 = 0;
                } else if (local_c0 != 1) {
                    snes_obj_func_001a90f8(list_mem);
                    return;
                }

                if (i10 > 0) {
                    float f14 = (float)(buf[1] - 0x80);
                    if (32.0f < f14) {
                        f14 -= 32.0f;
                        f13 += f14 / 96.0f;
                    }
                    if (f14 < -32.0f) {
                        f13 += (f14 + 32.0f) / 96.0f;
                    }
                    if (f13 < 1.0f) f13 = 1.0f;
                    if (i10 < (int)f13) f13 = (float)i10;
                }
            }

            int32_t i12 = (int)f13;
            int32_t i4 = local_c8 / 2;
            int32_t i11 = 1;
            if ((i4 < i12) && (i11 = i12 - i4, (i10 - i4) + 1 <= i12)) {
                i11 = (i10 - local_c8) + 1;
            }

            snes_frontend_sub_00101b64();

            if (i10 < 1) {
                snes_obj_blend_0019b948(0x1bb748, 100, 0x21c, 0xe5, 2, 0x80ffffff, 2, 0x1b13d0);
            } else {
                snes_obj_setup_00199ef8(g_vram_base, 0x78, 0x54, 0x208, 0x186);
                for (int32_t i4 = 0; i4 < local_c8; i4++) {
                    uint32_t color = 0x80ffffff;
                    if (i4 != (int)f13 - i11) color = 0x802060ff;

                    uint32_t path_offset = i11 * 0x40 + (uint32_t)list_mem - 0x20;
                    snes_mem_set_0019c528((void *)0x427018, path_offset);
                    int32_t l2 = snes_file_func_0019eaa4((void *)0x427018, 0x2e);
                    int32_t y = i4 * 0x12 + 0x6e;

                    void *src;
                    if (l2 != 0) {
                        *(uint8_t *)l2 = 0;
                    }

                    snes_obj_blend_0019b948(0x1bb748, 0x8c, 0x280, y, 2, color, 1, (void *)0x427018);

                    l2 = snes_file_func_00106bcc(path_offset);
                    void *pu6 = &g_unk_1b1370;
                    if (l2 == 0) {
                        pu6 = (void *)0x427018 + 0x110;  // auStack_110
                        l2 = snes_file_func_00106c08(path_offset, 0x181, 0x280, y, 2, color, 1, 0x1b1370);
                        if (l2 != 0) {
                            uint64_t u3 = snes_file_func_00106c08(path_offset);
                            snes_mem_copy_0019e3d0(pu6, 0x1b13c0, u3);
                            goto draw_filename;
                        }
                        uint32_t u5 = *(uint32_t *)(path_offset - 0x10);
                    } else {
                    draw_filename:
                        snes_obj_blend_0019b948(0x1bb748, 0x181, 0x280, y, 2, color, 1, pu6);
                        uint32_t u5 = *(uint32_t *)(path_offset - 0x10);
                    }

                    snes_mem_copy_0019e3d0((void *)0x427018 + 0x100, 0x1b1378, u5 >> 10);  // auStack_100
                    path_offset += 0x40;
                    y = i4 * 0x12;
                    snes_obj_blend_0019b948(0x1bb748, 0x1d1, 0x280, y + 0x6e, 2, color, 1, (void *)0x427018 + 0x100);
                }
                snes_obj_setup2_00199ef8b(g_vram_base, 0, 0, 0x280, 0x1e0);
            }

            snes_obj_blend_0019b948(0x1bb748, 400, 0x271, 0x1ae, 2, 0x80ffffff, 3, 0x1b1380);
            snes_frontend_sub_00101e8c();

        exit_browser:
            if (local_c4 != 0) break;
        }

        snes_mem_free_0019e784(list_mem);
        return;
    }
}