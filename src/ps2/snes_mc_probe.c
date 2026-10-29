/*
 * SNES Station v0.23 - Memory Card Probing
 * Canonical source from progress12_io_state_recovered.c @ 0x00106824.
 */

#include <stdint.h>

typedef int (*SnesMcInitCallback)(int type, void *opaque);
typedef int (*SnesMcGetInfoCallback)(int port, int slot, int *type,
                                     int *free_space, int *formatted,
                                     void *opaque);
typedef int (*SnesMcSyncCallback)(int mode, int *command, int *result,
                                  void *opaque);

typedef struct SnesMcProbeState {
    uint32_t initialized;
} SnesMcProbeState;

/* 0x00106824: lazy mcInit, mcGetInfo(0,0), mcSync(0,0,..), type == 2. */
int snes_mc_probe(SnesMcProbeState *state, SnesMcInitCallback init_fn,
                  SnesMcGetInfoCallback info_fn, SnesMcSyncCallback sync_fn,
                  void *opaque)
{
    int type = 0;
    int free_space = 0;
    int formatted = 0;
    int sync_result = 0;

    if (state->initialized != 1u) {
        if (init_fn != NULL)
            (void)init_fn(0, opaque);
        state->initialized = 1u;
    }
    if (info_fn != NULL)
        (void)info_fn(0, 0, &type, &free_space, &formatted, opaque);
    if (sync_fn != NULL)
        (void)sync_fn(0, NULL, &sync_result, opaque);
    return type == 2;
}