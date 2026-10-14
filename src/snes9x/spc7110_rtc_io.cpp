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
/* Historical Snes9x 1.41-1 SPC7110 recovery. Archive SHA-256:
 * 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * Persisted time uses the target's 32-bit field. RTC update retains its
 * target-proved disabled wall clock; no host clock substitution is made.
 */


//#define SPC7110_DEBUG

typedef unsigned char uint8;
typedef uint8 bool8;
#define TRUE 1
#define FALSE 0
struct SPC7110RTC { unsigned char reg[16]; short index; uint8 control; bool init; int last_used; };
typedef SPC7110RTC S7RTC;
extern "C" char *S9xGetFilename(const char *) __asm__("snes_p12_get_filename");
extern "C" int fioOpen(const char *,int);
extern "C" int fioClose(int);
extern "C" int fioRead(int,void *,int);
extern "C" int fioWrite(int,const void *,int);
/* 0x001833a4 -- original 24-byte little-endian FIO writer. */
bool8 S9xSaveSPC7110RTC (S7RTC *rtc_f9)
{
    int fp;

    if((fp=fioOpen(S9xGetFilename(".rtc"), 0x202))<0)
        return (FALSE);
	int i=0;
	uint8 temp=0;
	for (i=0;i<16;i++)
		fioWrite(fp,&rtc_f9->reg[i],1);
	temp=rtc_f9->index&0x00FF;
	fioWrite(fp,&temp,1);
	temp=(rtc_f9->index)>>8;
	fioWrite(fp,&temp,1);
	temp=(uint8)rtc_f9->control;
	fioWrite(fp,&temp,1);
	temp=(uint8)rtc_f9->init;
	fioWrite(fp,&temp,1);
	temp=rtc_f9->last_used&0x00FF;
	fioWrite(fp,&temp,1);
	temp=(rtc_f9->last_used>>8)&0x00FF;
	fioWrite(fp,&temp,1);
	temp=(rtc_f9->last_used>>16)&0x00FF;
	fioWrite(fp,&temp,1);
	temp=(rtc_f9->last_used>>24)&0x00FF;;
	fioWrite(fp,&temp,1);
	fioClose(fp);
    return (TRUE);
}

/* 0x001834f0 -- original FIO reader; per-byte read results remain ignored. */
bool8 S9xLoadSPC7110RTC (S7RTC *rtc_f9)
{
    int fp;

    if((fp=fioOpen(S9xGetFilename(".rtc"), 1))<0)
        return (FALSE);
	for (int i=0; i<16;i++)
	{
		fioRead(fp,&(rtc_f9->reg[i]),1);
	}
	uint8 temp=0;
	fioRead(fp,&temp,1);
	rtc_f9->index=temp;
	fioRead(fp,&temp,1);
	rtc_f9->index|=(temp<<8);
	fioRead(fp,&rtc_f9->control,1);
	fioRead(fp,&rtc_f9->init,1);

	fioRead(fp,&temp,1);
	rtc_f9->last_used=temp;
	fioRead(fp,&temp,1);
	rtc_f9->last_used|=(temp<<8);
	fioRead(fp,&temp,1);
	rtc_f9->last_used|=(temp<<16);
	fioRead(fp,&temp,1);
	rtc_f9->last_used|=(temp<<24);
    fioClose(fp);
    return (TRUE);
}
