/*
 * SNES Station v0.23 WIP SRAM routines recovered from the binary.
 *
 * 0x00153354 : LoadSRAM-like method
 * 0x001534b8 : SaveSRAM-like method
 *
 * The RTC/special-chip hooks remain address-named until identified.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define FIELD32(base, off) (*(uint32_t *)((uint8_t *)(base) + (off)))

extern uint8_t g_sram_size;             /* EE VA 0x0034e2d4 */
extern uint8_t g_sram_initial_value;    /* EE VA 0x0035b73c */
extern uint8_t g_special_load_flag;     /* EE VA 0x00345544 */
extern uint8_t g_special_save_enable;   /* EE VA 0x0035b328 */
extern uint8_t g_settings_54;           /* Settings + 0x54 */
extern uint8_t g_settings_64;           /* Settings + 0x64 */
extern uint8_t g_settings_66;           /* Settings + 0x66 */
extern uint8_t *g_sram;                 /* EE VA 0x0034e298 */

extern int  fioOpen_like(const char *path, int flags); /* 0x0019cfc0 */
extern int  fioClose_like(int fd);                       /* 0x0019d090 */
extern int  fioRead_like(int fd, void *dst, int size);  /* 0x0019d120 */
extern int  fioWrite_like(int fd, const void *src, int size); /* 0x0019d244 */
extern void *memmove_like(void *dst, const void *src, size_t size); /* 0x0019c4a0 */

extern void sub_0016fbb4(void);
extern void sub_00183678(void);
extern void sub_00183d40(void);
extern void sub_00183660(void);
extern void sub_001834f0(void *arg);
extern void sub_00183c58(void);
extern void sub_0016fb04(void);
extern void sub_001833a4(void *arg);

static uint32_t expected_sram_size(void)
{
    uint32_t size = 0;

    if (g_sram_size != 0)
        size = 0x80u << (g_sram_size + 3);

    if (size > 0x20000u)
        size = 0x20000u;

    return size;
}

int CMemory_LoadSRAM_recovered(void *memory, const char *filename)
{
    uint32_t expected;
    int fd;
    int bytes_read;

    /* In v0.23 +0x0c is the SRAM pointer used by this method. */
    memset((void *)(uintptr_t)FIELD32(memory, 0x000c),
           g_sram_initial_value, 0x20000);

    expected = expected_sram_size();

    if (expected == 0) {
        if (g_special_load_flag)
            sub_0016fbb4();
        return 1;
    }

    fd = fioOpen_like(filename, 1);
    if (!fd) {
        sub_00183678();
        return 0;
    }

    bytes_read = fioRead_like(fd, g_sram, 0x20000);
    fioClose_like(fd);

    /* Copier-header case: discard a leading 0x200 bytes in-place. */
    if ((uint32_t)(bytes_read - 0x200) == expected) {
        memmove_like(g_sram, g_sram + 0x200, expected);
        bytes_read = (int)(expected + 0x19);
    }

    /* The original has a +0x19-byte special/RTC trailer path. */
    if ((uint32_t)bytes_read == expected + 0x19u) {
        sub_00183d40();
        sub_00183660();
        /* Two bytes in an internal state block are then forced to FF/00. */
    } else {
        sub_00183678();
        /* Optional status-message hook follows in the original. */
    }

    return 1;
}

int CMemory_SaveSRAM_recovered(void *memory, const char *filename)
{
    uint32_t size = 0;
    int fd;

    (void)memory;

    if (g_sram_size != 0)
        size = 0x80u << (g_sram_size + 3);

    /* Two old special-chip/RTC hooks can alter the on-disk size/state. */
    if (g_settings_54) {
        sub_00183c58();
        size += 0x19;
    }
    if (g_settings_64)
        sub_0016fb04();

    if (size > 0x20000u)
        size = 0x20000u;

    if (size == 0 || !g_special_save_enable)
        return 0;

    fd = fioOpen_like(filename, 0x203);
    if (!fd)
        return 0;

    fioWrite_like(fd, g_sram, (int)size);
    fioClose_like(fd);

    if (g_settings_66)
        sub_001833a4((void *)(uintptr_t)0x00423548);

    return 1;
}
