from __future__ import annotations

import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Progress39GSLIBHWHeadersTests(unittest.TestCase):
    def test_matching_header_is_minimal(self):
        text = (ROOT / "matching/ee_abi_compat/stdint.h").read_text(encoding="utf-8")
        self.assertIn("typedef unsigned int uint32_t;", text)
        self.assertIn("typedef unsigned int uintptr_t;", text)
        self.assertNotIn("_Static_assert", text)
        self.assertNotIn("__SIZEOF_INT128__", text)

    def test_gslib_probe_uses_local_matching_header_only(self):
        text = (ROOT / "Makefile").read_text(encoding="utf-8")
        self.assertIn(
            "GSLIB_HW_EE_CFLAGS = $(EE_CFLAGS) -Imatching/ee_abi_compat",
            text,
        )
        self.assertIn("$(EE_CC) $(GSLIB_HW_EE_CFLAGS) -c $< -o $@", text)

    def test_global_matching_flags_remain_unchanged(self):
        text = (ROOT / "Makefile").read_text(encoding="utf-8")
        ee_cflags = next(
            line for line in text.splitlines()
            if line.startswith("EE_CFLAGS ?=")
        )
        self.assertNotIn("ee_abi_compat", ee_cflags)
        self.assertNotIn("ee_stage1_compat", ee_cflags)


if __name__ == "__main__":
    unittest.main()
