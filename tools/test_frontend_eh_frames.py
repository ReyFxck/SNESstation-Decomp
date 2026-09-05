#!/usr/bin/env python3
"""Public regression guards for the Stage-3H frontend unwind gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import frontend_eh_frames as gate


class FrontendEhFrameTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "frontend-eh.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.FrontendEhError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_semantic_roster_and_whole_image_delta(self):
        result = self.document["result"]
        self.assertEqual((3, 18, 944, 23), (
            result["frontend_eh_groups"], result["frontend_fdes"],
            result["frontend_eh_bytes"], result["frontend_eh_relocations"],
        ))
        self.assertEqual(13, result["exact_chunks"])
        self.assertEqual(38, result["mismatching_chunks"])
        self.assertEqual(14, result["exact_chunk_indices"][2])
        self.assertEqual(
            result["target_initialized_size"],
            result["equal_bytes"] + result["differing_bytes"],
        )

    def test_source_is_semantic_and_contains_no_private_payload_directive(self):
        text = self.args.source.read_text(encoding="utf-8")
        self.assertNotIn(".incbin", text)
        self.assertNotIn("SNES_EMU", text)
        self.assertIn("DW_CFA_offset_extended_sf", text)
        self.assertEqual(18, len(self.document["fdes"]))

    def test_false_completion_and_metric_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["result"]["frontend_eh_relocations"] -= 1
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["claims"]["replacement_elf"] = True
        self.reject(changed)


if __name__ == "__main__":
    unittest.main()
