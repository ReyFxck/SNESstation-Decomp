/*
 * HUNT1000+ V47 closure candidates.
 *
 * These small frontend/libc bodies use direct historical globals and APIs.
 * That removes the opaque callback shims used by the first behavioural
 * recovery pass and preserves the original EE ABI/code-generation shape.
 */
typedef unsigned long size_t;
typedef unsigned long long uint64_t;

extern int fioWrite(int fd, void *buffer, int size);
extern void *gzopen(const char *path, const char *mode);
extern int CDVD_getdir(const char *path, const char *extensions, int mode,
                       void *entries, unsigned int count, char *new_path);
extern int sprintf(char *buffer, const char *format, ...);
extern void *memset(void *buffer, int value, size_t size);

extern void *snes_records_00427010;
extern char snes_snapshot_path_004392c0[];
extern int snes_mode_001fbaec;
extern char snes_memory_0035e2b0[];
extern void *build_sram_path(void);
extern void CMemory_SaveSRAM(void *memory, void *path);
extern void audio_call_a(unsigned int value);
extern void audio_call_b(void);
extern void audio_call_c(unsigned int value);
extern void audio_call_d(unsigned int value);

extern void *snes_pipe_001bb2c0;
extern void gs_set_alpha(void *pipe, int enable);
extern void gs_rect_flat(void *pipe, int x1, int y1, int x2, int y2,
                         unsigned int z, unsigned int color);
extern void gs_flush(void *pipe);
extern char snes_cdfs_fixed_001cb340[];

extern unsigned int snes_control_001bb748;
extern unsigned int snes_mc_initialized_001fc1c8;
extern int mcInit(int type);
extern int mcGetInfo(int port, int slot, int *type, int *free_space,
                     int *formatted);
extern int mcSync(int mode, int *command, int *result);
extern int padGetState(int port, int slot);
extern int padSetMainMode(int port, int slot, int mode, int lock);

extern uint64_t snes_rand_state;

int v47_puts_like(const char *text)
{
    const char *cursor = text;
    int length = 0;
    while (*cursor++)
        length++;
    fioWrite(1, (void *)text, length);
    return length;
}

int v47_snes_rand(void)
{
    snes_rand_state = snes_rand_state * 6364136223846793005ULL + 1;
    return (int)((snes_rand_state >> 32) & 0x7fffffff);
}

void v47_leaf_control(unsigned int selector, unsigned int command)
{
    if (command == 0xffff && selector == 1)
        snes_control_001bb748 = 0;
}

void v47_gs_clear(void)
{
    gs_set_alpha(snes_pipe_001bb2c0, 0);
    gs_rect_flat(snes_pipe_001bb2c0, 0, 0, 640, 480, 0, 0x80000000u);
    gs_set_alpha(snes_pipe_001bb2c0, 1);
    gs_flush(snes_pipe_001bb2c0);
}

int v47_cdvd_getdir(char *path)
{
    return CDVD_getdir(path, ".ZIP .SMC .SFC .SWC .FIG .058 .BIN", 3,
                       snes_records_00427010, 4000, path);
}

int v47_record_is_file(unsigned int index)
{
    const unsigned char *records = (const unsigned char *)snes_records_00427010;
    unsigned int value = records[index * 0x90 - 0x88] & 2;
    if (value)
        return 1;
    return 0;
}

int v47_cdfs_path(unsigned int index, char *destination)
{
    const char *records = (const char *)snes_records_00427010;
    return sprintf(destination, "cdfs:%s/%s", snes_cdfs_fixed_001cb340,
                   records + index * 0x90 - 0x84);
}

int v47_pad_wait(int port, int slot)
{
    int state;
    do {
        state = padGetState(port, slot);
    } while (state != 0 && state != 6 && state != 2);
    return state;
}

void v47_pad_open(int port, int slot)
{
    if (v47_pad_wait(port, slot) == 6) {
        padSetMainMode(port, slot, 1, 3);
        v47_pad_wait(port, slot);
    }
}

char *v47_snapshot_path(void)
{
    sprintf(snes_snapshot_path_004392c0, "cdrom0:\\ROMS\\SNAP");
    return snes_snapshot_path_004392c0;
}

void v47_save_sram(void)
{
    if (snes_mode_001fbaec == 1)
        CMemory_SaveSRAM(snes_memory_0035e2b0, build_sram_path());
}

void v47_audio_shutdown(void)
{
    audio_call_a(0);
    audio_call_b();
    audio_call_c(0);
    audio_call_d(0x3fff);
}

int v47_mc_probe(void)
{
    int type;
    int free_space;
    int formatted;
    int result;
    if (snes_mc_initialized_001fc1c8 != 1) {
        mcInit(0);
        snes_mc_initialized_001fc1c8 = 1;
    }
    mcGetInfo(0, 0, &type, &free_space, &formatted);
    mcSync(0, 0, &result);
    return type == 2;
}
