from __future__ import annotations

import csv
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class Progress31Tests(unittest.TestCase):
    def test_signed_switch_masks_match_reference_decision_tree(self):
        source = (ROOT / "matching" / "candidates" / "libgcc_unwind_leaves.c").read_text(
            encoding="utf-8"
        )
        self.assertIn("switch (encoding & 7)", source)
        self.assertIn("switch (encoding & 0x70)", source)
        self.assertNotIn("switch (encoding & 7u)", source)
        self.assertNotIn("switch (encoding & 0x70u)", source)

    def test_leb_loop_preserves_int_width_before_unwind_word_or(self):
        source = (ROOT / "matching" / "candidates" / "libgcc_unwind_leaves.c").read_text(
            encoding="utf-8"
        )
        self.assertEqual(source.count("result |= (byte & 0x7f) << shift;"), 2)
        self.assertNotIn("((p29_unwind_word)byte & 0x7fu) << shift", source)

    def test_listing_manifest_excludes_alignment_padding(self):
        path = ROOT / "analysis" / "matching" / "libgcc_unwind_listing.csv"
        with path.open(encoding="utf-8") as stream:
            rows = {row["name"]: row for row in csv.DictReader(stream)}
        self.assertEqual(rows["size_of_encoded_value"]["end"], "0x001a3e2c")
        self.assertEqual(rows["read_uleb128"]["end"], "0x001a3f24")
        self.assertEqual(rows["base_of_encoded_value"]["end"], "0x001a3ee8")
        self.assertEqual(rows["read_sleb128"]["end"], "0x001a3f88")


if __name__ == "__main__":
    unittest.main()
