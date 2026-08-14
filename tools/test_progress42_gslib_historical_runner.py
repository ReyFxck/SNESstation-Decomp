from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class Progress42GslibHistoricalRunnerTests(unittest.TestCase):
    def test_historical_dma_inline_asm_is_present(self):
        text = (ROOT / "src/ps2/gslib_hw_recovered.c").read_text(encoding="utf-8")
        self.assertIn('__asm__("\\tsw  $0, 0x1000a080");', text)
        self.assertIn('__asm__("\\tli $3, 0x1000a000");', text)
        self.assertIn('__asm__("\\taddiu $29, -4");', text)
        self.assertNotIn("mmio32_store", text)
        self.assertNotIn("mmio32_load", text)

    def test_wait_preserves_historical_expression(self):
        text = (ROOT / "src/ps2/gslib_hw_recovered.c").read_text(encoding="utf-8")
        self.assertIn("while (VRcount_recovered < numvrs)", text)

    def test_runner_forces_fresh_object(self):
        text = (ROOT / "tools/run-gslib-frontier.sh").read_text(encoding="utf-8")
        self.assertIn("rm -f build/matching/gslib_hw/gslib_hw.o", text)
        self.assertIn("match-gslib-hw-listing-strict", text)


if __name__ == "__main__":
    unittest.main()
