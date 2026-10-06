/*
 * Isolated historical Snes9x 1.40 S9xGetByte body for SNES Station EE.
 * Official source: snes9x-1.40-src-2.tar.gz, getset.h.
 *
 * Target entry: 0x001ab63c, exact target span: 708 bytes.
 */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef int int32;

class CMemory
{
public:
    enum {
        MAP_PPU, MAP_CPU, MAP_DSP, MAP_LOROM_SRAM, MAP_HIROM_SRAM,
        MAP_NONE, MAP_DEBUG, MAP_C4, MAP_BWRAM, MAP_BWRAM_BITMAP,
        MAP_BWRAM_BITMAP2, MAP_SA1RAM, MAP_SPC7110_ROM, MAP_SPC7110_DRAM,
        MAP_RONLY_SRAM, MAP_OBC_RAM, MAP_SETA_DSP, MAP_SETA_RISC, MAP_LAST
    };

    uint8 *RAM;
    uint8 *ROM;
    uint8 *VRAM;
    uint8 *SRAM;
    uint8 *BWRAM;
    uint8 *FillRAM;
    uint8 *C4RAM;
    uint8 HiROM;
    uint8 LoROM;
    uint32 SRAMMask;
    uint8 SRAMSize;
    uint8 *Map[4096];
    uint8 *WriteMap[4096];
    uint8 MemorySpeed[4096];
    uint8 BlockIsRAM[4096];
    uint8 BlockIsROM[4096];
};

struct SCPUState
{
    uint32 Flags;
    uint8 BranchSkip;
    uint8 NMIActive;
    uint8 IRQActive;
    uint8 WaitingForInterrupt;
    uint8 InDMA;
    uint8 WhichEvent;
    uint8 reserved_0a_0b[2];
    uint8 *PC;
    uint8 *PCBase;
    uint8 *PCAtOpcodeStart;
    uint8 *WaitAddress;
    uint32 WaitCounter;
    int32 Cycles;
};

extern CMemory Memory __asm__("g_p12_memory");
extern SCPUState CPU __asm__("g_CPU_blob");
extern uint8 OpenBus __asm__("g_OpenBus_byte");

extern "C" uint8 S9xGetPPU(uint16) __asm__("dep_S9xGetPPU");
extern "C" uint8 S9xGetCPU(uint16) __asm__("dep_S9xGetCPU");
extern "C" uint8 S9xGetDSP(uint16) __asm__("dep_S9xGetDSP");
extern "C" uint8 S9xGetC4(uint16) __asm__("dep_S9xGetC4");
extern "C" uint8 S9xGetSPC7110Byte(uint32) __asm__("dep_S9xGetSPC7110Byte");
extern "C" uint8 S9xGetSPC7110(uint16) __asm__("dep_S9xGetSPC7110");
extern "C" uint8 GetOBC1(uint16) __asm__("dep_GetOBC1");
extern "C" uint8 S9xGetSetaDSP(uint32) __asm__("dep_S9xGetSetaDSP");
extern "C" uint8 S9xGetST018(uint32) __asm__("dep_S9xGetST018");

uint8 S9xGetByte (uint32 Address)
{
#if defined(VAR_CYCLES) || defined(CPU_SHUTDOWN)
    int block;
    uint8 *GetAddress = Memory.Map [block = (Address >> 12) & 0x0fff];
#else
    uint8 *GetAddress = Memory.Map [(Address >> 12) & 0x0fff];
#endif
    if (GetAddress >= (uint8 *) CMemory::MAP_LAST)
    {
#ifdef VAR_CYCLES
        CPU.Cycles += Memory.MemorySpeed [block];
#endif
#ifdef CPU_SHUTDOWN
        if (Memory.BlockIsRAM [block])
            CPU.WaitAddress = CPU.PCAtOpcodeStart;
#endif
        return (*(GetAddress + (Address & 0xffff)));
    }

    switch ((int) GetAddress)
    {
    case CMemory::MAP_PPU:
#ifdef VAR_CYCLES
        if (!CPU.InDMA)
            CPU.Cycles += 6;
#endif
        return (S9xGetPPU (Address & 0xffff));
    case CMemory::MAP_CPU:
#ifdef VAR_CYCLES
        CPU.Cycles += 6;
#endif
        return (S9xGetCPU (Address & 0xffff));
    case CMemory::MAP_DSP:
#ifdef VAR_CYCLES
        CPU.Cycles += 8;
#endif
        return (S9xGetDSP (Address & 0xffff));
    case CMemory::MAP_SA1RAM:
    case CMemory::MAP_LOROM_SRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += 8;
#endif
        return (*(Memory.SRAM + ((((Address&0xFF0000)>>1) |(Address&0x7FFF)) &Memory.SRAMMask)));
    case CMemory::MAP_RONLY_SRAM:
    case CMemory::MAP_HIROM_SRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += 8;
#endif
        return (*(Memory.SRAM + (((Address & 0x7fff) - 0x6000 +
            ((Address & 0xf0000) >> 3)) & Memory.SRAMMask)));
    case CMemory::MAP_BWRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += 8;
#endif
        return (*(Memory.BWRAM + ((Address & 0x7fff) - 0x6000)));
    case CMemory::MAP_C4:
        return (S9xGetC4 (Address & 0xffff));
    case CMemory::MAP_SPC7110_ROM:
        CPU.Cycles += 8;
        return S9xGetSPC7110Byte(Address);
    case CMemory::MAP_SPC7110_DRAM:
        CPU.Cycles += 8;
        return S9xGetSPC7110(0x4800);
    case CMemory::MAP_OBC_RAM:
        CPU.Cycles += 8;
        return GetOBC1(Address & 0xffff);
    case CMemory::MAP_SETA_DSP:
        CPU.Cycles += 8;
        return S9xGetSetaDSP(Address);
    case CMemory::MAP_SETA_RISC:
        CPU.Cycles += 8;
        return S9xGetST018(Address);
    case CMemory::MAP_DEBUG:
        return OpenBus;
    default:
    case CMemory::MAP_NONE:
#ifdef VAR_CYCLES
        CPU.Cycles += 8;
#endif
        return OpenBus;
    }
}
