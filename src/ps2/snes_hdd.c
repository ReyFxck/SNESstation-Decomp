/*
 * SNES Station v0.23 - HDD Detection and Check
 * Canonical source from progress28_structural_lift.c (0x0010689c range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern uint32_t g_unk_1ec1c4;                  /* 0x001ec1c4 */
extern void    *g_vram_base;                   /* 0x001bb2c0 */

/* External function declarations */
extern void    snes_frontend_sub_001019a8(void);
extern int32_t snes_check_00106824(void);
extern void    snes_hdd_func_001a1204(uint32_t a, uint32_t b, uint32_t path, uint32_t c, uint32_t d, void *buf);
extern void    snes_file_stat_001a1974(uint32_t a, uint32_t b, int32_t *out);
extern void    snes_gui_init_0019c0d0(void);
extern void    snes_obj_func_001a0a68(uint32_t a, uint32_t b, int32_t *out1, int32_t *out2, void *buf);
extern void    snes_file_stat_001a1974b(uint32_t a, uint32_t b, int32_t *out);
extern uint32_t snes_credits_func_00107358(void);
extern void    snes_obj_config_0019adf8(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
extern void    snes_frontend_sub_00101b64(void);
extern void    snes_obj_blend_0019b948(uint32_t *link, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, void *src);
extern void    snes_obj_vram_0019a588(void *obj, uint32_t status, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
extern void    snes_obj_config2_0019b028(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
extern uint64_t snes_obj_alloc_001a88a8(uint32_t a, uint32_t b);
extern void    snes_obj_read_001a8864(uint32_t a, uint32_t b, void *buf);
extern void    snes_frontend_sub_00101e8c(void);
extern uint32_t REG_GS_CSR;                    /* GS Control/Status Register */
extern void    snes_obj_wait_001998f8(void *obj);
extern void    snes_obj_poll_001991b0(void *obj);

/* 0x0010689c: HDD check */
int32_t snes_hdd_check_0010689c(void)
{
    snes_frontend_sub_001019a8();
    g_unk_1ec1c4 = 0;

    int32_t l1 = snes_check_00106824();
    if (l1 != 0) {
        uint8_t buf[64];
        snes_hdd_func_001a1204(0, 0, 0x1b1268, 0, 1, buf);
        snes_file_stat_001a1974(0, 0, (int32_t *)0x427018);
        if (*(int32_t *)0x427018 > 0) {
            g_unk_1ec1c4 = 1;
            return 1;
        }
        int32_t local_2c, local_28;
        snes_obj_func_001a0a68(0, 0, &local_2c, &local_28, (void *)0x427018);
        snes_file_stat_001a1974b(0, 0, (int32_t *)0x427018);
        if ((local_2c == 2) && (99 < local_28)) {
            return snes_credits_func_00107358();
        }
    }

    snes_obj_alloc_001a88a8(0, 0);
    snes_gui_init_0019c0d0();
    snes_obj_func_001a0a68(0, 0, &local_2c, &local_28, (void *)0x427018);
    snes_file_stat_001a1974b(0, 0, (int32_t *)0x427018);

    while (1) {
        if ((local_2c == 2) && (99 < local_28)) {
            uint8_t buf[64];
            snes_hdd_func_001a1204(0, 0, 0x1b1268, 0, 1, buf);
            snes_file_stat_001a1974(0, 0, (int32_t *)0x427018);
            if ((*(int32_t *)0x427018 > 0) && (g_unk_1ec1c4 = 1, *(int32_t *)0x427018 > 0)) {
                g_unk_1ec1c4 = 1;
                return 1;
            }
            snes_credits_func_00107358();
            return 1;
        }

        snes_obj_config_0019adf8(g_vram_base, 0, 0, 0x280, 0x1e0, 0, 0xffffffff80000000);
        snes_frontend_sub_00101b64();

        if (local_2c != 2) {
            snes_obj_blend_0019b948(0x1bb748, 0, 0x280, 0x78, 2, 0x80ffffff, 2, 0x1b1278);
        }
        if ((local_2c == 2) && (local_28 < 100)) {
            snes_obj_blend_0019b948(0x1bb748, 0, 0x280, 0x78, 2, 0x80ffffff, 2, 0x1b1310);
        }
        snes_obj_blend_0019b948(0x1bb748, 0, 0x280, 0xa0, 2, 0x80ffffff, 2, 0x1b12a0);
        snes_obj_blend_0019b948(0x1bb748, 0, 0x280, 200, 2, 0x80ffffff, 2, 0x1b12e0);

        uint8_t buf[3];
        snes_obj_read_001a8864(0, 0, buf);
        if ((~buf[2] & 0x10) != 0) break;

        snes_frontend_sub_00101e8c();
        uint32_t csr = REG_GS_CSR;
        REG_GS_CSR = csr & 8;
        do { csr = REG_GS_CSR; } while ((csr & 8) == 0);

        snes_obj_func_001a0a68(0, 0, &local_2c, &local_28, (void *)0x427018);
        snes_file_stat_001a1974b(0, 0, (int32_t *)0x427018);
    }

    return 0;
}