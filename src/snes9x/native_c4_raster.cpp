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
/* Native C4 raster helpers from Snes9x 1.41-1.
 * These share the original C4 RAM and signed-short math state. The packed
 * PS2 read forms and one allocation constraint reproduce original code.
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
extern "C" {
extern short C4WFXVal __asm__("_DAT_00335938");
extern short C4WFYVal __asm__("_DAT_0033593a");
extern short C4WFZVal __asm__("DAT_0033593c");
extern short C4WFX2Val __asm__("DAT_0033593e");
extern short C4WFY2Val __asm__("DAT_00335940");
extern short C4WFDist __asm__("DAT_00335942");
extern short C4WFScale __asm__("DAT_00335944");

void C4TransfWireFrame2();
void C4CalcWireFrame();
void *memset(void *, int, unsigned int);
}
struct Hunt1041PackedU32
{
    uint32 value;
} __attribute__((packed));

#define READ_WORD(s) ({ \
    register uint8 *source = (uint8 *) (s); \
    __asm__ volatile ("" : "+r" (source)); \
    register uint32 value = ((Hunt1041PackedU32 *) source)->value; \
    value &= 0xffff; \
    __asm__ volatile ("" : "+r" (value)); \
    (uint16) (value); \
})

/*
 * EE GCC 3.2.2 otherwise coalesces the final sign-extension back into $a0.
 * Keeping this single non-volatile shift as a code-generation constraint
 * reproduces the target's $a0 -> $v0 -> $s4 allocation without constraining
 * the surrounding C implementation or copying target instructions wholesale.
 */
#define READ_SIGNED_WORD_V0(s) ({ \
    register uint8 *source __asm__("$2") = (uint8 *) (s); \
    __asm__ volatile ("" : "+r" (source)); \
    register uint32 value __asm__("$4") = ((Hunt1041PackedU32 *) source)->value; \
    value &= 0xffff; \
    __asm__ volatile ("" : "+r" (value)); \
    register uint32 shifted __asm__("$2"); \
    __asm__ ("sll %0,%1,16" : "=r" (shifted) : "r" (value)); \
    ((int32) shifted) >> 16; \
})

/* 0x0010cbb0: original native raster helper. */
void C4DrawLine(int32 X1, int32 Y1, int16 Z1,
                       int32 X2, int32 Y2, int16 Z2, uint8 Color){
    // Transform coordinates
    C4WFXVal=(short)X1;
    C4WFYVal=(short)Y1;
    C4WFZVal=Z1;
    C4WFScale=Memory.C4RAM[0x1f90];
    C4WFX2Val=Memory.C4RAM[0x1f86];
    C4WFY2Val=Memory.C4RAM[0x1f87];
    C4WFDist=Memory.C4RAM[0x1f88];
    C4TransfWireFrame2();
    X1=(C4WFXVal+48)<<8;
    Y1=(C4WFYVal+48)<<8;

    C4WFXVal=(short)X2;
    C4WFYVal=(short)Y2;
    C4WFZVal=Z2;
    C4TransfWireFrame2();
    X2=(C4WFXVal+48)<<8;
    Y2=(C4WFYVal+48)<<8;

    // get line info
    C4WFXVal=(short)(X1>>8);
    C4WFYVal=(short)(Y1>>8);
    C4WFX2Val=(short)(X2>>8);
    C4WFY2Val=(short)(Y2>>8);
    C4CalcWireFrame();
    X2=(int16)C4WFXVal;
    Y2=(int16)C4WFYVal;

    // render line
    for(int i=C4WFDist?C4WFDist:1; i>0; i--)
	{ //.loop
        if(X1>0xff && Y1>0xff && X1<0x6000 && Y1<0x6000)
		{
            uint16 addr=((X1&~0x7ff) + (Y1&~0x7ff)*12 + (Y1&0x700))>>7;
            addr=(((Y1>>8)>>3)<<8)-(((Y1>>8)>>3)<<6)+(((X1>>8)>>3)<<4)+((Y1>>8)&7)*2;
            uint8 bit=0x80>>((X1>>8)&7);
            Memory.C4RAM[addr+0x300]&=~bit;
            Memory.C4RAM[addr+0x301]&=~bit;
            if(Color&1) Memory.C4RAM[addr+0x300]|=bit;
            if(Color&2) Memory.C4RAM[addr+0x301]|=bit;
        }
        X1+=X2;
        Y1+=Y2;
    }
}

/* 0x0010d4f0: original native raster helper. */
void C4SprDisintegrate()
{
    uint8 width, height;
    uint32 StartX, StartY;
    uint8 *src;
    register uint8 *ram __asm__("$7")=Memory.C4RAM;
    int32 scaleX, scaleY;
    int32 Cx, Cy;

    width=ram[0x1f89];
    height=ram[0x1f8c];
    Cx=(int16)READ_WORD(ram+0x1f80);
    Cy=(int16)READ_WORD(ram+0x1f83);

#ifdef DEBUGGER
    if((Cx&~1)!=width/2 || (Cy&~1)!=height/2) printf("Center is not middle of image for disintegrate! (%d, %d) != (%d, %d)\n", Cx, Cy, width/2, height/2);
#endif

    scaleX=(int16)READ_WORD(ram+0x1f86);
    scaleY=READ_SIGNED_WORD_V0(ram+0x1f8f);
    StartX=-Cx*scaleX+(Cx<<8);
    StartY=-Cy*scaleY+(Cy<<8);
    src=ram+0x600;

    memset(ram, 0, width*height/2);

    for(uint32 y=StartY, i=0; i<height; i++, y+=scaleY)
	{
        for(uint32 x=StartX, j=0; j<width; j++, x+=scaleX)
		{
            if((x>>8)<width && (y>>8)<height && (y>>8)*width+(x>>8)<0x2000)
			{
                uint8 pixel=(j&1)?(*src>>4):*src;
                int idx=(y>>11)*width*4+(x>>11)*32+((y>>8)&7)*2;
                uint8 mask=0x80>>((x>>8)&7);
                if(pixel&1) Memory.C4RAM[idx]|=mask;
                if(pixel&2) Memory.C4RAM[idx+1]|=mask;
                if(pixel&4) Memory.C4RAM[idx+16]|=mask;
                if(pixel&8) Memory.C4RAM[idx+17]|=mask;
            }
            if(j&1) src++;
        }
    }
}

typedef char NativeC4MemoryPrefixSize[(sizeof(NativeC4Memory)==28)?1:-1];
