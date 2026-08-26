/*
 * SNES Station v0.23 WIP — legacy ZIP Shrink (LZW + partial clearing).
 *
 * Target VAs:
 *   0x0018ea64  unShrink
 *   0x0018eeb4  partial_clear (callable internal block, no stack prologue)
 *
 * The missing prologue on partial_clear is important: a prologue-only function
 * scanner falsely makes unShrink appear to run until 0x0018f010.
 */
#include <stdint.h>
#include "../../include/legacy_zip_recovered.h"

#define SHRINK_HSIZE 8192
#define SHRINK_INIT_BITS 9
#define SHRINK_FIRST 257
#define SHRINK_CLEAR 256
#define SHRINK_MAX_BITS 13

static int codesize_recovered;
static int maxcode_recovered;
static int maxcodemax_recovered;
static int free_ent_recovered;

static unsigned read_shrink_code(void)
{
    uint64_t mask;
    while (g_legacy_zip_bits_left < (unsigned)codesize_recovered &&
           !g_legacy_zip_zipeof)
        FillBitBuffer_recovered();
    mask = (UINT64_C(1) << codesize_recovered) - 1u;
    {
        unsigned code = (unsigned)(g_legacy_zip_bitbuf & mask);
        g_legacy_zip_bitbuf >>= codesize_recovered;
        g_legacy_zip_bits_left -= (unsigned)codesize_recovered;
        return code;
    }
}

/* Target VA 0x0018eeb4. */
void partial_clear_recovered(void)
{
    LegacyShrinkWorkspace *ws = &g_shrink_workspace_recovered;
    int cd;

    for (cd = SHRINK_FIRST; cd < free_ent_recovered; ++cd)
        ws->prefix[cd] = (int16_t)((uint16_t)ws->prefix[cd] | 0x8000u);

    for (cd = SHRINK_FIRST; cd < free_ent_recovered; ++cd) {
        int pr = (int)((uint16_t)ws->prefix[cd] & 0x7fffu);
        if (pr >= SHRINK_FIRST)
            ws->prefix[pr] = (int16_t)((uint16_t)ws->prefix[pr] & 0x7fffu);
    }

    for (cd = SHRINK_FIRST; cd < free_ent_recovered; ++cd)
        if (((uint16_t)ws->prefix[cd] & 0x8000u) != 0)
            ws->prefix[cd] = -1;

    cd = SHRINK_FIRST;
    while (cd < maxcodemax_recovered && ws->prefix[cd] != -1)
        ++cd;
    free_ent_recovered = cd;
}

/* Target VA 0x0018ea64. */
void unShrink_recovered(void)
{
    LegacyShrinkWorkspace *ws = &g_shrink_workspace_recovered;
    int code, finchar, oldcode, incode, stackp;

    codesize_recovered = SHRINK_INIT_BITS;
    maxcode_recovered = (1 << codesize_recovered) - 1;
    maxcodemax_recovered = SHRINK_HSIZE;
    free_ent_recovered = SHRINK_FIRST;

    for (code = SHRINK_HSIZE - 1; code > 255; --code)
        ws->prefix[code] = -1;
    for (code = 255; code >= 0; --code) {
        ws->prefix[code] = 0;
        ws->suffix[code] = (uint8_t)code;
    }

    oldcode = (int)read_shrink_code();
    if (g_legacy_zip_zipeof) return;
    finchar = oldcode;
    uRam0044e206[0] = (uint8_t)finchar;
    flush_stack_recovered(1);
    stackp = SHRINK_HSIZE;

    while (!g_legacy_zip_zipeof) {
        code = (int)read_shrink_code();
        if (g_legacy_zip_zipeof) return;

        while (code == SHRINK_CLEAR) {
            code = (int)read_shrink_code();
            if (code == 1) {
                ++codesize_recovered;
                maxcode_recovered = codesize_recovered == SHRINK_MAX_BITS
                                  ? maxcodemax_recovered
                                  : (1 << codesize_recovered) - 1;
            } else if (code == 2) {
                partial_clear_recovered();
            }
            code = (int)read_shrink_code();
            if (g_legacy_zip_zipeof) return;
        }

        incode = code;
        if (ws->prefix[code] == -1) {
            uRam0044e206[--stackp] = (uint8_t)finchar;
            code = oldcode;
        }

        while (code >= SHRINK_FIRST) {
            if (ws->prefix[code] == -1) {
                uRam0044e206[--stackp] = (uint8_t)finchar;
                code = oldcode;
            } else {
                uRam0044e206[--stackp] = ws->suffix[code];
                code = ws->prefix[code];
            }
        }

        finchar = ws->suffix[code];
        uRam0044e206[--stackp] = (uint8_t)finchar;
        flush_stack_recovered((unsigned)(SHRINK_HSIZE - stackp));
        stackp = SHRINK_HSIZE;

        code = free_ent_recovered;
        if (code < maxcodemax_recovered) {
            ws->prefix[code] = (int16_t)oldcode;
            ws->suffix[code] = (uint8_t)finchar;
            do ++code;
            while (code < maxcodemax_recovered && ws->prefix[code] != -1);
            free_ent_recovered = code;
        }
        oldcode = incode;
    }
}
