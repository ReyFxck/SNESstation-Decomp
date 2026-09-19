#!/usr/bin/env python3
"""Public regression guards for the Stage-3P code-window gate."""
from __future__ import annotations

import copy
import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import code_windows as gate


class CodeWindowsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.args = gate.parse_args(["validate"])
        cls.document = gate.validate(cls.args)

    def reject(self, document):
        with tempfile.TemporaryDirectory() as name:
            manifest = Path(name) / "code-windows.json"
            manifest.write_text(json.dumps(document), encoding="utf-8")
            args = gate.parse_args(["validate", "--manifest", str(manifest)])
            with self.assertRaises(gate.CodeWindowsError):
                gate.validate(args)

    def test_public_gate_needs_no_private_image_or_process(self):
        with patch.object(gate, "probe", side_effect=AssertionError("private probe")), \
             patch("subprocess.run", side_effect=AssertionError("process")):
            self.assertEqual(self.document, gate.validate(self.args))

    def test_all_code_windows_are_exact(self):
        result = self.document["result"]
        self.assertEqual((65536 - 0x114) + 10 * 65536, result["source_bytes"])
        self.assertEqual(36340, result["residual_bytes"])
        self.assertEqual(73192, result["listing_bytes"])
        self.assertEqual(125540, result["direct_object_bytes"])
        self.assertEqual(2, result["direct_candidate_object_inputs"])
        self.assertEqual(595080, result["incbin_payload_bytes"])
        self.assertEqual((51, 0), (result["exact_chunks"], result["mismatching_chunks"]))
        self.assertEqual(list(range(51)), result["exact_chunk_indices"])
        self.assertEqual([], result["mismatching_chunk_indices"])

    def test_private_payload_is_not_frozen(self):
        self.assertFalse(self.document["claims"]["private_target_bytes_stored"])
        self.assertTrue(self.document["claims"]["windows_0_through_6_exact"])
        self.assertTrue(self.document["claims"]["windows_1_through_6_exact"])
        self.assertTrue(self.document["claims"]["windows_0_through_10_exact"])
        self.assertTrue(self.document["claims"]["unpacked_hash_matched"])
        self.assertFalse(self.document["claims"]["replacement_elf"])
        self.assertNotIn(".incbin", json.dumps(self.document))

    def test_claim_and_metric_drift_are_rejected(self):
        changed = copy.deepcopy(self.document)
        changed["claims"]["replacement_elf"] = True
        self.reject(changed)
        changed = copy.deepcopy(self.document)
        changed["result"]["exact_chunks"] = 45
        self.reject(changed)

    def test_evidence_rebuild_preserves_ledger_bytes(self):
        with tempfile.TemporaryDirectory() as name:
            ledgers = [Path(name) / "a.tsv", Path(name) / "b.tsv"]
            original = [b"user-ledger-a\n", b"user-ledger-b\n"]
            for path, data in zip(ledgers, original):
                path.write_bytes(data)

            def rebuild(*_args, **_kwargs):
                for path in ledgers:
                    path.write_bytes(b"generated-local-path-hash\n")
                return SimpleNamespace(returncode=0)

            args = gate.parse_args(["prepare"])
            with patch.object(gate, "EVIDENCE_MANIFESTS", tuple(map(str, ledgers))), \
                 patch.object(gate, "resolve_tool", return_value=Path("/tool/ee-g++")), \
                 patch.object(gate.subprocess, "run", side_effect=rebuild):
                gate.prepare_evidence(args)
            self.assertEqual(original, [path.read_bytes() for path in ledgers])


if __name__ == "__main__":
    unittest.main()
