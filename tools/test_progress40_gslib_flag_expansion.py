from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress40GSLIBFlagExpansionTests(unittest.TestCase):
    def test_gslib_flags_use_recursive_make_expansion(self):
        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn(
            "GSLIB_HW_EE_CFLAGS = $(EE_CFLAGS) -Imatching/ee_abi_compat",
            makefile,
        )
        self.assertNotIn(
            "GSLIB_HW_EE_CFLAGS := $(EE_CFLAGS) -Imatching/ee_abi_compat",
            makefile,
        )


if __name__ == "__main__":
    unittest.main()
