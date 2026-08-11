/*
 * Recovered low-level Hiryu GSLIB hw.c tail embedded in SNES Station v0.23.
 * Target corridor: 0x0019bd38..0x0019be6c.
 *
 * This deliberately models target machine behaviour. In particular,
 * WaitForNextVRstart was optimized into an infinite loop for every positive
 * argument because the historical VRcount global was not volatile; the EE
 * compiler therefore did not observe the interrupt handler as a writer.
 */
#include <stdint.h>
#include <stddef.h>

static uint32_t VRcount_recovered;

/* 0x0019bd38 */
void VRstart_handler_0019bd38(void)
{
    ++VRcount_recovered;
}

/* 0x0019bd50 -- preserve optimized target bug, not source intent. */
void WaitForNextVRstart_0019bd50(int numvrs)
{
    VRcount_recovered = 0;
    if (numvrs > 0) {
        for (;;) {
            /* target 0x19bd60/0x19bd64: NOP + unconditional-in-practice loop */
        }
    }
}

/* 0x0019bd78 */
int TestVRstart_0019bd78(void)
{
    return (int)VRcount_recovered;
}

/* 0x0019bd88 */
void ClearVRcount_0019bd88(void)
{
    VRcount_recovered = 0;
}

static inline void mmio32_store(uintptr_t address, uint32_t value)
{
    *(volatile uint32_t *)address = value;
}

static inline uint32_t mmio32_load(uintptr_t address)
{
    return *(volatile uint32_t *)address;
}

/* 0x0019bd98 -- Duke-style PS2 DMA reset copied into early GSLIB. */
void DmaReset_0019bd98(void)
{
    mmio32_store(0x1000a080u, 0);
    mmio32_store(0x1000a000u, 0);
    mmio32_store(0x1000a030u, 0);
    mmio32_store(0x1000a010u, 0);
    mmio32_store(0x1000a050u, 0);
    mmio32_store(0x1000a040u, 0);

    mmio32_store(0x1000e010u, 0xff1fu);
    mmio32_store(0x1000e000u, 0);
    mmio32_store(0x1000e020u, 0);
    mmio32_store(0x1000e030u, 0);
    mmio32_store(0x1000e050u, 0);
    mmio32_store(0x1000e040u, 0);
    mmio32_store(0x1000e000u, mmio32_load(0x1000e000u) | 1u);
}

/* 0x0019be20 */
void SendDma02_0019be20(const void *dma_tag)
{
    const uintptr_t ch2 = 0x1000a000u;
    mmio32_store(ch2 + 0x30u, (uint32_t)(uintptr_t)dma_tag);
    mmio32_store(ch2 + 0x20u, 0);
    mmio32_store(ch2 + 0x00u, mmio32_load(ch2) | 0x105u);
}

/* 0x0019be40 */
void Dma02Wait_0019be40(void)
{
    while (mmio32_load(0x1000a000u) & 0x100u) {
        /* spin */
    }
}
