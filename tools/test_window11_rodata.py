#!/usr/bin/env python3
"""Public regression guards for the Stage-3O window-11 rodata gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import window11_rodata as gate


class Window11RodataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "window11-rodata.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.Window11RodataError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_sections_are_disjoint_and_stay_in_window_11(self):
        rows = sorted(
            (*gate.SOURCE_SECTIONS, *gate.SEMANTIC_SECTIONS),
            key=lambda row: row["address"],
        )
        self.assertEqual(
            gate.EXPECTED["source_bytes"],
            sum(row["size"] for row in gate.SOURCE_SECTIONS),
        )
        self.assertEqual(
            gate.EXPECTED["source_relocations"],
            sum(row["relocations"] for row in gate.SOURCE_SECTIONS),
        )
        self.assertEqual(
            gate.EXPECTED["semantic_bytes"],
            sum(row["size"] for row in gate.SEMANTIC_SECTIONS),
        )
        self.assertEqual(
            gate.EXPECTED["semantic_relocations"],
            sum(row["relocations"] for row in gate.SEMANTIC_SECTIONS),
        )
        self.assertTrue(all(0x001B0000 <= row["address"] for row in rows))
        self.assertTrue(all(row["address"] + row["size"] <= 0x001C0000 for row in rows))
        for left, right in zip(rows, rows[1:]):
            self.assertLessEqual(left["address"] + left["size"], right["address"])

    def test_window_11_is_exact_without_false_elf_completion(self):
        result = self.document["result"]
        self.assertEqual(0, result["window11_differing_bytes"])
        self.assertEqual((40, 11), (result["exact_chunks"], result["mismatching_chunks"]))
        self.assertEqual([*range(11, 51)], result["exact_chunk_indices"])
        self.assertEqual(
            result["prior_differing_bytes"] - result["differing_bytes"],
            result["differences_removed"],
        )
        self.assertTrue(self.document["claims"]["window_11_exact"])
        self.assertFalse(self.document["claims"]["replacement_elf"])

    def test_private_payload_is_not_frozen_in_public_manifest(self):
        encoded = json.dumps(self.document)
        self.assertNotIn("SNES_EMU", encoded)
        self.assertNotIn("build/", encoded)
        self.assertFalse(self.document["claims"]["private_target_bytes_stored"])
        self.assertTrue(
            self.document["claims"]["private_oracle_used_for_verification_and_relocation_results"]
        )

    def test_claim_and_metric_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["claims"]["window_11_exact"] = False
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["result"]["window11_differing_bytes"] = 1
        self.reject(changed)


if __name__ == "__main__":
    unittest.main()
