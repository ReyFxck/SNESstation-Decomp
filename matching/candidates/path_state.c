/* Matching experiment for SNES Station 0x001057fc..0x00105898. */
#include <string.h>
#include <stdio.h>

extern char g_loaded_rom_path[];
extern char state_path_buf[];
extern char state_shortname_buf[];
extern char state_drive_buf[];
extern char state_dir_buf[];
extern char state_basename_buf[];
extern char state_ext_buf[];

extern void split_path_00105ae8(const char *src,
                                char *drive, char *dir,
                                char *name, char *ext);

char *build_state_path_candidate(int slot)
{
    split_path_00105ae8(g_loaded_rom_path,
                        state_drive_buf, state_dir_buf,
                        state_basename_buf, state_ext_buf);
    strncpy(state_shortname_buf, state_basename_buf, 0x1b);
    sprintf(state_path_buf, "mc0:SNES_EMU/%s.%03d",
            state_shortname_buf, slot);
    return state_path_buf;
}
