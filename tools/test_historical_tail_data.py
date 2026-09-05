#!/usr/bin/env python3
"""Public regression guards for the Stage-3I historical tail-data gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import historical_tail_data as gate


class HistoricalTailDataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "historical-tail.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.HistoricalTailError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_exact_source_and_semantic_accounting(self):
        result = self.document["result"]
        self.assertEqual((6, 123140, 1623), (
            result["source_providers"], result["source_bytes"],
            result["source_relocations"],
        ))
        self.assertEqual((9, 30, 1708, 36), (
            result["semantic_groups"], result["semantic_fdes"],
            result["semantic_bytes"], result["semantic_relocations"],
        ))
        self.assertEqual((10, 157), (
            result["absorbed_fixed_sections"], result["absorbed_symbol_aliases"],
        ))

    def test_four_more_windows_close_without_a_completion_claim(self):
        result, claims = self.document["result"], self.document["claims"]
        self.assertEqual(16, result["exact_chunks"])
        self.assertEqual(35, result["mismatching_chunks"])
        self.assertEqual([47, 48, 49], result["exact_chunk_indices"][-3:])
        self.assertLess(result["differing_bytes"], result["prior_differing_bytes"])
        self.assertNotEqual(result["integrated_padded_sha256"], result["target_sha256"])
        self.assertFalse(claims["replacement_elf"])
        self.assertFalse(claims["historical_link_order_complete"])

    def test_semantic_source_contains_no_private_bytes(self):
        semantic = gate.render_semantic_source()
        self.assertNotIn(".incbin", semantic)
        self.assertNotIn("SNES_EMU", semantic)
        self.assertIn("ST3I_S", semantic)
        self.assertTrue(all("payload" not in row for row in self.document["providers"]))

    def test_false_completion_and_metric_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["result"]["semantic_relocations"] -= 1
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["claims"]["unpacked_hash_matched"] = True
        self.reject(changed)


if __name__ == "__main__":
    unittest.main()
