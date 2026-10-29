/*
 * SNES Station v0.23 - Frontend Options Menu
 * Canonical source from progress28_structural_lift.c (0x00103314, 0x00103dd4 range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern uint32_t g_unk_1bb2cc;                  /* 0x001bb2cc */
extern uint8_t  g_unk_1ebd40;                  /* 0x001ebd40 */
extern uint8_t  g_unk_1ebd41;                  /* 0x001ebd41 */
extern uint8_t  g_unk_1ebd42;                  /* 0x001ebd42 */
extern uint8_t  g_unk_1ebd43;                  /* 0x001ebd43 */
extern uint8_t  g_unk_1ebd44;                  /* 0x001ebd44 */
extern uint8_t  g_unk_1ebd45;                  /* 0x001ebd45 */
extern uint8_t  g_unk_1ebd46;                  /* 0x001ebd46 */
extern uint8_t  g_unk_1ebaec;                  /* 0x001ebaec */
extern void    *g_vram_base;                   /* 0x001bb2c0 / DAT_001baf08 */

/* External function declarations */
extern uint64_t snes_obj_alloc_001a88a8(uint32_t a, uint32_t b);
extern void    snes_obj_read_001a8864(uint32_t a, uint32_t b, void *buf);
extern void    snes_frontend_sub_00101b64(void);
extern void    snes_obj_wait_001998f8(void *obj);
extern void    snes_obj_blend_0019b948(uint32_t *link, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, void *src);
extern void    snes_frontend_sub_00101e8c(void);
extern void    snes_obj_callback_00199070(void *obj, uint32_t x, uint32_t y);
extern uint32_t REG_GS_CSR;                    /* GS Control/Status Register */
extern void    snes_obj_config2_0019b028(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
extern void    snes_obj_config_0019adf8(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
extern void    snes_obj_poll_001991b0(void *obj);
extern void    snes_settings_menu_00103dd4(void);
extern void    snes_file_browser_00106ca0(void);
extern void    snes_config_save_001064c0(void);

/* 0x00103314: Options menu (11 options) */
void snes_frontend_options_00103314(void)
{
    g_unk_1bb2cc = 0;
    bool exit_menu = false;
    uint16_t prev_buttons = 0;
    int32_t selection = 0;

    while (1) {
        uint64_t l3 = snes_obj_alloc_001a88a8(0, 0);
        uint16_t buttons = prev_buttons;

        if (l3 == 6) {
            uint8_t buf[2];
            snes_obj_read_001a8864(0, 0, buf);
            buttons = ~(buf[0] | (buf[1] << 8));

            if (((buttons & 0x1000) != 0) && ((prev_buttons & 0x1000) == 0)) {
                selection--;
                if (selection < 0) selection = 10;
            }
            if (((buttons & 0x4000) != 0) && ((prev_buttons & 0x4000) == 0)) {
                selection++;
                if (selection > 10) selection = 0;
            }
            if ((buttons & 0x40) == 0) {
                g_unk_1bb2cc = 1;
            } else if (g_unk_1bb2cc == 1) {
                g_unk_1bb2cc = 0;
                switch (selection) {
                case 0:  /* Option 0: Toggle */
                    g_unk_1ebd40 = g_unk_1ebd40 ? 0 : 1;
                    break;
                case 1:  /* Option 1: Toggle */
                    g_unk_1ebd41 = g_unk_1ebd41 ? 0 : 1;
                    break;
                case 2:  /* Option 2: Cycle 1-3 */
                    g_unk_1ebd42++;
                    if (g_unk_1ebd42 > 3) g_unk_1ebd42 = 1;
                    break;
                case 3:  /* Option 3: Toggle */
                    g_unk_1ebd43 = g_unk_1ebd43 ? 0 : 1;
                    break;
                case 4:  /* Option 4: Toggle */
                    g_unk_1ebd45 = g_unk_1ebd45 ? 0 : 1;
                    break;
                case 5:  /* Option 5: Cycle 1-3 */
                    g_unk_1ebd46++;
                    if (g_unk_1ebd46 > 3) g_unk_1ebd46 = 1;
                    break;
                case 6:  /* Option 6: Settings submenu */
                    snes_settings_menu_00103dd4();
                    goto after_draw;
                case 7:  /* Option 7: File browser */
                    snes_file_browser_00106ca0();
                    goto after_draw;
                case 8:  /* Option 8: Toggle */
                case 9:  /* Option 9: Toggle */
                    g_unk_1ebd44 = g_unk_1ebd44 ? 0 : 1;
                    break;
                case 10: /* Option 10: Save config and exit */
                    exit_menu = true;
                    break;
                }
            }
        }

    after_draw:
        int32_t i9 = 0;
        snes_frontend_sub_00101b64();
        snes_obj_wait_001998f8(g_vram_base);
        snes_obj_blend_0019b948(0x1bb748, 0, 0x280, 0x6e, 4, 0x80ffffff, 2, 0x1b0bb0);

        while (i9 < 0xb) {
            uint32_t color = 0x80ffffff;
            if (i9 != selection) color = 0x802060ff;

            int32_t y = i9 * 0x14 + 0x91;
            uint32_t u8;

            switch (i9) {
            case 0:
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, 0x1b0bc0);
                u8 = g_unk_1ebd40 ? 0x1b0bd0 : 0x1b0bd8;
                break;
            case 1:
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, 0x1b0be0);
                u8 = g_unk_1ebd41 ? 0x1b0bf8 : 0x1b0c00;
                break;
            case 2:
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, 0x1b0c08);
                if (g_unk_1ebd42 == 2) {
                    u8 = 0x1b0c20;
                } else if (g_unk_1ebd42 < 3) {
                    u8 = 0x1b0c30;
                    if (g_unk_1ebd42 != 1) {
                        u8 = 0x1b0c38;
                    }
                } else {
                    u8 = 0x1b0c28;
                    if (g_unk_1ebd42 != 3) u8 = 0x1b0c38;
                }
                break;
            case 3:
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, 0x1b0c40);
                u8 = g_unk_1ebd43 ? 0x1b0bd0 : 0x1b0bd8;
                break;
            case 4:
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, 0x1b0c58);
                u8 = g_unk_1ebd45 ? 0x1b0c70 : 0x1b0c78;
                break;
            case 5:
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, 0x1b0c80);
                if (g_unk_1ebd46 == 2) {
                    u8 = 0x1b0c90;
                } else if (g_unk_1ebd46 < 3) {
                    u8 = 0x1b0ca0;
                    if (g_unk_1ebd46 != 1) u8 = 0x1b0c98;
                } else {
                    u8 = 0x1b0c98;
                    if (g_unk_1ebd46 != 3) u8 = 0x1b0c98;
                }
                break;
            case 6:
                u8 = 0x1b0ca8;
                break;
            case 7:
                u8 = 0x1b0cc0;
                break;
            case 8:
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, 0x1b0cd8);
                u8 = g_unk_1ebd44 ? 0x1b0ce8 : 0x1b0cf0;
                break;
            case 9:
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, 0x1b0cf8);
                u8 = g_unk_1ebd44 ? 0x1b0d08 : 0x1b0d10;
                break;
            case 10:
                u8 = 0x1b0d18;
                break;
            default:
                i9++;
                continue;
            }

            if (i9 >= 6 && i9 <= 7) {
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, u8);
            } else if (i9 == 10) {
                snes_obj_blend_0019b948(0x1bb748, 0xd4, 0x280, y, 4, color, 1, u8);
            } else {
                snes_obj_blend_0019b948(0x1bb748, 0x17a, 0x280, y, 4, color, 1, u8);
            }
            i9++;
        }

        snes_frontend_sub_00101e8c();
        prev_buttons = buttons;

        if (exit_menu) {
            if (g_unk_1ebaec == 1) {
                snes_config_save_001064c0();
            }
            return;
        }
    }
}

/* 0x00103dd4: Settings submenu (video adjust) */
void snes_settings_menu_00103dd4(void)
{
    uint32_t csr = REG_GS_CSR;
    REG_GS_CSR = csr & 8;
    do { csr = REG_GS_CSR; } while ((csr & 8) == 0);
    csr = REG_GS_CSR;
    REG_GS_CSR = csr & 8;
    do { csr = REG_GS_CSR; } while ((csr & 8) == 0);

    while (1) {
        snes_frontend_sub_00101b04();

        /* Draw adjustment rectangle */
        snes_obj_vram_0019a588(g_vram_base, 0, 0, 0, 0x78, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0, 0, 0x3c, 0, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0, 0x3c, 0x3c, 0x3c, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0x27f, 0, 0x243, 0, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0x27f, 0, 0x27f, 0x78, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0x27f, 0x3c, 0x243, 0x3c, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0, 0x1df, 0x3c, 0x1df, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0, 0x1df, 0, 0x167, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0, 0x1a3, 0x3c, 0x1a3, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0x27f, 0x1df, 0x243, 0x1df, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0x27f, 0x1df, 0x27f, 0x167, 1, 0xffffffff80ffffff);
        snes_obj_vram_0019a588(g_vram_base, 0x27f, 0x1a3, 0x243, 0x1a3, 1, 0xffffffff80ffffff);

        snes_obj_blend_0019b948(0x1bb748, 0, 0x280, 0xe0, 2, 0x80ffffff, 2, 0x1b0db0);

        uint64_t l2 = snes_obj_alloc_001a88a8(0, 0);
        if (l2 == 6) {
            uint8_t buf[2];
            snes_obj_read_001a8864(0, 0, buf);
            uint16_t buttons = ~(buf[0] | (buf[1] << 8));

            if ((buttons & 0x1000) && g_unk_1ebd4c > 0) g_unk_1ebd4c--;
            if ((buttons & 0x4000) && g_unk_1ebd4c < 0xff) g_unk_1ebd4c++;
            if ((buttons & 0x8000) && g_unk_1ebd48 > 0) g_unk_1ebd48--;
            if ((buttons & 0x2000) && g_unk_1ebd48 < 0xff) g_unk_1ebd48++;

            if ((buttons & 0x40) == 0) {
                g_unk_1bb2cc = 1;
            } else if (g_unk_1bb2cc == 1) {
                g_unk_1bb2cc = 0;
                snes_config_save_001064c0();
                return;
            }
        }

        snes_obj_callback_00199070(g_vram_base, g_unk_1ebd48, g_unk_1ebd4c);
        snes_obj_wait_001998f8(g_vram_base);

        csr = REG_GS_CSR;
        REG_GS_CSR = csr & 8;
        do { csr = REG_GS_CSR; } while ((csr & 8) == 0);
        snes_obj_poll_001991b0(g_vram_base);
    }
}