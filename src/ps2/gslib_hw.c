/*
 * Historical GSLIB hw.c corridor recovered for SNES Station v0.23.
 * Target corridor: 0x0019bd38..0x0019be6c.
 *
 * Historical evidence: ps2homebrew/gslib source/hw.c
 * commit d9e623a351627e53420f44b00d494346cee5d5a2.
 *
 * Preserve historical source idioms: the DMA helpers were GNU basic inline
 * assembly, not C volatile-MMIO wrappers.
 *
 * REGISTER USAGE (MIPS EE calling convention):
 *   $2 = v0 (return value / temp)
 *   $3 = v1 (temp)
 *   $4 = a0 (arg 0)
 *   $8 = t0 (temp)
 *   $29 = sp (stack pointer)
 *
 * PS2 DMA REGISTERS (DMAC0: 0x1000a000, DMAC1: 0x1000e000):
 *   DMAC0_CHCR_STRIDE  = 0x1000a000  (channel 0 CHCR)
 *   DMAC0_CHCR_MADR    = 0x1000a010  (channel 1 CHCR)
 *   DMAC0_CHCR_TADR    = 0x1000a020  (channel 2 CHCR)
 *   DMAC0_CHCR_VDATA   = 0x1000a030  (channel 3 CHCR)
 *   DMAC0_CHCR_GIF     = 0x1000a040  (channel 4 CHCR)
 *   DMAC0_CHCR_FROMIPU = 0x1000a050  (channel 5 CHCR)
 *   DMAC0_CHCR_TOIPU   = 0x1000a060  (channel 6 CHCR)
 *   DMAC0_CHCR_SIF0    = 0x1000a070  (channel 7 CHCR)
 *   DMAC0_CHCR_SIF1    = 0x1000a080  (channel 8 CHCR) - cleared first
 *
 *   DMAC1_CHCR_SIF2    = 0x1000e000  (channel 9 CHCR)
 *   DMAC1_CHCR_SIF3    = 0x1000e010  (channel 10 CHCR) - set to 0xff1f
 *   ... channels 11-16 at 0x1000e020-0x1000e050
 *
 * DmaReset sequence:
 *   1. Clear all DMAC0 CHCR registers (channels 0-8)
 *   2. Set DMAC1 channel 10 (SIF3) CHCR = 0xff1f (enable + direction + etc)
 *   3. Clear DMAC1 channels 9,11-16
 *   4. Read DMAC1 channel 9 status, OR with 1, write back (ack/clear interrupt?)
 */

static unsigned int VRcount = 0;

/* 0x0019bd38 - VBlank interrupt handler: increment VR counter */
void VRstart_handler_0019bd38(void)
{
    VRcount++;
    return;
}

/* 0x0019bd50 - Wait for N VBlank interrupts */
void WaitForNextVRstart_0019bd50(int numvrs)
{
    VRcount = 0;

    while (VRcount < numvrs)
        ;

    return;
}

/* 0x0019bd78 - Test/get current VR count */
int TestVRstart_0019bd78(void)
{
    return VRcount;
}

/* 0x0019bd88 - Clear VR counter */
void ClearVRcount_0019bd88(void)
{
    VRcount = 0;
    return;
}

/* 0x0019bd98 - Reset all DMA channels */
void DmaReset_0019bd98(void)
{
#if defined(__mips__)
    __asm__ volatile(
        /* ---- DMAC0: Clear CHCR for channels 0-8 (SIF0, SIF1, etc) ---- */
        "sw  $0, 0x1000a080\n"  /* CHCR[8]  = 0 (SIF1) */
        "sw  $0, 0x1000a000\n"  /* CHCR[0]  = 0 (STRIDE) */
        "sw  $0, 0x1000a030\n"  /* CHCR[3]  = 0 (VDATA) */
        "sw  $0, 0x1000a010\n"  /* CHCR[1]  = 0 (MADR) */
        "sw  $0, 0x1000a050\n"  /* CHCR[5]  = 0 (FROMIPU) */
        "sw  $0, 0x1000a040\n"  /* CHCR[4]  = 0 (GIF) */

        /* ---- DMAC1: Configure SIF channels ---- */
        "li  $2, 0xff1f\n"      /* v0 = 0xff1f (DMA enable + direction + chop mode + etc) */
        "sw  $2, 0x1000e010\n"  /* CHCR[10] = 0xff1f (SIF3) */
        "sw  $0, 0x1000e000\n"  /* CHCR[9]  = 0 (SIF2) */
        "sw  $0, 0x1000e020\n"  /* CHCR[11] = 0 */
        "sw  $0, 0x1000e030\n"  /* CHCR[12] = 0 */
        "sw  $0, 0x1000e050\n"  /* CHCR[14] = 0 */
        "sw  $0, 0x1000e040\n"  /* CHCR[13] = 0 */

        /* ---- Acknowledge/clear SIF2 interrupt ---- */
        "lw  $2, 0x1000e000\n"  /* v0 = CHCR[9] (SIF2 status) */
        "ori $3, $2, 1\n"       /* v1 = v0 | 1 (set bit 0) */
        "nop\n"                 /* branch delay slot for following store */
        "sw  $3, 0x1000e000\n"  /* CHCR[9] = v1 (ack interrupt) */
        "nop\n"                 /* branch delay slot */
        :
        :
        : "$2", "$3", "memory"
    );
#else
    (void)0;
#endif
    return;
}

/* 0x0019be20 - Send DMA chain via channel 0 (STRIDE/SPR) */
/* Args: a0 = DmaTag (pointer to DMA tag chain) */
void SendDma02_0019be20(void *DmaTag)
{
#if defined(__mips__)
    register void *a0 __asm__("$4") = DmaTag;   /* arg0 in $4 (a0) */
    register unsigned int v0 __asm__("$2");     /* return value in $2 (v0) */
    register unsigned int v1 __asm__("$3");     /* temp in $3 (v1) */

    __asm__ volatile(
        "li  $3, 0x1000a000\n"      /* v1 = DMAC0 base address (0x1000a000) */
        "sw  $4, 0x0030($3)\n"      /* DMAC0_MADR (0x30) = DmaTag (chain address) */
        "sw  $0, 0x0020($3)\n"      /* DMAC0_CHCR (0x20) = 0 (clear CHCR first) */
        "lw  $2, 0x0000($3)\n"      /* v0 = DMAC0_CHCR (read current status) */
        "ori $2, $2, 0x0105\n"      /* v0 |= 0x0105 (set DIR=to-memory, CHOP=chop, STR=start) */
        "sw  $2, 0x0000($3)\n"      /* DMAC0_CHCR = v0 (start DMA transfer) */
        : "=r"(v0), "=r"(v1)        /* outputs: v0, v1 clobbered */
        : "r"(a0)                   /* input: a0 = DmaTag */
        : "memory"
    );
#else
    (void)DmaTag;
#endif
    return;
}

/* 0x0019be40 - Wait for DMA channel 0 to complete */
void Dma02Wait_0019be40(void)
{
#if defined(__mips__)
    register unsigned int t0 __asm__("$8");     /* temp register $8 (t0) */

    __asm__ volatile(
        "addiu $29, -4\n"         /* sp -= 4 (save $t0 on stack) */
        "sw    $8, 0($29)\n"      /* *sp = $t0 */
        "1:\n"                    /* loop label */
        "lw    $8, 0x1000a000\n"  /* $t0 = DMAC0_CHCR (channel 0 status) */
        "nop\n"                   /* load delay slot */
        "andi  $8, $8, 0x0100\n"  /* $t0 &= 0x0100 (check BE bit - bus error / done) */
        "bnez  $8, 1b\n"          /* if (BE set) branch to 1b (wait) */
        "nop\n"                   /* branch delay slot */
        "lw    $8, 0($29)\n"      /* restore $t0 from stack */
        "addiu $29, 4\n"          /* sp += 4 */
        : "=r"(t0)                /* output: t0 clobbered */
        :                         /* no inputs */
        : "$29", "memory"         /* clobbers: sp, memory */
    );
#else
    (void)0;
#endif
    return;
}