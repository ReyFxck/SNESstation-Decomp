#!/usr/bin/env python3
from __future__ import annotations

import hashlib
import struct
import unittest

from layout_oracle import (
    DEFAULT_MANIFEST,
    OracleError,
    build_manifest,
    compare_images,
    first_structure_difference,
    load_manifest,
    render_json,
    validate_manifest,
)
from sjuncrunch import HEADER_OFF


def synthetic_container() -> tuple[bytes, bytes]:
    sections = [
        (0x00100000, 4, [b"abcd", b"EFGH"]),
        (0x00200000, 0, [b"ijkl"]),
    ]
    blob = bytearray(HEADER_OFF)
    blob.extend(struct.pack("<II", 0x00100008, len(sections)))
    unpacked = bytearray()
    for virtual_address, zero_fill_size, blocks in sections:
        compressed_size = sum(len(block) for block in blocks)
        initialized_size = compressed_size
        block_stream_size = compressed_size + 8 * len(blocks)
        padding_size = 4
        blob.extend(
            struct.pack(
                "<IIIII",
                compressed_size,
                initialized_size,
                zero_fill_size,
                virtual_address,
                block_stream_size + padding_size,
            )
        )
        for block in blocks:
            blob.extend(struct.pack("<II", len(block), len(block)))
            blob.extend(block)
            unpacked.extend(block)
        blob.extend(b"\0" * padding_size)
    blob.extend(b"outer-elf-trailer")
    return bytes(blob), bytes(unpacked)


class LayoutOracleTests(unittest.TestCase):
    def test_builds_deterministic_hash_only_manifest(self) -> None:
        packed, unpacked = synthetic_container()
        manifest = build_manifest(packed, unpacked, chunk_size=0x1000)
        validate_manifest(manifest)

        self.assertEqual(1, manifest["schema_version"])
        self.assertEqual(0x00100008, manifest["entry_address"])
        self.assertEqual(2, manifest["packed"]["section_count"])
        self.assertEqual(12, manifest["image"]["initialized_size"])
        self.assertEqual(4, manifest["image"]["zero_fill_size"])
        self.assertEqual(1, manifest["image"]["chunk_count"])
        self.assertEqual(hashlib.sha256(unpacked).hexdigest(), manifest["image"]["sha256"])
        self.assertEqual(3, sum(section["block_count"] for section in manifest["sections"]))
        self.assertNotIn("path", render_json(manifest))
        self.assertEqual(render_json(manifest), render_json(manifest))

    def test_rejects_unpacked_image_not_emitted_by_container(self) -> None:
        packed, unpacked = synthetic_container()
        changed = unpacked[:2] + b"X" + unpacked[3:]
        with self.assertRaisesRegex(OracleError, "offset 0x00000002"):
            build_manifest(packed, changed, chunk_size=0x1000)

    def test_reports_exact_first_byte_and_address(self) -> None:
        packed, reference = synthetic_container()
        manifest = build_manifest(packed, reference, chunk_size=0x1000)
        candidate = reference[:2] + b"X" + reference[3:]
        report = compare_images(reference, candidate, manifest)

        self.assertEqual("mismatch", report["status"])
        self.assertEqual(2, report["first_difference"]["image_offset"])
        self.assertEqual(0x00100002, report["first_difference"]["virtual_address"])
        self.assertEqual(reference[2], report["first_difference"]["reference_byte"])
        self.assertEqual(ord("X"), report["first_difference"]["candidate_byte"])
        self.assertEqual(1, report["different_byte_count"])
        self.assertEqual(1, report["mismatching_chunk_count"])

    def test_reports_candidate_truncation_without_crashing(self) -> None:
        packed, reference = synthetic_container()
        manifest = build_manifest(packed, reference, chunk_size=0x1000)
        candidate = reference[:-2]
        report = compare_images(reference, candidate, manifest)

        self.assertEqual("mismatch", report["status"])
        self.assertEqual(len(candidate), report["first_difference"]["image_offset"])
        self.assertIsNone(report["first_difference"]["candidate_byte"])
        self.assertEqual(2, report["different_byte_count"])

    def test_exact_candidate_closes_the_comparison(self) -> None:
        packed, reference = synthetic_container()
        manifest = build_manifest(packed, reference, chunk_size=0x1000)
        report = compare_images(reference, reference, manifest)

        self.assertEqual("exact", report["status"])
        self.assertIsNone(report["first_difference"])
        self.assertEqual(len(reference), report["matching_prefix_size"])
        self.assertEqual(0, report["different_byte_count"])

    def test_structure_difference_names_first_stale_field(self) -> None:
        expected = {"image": {"size": 12}, "rows": [1, 2]}
        actual = {"image": {"size": 13}, "rows": [1, 2]}
        self.assertEqual(
            "$.image.size: expected 12, got 13",
            first_structure_difference(expected, actual),
        )

    def test_frozen_reference_manifest_has_only_public_metadata(self) -> None:
        manifest = load_manifest(DEFAULT_MANIFEST)
        rendered = render_json(manifest)

        self.assertEqual(726_968, manifest["packed"]["size"])
        self.assertEqual(
            "4e7e2e22f7b4da9b861b884471f6343086765810581a4c00e96d0dce6754f487",
            manifest["packed"]["sha256"],
        )
        self.assertEqual(1, manifest["packed"]["section_count"])
        self.assertEqual(3_304_936, manifest["image"]["initialized_size"])
        self.assertEqual(171_568, manifest["image"]["zero_fill_size"])
        self.assertEqual(51, manifest["image"]["chunk_count"])
        self.assertEqual(
            "739e058834564ba81c2d8fc61fd9977502e9714c7eaafdd3a4ce3ec546fad71b",
            manifest["image"]["sha256"],
        )
        self.assertEqual(13, manifest["sections"][0]["block_count"])
        for forbidden in ("source_path", "packed_bytes", "unpacked_bytes", "data_base64"):
            self.assertNotIn(forbidden, rendered)


if __name__ == "__main__":
    unittest.main()
