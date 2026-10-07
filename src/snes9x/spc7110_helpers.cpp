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
typedef unsigned int uint32;
typedef int int32;
#define DECOMP_BUFFER_SIZE 0x10000
/* The shared target RTC state occupies 24 bytes. */
typedef struct SPC7110RTC
{
	unsigned char reg[16];
	short index;
	uint8 control;
	bool init;
	int32 last_used;
} S7RTC;

typedef struct SPC7110EmuVars
{
	unsigned char reg4800;
	unsigned char reg4801;
	unsigned char reg4802;
	unsigned char reg4803;
	unsigned char reg4804;
	unsigned char reg4805;
	unsigned char reg4806;
	unsigned char reg4807;
	unsigned char reg4808;
	unsigned char reg4809;
	unsigned char reg480A;
	unsigned char reg480B;
	unsigned char reg480C;
	unsigned char reg4811;
	unsigned char reg4812;
	unsigned char reg4813;
	unsigned char reg4814;
	unsigned char reg4815;
	unsigned char reg4816;
	unsigned char reg4817;
	unsigned char reg4818;
	unsigned char reg4820;
	unsigned char reg4821;
	unsigned char reg4822;
	unsigned char reg4823;
	unsigned char reg4824;
	unsigned char reg4825;
	unsigned char reg4826;
	unsigned char reg4827;
	unsigned char reg4828;
	unsigned char reg4829;
	unsigned char reg482A;
	unsigned char reg482B;
	unsigned char reg482C;
	unsigned char reg482D;
	unsigned char reg482E;
	unsigned char reg482F;
	unsigned char reg4830;
	unsigned char reg4831;
	unsigned char reg4832;
	unsigned char reg4833;
	unsigned char reg4834;
	unsigned char reg4840;
	unsigned char reg4841;
	unsigned char reg4842;
	uint8 AlignBy;
	uint8 written;
	uint8 offset_add;
	uint32 DataRomOffset;
	uint32 DataRomSize;
	uint32 bank50Internal;
	uint8 bank50[DECOMP_BUFFER_SIZE];

} SPC7110Regs;

extern SPC7110Regs s7r __asm__("g_s7r_blob");
extern S7RTC rtc_f9 __asm__("DAT_00423548");
extern uint8 *ROM __asm__("DAT_0034e29c");
extern "C" void *memset(void *,int,unsigned int);
int S9xRTCDaysInMonth(int,int);
#define DAYTICKS (60*60*24)
#define HOURTICKS (60*60)
#define MINUTETICKS 60
/* 0x00182638 -- target wall clock remains zero. */
void S9xUpdateRTC ()
{
	int32	cur_systime;
	long    time_diff;

    // Keep track of game time by computing the number of seconds that pass on the system
    // clock and adding the same number of seconds to the RTC clock structure.

    if (rtc_f9.init && 0==(rtc_f9.reg[0x0D]&0x01) && 0==(rtc_f9.reg[0x0F]&0x03))
    {
        cur_systime = 0;

        // This method assumes one time_t clock tick is one second
        //        which should work on PCs and GNU systems.
        //        If your tick interval is different adjust the
		//        DAYTICK, HOURTICK, and MINUTETICK defines

        time_diff = (long) (cur_systime - rtc_f9.last_used);
		rtc_f9.last_used = cur_systime;

        if ( time_diff > 0 )
        {
			int		seconds;
			int		minutes;
			int		hours;
			int		days;
			int		month;
			int		year;
			int		temp_days;

			int		year_hundreds;
			int		year_tens;
			int		year_ones;


			if ( time_diff > DAYTICKS )
			{
				days = time_diff / DAYTICKS;
				time_diff = time_diff - days * DAYTICKS;
			}
			else
			{
				days = 0;
			}

			if ( time_diff > HOURTICKS )
			{
				hours = time_diff / HOURTICKS;
				time_diff = time_diff - hours * HOURTICKS;
			}
			else
			{
				hours = 0;
			}

			if ( time_diff > MINUTETICKS )
			{
				minutes = time_diff / MINUTETICKS;
				time_diff = time_diff - minutes * MINUTETICKS;
			}
			else
			{
				minutes = 0;
			}

			if ( time_diff > 0 )
			{
				seconds = time_diff;
			}
			else
			{
				seconds = 0;
			}


			seconds += (rtc_f9.reg[1]*10 + rtc_f9.reg[0]);
			if ( seconds >= 60 )
			{
				seconds -= 60;
				minutes += 1;
			}

			minutes += (rtc_f9.reg[3]*10 + rtc_f9.reg[2]);
			if ( minutes >= 60 )
			{
				minutes -= 60;
				hours += 1;
			}

			hours += (rtc_f9.reg[5]*10 + rtc_f9.reg[4]);
			if ( hours >= 24 )
			{
				hours -= 24;
				days += 1;
			}

			year =  rtc_f9.reg[11]*10 + rtc_f9.reg[10];
			year += ( 1900 );
			month = rtc_f9.reg[8]+10*rtc_f9.reg[9];
			rtc_f9.reg[12]+=days;
			days += (rtc_f9.reg[7]*10 + rtc_f9.reg[6]);
			if ( days > 0 )
			{
				while ( days > (temp_days = S9xRTCDaysInMonth( month, year )) )
				{
					days -= temp_days;
					month += 1;
					if ( month > 12 )
					{
						year += 1;
						month = 1;
					}
				}
			}

			year_tens = year % 100;
			year_ones = year_tens % 10;
			year_tens /= 10;
			year_hundreds = (year - 1000) / 100;

			rtc_f9.reg[0] = seconds % 10;
			rtc_f9.reg[1] = seconds / 10;
			rtc_f9.reg[2] = minutes % 10;
			rtc_f9.reg[3] = minutes / 10;
			rtc_f9.reg[4] = hours % 10;
			rtc_f9.reg[5] = hours / 10;
			rtc_f9.reg[6] = days % 10;
			rtc_f9.reg[7] = days / 10;
			rtc_f9.reg[8] = month%10;
			rtc_f9.reg[9] = month /10;
			rtc_f9.reg[10] = year_ones;
			rtc_f9.reg[11] = year_tens;
			rtc_f9.reg[12] %= 7;
			return;
        }
    }
}
extern "C" {
/* 0x00182910 -- use the standalone ROM pointer, not Memory.ROM. */
uint8* Get7110BasePtr(uint32 Address)
{
	uint32 i;
	switch((Address&0x00F00000)>>16)
	{
	case 0xD0:
		i=s7r.reg4831*0x100000;
		break;
	case 0xE0:
		i=s7r.reg4832*0x100000;
		break;
	case 0xF0:
		i=s7r.reg4833*0x100000;
		break;
	default:i=0;
	}
	i+=Address&0x000F0000;
//not that anything DMAs from ROM, but...
//	i+=s7r.DataRomOffset;
	return &ROM[i];
}
}
/* 0x001832a4 -- preserve original register defaults and bank50 clearing. */
void S9xSpc7110Reset()
{
	s7r.reg4800=0;
	s7r.reg4801=0;
	s7r.reg4802=0;
	s7r.reg4803=0;
	s7r.reg4804=0;
	s7r.reg4805=0;
	s7r.reg4806=0;
	s7r.reg4807=0;
	s7r.reg4808=0;
	s7r.reg4809=0;
	s7r.reg480A=0;
	s7r.reg480B=0;
	s7r.reg480C=0;
	s7r.reg4811=0;
	s7r.reg4812=0;
	s7r.reg4813=0;
	s7r.reg4814=0;
	s7r.reg4815=0;
	s7r.reg4816=0;
	s7r.reg4817=0;
	s7r.reg4818=0;
	s7r.reg4820=0;
	s7r.reg4821=0;
	s7r.reg4822=0;
	s7r.reg4823=0;
	s7r.reg4824=0;
	s7r.reg4825=0;
	s7r.reg4826=0;
	s7r.reg4827=0;
	s7r.reg4828=0;
	s7r.reg4829=0;
	s7r.reg482A=0;
	s7r.reg482B=0;
	s7r.reg482C=0;
	s7r.reg482D=0;
	s7r.reg482E=0;
	s7r.reg482F=0;
	s7r.reg4830=0;
	s7r.reg4831=0;
	s7r.reg4832=1;
	s7r.reg4833=2;
	s7r.reg4834=0;
	s7r.reg4840=0;
	s7r.reg4841=0;
	s7r.reg4842=0;
	s7r.written=0;
	s7r.offset_add=0;
	s7r.AlignBy=1;
	s7r.bank50Internal=0;
	memset(s7r.bank50,0x00,DECOMP_BUFFER_SIZE);
}


