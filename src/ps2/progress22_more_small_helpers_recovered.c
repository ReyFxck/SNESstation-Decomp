/*
 * Progress 22: three small typed behavioral migrations from the committed
 * Progress-16 R5900 pseudocode snapshot.
 *
 * These functions preserve the observed target control flow and fixed-global
 * interactions. They are source-model promotions, not machine-code matches.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* --- 0x00103b34: frontend message slide / scissor helper. --- */
extern void *g_frontend_driver_001bb2c0;
extern unsigned char g_frontend_font_001bb748[];

extern void *operator_new_001a9e88(size_t size);
extern void gsDriver_ctor_00198cc8(void *driver);
extern void sub_001019a8(void);
extern void CDVD_Stop_0019c0d0(void);
extern void gsPipe_setScissorRect_00199ef8(void *pipe,
                                           int32_t x1, int32_t y1,
                                           int32_t x2, int32_t y2);
extern void sub_00101b64(void);
extern void gsFont_Print_0019b948(void *font, int x, int xend, int y, int z,
                                  uint64_t colour, int alignment,
                                  const char *text);
extern void snes_p20_00101e8c(void);

void snes_p22_00103b34(const char *text, int travel)
{
    int y = 0x186;
    const int limit = 0x186 - travel;

    if (g_frontend_driver_001bb2c0 == NULL) {
        g_frontend_driver_001bb2c0 = operator_new_001a9e88(0x74u);
        gsDriver_ctor_00198cc8(g_frontend_driver_001bb2c0);
    }

    sub_001019a8();
    CDVD_Stop_0019c0d0();

    if (limit < 0x186) {
        do {
            gsPipe_setScissorRect_00199ef8(g_frontend_driver_001bb2c0,
                                           0, 0, 0x280, 0x1e0);
            sub_00101b64();
            gsPipe_setScissorRect_00199ef8(g_frontend_driver_001bb2c0,
                                           100, 0x74, 0x21c, 0x166);
            gsFont_Print_0019b948(g_frontend_font_001bb748,
                                  0x84, 0x1fc, y, 2,
                                  UINT64_C(0x80ffffff), 2, text);
            --y;
            snes_p20_00101e8c();
        } while (limit < y);
    }

    gsPipe_setScissorRect_00199ef8(g_frontend_driver_001bb2c0,
                                   0, 0, 0x280, 0x1e0);
}

/* --- 0x00104234: load an entire file into a 64-byte-aligned buffer. --- */
typedef struct P22FileBufferTarget {
    uint32_t reserved_00;
    uint32_t data_ee;  /* +0x04 target pointer */
    int32_t size;      /* +0x08 */
} P22FileBufferTarget;

_Static_assert(offsetof(P22FileBufferTarget, data_ee) == 4,
               "0x00104234 data pointer offset");
_Static_assert(offsetof(P22FileBufferTarget, size) == 8,
               "0x00104234 size offset");

extern int fioOpen_0019cfc0(const char *path, int mode);
extern int fioLseek_0019d360(int fd, int offset, int whence);
extern int fioClose_0019d090(int fd);
extern int fioRead_0019d120(int fd, void *buffer, int size);
extern void *memalign_0019e698(size_t alignment, size_t size);

int snes_p22_00104234(P22FileBufferTarget *state, const char *path)
{
    int fd;
    int size;
    int rounded;
    void *buffer;

    fd = fioOpen_0019cfc0(path, 1);
    if (fd < 0)
        return 2;

    size = fioLseek_0019d360(fd, 0, 2);
    state->size = size;
    if (size == 0) {
        (void)fioClose_0019d090(fd);
        return 2;
    }

    rounded = size;
    if (rounded < 0)
        rounded += 0x3f;
    rounded = (rounded >> 6) * 0x40 + 0x40;

    buffer = memalign_0019e698(0x40u, (size_t)rounded);
    state->data_ee = (uint32_t)(uintptr_t)buffer;
    (void)memset(buffer, 0xff, (size_t)rounded);

    (void)fioLseek_0019d360(fd, 0, 0);
    (void)fioRead_0019d120(fd, buffer, state->size);
    ((uint8_t *)buffer)[state->size] = 0;
    (void)fioClose_0019d090(fd);
    return 1;
}

/* --- 0x0015d334: small conditional PPU timing/update helper. --- */
extern uint8_t *g_fillram_0034e2c4;
/* Shared frontend/PPU state anchored at 0x003454e0. */
extern uint8_t g_state_003454e0[];
extern int sub_001309c4(uint32_t value);
extern void sub_00116128(int value);

void snes_p22_0015d334(void)
{
    uint8_t *fillram;
    int status;

    if (g_state_003454e0[0x60] != 0) {
        fillram = g_fillram_0034e2c4;
        if ((fillram[0x3030] & 0x20u) != 0 &&
            (fillram[0x303a] & 0x18u) == 0x18u) {
            if (!g_state_003454e0[0x12b] || g_state_003454e0[0x12a])
                (void)sub_001309c4(~0u);
            else
                (void)sub_001309c4((fillram[0x3039] & 1u) ? 700u : 350u);

            status = g_fillram_0034e2c4[0x3030] |
                     (g_fillram_0034e2c4[0x3031] << 8);
            if ((status & 0x8020) == 0x8000)
                sub_00116128(4);
        }
    }
}
