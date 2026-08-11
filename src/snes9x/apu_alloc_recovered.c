/*
 * SNES Station v0.23 — APU/audio buffer allocation helpers recovered from
 * target R5900 code at 0x0010a840..0x0010a933.
 *
 * The state name is intentionally address-based: no historical source symbol
 * has been proven for the global object at EE VA 0x00345498.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

struct RecoveredApuBufferState_00345498 {
    uint32_t unknown_0000;
    uint32_t buffer_0004;       /* allocation size 0x10000 */
    uint8_t unknown_0008[0x18];
    uint32_t buffer_0020;       /* allocation size 0x10000 */
    uint32_t buffer_0024;       /* allocation size 0x40000 */
};

_Static_assert(offsetof(struct RecoveredApuBufferState_00345498, buffer_0004) == 0x04,
               "target offset mismatch");
_Static_assert(offsetof(struct RecoveredApuBufferState_00345498, buffer_0020) == 0x20,
               "target offset mismatch");
_Static_assert(offsetof(struct RecoveredApuBufferState_00345498, buffer_0024) == 0x24,
               "target offset mismatch");

extern struct RecoveredApuBufferState_00345498 g_apu_state_00345498;

void apu_buffer_cleanup_0010a8bc(void);

static uint32_t alloc32(size_t size)
{
    return (uint32_t)(uintptr_t)malloc(size);
}

int apu_buffer_allocator_0010a840(void)
{
    /*
     * Target order is exact:
     *   +0x04 <- malloc(0x10000)
     *   +0x20 <- malloc(0x10000)
     *   +0x24 <- malloc(0x40000)
     */
    g_apu_state_00345498.buffer_0004 = alloc32(0x10000);
    g_apu_state_00345498.buffer_0020 = alloc32(0x10000);
    g_apu_state_00345498.buffer_0024 = alloc32(0x40000);

    if (!g_apu_state_00345498.buffer_0004 ||
        !g_apu_state_00345498.buffer_0020 ||
        !g_apu_state_00345498.buffer_0024) {
        apu_buffer_cleanup_0010a8bc();
        return 0;
    }

    return 1;
}

/*
 * Address-stable integration spelling used by main_flow_recovered.c.
 * It is an aliasing research helper, not a claim about the historical symbol.
 */
int apu_buffers_init_0010a840(void)
{
    return apu_buffer_allocator_0010a840();
}

void apu_buffer_cleanup_0010a8bc(void)
{
    /* Target free/null order is +0x04, +0x20, +0x24. */
    if (g_apu_state_00345498.buffer_0004) {
        free((void *)(uintptr_t)g_apu_state_00345498.buffer_0004);
        g_apu_state_00345498.buffer_0004 = 0;
    }

    if (g_apu_state_00345498.buffer_0020) {
        free((void *)(uintptr_t)g_apu_state_00345498.buffer_0020);
        g_apu_state_00345498.buffer_0020 = 0;
    }

    if (g_apu_state_00345498.buffer_0024) {
        free((void *)(uintptr_t)g_apu_state_00345498.buffer_0024);
        g_apu_state_00345498.buffer_0024 = 0;
    }
}
