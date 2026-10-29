/*
 * Historical GSLIB hw.c corridor recovered for SNES Station v0.23.
 * Target corridor: 0x0019bd38..0x0019be6c.
 *
 * Historical evidence: ps2homebrew/gslib source/hw.c
 * commit d9e623a351627e53420f44b00d494346cee5d5a2.
 *
 * Preserve historical source idioms: the DMA helpers were GNU basic inline
 * assembly, not C volatile-MMIO wrappers.
 */

static unsigned int VRcount = 0;

/* 0x0019bd38 */
void VRstart_handler_0019bd38(void)
{
    VRcount++;
    return;
}

/* 0x0019bd50 */
void WaitForNextVRstart_0019bd50(int numvrs)
{
    VRcount = 0;

    while (VRcount < numvrs)
        ;

    return;
}

/* 0x0019bd78 */
int TestVRstart_0019bd78(void)
{
    return VRcount;
}

/* 0x0019bd88 */
void ClearVRcount_0019bd88(void)
{
    VRcount = 0;
    return;
}

/* 0x0019bd98 */
void DmaReset_0019bd98(void)
{
#if defined(__mips__)
    __asm__ volatile(
        "sw  $0, 0x1000a080\n"
        "sw  $0, 0x1000a000\n"
        "sw  $0, 0x1000a030\n"
        "sw  $0, 0x1000a010\n"
        "sw  $0, 0x1000a050\n"
        "sw  $0, 0x1000a040\n"
        "li  $2, 0xff1f\n"
        "sw  $2, 0x1000e010\n"
        "sw  $0, 0x1000e000\n"
        "sw  $0, 0x1000e020\n"
        "sw  $0, 0x1000e030\n"
        "sw  $0, 0x1000e050\n"
        "sw  $0, 0x1000e040\n"
        "lw  $2, 0x1000e000\n"
        "ori $3, $2, 1\n"
        "nop\n"
        "sw  $3, 0x1000e000\n"
        "nop\n"
        :
        :
        : "$2", "$3", "memory"
    );
#else
    (void)0;
#endif
    return;
}

/* 0x0019be20 */
void SendDma02_0019be20(void *DmaTag)
{
#if defined(__mips__)
    register void *a0 __asm__("$4") = DmaTag;
    register uint32_t v0 __asm__("$2");
    register uint32_t v1 __asm__("$3");

    __asm__ volatile(
        "li  $3, 0x1000a000\n"
        "sw  $4, 0x0030($3)\n"
        "sw  $0, 0x0020($3)\n"
        "lw  $2, 0x0000($3)\n"
        "ori $2, $2, 0x0105\n"
        "sw  $2, 0x0000($3)\n"
        : "=r"(v0), "=r"(v1)
        : "r"(a0)
        : "memory"
    );
#else
    (void)DmaTag;
#endif
    return;
}

/* 0x0019be40 */
void Dma02Wait_0019be40(void)
{
#if defined(__mips__)
    register uint32_t t0 __asm__("$8");

    __asm__ volatile(
        "addiu $29, -4\n"
        "sw    $8, 0($29)\n"
        "1:\n"
        "lw    $8, 0x1000a000\n"
        "nop\n"
        "andi  $8, $8, 0x0100\n"
        "bnez  $8, 1b\n"
        "nop\n"
        "lw    $8, 0($29)\n"
        "addiu $29, 4\n"
        : "=r"(t0)
        :
        : "$29", "memory"
    );
#else
    (void)0;
#endif
    return;
}