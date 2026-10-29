/*
 * SNES Station v0.23 - S-DD1 File I/O Operations
 * Canonical source from progress12_io_state.c (0x0016fbb4, 0x0016fb04 range)
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct SnesSdd1MemoryView {
    uint8_t reserved_0000[0xb070];
    uint32_t saved;
    uint32_t current;
    uint8_t records[0x10000];
} SnesSdd1MemoryView;

extern SnesSdd1MemoryView g_snes_sdd1_memory;
extern const char g_snes_sdd1_dat_extension[];
extern char *snes_sdd1_get_filename(const char *extension);
extern int snes_sdd1_compare_sdd1_entries(const void *left, const void *right);
extern void snes_sdd1_qsort(void *base, uint32_t count, uint32_t width,
                            int (*compare)(const void *, const void *));
extern int snes_sdd1_fio_open(const char *path, int flags);
extern int snes_sdd1_fio_read(int fd, void *data, int bytes);
extern int snes_sdd1_fio_write(int fd, const void *data, int bytes);
extern int snes_sdd1_fio_close(int fd);

/*
 * 0x0016fbb4: open/read up to 0x10000 bytes, mirror the returned value into
 * both target counters, then close. The target intentionally stores the raw
 * fioRead result; this model does not reinterpret it as a record count.
 */
void snes_sdd1_read_dat_0016fbb4(void)
{
    int fd;

    fd = snes_sdd1_fio_open(snes_sdd1_get_filename(g_snes_sdd1_dat_extension), 1);
    g_snes_sdd1_memory.current = g_snes_sdd1_memory.saved = 0;
    if (fd >= 0) {
        int result = snes_sdd1_fio_read(fd, g_snes_sdd1_memory.records, 0x10000);
        if (result != -1)
            g_snes_sdd1_memory.current = g_snes_sdd1_memory.saved = (uint32_t)result;
        (void)snes_sdd1_fio_close(fd);
    }
}

/*
 * 0x0016fb04: if the two counters differ, sort 8-byte entries, recreate the
 * .dat file, write current*8 bytes, close, then mirror current into saved.
 */
void snes_sdd1_write_dat_0016fb04(void)
{
    int fd;

    if (g_snes_sdd1_memory.current != g_snes_sdd1_memory.saved) {
        snes_sdd1_qsort(g_snes_sdd1_memory.records, g_snes_sdd1_memory.current, 8,
                        snes_sdd1_compare_sdd1_entries);
        fd = snes_sdd1_fio_open(
            snes_sdd1_get_filename(g_snes_sdd1_dat_extension), 0x202);
        if (fd >= 0) {
            (void)snes_sdd1_fio_write(fd, g_snes_sdd1_memory.records,
                                     (int)(g_snes_sdd1_memory.current << 3));
            (void)snes_sdd1_fio_close(fd);
        }
        g_snes_sdd1_memory.saved = g_snes_sdd1_memory.current;
    }
}