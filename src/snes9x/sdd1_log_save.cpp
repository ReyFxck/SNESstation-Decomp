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

/* Historical Snes9x 1.41-1 S-DD1 log persistence with target PS2 FIO.
 * Archive SHA-256: 5e8b72c88c889464746e2f2f10449b9b324451c095a343081057f8f7ceb8378b.
 * The target stores the raw byte count from fioRead in both record counters.
 */
typedef unsigned char uint8;
typedef unsigned int uint32;
struct HistoricalSdd1LogMemory {
    uint8 reserved[0xb070];
    uint32 SDD1LoggedDataCountPrev, SDD1LoggedDataCount;
    uint8 SDD1LoggedData[0x10000];
};
extern HistoricalSdd1LogMemory Memory __asm__("g_p12_memory");
extern const char log_extension[] __asm__("g_p12_sdd1_dat_extension");
extern "C" char *S9xGetFilename(const char *) __asm__("snes_p12_get_filename");
extern "C" int S9xCompareSDD1LoggedDataEntries(const void *, const void *) __asm__("snes_p12_compare_sdd1_entries");
extern "C" void qsort(void *, uint32, uint32, int (*)(const void *, const void *)) __asm__("snes_qsort_001080cc");
extern "C" int fioOpen(const char *, int);
extern "C" int fioClose(int);
extern "C" int fioRead(int, void *, int);
extern "C" int fioWrite(int, const void *, int);
/* 0x0016fb04 -- original dirty counter, eight-byte sort and PS2 file rewrite. */
void S9xSDD1SaveLoggedData ()
{
    int fd;
    if (Memory.SDD1LoggedDataCount != Memory.SDD1LoggedDataCountPrev)
    {
        qsort (Memory.SDD1LoggedData, Memory.SDD1LoggedDataCount, 8,
               S9xCompareSDD1LoggedDataEntries);
        fd = fioOpen (S9xGetFilename (log_extension), 0x202);
        if (fd >= 0)
        {
            (void) fioWrite (fd, Memory.SDD1LoggedData,
                            (int) (Memory.SDD1LoggedDataCount << 3));
            (void) fioClose (fd);
        }
        Memory.SDD1LoggedDataCountPrev = Memory.SDD1LoggedDataCount;
    }
}
