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
        return None
    fn = lib.lzo1x_decompress
    fn.argtypes = [ctypes.c_void_p, ctypes.c_size_t,
                   ctypes.c_void_p, ctypes.POINTER(ctypes.c_size_t),
                   ctypes.c_void_p]
    fn.restype = ctypes.c_int
    return fn


class LZOError(ValueError):
    """Malformed or unsupported LZO1X block."""


class SJCrunchError(ValueError):
    """Malformed or inconsistent SJCRUNCH2 container."""


def lzo1x_decompress_python(data: bytes, expected_size: int) -> bytes:
    """Decode an LZO1X block without a system liblzo dependency.

    SJCRUNCH2 stores independent LZO1X blocks.  Keeping this small safe decoder
    in-tree makes reference extraction work in rootless Android/container
    environments where ``liblzo2.so`` is commonly unavailable.
    """
    if not data:
        raise LZOError("empty LZO1X block")
    if expected_size < 0:
        raise LZOError("negative output size")

    source_size = len(data)
    ip = 0
    output = bytearray()

    def read_byte() -> int:
        nonlocal ip
        if ip >= source_size:
            raise LZOError("LZO1X input overrun")
        value = data[ip]
        ip += 1
        return value

    def copy_literals(count: int) -> None:
        nonlocal ip
        if count < 0 or ip + count > source_size:
            raise LZOError("LZO1X literal input overrun")
        if len(output) + count > expected_size:
            raise LZOError("LZO1X literal output overrun")
        output.extend(data[ip:ip + count])
        ip += count

    def copy_match(position: int, count: int) -> None:
        if position < 0 or position >= len(output):
            raise LZOError("LZO1X look-behind overrun")
        if count < 0 or len(output) + count > expected_size:
            raise LZOError("LZO1X match output overrun")
        # Bytewise copying is intentional: LZO matches may overlap.
        for _ in range(count):
            output.append(output[position])
            position += 1

    def extended_length(initial: int, base: int) -> int:
        value = initial
        if value:
            return value
        while True:
            byte = read_byte()
            if byte:
                return value + base + byte
            value += 255

    state = "outer"
    t = 0
    first = data[0]
    if first > 17:
        ip = 1
        t = first - 17
        if t < 4:
            state = "match_next"
        else:
            copy_literals(t)
            state = "first_literal_run"

    while True:
        if state == "outer":
            t = read_byte()
            if t >= 16:
                state = "match"
                continue
            t = extended_length(t, 15)
            copy_literals(t + 3)
            state = "first_literal_run"
            continue

        if state == "first_literal_run":
            t = read_byte()
            if t >= 16:
                state = "match"
                continue
            position = len(output) - (1 + 0x0800)
            position -= t >> 2
            position -= read_byte() << 2
            copy_match(position, 3)
            state = "match_done"
            continue

        if state == "match":
            if t >= 64:
                position = len(output) - 1
                position -= (t >> 2) & 7
                position -= read_byte() << 3
                length = (t >> 5) - 1
                copy_match(position, length + 2)
            elif t >= 32:
                length = extended_length(t & 31, 31)
                first_offset = read_byte()
                second_offset = read_byte()
                position = len(output) - 1
                position -= (first_offset >> 2) + (second_offset << 6)
                copy_match(position, length + 2)
            elif t >= 16:
                position = len(output) - ((t & 8) << 11)
                length = extended_length(t & 7, 7)
                first_offset = read_byte()
                second_offset = read_byte()
                position -= (first_offset >> 2) + (second_offset << 6)
                if position == len(output):
                    if length != 1:
                        raise LZOError("invalid LZO1X end marker")
                    if ip != source_size:
                        raise LZOError("LZO1X input not fully consumed")
                    if len(output) != expected_size:
                        raise LZOError(
                            f"LZO1X size mismatch: expected {expected_size}, "
                            f"got {len(output)}"
                        )
                    return bytes(output)
                position -= 0x4000
                copy_match(position, length + 2)
            else:
                position = len(output) - 1
                position -= t >> 2
                position -= read_byte() << 2
                copy_match(position, 2)
            state = "match_done"
            continue

        if state == "match_done":
            if ip < 2:
                raise LZOError("invalid LZO1X match state")
            t = data[ip - 2] & 3
            state = "outer" if t == 0 else "match_next"
            continue

        if state == "match_next":
            copy_literals(t)
            t = read_byte()
            state = "match"
            continue

        raise AssertionError(f"unknown LZO1X decoder state: {state}")


def decompress_block(comp: bytes, expected_size: int, native_lzo) -> bytes:
    if native_lzo is None:
        return lzo1x_decompress_python(comp, expected_size)
    csize = len(comp)
    srcbuf = (ctypes.c_ubyte * csize).from_buffer_copy(comp)
    dstbuf = (ctypes.c_ubyte * expected_size)()
    outlen = ctypes.c_size_t(expected_size)
    rv = native_lzo(srcbuf, csize, dstbuf, ctypes.byref(outlen), None)
    if rv != 0:
        raise LZOError(f"native LZO error {rv}")
    if outlen.value != expected_size:
        raise LZOError(
            f"native LZO size mismatch: expected {expected_size}, got {outlen.value}"
        )
    return bytes(dstbuf)


def decode_container(data: bytes, native_lzo=None) -> dict[str, object]:
    """Decode an SJCRUNCH2 payload and retain deterministic layout metadata."""
    if len(data) < HEADER_OFF + 28:
        raise SJCrunchError("file too small for SJCRUNCH2 header")

    off = HEADER_OFF
    entry, num_sections = struct.unpack_from("<II", data, off)
    off += 8
    if num_sections == 0 or num_sections > 64:
        raise SJCrunchError(f"invalid SJCRUNCH2 section count: {num_sections}")

    output = bytearray()
    sections: list[dict[str, object]] = []

    for section_index in range(num_sections):
        if off + 20 > len(data):
            raise SJCrunchError("truncated section header")

        header_offset = off
        compressed_size, original_size, zero_size, vaddr, file_size = \
            struct.unpack_from("<IIIII", data, off)
        off += 20
        block_stream_offset = off
        image_offset = len(output)
        section_out = bytearray(original_size)
        total = 0
        compressed_total = 0
        blocks: list[dict[str, object]] = []

        while total < original_size:
            if off + 8 > len(data):
                raise SJCrunchError("truncated block header")
            block_header_offset = off
            usize, csize = struct.unpack_from("<II", data, off)
            off += 8
            if usize == 0 or csize == 0:
                raise SJCrunchError("invalid zero-sized compressed block")
            if usize > original_size - total:
                raise SJCrunchError("compressed block overruns declared section size")

            block_data_offset = off
            comp = data[off:off+csize]
            off += csize
            if len(comp) != csize:
                raise SJCrunchError("truncated compressed block")

            if usize == csize:
                chunk = comp
                encoding = "stored"
            else:
                try:
                    chunk = decompress_block(comp, usize, native_lzo)
                except LZOError as exc:
                    raise SJCrunchError(
                        f"LZO error in block {len(blocks)}: {exc}"
                    ) from exc
                encoding = "lzo1x"

            section_out[total:total+usize] = chunk
            blocks.append(
                {
                    "index": len(blocks),
                    "packed_header_offset": block_header_offset,
                    "packed_data_offset": block_data_offset,
                    "packed_data_size": csize,
                    "image_offset": image_offset + total,
                    "virtual_address": vaddr + total,
                    "initialized_size": usize,
                    "encoding": encoding,
                    "sha256": hashlib.sha256(chunk).hexdigest(),
                }
            )
            total += usize
            compressed_total += csize

        block_stream_end = off
        block_stream_size = block_stream_end - block_stream_offset
        if compressed_total != compressed_size:
            raise SJCrunchError(
                "section compressed-size mismatch: "
                f"declared {compressed_size}, decoded {compressed_total}"
            )
        if file_size < block_stream_size:
            raise SJCrunchError(
                "section file size is smaller than its block stream: "
                f"declared {file_size}, consumed {block_stream_size}"
            )

        packed_end_offset = block_stream_offset + file_size
        if packed_end_offset > len(data):
            raise SJCrunchError("section file span extends beyond the packed input")
        padding = data[block_stream_end:packed_end_offset]
        off = packed_end_offset

        output.extend(section_out)
        sections.append(
            {
                "index": section_index,
                "packed_header_offset": header_offset,
                "packed_block_stream_offset": block_stream_offset,
                "packed_block_stream_end": block_stream_end,
                "packed_end_offset": packed_end_offset,
                "compressed_data_size": compressed_size,
                "block_stream_size": block_stream_size,
                "file_size": file_size,
                "padding_size": len(padding),
                "padding_is_zero": not any(padding),
                "image_offset": image_offset,
                "virtual_address": vaddr,
                "initialized_size": original_size,
                "zero_fill_size": zero_size,
                "memory_size": original_size + zero_size,
                "sha256": hashlib.sha256(section_out).hexdigest(),
                "blocks": blocks,
            }
        )

    return {
        "entry_address": entry,
        "header_offset": HEADER_OFF,
        "container_end_offset": off,
        "image": bytes(output),
        "sections": sections,
    }


def unpack(src: Path, dst: Path):
    data = src.read_bytes()
    native_lzo = load_lzo()
    try:
        decoded = decode_container(data, native_lzo=native_lzo)
    except SJCrunchError as exc:
        raise SystemExit(str(exc)) from exc

    output = decoded["image"]
    sections = decoded["sections"]
    assert isinstance(output, bytes)
    assert isinstance(sections, list)
    dst.write_bytes(output)
    print(f"entry=0x{decoded['entry_address']:08x}")
    print(
        "decompressor="
        f"{'native-liblzo2' if native_lzo is not None else 'python-lzo1x'}"
    )
    print(f"sections={len(sections)}")
    for section in sections:
        blocks = section["blocks"]
        assert isinstance(blocks, list)
        print(
            f"section[{section['index']}]: "
            f"vaddr=0x{section['virtual_address']:08x} "
            f"size={section['initialized_size']} "
            f"blocks={len(blocks)} bss={section['zero_fill_size']}"
        )
    print(f"output_size={len(output)}")
    print(f"sha256={hashlib.sha256(output).hexdigest()}")
    return decoded["entry_address"], sections


def main():
    ap = argparse.ArgumentParser(description="Unpack SNES Station SJCRUNCH2 payload without LZO headers")
    ap.add_argument("input", type=Path)
    ap.add_argument("output", type=Path)
    args = ap.parse_args()
    unpack(args.input, args.output)

if __name__ == "__main__":
    main()
