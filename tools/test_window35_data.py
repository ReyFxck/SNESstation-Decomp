#!/usr/bin/env python3
"""Public regression guards for the Stage-3N window-35 gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import window35_data as gate


class Window35DataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "window35-data.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.Window35Error):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_source_and_semantic_ranges_form_one_exact_corridor(self):
        rows = sorted(
            [*gate.SOURCE_SECTIONS, *gate.SEMANTIC_GROUPS],
            key=lambda row: row["address"],
        )
        self.assertEqual(0x00335284, rows[0]["address"])
        self.assertEqual(0x0033CE78, rows[-1]["address"] + rows[-1]["size"])
        for left, right in zip(rows, rows[1:]):
            self.assertEqual(left["address"] + left["size"], right["address"])
        self.assertEqual(
            gate.EXPECTED["source_bytes"] + gate.EXPECTED["semantic_bytes"],
            sum(row["size"] for row in rows),
        )

    def test_semantic_fdes_are_explicit_and_counted(self):
        fdes = [entry for group in gate.SEMANTIC_GROUPS for entry in group["fdes"]]
        self.assertEqual(gate.EXPECTED["semantic_fdes"], len(fdes))
        self.assertTrue(
            all(
                op[0] in {"advance_loc4", "def_cfa_offset", "offset_extended_sf"}
                for entry in fdes
                for op in entry["ops"]
            )
        )
        self.assertEqual(45, gate.EXPECTED["semantic_relocations"])

    def test_window_35_is_exact_and_payload_is_not_tracked(self):
        result = self.document["result"]
        self.assertEqual(0, result["window35_differing_bytes"])
        self.assertEqual((39, 12), (result["exact_chunks"], result["mismatching_chunks"]))
        self.assertEqual([*range(12, 51)], result["exact_chunk_indices"])
        self.assertEqual(17_218, result["differences_removed"])
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
