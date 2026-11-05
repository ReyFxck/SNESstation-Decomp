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

static unsigned int VRcount_recovered = 0;

/* 0x0019bd38 */
void VRstart_handler_0019bd38(void)
{
    VRcount_recovered++;
    return;
}

/* 0x0019bd50 */
void WaitForNextVRstart_0019bd50(int numvrs)
{
    VRcount_recovered = 0;

    while (VRcount_recovered < numvrs)
        ;

    return;
}

/* 0x0019bd78 */
int TestVRstart_0019bd78(void)
{
    return VRcount_recovered;
}

/* 0x0019bd88 */
void ClearVRcount_0019bd88(void)
{
    VRcount_recovered = 0;
    return;
}

/* 0x0019bd98 */
void DmaReset_0019bd98(void)
{
    __asm__("\tsw  $0, 0x1000a080");
    __asm__("\tsw  $0, 0x1000a000");
    __asm__("\tsw  $0, 0x1000a030");
    __asm__("\tsw  $0, 0x1000a010");
    __asm__("\tsw  $0, 0x1000a050");
    __asm__("\tsw  $0, 0x1000a040");
    __asm__("\tli  $2, 0xff1f");
    __asm__("\tsw  $2, 0x1000e010");
    __asm__("\tsw  $0, 0x1000e000");
    __asm__("\tsw  $0, 0x1000e020");
    __asm__("\tsw  $0, 0x1000e030");
    __asm__("\tsw  $0, 0x1000e050");
    __asm__("\tsw  $0, 0x1000e040");
    __asm__("\tlw  $2, 0x1000e000");
    __asm__("\tori $3,$2,1");
    __asm__("\tnop");
    __asm__("\tsw  $3, 0x1000e000");
    __asm__("\tnop");

    return;
}

/* 0x0019be20 */
void SendDma02_0019be20(void *DmaTag)
{
    __asm__("\tli $3, 0x1000a000");

    __asm__("\tsw $4, 0x0030($3)");
    __asm__("\tsw $0, 0x0020($3)");
    __asm__("\tlw $2, 0x0000($3)");
    __asm__("\tori $2, 0x0105");
    __asm__("\tsw $2, 0x0000($3)");

    return;
}

/* 0x0019be40 */
void Dma02Wait_0019be40(void)
{
    __asm__("\taddiu $29, -4");
    __asm__("\tsw $8, 0($29)");

    __asm__("Dma02Wait.poll:");
    __asm__("\tlw $8, 0x1000a000");
    __asm__("\tnop");
    __asm__("\tandi $8, $8, 0x0100");
    __asm__("\tbnez $8, Dma02Wait.poll");
    __asm__("\tnop");

    __asm__("\tlw $8, 0($29)");
    __asm__("\taddiu $29, 4");

    return;
}
