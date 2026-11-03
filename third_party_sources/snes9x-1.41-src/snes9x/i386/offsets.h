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
#define Flags CPU + 0
#define BranchSkip CPU + 4
#define NMIActive CPU + 5
#define IRQActive CPU + 6
#define WaitingForInterrupt CPU + 7
#define InDMA CPU + 8
#define WhichEvent CPU + 9
#define PCS CPU + 12
#define PCBase CPU + 16
#define PCAtOpcodeStart CPU + 20
#define WaitAddress CPU + 24
#define WaitCounter CPU + 28
#define Cycles CPU + 32
#define NextEvent CPU + 36
#define V_Counter CPU + 40
#define MemSpeed CPU + 44
#define MemSpeedx2 CPU + 48
#define FastROMSpeed CPU + 52
#define AutoSaveTimer CPU + 56
#define SRAMModified CPU + 60
#define NMITriggerPoint CPU + 64
#define TriedInterleavedMode2 CPU + 69
#define BRKTriggered CPU + 68
#define NMICycleCount CPU + 72
#define IRQCycleCount CPU + 76
#define PB Registers + 0
#define DB Registers + 1
#define PP Registers + 2
#define PL Registers + 2
#define PH PL + 1
#define AA Registers + 4
#define AL Registers + 4
#define AH AL + 1
#define DD Registers + 6
#define DL Registers + 6
#define DH DL + 1
#define SS Registers + 8
#define SL Registers + 8
#define SH SL + 1
#define XX Registers + 10
#define XL Registers + 10
#define XH XL + 1
#define YY Registers + 12
#define YL Registers + 12
#define YH YL + 1
#define PCR Registers + 14
#define RAM Memory + 0
#define ROM Memory + 4
#define VRAM Memory + 8
#define SRAM Memory + 12
#define BWRAM Memory + 16
#define FillRAM Memory + 20
#define C4RAM Memory + 24
#define HiROM Memory + 28
#define LoROM Memory + 29
#define SRAMMask Memory + 32
#define SRAMSize Memory + 36
#define Map Memory + 40
#define WriteMap Memory + 16424
#define MemorySpeed Memory + 32808
#define BlockIsRAM Memory + 36904
#define BlockIsROM Memory + 41000
#define ROMFilename Memory + 53368
#define APUPCS IAPU + 0
#define APURAM IAPU + 4
#define APUExecuting IAPU + 12
#define APUDirectPage IAPU + 8
#define APUBit IAPU + 13
#define APUAddress IAPU + 16
#define APUWaitAddress1 IAPU + 20
#define APUWaitAddress2 IAPU + 24
#define APUWaitCounter IAPU + 28
#define APUShadowRAM IAPU + 32
#define APUCachedSamples IAPU + 36
#define APU_Carry IAPU + 40
#define APU_Zero IAPU + 41
#define APU_Overflow IAPU + 42
#define APUTimerErrorCounter IAPU + 44
#define APUCycles APU + 0
#define APUShowROM APU + 4
#define APUFlags APU + 5
#define APUKeyedChannels APU + 6
#define APUOutPorts APU + 7
#define APUDSP APU + 11
#define APUExtraRAM APU + 139
#define APUTimer APU + 204
#define APUTimerTarget APU + 210
#define APUTimerEnabled APU + 216
#define TimerValueWritten APU + 219
#define CPUSpeed ICPU + 0
#define CPUOpcodes ICPU + 4
#define _Carry ICPU + 8
#define _Zero ICPU + 9
#define _Negative ICPU + 10
#define _Overflow ICPU + 11
#define ShiftedDB ICPU + 20
#define ShiftedPB ICPU + 16
#define CPUExecuting ICPU + 12
#define Scanline ICPU + 28
#define Frame ICPU + 24
#define APUEnabled Settings + 0
#define Shutdown Settings + 1
#define SoundSkipMethod Settings + 2
#define H_Max Settings + 4
#define HBlankStart Settings + 8
#define CyclesPercentage Settings + 12
#define DisableIRQ Settings + 16
#define Paused Settings + 17
#define PAL Settings + 29
#define SoundSync Settings + 108
#define SA1Enabled Settings + 82
#define SuperFXEnabled Settings + 80
#define ApuP APURegisters + 0
#define ApuYA APURegisters + 2
#define ApuA APURegisters + 2
#define ApuY APURegisters + 3
#define ApuX APURegisters + 4
#define ApuS APURegisters + 5
#define ApuPC APURegisters + 6
#define APUPCR APURegisters + 6
#define BGMode PPU + 0
#define BG3Priority PPU + 1
#define Brightness PPU + 2
#define GHight PPU + 4
#define GInc PPU + 5
#define GAddress PPU + 6
#define GMask1 PPU + 8
#define GFullGraphicCount PPU + 10
#define GShift PPU + 12
#define CGFLIP PPU + 62
#define CGDATA PPU + 64
#define FirstSprite PPU + 576
#define LastSprite PPU + 577
#define OBJ PPU + 578
#define OAMPriorityRotation PPU + 2114
#define OAMAddr PPU + 2116
#define OAMFlip PPU + 2119
#define OAMTileAddress PPU + 2120
#define IRQVBeamPos PPU + 2122
#define IRQHBeamPos PPU + 2124
#define VBeamPosLatched PPU + 2126
#define HBeamPosLatched PPU + 2128
#define HBeamFlip PPU + 2130
#define VBeamFlip PPU + 2131
#define HVBeamCounterLatched PPU + 2132
#define MatrixA PPU + 2134
#define MatrixB PPU + 2136
#define MatrixC PPU + 2138
#define MatrixD PPU + 2140
#define CentreX PPU + 2142
#define CentreY PPU + 2144
#define Joypad1ButtonReadPos PPU + 2146
#define Joypad2ButtonReadPos PPU + 2147
#define CGADD PPU + 2148
#define FixedColourGreen PPU + 2150
#define FixedColourRed PPU + 2149
#define FixedColourBlue PPU + 2151
#define SavedOAMAddr PPU + 2152
#define ScreenHeight PPU + 2154
#define WRAM PPU + 2156
#define BG_Forced PPU + 2160
#define ForcedBlanking PPU + 2161
#define OBJThroughMain PPU + 2162
#define OBJThroughSub PPU + 2163
#define OBJSizeSelect PPU + 2164
#define OBJNameBase PPU + 2166
#define OAMReadFlip PPU + 2169
#define OAMData PPU + 2170
#define VTimerEnabled PPU + 2714
#define HTimerEnabled PPU + 2715
#define HTimerPosition PPU + 2716
#define Mosaic PPU + 2718
#define BGMosaic PPU + 2719
#define Mode7HFlip PPU + 2723
#define Mode7VFlip PPU + 2724
#define Mode7Repeat PPU + 2725
#define Window1Left PPU + 2726
#define Window1Right PPU + 2727
#define Window2Left PPU + 2728
#define Window2Right PPU + 2729
#define ClipWindowOverlapLogic PPU + 2736
#define ClipWindow1Enable PPU + 2742
#define ClipWindow2Enable PPU + 2748
#define ClipWindow1Inside PPU + 2754
#define ClipWindow2Inside PPU + 2760
#define RecomputeClipWindows PPU + 2766
#define CGFLIPRead PPU + 2767
#define OBJNameSelect PPU + 2768
#define Need16x8Mulitply PPU + 2770
#define Joypad3ButtonReadPos PPU + 2771
#define MouseSpeed PPU + 2772
#define ColorsChanged IPPU + 0
#define HDMA IPPU + 1
#define HDMAStarted IPPU + 2
#define MaxBrightness IPPU + 3
#define LatchedBlanking IPPU + 4
#define OBJChanged IPPU + 5
#define RenderThisFrame IPPU + 6
#define SkippedFrames IPPU + 20
#define FrameSkip IPPU + 24
#define TileCache IPPU + 28
#define TileCached IPPU + 40
#define FirstVRAMRead IPPU + 52
#define Interlace IPPU + 54
#define DoubleWidthPixels IPPU + 56
#define RenderedScreenHeight IPPU + 60
#define RenderedScreenWidth IPPU + 64
#define Red IPPU + 68
#define Green IPPU + 1092
#define Blue IPPU + 2116
#define XB IPPU + 3140
#define ScreenColors IPPU + 3144
#define PreviousLine IPPU + 3656
#define CurrentLine IPPU + 3660
#define Joypads IPPU + 3668
#define SuperScope IPPU + 3688
#define Mouse IPPU + 3692
#define PrevMouseX IPPU + 3700
#define PrevMouseY IPPU + 3708
#define Clip IPPU + 3716
#define SA1Opcodes SA1 + 0
#define SA1_Carry SA1 + 4
#define SA1_Zero SA1 + 5
#define SA1_Negative SA1 + 6
#define SA1_Overflow SA1 + 7
#define SA1CPUExecuting SA1 + 8
#define SA1ShiftedPB SA1 + 12
#define SA1ShiftedDB SA1 + 16
#define SA1Flags SA1 + 20
#define SA1Executing SA1 + 24
#define SA1NMIActive SA1 + 25
#define SA1IRQActive SA1 + 26
#define SA1WaitingForInterrupt SA1 + 27
#define SA1PCS SA1 + 32
#define SA1PCBase SA1 + 36
#define SA1PCAtOpcodeStart SA1 + 44
#define SA1WaitAddress SA1 + 48
#define SA1WaitCounter SA1 + 52
#define SA1WaitByteAddress1 SA1 + 56
#define SA1WaitByteAddress2 SA1 + 60
#define SA1BWRAM SA1 + 40
#define SA1Map SA1 + 64
#define SA1WriteMap SA1 + 16448
#define SA1op1 SA1 + 32832
#define SA1op2 SA1 + 32834
#define SA1arithmetic_op SA1 + 32836
#define SA1sum SA1 + 32840
#define SA1overflow SA1 + 32848
#define VirtualBitmapFormat SA1 + 32849
#define SA1_in_char_dma SA1 + 32850
#define SA1variable_bit_pos SA1 + 32851
#define SA1PB SA1Registers + 0
#define SA1DB SA1Registers + 1
#define SA1PP SA1Registers + 2
#define SA1PL SA1Registers + 2
#define SA1PH SA1PL + 1
#define SA1AA SA1Registers + 4
#define SA1AL SA1Registers + 4
#define SA1AH SA1AL + 1
#define SA1DD SA1Registers + 6
#define SA1DL SA1Registers + 6
#define SA1DH SA1DL + 1
#define SA1SS SA1Registers + 8
#define SA1SL SA1Registers + 8
#define SA1SH SA1SL + 1
#define SA1XX SA1Registers + 10
#define SA1XL SA1Registers + 10
#define SA1XH SA1XL + 1
#define SA1YY SA1Registers + 12
#define SA1YL SA1Registers + 12
#define SA1YH SA1YL + 1
#define SA1PCR SA1Registers + 14
