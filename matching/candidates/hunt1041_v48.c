/*
 * Small direct-ABI candidates for the final SNES Station matching pass.
 *
 * The older behavioral models expose globals and fixed calls as parameters so
 * they can be exercised on a host.  These candidates preserve the target's
 * actual EE calling convention and global accesses for strict comparison.
 */
#include <stdint.h>
#include <string.h>

typedef struct V48AudioConfig {
    uint32_t pad00[2];
    uint32_t rate;
    uint32_t arg;
    uint32_t pad10[2];
    uint32_t option;
    uint8_t valid;
} V48AudioConfig;

typedef struct V48DisplayDefaults {
    uint8_t enable0;
    uint8_t enable1;
    uint8_t mode;
    uint8_t zero3;
    uint8_t zero4;
    uint8_t zero5;
    uint8_t enable6;
    uint8_t pad07;
    uint32_t width_code;
    uint32_t height_code;
    uint32_t value10;
    uint32_t value14;
    uint32_t value18;
    uint32_t value1c;
} V48DisplayDefaults;

extern void v48_empty_vsync_leaf(void);
extern void *v48_frontend_driver;
extern uint32_t v48_vsync_callback_id;
extern void gsDriver_RemoveVSyncCallback_00199420(void *driver,
                                                  uint32_t callback_id);
extern void gsDriver_DisableVSyncCallbacks_00199460(void *driver);

extern volatile V48AudioConfig v48_audio_config;
extern V48DisplayDefaults v48_display_defaults;
extern uint8_t v48_srtc_state[0x1c];
extern uint16_t v48_renderer_color_table[8u * 256u];
extern uint8_t v48_renderer_color_table_ready;
extern char v48_loaded_rom_path[];
extern void split_path_00105ae8(const char *src, char *drive, char *dir,
                                char *name, char *ext);
extern void _makepath(char *path, const char *drive, const char *dir,
                      const char *name, const char *ext);
extern int32_t v48_audio_module_size;
extern const uint8_t v48_audio_module[];
extern void v48_audio_set_volume(uint32_t value);
extern void v48_audio_quit(void);
extern void v48_audio_module_init(uint32_t value);
extern void v48_audio_module_prepare(void);
extern void v48_audio_module_load(const void *module, uint32_t size);
extern void v48_audio_module_play(uint32_t enabled);
extern void v48_audio_module_volume(uint32_t value);
extern int sprintf(char *buffer, const char *format, ...);
extern int v48_strlen_int(const char *text) __asm__("strlen");
extern int v48_gzwrite(void *handle, const void *data, uint32_t size);

#define V48_ROM_REGION (*(const int8_t *)(uintptr_t)UINT32_C(0x1fc7ff52))

/* Target entry 0x001005b0. */
void v48_001005b0(void)
{
    v48_empty_vsync_leaf();
    gsDriver_RemoveVSyncCallback_00199420(v48_frontend_driver,
                                          v48_vsync_callback_id);
    gsDriver_DisableVSyncCallbacks_00199460(v48_frontend_driver);
}

/* Target entry 0x00105cb8. */
int v48_00105cb8(int mode, uint32_t option, uint32_t arg)
{
    v48_audio_config.option = option & UINT32_C(0xff);
    v48_audio_config.valid = 1;
    switch (mode) {
    case 1:
        v48_audio_config.rate = 12000;
        break;
    case 2:
        v48_audio_config.rate = 24000;
        break;
    case 3:
        v48_audio_config.rate = 48000;
        break;
    default:
        v48_audio_config.rate = 0;
        break;
    }
    v48_audio_config.arg = arg;
    return 1;
}

/* Target entry 0x00106054. */
void v48_00106054(void)
{
    v48_display_defaults.enable0 = 1;
    v48_display_defaults.mode = 2;
    v48_display_defaults.enable1 = 1;
    v48_display_defaults.enable6 = 1;
    v48_display_defaults.zero3 = 0;
    v48_display_defaults.zero4 = 0;
    v48_display_defaults.zero5 = 0;
    if (V48_ROM_REGION == 0x45) {
        v48_display_defaults.width_code = 0xaa;
        v48_display_defaults.height_code = 0x50;
    } else {
        v48_display_defaults.width_code = 0x82;
        v48_display_defaults.height_code = 0x32;
    }
    v48_display_defaults.value10 = 0x3e;
    v48_display_defaults.value14 = 0x19;
    v48_display_defaults.value18 = 0x46;
    v48_display_defaults.value1c = 0x2d;
}

/* Target entry 0x00183678. */
void v48_00183678(void)
{
    memset(v48_srtc_state, 0, 0x1c);
    v48_srtc_state[0] = 1;
    v48_srtc_state[0x0f] = 0xff;
    v48_srtc_state[0x10] = 0;
    v48_srtc_state[1] = 0;
    *(uint32_t *)(v48_srtc_state + 0x14) = 0;
}

/* Target entry 0x0014308c. */
void v48_0014308c(void)
{
    uint32_t group;

    for (group = 0; group < 8; ++group) {
        uint32_t value;

        for (value = 0; value < 256; ++value) {
            uint32_t index = (group << 8) + value;
            uint32_t part_a = ((value & 0xc0) >> 3) | (group & 4);
            uint32_t part_b = ((value & 0x38) >> 1) | (group & 2);
            uint32_t part_c = ((value & 7) << 2) | ((group & 1) << 1);

            v48_renderer_color_table[index] =
                (uint16_t)((part_a << 10) | (part_b << 5) | part_c);
        }
    }
    v48_renderer_color_table_ready = 0;
}

typedef struct V48PathScratch {
    char drive[0x10];
    char dir[0x410];
    char name[0x410];
    char ext[0x410];
    char output[0x410];
} V48PathScratch;

/* Target entry 0x00101924. */
char *v48_00101924(const char *suffix)
{
    V48PathScratch scratch;

    split_path_00105ae8(v48_loaded_rom_path, scratch.drive, scratch.dir,
                        scratch.name, scratch.ext);
    _makepath(scratch.output, scratch.drive, scratch.dir, scratch.name, suffix);
    return scratch.output;
}

/* Target entry 0x00105d78. */
void v48_00105d78(void)
{
    v48_audio_set_volume(0);
    v48_audio_quit();
    if ((v48_audio_module_size & 15) != 0)
        v48_audio_module_size =
            (v48_audio_module_size / 16) * 16 + 16;
    v48_audio_module_init(0);
    v48_audio_module_prepare();
    v48_audio_module_load(v48_audio_module,
                          (uint32_t)v48_audio_module_size);
    v48_audio_module_play(1);
    v48_audio_module_volume(0x3fff);
}

/* Target entry 0x00172174. */
void v48_00172174(void *handle, const char *label, const void *payload,
                  uint32_t payload_size)
{
    char header[0x200];

    sprintf(header, "%s:%06d:", label, payload_size);
    v48_gzwrite(handle, header, (uint32_t)v48_strlen_int(header));
    v48_gzwrite(handle, payload, payload_size);
}
