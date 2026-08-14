from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress36Int128ProbeTests(unittest.TestCase):
    def test_scan_shim_only_supplies_missing_size_macro(self):
        text = (ROOT / "include/ee_stage1_compat/stdint.h").read_text(
            encoding="utf-8"
        )
        self.assertIn("#define __SIZEOF_INT128__ 16", text)
        self.assertNotIn("__attribute__((mode(TI)))", text)

    def test_progress28_still_uses_compiler_owned_uint128_name(self):
        text = (ROOT / "src/ps2/progress28_structural_lift_recovered.c").read_text(
            encoding="utf-8"
        )
        self.assertIn("typedef __uint128_t p28_u128;", text)
        self.assertIn("#if defined(__SIZEOF_INT128__)", text)


if __name__ == "__main__":
    unittest.main()
