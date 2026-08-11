# Recovered main flow — v0.23 WIP

High-confidence control flow from original `main` at `0x00104f18`:

```text
construct PS2 frontend object
        |
reset IOP / load ROM modules
        |
init memory card
        |
load embedded CDVD + AmigaMod + SjPCM IRXs
        |
CDVD_Init
        |
init local globals / decode two XOR tables
        |
load SNES Station config
        |
init frontend + memory-card save manager
        |
+---------------- ROM SELECTOR <---------------------------+
|       |                                                  |
|   top-level GUI (0x102ab0)                               |
|       | selected ROM path                                |
|   initialise Snes9x Settings                             |
|       |                                                  |
|   CMemory::Init-like (0x151074)                          |
|       |                                                  |
|   APU buffer allocator (0x10a840)                        |
|       |                                                  |
|   CMemory::LoadROM (0x1513bc)                            |
|       |                                                  |
|   optional LoadSRAM (0x153354)                           |
|       |                                                  |
|   renderer/audio setup                                   |
|       |                                                  |
|   emulation/menu loop                                    |
|       |                                                  |
|   per-ROM cleanup (0x151330)                             |
|       +--------------------------------------------------+
```

The failure string `Memory.LoadROM returned FALSE` branches directly out of
`0x001513bc`, making that method identification particularly strong.
