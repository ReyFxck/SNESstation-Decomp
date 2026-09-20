#!/usr/bin/env python3

import json
import unittest
from pathlib import Path

import object_linkage


ROOT = Path(__file__).resolve().parents[1]


class ObjectLinkageAuditTests(unittest.TestCase):
    def test_frozen_manifest_matches_current_transport(self) -> None:
        document = object_linkage.validate()
        self.assertFalse(document["result"]["object_native_complete"])
        self.assertEqual(document["result"]["direct_object_bytes"], 632_936)
        self.assertEqual(document["result"]["remaining_object_native_bytes"], 87_684)

    def test_candidate_object_inventory_is_explicit(self) -> None:
        document = object_linkage.validate()
        result = document["result"]
        self.assertEqual(result["candidate_elf_objects"], 97)
        self.assertEqual(result["candidate_elf_object_slices"], 432)
        self.assertEqual(result["candidate_elf_object_bytes"], 611_088)
        self.assertEqual(result["listing_bytes"], 73_192)
        self.assertEqual(result["residual_assembly_bytes"], 36_340)
        self.assertEqual(result["direct_candidate_object_inputs"], 55)
        self.assertTrue(result["transport"]["selected_candidate_objects_linked_directly"])
        self.assertEqual(result["direct_object_inputs"], 60)
        self.assertEqual(result["direct_object_sections"], 202)
        self.assertEqual(result["incbin_payload_bytes"], 87_684)
        self.assertEqual(result["incbin_payload_sections"], 105)

    def test_strict_completion_gate_rejects_incbin_transport(self) -> None:
        document = object_linkage.validate()
        with self.assertRaises(object_linkage.ObjectLinkageError):
            object_linkage.require_complete(document)

    def test_manifest_is_public_metadata_only(self) -> None:
        document = json.loads(
            (ROOT / "analysis/link_identity/object_linkage.json").read_text(
                encoding="utf-8"
            )
        )
        serialized = json.dumps(document, sort_keys=True)
        self.assertNotIn("SNES_EMU.unpacked.bin", serialized)
        self.assertTrue(
            document["claims"]["private_target_payload_committed"] is False
        )


if __name__ == "__main__":
    unittest.main()
