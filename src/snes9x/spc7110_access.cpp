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
/* Original SPC7110 register and ROM access from Snes9x 1.41-1, with the
 * V72-proved 32-bit/disabled clock adaptation. Archive SHA-256:
 * 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * All state aliases retain the original target objects and pointer slots.
 * The complete readable switch bodies and native arithmetic are retained.
 */


//#define SPC7110_DEBUG

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char bool8;
typedef int int32;
#define DECOMP_BUFFER_SIZE 0x10000
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

class CMemory {public:void SPC7110Sram(uint8);};
extern CMemory Memory __asm__("DAT_0034e2b0");
struct HistoricalSpcSettings {uint8 prefix[0x66];bool8 SPC7110RTC;};
extern HistoricalSpcSettings Settings __asm__("DAT_003454e0");
extern void (*Copy7110)(void) __asm__("DAT_004134f4");
void S9xUpdateRTC();
extern "C" {
/* 0x001813f0 -- historical S9xGetSPC7110. */
uint8 S9xGetSPC7110(uint16 Address)
{
#ifdef SPC7110_DEBUG
	printf("%04X read\n", Address);
#endif
	switch (Address)
	{
		//decompressed data read port. decrements 4809-A (with wrap)
		//4805-6 is the offset into the bank
		//AlignBy is set (afaik) at decompression time, and is the offset multiplier
		//bank50internal is an internal pointer to the actual byte to read.
		//so you read from offset*multiplier + bank50internal
		//the offset registers cannot be incremented due to the offset multiplier.
	case 0x4800:
		{
			unsigned short count=s7r.reg4809|(s7r.reg480A<<8);
			uint32 i, j;
			j=(s7r.reg4805|(s7r.reg4806<<8));
			j*=s7r.AlignBy;
			i=j;
			if(count >0)
				count--;
			else count = 0xFFFF;
			s7r.reg4809=0x00ff&count;
			s7r.reg480A=(0xff00&count)>>8;
			i+=s7r.bank50Internal;
			i%=DECOMP_BUFFER_SIZE;
			s7r.reg4800=s7r.bank50[i];

			s7r.bank50Internal++;
			s7r.bank50Internal%=DECOMP_BUFFER_SIZE;
#ifdef SPC7110_DEBUG
			printf("Returned %02X\n", s7r.reg4800);
#endif
		}
		return s7r.reg4800;
	//table register low
	case 0x4801: return s7r.reg4801;
	//table register middle
	case 0x4802: return s7r.reg4802;
	//table register high
	case 0x4803: return s7r.reg4803;
	//index of pointer in table (each entry is 4 bytes)
	case 0x4804: return s7r.reg4804;
	//offset register low
	case 0x4805: return s7r.reg4805;
	//offset register high
	case 0x4806: return s7r.reg4806;
	//DMA channel (not that I see this usually set,
	//regardless of what channel DMA is on)
	case 0x4807: return s7r.reg4807;
	//C r/w option, unknown, defval:00 is what Dark Force says
	//afaict, Snes9x doesn't use this at all.
	case 0x4808: return s7r.reg4808;
	//C-Length low
	//counts down the number of bytes left to read from the decompression buffer.
	//this is set by the ROM, and wraps on bounds.
	case 0x4809: return s7r.reg4809;
	//C Length high
	case 0x480A: return s7r.reg480A;
	//Offset enable.
	//if this is zero, 4805-6 are useless. Emulated by setting AlignBy to 0
	case 0x480B:
		return s7r.reg480B;
	//decompression finished: just emulated by switching each read.
	case 0x480C:
		s7r.reg480C^=0x80;
		return s7r.reg480C^0x80;

	//Data access port
	//reads from the data ROM (anywhere over the first 8 mbits
	//behavior is complex, will document later,
	//possibly missing cases, because of the number of switches in play
	case 0x4810:
		if(s7r.written==0)
			return 0;
		if((s7r.written&0x07)==0x07)
		{
			uint32 i=(s7r.reg4813<<16)|(s7r.reg4812<<8)|s7r.reg4811;
			i%=s7r.DataRomSize;
			if(s7r.reg4818&0x02)
			{
				if(s7r.reg4818&0x08)
				{
					signed short r4814;
					r4814=(s7r.reg4815<<8)|s7r.reg4814;
					i+=r4814;
					r4814++;
					s7r.reg4815=(uint8)(r4814>>8);
					s7r.reg4814=(uint8)(r4814&0x00FF);
				}
				else
				{
					unsigned short r4814;
					r4814=(s7r.reg4815<<8)|s7r.reg4814;
					i+=r4814;
					if(r4814!=0xFFFF)
						r4814++;
					else r4814=0;
					s7r.reg4815=(uint8)(r4814>>8);
					s7r.reg4814=(uint8)(r4814&0x00FF);

				}
			}
			i+=s7r.DataRomOffset;
			uint8 tmp=ROM[i];
			i=(s7r.reg4813<<16)|(s7r.reg4812<<8)|s7r.reg4811;

			if(s7r.reg4818&0x02)
			{
			}
			else if(s7r.reg4818&0x01)
			{
				if(s7r.reg4818&0x04)
				{
					signed short inc;
					inc=(s7r.reg4817<<8)|s7r.reg4816;

					if(!(s7r.reg4818&0x10))
						i+=inc;
					else
					{
						if(s7r.reg4818&0x08)
						{
							signed short r4814;
							r4814=(s7r.reg4815<<8)|s7r.reg4814;
							r4814+=inc;
							s7r.reg4815=(r4814&0xFF00)>>8;
							s7r.reg4814=r4814&0xFF;
						}
						else
						{
							unsigned short r4814;
							r4814=(s7r.reg4815<<8)|s7r.reg4814;
							r4814+=inc;
							s7r.reg4815=(r4814&0xFF00)>>8;
							s7r.reg4814=r4814&0xFF;

						}
					}
					//is signed
				}
				else
				{
					uint16 inc;
					inc=(s7r.reg4817<<8)|s7r.reg4816;
					if(!(s7r.reg4818&0x10))
						i+=inc;
					else
					{
						if(s7r.reg4818&0x08)
						{
							signed short r4814;
							r4814=(s7r.reg4815<<8)|s7r.reg4814;
							r4814+=inc;
							s7r.reg4815=(r4814&0xFF00)>>8;
							s7r.reg4814=r4814&0xFF;
						}
						else
						{
							unsigned short r4814;
							r4814=(s7r.reg4815<<8)|s7r.reg4814;
							r4814+=inc;
							s7r.reg4815=(r4814&0xFF00)>>8;
							s7r.reg4814=r4814&0xFF;

						}
					}
				}
			}
			else
			{
				if(!(s7r.reg4818&0x10))
					i+=1;
				else
				{
					if(s7r.reg4818&0x08)
					{
						signed short r4814;
						r4814=(s7r.reg4815<<8)|s7r.reg4814;
						r4814+=1;
						s7r.reg4815=(r4814&0xFF00)>>8;
						s7r.reg4814=r4814&0xFF;
					}
					else
					{
						unsigned short r4814;
						r4814=(s7r.reg4815<<8)|s7r.reg4814;
						r4814+=1;
						s7r.reg4815=(r4814&0xFF00)>>8;
						s7r.reg4814=r4814&0xFF;

					}
				}
			}

#ifdef SPC7110_DEBUG
			printf("Returned %02X\n", tmp);
#endif

			i%=s7r.DataRomSize;
			s7r.reg4811=i&0x00FF;
			s7r.reg4812=(i&0x00FF00)>>8;
			s7r.reg4813=((i&0xFF0000)>>16);
			return tmp;
		}
		else return 0;
	//direct read address low
	case 0x4811: return s7r.reg4811;
	//direct read address middle
	case 0x4812: return s7r.reg4812;
	//direct read access high
	case 0x4813: return s7r.reg4813;
	//read adjust low
	case 0x4814: return s7r.reg4814;
	//read adjust high
	case 0x4815: return s7r.reg4815;
	//read increment low
	case 0x4816: return s7r.reg4816;
	//read increment high
	case 0x4817: return s7r.reg4817;
	//Data ROM command mode
	//essentially, this controls the insane code of $4810 and $481A
	case 0x4818: return s7r.reg4818;
	//read after adjust port
	//what this does, besides more nasty stuff like 4810,
	//I don't know. Just assume it is a different implementation of $4810,
	//if it helps your sanity
	case 0x481A:
		if(s7r.written==0x1F)
		{
			uint32 i=((s7r.reg4813<<16)|(s7r.reg4812<<8)|s7r.reg4811);
			if(s7r.reg4818&0x08)
			{
				short adj;
				adj=((short)(s7r.reg4815<<8))|s7r.reg4814;
				i+=adj;
			}
			else
			{
				uint16 adj;
				adj=(s7r.reg4815<<8)|s7r.reg4814;
				i+=adj;
			}

			i%=s7r.DataRomSize;
			i+=s7r.DataRomOffset;
			uint8 tmp=ROM[i];
			i=((s7r.reg4813<<16)|(s7r.reg4812<<8)|s7r.reg4811);
			if(0x60==(s7r.reg4818&0x60))
			{
				i=((s7r.reg4813<<16)|(s7r.reg4812<<8)|s7r.reg4811);

				if(!(s7r.reg4818&0x10))
				{
					if(s7r.reg4818&0x08)
					{
						short adj;
						adj=((short)(s7r.reg4815<<8))|s7r.reg4814;
						i+=adj;
					}
					else
					{
						uint16 adj;
						adj=(s7r.reg4815<<8)|s7r.reg4814;
						i+=adj;
					}
					i%=s7r.DataRomSize;
					s7r.reg4811=i&0x00FF;
					s7r.reg4812=(i&0x00FF00)>>8;
					s7r.reg4813=((i&0xFF0000)>>16);
				}
				else
				{
					if(s7r.reg4818&0x08)
					{
						short adj;
						adj=((short)(s7r.reg4815<<8))|s7r.reg4814;
						adj+=adj;
						s7r.reg4815=(adj&0xFF00)>>8;
						s7r.reg4814=adj&0xFF;
					}
					else
					{
						uint16 adj;
						adj=(s7r.reg4815<<8)|s7r.reg4814;
						adj+=adj;
						s7r.reg4815=(adj&0xFF00)>>8;
						s7r.reg4814=adj&0xFF;
					}
				}
			}
#ifdef SPC7110_DEBUG
			printf("Returned %02X\n", tmp);
#endif
			return tmp;
		}
		else return 0;

	//multiplicand low or dividend lowest
	case 0x4820: return s7r.reg4820;
	//multiplicand high or divdend lower
	case 0x4821: return s7r.reg4821;
	//dividend higher
	case 0x4822: return s7r.reg4822;
	//dividend highest
	case 0x4823: return s7r.reg4823;
	//multiplier low
	case 0x4824: return s7r.reg4824;
	//multiplier high
	case 0x4825: return s7r.reg4825;
	//divisor low
	case 0x4826: return s7r.reg4826;
	//divisor high
	case 0x4827: return s7r.reg4827;

	//result lowest
	case 0x4828:
		return s7r.reg4828;
	//result lower
	case 0x4829:
		return s7r.reg4829;
	//result higher
	case 0x482A:
		return s7r.reg482A;
	//result highest
	case 0x482B:
		return s7r.reg482B;
	//remainder (division) low
	case 0x482C: return s7r.reg482C;
	//remainder (division) high
	case 0x482D: return s7r.reg482D;
	//signed/unsigned
	case 0x482E: return s7r.reg482E;
	//finished flag, emulated as an on-read toggle.
	case 0x482F:
		if(s7r.reg482F)
		{
			s7r.reg482F=0;
			return 0x80;
		}
		return 0;
		break;

	//SRAM toggle
	case 0x4830:
		return s7r.reg4830;
	//DX bank mapping
	case 0x4831:
		return s7r.reg4831;
	//EX bank mapping
	case 0x4832:
		return s7r.reg4832;
	//FX bank mapping
	case 0x4833:
		return s7r.reg4833;
	//SRAM mapping? We have no clue!
	case 0x4834:
		return s7r.reg4834;
//RTC enable
	case 0x4840:
		if(!Settings.SPC7110RTC)
			return Address>>8;
		return s7r.reg4840;
//command/index/value of RTC (essentially, zero unless we're in read mode
	case 0x4841:
		if(!Settings.SPC7110RTC)
			return Address>>8;
		if(rtc_f9.init)
		{
			S9xUpdateRTC();
			uint8 tmp=rtc_f9.reg[rtc_f9.index];
			rtc_f9.index++;
			rtc_f9.index%=0x10;
#ifdef SPC7110_DEBUG
			printf("$4841 returned %02X\n", tmp);
#endif
			return tmp;
		}
		else return 0;
//RTC done flag
	case 0x4842:
		if(!Settings.SPC7110RTC)
			return Address>>8;
		s7r.reg4842^=0x80;
		return s7r.reg4842^0x80;
	default:
#ifdef SPC7110_DEBUG
		printf("Access to Reg %04X\n", Address);
#endif
		return 0x00;
	}
}
}
/* 0x00181bac -- historical S9xSetSPC7110. */
void S9xSetSPC7110 (uint8 data, uint16 Address)
{
#ifdef SPC7110_DEBUG
	printf("%04X written to, value %02X\n", Address, data);
#endif
	switch(Address)
	{
//Writes to $4800 are undefined.

	//table low, middle, and high.
	case 0x4801:
		s7r.reg4801=data;
		break;
	case 0x4802:
		s7r.reg4802=data;
		break;
	case 0x4803:
		s7r.reg4803=data;
		break;

	//table index (4 byte entries, bigendian with a multiplier byte)
	case 0x4804:
		s7r.reg4804=data;
		break;

	//offset low
	case 0x4805:
		s7r.reg4805=data;
		break;

	//offset high, starts decompression
	case 0x4806:
		s7r.reg4806=data;
		(*Copy7110)();
		s7r.bank50Internal=0;
		s7r.reg480C&=0x7F;
		break;

	//DMA channel register (Is it used??)
	case 0x4807:
		s7r.reg4807=data;
		break;

	//C r/w? I have no idea. If you get weird values written here before a bug,
	//The Dumper should probably be contacted about running a test.
	case 0x4808:
		s7r.reg4808=data;
		break;

	//C-Length low
	case 0x4809:
		s7r.reg4809=data;
		break;
	//C-Length high
	case 0x480A:
		s7r.reg480A=data;
		break;

	//Offset enable
	case 0x480B:
		{
			s7r.reg480B=data;
			int table=(s7r.reg4803<<16)|(s7r.reg4802<<8)|s7r.reg4801;

			int j= 4*s7r.reg4804;
			j+=s7r.DataRomOffset;
			j+=table;

			if(s7r.reg480B==0)
				s7r.AlignBy=0;
			else
			{
				switch(ROM[j])
				{
				case 0x03:
					s7r.AlignBy=8;
					break;
				case 0x01:
					s7r.AlignBy=2;
					break;
				case 0x02:
					s7r.AlignBy=4;
					break;
				case 0x00:
				default:
					s7r.AlignBy=1;
					break;
				}
			}
//			s7r.decomp_set=true;
		}
		break;
//$4810 is probably read only.

	//Data port address low
	case 0x4811:
		s7r.reg4811=data;
		s7r.written|=0x01;
		break;

	//data port address middle
	case 0x4812:
		s7r.reg4812=data;
		s7r.written|=0x02;
		break;

	//data port address high
	case 0x4813:
		s7r.reg4813=data;
		s7r.written|=0x04;
		break;

	//data port adjust low (has a funky immediate increment mode)
	case 0x4814:
		s7r.reg4814=data;
		if(s7r.reg4818&0x02)
		{
			if((s7r.reg4818&0x20)&&!(s7r.reg4818&0x40))
			{
				s7r.offset_add|=0x01;
				if(s7r.offset_add==3)
				{
					if(s7r.reg4818&0x10)
					{
					}
					else
					{
						uint32 i=(s7r.reg4813<<16)|(s7r.reg4812<<8)|s7r.reg4811;
						if(s7r.reg4818&0x08)
						{
							i+=(signed char)s7r.reg4814;
						}
						else
						{
							i+=s7r.reg4814;
						}
						i%=s7r.DataRomSize;
						s7r.reg4811=i&0x00FF;
						s7r.reg4812=(i&0x00FF00)>>8;
						s7r.reg4813=((i&0xFF0000)>>16);
					}
				}
			}
			else if((s7r.reg4818&0x40)&&!(s7r.reg4818&0x20))
			{
				s7r.offset_add|=0x01;
				if(s7r.offset_add==3)
				{
					if(s7r.reg4818&0x10)
					{
					}
					else
					{
						uint32 i=(s7r.reg4813<<16)|(s7r.reg4812<<8)|s7r.reg4811;
						if(s7r.reg4818&0x08)
						{
							short adj;
							adj=((short)(s7r.reg4815<<8))|s7r.reg4814;
							i+=adj;
						}
						else
						{
							uint16 adj;
							adj=(s7r.reg4815<<8)|s7r.reg4814;
							i+=adj;
						}
						i%=s7r.DataRomSize;
						s7r.reg4811=i&0x00FF;
						s7r.reg4812=(i&0x00FF00)>>8;
						s7r.reg4813=((i&0xFF0000)>>16);
					}
				}

			}
		}

		s7r.written|=0x08;
		break;

	//data port adjust high (has a funky immediate increment mode)
	case 0x4815:
		s7r.reg4815=data;
		if(s7r.reg4818&0x02)
		{
			if(s7r.reg4818&0x20&&!(s7r.reg4818&0x40))
			{
				s7r.offset_add|=0x02;
				if(s7r.offset_add==3)
				{
					if(s7r.reg4818&0x10)
					{
					}
					else
					{
						uint32 i=(s7r.reg4813<<16)|(s7r.reg4812<<8)|s7r.reg4811;

						if(s7r.reg4818&0x08)
						{
							i+=(signed char)s7r.reg4814;
						}
						else
						{
							i+=s7r.reg4814;
						}
						i%=s7r.DataRomSize;
						s7r.reg4811=i&0x00FF;
						s7r.reg4812=(i&0x00FF00)>>8;
						s7r.reg4813=((i&0xFF0000)>>16);
					}
				}
			}
			else if(s7r.reg4818&0x40&&!(s7r.reg4818&0x20))
			{
				s7r.offset_add|=0x02;
				if(s7r.offset_add==3)
				{
					if(s7r.reg4818&0x10)
					{
					}
					else
					{
						uint32 i=(s7r.reg4813<<16)|(s7r.reg4812<<8)|s7r.reg4811;
						if(s7r.reg4818&0x08)
						{
							short adj;
							adj=((short)(s7r.reg4815<<8))|s7r.reg4814;
							i+=adj;
						}
						else
						{
							uint16 adj;
							adj=(s7r.reg4815<<8)|s7r.reg4814;
							i+=adj;
						}
						i%=s7r.DataRomSize;
						s7r.reg4811=i&0x00FF;
						s7r.reg4812=(i&0x00FF00)>>8;
						s7r.reg4813=((i&0xFF0000)>>16);
					}
				}
			}
		}
		s7r.written|=0x10;
		break;
	//data port increment low
	case 0x4816:
		s7r.reg4816=data;
		break;
	//data port increment high
	case 0x4817:
		s7r.reg4817=data;
		break;

	//data port mode switches
	//note that it starts inactive.
	case 0x4818:
		if((s7r.written&0x18)!=0x18)
			break;
		s7r.offset_add=0;
		s7r.reg4818=data;
		break;

	//multiplicand low or dividend lowest
	case 0x4820:
		s7r.reg4820=data;
		break;
	//multiplicand high or dividend lower
	case 0x4821:
		s7r.reg4821=data;
		break;
	//dividend higher
	case 0x4822:
		s7r.reg4822=data;
		break;
	//dividend highest
	case 0x4823:
		s7r.reg4823=data;
		break;
	//multiplier low
	case 0x4824:
		s7r.reg4824=data;
		break;
	//multiplier high (triggers operation)
	case 0x4825:
		s7r.reg4825=data;
		if(s7r.reg482E&0x01)
		{
			int mul;
			short m1=(short)((s7r.reg4824)|(s7r.reg4825<<8));
			short m2=(short)((s7r.reg4820)|(s7r.reg4821<<8));

			mul=m1*m2;
			s7r.reg4828=(uint8)(mul&0x000000FF);
			s7r.reg4829=(uint8)((mul&0x0000FF00)>>8);
			s7r.reg482A=(uint8)((mul&0x00FF0000)>>16);
			s7r.reg482B=(uint8)((mul&0xFF000000)>>24);
		}
		else
		{
			uint32 mul;
			uint16 m1=(uint16)((s7r.reg4824)|(s7r.reg4825<<8));
			uint16 m2=(uint16)((s7r.reg4820)|(s7r.reg4821<<8));

			mul=m1*m2;
			s7r.reg4828=(uint8)(mul&0x000000FF);
			s7r.reg4829=(uint8)((mul&0x0000FF00)>>8);
			s7r.reg482A=(uint8)((mul&0x00FF0000)>>16);
			s7r.reg482B=(uint8)((mul&0xFF000000)>>24);
		}
		s7r.reg482F=0x80;
		break;
	//divisor low
	case 0x4826:
		s7r.reg4826=data;
		break;
	//divisor high (triggers operation)
	case 0x4827:
		s7r.reg4827=data;
		if(s7r.reg482E&0x01)
		{
			int quotient;
			short remainder;
			int dividend=(int)(s7r.reg4820|(s7r.reg4821<<8)|(s7r.reg4822<<16)|(s7r.reg4823<<24));
			short divisor=(short)(s7r.reg4826|(s7r.reg4827<<8));
			if(divisor != 0)
			{
				quotient=(int)(dividend/divisor);
				remainder=(short)(dividend%divisor);
			}
			else
			{
				quotient=0;
				remainder=dividend&0x0000FFFF;
			}
			s7r.reg4828=(uint8)(quotient&0x000000FF);
			s7r.reg4829=(uint8)((quotient&0x0000FF00)>>8);
			s7r.reg482A=(uint8)((quotient&0x00FF0000)>>16);
			s7r.reg482B=(uint8)((quotient&0xFF000000)>>24);
			s7r.reg482C=(uint8)remainder&0x00FF;
			s7r.reg482D=(uint8)((remainder&0xFF00)>>8);
		}
		else
		{
			uint32 quotient;
			uint16 remainder;
			uint32 dividend=(uint32)(s7r.reg4820|(s7r.reg4821<<8)|(s7r.reg4822<<16)|(s7r.reg4823<<24));
			uint16 divisor=(uint16)(s7r.reg4826|(s7r.reg4827<<8));
			if(divisor != 0)
			{
				quotient=(uint32)(dividend/divisor);
				remainder=(uint16)(dividend%divisor);
			}
			else
			{
				quotient=0;
				remainder=dividend&0x0000FFFF;
			}
			s7r.reg4828=(uint8)(quotient&0x000000FF);
			s7r.reg4829=(uint8)((quotient&0x0000FF00)>>8);
			s7r.reg482A=(uint8)((quotient&0x00FF0000)>>16);
			s7r.reg482B=(uint8)((quotient&0xFF000000)>>24);
			s7r.reg482C=(uint8)remainder&0x00FF;
			s7r.reg482D=(uint8)((remainder&0xFF00)>>8);
		}
		s7r.reg482F=0x80;
		break;
	//result registers are possibly read-only

	//reset: writes here nuke the whole math unit
	//Zero indicates unsigned math, resets with non-zero values turn on signed math
	case 0x482E:
		s7r.reg4820=s7r.reg4821=s7r.reg4822=s7r.reg4823=s7r.reg4824=s7r.reg4825=s7r.reg4826=s7r.reg4827=s7r.reg4828=s7r.reg4829=s7r.reg482A=s7r.reg482B=s7r.reg482C=s7r.reg482D=0;
		s7r.reg482E=data;
		break;

	//math status register possibly read only

	//SRAM toggle
	case 0x4830:
		Memory.SPC7110Sram(data);
		s7r.reg4830=data;
		break;
	//Bank DX mapping
	case 0x4831:
		s7r.reg4831=data;
		break;
	//Bank EX mapping
	case 0x4832:
		s7r.reg4832=data;
		break;
	//Bank FX mapping
	case 0x4833:
		s7r.reg4833=data;
		break;
	//S-RAM mapping? who knows?
	case 0x4834:
		s7r.reg4834=data;
		break;
	//RTC Toggle
	case 0x4840:
		if(0==data)
		{
			S9xUpdateRTC();
			//	rtc_f9.init=false;
			//	rtc_f9.index=-1;
		}
		if(data&0x01)
		{
			s7r.reg4842=0x80;
			//rtc_f9.last_used=time(NULL);//????
			rtc_f9.init=false;
			rtc_f9.index=-1;
		}
		s7r.reg4840=data;
		break;
	//RTC init/command/index register
	case 0x4841:
		if(rtc_f9.init)
		{
			if(-1==rtc_f9.index)
			{
				rtc_f9.index=data&0x0F;
				break;
			}
			if(rtc_f9.control==0x0C)
			{
				rtc_f9.index=data&0x0F;
				s7r.reg4842=0x80;
				rtc_f9.last_used=0;
			}
			else
			{

				if(0x0D==rtc_f9.index)
				{
					if(data&0x08)
					{
						if(rtc_f9.reg[1]<3)
						{
							S9xUpdateRTC();
							rtc_f9.reg[0]=0;
							rtc_f9.reg[1]=0;
							rtc_f9.last_used=0;
						}
						else
						{
							S9xUpdateRTC();
							rtc_f9.reg[0]=0;
							rtc_f9.reg[1]=0;
							rtc_f9.last_used=0-60;
							S9xUpdateRTC();
							rtc_f9.last_used=0;
						}
						data&=0x07;
					}
					if(rtc_f9.reg[0x0D]&0x01)
					{
						if(!(data%2))
						{
							rtc_f9.reg[rtc_f9.index&0x0F]=data;
							rtc_f9.last_used=0-1;
							S9xUpdateRTC();
							rtc_f9.last_used=0;
						}
					}
				}
				if(0x0F==rtc_f9.index)
				{
					if(data&0x01&&!(rtc_f9.reg[0x0F]&0x01))
					{
						S9xUpdateRTC();
						rtc_f9.reg[0]=0;
						rtc_f9.reg[1]=0;
						rtc_f9.last_used=0;
					}
					if(data&0x02&&!(rtc_f9.reg[0x0F]&0x02))
					{
						S9xUpdateRTC();
						rtc_f9.last_used=0;
					}
				}
				rtc_f9.reg[rtc_f9.index&0x0F]=data;
				s7r.reg4842=0x80;
				rtc_f9.index=(rtc_f9.index+1)%0x10;
			}
		}
		else
		{
			if(data==0x03||data==0x0C)
			{
				rtc_f9.init=true;
				rtc_f9.control=data;
				rtc_f9.index=-1;
			}
		}
		break;
	//writes to RTC status register aren't expected to be meaningful
	default:
		Address-=0x4800;
		break;
		//16 BIT MULTIPLIER: ($FF00) high byte, defval:00
	}
}
extern "C"{
//emulate the SPC7110's ability to remap banks Dx, Ex, and Fx.
/* 0x0018255c -- historical S9xGetSPC7110Byte. */
uint8 S9xGetSPC7110Byte(uint32 Address)
{
	uint32 i;
	switch((Address&0x00F00000)>>16)
	{
	case 0xD0:
		i=s7r.reg4831*0x00100000;
		break;
	case 0xE0:
		i=s7r.reg4832*0x00100000;
		break;
	case 0xF0:
		i=s7r.reg4833*0x00100000;
		break;
	default:i=0;
	}
	i+=Address&0x000FFFFF;
	i+=s7r.DataRomOffset;
	return ROM[i];
}
}
