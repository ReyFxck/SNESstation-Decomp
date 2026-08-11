#!/usr/bin/env python3
import argparse
import ctypes
import ctypes.util
import hashlib
import os
import struct
from pathlib import Path

HEADER_OFF = 0x2F00


def load_lzo():
    names = [
        os.environ.get("SNESSTATION_LZO_LIBRARY"),
        ctypes.util.find_library("lzo2"),
        "liblzo2.so.2",
        "liblzo2.so",
    ]
    lib = None
    for name in names:
        if not name:
            continue
        try:
            lib = ctypes.CDLL(name)
            break
        except OSError:
            pass
    if lib is None:
        raise SystemExit(
            "liblzo2 runtime not found (Debian: apt install liblzo2-2; "
            "or set SNESSTATION_LZO_LIBRARY to its full path)"
        )
    fn = lib.lzo1x_decompress
    fn.argtypes = [ctypes.c_void_p, ctypes.c_size_t,
                   ctypes.c_void_p, ctypes.POINTER(ctypes.c_size_t),
                   ctypes.c_void_p]
    fn.restype = ctypes.c_int
    return fn


def unpack(src: Path, dst: Path):
    data = src.read_bytes()
    if len(data) < HEADER_OFF + 28:
        raise SystemExit("file too small for SJCRUNCH2 header")

    off = HEADER_OFF
    entry, num_sections = struct.unpack_from("<II", data, off)
    off += 8
    lzo = load_lzo()
    output = bytearray()
    sections = []

    for section_index in range(num_sections):
        compressed_size, original_size, zero_size, vaddr, file_size = \
            struct.unpack_from("<IIIII", data, off)
        off += 20
        section_out = bytearray(original_size)
        total = 0
        blocks = 0

        while total < original_size:
            if off + 8 > len(data):
                raise SystemExit("truncated block header")
            usize, csize = struct.unpack_from("<II", data, off)
            off += 8
            comp = data[off:off+csize]
            off += csize
            if len(comp) != csize:
                raise SystemExit("truncated compressed block")

            if usize == csize:
                chunk = comp
            else:
                srcbuf = (ctypes.c_ubyte * csize).from_buffer_copy(comp)
                dstbuf = (ctypes.c_ubyte * usize)()
                outlen = ctypes.c_size_t(usize)
                rv = lzo(srcbuf, csize, dstbuf, ctypes.byref(outlen), None)
                if rv != 0:
                    raise SystemExit(f"LZO error {rv} in block {blocks}")
                if outlen.value != usize:
                    raise SystemExit(f"size mismatch: expected {usize}, got {outlen.value}")
                chunk = bytes(dstbuf)

            section_out[total:total+usize] = chunk
            total += usize
            blocks += 1

        output.extend(section_out)
        sections.append({
            "index": section_index,
            "compressed_size": compressed_size,
            "original_size": original_size,
            "zero_byte_size": zero_size,
            "virtual_address": vaddr,
            "file_size": file_size,
            "blocks": blocks,
        })

    dst.write_bytes(output)
    print(f"entry=0x{entry:08x}")
    print(f"sections={num_sections}")
    for s in sections:
        print(f"section[{s['index']}]: vaddr=0x{s['virtual_address']:08x} size={s['original_size']} blocks={s['blocks']} bss={s['zero_byte_size']}")
    print(f"output_size={len(output)}")
    print(f"sha256={hashlib.sha256(output).hexdigest()}")
    return entry, sections


def main():
    ap = argparse.ArgumentParser(description="Unpack SNES Station SJCRUNCH2 payload without LZO headers")
    ap.add_argument("input", type=Path)
    ap.add_argument("output", type=Path)
    args = ap.parse_args()
    unpack(args.input, args.output)

if __name__ == "__main__":
    main()
