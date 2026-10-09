"""Ownership metadata refresh must never recapture binary/source evidence."""
import json
import tempfile
import unittest
from pathlib import Path

from refresh_source_provenance import (
    refresh_contract, refresh_pinned_requesters, refresh_requesters,
)
from libgcc_contracts import read_table
from build_source_tree import render_tsv


class SourceProvenanceTests(unittest.TestCase):
    def test_dependency_hash_refresh_preserves_private_results_and_source_pins(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "manifest.json"
            document = {"source_contract": {"source_sha256": "a" * 64,
                                            "dependency_sha256": "b" * 64},
                        "result": {"target_sha256": "c" * 64, "differing_bytes": 0},
                        "claims": {"exact": True}}
            path.write_text(json.dumps(document))
            expected = {**document["source_contract"], "dependency_sha256": "d" * 64}
            refresh_contract(path, expected, {"dependency_sha256"})
            updated = json.loads(path.read_text())
            self.assertEqual(document["result"], updated["result"])
            self.assertEqual(document["claims"], updated["claims"])
            self.assertEqual("a" * 64, updated["source_contract"]["source_sha256"])
            self.assertEqual(expected, updated["source_contract"])

    def test_source_pin_change_requires_recapture_and_leaves_file_untouched(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "manifest.json"
            original = json.dumps({"source_contract": {"source_sha256": "a" * 64}})
            path.write_text(original)
            with self.assertRaisesRegex(ValueError, "recapture required"):
                refresh_contract(path, {"source_sha256": "b" * 64}, {"dependency_sha256"})
            self.assertEqual(original, path.read_text())

    def test_requester_refresh_preserves_frozen_proof(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "access.tsv"
            fields = ("symbol", "sha256", "requesters")
            original = {"symbol": "DAT_12345678", "sha256": "a" * 64,
                        "requesters": "original.o"}
            path.write_text(render_tsv(fields, [original]))
            expected = [{**original, "requesters": "original.o;recovered.o"}]
            refresh_requesters(path, fields, expected)
            self.assertEqual(expected, read_table(path, fields))

    def test_requester_refresh_rejects_payload_or_roster_drift(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "access.tsv"
            fields = ("symbol", "sha256", "requesters")
            row = {"symbol": "DAT_12345678", "sha256": "a" * 64,
                   "requesters": "original.o"}
            original = render_tsv(fields, [row])
            for expected in ([{**row, "sha256": "b" * 64}], []):
                path.write_text(original)
                with self.assertRaisesRegex(ValueError, "recapture required"):
                    refresh_requesters(path, fields, expected)
                self.assertEqual(original, path.read_text())

    def test_public_requesters_retain_the_captured_private_range_hash(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "named.tsv"
            fields = ("symbol", "extent_hex", "sha256", "requesters")
            row = {"symbol": "state", "extent_hex": "0x20",
                   "sha256": "a" * 64, "requesters": "original.o"}
            path.write_text(render_tsv(fields, [row]))
            public = [{**row, "sha256": "", "requesters": "original.o;recovered.o"}]
            refresh_pinned_requesters(path, fields, public)
            self.assertEqual([{**row, "requesters": public[0]["requesters"]}],
                             read_table(path, fields))

    def test_public_range_refresh_rejects_geometry_hash_or_roster_changes(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "named.tsv"
            fields = ("symbol", "extent_hex", "sha256", "requesters")
            row = {"symbol": "state", "extent_hex": "0x20",
                   "sha256": "a" * 64, "requesters": "original.o"}
            original = render_tsv(fields, [row])
            for public in ([{**row, "sha256": "", "extent_hex": "0x24"}],
                           [{**row, "sha256": "b" * 64}], []):
                path.write_text(original)
                with self.assertRaisesRegex(ValueError, "recapture required"):
                    refresh_pinned_requesters(path, fields, public)
                self.assertEqual(original, path.read_text())


if __name__ == "__main__":
    unittest.main()
