# Recovered main flow — v0.23 WIP

High-confidence control flow from original `main` at `0x00104f18`:

```text
allocate 0x74-byte gsDriver + construct/clear screen
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


## Startup graphics-object correction

The first allocation in `main` is now proven to be the embedded Hiryu GSLIB
`gsDriver`, not a generic frontend object. `operator new(0x74)` returns a
pointer preserved in `$17`; `0x00198cc8` constructs `gsDriver` in that storage,
and `0x001990f8` is `gsDriver::clearScreen`. The constructor return register is
ignored, and main stores the original `$17` allocation into the global.

`sizeof(gsDriver) == 0x74` is independently explained by its embedded
`gsPipe` (`0x34` bytes) followed by the driver state through offset `+0x70`.
See [`PS2_GS_MAP.md`](PS2_GS_MAP.md).

The failure string `Memory.LoadROM returned FALSE` branches directly out of
`0x001513bc`, making that method identification particularly strong.
