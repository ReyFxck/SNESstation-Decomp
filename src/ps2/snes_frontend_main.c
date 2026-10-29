/*
 * SNES Station v0.23 - Frontend Main Initialization and Loop
 * Canonical source from progress28_structural_lift.c (0x00104f18 range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern void    *g_vram_base;                   /* 0x001baf08 */
extern uint32_t g_unk_1eba80;                  /* 0x001eba80 */
extern uint32_t g_unk_1bb340;                  /* 0x001bb340 */
extern uint32_t g_unk_1bb341;                  /* 0x001bb341 */
extern void    *g_framebuffer1;                /* 0x0035d480 */
extern void    *g_framebuffer2;                /* 0x0035d484 */
extern void    *g_framebuffer3;                /* 0x0035d488 */
extern void    *g_framebuffer4;                /* 0x0035d48c */
extern uint32_t g_unk_2ec53c;                  /* 0x002ec53c */
extern uint32_t g_unk_2ec1f4;                  /* 0x002ec1f4 */
extern uint32_t g_unk_2ebc30;                  /* 0x002ebc30 */
extern uint32_t g_unk_2ec200;                  /* 0x002ec200 */
extern uint32_t g_unk_1ebaec;                  /* 0x001ebaec */
extern uint32_t g_unk_1ec1c4;                  /* 0x001ec1c4 */
extern uint32_t g_unk_1f40d4;                  /* 0x001f40d4 */
extern uint32_t g_unk_1fafa0;                  /* 0x001fafa0 */
extern uint32_t g_unk_1f6140;                  /* 0x001f6140 */
extern uint32_t g_unk_1f60c8;                  /* 0x001f60c8 */
extern uint32_t g_unk_1f4100;                  /* 0x001f4100 */
extern uint32_t g_unk_3454e8;                  /* 0x003454e8 */
extern uint32_t g_unk_3454f0;                  /* 0x003454f0 */
extern uint32_t g_unk_34551c;                  /* 0x0034551c */
extern uint32_t g_unk_345548;                  /* 0x00345548 */
extern uint8_t  g_unk_34554d;                  /* 0x0034554d */
extern uint16_t g_unk_345514;                  /* 0x00345514 */
extern uint32_t g_unk_345518;                  /* 0x00345518 */
extern uint32_t g_unk_345550;                  /* 0x00345550 */
extern uint8_t  g_unk_345559;                  /* 0x00345559 */
extern uint32_t g_unk_34553c;                  /* 0x0034553c */
extern uint32_t g_unk_345510;                  /* 0x00345510 */
extern uint8_t  g_unk_34555a;                  /* 0x0034555a */
extern uint8_t  g_unk_34555b;                  /* 0x0034555b */
extern uint16_t g_unk_345532;                  /* 0x00345532 */
extern uint16_t g_unk_345533;                  /* 0x00345533 */
extern uint8_t  g_unk_345538;                  /* 0x00345538 */
extern uint8_t  g_unk_345531;                  /* 0x00345531 */
extern uint8_t  g_unk_345567;                  /* 0x00345567 */
extern uint8_t  g_unk_345561;                  /* 0x00345561 */
extern uint8_t  g_unk_345564;                  /* 0x00345564 */
extern uint8_t  g_unk_345563;                  /* 0x00345563 */
extern uint8_t  g_unk_345541;                  /* 0x00345541 */
extern uint8_t  g_unk_345540;                  /* 0x00345540 */
extern uint8_t  g_unk_345543;                  /* 0x00345543 */
extern uint8_t  g_unk_345542;                  /* 0x00345542 */
extern uint8_t  g_unk_345544;                  /* 0x00345544 */
extern uint8_t  g_unk_345566;                  /* 0x00345566 */
extern uint8_t  g_unk_34556c;                  /* 0x0034556c */
extern uint8_t  g_rom_type;                    /* 0x00345565 */
extern uint8_t  g_unk_345569;                  /* 0x00345569 */
extern uint32_t g_unk_3455fc;                  /* 0x003455fc */
extern uint16_t g_unk_345534;                  /* 0x00345534 */
extern uint16_t g_unk_34556e;                  /* 0x0034556e */
extern uint32_t g_unk_3455f8;                  /* 0x003455f8 */
extern uint8_t  g_unk_345608;                  /* 0x00345608 */
extern uint32_t g_unk_3454e0;                  /* 0x003454e0 */
extern uint8_t  g_unk_345560;                  /* 0x00345560 */
extern uint8_t  g_unk_345501;                  /* 0x00345501 */
extern uint32_t g_unk_1ebd40;                  /* 0x001ebd40 */
extern uint8_t  g_unk_1ebd41;                  /* 0x001ebd41 */
extern uint8_t  g_unk_1ebd42;                  /* 0x001ebd42 */
extern uint8_t  g_unk_1ebd43;                  /* 0x001ebd43 */
extern uint8_t  g_unk_1ebd45;                  /* 0x001ebd45 */
extern uint8_t  g_unk_1ebd46;                  /* 0x001ebd46 */
extern uint32_t g_unk_3592fc;                  /* 0x003592fc */
extern uint32_t g_unk_1ebae0;                  /* 0x001ebae0 */
extern uint32_t g_unk_1ebae4;                  /* 0x001ebae4 */
extern uint32_t g_unk_1bbd44;                  /* 0x001bbd44 */
extern uint32_t g_unk_1bbd50;                  /* 0x001bbd50 */
extern uint32_t g_unk_345340;                  /* 0x00345340 */

/* External function declarations */
extern void   *snes_obj_alloc_001a9e88(uint32_t size);
extern void    snes_obj_init_00198cc8(void *obj);
extern void    snes_obj_start_001990f8(void *obj);
extern void    snes_sdd1_init_0019d740(uint32_t addr, uint32_t val);
extern void    snes_libc_func_0019cc0c(uint32_t val);
extern void    snes_file_init_0019d600(uint32_t addr, uint32_t a, uint32_t b);
extern int32_t snes_file_check_001a08ec(uint32_t mode);
extern void    snes_error_0019e414(uint32_t msg);
extern void    snes_module_load_00104e7c(uint32_t addr, uint32_t size, uint32_t a, uint32_t b);
extern void    snes_be_func_0019be70(void);
extern void    snes_mem_zero_0019c39c(void *dst, uint32_t val, uint32_t size);
extern void    snes_func_00104a54(void);
extern void    snes_xor_init_001042a0(void);  /* XOR deobfuscation */
extern void    snes_func_00106054(void);
extern int32_t snes_check_00106824(void);
extern void    snes_func_001060dc(void);
extern void    snes_splash_00101ef0(void);
extern void    snes_func_00105d78(void);
extern int32_t snes_check2_0010689c(void);
extern void    snes_hdd_check_001a147c(uint32_t a, uint32_t b, uint32_t path);
extern void    snes_file_stat_001a1974(uint32_t a, uint32_t b, void *buf);
extern void    snes_path_func_00103b34(uint32_t addr, uint32_t size);
extern void    snes_gui_main_00102ab0(uint32_t param);
extern void    snes_config_save2_00153354(uint32_t addr, uint32_t menu);
extern void    snes_gui_init_0019c0d0(void);
extern uint32_t g_unk_1ebd42_val;              /* DAT_001ebd42 */
extern uint32_t snes_file_check2_00108ce4(uint32_t mode);
extern uint32_t snes_libc_func_001a1db8(uint32_t a, uint32_t b);
extern int32_t snes_rom_load_00151074(uint32_t path);
extern int32_t snes_rom_load2_0010a840(void);
extern int32_t snes_rom_load3_001513bc(uint32_t path, uint32_t addr);
extern void    snes_rom_unload_00151330(uint32_t path);
extern void    snes_func_00104e58(void);
extern uint32_t snes_func_00105750(void);
extern void    snes_frontend_display_0010038c(uint64_t a, uint32_t b, uint32_t c, uint32_t d);
extern void    snes_frontend_core_init2_00100114(uint32_t a, uint64_t b);
extern int32_t snes_cpu_main_00142a78(void);
extern void    snes_func_00101860(void);
extern void    snes_func_00105e04(void);
extern void    snes_emulator_main_00177cec(uint32_t a, uint8_t b, uint32_t c);
extern void    snes_func_0010768c(void);
extern void    snes_func_0017487c(uint32_t mode);
extern void    snes_func_001076e4(void);
extern void    snes_func_00101858(void);
extern void    snes_mem_copy_0019e3d0(void *dst, uint32_t src1, uint32_t src2, uint32_t src3, uint32_t src4);
extern void    snes_func_0010191c(uint32_t addr);
extern void    snes_audio_init_0010a934(void);
extern void    snes_emulator_loop_00115db0(void);
extern void    snes_frontend_menu_00100e78(void);
extern void    snes_func_001005b0(void);

/* 0x00104f18: Main frontend initialization and loop */
void snes_frontend_main_00104f18(uint64_t param_1, uint64_t param_2)
{
    /* Initialize GS object */
    void *obj = snes_obj_alloc_001a9e88(0x74);
    snes_obj_init_00198cc8(obj);
    g_vram_base = obj;
    snes_obj_start_001990f8(g_vram_base);

    /* Initialize subsystems */
    snes_sdd1_init_0019d740(0x1b0e38, 0);
    snes_libc_func_0019cc0c(0);
    snes_file_init_0019d600(0x1b0e58, 0, 0);
    snes_file_init_0019d600(0x1b0e68, 0, 0);
    snes_file_init_0019d600(0x1b0e78, 0, 0);
    snes_file_init_0019d600(0x1b0e88, 0, 0);
    snes_file_init_0019d600(0x1b0e98, 0, 0);

    /* Check MC */
    if (snes_file_check_001a08ec(1) < 0) {
        snes_error_0019e414(0x1b0f50);
        while (1) { /* infinite loop */ }
    }

    snes_file_init_0019d600(0x1b0ea8, 0, 0);

    /* Load modules */
    snes_module_load_00104e7c(0x1ec300, g_unk_1f40d4, 0, 0);
    snes_module_load_00104e7c(0x1f6140, g_unk_1fafa0, 0, 0);
    snes_module_load_00104e7c(0x1f4100, g_unk_1f60c8, 0, 0);

    snes_be_func_0019be70();

    /* Clear framebuffer pointers */
    g_framebuffer1 = NULL;
    g_unk_1bb340 = g_unk_1b0eb8;
    g_unk_1bb341 = g_unk_1b0eb9;
    g_framebuffer4 = NULL;
    g_framebuffer2 = NULL;
    g_framebuffer3 = NULL;

    snes_func_00104a54();
    snes_mem_zero_0019c39c((void *)0x3454e0, 0, 0x148);

    /* XOR deobfuscation of data sections */
    int32_t i7 = 0;
    while (1) {
        int32_t i2 = g_unk_2ec53c;
        if (i2 < 0) i2 += 3;
        if (i2 >> 2 <= i7) break;
        uint32_t *ptr = (uint32_t *)&g_unk_2ec200 + i7;
        i7++;
        *ptr ^= 0x96695aa5;
    }

    int32_t i8 = 0;
    while (1) {
        int32_t i7b = g_unk_2ec1f4;
        if (i7b < 0) i7b += 3;
        if (i7b >> 2 <= i8) break;
        uint32_t *ptr = (uint32_t *)&g_unk_2ebc30 + i8;
        i8++;
        *ptr ^= 0x96695aa5;
    }

    snes_func_00106054();

    /* Check for HDD */
    if (snes_check_00106824() != 0) {
        snes_func_001060dc();
    }

    /* Show splash screen */
    snes_splash_00101ef0();
    snes_func_00105d78();

    /* Check HDD again */
    int32_t l4 = snes_check2_0010689c();
    g_unk_1ebaec = l4;
    if (l4 != 0) {
        snes_hdd_check_001a147c(0, 0, 0x1b0ec0);
        snes_file_stat_001a1974(0, 0, (void *)0x428180);  /* auStack_60 */
    }

    if (g_unk_1ec1c4 != 1) {
        *(uint8_t *)((int)&g_unk_2ec200 + g_unk_2ec53c) = 0;
        snes_path_func_00103b34(0x2ec200, 0x2ee);
    }

    /* Main loop */
    while (1) {
        while (1) {
            g_unk_1bbd44 = 0;
            snes_gui_main_00102ab0(0x428180);
            snes_error_0019e414(0x1b0ed8);

            /* Initialize emulator settings */
            g_unk_3454e8 = 100;
            g_unk_3454f0 = 0x558;
            g_unk_34551c = 200;
            g_unk_345548 = g_unk_1ebd42;
            g_unk_34554d = g_unk_1ebd41;
            g_unk_345514 = 0x411b;
            g_unk_345518 = 20000;
            g_unk_345550 = 1;
            g_unk_345559 = 0;
            g_unk_34553c = 1;
            g_unk_345510 = 20000;
            g_unk_34555a = 0;
            g_unk_34555b = 1;
            g_unk_345532 = 0;
            g_unk_345533 = 0;
            g_unk_345538 = (snes_file_check2_00108ce4(1) != 1);
            g_unk_345531 = 1;
            g_unk_345567 = 1;
            g_unk_345561 = g_unk_1ebd43;
            g_unk_345564 = 1;
            g_unk_345563 = 1;
            g_unk_345541 = 1;
            g_unk_345540 = 1;
            g_unk_345543 = 1;
            g_unk_345542 = 1;
            g_unk_345544 = 1;
            g_unk_345566 = 1;
            g_unk_34556c = g_unk_1ebd45;
            /* p28_write_piece(&g_unk_345508, sizeof(g_unk_345508), 1u, 1u, 0); */
            g_unk_345569 = 0;
            g_rom_type = 0;
            g_unk_3455fc = 0;
            g_unk_345534 = 0;
            g_unk_34556e = 0;
            g_unk_3455f8 = 0;

            snes_func_0017487c(1);
            g_unk_3454f0 = snes_libc_func_001a1db8(g_unk_3454e8 << 8, 0x156);
            g_unk_345608 = 0;

            int32_t l4b = snes_rom_load_00151074(0x34e2b0);
            if ((l4b == 0) || (l4b = snes_rom_load2_0010a840(), l4b == 0)) {
                snes_error_0019e414(0x1b0ef0);
                snes_func_00104e58();
            }

            uint32_t u1 = g_unk_345340;
            l4b = snes_rom_load3_001513bc(0x34e2b0, 0x428180);
            if (l4b != 0) break;

            snes_error_0019e414(0x1b0f30);
            snes_rom_unload_00151330(0x34e2b0);
        }

        if (g_unk_1ebaec == 1) {
            uint32_t u3 = snes_func_00105750();
            snes_config_save2_00153354(0x34e2b0, u3);
        }

        snes_gui_init_0019c0d0();
        g_unk_1bbd50 = 0;
        snes_func_00115b58();
        g_unk_345340 = u1;

        if ((g_unk_1ebd46 == 2) ||
            (((g_unk_1ebd46 < 3 || (g_unk_1ebd46 != 3)) && (g_unk_3592fc == 0x32)))) {
            uint32_t u3 = (g_rom_type == 0) ? 0xef : 0x1de;
            uint64_t u6 = (g_rom_type == 0) ? 0x100 : 0x200;
            snes_frontend_display_0010038c(u6, u3, 0, 3);
            g_unk_1ebae0 = 0x3c0;
        } else {
            uint32_t u3 = (g_rom_type == 0) ? 0xef : 0x1de;
            uint64_t u6 = (g_rom_type == 0) ? 0x100 : 0x200;
            snes_frontend_display_0010038c(u6, u3, 0, 2);
            g_unk_1ebae0 = 800;
        }

        g_unk_1ebae4 = g_unk_1ebae0;
        if (g_unk_34554d == 1) {
            g_unk_1ebae4 = g_unk_1ebae0 << 1;
        }
        if (g_unk_345548 == 1) {
            g_unk_1ebae4 = g_unk_1ebae4 >> 2;
        } else if (g_unk_345548 == 2) {
            g_unk_1ebae4 = g_unk_1ebae4 / 2;
        }

        snes_frontend_core_init2_00100114(0, param_2);

        if (snes_cpu_main_00142a78() == 0) {
            snes_func_00104e58();
        }

        snes_func_00101860();
        snes_func_00105e04();

        if (g_unk_1ebd40 == 1) {
            g_unk_3454e0 = g_unk_1ebd40;
            g_unk_345560 = g_unk_1ebd40;
            snes_emulator_main_00177cec(g_unk_345548, g_unk_34554d, g_unk_345550);
            snes_func_0010768c();
        } else {
            g_unk_3454e0 = 0;
            g_unk_345560 = 0;
            snes_func_0017487c(1);
            snes_func_001076e4();
        }

        snes_func_00101858();
        snes_mem_copy_0019e3d0((void *)0x345060, 0x1b0f10, 0x3592d8, 0x1b0f20);
        snes_func_0010191c(0x345060);
        snes_audio_init_0010a934();

        /* Emulator main loop */
        while (g_unk_1bbd44 != 1) {
            if (g_unk_345501 == 0) {
                snes_emulator_loop_00115db0();
            }
            if (g_unk_345501 != 0) {
                snes_func_0017487c(1);
                snes_func_00104bbc(0);
                snes_frontend_menu_00100e78();
            }
            if (g_unk_345501 == 0) {
                snes_func_0017487c(0);
            }
        }

        snes_func_001005b0();
        snes_rom_unload_00151330(0x34e2b0);
        snes_func_00105d78();
    }
}