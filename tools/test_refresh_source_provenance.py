"""Ownership metadata refresh must never recapture binary/source evidence."""
import json
import tempfile
import unittest
from pathlib import Path

from refresh_source_provenance import refresh_contract


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


if __name__ == "__main__":
    unittest.main()
