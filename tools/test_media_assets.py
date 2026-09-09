#!/usr/bin/env python3
"""Public regression guards for the Stage-3M embedded-media gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import media_assets as gate


class MediaAssetsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "media-assets.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.MediaAssetsError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_media_envelopes_are_ordered_and_include_size_words(self):
        rows = gate.MEDIA_SECTIONS
        self.assertEqual(gate.MEDIA_NAMES, tuple(row["name"] for row in rows))
        for row in rows:
            self.assertEqual(row["address"] + row["asset_size"], row["size_word_address"])
            self.assertEqual(row["asset_size"] + 4, row["envelope_size"])
        for left, right in zip(rows, rows[1:]):
            self.assertLessEqual(left["address"] + left["envelope_size"], right["address"])

    def test_twenty_window_corridor_is_exact(self):
        result = self.document["result"]
        self.assertEqual(0, result["windows15_34_differing_bytes"])
        self.assertEqual(20, result["closed_media_windows"])
        self.assertEqual((38, 13), (result["exact_chunks"], result["mismatching_chunks"]))
        self.assertEqual([*range(12, 35), *range(36, 51)], result["exact_chunk_indices"])
        self.assertEqual(1_184_204, result["media_differing_bytes_removed"])

    def test_manifest_contains_no_private_payload(self):
        encoded = json.dumps(self.document)
        self.assertNotIn("SNES_EMU", encoded)
        self.assertNotIn("build/", encoded)
        self.assertFalse(self.document["claims"]["private_target_bytes_stored"])
        self.assertFalse(self.document["claims"]["replacement_elf"])

    def test_false_completion_and_metric_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["claims"]["unpacked_hash_matched"] = True
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["result"]["exact_chunks"] = 51
        self.reject(changed)


if __name__ == "__main__":
    unittest.main()
