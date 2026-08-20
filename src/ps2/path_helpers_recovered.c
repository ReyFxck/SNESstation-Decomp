/* SNES Station v0.23 PS2 save-path helpers, recovered from 0x00105750+. */
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* The target's pre-C99 PS2 headers declared strlen with an int result. */
extern int target_strlen_int(const char *text) __asm__("strlen");

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
static char sram_path_buf[0x400] __attribute__((aligned(8)));
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

static const char sram_path_prefix[] __attribute__((aligned(8))) =
    "mc0:SNES_EMU/";

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
    __builtin_memcpy(sram_path_buf, sram_path_prefix,
                     sizeof(sram_path_prefix));
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

/*
 * 0x001059cc: PS2 device-aware _makepath recovered from the frontend path corridor.
 * Unlike the desktop Snes9x helper, drive is a complete device token such as
 * "mc0" or "cdfs", not a single drive letter.
 */
void _makepath(char *path, const char *drive, const char *dir,
               const char *name, const char *ext)
{
    if (drive != NULL && *drive != '\0') {
        int drive_len;

        strcpy(path, drive);
        drive_len = target_strlen_int(drive);
        path[drive_len] = ':';
        path[drive_len + 1] = '\0';
    } else {
        *path = '\0';
    }

    if (dir != NULL && *dir != '\0') {
        strcat(path, dir);
        if (target_strlen_int(dir) != 1 || *dir != '/')
            strcat(path, "/");
    }

    strcat(path, name);

    if (ext != NULL && *ext != '\0') {
        strcat(path, ".");
        strcat(path, ext);
    }
}

/*
 * 0x00105ae8: PS2 device-aware _splitpath recovered from the same corridor.
 * Observed callers use a device prefix ("device:path").  The no-colon branch
 * below is a safe source-level fallback; all target-observed device paths take
 * the colon branch.
 */
void _splitpath(const char *path, char *drive, char *dir,
                char *name, char *ext)
{
    char scratch[0x400];
    char *colon;
    char *tail;
    char *slash;
    char *dot;

    strcpy(scratch, path);
    colon = strchr(scratch, ':');
    if (colon != NULL) {
        *colon = '\0';
        strcpy(drive, scratch);
        tail = colon + 1;
    } else {
        *drive = '\0';
        tail = scratch;
    }

    slash = strrchr(tail, '/');
    if (slash == NULL)
        slash = strrchr(tail, '/'); /* duplicate lookup exists in the target */

    dot = strrchr(tail, '.');
    if (dot != NULL && slash != NULL && dot < slash)
        dot = NULL;

    if (slash != NULL) {
        if (*drive != '\0' && *tail != '/') {
            strcpy(dir, "/");
            strcat(dir, tail);
            dir[(size_t)(slash - tail) + 1] = '\0';
        } else {
            strcpy(dir, tail);
            if (slash != tail)
                dir[slash - tail] = '\0';
            else
                dir[1] = '\0';
        }

        strcpy(name, slash + 1);
        if (dot != NULL) {
            name[dot - slash - 1] = '\0';
            strcpy(ext, dot + 1);
        } else {
            *ext = '\0';
        }
    } else {
        if (*drive != '\0')
            strcpy(dir, "/");
        else
            *dir = '\0';

        strcpy(name, tail);
        if (dot != NULL) {
            name[dot - tail] = '\0';
            strcpy(ext, dot + 1);
        } else {
            *ext = '\0';
        }
    }
}

/* Keep the address-labelled spelling used by the already-recovered callers. */
void split_path_00105ae8(const char *src,
                         char *drive, char *dir,
                         char *name, char *ext)
{
    _splitpath(src, drive, dir, name, ext);
}

/* Snes9x 1.x uses 200 for AUTO_FRAMERATE. */
#define SNES_AUTO_FRAMERATE 200u

/*
 * Narrow view of the target IPPU object used by the adjacent frontend frame
 * sync callback.  Final project-wide type ownership remains a later gate.
 */
typedef struct S9xFrameSyncIPPUView {
    uint8_t  reserved_00[6];
    uint8_t  RenderThisFrame;
    uint8_t  reserved_07[13];
    uint32_t SkippedFrames;
    uint32_t FrameSkip;
} S9xFrameSyncIPPUView;

/* Logical bindings for fixed globals used by the original callback. */
extern uint32_t Settings_SkipFrames;
extern int32_t  Memory_ROMFramesPerSecond;
extern S9xFrameSyncIPPUView IPPU_FrameSync;

extern int64_t  S9xSync_PeriodCounter;
extern uint32_t S9xSync_PeriodPrevious;
extern uint32_t S9xSync_PeriodValue;
extern uint32_t S9xSync_AutoFrameLatch;

extern uint8_t  S9xSync_AudioActive;
extern uint32_t S9xSync_AudioArg0;
extern uint32_t S9xSync_AudioArg1;

/* Address-bound helpers remain opaque until their owning modules are typed. */
extern void S9xSync_AudioStep(uintptr_t state, uint32_t arg);
extern void S9xSync_SetVolume(uintptr_t state, uintptr_t work,
                             uint32_t arg, uint32_t immediate);

/*
 * 0x00105898: strong semantic identification as S9xSyncSpeed from the Snes9x callback
 * contract: SkipFrames/AUTO_FRAMERATE drives IPPU.RenderThisFrame and
 * IPPU.SkippedFrames.  This is build-ready behavioral source, not yet a
 * machine-code matching claim.
 */
void S9xSyncSpeed(void)
{
    uint32_t previous = S9xSync_PeriodValue;

    if (Settings_SkipFrames == SNES_AUTO_FRAMERATE) {
        if (S9xSync_AutoFrameLatch == 1) {
            S9xSync_AutoFrameLatch = 0;
            IPPU_FrameSync.RenderThisFrame = 0;
            IPPU_FrameSync.SkippedFrames++;
        } else {
            IPPU_FrameSync.RenderThisFrame = 1;
            IPPU_FrameSync.SkippedFrames = 0;
        }
    } else {
        if (S9xSync_PeriodCounter >= (int64_t)Memory_ROMFramesPerSecond) {
            S9xSync_PeriodCounter = 0;
            S9xSync_PeriodValue = 0;
            S9xSync_PeriodPrevious = previous;
        }

        IPPU_FrameSync.FrameSkip++;
        if (IPPU_FrameSync.FrameSkip < Settings_SkipFrames) {
            IPPU_FrameSync.RenderThisFrame = 0;
            IPPU_FrameSync.SkippedFrames++;
        } else {
            IPPU_FrameSync.SkippedFrames = 0;
            IPPU_FrameSync.RenderThisFrame = 1;
            IPPU_FrameSync.FrameSkip = 0;
        }
    }

    if (S9xSync_AudioActive == 1) {
        S9xSync_AudioStep((uintptr_t)0x001eab80u, S9xSync_AudioArg0);
        S9xSync_SetVolume((uintptr_t)0x001bbd80u,
                          (uintptr_t)0x001d3480u,
                          S9xSync_AudioArg1, 1);
    }
}
