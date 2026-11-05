/*
 * Progress 15 -- complete behavioral reconstruction of padInit @ 0x001a8484.
 *
 * The stripped SNES Station v0.23 target is authoritative.  This host-safe
 * model preserves the two XPADMAN bind/retry corridors, the fixed delay window,
 * module-version query, 8-entry PadState clear, and the final command-0x10
 * SifCallRpc transaction.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define SNES_P15_PAD_BIND0 UINT32_C(0x80000100)
#define SNES_P15_PAD_BIND1 UINT32_C(0x80000101)
#define SNES_P15_PAD_INIT_CMD UINT32_C(0x10)
#define SNES_P15_PAD_BIND_SPINS UINT32_C(0x00100001)

typedef int (*SnesP15PadBindFn)(unsigned client, uint32_t rpc_id, void *opaque);
typedef int (*SnesP15PadIsBoundFn)(unsigned client, void *opaque);
typedef void (*SnesP15PadDelayFn)(uint32_t iterations, void *opaque);
typedef int (*SnesP15PadGetModVersionFn)(void *opaque);
typedef int (*SnesP15PadCallFn)(
    unsigned client, unsigned function, unsigned mode,
    void *send, unsigned send_size, void *recv, unsigned recv_size,
    void *end_function, void *end_parameter, void *opaque);

/*
 * The target loop clears six words inside each 0x28-byte state record:
 * +0x00, +0x04, +0x08, +0x14, +0x18 and +0x1c.
 * Unknown/reserved words are intentionally retained.
 */
typedef struct SnesP15PadState {
    uint32_t word00;
    uint32_t word04;
    uint32_t word08;
    uint32_t word0c;
    uint32_t word10;
    uint32_t word14;
    uint32_t word18;
    uint32_t word1c;
    uint32_t word20;
    uint32_t word24;
} SnesP15PadState;

typedef char snes_p15_pad_state_size[
    (sizeof(SnesP15PadState) == 0x28) ? 1 : -1];

typedef struct SnesP15PadInitRuntime {
    int initialised;
    uint8_t rpc_buffer[128];
    SnesP15PadState state[8];
    SnesP15PadBindFn bind;
    SnesP15PadIsBoundFn is_bound;
    SnesP15PadDelayFn delay;
    SnesP15PadGetModVersionFn get_mod_version;
    SnesP15PadCallFn call;
    void *opaque;
} SnesP15PadInitRuntime;

static void store32(uint8_t *p, uint32_t value)
{
    memcpy(p, &value, sizeof(value));
}

static int bind_until_ready(SnesP15PadInitRuntime *rt, unsigned client,
                            uint32_t rpc_id, int failure)
{
    for (;;) {
        if (rt->bind == NULL || rt->bind(client, rpc_id, rt->opaque) < 0)
            return failure;

        /*
         * 0x001a84cc..0x001a8500 and 0x001a852c..0x001a8560:
         * the target counts from 0x000fffff down through -2, i.e. 0x100001
         * decrement/branch iterations, before reading the bound-server word.
         */
        if (rt->delay != NULL)
            rt->delay(SNES_P15_PAD_BIND_SPINS, rt->opaque);

        if (rt->is_bound == NULL || rt->is_bound(client, rt->opaque))
            return 0;
    }
}

/* 0x001a8484..0x001a8610 */
int snes_p15_padInit_001a8484(SnesP15PadInitRuntime *rt, int ignored)
{
    unsigned i;
    int rc;
    (void)ignored;

    if (rt == NULL)
        return -1;

    if (rt->initialised)
        return 0;

    rc = bind_until_ready(rt, 0, SNES_P15_PAD_BIND0, -1);
    if (rc < 0)
        return rc;

    rc = bind_until_ready(rt, 1, SNES_P15_PAD_BIND1, -3);
    if (rc < 0)
        return rc;

    /* 0x001a8570: return value is deliberately ignored by the target. */
    if (rt->get_mod_version != NULL)
        (void)rt->get_mod_version(rt->opaque);

    for (i = 0; i < 8; ++i) {
        rt->state[i].word1c = 0;
        rt->state[i].word00 = 0;
        rt->state[i].word04 = 0;
        rt->state[i].word08 = 0;
        rt->state[i].word14 = 0;
        rt->state[i].word18 = 0;
    }

    store32(rt->rpc_buffer, SNES_P15_PAD_INIT_CMD);

    if (rt->call == NULL)
        return -1;

    /*
     * 0x001a85b4..0x001a85ec:
     * SifCallRpc(client0, 1, 0, buf, 0x80, buf, 0x80, NULL, NULL).
     */
    rc = rt->call(0, 1, 0,
                  rt->rpc_buffer, 0x80,
                  rt->rpc_buffer, 0x80,
                  NULL, NULL, rt->opaque);
    if (rc < 0)
        return -1;

    rt->initialised = 1;
    return 0;
}
