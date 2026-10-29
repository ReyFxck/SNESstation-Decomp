/*
 * SNES Station v0.23 - Startup and Frontend Initialization
 * Canonical source from progress28_structural_lift.c (0x00100000-0x0010ffff range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern uint8_t  g_rom_type;                    /* 0x00345565 */
extern uint32_t g_screen_width;                /* 0x0035d490 */
extern void    *g_framebuffer1;                /* 0x0035d480 */
extern void    *g_framebuffer2;                /* 0x0035d484 */
extern void    *g_framebuffer3;                /* 0x001baf00 */
extern void    *g_framebuffer4;                /* 0x0035d488 */
extern void    *g_framebuffer5;                /* 0x0035d48c */
extern void    *g_vram_base;                   /* 0x001baf08 */
extern uint32_t g_vram_size;                   /* 0x001baf10 */
extern void    *g_input_state;                 /* 0x00426e80 */
extern void   **g_input_state_ptr;             /* 0x00426fc4 */
extern uint64_t g_unk_426fa4;                  /* 0x00426fa4 */
extern uint32_t g_unk_426fb4;                  /* 0x00426fb4 */
extern uint32_t g_unk_427000;                  /* 0x00427000 */
extern uint32_t g_unk_427004;                  /* 0x00427004 */
extern uint32_t g_unk_427008;                  /* 0x00427008 */
extern uint8_t  g_unk_345608;                  /* 0x00345608 */
extern uint8_t  g_unk_34556c;                  /* 0x0034556c */
extern uint32_t g_unk_35c278;                  /* 0x0035c278 */
extern uint32_t g_unk_3592fc;                  /* 0x003592fc */
extern uint32_t g_unk_1ebd50;                  /* 0x001ebd50 */
extern uint32_t g_unk_1ebd54;                  /* 0x001ebd54 */
extern uint32_t g_unk_1ebd58;                  /* 0x001ebd58 */
extern uint32_t g_unk_1ebd5c;                  /* 0x001ebd5c */

/* External function declarations */
extern void    syscall(int n);
extern void    EI(void);
extern void    snes_frontend_init_00104f18(uint32_t, void*, uint32_t, uint32_t, uint32_t);
extern void   *snes_mem_alloc_0019e698(uint32_t align, uint32_t size);
extern void    snes_mem_zero_0019c39c(void *dst, uint32_t val, uint32_t size);
extern uint64_t snes_vram_init_001993c8(void *base, uint32_t size);
extern void    snes_panic_001018fc(void);
extern void   *snes_obj_alloc_001a9e88(uint32_t size);
extern void    snes_obj_init_00198cc8(void *obj);
extern void    snes_obj_config_00198d78(void *obj, uint64_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6, uint32_t p7);
extern void    snes_obj_start_001990f8(void *obj);
extern void    snes_obj_callback_0019a580(void *obj, uint32_t cb);
extern void    snes_obj_set_00199aa8(void *obj, uint32_t val);
extern void    snes_obj_enable_00199b80(void *obj, uint32_t enable);
extern void    snes_obj_link_0019b7f0(uint32_t *link, void *ptr, uint32_t a, uint32_t b, uint32_t c, uint32_t d);
extern void    snes_obj_wait_001998f8(void *obj);
extern uint32_t snes_obj_status_00199198(void *obj);
extern void    snes_obj_setup_00199f88(void *obj, uint32_t status, uint32_t a, uint32_t b, uint32_t c, uint32_t d, void *fb, uint32_t e);
extern void    snes_obj_vram_0019a588(void *obj, uint32_t status, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f);
extern void    snes_obj_config2_0019b028(void *obj, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g);
extern void    snes_copy_0019e3d0(void *dst, uint32_t src, uint32_t a, uint32_t b);
extern void    snes_obj_blend_0019b948(uint32_t *link, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, void *src);
extern void    snes_obj_poll_001991e8(void *obj);
extern void    snes_obj_wait2_00199208(void *obj);
extern void    snes_obj_poll2_00199268(void *obj);

/* 0x00100008: Main entry point - initializes memory, calls frontend init */
void snes_entry_00100008(void *param_1)
{
    /* Clear memory region 0x426e00-0x450c18 */
    uint32_t *ptr = (uint32_t *)0x426e00;
    do {
        ptr[0] = 0;
        ptr[1] = 0;
        ptr[2] = 0;
        ptr[3] = 0;
        ptr += 4;
    } while (ptr < (uint32_t *)0x450c18);

    syscall(0);
    syscall(0);
    syscall(0);
    EI();

    if (g_input_state == NULL) {
        if (param_1 == NULL) {
            g_unk_427004 = 0;
            g_unk_427008 = (uint32_t)(param_1 + 1);
        } else {
            g_unk_427004 = ((uint32_t *)param_1)[1];
            g_unk_427008 = (uint32_t)(param_1 + 2);
        }
    } else {
        g_unk_427004 = ((uint32_t *)g_input_state)[1];
        g_unk_427008 = (uint32_t)g_input_state + 4;
    }

    g_input_state_ptr = param_1;
    snes_frontend_init_00104f18(g_unk_427004, (void *)g_unk_427008, 0x20000, 0x426e80, 0x10010c);

    if (g_input_state_ptr != NULL) {
        *(uint32_t *)*g_input_state_ptr = 0;
        syscall(0);
    }
    syscall(0);
    syscall(0);

    g_screen_width = 0x404;
    if (g_rom_type == 0) {
        g_screen_width = 0x204;
    }

    g_unk_426fa4 = 0;  /* unaff_s0 - saved register */
    g_unk_426fb4 = 0x1000e0;

    /* Allocate framebuffers */
    uint32_t fb_count = (g_rom_type == 0) ? 0xef : 0x1de;

    if (g_framebuffer1 == NULL) {
        g_framebuffer1 = snes_mem_alloc_0019e698(0x40, g_screen_width * fb_count);
    }
    if (g_framebuffer2 == NULL) {
        g_framebuffer2 = snes_mem_alloc_0019e698(0x40, g_screen_width * fb_count);
    }
    if (g_framebuffer3 == NULL) {
        g_framebuffer3 = snes_mem_alloc_0019e698(0x40, g_screen_width * fb_count);
    }
    if (g_framebuffer4 == NULL) {
        g_framebuffer4 = snes_mem_alloc_0019e698(0x40, (g_screen_width >> 1) * fb_count);
    }
    if (g_framebuffer5 == NULL) {
        g_framebuffer5 = snes_mem_alloc_0019e698(0x40, (g_screen_width >> 1) * fb_count);
    }

    /* Check allocation success */
    if (g_framebuffer1 == NULL || g_framebuffer2 == NULL || g_framebuffer3 == NULL ||
        g_framebuffer4 == NULL || g_framebuffer5 == NULL) {
        snes_panic_001018fc();
    }

    /* Zero framebuffers */
    snes_mem_zero_0019c39c(g_framebuffer1, 0, g_screen_width * fb_count);
    if (g_framebuffer2 != NULL) {
        uint32_t size = g_screen_width << 9;
        if (g_rom_type == 0) size = g_screen_width << 8;
        snes_mem_zero_0019c39c(g_framebuffer2, 0, size);
    }
    if (g_framebuffer3 != NULL) {
        uint32_t size = g_screen_width << 9;
        if (g_rom_type == 0) size = g_screen_width << 8;
        snes_mem_zero_0019c39c(g_framebuffer3, 0, size);
    }

    /* Initialize VRAM */
    uint64_t vram_size = snes_vram_init_001993c8(g_vram_base, 0x100870);
    g_vram_size = (uint32_t)vram_size;
}

/* 0x001000e0: Frontend core init (re-entry point with syscalls) */
void snes_frontend_core_init_001000e0(void)
{
    if (g_input_state_ptr != NULL) {
        *(uint32_t *)*g_input_state_ptr = 0;
        syscall(0);
    }
    syscall(0);
    syscall(0);

    g_screen_width = 0x404;
    if (g_rom_type == 0) {
        g_screen_width = 0x204;
    }

    uint32_t fb_count = (g_rom_type == 0) ? 0xef : 0x1de;

    if (g_framebuffer1 == NULL) {
        g_framebuffer1 = snes_mem_alloc_0019e698(0x40, g_screen_width * fb_count);
    }
    if (g_framebuffer2 == NULL) {
        g_framebuffer2 = snes_mem_alloc_0019e698(0x40, g_screen_width * fb_count);
    }
    if (g_framebuffer3 == NULL) {
        g_framebuffer3 = snes_mem_alloc_0019e698(0x40, g_screen_width * fb_count);
    }
    if (g_framebuffer4 == NULL) {
        g_framebuffer4 = snes_mem_alloc_0019e698(0x40, (g_screen_width >> 1) * fb_count);
    }
    if (g_framebuffer5 == NULL) {
        g_framebuffer5 = snes_mem_alloc_0019e698(0x40, (g_screen_width >> 1) * fb_count);
    }

    if (g_framebuffer1 == NULL || g_framebuffer2 == NULL || g_framebuffer3 == NULL ||
        g_framebuffer4 == NULL || g_framebuffer5 == NULL) {
        snes_panic_001018fc();
    }

    fb_count = (g_rom_type == 0) ? 0xef : 0x1de;
    snes_mem_zero_0019c39c(g_framebuffer1, 0, g_screen_width * fb_count);
    if (g_framebuffer2 != NULL) {
        uint32_t size = g_screen_width << 9;
        if (g_rom_type == 0) size = g_screen_width << 8;
        snes_mem_zero_0019c39c(g_framebuffer2, 0, size);
    }
    if (g_framebuffer3 != NULL) {
        uint32_t size = g_screen_width << 9;
        if (g_rom_type == 0) size = g_screen_width << 8;
        snes_mem_zero_0019c39c(g_framebuffer3, 0, size);
    }

    uint64_t vram_size = snes_vram_init_001993c8(g_vram_base, 0x100870);
    g_vram_size = (uint32_t)vram_size;
}

/* 0x00100114: Frontend core init (simplified, no syscalls/input check) */
void snes_frontend_core_init2_00100114(void)
{
    g_screen_width = 0x404;
    if (g_rom_type == 0) {
        g_screen_width = 0x204;
    }

    uint32_t fb_count = (g_rom_type == 0) ? 0xef : 0x1de;

    if (g_framebuffer1 == NULL) {
        g_framebuffer1 = snes_mem_alloc_0019e698(0x40, g_screen_width * fb_count);
    }
    if (g_framebuffer2 == NULL) {
        g_framebuffer2 = snes_mem_alloc_0019e698(0x40, g_screen_width * fb_count);
    }
    if (g_framebuffer3 == NULL) {
        g_framebuffer3 = snes_mem_alloc_0019e698(0x40, g_screen_width * fb_count);
    }
    if (g_framebuffer4 == NULL) {
        g_framebuffer4 = snes_mem_alloc_0019e698(0x40, (g_screen_width >> 1) * fb_count);
    }
    if (g_framebuffer5 == NULL) {
        g_framebuffer5 = snes_mem_alloc_0019e698(0x40, (g_screen_width >> 1) * fb_count);
    }

    if (g_framebuffer1 == NULL || g_framebuffer2 == NULL || g_framebuffer3 == NULL ||
        g_framebuffer4 == NULL || g_framebuffer5 == NULL) {
        snes_panic_001018fc();
    }

    fb_count = (g_rom_type == 0) ? 0xef : 0x1de;
    snes_mem_zero_0019c39c(g_framebuffer1, 0, g_screen_width * fb_count);
    if (g_framebuffer2 != NULL) {
        uint32_t size = g_screen_width << 9;
        if (g_rom_type == 0) size = g_screen_width << 8;
        snes_mem_zero_0019c39c(g_framebuffer2, 0, size);
    }
    if (g_framebuffer3 != NULL) {
        uint32_t size = g_screen_width << 9;
        if (g_rom_type == 0) size = g_screen_width << 8;
        snes_mem_zero_0019c39c(g_framebuffer3, 0, size);
    }

    uint64_t vram_size = snes_vram_init_001993c8(g_vram_base, 0x100870);
    g_vram_size = (uint32_t)vram_size;
}

/* 0x0010038c: Frontend display config */
void snes_frontend_display_config_0010038c(uint64_t param_1, uint32_t param_2, uint64_t param_3, uint32_t param_4)
{
    if (g_vram_base == NULL) {
        void *obj = snes_obj_alloc_001a9e88(0x74);
        snes_obj_init_00198cc8(obj);
        g_vram_base = obj;
    }

    g_unk_427000 = param_4;
    g_unk_427004 = (uint32_t)param_1;
    g_unk_427008 = param_2;

    uint32_t mode, val1, val2;
    if (param_4 == 2) {
        mode = 2;
        val1 = g_unk_1ebd50;
        val2 = g_unk_1ebd54;
    } else if (param_4 == 3) {
        mode = 3;
        val1 = g_unk_1ebd58;
        val2 = g_unk_1ebd5c;
    } else {
        goto skip_config;
    }

    snes_obj_config_00198d78(g_vram_base, param_1, param_2, val1, val2, 0, 5, mode);

skip_config:
    snes_obj_start_001990f8(g_vram_base);
    snes_obj_callback_0019a580(g_vram_base, 0);
    snes_obj_set_00199aa8(g_vram_base, 0);
    snes_obj_enable_00199b80(g_vram_base, 1);
    g_vram_base = g_vram_base;  /* DAT_001baf20 = DAT_001baf08 */
    snes_obj_link_0019b7f0(0x1baf20, (void *)0x1bb31c, 0x3c0000, *(uint32_t *)0x1bb320, 0, 0);
    snes_obj_wait_001998f8(g_vram_base);
}

/* 0x001005ec: Frontend display main loop */
void snes_frontend_display_main_001005ec(void)
{
    snes_obj_wait_001998f8(g_vram_base);

    if (g_unk_345608 != 1) {
        snes_obj_set_00199aa8(g_vram_base, 0);
        uint32_t w = 0x200;
        uint32_t fb_count = (g_rom_type == 0) ? 0xef : 0x1de;

        snes_obj_config2_0019b028(g_vram_base, 0, 0, 0, 0, w, fb_count, w);

        uint32_t status = snes_obj_status_00199198(g_vram_base);
        uint32_t w2 = (g_rom_type == 0) ? 0x102 : 0x202;
        uint32_t w3 = (g_rom_type == 0) ? 0x100 : 0x200;
        snes_obj_setup_00199f88(g_vram_base, status, w3, 0, 0, 2, g_framebuffer1, w2);

        status = snes_obj_status_00199198(g_vram_base);
        snes_obj_vram_0019a588(g_vram_base, status, 0x100, 8, 8, 2, 0, 0);

        uint32_t w4 = (g_rom_type == 0) ? 0xef : 0x1de;
        uint32_t w5 = (g_rom_type == 0) ? 0x100 : 0x200;
        snes_obj_config2_0019b028(g_vram_base, 0, 0, 0, 0, w5, w4, w5);

        if (g_unk_34556c != 0) {
            uint8_t stack_buf[16];
            snes_copy_0019e3d0(stack_buf, 0x1b0880, g_unk_35c278, g_unk_3592fc);
            uint32_t blend_mode = (g_rom_type == 0) ? 0xdf : 0x1ce;
            snes_obj_blend_0019b948(0x1baf20, 0, 0x100, blend_mode, 2, 0x80808080, 1, stack_buf);
        }
    }

    snes_obj_wait_001998f8(g_vram_base);
    snes_obj_poll2_00199268(g_vram_base);

    /* Wait for VSYNC */
    while (snes_obj_poll_001991e8(g_vram_base) == 0) {
        /* busy wait */
    }
    snes_obj_wait2_00199208(g_vram_base);

    if (g_unk_345608 != 0) {
        snes_obj_set_00199aa8(g_vram_base, 0);
        uint32_t w = (g_rom_type == 0) ? 0x100 : 0x200;
        uint32_t h = (g_rom_type == 0) ? 0xe0 : 0x1c0;
        /* ... rest of function continues */
    }
}