import copy
import hashlib
import struct
import unittest

from sjuncrunch import HEADER_OFF
from sjcrunch_pack import (
    DEFAULT_MANIFEST,
    PackingError,
    build_container,
    load_packing_manifest,
    validate_packing_manifest,
)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def synthetic_manifests() -> tuple[bytes, bytes, dict[str, object], dict[str, object]]:
    image = b"abcd"
    compressed = b"xy"
    stream = struct.pack("<II", len(image), len(compressed)) + compressed + b"\0\0"
    container = (
        struct.pack("<II", 0x00100008, 1)
        + struct.pack("<IIIII", len(compressed), len(image), 2, 0x00100000, len(stream))
        + stream
    )
    layout = {
        "entry_address": 0x00100008,
        "packed": {
            "size": 100,
            "sha256": "0" * 64,
            "sjcrunch2_header_offset": HEADER_OFF,
            "sjcrunch2_end_offset": HEADER_OFF + len(container),
        },
        "image": {"initialized_size": len(image), "sha256": sha256(image)},
        "sections": [
            {
                "index": 0,
                "image_offset": 0,
                "initialized_size": len(image),
                "zero_fill_size": 2,
                "virtual_address": 0x00100000,
                "compressed_data_size": len(compressed),
                "file_size": len(stream),
                "padding_size": 2,
                "packed_header_offset": HEADER_OFF + 8,
                "blocks": [
                    {
                        "index": 0,
                        "image_offset": 0,
                        "initialized_size": len(image),
                        "packed_data_size": len(compressed),
                        "packed_header_offset": HEADER_OFF + 28,
                        "encoding": "lzo1x",
                        "sha256": sha256(image),
                    }
                ],
            }
        ],
    }
    packing = {
        "schema_version": 1,
        "format": "snesstation-sjcrunch2-packing",
        "algorithm": "lzo1x_999",
        "compression_level": 8,
        "block_size": 0x40000,
        "container": {
            "packed_offset": HEADER_OFF,
            "packed_end_offset": HEADER_OFF + len(container),
            "size": len(container),
            "sha256": sha256(container),
            "section_count": 1,
            "block_count": 1,
            "compressed_data_size": len(compressed),
        },
        "outer_elf": {
            "prefix_size": HEADER_OFF,
            "prefix_sha256": "0" * 64,
            "trailing_size": 1,
            "trailing_sha256": "0" * 64,
            "unreproduced_size": HEADER_OFF + 1,
            "reproduced": False,
        },
        "version_evidence": {
            "embedded_minilzo_version": "1.08",
            "embedded_minilzo_date": "12-Jul-2002",
            "verified_compatible_host_lzo": "2.10",
        },
        "blocks": [
            {
                "index": 0,
                "image_offset": 0,
                "initialized_size": len(image),
                "packed_data_size": len(compressed),
                "compressed_sha256": sha256(compressed),
            }
        ],
    }
    return image, container, layout, packing


class SJCrunchPackingTests(unittest.TestCase):
    def test_builds_exact_container_with_injected_compressor(self) -> None:
        image, expected, layout, packing = synthetic_manifests()
        calls = []

        def compressor(data: bytes, level: int) -> bytes:
            calls.append((data, level))
            return b"xy"

        self.assertEqual(expected, build_container(image, layout, packing, compressor))
        self.assertEqual([(b"abcd", 8)], calls)

    def test_rejects_changed_compressed_bytes(self) -> None:
        image, _, layout, packing = synthetic_manifests()
        with self.assertRaisesRegex(PackingError, "compressed hash differs"):
            build_container(image, layout, packing, lambda _data, _level: b"xz")

    def test_rejects_private_payload_fields(self) -> None:
        _, _, _, packing = synthetic_manifests()
        changed = copy.deepcopy(packing)
        changed["packed_bytes"] = "forbidden"
        with self.assertRaisesRegex(PackingError, "forbidden private field"):
            validate_packing_manifest(changed)

    def test_frozen_manifest_records_exact_compressor_frontier(self) -> None:
        manifest = load_packing_manifest(DEFAULT_MANIFEST)
        self.assertEqual("lzo1x_999", manifest["algorithm"])
        self.assertEqual(8, manifest["compression_level"])
        self.assertEqual(13, len(manifest["blocks"]))
        self.assertEqual(714_268, manifest["container"]["size"])
        self.assertEqual(
            "597bdcee162d4f197154eaa58131799643ab2c061bb127bf4c9b7c91e9cce078",
            manifest["container"]["sha256"],
        )
        self.assertFalse(manifest["outer_elf"]["reproduced"])
        self.assertEqual(12_700, manifest["outer_elf"]["unreproduced_size"])


if __name__ == "__main__":
    unittest.main()
