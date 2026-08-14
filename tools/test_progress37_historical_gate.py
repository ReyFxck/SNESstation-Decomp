from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

class Progress37HistoricalGateTests(unittest.TestCase):
    def test_gate_combines_strict_checks(self):
        makefile = (ROOT / 'Makefile').read_text(encoding='utf-8')
        self.assertIn('historical-ee-gate: check match-libgcc-unwind-listing-strict ee-source-scan-strict', makefile)
        self.assertIn('historical EE gate: OK', makefile)

if __name__ == '__main__':
    unittest.main()
