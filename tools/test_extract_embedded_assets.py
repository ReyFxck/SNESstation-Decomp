import csv
import struct
import sys
import unittest
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import extract_embedded_assets as assets


ROOT = Path(__file__).resolve().parents[1]


class EmbeddedAssetParserTests(unittest.TestCase):
    def test_xor_decoder_matches_word_loop_and_preserves_remainder(self):
        plain = b"ABCDEFGH!"
        encoded = bytearray(plain)
        for offset in (0, 4):
            word = struct.unpack_from("<I", encoded, offset)[0] ^ assets.XOR_KEY
            struct.pack_into("<I", encoded, offset, word)
        self.assertEqual(plain, assets.decode_xor_words(bytes(encoded)))

    def test_iif32_geometry_and_alpha_conversion(self):
        data = b"IIF1" + struct.pack("<III", 2, 1, 0) + bytes(
            (10, 20, 30, 0x40, 1, 2, 3, 0x80)
        )
        info = assets.parse_iif(data)
        self.assertEqual((2, 1, 0), (info["width"], info["height"], info["psm"]))
        self.assertEqual(
            bytes((10, 20, 30, 0x80, 1, 2, 3, 0xFF)),
            assets.rgba_from_iif(data, info),
        )

    def test_bfnt_header_derives_pixel_boundary(self):
        header = b"BFNT" + struct.pack("<7I", 1, 1, 0, 1, 1, 1, 1)
        data = header + bytes(256) + bytes((255, 255, 255, 0x80))
        self.assertEqual(0x124, len(data))
        info = assets.parse_bfnt(data)
        self.assertEqual(0x120, info["pixel_offset"])
        self.assertEqual((1, 1), (info["columns"], info["rows"]))

    def test_protracker_size_is_derived_not_assumed(self):
        data = bytearray(1084 + 1024)
        data[:4] = b"test"
        data[950] = 1
        data[951] = 0x7F
        data[952] = 0
        data[1080:1084] = b"M.K."
        info = assets.parse_protracker_mod(bytes(data))
        self.assertEqual("test", info["title"])
        self.assertEqual(4, info["channels"])
        self.assertEqual(1, info["patterns"])
        self.assertEqual(len(data), info["derived_size"])

    def test_ps2_icon_texture_boundary_is_structural(self):
        data = bytearray(struct.pack("<5I", 0x10000, 1, 7, 0x3F800000, 3))
        for vertex in range(3):
            data.extend(struct.pack("<4h", vertex * 4096, 0, 0, 0))
            data.extend(struct.pack("<4h", 0, 0, 4096, 0))
            data.extend(struct.pack("<hhI", 0, 0, 0xFFFFFFFF))
        data.extend(struct.pack("<IIfII", 1, 1, 1.0, 0, 1))
        data.extend(struct.pack("<IIff", 0, 1, 1.0, 1.0))
        data.extend(bytes(128 * 128 * 2))
        info = assets.parse_ps2_icon(bytes(data))
        self.assertEqual(3, info["vertices"])
        self.assertEqual(1, info["triangles"])
        self.assertEqual(len(data) - 32768, info["texture_offset"])

    def test_png_writer_emits_rgba_png_with_dimensions(self):
        png = assets.png_rgba(2, 1, bytes((0, 0, 0, 255) * 2))
        self.assertEqual(b"\x89PNG\r\n\x1a\n", png[:8])
        self.assertEqual((2, 1), struct.unpack_from(">II", png, 16))


class EmbeddedAssetCatalogTests(unittest.TestCase):
    def test_public_catalog_matches_extractor_constants(self):
        with (ROOT / "analysis" / "embedded_assets.csv").open(
            newline="", encoding="utf-8"
        ) as stream:
            rows = list(csv.DictReader(stream))
        self.assertEqual([item.name for item in assets.ASSETS], [row["name"] for row in rows])
        for item, row in zip(assets.ASSETS, rows):
            self.assertEqual(f"0x{item.va:08x}", row["va_start"])
            self.assertEqual(f"0x{item.end_va:08x}", row["va_end"])
            self.assertEqual(item.size, int(row["size_dec"]))
            self.assertEqual(item.sha256, row["sha256"])
            self.assertEqual(item.decoded_sha256 or "", row["decoded_sha256"])

    def test_ranges_are_ordered_and_non_overlapping(self):
        for previous, current in zip(assets.ASSETS, assets.ASSETS[1:]):
            self.assertLessEqual(previous.end_va, current.va)

    @unittest.skipUnless(
        (ROOT / "build" / "SNES_EMU.unpacked.bin").is_file(),
        "private unpacked reference is not present",
    )
    def test_all_ranges_against_private_verified_reference(self):
        image = (ROOT / "build" / "SNES_EMU.unpacked.bin").read_bytes()
        self.assertEqual(assets.REFERENCE_SIZE, len(image))
        self.assertEqual(assets.REFERENCE_SHA256, assets.sha256(image))
        self.assertEqual(3, image.count(b"\x7fELF"))
        self.assertEqual(3, image.count(b"IIF1"))
        self.assertEqual(1, image.count(b"BFNT"))
        self.assertEqual(1, image.count(b"M.K."))
        for item in assets.ASSETS:
            data = assets.image_slice(image, item)
            self.assertEqual(item.size, len(data))
            assets.validate_size_word(image, item)


if __name__ == "__main__":
    unittest.main()
