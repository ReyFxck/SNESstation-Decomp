#!/usr/bin/env python3
import argparse
import struct
from pathlib import Path

PS2_EFLAGS = 0x20924001


def align(buf, n):
    while len(buf) % n:
        buf.append(0)


def main():
    ap = argparse.ArgumentParser(description="Wrap an unpacked PS2 load image in a minimal ELF for disassemblers")
    ap.add_argument("input", type=Path)
    ap.add_argument("output", type=Path)
    ap.add_argument("--base", type=lambda x: int(x, 0), default=0x00100000)
    ap.add_argument("--entry", type=lambda x: int(x, 0), default=0x00100008)
    args = ap.parse_args()

    raw = args.input.read_bytes()
    text_off = 0x1000
    shstr = b"\x00.text\x00.shstrtab\x00"
    blob = bytearray(text_off)
    blob.extend(raw)
    align(blob, 4)
    shstr_off = len(blob)
    blob.extend(shstr)
    align(blob, 4)
    shoff = len(blob)

    def sh(name, typ, flags, addr, off, size, link=0, info=0, addralign=1, entsize=0):
        return struct.pack("<IIIIIIIIII", name, typ, flags, addr, off, size,
                           link, info, addralign, entsize)

    sections = [
        bytes(40),
        sh(1, 1, 0x6, args.base, text_off, len(raw), addralign=16),
        sh(7, 3, 0, 0, shstr_off, len(shstr)),
    ]
    for section in sections:
        blob.extend(section)

    ident = bytearray(b"\x7fELF") + bytes([1, 1, 1, 0]) + bytes(8)
    ehdr = struct.pack(
        "<16sHHIIIIIHHHHHH", bytes(ident), 2, 8, 1, args.entry,
        52, shoff, PS2_EFLAGS, 52, 32, 1, 40, 3, 2
    )
    phdr = struct.pack("<IIIIIIII", 1, text_off, args.base, args.base,
                       len(raw), len(raw), 7, 0x1000)
    blob[0:52] = ehdr
    blob[52:84] = phdr
    args.output.write_bytes(blob)
    print(args.output)

if __name__ == "__main__":
    main()
