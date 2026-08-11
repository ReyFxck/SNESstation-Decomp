/*
 * SNES Station v0.23 WIP reconstruction
 * Original main VA: 0x00104f18
 *
 * This file reconstructs only the startup portion that is currently high
 * confidence.  Unknown application/renderer calls remain explicitly named by
 * address instead of being guessed.
 */

#include <stdint.h>
#include <stddef.h>

#include "../../include/gslib_recovered.h"

extern void *operator_new_u32(unsigned size);          /* 0x001a9e88 */
extern void gsDriver_ctor_00198cc8(gsDriverRecovered *obj); /* 0x00198cc8 */
extern void gsDriver_clearScreen_001990f8(gsDriverRecovered *obj); /* 0x001990f8 */

extern int SifIopReset(const char *arg, int mode);     /* 0x0019d740 */
extern void SifInitRpc(int mode);                      /* 0x0019cc0c */
extern int SifLoadModule(const char *path, int arg_len,
                         const char *args);             /* 0x0019d600 */
extern int mcInit(int type);                           /* 0x001a08ec */
extern int loadModuleBuffer(const void *module, int size,
                            int arg_len, const char *args); /* 0x00104e7c */
extern int CDVD_Init(void);                            /* 0x0019be70 */
extern int puts(const char *s);                        /* 0x0019e414 */

/* Embedded modules recovered directly from the unpacked image. */
extern const unsigned char embedded_cdvd_irx[];        /* VA 0x001ec300 */
extern const unsigned int  embedded_cdvd_irx_size;     /* VA 0x001f40d4 = 0x7dd4 */
extern const unsigned char embedded_amigamod_irx[];    /* VA 0x001f6140 */
extern const unsigned int  embedded_amigamod_irx_size;/* VA 0x001fafa0 = 0x4e5d */
extern const unsigned char embedded_sjpcm_irx[];       /* VA 0x001f4100 */
extern const unsigned int  embedded_sjpcm_irx_size;    /* VA 0x001f60c8 = 0x1fc5 */

/* Original global at VA 0x001baf08 (name not recovered yet). */
static gsDriverRecovered *g_gs_driver;

/* Original global VA 0x001eba80; purpose still unknown. */
static uint32_t g_unknown_magic;

/*
 * Partial reconstruction of main(int, char **).
 * The original stores argv but does not preserve argc in a callee-saved
 * register at entry.
 */
int main_bootstrap_recovered(int argc, char **argv)
{
    (void)argv;
    gsDriverRecovered *obj;
    (void)argc;

    /* The target literally allocates 0x74 bytes. That is now proven to be
       sizeof(gsDriver) for this embedded Hiryu GSLIB revision. The old GCC
       constructor return value is ignored; main preserves the allocation in
       $17 and stores that pointer globally after the call. */
    obj = (gsDriverRecovered *)operator_new_u32(0x74);
    gsDriver_ctor_00198cc8(obj);
    g_gs_driver = obj;
    gsDriver_clearScreen_001990f8(g_gs_driver);

    SifIopReset("rom0:UDNL rom0:EELOADCNF", 0);
    SifInitRpc(0);

    g_unknown_magic = 0xfc660869u;

    SifLoadModule("rom0:XSIO2MAN", 0, NULL);
    SifLoadModule("rom0:XPADMAN",  0, NULL);
    SifLoadModule("rom0:XMTAPMAN", 0, NULL);
    SifLoadModule("rom0:XMCMAN",   0, NULL);
    SifLoadModule("rom0:XMCSERV",  0, NULL);

    if (mcInit(1) < 0) {
        puts("Failed to initialise memcard !");
        for (;;) {
            /* original binary intentionally hangs here */
        }
    }

    SifLoadModule("rom0:LIBSD", 0, NULL);

    loadModuleBuffer(embedded_cdvd_irx,     embedded_cdvd_irx_size,     0, NULL);
    loadModuleBuffer(embedded_amigamod_irx, embedded_amigamod_irx_size, 0, NULL);
    loadModuleBuffer(embedded_sjpcm_irx,    embedded_sjpcm_irx_size,    0, NULL);

    CDVD_Init();

    /* main continues at original VA 0x00105060. */
    return 0;
}
