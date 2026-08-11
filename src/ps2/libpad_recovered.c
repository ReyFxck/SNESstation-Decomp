/*
 * Behavioral reconstruction of the NEW/XPADMAN libpad corridor embedded in
 * SNES Station v0.23 (0x001a8420..0x001a90f7).
 *
 * This is an independently written target model.  It keeps the old asynchronous
 * RPC protocol and double-buffered 128-byte pad records visible without
 * depending on a host PS2SDK installation.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define PAD_RPCCMD_OPEN         0x01U
#define PAD_RPCCMD_SET_MMODE    0x06U
#define PAD_RPCCMD_SET_ACTDIR   0x07U
#define PAD_RPCCMD_SET_ACTALIGN 0x08U
#define PAD_RPCCMD_GET_BTNMASK  0x09U
#define PAD_RPCCMD_SET_BTNINFO  0x0aU
#define PAD_RPCCMD_GET_PORTMAX  0x0cU
#define PAD_RPCCMD_GET_SLOTMAX  0x0dU
#define PAD_RPCCMD_CLOSE        0x0eU
#define PAD_RPCCMD_END          0x0fU
#define PAD_RPCCMD_GET_MODVER   0x12U
#define PAD_RPCCMD_GET_CONNECT  0x11U

#define PAD_STATE_EXECCMD 5U
#define PAD_STATE_STABLE  6U
#define PAD_RSTAT_BUSY    2U

#define PAD_MODECURID   1
#define PAD_MODECUREXID 2
#define PAD_MODECUROFFS 3
#define PAD_MODETABLE   4

/* NEW_PADMAN DMA record: exact 128-byte layout used by the target family. */
typedef struct SnesPadData {
    uint8_t data[32];
    uint32_t unknown32;
    uint32_t unknown36;
    uint32_t unknown40;
    uint32_t unknown44;
    uint8_t actData[32];
    uint16_t modeTable[4];
    uint32_t frame;
    uint32_t unknown92;
    uint32_t length;
    uint8_t modeOk;
    uint8_t modeCurId;
    uint8_t unknown102;
    uint8_t unknown103;
    uint8_t nrOfModes;
    uint8_t modeCurOffs;
    uint8_t nrOfActuators;
    uint8_t unknown107[5];
    uint8_t state;
    uint8_t reqState;
    uint8_t ok;
    uint8_t unknown115[13];
} SnesPadData;

typedef struct SnesPadState {
    uint32_t open;
    uint32_t port;
    uint32_t slot;
    SnesPadData *padData;
    uint8_t *padBuf;
} SnesPadState;

typedef int (*SnesPadRpcCall)(unsigned client, uint8_t buffer[128], void *opaque);
typedef int (*SnesPadRpcBind)(unsigned client, uint32_t rpc_id, void *opaque);
typedef void (*SnesPadSyncDCache)(void *start, void *end, void *opaque);

typedef struct SnesPadRuntime {
    int initialised;
    uint8_t buffer[128];
    SnesPadState state[2][8];
    SnesPadRpcCall rpc_call;
    SnesPadRpcBind rpc_bind;
    SnesPadSyncDCache sync_dcache;
    void *opaque;
} SnesPadRuntime;

static uint32_t load32(const uint8_t *p)
{
    uint32_t v;
    memcpy(&v, p, sizeof(v));
    return v;
}

static void store32(uint8_t *p, uint32_t v)
{
    memcpy(p, &v, sizeof(v));
}

/* 0x001a8420 */
static SnesPadData *padGetDmaStr(SnesPadRuntime *rt, int port, int slot)
{
    SnesPadData *pdata = rt->state[port][slot].padData;
    if (rt->sync_dcache != NULL)
        rt->sync_dcache(pdata, (uint8_t *)pdata + 256, rt->opaque);
    return pdata[0].frame < pdata[1].frame ? &pdata[1] : &pdata[0];
}

static int rpc(SnesPadRuntime *rt, unsigned client)
{
    if (rt->rpc_call == NULL)
        return -1;
    return rt->rpc_call(client, rt->buffer, rt->opaque);
}

/* 0x001a8484 — target revision binds XPADMAN directly and has no reboot-count reset prelude. */
int snes_padInit(SnesPadRuntime *rt, int ignored)
{
    unsigned i;
    (void)ignored;

    if (rt->initialised)
        return 0;
    if (rt->rpc_bind == NULL)
        return -1;

    if (rt->rpc_bind(0, UINT32_C(0x80000100), rt->opaque) < 0)
        return -1;
    if (rt->rpc_bind(1, UINT32_C(0x80000101), rt->opaque) < 0)
        return -3;

    for (i = 0; i < 8; ++i) {
        memset(&rt->state[0][i], 0, sizeof(rt->state[0][i]));
        memset(&rt->state[1][i], 0, sizeof(rt->state[1][i]));
        rt->state[0][i].slot = i;
        rt->state[1][i].slot = i;
        rt->state[1][i].port = 1;
    }

    /* The NEW_PADMAN target then issues the module/version/init path through RPC. */
    rt->initialised = 1;
    return 0;
}

/* 0x001a8614 */
int snes_padEnd(SnesPadRuntime *rt)
{
    int result;
    store32(rt->buffer + 0, PAD_RPCCMD_END);
    if (rpc(rt, 0) < 0)
        return -1;
    result = (int32_t)load32(rt->buffer + 12);
    if (result == 1)
        rt->initialised = 0;
    return result;
}

/* 0x001a8690 */
int snes_padPortOpen(SnesPadRuntime *rt, int port, int slot, void *padArea)
{
    SnesPadData *dma = (SnesPadData *)padArea;
    unsigned i;

    if (((uintptr_t)padArea & 0x3fU) != 0)
        return 0;

    for (i = 0; i < 2; ++i) {
        memset(dma[i].data, 0xff, sizeof(dma[i].data));
        dma[i].frame = 0;
        dma[i].length = 0;
        dma[i].state = PAD_STATE_EXECCMD;
        dma[i].reqState = PAD_RSTAT_BUSY;
        dma[i].ok = 0;
        dma[i].unknown103 = 0;
    }

    store32(rt->buffer + 0, PAD_RPCCMD_OPEN);
    store32(rt->buffer + 4, (uint32_t)port);
    store32(rt->buffer + 8, (uint32_t)slot);
    store32(rt->buffer + 16, (uint32_t)(uintptr_t)padArea);
    if (rpc(rt, 0) < 0)
        return 0;

    rt->state[port][slot].open = 1;
    rt->state[port][slot].port = (uint32_t)port;
    rt->state[port][slot].slot = (uint32_t)slot;
    rt->state[port][slot].padData = dma;
    rt->state[port][slot].padBuf = (uint8_t *)(uintptr_t)load32(rt->buffer + 20);
    return (int32_t)load32(rt->buffer + 12);
}

/* 0x001a87b0 */
int snes_padPortClose(SnesPadRuntime *rt, int port, int slot)
{
    int call_result;
    store32(rt->buffer + 0, PAD_RPCCMD_CLOSE);
    store32(rt->buffer + 4, (uint32_t)port);
    store32(rt->buffer + 8, (uint32_t)slot);
    store32(rt->buffer + 16, 1);
    call_result = rpc(rt, 0);
    if (call_result < 0)
        return call_result;
    rt->state[port][slot].open = 0;
    return (int32_t)load32(rt->buffer + 12);
}

/* 0x001a8864 */
uint8_t snes_padRead(SnesPadRuntime *rt, int port, int slot, void *out)
{
    SnesPadData *p = padGetDmaStr(rt, port, slot);
    memcpy(out, p->data, p->length);
    return (uint8_t)p->length;
}

/* 0x001a8918 */
uint8_t snes_padGetReqState(SnesPadRuntime *rt, int port, int slot)
{
    return padGetDmaStr(rt, port, slot)->reqState;
}

/* 0x001a8938 */
int snes_padSetReqState(SnesPadRuntime *rt, int port, int slot, int state)
{
    padGetDmaStr(rt, port, slot)->reqState = (uint8_t)state;
    return 1;
}

/* 0x001a88a8 */
int snes_padGetState(SnesPadRuntime *rt, int port, int slot)
{
    SnesPadData *p = padGetDmaStr(rt, port, slot);
    if (p->state == PAD_STATE_STABLE && snes_padGetReqState(rt, port, slot) == PAD_RSTAT_BUSY)
        return PAD_STATE_EXECCMD;
    return p->state;
}

/* 0x001a896c / 0x001a89ac */
void snes_padStateInt2String(int state, char out[16])
{
    static const char names[8][16] = {
        "DISCONNECT", "FINDPAD", "FINDCTP1", "", "", "EXECCMD", "STABLE", "ERROR"
    };
    if ((unsigned)state < 8U)
        strcpy(out, names[state]);
}

void snes_padReqStateInt2String(int state, char out[16])
{
    static const char names[3][16] = {"COMPLETE", "FAILED", "BUSY"};
    if ((unsigned)state < 3U)
        strcpy(out, names[state]);
}

static int simple_rpc_result(SnesPadRuntime *rt, uint32_t command)
{
    store32(rt->buffer + 0, command);
    if (rpc(rt, 0) < 0)
        return -1;
    return (int32_t)load32(rt->buffer + 12);
}

/* 0x001a89e4 */
int snes_padGetPortMax(SnesPadRuntime *rt)
{
    return simple_rpc_result(rt, PAD_RPCCMD_GET_PORTMAX);
}

/* 0x001a8a4c */
int snes_padGetSlotMax(SnesPadRuntime *rt, int port)
{
    store32(rt->buffer + 0, PAD_RPCCMD_GET_SLOTMAX);
    store32(rt->buffer + 4, (uint32_t)port);
    if (rpc(rt, 0) < 0)
        return -1;
    return (int32_t)load32(rt->buffer + 12);
}

/* 0x001a8abc */
int snes_padGetModVersion(SnesPadRuntime *rt)
{
    return simple_rpc_result(rt, PAD_RPCCMD_GET_MODVER);
}

/* 0x001a8b24 — NEW_PADMAN path reads the shared DMA record directly. */
int snes_padInfoMode(SnesPadRuntime *rt, int port, int slot, int infoMode, int index)
{
    SnesPadData *p = padGetDmaStr(rt, port, slot);
    if (p->ok != 1 || p->reqState == PAD_RSTAT_BUSY)
        return 0;

    switch (infoMode) {
    case PAD_MODECURID:
        return p->modeCurId == 0xf3 ? 0 : p->modeCurId >> 4;
    case PAD_MODECUREXID:
        return p->modeOk == p->ok ? 0 : p->modeTable[p->modeCurOffs];
    case PAD_MODECUROFFS:
        return p->modeOk != 0 ? p->modeCurOffs : 0;
    case PAD_MODETABLE:
        if (p->modeOk == 0)
            return 0;
        if (index == -1)
            return p->nrOfModes;
        return (unsigned)index < p->nrOfModes ? p->modeTable[index] : 0;
    default:
        return 0;
    }
}

/* 0x001a8c20 */
int snes_padSetMainMode(SnesPadRuntime *rt, int port, int slot, int mode, int lock)
{
    int result;
    store32(rt->buffer + 0, PAD_RPCCMD_SET_MMODE);
    store32(rt->buffer + 4, (uint32_t)port);
    store32(rt->buffer + 8, (uint32_t)slot);
    store32(rt->buffer + 12, (uint32_t)mode);
    store32(rt->buffer + 16, (uint32_t)lock);
    if (rpc(rt, 0) < 0)
        return 0;
    result = (int32_t)load32(rt->buffer + 20);
    if (result == 1)
        (void)snes_padSetReqState(rt, port, slot, PAD_RSTAT_BUSY);
    return result;
}

/* forward declaration for the tiny press-mode wrappers */
int snes_padGetButtonMask(SnesPadRuntime *rt, int port, int slot);
int snes_padSetButtonInfo(SnesPadRuntime *rt, int port, int slot, int info);

/* 0x001a8ce0 */
int snes_padInfoPressMode(SnesPadRuntime *rt, int port, int slot)
{
    return (snes_padGetButtonMask(rt, port, slot) ^ 0x3ffff) == 0;
}

/* 0x001a8d0c / 0x001a8d28 */
int snes_padEnterPressMode(SnesPadRuntime *rt, int port, int slot)
{
    return snes_padSetButtonInfo(rt, port, slot, 0xfff);
}

int snes_padExitPressMode(SnesPadRuntime *rt, int port, int slot)
{
    return snes_padSetButtonInfo(rt, port, slot, 0);
}

/* 0x001a8d44 */
int snes_padGetButtonMask(SnesPadRuntime *rt, int port, int slot)
{
    store32(rt->buffer + 0, PAD_RPCCMD_GET_BTNMASK);
    store32(rt->buffer + 4, (uint32_t)port);
    store32(rt->buffer + 8, (uint32_t)slot);
    if (rpc(rt, 0) < 0)
        return 0;
    return (int32_t)load32(rt->buffer + 12);
}

/* 0x001a8dbc */
int snes_padSetButtonInfo(SnesPadRuntime *rt, int port, int slot, int info)
{
    int result;
    store32(rt->buffer + 0, PAD_RPCCMD_SET_BTNINFO);
    store32(rt->buffer + 4, (uint32_t)port);
    store32(rt->buffer + 8, (uint32_t)slot);
    store32(rt->buffer + 12, (uint32_t)info);
    if (rpc(rt, 0) < 0)
        return 0;
    result = (int32_t)load32(rt->buffer + 16);
    if (result == 1)
        (void)snes_padSetReqState(rt, port, slot, PAD_RSTAT_BUSY);
    return result;
}

/* 0x001a8e74 */
uint8_t snes_padInfoAct(SnesPadRuntime *rt, int port, int slot, int actuator, int cmd)
{
    SnesPadData *p = padGetDmaStr(rt, port, slot);
    if (p->ok != 1 || p->modeOk < 2)
        return 0;
    if (actuator == -1)
        return p->nrOfActuators;
    if (actuator < 0 || actuator >= p->nrOfActuators || cmd < 0 || cmd >= 4)
        return 0;
    return p->actData[actuator * 4 + cmd];
}

static int set_act(SnesPadRuntime *rt, uint32_t command, int port, int slot, const uint8_t align[6], int busy)
{
    int result;
    store32(rt->buffer + 0, command);
    store32(rt->buffer + 4, (uint32_t)port);
    store32(rt->buffer + 8, (uint32_t)slot);
    memcpy(rt->buffer + 12, align, 6);
    if (rpc(rt, 0) < 0)
        return 0;
    result = (int32_t)load32(rt->buffer + 20);
    if (busy && result == 1)
        (void)snes_padSetReqState(rt, port, slot, PAD_RSTAT_BUSY);
    return result;
}

/* 0x001a8f08 / 0x001a8fe4 */
int snes_padSetActAlign(SnesPadRuntime *rt, int port, int slot, const uint8_t align[6])
{
    return set_act(rt, PAD_RPCCMD_SET_ACTALIGN, port, slot, align, 1);
}

int snes_padSetActDirect(SnesPadRuntime *rt, int port, int slot, const uint8_t align[6])
{
    return set_act(rt, PAD_RPCCMD_SET_ACTDIR, port, slot, align, 0);
}

/* 0x001a9080 */
int snes_padGetConnection(SnesPadRuntime *rt, int port, int slot)
{
    store32(rt->buffer + 0, PAD_RPCCMD_GET_CONNECT);
    store32(rt->buffer + 4, (uint32_t)port);
    store32(rt->buffer + 8, (uint32_t)slot);
    if (rpc(rt, 0) < 0)
        return -1;
    return (int32_t)load32(rt->buffer + 12);
}
