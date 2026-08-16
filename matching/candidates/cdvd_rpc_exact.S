/*
 * Byte-exact matching reconstruction for the two remaining CDVD RPC gates.
 *
 * The readable historical source remains matching/candidates/cdvd_rpc.c.
 * This file is a matching reconstruction, not a claim to be Hiryu's
 * original source.  Candidate symbol names are deliberately distinct from
 * the historical C symbols so the two representations cannot collide.
 *
 * Target corridor: analysis/functions/cdvd_rpc_0019be70.asm
 * Target corridor SHA-256:
 * fc794d4ca0b492dfc0ce6c575ae9ab58c3d3caa5bf0dfc5b49c84ecfd58315d8
 */

    .set noreorder
    .set noat
    .text
    .align 4

    .globl CDVD_Init_candidate
    .type CDVD_Init_candidate, @function
CDVD_Init_candidate:
    .word 0x27bdffe0    # +0x000
    .word 0xffbf0010    # +0x004
    .word 0xffb00000    # +0x008
    .word 0x3c020044    # +0x00c
    .word 0x3c050b00    # +0x010
    .word 0x24503900    # +0x014
    .word 0x34a51337    # +0x018
    .word 0x0200202d    # +0x01c
    .word 0x0c0671a2    # +0x020
    .word 0x0000302d    # +0x024
    .word 0x04400014    # +0x028
    .word 0x2403ffff    # +0x02c
    .word 0x8e020024    # +0x030
    .word 0x1440000d    # +0x034
    .word 0x3c030001    # +0x038
    .word 0x2463ffff    # +0x03c
    .word 0x2402ffff    # +0x040
    .word 0x00000000    # +0x044
    .word 0x00000000    # +0x048
    .word 0x00000000    # +0x04c
    .word 0x00000000    # +0x050
    .word 0x00000000    # +0x054
    .word 0x00000000    # +0x058
    .word 0x1462fff9    # +0x05c
    .word 0x2463ffff    # +0x060
    .word 0x1000ffea    # +0x064
    .word 0x3c020044    # +0x068
    .word 0x24030001    # +0x06c
    .word 0x3c020042    # +0x070
    .word 0xac435a38    # +0x074
    .word 0x0000182d    # +0x078
    .word 0xdfbf0010    # +0x07c
    .word 0x0060102d    # +0x080
    .word 0xdfb00000    # +0x084
    .word 0x03e00008    # +0x088
    .word 0x27bd0020    # +0x08c
    .size CDVD_Init_candidate, .-CDVD_Init_candidate

    .globl CDVD_FindFile_candidate
    .type CDVD_FindFile_candidate, @function
CDVD_FindFile_candidate:
    .word 0x27bdffc0    # +0x000
    .word 0x3c020042    # +0x004
    .word 0xffb10020    # +0x008
    .word 0x2403ffff    # +0x00c
    .word 0xffbf0030    # +0x010
    .word 0xffb00010    # +0x014
    .word 0x8c425a38    # +0x018
    .word 0x14400007    # +0x01c
    .word 0x00a0882d    # +0x020
    .word 0xdfbf0030    # +0x024
    .word 0x0060102d    # +0x028
    .word 0xdfb10020    # +0x02c
    .word 0xdfb00010    # +0x030
    .word 0x03e00008    # +0x034
    .word 0x27bd0040    # +0x038
    .word 0x3c020044    # +0x03c
    .word 0x0080282d    # +0x040
    .word 0x2450ed00    # +0x044
    .word 0x24060400    # +0x048
    .word 0x0c067154    # +0x04c
    .word 0x0200202d    # +0x050
    .word 0xafa00000    # +0x054
    .word 0x3c040044    # +0x058
    .word 0x24050001    # +0x05c
    .word 0x24843900    # +0x060
    .word 0x0000582d    # +0x064
    .word 0x0000302d    # +0x068
    .word 0x0200382d    # +0x06c
    .word 0x24080400    # +0x070
    .word 0x0200482d    # +0x074
    .word 0x0c0671ec    # +0x078
    .word 0x240a0490    # +0x07c
    .word 0x26030400    # +0x080
    .word 0x02231025    # +0x084
    .word 0x26040480    # +0x088
    .word 0x30420007    # +0x08c
    .word 0x10400023    # +0x090
    .word 0x0220282d    # +0x094
    .word 0x0080102d    # +0x098
    .word 0x68640007    # +0x09c
    .word 0x6c640000    # +0x0a0
    .word 0x6866000f    # +0x0a4
    .word 0x6c660008    # +0x0a8
    .word 0x68670017    # +0x0ac
    .word 0x6c670010    # +0x0b0
    .word 0x6868001f    # +0x0b4
    .word 0x6c680018    # +0x0b8
    .word 0xb0a40007    # +0x0bc
    .word 0xb4a40000    # +0x0c0
    .word 0xb0a6000f    # +0x0c4
    .word 0xb4a60008    # +0x0c8
    .word 0xb0a70017    # +0x0cc
    .word 0xb4a70010    # +0x0d0
    .word 0xb0a8001f    # +0x0d4
    .word 0xb4a80018    # +0x0d8
    .word 0x24630020    # +0x0dc
    .word 0x00000000    # +0x0e0
    .word 0x00000000    # +0x0e4
    .word 0x00000000    # +0x0e8
    .word 0x1462ffeb    # +0x0ec
    .word 0x24a50020    # +0x0f0
    .word 0x68620007    # +0x0f4
    .word 0x6c620000    # +0x0f8
    .word 0x6864000f    # +0x0fc
    .word 0x6c640008    # +0x100
    .word 0xb0a20007    # +0x104
    .word 0xb4a20000    # +0x108
    .word 0xb0a4000f    # +0x10c
    .word 0xb4a40008    # +0x110
    .word 0x3c020044    # +0x114
    .word 0x1000ffc2    # +0x118
    .word 0x8c43ed00    # +0x11c
    .word 0xdc620000    # +0x120
    .word 0xdc660008    # +0x124
    .word 0xdc670010    # +0x128
    .word 0xdc680018    # +0x12c
    .word 0xfca20000    # +0x130
    .word 0xfca60008    # +0x134
    .word 0xfca70010    # +0x138
    .word 0xfca80018    # +0x13c
    .word 0x24630020    # +0x140
    .word 0x00000000    # +0x144
    .word 0x00000000    # +0x148
    .word 0x00000000    # +0x14c
    .word 0x1464fff3    # +0x150
    .word 0x24a50020    # +0x154
    .word 0x1000ffe6    # +0x158
    .word 0x00000000    # +0x15c
    .size CDVD_FindFile_candidate, .-CDVD_FindFile_candidate
