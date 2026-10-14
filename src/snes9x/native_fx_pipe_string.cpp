/* Original Snes9x 1.41-1 Super FX pipe formatter.  The opcode mnemonic
 * table and GSU state are owned by their canonical translation units. */
#include "native_fx_opcode_types.h"

extern const char *fx_apvMnemonicTable[] __asm__("DAT_00341a38");
extern struct FxRegs_s GSU __asm__("DAT_00342a38");
extern const char fx_pipe_address_format[] __asm__("DAT_001b2ca0");
extern const char fx_pipe_word_format[] __asm__("DAT_001b2cb8");
extern const char fx_pipe_moves_format[] __asm__("DAT_001b2cc8");
extern const char fx_pipe_byte_format[] __asm__("DAT_001b2cd8");
extern const char fx_pipe_move_format[] __asm__("DAT_001b2ce8");

extern "C" int sprintf(char *, const char *, ...);
extern "C" unsigned int strlen(const char *);
extern "C" char *strcpy(char *, const char *);

void FxPipeString(char *pvString)
{
    char *p;
    uint32 vOpcode = (GSU.vStatusReg & 0x300) | ((uint32) PIPE);
    const char *m = fx_apvMnemonicTable[vOpcode];
    uint8 vPipe1, vPipe2, vByte1, vByte2;
    uint8 vPipeBank = GSU.vPipeAdr >> 16;

    vPipe1 = GSU.apvRomBank[vPipeBank][USEX16(GSU.vPipeAdr + 1)];
    vPipe2 = GSU.apvRomBank[vPipeBank][USEX16(GSU.vPipeAdr + 2)];
    vByte1 = PRGBANK(USEX16(R15));
    vByte2 = PRGBANK(USEX16(R15 + 1));

    sprintf(pvString, fx_pipe_address_format,
            USEX8(vPipeBank), USEX16(GSU.vPipeAdr), USEX8(PIPE));
    p = &pvString[strlen(pvString)];

    if (PIPE >= 0x05 && PIPE <= 0x0f)
    {
        sprintf(&pvString[11], fx_pipe_byte_format, USEX8(vPipe1));
        sprintf(p, m, USEX16(R15 + SEX8(vByte1) + 1));
    }
    else if (PIPE >= 0x10 && PIPE <= 0x1f && TF(B))
        sprintf(p, fx_pipe_move_format, USEX8(PIPE & 0x0f),
                GSU.pvSreg - GSU.avReg);
    else if (PIPE >= 0xa0 && PIPE <= 0xaf)
    {
        sprintf(&pvString[11], fx_pipe_byte_format, USEX8(vPipe1));
        if ((GSU.vStatusReg & 0x300) == 0x100 ||
            (GSU.vStatusReg & 0x300) == 0x200)
            sprintf(p, m, USEX16(vByte1) << 1);
        else
            sprintf(p, m, USEX16(vByte1));
    }
    else if (PIPE >= 0xb0 && PIPE <= 0xbf && TF(B))
        sprintf(p, fx_pipe_moves_format, GSU.pvDreg - GSU.avReg,
                USEX8(PIPE & 0x0f));
    else if (PIPE >= 0xf0)
    {
        sprintf(&pvString[11], fx_pipe_word_format, USEX8(vPipe1), USEX8(vPipe2));
        sprintf(p, m, USEX8(vByte1) | (USEX16(vByte2) << 8));
    }
    else
        strcpy(p, m);
}
