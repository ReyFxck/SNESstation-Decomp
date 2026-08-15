from __future__ import annotations

import csv
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress41GSLIBSourceShapeTests(unittest.TestCase):
    def test_wait_preserves_historical_parameter_and_unsigned_counter(self):
        text = (ROOT / "src/ps2/gslib_hw_recovered.c").read_text(encoding="utf-8")
        self.assertIn("static unsigned int VRcount_recovered = 0;", text)
        self.assertIn(
            "void WaitForNextVRstart_0019bd50(int numvrs)",
            text,
        )
        self.assertIn("while (VRcount_recovered < numvrs)", text)

    def test_dma_reset_preserves_historical_basic_inline_assembly(self):
        text = (ROOT / "src/ps2/gslib_hw_recovered.c").read_text(encoding="utf-8")
        self.assertIn(r'__asm__("\tsw  $0, 0x1000a080");', text)
        self.assertIn(r'__asm__("\tori $3,$2,1");', text)
        self.assertIn(r'__asm__("\tsw  $3, 0x1000e000");', text)
        self.assertNotIn("GSLIB_HW32(", text)

    def test_manifest_excludes_alignment_padding(self):
        with (ROOT / "analysis/matching/gslib_hw_listing.csv").open(
            newline="", encoding="utf-8"
        ) as f:
            rows = {row["name"]: row for row in csv.DictReader(f)}
        self.assertEqual("0x0019bd74", rows["WaitForNextVRstart"]["end"])
        self.assertEqual("0x0019be1c", rows["DmaReset"]["end"])


if __name__ == "__main__":
    unittest.main()
