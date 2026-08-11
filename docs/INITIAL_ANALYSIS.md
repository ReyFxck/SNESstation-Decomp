# Initial analysis — historical v0.23 WIP sample

This report records only facts independently recovered from the binary/container format.

## Packed ELF

- 32-bit little-endian ELF
- MIPS, PS2/R5900 flags present
- statically linked
- stripped
- packed entry: `0x01b00008`
- one loadable segment
- visible SJCRUNCH payload begins at file offset `0x2f00`

## SJCRUNCH payload

Recovered without using any historical symbol map:

- unpacked virtual base: `0x00100000`
- unpacked entry: `0x00100008`
- one packed section
- unpacked payload size: `3,304,936` bytes
- zero/BSS amount reported by packer metadata: `171,568` bytes
- payload is split into 13 LZO/raw blocks

## First independent code observation

At `0x00100008` execution begins with runtime/startup setup. The sequence initializes high memory/global ranges, establishes `$gp`/stack-related state and executes PS2 syscalls before calling into the application.

No historical extension symbol names have been imported into `notes/symbols.csv`.
