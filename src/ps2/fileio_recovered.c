/*
 * Recovered old libkernel file I/O wrappers used by SNES Station v0.23.
 * Target entries: 0x0019cfc0..0x0019d5ff.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../../include/ps2_libkernel_recovered.h"

typedef void (*SifRpcEndFunc_recovered)(void *);

extern int fioInit_recovered(void);                 /* target 0x0019f600 */
extern int WaitSema(int);                           /* 0x0019cea0 */
extern int iSignalSema(int);                        /* 0x0019ce90 */
extern void SifWriteBackDCache(void *, int);        /* 0x0019cf10 */
extern int SifCallRpc(SifRpcClientData32 *, int, int,
                      void *, int, void *, int, SifRpcEndFunc_recovered, void *);

extern SifRpcClientData32 fio_cd_recovered;
extern int fio_init_recovered;
extern int fio_completion_sema_recovered;
extern int fio_block_mode_recovered;
static int fio_recv_data_recovered[512];
static int fio_intr_data_recovered[32];

enum { FIO_WAIT_RECOVERED = 0, FIO_NOWAIT_RECOVERED = 1 };

static void fio_intr_recovered(void *arg)
{
    (void)arg;
    (void)iSignalSema(fio_completion_sema_recovered);
}

typedef struct {
    uint32_t size1;
    uint32_t size2;
    ee_addr32_t dest1;
    ee_addr32_t dest2;
    uint8_t buf1[16];
    uint8_t buf2[16];
} fio_read_data_recovered;

typedef struct {
    int32_t mode;
    char name[256];
    uint8_t align_pad[12];
} fio_open_arg_recovered;

typedef struct {
    int32_t fd;
    ee_addr32_t ptr;
    int32_t size;
    ee_addr32_t read_data;
} fio_read_arg_recovered;

typedef struct {
    int32_t fd;
    ee_addr32_t ptr;
    uint32_t size;
    uint32_t mis;
    uint8_t aligned[16];
} fio_write_arg_recovered;

typedef struct {
    union { int32_t fd; int32_t result; } p;
    int32_t offset;
    int32_t whence;
    uint32_t align_pad;
} fio_lseek_arg_recovered;

typedef union {
    char path[257]; /* target writes a terminator one byte beyond RPC payload */
    int32_t result;
} fio_mkdir_local_recovered;

typedef char recovered_assert_fio_read_data_size[(sizeof(fio_read_data_recovered) == 0x30) ? 1 : -1];
typedef char recovered_assert_fio_open_arg_size[(sizeof(fio_open_arg_recovered) == 0x110) ? 1 : -1];
typedef char recovered_assert_fio_read_arg_size[(sizeof(fio_read_arg_recovered) == 0x10) ? 1 : -1];
typedef char recovered_assert_fio_write_arg_size[(sizeof(fio_write_arg_recovered) == 0x20) ? 1 : -1];
typedef char recovered_assert_fio_lseek_arg_size[(sizeof(fio_lseek_arg_recovered) == 0x10) ? 1 : -1];

/* 0x0019cfc0 */
int fioOpen_0019cfc0(const char *name, int mode)
{
    fio_open_arg_recovered arg;
    int res;
    if (!fio_init_recovered && (res = fioInit_recovered()) < 0)
        return res;
    (void)WaitSema(fio_completion_sema_recovered);
    arg.mode = mode;
    strncpy(arg.name, name, sizeof(arg.name));
    if ((res = SifCallRpc(&fio_cd_recovered, 0, fio_block_mode_recovered,
                          &arg, 0x110, fio_recv_data_recovered, 4,
                          fio_intr_recovered, NULL)) < 0)
        return res;
    return fio_block_mode_recovered == FIO_NOWAIT_RECOVERED ? 0 : fio_recv_data_recovered[0];
}

/* 0x0019d090 */
int fioClose_0019d090(int fd)
{
    union { int fd; int result; } arg;
    int res;
    if (!fio_init_recovered && (res = fioInit_recovered()) < 0)
        return res;
    (void)WaitSema(fio_completion_sema_recovered);
    arg.fd = fd;
    if ((res = SifCallRpc(&fio_cd_recovered, 1, 0, &arg, 4, &arg, 4,
                          fio_intr_recovered, NULL)) < 0)
        return res;
    return arg.result;
}

/* 0x0019d4b0 -- callback used by fioRead. */
void fio_read_intr_0019d4b0(void *argp)
{
    fio_read_data_recovered *data = argp;
    /* Target accesses this through KSEG1; C-level behavior is the same copy. */
    if (data->size1 != 0 && data->dest1 != 0)
        memcpy(ee_ptr_from_addr32(data->dest1), data->buf1, data->size1);
    if (data->size2 != 0 && data->dest2 != 0)
        memcpy(ee_ptr_from_addr32(data->dest2), data->buf2, data->size2);
    (void)iSignalSema(fio_completion_sema_recovered);
}

/* 0x0019d120 */
int fioRead_0019d120(int fd, void *ptr, int size)
{
    fio_read_arg_recovered arg;
    int res;
    if (!fio_init_recovered && (res = fioInit_recovered()) < 0)
        return res;
    (void)WaitSema(fio_completion_sema_recovered);
    arg.fd = fd;
    arg.ptr = ee_addr32_from_ptr(ptr);
    arg.size = size;
    arg.read_data = ee_addr32_from_ptr(fio_intr_data_recovered);
    if ((((uintptr_t)ptr) & 0x20000000u) == 0)
        SifWriteBackDCache(ptr, size);
    SifWriteBackDCache(fio_intr_data_recovered, 128);
    SifWriteBackDCache(&arg, 0x10);
    if ((res = SifCallRpc(&fio_cd_recovered, 2, fio_block_mode_recovered,
                          &arg, 0x10, fio_recv_data_recovered, 4,
                          fio_read_intr_0019d4b0, fio_intr_data_recovered)) < 0)
        return res;
    return fio_block_mode_recovered == FIO_NOWAIT_RECOVERED ? 0 : fio_recv_data_recovered[0];
}

/* 0x0019d244 */
int fioWrite_0019d244(int fd, const void *ptr, int size)
{
    fio_write_arg_recovered arg;
    int mis = 0;
    int res;
    if (!fio_init_recovered && (res = fioInit_recovered()) < 0)
        return res;
    (void)WaitSema(fio_completion_sema_recovered);
    arg.fd = fd;
    arg.ptr = ee_addr32_from_ptr(ptr);
    arg.size = (uint32_t)size;
    if (((uintptr_t)ptr & 0xfu) != 0) {
        mis = 16 - (int)((uintptr_t)ptr & 0xfu);
        if (mis > size) mis = size;
    }
    arg.mis = (uint32_t)mis;
    if (mis != 0)
        memcpy(arg.aligned, ptr, (size_t)mis);
    if ((((uintptr_t)ptr) & 0x20000000u) == 0)
        SifWriteBackDCache((void *)ptr, size);
    if ((res = SifCallRpc(&fio_cd_recovered, 3, fio_block_mode_recovered,
                          &arg, 0x20, fio_recv_data_recovered, 4,
                          fio_intr_recovered, NULL)) < 0)
        return res;
    return fio_block_mode_recovered == FIO_NOWAIT_RECOVERED ? 0 : fio_recv_data_recovered[0];
}

/* 0x0019d360 */
int fioLseek_0019d360(int fd, int offset, int whence)
{
    fio_lseek_arg_recovered arg;
    int res;
    if (!fio_init_recovered && (res = fioInit_recovered()) < 0)
        return res;
    (void)WaitSema(fio_completion_sema_recovered);
    arg.p.fd = fd;
    arg.offset = offset;
    arg.whence = whence;
    if ((res = SifCallRpc(&fio_cd_recovered, 4, 0, &arg, 0x10,
                          &arg, 4, fio_intr_recovered, NULL)) < 0)
        return res;
    return arg.p.result;
}

/* 0x0019d410 */
int fioMkdir_0019d410(const char *path)
{
    fio_mkdir_local_recovered arg;
    int res;
    if (!fio_init_recovered && (res = fioInit_recovered()) < 0)
        return res;
    (void)WaitSema(fio_completion_sema_recovered);
    strncpy(arg.path, path, 0x100);
    arg.path[0x100] = '\0'; /* target writes this outside the 0x100-byte RPC payload */
    if ((res = SifCallRpc(&fio_cd_recovered, 7, 0, &arg, 0x100,
                          &arg, 4, fio_intr_recovered, NULL)) < 0)
        return res;
    return arg.result;
}

/* 0x0019d534 */
int fioPutc_0019d534(int fd, int c)
{
    return fioWrite_0019d244(fd, &c, 1);
}

/* 0x0019d558
 * Target bulk-reads n bytes and seeks backward after NUL/newline, rather than
 * doing a byte-at-a-time fgets-style loop.
 */
int fioGets_0019d558(int fd, char *buffer, int n)
{
    int read = fioRead_0019d120(fd, buffer, n);
    int i = 0;
    int limit = read - 1;
    while (i < limit) {
        if (buffer[i] == '\0') {
            (void)fioLseek_0019d360(fd, i - read, 1);
            return i;
        }
        if (buffer[i] == '\n') {
            ++i;
            (void)fioLseek_0019d360(fd, i - read, 1);
            buffer[i] = '\0';
            return i;
        }
        ++i;
    }
    return i;
}
