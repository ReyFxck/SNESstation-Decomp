from __future__ import annotations

import csv
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress38GSLIBHWProbeTests(unittest.TestCase):
    def test_manifest_covers_seven_functions(self):
        path = ROOT / "analysis/matching/gslib_hw_listing.csv"
        with path.open(newline="", encoding="utf-8") as f:
            rows = list(csv.DictReader(f))
        self.assertEqual(7, len(rows))
        self.assertEqual("0x0019bd38", rows[0]["address"])
        self.assertEqual("0x0019be70", rows[-1]["end"])

    def test_makefile_has_local_probe(self):
        text = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn("match-gslib-hw-listing:", text)
        self.assertIn("--base-address 0x0019bd38", text)
        self.assertIn("--end-address 0x0019be70", text)

    def test_probe_compiles_recovered_source_directly(self):
        text = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn("GSLIB_HW_SOURCE := src/ps2/gslib_hw_recovered.c", text)


if __name__ == "__main__":
    unittest.main()
