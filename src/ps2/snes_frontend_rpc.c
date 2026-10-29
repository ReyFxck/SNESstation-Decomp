/*
 * SNES Station v0.23 PS2 - Frontend RPC State Machines
 * Canonical source from progress11_frontend.c @ 0x0010768c, 0x001076e4, 0x0010773c,
 * 0x00108bfc, 0x00108ce4
 */

#include <stdint.h>

typedef int (*SnesFrontendRpcCallback)(unsigned command, const void *send_buffer,
                                      unsigned send_size, void *recv_buffer,
                                      unsigned recv_size, void *opaque);

typedef struct {
    uint32_t initialized;
    uint8_t buffer[0x40];
    SnesFrontendRpcCallback call;
    void *opaque;
} SnesFrontendRpcState;

static int snes_frontend_rpc_empty(SnesFrontendRpcState *state, unsigned command)
{
    if (state->initialized == 0u)
        return 0;
    if (state->call == NULL)
        return 0;
    return state->call(command, state->buffer, 0u, state->buffer, 0u,
                       state->opaque);
}

int snes_frontend_rpc_cmd3_0010768c(SnesFrontendRpcState *state)
{
    return snes_frontend_rpc_empty(state, 3u);
}

int snes_frontend_rpc_cmd4_001076e4(SnesFrontendRpcState *state)
{
    return snes_frontend_rpc_empty(state, 4u);
}

int snes_frontend_rpc_cmd5_0010773c(SnesFrontendRpcState *state, uint32_t value)
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

typedef struct {
    uint32_t initialized;
    uint32_t request;
    uint32_t response;
    SnesFrontendRpcCallback call;
    void *opaque;
} SnesFrontendRpcWordState;

static int snes_frontend_rpc_word(SnesFrontendRpcWordState *state, uint32_t value)
{
    if (state->initialized == 0u)
        return -1;
    state->request = value;
    if (state->call != NULL)
        (void)state->call(1u, &state->request, 4u, &state->request, 8u,
                          state->opaque);
    return (int)state->response;
}

int snes_frontend_rpc_word_00108bfc(SnesFrontendRpcWordState *state, uint32_t value)
{
    return snes_frontend_rpc_word(state, value);
}

int snes_frontend_rpc_word_00108ce4(SnesFrontendRpcWordState *state, uint32_t value)
{
    return snes_frontend_rpc_word(state, value);
}