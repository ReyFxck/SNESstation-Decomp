/*
 * Progress 11: small frontend / RPC / controller leaves recovered from
 * SNES Station v0.23 target assembly.
 *
 * Historical symbol names are not asserted here.  The address-labelled entry
 * names make it explicit that these are behavioral reconstructions, not
 * matching-source claims.
 */
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "gslib_recovered.h"

typedef void (*P11VoidCallback)(void *opaque);
typedef void (*P11FreeCallback)(void *ptr, void *opaque);

typedef struct P11ByteModeState {
    uint32_t value;
    uint8_t mode;
} P11ByteModeState;

void snes_p11_001742b4(P11ByteModeState *state, uint32_t value,
                       void (*apply)(uint32_t mode, void *opaque), void *opaque)
{
    state->value = value & UINT32_C(0xff);
    if (apply != NULL)
        apply((uint32_t)(state->mode & UINT8_C(0x0f)), opaque);
}

typedef struct P11ShortTransformState {
    int16_t input_x;
    int16_t input_y;
    int16_t output_x;
    int16_t output_y;
} P11ShortTransformState;

void snes_p11_0012c2a4(P11ShortTransformState *state,
                       void (*transform)(int16_t, int16_t, int16_t *, int16_t *, void *),
                       void *opaque)
{
    if (transform != NULL)
        transform(state->input_x, state->input_y,
                  &state->output_x, &state->output_y, opaque);
}

void snes_p11_001005b0(gsDriverRecovered *driver, uint32_t callback_id)
{
    /* 0x00101860 is an empty target leaf immediately before these calls. */
    gsDriver_RemoveVSyncCallback_00199420(driver, callback_id);
    gsDriver_DisableVSyncCallbacks_00199460(driver);
}

typedef struct P11OwnedObject {
    uint32_t vtable;
    void *owned;
} P11OwnedObject;

void snes_p11_00104358(P11OwnedObject *object, P11FreeCallback free_fn, void *opaque)
{
    object->vtable = UINT32_C(0x00426c28);
    if (object->owned != NULL && free_fn != NULL)
        free_fn(object->owned, opaque);
}

typedef int (*P11OpenCallback)(const char *path, unsigned mode, int *handle, void *opaque);
typedef void (*P11CloseCallback)(int handle, void *opaque);

int snes_p11_001711e8(const char *path, P11OpenCallback open_fn,
                      void (*process)(int handle, void *opaque),
                      P11CloseCallback close_fn, void *opaque)
{
    int handle = 0;

    if (open_fn == NULL || !open_fn(path, 0u, &handle, opaque))
        return 0;
    if (process != NULL)
        process(handle, opaque);
    if (close_fn != NULL)
        close_fn(handle, opaque);
    return 1;
}

typedef int (*P11ProcessResultCallback)(int handle, void *opaque);

int snes_p11_0017022c(const char *path, P11OpenCallback open_fn,
                      P11ProcessResultCallback process,
                      P11CloseCallback close_fn, void *opaque)
{
    int handle = 0;
    int result = 0;

    if (open_fn == NULL || !open_fn(path, 1u, &handle, opaque))
        return 0;
    if (process != NULL)
        result = process(handle, opaque);
    if (close_fn != NULL)
        close_fn(handle, opaque);
    return result == 1;
}

typedef int (*P11RpcCallback)(unsigned command, const void *send_buffer,
                              unsigned send_size, void *recv_buffer,
                              unsigned recv_size, void *opaque);

typedef struct P11RpcState {
    uint32_t initialized;
    uint8_t buffer[0x40];
    P11RpcCallback call;
    void *opaque;
} P11RpcState;

static int p11_rpc_empty(P11RpcState *state, unsigned command)
{
    if (state->initialized == 0u)
        return 0;
    if (state->call == NULL)
        return 0;
    return state->call(command, state->buffer, 0u, state->buffer, 0u,
                       state->opaque);
}

int snes_p11_0010768c(P11RpcState *state)
{
    return p11_rpc_empty(state, 3u);
}

int snes_p11_001076e4(P11RpcState *state)
{
    return p11_rpc_empty(state, 4u);
}

int snes_p11_0010773c(P11RpcState *state, uint32_t value)
{
    uint32_t masked = value & UINT32_C(0x3fff);

    if (state->initialized == 0u)
        return 0;
    state->buffer[0x14] = (uint8_t)masked;
    state->buffer[0x15] = (uint8_t)(masked >> 8);
    state->buffer[0x16] = (uint8_t)(masked >> 16);
    state->buffer[0x17] = (uint8_t)(masked >> 24);
    if (state->call == NULL)
        return 0;
    return state->call(5u, state->buffer, 0x40u, state->buffer, 0x40u,
                       state->opaque);
}

typedef struct P11RpcWordState {
    uint32_t initialized;
    uint32_t request;
    uint32_t response;
    P11RpcCallback call;
    void *opaque;
} P11RpcWordState;

static int p11_rpc_word(P11RpcWordState *state, uint32_t value)
{
    if (state->initialized == 0u)
        return -1;
    state->request = value;
    if (state->call != NULL)
        (void)state->call(1u, &state->request, 4u, &state->request, 8u,
                          state->opaque);
    return (int)state->response;
}

int snes_p11_00108bfc(P11RpcWordState *state, uint32_t value)
{
    return p11_rpc_word(state, value);
}

int snes_p11_00108ce4(P11RpcWordState *state, uint32_t value)
{
    return p11_rpc_word(state, value);
}

typedef struct P11MemoryCleanupPair {
    void *first;
    void *second;
} P11MemoryCleanupPair;

void snes_p11_00151360(P11MemoryCleanupPair *pair, P11FreeCallback free_fn,
                       void *opaque)
{
    if (pair->first != NULL) {
        if (free_fn != NULL)
            free_fn(pair->first, opaque);
        pair->first = NULL;
    }
    if (pair->second != NULL) {
        if (free_fn != NULL)
            free_fn(pair->second, opaque);
        pair->second = NULL;
    }
}

void snes_p11_00101b04(gsPipeRecovered *pipe)
{
    gsPipe_setAlphaEnable_00199b80(pipe, 0);
    gsPipe_RectFlat_0019adf8(pipe, 0, 0, 640, 480, 0,
                            UINT32_C(0x80000000));
    gsPipe_setAlphaEnable_00199b80(pipe, 1);
    gsPipe_Flush_001998f8(pipe);
}

typedef struct P11AudioConfig {
    uint32_t pad00[2];
    uint32_t rate;
    uint32_t arg;
    uint32_t pad10[2];
    uint32_t option;
    uint8_t valid;
} P11AudioConfig;

int snes_p11_00105cb8(P11AudioConfig *state, int mode, uint32_t option,
                       uint32_t arg)
{
    uint32_t rate = 0u;

    state->option = option & UINT32_C(0xff);
    state->valid = 1u;
    if (mode == 1)
        rate = 12000u;
    else if (mode == 2)
        rate = 24000u;
    else if (mode == 3)
        rate = 48000u;
    state->rate = rate;
    state->arg = arg;
    return 1;
}

typedef struct P11ThreePointers {
    void *first;
    void *second;
    void *third;
} P11ThreePointers;

void snes_p11_0010a8bc(P11ThreePointers *state, P11FreeCallback free_fn,
                       void *opaque)
{
    void **slots[3] = { &state->first, &state->second, &state->third };
    unsigned i;

    for (i = 0u; i < 3u; ++i) {
        if (*slots[i] != NULL) {
            if (free_fn != NULL)
                free_fn(*slots[i], opaque);
            *slots[i] = NULL;
        }
    }
}

typedef struct P11FloatPairState {
    int16_t input_x;
    int16_t input_y;
    int16_t output_x;
    int16_t output_y;
    float work_x;
    float work_y;
    float result_x;
    float result_y;
} P11FloatPairState;

void snes_p11_0012dde0(P11FloatPairState *state,
                       void (*transform)(P11FloatPairState *, void *),
                       void *opaque)
{
    state->work_x = (float)state->input_x;
    state->work_y = (float)state->input_y;
    if (transform != NULL)
        transform(state, opaque);
    state->output_x = (int16_t)lrintf(state->result_x);
    state->output_y = (int16_t)lrintf(state->result_y);
}

typedef struct P11ControllerMixState {
    uint8_t selector;
    uint8_t force_center;
    uint32_t preserve_axis;
    int16_t mixed_x;
    int16_t y;
    int32_t slot_value[2];
    int32_t slot_pair_value[2];
} P11ControllerMixState;

static int16_t p11_axis_mix(int16_t x, int16_t y)
{
    int32_t ax = x;
    int32_t ay = y;

    if (ax < 0)
        ax = -ax;
    if (ay < 0)
        ay = -ay;
    return (int16_t)((ax + ay) / 2);
}

void snes_p11_0017409c(P11ControllerMixState *state, int16_t x, int16_t y)
{
    uint32_t slot = (uint32_t)(state->selector & 1u);
    uint32_t other = slot ^ 1u;
    int16_t mixed = x;

    if (state->preserve_axis == 0u)
        mixed = p11_axis_mix(x, y);
    state->slot_pair_value[slot] = mixed;
    state->mixed_x = mixed;
    state->y = y;
    state->slot_pair_value[other] = y;
}

void snes_p11_00173ff0(P11ControllerMixState *state, int16_t x, int16_t y)
{
    uint32_t slot;
    uint32_t other;
    int16_t mixed = x;

    if (state->force_center != 0u) {
        state->mixed_x = 0x7f;
        state->y = 0x7f;
        state->slot_value[0] = 0x7f;
        state->slot_value[1] = 0x7f;
        return;
    }
    if (state->preserve_axis == 0u)
        mixed = p11_axis_mix(x, y);
    state->mixed_x = mixed;
    state->y = y;
    slot = (uint32_t)(state->selector & 1u);
    other = slot ^ 1u;
    state->slot_value[slot] = mixed;
    state->slot_value[other] = y;
}

typedef struct P11DisplayDefaults {
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
} P11DisplayDefaults;

void snes_p11_00106054(P11DisplayDefaults *state, int8_t region_byte)
{
    state->enable0 = 1u;
    state->enable1 = 1u;
    state->mode = 2u;
    state->zero3 = 0u;
    state->zero4 = 0u;
    state->zero5 = 0u;
    state->enable6 = 1u;
    if (region_byte == 0x45) {
        state->width_code = 0xaau;
        state->height_code = 0x50u;
    } else {
        state->width_code = 0x82u;
        state->height_code = 0x32u;
    }
    state->value10 = 0x3eu;
    state->value14 = 0x19u;
    state->value18 = 0x46u;
    state->value1c = 0x2du;
}

typedef struct P11ControllerTableState {
    uint32_t values[8];
    uint8_t neutral_only;
} P11ControllerTableState;

void snes_p11_001744d0(P11ControllerTableState *state, uint32_t index,
                       uint32_t value)
{
    unsigned i;
    uint8_t neutral = 0u;

    state->values[index & 7u] = value;
    if (state->values[0] == 0u || state->values[0] == 0x7fu) {
        neutral = 1u;
        for (i = 1u; i < 8u; ++i) {
            if (state->values[i] != 0u) {
                neutral = 0u;
                break;
            }
        }
    }
    state->neutral_only = neutral;
}
