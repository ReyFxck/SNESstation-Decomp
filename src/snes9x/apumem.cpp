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

/*
 * Historical Snes9x 1.42 apumem.h bodies; archive SHA-256:
 * 136c7c9bf826bf9dba91073aae14e4b315ab78e67eee189322dec2637e949ffb.
 * Isolated out-of-line definitions using the SNES Station EE data layout.
 * The target listing proves SAPU ExtraRAM/TimerTarget offsets 0x8b/0xd2
 * and SIAPU RAM/DirectPage at 4/8, preserving the historical structures.
 * The audit's Get/SetByte labels swap the historical Z/non-Z names.
 */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned char bool8;

struct SIAPU {
    uint8 *PC;
    uint8 *RAM;
    uint8 *DirectPage;
    bool8 APUExecuting;
    uint8 Bit;
    uint32 Address;
    uint8 *WaitAddress1;
    uint8 *WaitAddress2;
};
struct SAPU {
    int32 Cycles;
    bool8 ShowROM;
    uint8 Flags;
    uint8 KeyedChannels;
    uint8 OutPorts[4];
    uint8 DSP[0x80];
    uint8 ExtraRAM[64];
    uint16 Timer[3];
    uint16 TimerTarget[3];
    bool8 TimerEnabled[3];
    bool8 TimerValueWritten[3];
};
extern SIAPU IAPU __asm__("g_apu_state_00345498");
extern SAPU APU __asm__("g_APU_003453b8");
extern "C" uint8 S9xGetAPUDSP() __asm__("snes_p24_0010b7f8");
extern "C" void S9xSetAPUDSP(uint8) __asm__("snes_p28_0010ad48");
extern "C" void S9xSetAPUControl(uint8) __asm__("snes_p28_0010b564");

/* 0x001ac994 */
uint8 S9xAPUGetByteZ (uint8 Address)
{
    if (Address >= 0xf0 && IAPU.DirectPage == IAPU.RAM)
    {
	if (Address >= 0xf4 && Address <= 0xf7)
	{
#ifdef SPC700_SHUTDOWN
	    IAPU.WaitAddress2 = IAPU.WaitAddress1;
	    IAPU.WaitAddress1 = IAPU.PC;
#endif
	    return (IAPU.RAM [Address]);
	}
	if (Address >= 0xfd)
	{
#ifdef SPC700_SHUTDOWN
	    IAPU.WaitAddress2 = IAPU.WaitAddress1;
	    IAPU.WaitAddress1 = IAPU.PC;
#endif
	    uint8 t = IAPU.RAM [Address];
	    IAPU.RAM [Address] = 0;
	    return (t);
	}
	else
	if (Address == 0xf3)
	    return (S9xGetAPUDSP ());

	return (IAPU.RAM [Address]);
    }
    else
	return (IAPU.DirectPage [Address]);
}

/* 0x001aca50 */
void S9xAPUSetByteZ (uint8 byte, uint8 Address)
{
    if (Address >= 0xf0 && IAPU.DirectPage == IAPU.RAM)
    {
	if (Address == 0xf3)
	    S9xSetAPUDSP (byte);
	else
	if (Address >= 0xf4 && Address <= 0xf7)
	    APU.OutPorts [Address - 0xf4] = byte;
	else
	if (Address == 0xf1)
	    S9xSetAPUControl (byte);
	else
	if (Address < 0xfd)
	{
	    IAPU.RAM [Address] = byte;
	    if (Address >= 0xfa)
	    {
		if (byte == 0)
		    APU.TimerTarget [Address - 0xfa] = 0x100;
		else
		    APU.TimerTarget [Address - 0xfa] = byte;
	    }
	}
    }
    else
	IAPU.DirectPage [Address] = byte;
}

/* 0x001acb3c */
uint8 S9xAPUGetByte (uint32 Address)
{
    Address &= 0xffff;

    if (Address <= 0xff && Address >= 0xf0)
    {
	if (Address >= 0xf4 && Address <= 0xf7)
	{
#ifdef SPC700_SHUTDOWN
	    IAPU.WaitAddress2 = IAPU.WaitAddress1;
	    IAPU.WaitAddress1 = IAPU.PC;
#endif
	    return (IAPU.RAM [Address]);
	}
	else
	if (Address == 0xf3)
	    return (S9xGetAPUDSP ());
	if (Address >= 0xfd)
	{
#ifdef SPC700_SHUTDOWN
	    IAPU.WaitAddress2 = IAPU.WaitAddress1;
	    IAPU.WaitAddress1 = IAPU.PC;
#endif
	    uint8 t = IAPU.RAM [Address];
	    IAPU.RAM [Address] = 0;
	    return (t);
	}
	return (IAPU.RAM [Address]);
    }
    else
	return (IAPU.RAM [Address]);
}

/* 0x001acbf0 */
void S9xAPUSetByte (uint8 byte, uint32 Address)
{
    Address &= 0xffff;

    if (Address <= 0xff && Address >= 0xf0)
    {
	if (Address == 0xf3)
	    S9xSetAPUDSP (byte);
	else
	if (Address >= 0xf4 && Address <= 0xf7)
	    APU.OutPorts [Address - 0xf4] = byte;
	else
	if (Address == 0xf1)
	    S9xSetAPUControl (byte);
	else
	if (Address < 0xfd)
	{
	    IAPU.RAM [Address] = byte;
	    if (Address >= 0xfa)
	    {
		if (byte == 0)
		    APU.TimerTarget [Address - 0xfa] = 0x100;
		else
		    APU.TimerTarget [Address - 0xfa] = byte;
	    }
	}
    }
    else
    {
#if 0
if (Address >= 0x2500 && Address <= 0x2504)
printf ("%06d %04x <- %02x\n", ICPU.Scanline, Address, byte);
if (Address == 0x26c6)
{
    extern FILE *apu_trace;
    extern FILE *trace;
    APU.Flags |= TRACE_FLAG;
    CPU.Flags |= TRACE_FLAG;
    if (apu_trace == NULL)
	apu_trace = fopen ("aputrace.log", "wb");
    if (trace == NULL)
	trace = fopen ("trace.log", "wb");
    printf ("TRACING SWITCHED ON\n");
}
#endif
	if (Address < 0xffc0)
	    IAPU.RAM [Address] = byte;
	else
	{
	    APU.ExtraRAM [Address - 0xffc0] = byte;
	    if (!APU.ShowROM)
		IAPU.RAM [Address] = byte;
	}
    }
}
