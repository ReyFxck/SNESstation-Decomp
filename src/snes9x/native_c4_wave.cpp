/*******************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2003 Gary Henderson (gary.henderson@ntlworld.com) and
                            Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2003 Matthew Kendora and
                            Brad Jorsch (anomie@users.sourceforge.net)



  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003 zsKnight (zsknight@zsnes.com),
                            _Demo_ (_demo_@zsnes.com), and
                            Nach (n-a-c-h@users.sourceforge.net)

  C4 C++ code
  (c) Copyright 2003 Brad Jorsch

  DSP-1 emulator code
  (c) Copyright 1998 - 2003 Ivar (ivar@snes9x.com), _Demo_, Gary Henderson,
                            John Weidman (jweidman@slip.net),
                            neviksti (neviksti@hotmail.com), and
                            Kris Bleakley (stinkfish@bigpond.com)

  DSP-2 emulator code
  (c) Copyright 2003 Kris Bleakley, John Weidman, neviksti, Matthew Kendora, and
                     Lord Nightmare (lord_nightmare@users.sourceforge.net

  OBC1 emulator code
  (c) Copyright 2001 - 2003 zsKnight, pagefault (pagefault@zsnes.com)
  Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002 Matthew Kendora with research by
                     zsKnight, John Weidman, and Dark Force

  S-RTC C emulator code
  (c) Copyright 2001 John Weidman

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003 zsKnight, _Demo_, and pagefault

  Super FX C emulator code
  (c) Copyright 1997 - 1999 Ivar and Gary Henderson.




  Specific ports contains the works of other authors. See headers in
  individual files.

  Snes9x homepage: http://www.snes9x.com

  Permission to use, copy, modify and distribute Snes9x in both binary and
  source form, for non-commercial purposes, is hereby granted without fee,
  providing that this license information and copyright notice appear with
  all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes
  charging money for Snes9x or software derived from Snes9x.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
*******************************************************************************/
/* Native C4 wave bitplane rendering from Snes9x 1.41-1.
 * Packed PS2 loads retain the original load registers and writes. This TU
 * alone uses the independently proved local $t5-before-$t4 compiler profile.
 * The original 80-byte bitmap offset table is retained at 0x00335a00.
 */
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef signed short   int16;
typedef unsigned int   uint32;
typedef signed int     int32;

struct NativeC4Memory
{
    uint8 *reserved_00;
    uint8 *reserved_04;
    uint8 *reserved_08;
    uint8 *reserved_0c;
    uint8 *reserved_10;
    uint8 *reserved_14;
    uint8 *C4RAM; /* historical Snes9x/PS2 layout: +0x18 */
};

extern NativeC4Memory Memory __asm__("DAT_0034e2b0");
typedef signed char int8;
struct Hunt1041PackedU32 { uint32 value; } __attribute__((packed));
#define WRITE_WORD(s, d) *(uint8 *) (s) = (d), \
                         *((uint8 *) (s) + 1) = (d) >> 8

#define READ_WAVE_WORD_REG(s, regname) ({ \
    register uint8 *source = (uint8 *) (s); \
    __asm__ volatile ("" : "+r" (source)); \
    register uint32 value __asm__(regname) = ((Hunt1041PackedU32 *) source)->value; \
    value &= 0xffff; \
    __asm__ volatile ("" : "+r" (value)); \
    register uint32 result = value & 0xffff; \
    __asm__ volatile ("" : "+r" (value), "+r" (result)); \
    result; \
})
/* 0x0010d2a8: original native C4 wave renderer. */
void C4BitPlaneWave(){
    static uint16 bmpdata[]={
        0x0000, 0x0002, 0x0004, 0x0006, 0x0008, 0x000A, 0x000C, 0x000E,
        0x0200, 0x0202, 0x0204, 0x0206, 0x0208, 0x020A, 0x020C, 0x020E,
        0x0400, 0x0402, 0x0404, 0x0406, 0x0408, 0x040A, 0x040C, 0x040E,
        0x0600, 0x0602, 0x0604, 0x0606, 0x0608, 0x060A, 0x060C, 0x060E,
        0x0800, 0x0802, 0x0804, 0x0806, 0x0808, 0x080A, 0x080C, 0x080E
    };

    uint8 *dst=Memory.C4RAM;
    uint32 waveptr=Memory.C4RAM[0x1f83];
    uint16 mask1=0xc0c0;
    uint16 mask2=0x3f3f;

#ifdef DEBUGGER
    if(READ_3WORD(Memory.C4RAM+0x1f80) != Memory.C4RAM[waveptr+0xb00]) printf("$7f80=%06x, expected %02x\n", READ_3WORD(Memory.C4RAM+0x1f80), Memory.C4RAM[waveptr+0xb00]);
#endif

    register int j __asm__("$16");
    for(j=0; j<0x10; j++){
        do {
            int16 height=-((int8)Memory.C4RAM[waveptr+0xb00])-16;
            for(int i=0; i<40; i++){
                uint16 tmp=READ_WAVE_WORD_REG(dst+bmpdata[i], "$14") & mask2;
                if(height>=0){
                    if(height<8){
                        tmp|=mask1&READ_WAVE_WORD_REG(Memory.C4RAM+0xa00+height*2, "$15");
                    } else {
                        tmp|=mask1&0xff00;
                    }
                }
                WRITE_WORD(dst+bmpdata[i], tmp);
                height++;
            }
            waveptr=(waveptr+1)&0x7f;
            mask1=(mask1>>2)|(mask1<<6);
            mask2=(mask2>>2)|(mask2<<6);
        } while(mask1!=0xc0c0);
        dst+=16;

        do {
            int16 height=-((int8)Memory.C4RAM[waveptr+0xb00])-16;
            for(int i=0; i<40; i++){
                uint32 tmp=mask2 & READ_WAVE_WORD_REG(dst+bmpdata[i], "$24");
                if(height>=0){
                    if(height<8){
                        tmp|=mask1&READ_WAVE_WORD_REG(Memory.C4RAM+0xa10+height*2, "$25");
                    } else {
                        tmp|=mask1&0xff00;
                    }
                }
                WRITE_WORD(dst+bmpdata[i], tmp);
                height++;
            }
            waveptr=(waveptr+1)&0x7f;
            mask1=(mask1>>2)|(mask1<<6);
            mask2=(mask2>>2)|(mask2<<6);
        } while(mask1!=0xc0c0);
        dst+=16;
    }
}
