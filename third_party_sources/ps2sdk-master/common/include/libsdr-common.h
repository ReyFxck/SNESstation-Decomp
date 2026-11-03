/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/

/**
 * @file
 * Common definitions for libsdr on the EE and IOP
 */

#ifndef __LIBSDR_COMMON_H__
#define __LIBSDR_COMMON_H__

#define sce_SDR_DEV 0x80000701
#define sce_SDRST_CB 0x80000704

#define rSdInit 0x8000
#define rSdSetParam 0x8010
#define rSdGetParam 0x8020
#define rSdSetSwitch 0x8030
#define rSdGetSwitch 0x8040
#define rSdSetAddr 0x8050
#define rSdGetAddr 0x8060
#define rSdSetCoreAttr 0x8070
#define rSdGetCoreAttr 0x8080
#define rSdNote2Pitch 0x8090
#define rSdPitch2Note 0x80A0
#define rSdProcBatch 0x80B0
#define rSdProcBatchEx 0x80C0
#define rSdVoiceTrans 0x80D0
#define rSdBlockTrans 0x80E0
#define rSdVoiceTransStatus 0x80F0
#define rSdBlockTransStatus 0x8100
#define rSdSetTransCallback 0x8110
#define rSdSetIRQCallback 0x8120
#define rSdSetEffectAttr 0x8130
#define rSdGetEffectAttr 0x8140
#define rSdClearEffectWorkArea 0x8150
#define rSdSetTransIntrHandler 0x8160
#define rSdSetSpu2IntrHandler 0x8170
#define rSdStopTrans 0x8180
#define rSdCleanEffectWorkArea 0x8190
#define rSdSetEffectMode 0x81A0
#define rSdSetEffectModeParams 0x81B0
#define rSdProcBatch2 0x81C0
#define rSdProcBatchEx2 0x81D0
#define rSdChangeThreadPriority 0x8F10
#define rSdUserCommand0 0x9000
#define rSdUserCommand1 0x9010
#define rSdUserCommand2 0x9020
#define rSdUserCommand3 0x9030
#define rSdUserCommand4 0x9040
#define rSdUserCommand5 0x9050
#define rSdUserCommand6 0x9060
#define rSdUserCommand7 0x9070
#define rSdUserCommand8 0x9080
#define rSdUserCommand9 0x9090
#define rSdUserCommandA 0x90A0
#define rSdUserCommandB 0x90B0
#define rSdUserCommandC 0x90C0
#define rSdUserCommandD 0x90D0
#define rSdUserCommandE 0x90E0
#define rSdUserCommandF 0x90F0
#define rSdUserCommandMin rSdUserCommand0
#define rSdUserCommandMax rSdUserCommandF

#define SDR_CMD_CB_INIT 0xE620
#define SDR_CMD_CB_DEINIT 0xE630

#define SDR_CB_DMA0 (1 << 0)
#define SDR_CB_DMA1 (1 << 1)
#define SDR_CB_IRQ (1 << 2)
#define SDR_CB_DMA0INT (1 << 8)
#define SDR_CB_DMA1INT (1 << 9)
#define SDR_CB_IRQINT (1 << 10)


typedef struct SdrEECBData_
{
    // cppcheck-suppress unusedStructMember
    int mode;
    // cppcheck-suppress unusedStructMember
    int voice_bit;
    // cppcheck-suppress unusedStructMember
    int status;
    // cppcheck-suppress unusedStructMember
    int opt;
    // cppcheck-suppress unusedStructMember
    int pad[12];
} SdrEECBData;

#endif /* __LIBSDR_COMMON_H__ */
