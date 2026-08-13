/* SNES Station v0.23 PS2 save-path helpers, recovered from 0x00105750+. */
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/*
 * The original image keeps two independent scratch/path work areas:
 *
 * SRAM:  path=0x00429300 short=0x00429700 drive=0x00429720
 *        dir=0x00429728 name=0x00429b28 ext=0x00429f28
 * State: path=0x0042a380 short=0x0042a780 drive=0x0042a7a0
 *        dir=0x0042a7a8 name=0x0042aba8 ext=0x0042afa8
 *
 * These C objects preserve the observed separation/size relationships without
 * claiming final linker ownership or exact absolute placement.
 */
static char sram_path_buf[0x400];
static char sram_shortname_buf[0x20];
static char sram_drive_buf[8];
static char sram_dir_buf[0x400];
static char sram_basename_buf[0x400];
static char sram_ext_buf[0x400];

static char state_path_buf[0x400];
static char state_shortname_buf[0x20];
static char state_drive_buf[8];
static char state_dir_buf[0x400];
static char state_basename_buf[0x400];
static char state_ext_buf[0x400];

extern void split_path_00105ae8(const char *src,
                                char *drive, char *dir,
                                char *name, char *ext);
extern char g_loaded_rom_path[]; /* object begins at EE VA 0x0035b328 */

char *build_sram_path_00105750(void)
{
    split_path_00105ae8(g_loaded_rom_path,
                        sram_drive_buf, sram_dir_buf,
                        sram_basename_buf, sram_ext_buf);

    /* 0x0010578c..0x001057a0: target calls strncpy(..., 0x1b). */
    strncpy(sram_shortname_buf, sram_basename_buf, 0x1b);

    /* The prefix copy is inlined in the linked target, then two strcat calls. */
    strcpy(sram_path_buf, "mc0:SNES_EMU/");
    strcat(sram_path_buf, sram_shortname_buf);
    strcat(sram_path_buf, ".SRM");
    return sram_path_buf;
}

/*
 * 0x001057fc..0x00105894, promoted from Progress-16 structural pseudocode.
 * Exact observed call sequence:
 *   split_path_00105ae8(0x35b328, 0x42a7a0, 0x42a7a8,
 *                       0x42aba8, 0x42afa8)
 *   strncpy(0x42a780, 0x42aba8, 0x1b)
 *   sprintf(0x42a380, "mc0:SNES_EMU/%s.%03d", 0x42a780, slot)
 * and return 0x42a380.
 */
char *build_state_path_001057fc(int slot)
{
    split_path_00105ae8(g_loaded_rom_path,
                        state_drive_buf, state_dir_buf,
                        state_basename_buf, state_ext_buf);
    strncpy(state_shortname_buf, state_basename_buf, 0x1b);
    sprintf(state_path_buf, "mc0:SNES_EMU/%s.%03d",
            state_shortname_buf, slot);
    return state_path_buf;
}

/* Compatibility spellings retained for older research callers. */
char *build_sram_path_recovered(void)
{
    return build_sram_path_00105750();
}

char *build_state_path_recovered(int slot)
{
    return build_state_path_001057fc(slot);
}
