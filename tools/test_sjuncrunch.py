import struct
import tempfile
import unittest
from pathlib import Path

from sjuncrunch import HEADER_OFF, LZOError, lzo1x_decompress_python, unpack


class LZO1XFallbackTests(unittest.TestCase):
    def test_literal_block(self):
        stream = b"\x15abcd\x11\x00\x00"
        self.assertEqual(lzo1x_decompress_python(stream, 4), b"abcd")

    def test_overlapping_match(self):
        # Four literals followed by an offset-one, length-three M2 match.
        stream = b"\x15aaaa\x40\x00\x11\x00\x00"
        self.assertEqual(lzo1x_decompress_python(stream, 7), b"aaaaaaa")

    def test_rejects_wrong_output_size(self):
        stream = b"\x15abcd\x11\x00\x00"
        with self.assertRaisesRegex(LZOError, "size mismatch"):
            lzo1x_decompress_python(stream, 5)

    def test_rejects_trailing_input(self):
        stream = b"\x15abcd\x11\x00\x00x"
        with self.assertRaisesRegex(LZOError, "not fully consumed"):
            lzo1x_decompress_python(stream, 4)

    def test_rejects_invalid_container_section_count(self):
        blob = bytearray(HEADER_OFF + 28)
        struct.pack_into("<II", blob, HEADER_OFF, 0x00100008, 0)
        with tempfile.TemporaryDirectory(prefix="snesstation-sj-test-") as directory:
            source = Path(directory) / "bad.elf"
            target = Path(directory) / "bad.bin"
            source.write_bytes(blob)
            with self.assertRaisesRegex(SystemExit, "section count"):
                unpack(source, target)

    def test_rejects_block_larger_than_declared_section(self):
        blob = bytearray(HEADER_OFF)
        blob.extend(struct.pack("<II", 0x00100008, 1))
        blob.extend(struct.pack("<IIIII", 10, 1, 0, 0x00100000, 1))
        blob.extend(struct.pack("<II", 2, 2))
        blob.extend(b"xx")
        with tempfile.TemporaryDirectory(prefix="snesstation-sj-test-") as directory:
            source = Path(directory) / "bad.elf"
            target = Path(directory) / "bad.bin"
            source.write_bytes(blob)
            with self.assertRaisesRegex(SystemExit, "overruns declared section"):
                unpack(source, target)


if __name__ == "__main__":
    unittest.main()
