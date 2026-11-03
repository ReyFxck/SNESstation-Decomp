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
#include "seta.h"
#include "MEMMAP.H"

//temporary Op04 requirement
#include <math.h>

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

ST010_Regs ST010;

uint8 S9xGetST010(uint32 Address)
{
	printf("Read %06X\n", Address);
		return Memory.SRAM[Address&0xFFFF];
}

/*
 *
 Inputs:
 *
 *   scale value: 680004-680005
 *   x:           680002-680003
 *   y:           680000-680001
 *
 * Outputs:
 *
 *   x_scaled: 680014-680017
 *   y_scaled: 680010-680013
 */
void St010_Op03(int16 s, int16 x, int16 y, int32 &xs, int32 &ys )
{
	xs = (int32) x * (int32) s * 2;
	ys = (int32) y * (int32) s * 2;
	return;
}

void St010_Rotate(short Theta, short X0, short Y0, short &X1, short &Y1)
{
	X1 =(short)( ((sin(((short)Theta) * ((float)PI/0x8000))*Y0)*2)  +  ((cos(((short)Theta) * ((float)PI/0x8000))*X0)*2));
	Y1 =(short)( ((cos(((short)Theta) * ((float)PI/0x8000))*Y0)*2) - 	((sin(((short)Theta) * ((float)PI/0x8000))*X0)*2));
}



void S9xSetST010(uint32 Address, uint8 Byte)
{
	if(Address==0x600000)
	{
		ST010.sram_enable=Byte;
		return;
	}
	printf("Write %06X:%02X\n", Address, Byte);

	Memory.SRAM[Address&0xFFFF]=Byte;

	if(Memory.SRAM[0x21]&0x80)
	{
		switch(Memory.SRAM[0x20])
		{
		case 0x06:
			{
				int32 temp=(int16)(Memory.SRAM[0]|(Memory.SRAM[1]<<8));
				temp*=(int16)(Memory.SRAM[2]|(Memory.SRAM[3]<<8));
				temp*=2;
				Memory.SRAM[0x10]=(uint8)(temp);
				Memory.SRAM[0x11]=(uint8)(temp>>8);
				Memory.SRAM[0x12]=(uint8)(temp>>16);
				Memory.SRAM[0x13]=(uint8)(temp>>24);
				break;
			}
		case 0x08:
			{
				int16 angle, xin, xout, yin, yout;
#ifdef FAST_LSB_WORD_ACCESS
				angle=*((int16*)&Memory.SRAM[4]);
				xin=*((int16*)&Memory.SRAM[2]);
				yin=*((int16*)Memory.SRAM);
#else
				angle=Memory.SRAM[4]|(Memory.SRAM[5]<<8);
				xin=Memory.SRAM[2]|(Memory.SRAM[3]<<8);
				yin=Memory.SRAM[0]|(Memory.SRAM[1]<<8);
#endif
				St010_Rotate(angle,xin,yin,xout, yout);

#ifdef FAST_LSB_WORD_ACCESS
				*((int16*)&Memory.SRAM[0x10])=yout;
				*((int16*)&Memory.SRAM[0x12])=xout;
#else
				Memory.SRAM[0x10]=(uint8)yout;
				Memory.SRAM[0x11]=(uint8)(yout>>8);
				Memory.SRAM[0x12]=(uint8)(xout);
				Memory.SRAM[0x13]=(uint8)(xout>>8);
#endif

				//hack to get approximately correct results from the SETA chip
				break;
			}

		case 0x03:
			{
				int32 outx, outy;
				St010_Op03(Memory.SRAM[4]|(Memory.SRAM[5]<<8),Memory.SRAM[2]|(Memory.SRAM[3]<<8),Memory.SRAM[0]|(Memory.SRAM[1]<<8),outx, outy);
#ifdef FAST_LSB_WORD_ACCESS
				*((int32*)&Memory.SRAM[0x14])=outx;
				*((int32*)&Memory.SRAM[0x10])=outy;
#else
				Memory.SRAM[0x14]=(uint8)outx;
				Memory.SRAM[0x15]=(uint8)(outx>>8);
				Memory.SRAM[0x16]=(uint8)(outx>>16);
				Memory.SRAM[0x17]=(uint8)(outx>>24);

				Memory.SRAM[0x10]=(uint8)outy;
				Memory.SRAM[0x11]=(uint8)(outy>>8);
				Memory.SRAM[0x12]=(uint8)(outy>>16);
				Memory.SRAM[0x13]=(uint8)(outy>>24);
#endif
			}
			break;
		case 0x04:
			{
				int16 square, x,y;
#ifdef FAST_LSB_WORD_ACCESS
				x=*((int16*)Memory.SRAM);
				y=*((int16*)&Memory.SRAM[2]);
#else
				x=Memory.SRAM[0]|(Memory.SRAM[1]<<8);
				y=Memory.SRAM[2]|(Memory.SRAM[3]<<8);
#endif
				square=(int16)sqrt(x*x+y*y);

#ifdef FAST_LSB_WORD_ACCESS
				*((int32*)&Memory.SRAM[0x10])=square;
#else
				Memory.SRAM[0x10]=(uint8)square;
				Memory.SRAM[0x11]=(uint8)(square>>8);
#endif
				break;
			}
		case 0x01:
		case 0x07:
		case 0x02:
			break;
		default: printf("Unknown Op\n");break;
		}
		Memory.SRAM[0x20]=0;
		Memory.SRAM[0x21]=0;
	}
}
