#!/usr/bin/env python3
"""Public regression guards for the Stage-3J runtime-tail source gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import runtime_tail_data as gate


class RuntimeTailDataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "runtime-tail.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.RuntimeTailError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_source_section_accounting(self):
        result = self.document["result"]
        self.assertEqual((14, 3868, 73, 56), (
            result["source_sections"], result["source_bytes"],
            result["source_relocations"], result["unwind_fdes"],
        ))
        self.assertEqual(len(gate.SECTIONS), len({row["section"] for row in gate.SECTIONS}))

    def test_honest_whole_image_delta(self):
        result, claims = self.document["result"], self.document["claims"]
        self.assertEqual((16, 35), (result["exact_chunks"], result["mismatching_chunks"]))
        self.assertEqual(3173, result["chunk50_differing_bytes"])
        self.assertLess(result["differing_bytes"], result["prior_differing_bytes"])
        self.assertNotEqual(result["integrated_padded_sha256"], result["target_sha256"])
        self.assertFalse(claims["window_50_exact"])
        self.assertFalse(claims["replacement_elf"])

    def test_manifest_contains_no_private_payload(self):
        encoded = json.dumps(self.document)
        self.assertNotIn("SNES_EMU", encoded)
        self.assertTrue(all("payload" not in row for row in self.document["sections"]))

    def test_false_completion_and_metric_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["result"]["source_relocations"] -= 1
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["claims"]["unpacked_hash_matched"] = True
        self.reject(changed)


if __name__ == "__main__":
    unittest.main()
