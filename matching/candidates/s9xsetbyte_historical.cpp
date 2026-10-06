/*
 * Exact historical Snes9x 1.40 S9xSetByte body isolated from getset.h for
 * the SNES Station EE target.
 *
 * Official source: snes9x-1.40-src-2.tar.gz
 * Target entry: 0x001ab900, exact target span: 808 bytes.
 */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned char bool8;

#ifndef NULL
#define NULL 0
#endif

enum {
    MEMMAP_SHIFT = 12,
    MEMMAP_MASK = 0x0fff,
    MEMMAP_NUM_BLOCKS = 4096,
    ONE_CYCLE = 6,
    SLOW_ONE_CYCLE = 8,
    TRUE = 1
};

class CMemory
{
public:
    enum {
        MAP_PPU,
        MAP_CPU,
        MAP_DSP,
        MAP_LOROM_SRAM,
        MAP_HIROM_SRAM,
        MAP_NONE,
        MAP_DEBUG,
        MAP_C4,
        MAP_BWRAM,
        MAP_BWRAM_BITMAP,
        MAP_BWRAM_BITMAP2,
        MAP_SA1RAM,
        MAP_SPC7110_ROM,
        MAP_SPC7110_DRAM,
        MAP_RONLY_SRAM,
        MAP_OBC_RAM,
        MAP_SETA_DSP,
        MAP_SETA_RISC,
        MAP_LAST
    };

    uint8 *RAM;
    uint8 *ROM;
    uint8 *VRAM;
    uint8 *SRAM;
    uint8 *BWRAM;
    uint8 *FillRAM;
    uint8 *C4RAM;
    bool8 HiROM;
    bool8 LoROM;
    uint32 SRAMMask;
    uint8 SRAMSize;
    uint8 *Map[MEMMAP_NUM_BLOCKS];
    uint8 *WriteMap[MEMMAP_NUM_BLOCKS];
    uint8 MemorySpeed[MEMMAP_NUM_BLOCKS];
    uint8 BlockIsRAM[MEMMAP_NUM_BLOCKS];
    uint8 BlockIsROM[MEMMAP_NUM_BLOCKS];
};

struct HistoricalCPU
{
    uint32 Flags;
    uint8 BranchSkip;
    uint8 NMIActive;
    uint8 IRQActive;
    uint8 WaitingForInterrupt;
    uint8 InDMA;
    uint8 WhichEvent;
    uint8 reserved_0a[2];
    uint8 *PC;
    uint8 *PCBase;
    uint8 *PCAtOpcodeStart;
    uint8 *WaitAddress;
    uint32 WaitCounter;
    long Cycles;
    long NextEvent;
    long V_Counter;
    long MemSpeed;
    long MemSpeedx2;
    long FastROMSpeed;
    uint32 AutoSaveTimer;
    bool8 SRAMModified;
};

struct HistoricalSA1
{
    void *S9xOpcodes;
    uint8 _Carry;
    uint8 _Zero;
    uint8 _Negative;
    uint8 _Overflow;
    bool8 CPUExecuting;
    uint32 ShiftedPB;
    uint32 ShiftedDB;
    uint32 Flags;
    bool8 Executing;
    bool8 NMIActive;
    bool8 IRQActive;
    bool8 WaitingForInterrupt;
    bool8 Waiting;
    uint8 *PC;
    uint8 *PCBase;
    uint8 *BWRAM;
    uint8 *PCAtOpcodeStart;
    uint8 *WaitAddress;
    uint32 WaitCounter;
    uint8 *WaitByteAddress1;
    uint8 *WaitByteAddress2;
};

struct HistoricalSPC7110Regs
{
    uint8 register_state[48];
    uint32 DataRomOffset;
    uint32 DataRomSize;
    uint32 bank50Internal;
    uint8 bank50[0x10000];
};

extern CMemory Memory __asm__("g_p12_memory");
extern HistoricalCPU CPU __asm__("g_CPU_blob");
extern HistoricalSA1 SA1 __asm__("g_SA1_blob");
extern HistoricalSPC7110Regs s7r __asm__("g_s7r_blob");

extern "C" void S9xSetPPU(uint8, uint16);
extern "C" void S9xSetCPU(uint8, uint16);
extern "C" void S9xSetDSP(uint8, uint16);
extern "C" void S9xSetC4(uint8, uint16);
extern "C" void SetOBC1(uint8, uint16);
extern "C" void S9xSetSetaDSP(uint32, uint8);
extern "C" void S9xSetST018(uint32, uint8);

void S9xSetByte (uint8 Byte, uint32 Address)
{
#if defined(CPU_SHUTDOWN)
    CPU.WaitAddress = NULL;
#endif
#if defined(VAR_CYCLES)
    int block;
    uint8 *SetAddress = Memory.WriteMap [block = ((Address >> MEMMAP_SHIFT) & MEMMAP_MASK)];
#else
    uint8 *SetAddress = Memory.WriteMap [(Address >> MEMMAP_SHIFT) & MEMMAP_MASK];
#endif

    if (SetAddress >= (uint8 *) CMemory::MAP_LAST)
    {
#ifdef VAR_CYCLES
        CPU.Cycles += Memory.MemorySpeed [block];
#endif
#ifdef CPU_SHUTDOWN
        SetAddress += Address & 0xffff;
        if (SetAddress == SA1.WaitByteAddress1 ||
            SetAddress == SA1.WaitByteAddress2)
        {
            SA1.Executing = SA1.S9xOpcodes != NULL;
            SA1.WaitCounter = 0;
        }
        *SetAddress = Byte;
#else
        *(SetAddress + (Address & 0xffff)) = Byte;
#endif
        return;
    }

    switch ((int) SetAddress)
    {
    case CMemory::MAP_PPU:
#ifdef VAR_CYCLES
        if (!CPU.InDMA)
            CPU.Cycles += ONE_CYCLE;
#endif
        S9xSetPPU (Byte, Address & 0xffff);
        return;

    case CMemory::MAP_CPU:
#ifdef VAR_CYCLES
        CPU.Cycles += ONE_CYCLE;
#endif
        S9xSetCPU (Byte, Address & 0xffff);
        return;

    case CMemory::MAP_DSP:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        S9xSetDSP (Byte, Address & 0xffff);
        return;

    case CMemory::MAP_LOROM_SRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        if (Memory.SRAMMask)
        {
            *(Memory.SRAM + ((((Address&0xFF0000)>>1)|(Address&0x7FFF))& Memory.SRAMMask))=Byte;
            CPU.SRAMModified = TRUE;
        }
        return;

    case CMemory::MAP_HIROM_SRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        if (Memory.SRAMMask)
        {
            *(Memory.SRAM + (((Address & 0x7fff) - 0x6000 +
                ((Address & 0xf0000) >> 3)) & Memory.SRAMMask)) = Byte;
            CPU.SRAMModified = TRUE;
        }
        return;

    case CMemory::MAP_BWRAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        *(Memory.BWRAM + ((Address & 0x7fff) - 0x6000)) = Byte;
        CPU.SRAMModified = TRUE;
        return;

    case CMemory::MAP_DEBUG:
    case CMemory::MAP_SA1RAM:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        *(Memory.SRAM + (Address & 0xffff)) = Byte;
        SA1.Executing = !SA1.Waiting;
        break;

    case CMemory::MAP_C4:
        S9xSetC4 (Byte, Address & 0xffff);
        return;

    case CMemory::MAP_SPC7110_DRAM:
        CPU.Cycles += SLOW_ONE_CYCLE;
        s7r.bank50[(Address & 0xffff)] = (uint8) Byte;
        break;

    case CMemory::MAP_OBC_RAM:
        CPU.Cycles += SLOW_ONE_CYCLE;
        SetOBC1(Byte, Address & 0xFFFF);
        return;

    case CMemory::MAP_SETA_DSP:
        CPU.Cycles += SLOW_ONE_CYCLE;
        S9xSetSetaDSP(Address, Byte);
        return;

    case CMemory::MAP_SETA_RISC:
        CPU.Cycles += SLOW_ONE_CYCLE;
        S9xSetST018(Address, Byte);
        return;

    default:
    case CMemory::MAP_NONE:
#ifdef VAR_CYCLES
        CPU.Cycles += SLOW_ONE_CYCLE;
#endif
        return;
    }
}
