/*
 * SNES Station v0.23 - Configuration File Handling
 * Canonical source from progress28_structural_lift.c (0x00104418, 0x001060dc, 0x001064c0 range)
 */

#include <stdint.h>
#include <stddef.h>

/* External references to globals defined elsewhere */
extern uint8_t  g_unk_1ebd40;                  /* 0x001ebd40 */
extern uint8_t  g_unk_1ebd41;                  /* 0x001ebd41 */
extern uint8_t  g_unk_1ebd42;                  /* 0x001ebd42 */
extern uint8_t  g_unk_1ebd43;                  /* 0x001ebd43 */
extern uint8_t  g_unk_1ebd44;                  /* 0x001ebd44 */
extern uint8_t  g_unk_1ebd45;                  /* 0x001ebd45 */
extern uint8_t  g_unk_1ebd46;                  /* 0x001ebd46 */
extern uint8_t  g_unk_1ebd48;                  /* 0x001ebd48 */
extern uint8_t  g_unk_1ebd4c;                  /* 0x001ebd4c */
extern uint32_t g_unk_1ebd50;                  /* 0x001ebd50 */
extern uint32_t g_unk_1ebd54;                  /* 0x001ebd54 */
extern uint32_t g_unk_1ebd58;                  /* 0x001ebd58 */
extern uint32_t g_unk_1ebd5c;                  /* 0x001ebd5c */

/* External function declarations */
extern void    snes_mem_copy_0019e3d0(void *dst, uint32_t src, uint32_t a, uint32_t b);
extern void   *snes_mem_alloc_0019e698(uint32_t align, uint32_t size);
extern void    snes_mem_set_0019c528(void *dst, uint32_t val);
extern uint8_t *snes_mem_find_0019e99c(void *mem, uint32_t tag);
extern uint8_t *snes_mem_parse_0019eaf8(uint8_t *ptr, uint32_t tag);
extern uint32_t snes_mem_check_0019efac(uint8_t c);
extern uint32_t snes_mem_len_0019c5e8(uint8_t *str);
extern uint32_t snes_mem_cmp_0019c610(uint8_t *str, uint32_t end_char);
extern void    snes_mem_free_0019e784(void *mem);
extern uint32_t snes_file_open_0010421c(void *buf);
extern uint32_t snes_file_read_00104234(void *buf, uint32_t path);
extern uint32_t snes_file_close_00104358(void *buf);
extern uint32_t snes_str_to_int_0019eb80(uint8_t *str, uint32_t base, uint32_t len);
extern void    snes_obj_write_0019d244(uint32_t fd, uint8_t *src, uint32_t len);
extern uint32_t snes_file_create_0019cfc0(uint32_t path, uint32_t mode);
extern uint64_t snes_file_size_0019d360(uint32_t fd, uint32_t a, uint32_t b);
extern void    snes_file_close2_0019d090(uint32_t fd);
extern uint32_t snes_mem_alloc2_0019e4b4(uint64_t size);

/* 0x00104418: Parse config file for a key */
uint32_t snes_config_parse_00104418(int32_t param_1, uint64_t param_2, uint64_t param_3, uint64_t param_4)
{
    snes_mem_copy_0019e3d0((void *)0x427e18, 0x1b0e00, (uint32_t)param_2);

    int32_t i5 = *(int32_t *)(param_1 + 8);
    if (i5 < 0) i5 += 0x3f;
    void *mem = snes_mem_alloc_0019e698(0x40, (i5 >> 6) * 0x40 + 0x40);
    snes_mem_set_0019c528(mem, *(uint32_t *)(param_1 + 4));

    uint8_t *ptr = snes_mem_find_0019e99c(mem, 0x1b0e08);
    while (ptr != NULL) {
        while (snes_mem_check_0019efac(*ptr) != 0) {
            ptr++;
        }
        uint8_t *next = snes_mem_parse_0019eaf8(ptr, 0x427e18);
        if (next == ptr) {
            snes_mem_set_0019c528(mem, ptr + (*(int32_t *)(param_1 + 4) - (uint32_t)mem));
            snes_mem_find_0019e99c(mem, 0x1b0e08);
            uint8_t *pc3 = snes_mem_find_0019e99c(0, 0x1b0e08);
            if (pc3 != NULL) goto parse_loop;
            break;
        }
        ptr = snes_mem_find_0019e99c(0, 0x1b0e08);
    }
    goto not_found;

    while (1) {
        uint8_t *pc4 = snes_mem_parse_0019eaf8(pc3, param_3);
        if (pc4 == pc3) {
            int32_t len = snes_mem_len_0019c5e8(param_3);
            pc3 += len;
            while (snes_mem_check_0019efac(*pc3) != 0) pc3++;
            if (*pc3 == '=') {
                do { pc3++; } while (snes_mem_check_0019efac(*pc3) != 0);
                for (int32_t i = snes_mem_len_0019c5e8(pc3); i > 0; i--) {
                    if (snes_mem_check_0019efac(pc3[i - 1]) == 0) {
                        i = 0;
                    } else {
                        pc3[i - 1] = '\0';
                    }
                }
                snes_mem_set_0019c528(param_4, pc3);
                snes_mem_free_0019e784(mem);
                return 1;
            }
        }
        pc3 = snes_mem_find_0019e99c(0, 0x1b0e08);
        if (pc3 == NULL) break;
    parse_loop:
        while (snes_mem_check_0019efac(*pc3) != 0) pc3++;
        if (*pc3 == '[' && snes_mem_cmp_0019c610(pc3, 0x5d) != 0) break;
    }

not_found:
    snes_mem_free_0019e784(mem);
    return 2;
}

/* 0x001060dc: Load config from file */
void snes_config_load_001060dc(void)
{
    uint8_t buf1[16];
    uint8_t buf2[80];

    snes_file_open_0010421c(buf1);
    if (snes_file_read_00104234(buf1, 0x1b1020) == 1) {
        uint8_t *val;

        val = (uint8_t *)0x1b1048;
        if (snes_config_parse_00104418(buf1, 0x1b1038, val, buf2) == 1) {
            g_unk_1ebd40 = snes_str_to_int_0019eb80(buf2, 0, 10) != 0;
        }

        val = (uint8_t *)0x1b1050;
        if (snes_config_parse_00104418(buf1, 0x1b1038, val, buf2) == 1) {
            g_unk_1ebd41 = snes_str_to_int_0019eb80(buf2, 0, 10) != 0;
        }

        val = (uint8_t *)0x1b1058;
        if (snes_config_parse_00104418(buf1, 0x1b1038, val, buf2) == 1) {
            g_unk_1ebd42 = snes_str_to_int_0019eb80(buf2, 0, 10);
        }

        val = (uint8_t *)0x1b1060;
        if (snes_config_parse_00104418(buf1, 0x1b1038, val, buf2) == 1) {
            g_unk_1ebd43 = snes_str_to_int_0019eb80(buf2, 0, 10) != 0;
        }

        val = (uint8_t *)0x1b1080;
        if (snes_config_parse_00104418(buf1, 0x1b1070, val, buf2) == 1) {
            g_unk_1ebd44 = snes_str_to_int_0019eb80(buf2, 0, 10) != 0;
        }

        val = (uint8_t *)0x1b10a8;
        if (snes_config_parse_00104418(buf1, 0x1b1090, val, buf2) == 1) {
            g_unk_1ebd45 = snes_str_to_int_0019eb80(buf2, 0, 10) != 0;
        }

        val = (uint8_t *)0x1b10b8;
        if (snes_config_parse_00104418(buf1, 0x1b1090, val, buf2) == 1) {
            g_unk_1ebd46 = snes_str_to_int_0019eb80(buf2, 0, 10);
        }

        val = (uint8_t *)0x1b10c8;
        if (snes_config_parse_00104418(buf1, 0x1b1090, val, buf2) == 1) {
            g_unk_1ebd48 = snes_str_to_int_0019eb80(buf2, 0, 10);
        }

        val = (uint8_t *)0x1b10d8;
        if (snes_config_parse_00104418(buf1, 0x1b1090, val, buf2) == 1) {
            g_unk_1ebd4c = snes_str_to_int_0019eb80(buf2, 0, 10);
        }

        val = (uint8_t *)0x1b10e8;
        if (snes_config_parse_00104418(buf1, 0x1b1090, val, buf2) == 1) {
            g_unk_1ebd58 = snes_str_to_int_0019eb80(buf2, 0, 10);
        }

        val = (uint8_t *)0x1b10f8;
        if (snes_config_parse_00104418(buf1, 0x1b1090, val, buf2) == 1) {
            g_unk_1ebd5c = snes_str_to_int_0019eb80(buf2, 0, 10);
        }

        val = (uint8_t *)0x1b1108;
        if (snes_config_parse_00104418(buf1, 0x1b1090, val, buf2) == 1) {
            g_unk_1ebd50 = snes_str_to_int_0019eb80(buf2, 0, 10);
        }

        val = (uint8_t *)0x1b1118;
        if (snes_config_parse_00104418(buf1, 0x1b1090, val, buf2) == 1) {
            g_unk_1ebd54 = snes_str_to_int_0019eb80(buf2, 0, 10);
        }
    }
    snes_file_close_00104358(buf1);
}

/* 0x001064c0: Save config to file */
void snes_config_save_001064c0(void)
{
    if (g_unk_1ebaec != 0) {
        uint32_t fd = snes_file_create_0019cfc0(0x1b1020, 0x203);
        if ((int32_t)fd >= 0) {
            uint8_t buf[80];
            uint32_t len;

            snes_obj_write_0019d244(fd, (uint8_t *)0x1b1128, snes_mem_len_0019c5e8((uint8_t *)0x1b1128));

            snes_mem_copy_0019e3d0(buf, 0x1b1158, g_unk_1ebd40);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b1168, g_unk_1ebd41);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b1178, g_unk_1ebd42);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b1188, g_unk_1ebd43);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_obj_write_0019d244(fd, (uint8_t *)0x1b1140, snes_mem_len_0019c5e8((uint8_t *)0x1b1140));
            snes_obj_write_0019d244(fd, (uint8_t *)0x1b1148, snes_mem_len_0019c5e8((uint8_t *)0x1b1148));

            snes_mem_copy_0019e3d0(buf, 0x1b11b8, g_unk_1ebd44);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_obj_write_0019d244(fd, (uint8_t *)0x1b1140, snes_mem_len_0019c5e8((uint8_t *)0x1b1140));
            snes_obj_write_0019d244(fd, (uint8_t *)0x1b11a0, snes_mem_len_0019c5e8((uint8_t *)0x1b11a0));

            snes_mem_copy_0019e3d0(buf, 0x1b11c8, g_unk_1ebd45);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b11e0, g_unk_1ebd46);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b11f8, g_unk_1ebd48);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b1208, g_unk_1ebd4c);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b1218, g_unk_1ebd58);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b1228, g_unk_1ebd5c);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b1238, g_unk_1ebd50);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_mem_copy_0019e3d0(buf, 0x1b1250, g_unk_1ebd54);
            len = snes_mem_len_0019c5e8(buf);
            snes_obj_write_0019d244(fd, buf, len);

            snes_file_close2_0019d090(fd);
        }
    }
}