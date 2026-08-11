/*
 * SNES Station v0.23 WIP — unzip 0.15-style API glue.
 *
 * Binary range mapped in progress 3: 0x0018f010..0x00190700.
 * This file reconstructs the small/medium helpers whose semantics are fully
 * visible. Large stateful functions are declared with target VAs and remain
 * IDENTIFIED until their concrete target structure layouts are recovered.
 *
 * Target was compiled with -mlong64, so ZIP uLong-like fields are represented
 * as uint64_t here. Adapter structures are behavioral and are NOT yet claims
 * about exact in-memory target offsets.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define UNZ_OK 0
#define UNZ_EOF 0
#define UNZ_ERRNO (-1)
#define UNZ_END_OF_LIST_OF_FILE (-100)
#define UNZ_PARAMERROR (-102)
#define UNZ_CRCERROR (-105)
#define UNZ_MAXFILENAMEINZIP 256u
#define BUFREADCOMMENT 0x400u

typedef struct {
    uint64_t number_entry;
    uint64_t size_comment;
} unz_global_info_recovered;

typedef struct {
    uint32_t tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year;
} tm_unz_recovered;

typedef struct {
    uint64_t size_filename;
    uint64_t size_file_extra;
    uint64_t size_file_comment;
} unz_file_info_recovered;

typedef struct {
    uint8_t *read_buffer;
    uint64_t total_out;
    uint64_t rest_read_uncompressed;
    uint64_t crc32;
    uint64_t crc32_wait;
    uint64_t offset_local_extrafield;
    uint64_t size_local_extrafield;
    uint64_t pos_local_extrafield;
    int stream_initialised;
    int file;
} file_in_zip_read_info_recovered;

typedef struct {
    int file;
    unz_global_info_recovered gi;
    uint64_t num_file;
    uint64_t pos_in_central_dir;
    int current_file_ok;
    uint64_t central_pos;
    uint64_t offset_central_dir;
    unz_file_info_recovered cur_file_info;
    file_in_zip_read_info_recovered *pfile_in_zip_read;
} unz_state_recovered;

typedef struct {
    int64_t (*seek)(int file, int64_t offset, int whence);
    int64_t (*tell)(int file);
    int (*read)(int file, void *dst, unsigned size);
    int (*close)(int file);
    int (*error)(int file);
    int (*inflate_end)(void *stream_owner);
} unz_ops_recovered;

extern unz_ops_recovered g_unz_ops_recovered;

int unzCloseCurrentFile_recovered(unz_state_recovered *s);

/* Large target functions mapped but not reconstructed in this adapter file. */
extern int unzlocal_GetCurrentFileInfoInternal_0018f6cc(
    unz_state_recovered *file, unz_file_info_recovered *info,
    void *internal_info, char *name, uint64_t name_size,
    void *extra, uint64_t extra_size, char *comment, uint64_t comment_size);

/* Target VA 0x0018f010. */
int unzlocal_getByte_recovered(int fin, uint64_t *pi)
{
    uint8_t c = 0;
    int got = g_unz_ops_recovered.read != NULL
            ? g_unz_ops_recovered.read(fin, &c, 1) : 0;
    if (got == 1) {
        *pi = c;
        return UNZ_OK;
    }
    if (g_unz_ops_recovered.error != NULL && g_unz_ops_recovered.error(fin))
        return UNZ_ERRNO;
    return UNZ_EOF;
}

/* Target VA 0x0018f070. */
int unzlocal_getShort_recovered(int fin, uint64_t *out)
{
    uint64_t x = 0, i = 0;
    int err = unzlocal_getByte_recovered(fin, &i);
    x = i;
    if (err == UNZ_OK) err = unzlocal_getByte_recovered(fin, &i);
    x += i << 8;
    *out = err == UNZ_OK ? x : 0;
    return err;
}

/* Target VA 0x0018f0ec. */
int unzlocal_getLong_recovered(int fin, uint64_t *out)
{
    uint64_t x = 0, i = 0;
    int err = unzlocal_getByte_recovered(fin, &i);
    x = i;
    if (err == UNZ_OK) err = unzlocal_getByte_recovered(fin, &i);
    x += i << 8;
    if (err == UNZ_OK) err = unzlocal_getByte_recovered(fin, &i);
    x += i << 16;
    if (err == UNZ_OK) err = unzlocal_getByte_recovered(fin, &i);
    x += i << 24;
    *out = err == UNZ_OK ? x : 0;
    return err;
}

/* Target VA 0x0018f1b8. */
int strcmpcasenosensitive_internal_recovered(const char *a, const char *b)
{
    for (;;) {
        int c1 = (signed char)*a++;
        int c2 = (signed char)*b++;
        if (c1 >= 'a' && c1 <= 'z') c1 -= 0x20;
        if (c2 >= 'a' && c2 <= 'z') c2 -= 0x20;
        if (c1 == 0) return c2 == 0 ? 0 : -1;
        if (c2 == 0) return 1;
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
    }
}

/* Target starts at 0x0018f240; the stack adjustment is at +4. */
int unzStringFileNameCompare_recovered(const char *a, const char *b, int sensitivity)
{
    if (sensitivity == 0) sensitivity = 2;
    if (sensitivity == 1) return strcmp(a, b);
    return strcmpcasenosensitive_internal_recovered(a, b);
}

/* Target VA 0x0018f27c. */
uint64_t unzlocal_SearchCentralDir_recovered(int fin)
{
    uint8_t *buf;
    uint64_t size, back, max_back = 0xffffu, found = 0;
    if (g_unz_ops_recovered.seek == NULL || g_unz_ops_recovered.tell == NULL ||
        g_unz_ops_recovered.read == NULL)
        return 0;
    if (g_unz_ops_recovered.seek(fin, 0, 2) != 0) return 0;
    size = (uint64_t)g_unz_ops_recovered.tell(fin);
    if (max_back > size) max_back = size;
    buf = (uint8_t *)malloc(BUFREADCOMMENT + 4u);
    if (buf == NULL) return 0;
    back = 4;
    while (back < max_back) {
        uint64_t read_pos, read_size, i;
        back = back + BUFREADCOMMENT > max_back ? max_back : back + BUFREADCOMMENT;
        read_pos = size - back;
        read_size = (BUFREADCOMMENT + 4u < size - read_pos)
                  ? BUFREADCOMMENT + 4u : size - read_pos;
        if (g_unz_ops_recovered.seek(fin, (int64_t)read_pos, 0) != 0) break;
        if (g_unz_ops_recovered.read(fin, buf, (unsigned)read_size) != (int)read_size) break;
        for (i = 0; i + 3 < read_size; ++i) {
            if (buf[i] == 0x50 && buf[i + 1] == 0x4b &&
                buf[i + 2] == 0x05 && buf[i + 3] == 0x06) {
                found = read_pos + i;
                break;
            }
        }
        if (found != 0) break;
    }
    free(buf);
    return found;
}

/* Target VA 0x0018f5e0. */
int unzClose_recovered(unz_state_recovered *s)
{
    if (s == NULL) return UNZ_PARAMERROR;
    /* Target closes an active current file before closing the archive. */
    if (s->pfile_in_zip_read != NULL)
        (void)unzCloseCurrentFile_recovered(s);
    if (g_unz_ops_recovered.close != NULL) g_unz_ops_recovered.close(s->file);
    free(s);
    return UNZ_OK;
}

/* Target VA 0x0018f638; no stack-frame prologue. */
int unzGetGlobalInfo_recovered(unz_state_recovered *s,
                               unz_global_info_recovered *out)
{
    if (s == NULL) return UNZ_PARAMERROR;
    *out = s->gi;
    return UNZ_OK;
}

/* Target VA 0x0018f654; leaf function. */
void unzlocal_DosDateToTmuDate_recovered(uint64_t dos, tm_unz_recovered *tm)
{
    uint64_t date = dos >> 16;
    tm->tm_mday = (uint32_t)(date & 0x1f);
    tm->tm_mon  = (uint32_t)(((date & 0x1e0) >> 5) - 1u);
    tm->tm_year = (uint32_t)(((date & 0xfe00) >> 9) + 1980u);
    tm->tm_hour = (uint32_t)((dos & 0xf800) >> 11);
    tm->tm_min  = (uint32_t)((dos & 0x07e0) >> 5);
    tm->tm_sec  = (uint32_t)(2u * (dos & 0x1f));
}

/* Target VA 0x0018fab4. */
int unzGetCurrentFileInfo_recovered(unz_state_recovered *s,
                                    unz_file_info_recovered *info,
                                    char *name, uint64_t name_size,
                                    void *extra, uint64_t extra_size,
                                    char *comment, uint64_t comment_size)
{
    return unzlocal_GetCurrentFileInfoInternal_0018f6cc(
        s, info, NULL, name, name_size, extra, extra_size, comment, comment_size);
}

/* Target VA 0x0018faec. */
int unzGoToFirstFile_recovered(unz_state_recovered *s)
{
    int err;
    if (s == NULL) return UNZ_PARAMERROR;
    s->pos_in_central_dir = s->offset_central_dir;
    s->num_file = 0;
    err = unzlocal_GetCurrentFileInfoInternal_0018f6cc(
        s, &s->cur_file_info, NULL, NULL, 0, NULL, 0, NULL, 0);
    s->current_file_ok = (err == UNZ_OK);
    return err;
}

/* Target VA 0x0018fb54. */
int unzGoToNextFile_recovered(unz_state_recovered *s)
{
    int err;
    if (s == NULL) return UNZ_PARAMERROR;
    if (!s->current_file_ok || s->num_file + 1 == s->gi.number_entry)
        return UNZ_END_OF_LIST_OF_FILE;
    s->pos_in_central_dir += 46u + s->cur_file_info.size_filename +
                             s->cur_file_info.size_file_extra +
                             s->cur_file_info.size_file_comment;
    ++s->num_file;
    err = unzlocal_GetCurrentFileInfoInternal_0018f6cc(
        s, &s->cur_file_info, NULL, NULL, 0, NULL, 0, NULL, 0);
    s->current_file_ok = (err == UNZ_OK);
    return err;
}

/* Target VA 0x0018fbfc. */
int unzLocateFile_recovered(unz_state_recovered *s, const char *name, int sensitivity)
{
    uint64_t saved_num, saved_pos;
    int err;
    if (s == NULL || strlen(name) >= UNZ_MAXFILENAMEINZIP)
        return UNZ_PARAMERROR;
    if (!s->current_file_ok) return UNZ_END_OF_LIST_OF_FILE;
    saved_num = s->num_file;
    saved_pos = s->pos_in_central_dir;
    err = unzGoToFirstFile_recovered(s);
    while (err == UNZ_OK) {
        char current[UNZ_MAXFILENAMEINZIP + 1];
        current[0] = 0;
        (void)unzGetCurrentFileInfo_recovered(s, NULL, current,
                                              UNZ_MAXFILENAMEINZIP,
                                              NULL, 0, NULL, 0);
        if (unzStringFileNameCompare_recovered(current, name, sensitivity) == 0)
            return UNZ_OK;
        err = unzGoToNextFile_recovered(s);
    }
    s->num_file = saved_num;
    s->pos_in_central_dir = saved_pos;
    return err;
}

/* Target VA 0x00190458; leaf function after unzReadCurrentFile tail blocks. */
int64_t unztell_recovered(unz_state_recovered *s)
{
    if (s == NULL || s->pfile_in_zip_read == NULL) return UNZ_PARAMERROR;
    return (int64_t)s->pfile_in_zip_read->total_out;
}

/* Target VA 0x00190474; leaf function. */
int unzeof_recovered(unz_state_recovered *s)
{
    if (s == NULL || s->pfile_in_zip_read == NULL) return UNZ_PARAMERROR;
    return s->pfile_in_zip_read->rest_read_uncompressed == 0 ? 1 : 0;
}

/* Target VA 0x001904a0. */
int unzGetLocalExtrafield_recovered(unz_state_recovered *s, void *buf, unsigned len)
{
    file_in_zip_read_info_recovered *r;
    uint64_t available;
    unsigned now;
    if (s == NULL || s->pfile_in_zip_read == NULL) return UNZ_PARAMERROR;
    r = s->pfile_in_zip_read;
    available = r->size_local_extrafield - r->pos_local_extrafield;
    if (buf == NULL) return (int)available;
    now = len > available ? (unsigned)available : len;
    if (now == 0) return 0;
    if (g_unz_ops_recovered.seek == NULL || g_unz_ops_recovered.read == NULL)
        return UNZ_ERRNO;
    if (g_unz_ops_recovered.seek(r->file,
        (int64_t)(r->offset_local_extrafield + r->pos_local_extrafield), 0) != 0)
        return UNZ_ERRNO;
    if (g_unz_ops_recovered.read(r->file, buf, now) != (int)now)
        return UNZ_ERRNO;
    return (int)now;
}

/* Target VA 0x00190578. */
int unzCloseCurrentFile_recovered(unz_state_recovered *s)
{
    file_in_zip_read_info_recovered *r;
    int err = UNZ_OK;
    if (s == NULL || s->pfile_in_zip_read == NULL) return UNZ_PARAMERROR;
    r = s->pfile_in_zip_read;
    if (r->rest_read_uncompressed == 0 && r->crc32 != r->crc32_wait)
        err = UNZ_CRCERROR;
    free(r->read_buffer);
    r->read_buffer = NULL;
    if (r->stream_initialised && g_unz_ops_recovered.inflate_end != NULL)
        g_unz_ops_recovered.inflate_end(r);
    r->stream_initialised = 0;
    free(r);
    s->pfile_in_zip_read = NULL;
    return err;
}

/* Target VA 0x00190628. */
int unzGetGlobalComment_recovered(unz_state_recovered *s, char *comment,
                                  uint64_t buffer_size)
{
    uint64_t n;
    if (s == NULL) return UNZ_PARAMERROR;
    n = buffer_size > s->gi.size_comment ? s->gi.size_comment : buffer_size;
    if (g_unz_ops_recovered.seek == NULL || g_unz_ops_recovered.read == NULL)
        return UNZ_ERRNO;
    if (g_unz_ops_recovered.seek(s->file, (int64_t)(s->central_pos + 22u), 0) != 0)
        return UNZ_ERRNO;
    if (n != 0) {
        if (comment != NULL) comment[0] = 0;
        if (comment == NULL || g_unz_ops_recovered.read(s->file, comment, (unsigned)n) != (int)n)
            return UNZ_ERRNO;
    }
    if (comment != NULL && buffer_size > s->gi.size_comment)
        comment[s->gi.size_comment] = 0;
    return (int)n;
}
