#!/usr/bin/env python3
"""Public regression guards for the Stage-3K tail-metadata gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import tail_metadata as gate


class TailMetadataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "tail-metadata.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.TailMetadataError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_source_and_semantic_accounting(self):
        result = self.document["result"]
        self.assertEqual((34, 9428, 151), (
            result["source_sections"], result["source_bytes"],
            result["source_relocations"],
        ))
        self.assertEqual((5, 944, 14), (
            result["semantic_sections"], result["semantic_bytes"],
            result["spc7110_fdes"],
        ))
        sections = [row["section"] for row in gate.SOURCE_SECTIONS]
        sections += [row["section"] for row in gate.SEMANTIC_SECTIONS]
        self.assertEqual(len(sections), len(set(sections)))

    def test_window_50_is_exact_without_false_completion(self):
        result, claims = self.document["result"], self.document["claims"]
        self.assertEqual((17, 34), (result["exact_chunks"], result["mismatching_chunks"]))
        self.assertEqual(0, result["chunk50_differing_bytes"])
        self.assertIn(50, result["exact_chunk_indices"])
        self.assertLess(result["differing_bytes"], result["prior_differing_bytes"])
        self.assertNotEqual(result["integrated_padded_sha256"], result["target_sha256"])
        self.assertTrue(claims["window_50_exact"])
        self.assertFalse(claims["replacement_elf"])
        self.assertFalse(claims["unpacked_hash_matched"])

    def test_manifest_contains_no_private_payload(self):
        encoded = json.dumps(self.document)
        self.assertNotIn("SNES_EMU", encoded)
        self.assertTrue(all("payload" not in row for row in self.document["source_sections"]))

    def test_false_completion_and_metric_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["result"]["chunk50_differing_bytes"] = 1
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["claims"]["unpacked_hash_matched"] = True
        self.reject(changed)


if __name__ == "__main__":
    unittest.main()
