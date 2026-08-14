from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress35EEFinalFrontendTests(unittest.TestCase):
    def test_scan_ctype_header_exists(self):
        text = (ROOT / "include/ee_stage1_compat/ctype.h").read_text(encoding="utf-8")
        self.assertIn("int isdigit(int c);", text)

    def test_scan_exposes_int128_feature_without_redeclaring_builtin_type(self):
        text = (ROOT / "include/ee_stage1_compat/stdint.h").read_text(encoding="utf-8")
        self.assertIn("#define __SIZEOF_INT128__ 16", text)
        self.assertNotIn("typedef unsigned int __uint128_t", text)
        self.assertNotIn("typedef signed int __int128_t", text)

    def test_gsfont_has_no_c99_for_declarations(self):
        text = (ROOT / "src/ps2/gsfont_recovered.c").read_text(encoding="utf-8")
        self.assertNotIn("for (int ", text)

    def test_ti_bridge_is_not_in_matching_flags(self):
        makefile = (ROOT / "Makefile").read_text(encoding="utf-8")
        ee_cflags = next(
            line for line in makefile.splitlines()
            if line.startswith("EE_CFLAGS ?=")
        )
        self.assertNotIn("ee_stage1_compat", ee_cflags)


if __name__ == "__main__":
    unittest.main()
