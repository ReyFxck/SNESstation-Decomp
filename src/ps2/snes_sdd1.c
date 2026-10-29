/*
 * SNES Station v0.23 - S-DD1 Chip Emulation Support
 * Canonical source from progress28_structural_lift.c (0x00105e48, 0x00108a9c range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern uint32_t g_unk_359314;                  /* 0x00359314 */
extern uint32_t g_unk_35931c;                  /* 0x0035931c */
extern uint32_t g_unk_359318;                  /* 0x00359318 */
extern uint32_t g_unk_335280;                  /* 0x00335280 */

/* External function declarations */
extern void    snes_path_func_00151360(uint32_t path);
extern void    snes_file_func_00105ae8(uint32_t a, void *b, void *c, void *d, void *e);
extern void    snes_file_func_001059cc(void *a, void *b, void *c, uint32_t d, uint32_t e);
extern void    snes_mem_copy_0019c3d4(void *dst, const char *src);
extern void    snes_mem_set_0019c528(void *dst, void *src);
extern int32_t snes_mem_cmp_0019c458(uint32_t a, uint32_t b, uint32_t c);
extern void    snes_file_func_0019c3d4b(void *dst, const char *src);
extern int32_t snes_file_open_0019cfc0(void *path, uint32_t mode);
extern uint64_t snes_file_size_0019d360(int32_t fd, uint32_t a, uint32_t b);
extern void    snes_file_close_0019d090(int32_t fd);
extern uint32_t snes_mem_alloc_0019e4b4(uint64_t size);
extern void    snes_file_read_0019d120(int32_t fd, uint32_t buf, uint64_t size);
extern void    snes_func_001080cc(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
extern void    snes_mem_free_0019e784(uint32_t ptr);
extern int32_t snes_sdd1_detect_00108a9c(void);

/* 0x00105e48: Initialize S-DD1 emulation */
void snes_sdd1_init_00105e48(void)
{
    snes_path_func_00151360(0x34e2b0);
    uint8_t stack1[1040], stack2[1040], stack3[1040], stack4[1040], stack5[16], stack6[1040], stack7[1040];

    snes_file_func_00105ae8(0x35b328, stack5, stack2, stack6, stack7);
    snes_file_func_001059cc(stack1, stack5, stack2, 0, 0);

    int32_t l1 = snes_mem_cmp_0019c458(0x3592d8, 0x1b0fd0, 10);
    const char *sdd1_file = (l1 == 0) ? "\\socnsdd1" : "\\sfa2sdd1";

    snes_mem_copy_0019c3d4(stack1, sdd1_file);
    snes_mem_set_0019c528(stack3, stack1);
    snes_mem_copy_0019c3d4(stack3, 0x1b1000);
    snes_mem_set_0019c528(stack4, stack1);
    snes_mem_copy_0019c3d4(stack4, 0x1b1010);

    l1 = snes_file_open_0019cfc0(stack3, 1);
    if (l1 != 0) {
        uint64_t u2 = snes_file_size_0019d360(l1, 0, 2);
        snes_file_close_0019d090(l1);
        uint32_t u3 = snes_mem_alloc_0019e4b4(u2);
        g_unk_359314 = u3;
        snes_file_read_0019d120(l1, u3, u2);
        snes_file_close_0019d090(l1);
        g_unk_35931c = (int)u2 / 0xc;

        l1 = snes_file_open_0019cfc0(stack4, 1);
        if (l1 == 0) {
            snes_mem_free_0019e784(g_unk_359314);
            g_unk_359314 = 0;
            g_unk_35931c = 0;
        } else {
            u2 = snes_file_size_0019d360(l1, 0, 2);
            snes_file_close_0019d090(l1);
            u3 = snes_mem_alloc_0019e4b4(u2);
            g_unk_359318 = u3;
            snes_file_read_0019d120(l1, u3, u2);
            snes_file_close_0019d090(l1);
            snes_func_001080cc(g_unk_359314, g_unk_35931c, 0xc, 0x105e38);
        }
    }
}

/* 0x00108a9c: Detect S-DD1 chip */
int32_t snes_sdd1_detect_00108a9c(void)
{
    bool done;

    if (g_unk_335280 == 0) {
        while (1) {
            int32_t l2 = snes_file_func_0019c688(0x42c740, 0xffffffff80000901, 0);
            if (l2 < 0) break;

            if (*(int32_t *)0x42c764 != 0) {
                while (1) {
                    l2 = snes_file_func_0019c688(0x42c780, 0xffffffff80000902, 0);
                    if (l2 < 0) return -1;
                    if (*(int32_t *)0x42c7a4 != 0) break;
                    int32_t i3 = 0xffff;
                    do { i3--; } while (i3 != -1);
                }
                while (1) {
                    l2 = snes_file_func_0019c688(0x42c7c0, 0xffffffff80000903, 0);
                    if (l2 < 0) return -1;
                    if (*(int32_t *)0x42c7e4 != 0) break;
                    int32_t i3 = 0xffff;
                    do { i3--; } while (i3 != -1);
                }
                g_unk_335280 = 1;
                return 1;
            }
            int32_t i3 = 0xffff;
            do { i3--; } while (i3 != -1);
        }
    }
    return 0;
}