import copy
import tempfile
import unittest
from pathlib import Path

import runtime_code_pointers as gate


class RuntimeCodePointerTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.rows = gate.validate()

    def reject(self, rows):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "runtime.tsv"
            path.write_text(gate.render(rows), encoding="utf-8")
            with self.assertRaises(gate.RuntimeCodePointerError):
                gate.validate(path)

    def test_exact_roster(self):
        self.assertEqual(
            {"LAB_0012f8a8","LAB_0012fb78","LAB_00170138","LAB_00170194"},
            {r["symbol"] for r in self.rows},
        )

    def test_no_storage_claim(self):
        self.assertTrue(all("no data storage" in r["claim"] for r in self.rows))

    def test_matching_writers(self):
        self.assertEqual(
            {"CMemory_ApplyROMFixes","CMemory_InitROM"},
            {r["writer_function"] for r in self.rows},
        )
        self.assertTrue(all(r["matching_evidence"] for r in self.rows))

    def test_pointer_slots_are_unique(self):
        self.assertEqual(4, len({r["pointer_address"] for r in self.rows}))

    def test_geometry_drift_rejected(self):
        rows = copy.deepcopy(self.rows)
        rows[0]["store_word"] = "0x00000000"
        self.reject(rows)

    def test_hash_format_required(self):
        rows = copy.deepcopy(self.rows)
        rows[0]["writer_sha256"] = "0" * 63
        self.reject(rows)


if __name__ == "__main__":
    unittest.main()
