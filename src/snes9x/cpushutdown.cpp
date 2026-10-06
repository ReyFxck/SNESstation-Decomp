/*
 * Exact historical Snes9x 1.42 CPUShutdown body isolated from cpuops.cpp
 * for the SNES Station EE target.
 *
 * Target: 0x001ac604, 304 bytes, 29 text relocations.
 */
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned char bool8;

#ifndef NULL
#define NULL 0
#endif

enum {
    FALSE = 0,
    TRUE = 1,
    IRQ_PENDING_FLAG = 0x080,
    NMI_FLAG = 0x800
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

struct HistoricalICPU
{
    uint8 reserved_00[12];
    bool8 CPUExecuting;
};

struct HistoricalAPU
{
    int Cycles;
};

struct HistoricalIAPU
{
    uint8 *PC;
    uint8 *DirectPage;
    uint8 *RAM;
    bool8 APUExecuting;
};

struct HistoricalSettings
{
    uint8 reserved_00;
    bool8 Shutdown;
    uint8 reserved_02[0x60];
    bool8 SA1;
};

extern HistoricalCPU CPU __asm__("CPU");
extern HistoricalICPU ICPU __asm__("ICPU");
extern HistoricalAPU APU __asm__("APU");
extern HistoricalIAPU IAPU __asm__("IAPU");
extern HistoricalSettings Settings __asm__("Settings");
extern int S9xAPUCycles[256] __asm__("S9xAPUCycles");
extern void (*S9xApuOpcodes[256])() __asm__("S9xApuOpcodes");
extern "C" void S9xSA1ExecuteDuringSleep();

#define APU_EXECUTE1()                 \
    do {                               \
        APU.Cycles += S9xAPUCycles[*IAPU.PC]; \
        (*S9xApuOpcodes[*IAPU.PC])();  \
    } while (0)

void CPUShutdown()
{
    if (Settings.Shutdown && CPU.PC == CPU.WaitAddress)
    {
        // Don't skip cycles with a pending NMI or IRQ - could cause delayed
        // interrupt. Interrupts are delayed for a few cycles already, but
        // the delay could allow the shutdown code to cycle skip again.
        // Was causing screen flashing on Top Gear 3000.

        if (CPU.WaitCounter == 0 &&
            !(CPU.Flags & (IRQ_PENDING_FLAG | NMI_FLAG)))
        {
            CPU.WaitAddress = NULL;
            if (Settings.SA1)
                S9xSA1ExecuteDuringSleep ();
            CPU.Cycles = CPU.NextEvent;
            if (IAPU.APUExecuting)
            {
                ICPU.CPUExecuting = FALSE;
                do
                {
                    APU_EXECUTE1();
                } while (APU.Cycles < CPU.NextEvent);
                ICPU.CPUExecuting = TRUE;
            }
        }
        else
        if (CPU.WaitCounter >= 2)
            CPU.WaitCounter = 1;
        else
            CPU.WaitCounter--;
    }
}
