/* SNES Station v0.23 PS2 save-path helpers, recovered from 0x00105750+. */
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* These backing buffers are fixed globals in the original image. */
static char basename_buf[0x400];
static char shortname_buf[0x400];
static char sram_path_buf[0x400];
static char state_path_buf[0x400];

extern void split_path_00105ae8(const char *src,
                                char *drive, char *dir,
                                char *name, char *ext);
extern const char *g_loaded_rom_path; /* logical source at VA 0x0035b328 */

char *build_sram_path_00105750(void)
{
    char drive[8] = {0};
    char dir[0x400] = {0};
    char ext[0x400] = {0};

    split_path_00105ae8(g_loaded_rom_path, drive, dir, basename_buf, ext);

    /* Original truncates/copies the base name to at most 0x1b bytes. */
    strncpy(shortname_buf, basename_buf, 0x1b);
    shortname_buf[0x1b] = '\0';

    strcpy(sram_path_buf, "mc0:SNES_EMU/");
    strcat(sram_path_buf, shortname_buf);
    strcat(sram_path_buf, ".SRM");
    return sram_path_buf;
}

char *build_state_path_001057fc(unsigned slot)
{
    char drive[8] = {0};
    char dir[0x400] = {0};
    char ext[0x400] = {0};

    split_path_00105ae8(g_loaded_rom_path, drive, dir, basename_buf, ext);
    strncpy(shortname_buf, basename_buf, 0x1b);
    shortname_buf[0x1b] = '\0';
    snprintf(state_path_buf, sizeof(state_path_buf),
             "mc0:SNES_EMU/%s.%03u", shortname_buf, slot);
    return state_path_buf;
}

/* Compatibility spellings retained for older research callers. */
char *build_sram_path_recovered(void)
{
    return build_sram_path_00105750();
}

char *build_state_path_recovered(unsigned slot)
{
    return build_state_path_001057fc(slot);
}
